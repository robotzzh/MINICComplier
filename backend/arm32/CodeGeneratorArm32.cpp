/**
 * @file CodeGeneratorArm32.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief ARM32的后端处理源文件
 * @version 0.1
 * @date 2023-09-26
 * @copyright Copyright (c) 2023
 *
 */
#include <cstdint>
#include <cstdio>
#include <vector>

#include "CodeGeneratorArm32.h"
#include "ILocArm32.h"
#include "IRInst.h"
#include "InstSelectorArm32.h"
#include "PlatformArm32.h"
#include "SymbolTable.h"
#include "Value.h"

/// @brief 构造函数
/// @param tab 符号表
CodeGeneratorArm32::CodeGeneratorArm32(SymbolTable & tab) : CodeGeneratorAsm(tab)
{
    for (int k = 0; k < PlatformArm32::maxRegNum; k++) {
        RegVal[k] = new IntRegValue(PlatformArm32::regName[k], k);
    }
}

/// @brief 析构函数
CodeGeneratorArm32::~CodeGeneratorArm32()
{
    for (int k = 0; k < PlatformArm32::maxRegNum; k++) {
        delete RegVal[k];
        RegVal[k] = nullptr;
    }
}

/// @brief 产生汇编头部分
void CodeGeneratorArm32::genHeader()
{
    fprintf(fp, "%s\n", ".arch armv7ve");
    fprintf(fp, "%s\n", ".arm");
    fprintf(fp, "%s\n", ".fpu vfpv4");

    // ARM32的32位立即数mov操作宏，需要时请打开
    fprintf(fp, "%s\n", R"(
.macro mov32, cond, reg, val
    movw\cond \reg, #:lower16:\val
    movt\cond \reg, #:upper16:\val
.endm
)");
}

/// @brief 全局变量Section，主要包含初始化的和未初始化过的
void CodeGeneratorArm32::genDataSection()
{
    //std::vector<IRInst *> insts = symtab->global_inst.getInst();
    // 可直接操作文件指针fp进行写操作
    std::string GStr;
    for (auto inst: symtab.global_inst.getInsts()) {
        GLOBAL_DECLARE_Inst * ginst = (GLOBAL_DECLARE_Inst *) inst;
		if(ginst->B != BasicType::TYPE_ARRAY_i32){
			Value * Val = ginst->getSrc1();
			GStr = Val->name + ":\n\t.long " + std::to_string(ginst->value) +"\n";
			fprintf(fp, "%s\n", GStr.c_str());
		}else{
			int temp = 1;
            for (auto t: ginst->dims)
                temp = temp * t;
			if(ginst->array_values.empty()){
            
                Value * Val = ginst->getSrc1();
                GStr = Val->name + ":\n\t.zero " + std::to_string(temp) + "\n";
                fprintf(fp, "%s\n", GStr.c_str());
            }else{
				 Value * Val = ginst->getSrc1();
                 GStr = Val->name + ":\n\t";
                 for (auto value : ginst->array_values){
					 fprintf(fp, ".long %d\n", value);
                     temp--;
					 if(temp < 1)
                         break;
                 }
                 for (int i = 0;i< temp % ginst->dims[ginst->dims.size()-1];i++){
					 fprintf(fp, ".long 0\n");
                     temp--;
                 }
				 if(temp != 0){
                     int temp1 = 1;
                     for (int i = ginst->dims.size() - 1; i > 0; i--) {
						 temp1 = temp1 * ginst->dims[i];
						 fprintf(fp, ".zero %d\n", temp1);
                         temp -= temp1;
                     }
                 }
            }
        }
    }

    // 目前支持全局变量和静态变量
}

/// @brief 针对函数进行汇编指令生成，放到.text代码段中
/// @param func 要处理的函数
void CodeGeneratorArm32::genCodeSection(Function * func)
{
    // 寄存器分配以及栈内局部变量的站内地址重新分配
    registerAllocation(func);

    // 获取函数的指令列表
    std::vector<IRInst *> & IrInsts = func->getInterCode().getInsts();

    // ILOC代码序列
    ILocArm32 iloc(&symtab);

    // 指令选择生成汇编指令 
    InstSelectorArm32 instSelector(IrInsts, iloc, func);
    instSelector.run();

    // 删除无用的Label指令
    iloc.deleteUsedLabel();

    // ILOC代码输出为汇编代码
    string name = func->getName();
    string asmName = name[0] == '@' ? name.substr(1) : name;
    fprintf(fp, ".global %s\n", asmName.c_str());
    fprintf(fp, ".type %s, %%function\n", asmName.c_str());
    fprintf(fp, "%s:\n", asmName.c_str());
    iloc.outPut(fp);
}

/// @brief 寄存器分配
/// @param func 函数指针
void CodeGeneratorArm32::registerAllocation(Function * func)
{
    // 内置函数不需要处理
    if (func->isBuiltin()) {
        return;
    }

#ifndef FUNC_PARAM_STACK_ONE
    // 对于形参的前四个参数默认分配四个局部变量，用于保存前四个寄存器的值
    // 前四个之后的形参直接用实参的地址进行访问。
    // 如果定义宏FUNC_PARAM_STACK_ONE，则前四个参数后的形参也分配局部变量，
    // 需要额外的赋值指令，把实参的值赋值给形参对应的局部变量
    adjustFormalParamStack(func);
#endif

    // 局部变量都保存在栈内，同时调整函数内栈内大小
    stackAlloc(func);

    // 目前采用最简单的寄存器分配策略，2个寄存器，最多三个寄存器使用，除了函数调用

    // R0,R1,R2,R3这些寄存器不需要保护
    // R10(10), fp(11), lx(14)需要保护
    auto & protectedRegNo = func->getProtectedReg();
    protectedRegNo.push_back(REG_ALLOC_SIMPLE_TMP_REG_NO);
    protectedRegNo.push_back(REG_ALLOC_SIMPLE_FP_REG_NO);
    if (func->getExistFuncCall()) {
        protectedRegNo.push_back(REG_ALLOC_SIMPLE_LX_REG_NO);
    }

    // 函数形参要求前四个寄存器分配，后面的参数采用栈传递
    // 针对后面的栈传递参数，一种处理方式为增加赋值操作，内存赋值给形参，形参再次分配空间
    // 另外一种处理方式，形参直接用实参的地址，也就是变为内存变量，具体见如下的函数处理
    adjustFormalParamInsts(func);

    // 调整函数调用指令，主要是前四个寄存器传值，后面用栈传递
    adjustFuncCallInsts(func);
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorArm32::adjustFormalParamStack(Function * func)
{
    // 函数形参的前四个参数采用的是寄存器传值，需要把值拷贝到本地栈形参变量上
    auto & params = func->getParams();

    // 根据C语言的约定，除前4个外的实参进行值传递，逆序入栈
    for (int k = 4; k < (int) params.size(); k++) {

        // 形参名字无效，则忽略
        if (params[k].name.empty()) {
            continue;
        }

        Value * val = params[k].val;

        // 栈内分配，但是寄存器偏移先不填写，待定
        val->baseRegNo = REG_ALLOC_SIMPLE_FP_REG_NO;
    }
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorArm32::adjustFormalParamInsts(Function * func)
{
    // 函数形参要求前四个寄存器分配，后面的参数采用栈传递
    // 针对后面的栈传递参数，一种处理方式为增加赋值操作，内存赋值给形参，形参再次分配空间
    // 另外一种处理方式，形参直接用实参的地址，也就是变为内存变量
    std::vector<IRInst *> newInsts;

    // 函数形参的前四个参数采用的是寄存器传值，需要把值拷贝到本地栈形参变量上
    auto & params = func->getParams();

    // 因为假定用的R1与R2进行运算，而寄存器传值也用R0到R3
    // 如果先处理了后面的参数，R1与R2的值会被更改。
    for (int k = 0; k < (int) params.size() && k <= 3; k++) {

        // 形参名字无效，则忽略
        if (params[k].name.empty()) {
            continue;
        }

        Value * val = params[k].val;

        // 产生赋值指令,R#n寄存器赋值到对应的变量上，放到放到Entry指令的后面
        // 注意R#n寄存器需要事先分配几个Value
        newInsts.push_back(new AssignIRInst(val, RegVal[k]));
    }

    // 形参的前四个采用fp+偏移寻址，后面的sp+偏移寻址实参空间
    // 根据C语言的约定，除前4个外的实参进行值传递，逆序入栈
    int fp_esp = func->getMaxDep() + (int) func->getProtectedReg().size() * 4;
    for (int k = 4; k < (int) params.size(); k++) {

        Value * val = params[k].val;

        // 目前假定变量大小都是4字节

        // 形参名字无效，则忽略
        if (params[k].name.empty()) {

            // 增加4字节
            fp_esp += 4;

            continue;
        }

#ifdef FUNC_PARAM_STACK_ONE
        // 新建一个内存变量，用于栈传值到内存中
        Value * newVal = new MemValue(BasicType::TYPE_INT);
        newVal->baseRegNo = REG_ALLOC_SIMPLE_FP_REG_NO;
        newVal->setOffset(fp_esp);

        // 新建一个赋值指令
        newInsts.push_back(new AssignIRInst(val, newVal));
#else
        val->setOffset(fp_esp);
#endif

        // 增加4字节
        fp_esp += 4;
    }

    // 当前函数的指令列表
    auto & insts = func->getInterCode().getInsts();

    // Entry指令的下一条指令
    auto pEntryAfterIter = insts.begin() + 1;

    // 逐个插入指令到entry的后面
    insts.insert(pEntryAfterIter, newInsts.begin(), newInsts.end());
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorArm32::adjustFuncCallInsts(Function * func)
{
    std::vector<IRInst *> newInsts;

    // 当前函数的指令列表
    auto & insts = func->getInterCode().getInsts();

    // 函数返回值用R0寄存器，若函数调用有返回值，则赋值R0到对应寄存器
    for (auto pIter = insts.begin(); pIter != insts.end(); pIter++) {

        // 检查是否是函数调用指令，并且含有返回值
        FuncCallIRInst * callInst = dynamic_cast<FuncCallIRInst *>(*pIter);
        if (callInst) {

            // 实参前四个要寄存器传值，其它参数通过栈传递

            IRInst * assignInst;

            std::vector<Value *> & args = callInst->getSrc();

            // 前四个参数的后面采用栈传递
            int esp = 0;
            for (int k = 4; k < (int) args.size(); k++) {

                // 新建一个内存变量，用于栈传值到形参变量中
                Value * newVal = new MemValue(BasicType::TYPE_INT);
                newVal->baseRegNo = REG_ALLOC_SIMPLE_SP_REG_NO;
                newVal->setOffset(esp);
                esp += 4;

                assignInst = new AssignIRInst(newVal, args[k]);

                // 函数调用指令前插入后，pIter仍指向函数调用指令
                pIter = insts.insert(pIter, assignInst);
                pIter++;
            }

            for (int k = 0; k < (int) args.size() && k < 4; k++) {

                // 寄存器传值到形参变量中
                assignInst = new AssignIRInst(RegVal[k], args[k]);

                // 函数调用指令前插入后，pIter仍指向函数调用指令
                pIter = insts.insert(pIter, assignInst);
                pIter++;
            }

            // 这里应该区分临时变量或者其它变量，临时变量可变更为寄存器变量，其它需要复制给变量
            // 目前简化处理，都直接赋值给变量

            // 赋值指令
            Value * resultVal = callInst->getDst();
            if (resultVal) {

                if (resultVal->isTemp() && resultVal->regId == -1 && resultVal->baseRegNo == -1) {

                    // 该临时变量既没有寄存器分配，也没有设置基址寄存器，直接设置为寄存器即可
                    resultVal->regId = 0;
                } else {

                    // 新建一个赋值操作
                    assignInst = new AssignIRInst(resultVal, RegVal[0]);

                    // 函数调用指令的下一个指令的前面插入指令，因为有Exit指令，+1肯定有效
                    pIter = insts.insert(pIter + 1, assignInst);
                }
            }
        }
    }
}

/// @brief 栈空间分配
/// @param func 要处理的函数
void CodeGeneratorArm32::stackAlloc(Function * func)
{
    // 遍历函数内的所有指令，查找没有寄存器分配的变量，然后进行栈内空间分配

    int32_t sp_esp = 0;

    // 获取函数变量列表
    std::vector<Value *> & vars = func->getVarValues();

    for (auto var: vars) {

        // 对于简单类型的寄存器分配策略，临时变量都会用寄存器，这里需要忽略
        // 而对于图着色等，临时变量可能会变更内存分配，这时应该调整类型
        if (var->isTemp())
            continue;

        if ((var->regId == -1) && (var->baseRegNo == -1)) {

            // 该变量没有分配寄存器

            int32_t size = var->getSize();

            // 32位ARM平台按照4字节的大小整数倍分配局部变量
            size += (4 - size % 4) % 4;

            // 局部变量偏移
            var->setOffset(sp_esp);

            // 累计当前作用域大小
            sp_esp += size;

            // 设置基址寄存器名字
            var->baseRegNo = REG_ALLOC_SIMPLE_FP_REG_NO;
        }
    }

    // 设置函数的最大栈帧深度，在加上实参内存传值的空间
    // 请注意若支持浮点数，则必须保持栈内空间8字节对齐
    func->setMaxDep(sp_esp);
}
