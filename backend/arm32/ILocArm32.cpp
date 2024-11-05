/**
 * @file ILocArm32.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 指令序列管理
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <cstdio>
#include <sstream>
#include <string>

#include "ILocArm32.h"

#include "Function.h"
#include "PlatformArm32.h"
#include "SymbolTable.h"
#include "Value.h"

ArmInst::ArmInst(std::string op, std::string rs, std::string s1, std::string s2, std::string extra)
    : opcode(op), result(rs), arg1(s1), arg2(s2), addition(extra), dead(false)
{}


/*
    指令内容替换
*/
void ArmInst::replace(std::string op, std::string rs, std::string s1, std::string s2, std::string extra)
{
    opcode = op;
    result = rs;
    arg1 = s1;
    arg2 = s2;
    addition = extra;

#if 0
    // 空操作，则设置为dead
    if (op == "") {
        dead = true;
    }
#endif
}

/*
    设置为无效指令
*/
void ArmInst::setDead()
{
    dead = true;
}

/*
    输出函数
*/
std::string ArmInst::outPut()
{
    // 无用代码，什么都不输出
    if (dead) {
        return "";
    }

    // 占位指令,可能需要输出一个空操作，看是否支持 FIXME
    if (opcode.empty()) {
        return "";
    }

    std::string ret = opcode;
	if(ret[0] == '.'){
        return ret+":";
    }
    // 结果输出
    if (!result.empty()) {
        ret += " " + result;
    }

    // 第一元参数输出
    if (!arg1.empty()) {
        ret += "," + arg1;
    }

    // 第二元参数输出
    if (!arg2.empty()) {
        ret += "," + arg2;
    }

    // 其他附加信息输出
    if (!addition.empty()) {
        ret += "," + addition;
    }

    return ret;
}

#define emit(...) code.push_back(new ArmInst(__VA_ARGS__))

/**
 * 数字变字符串，若flag为真，则变为立即数寻址（加#）
 */
std::string ILocArm32::toStr(int num, bool flag)
{
    std::string ret;

    if (flag) {
        ret = "#";
    }

    std::stringstream ss;
    ss << num;
    ret += ss.str();

    return ret;
}

/*
    产生标签
*/
void ILocArm32::label(std::string name)
{
    // .L1:
    emit("."+name, ":");
}

/// @brief 0个源操作数指令
/// @param op 操作码
/// @param rs 操作数
void ILocArm32::inst(std::string op, std::string rs)
{
    emit(op, rs);
}

/// @brief 一个操作数指令
/// @param op 操作码
/// @param rs 操作数
/// @param arg1 源操作数
void ILocArm32::inst(std::string op, std::string rs, std::string arg1)
{
    emit(op, rs, arg1);
}

/// @brief 一个操作数指令
/// @param op 操作码
/// @param rs 操作数
/// @param arg1 源操作数
/// @param arg2 源操作数
void ILocArm32::inst(std::string op, std::string rs, std::string arg1, std::string arg2)
{
    emit(op, rs, arg1, arg2);
}

/*
    加载立即数 ldr r0,=#100
*/
void ILocArm32::load_imm(int rs_reg_no, int num)
{
    std::string rsReg = PlatformArm32::regName[rs_reg_no];

    if (PlatformArm32::constExpr(num)) {
        // mov r8,#12
        emit("mov", rsReg, toStr(num));
    } else {
        // ldr r8,=0xfff0
        load_label(rs_reg_no, toStr(num, false));
    }
}

/// @brief 加载符号值 ldr r0,=g ldr r0,=.L1
/// @param rs_reg_no 结果寄存器编号
/// @param name Label名字
void ILocArm32::load_label(int rs_reg_no, std::string name)
{
    std::string rsReg = PlatformArm32::regName[rs_reg_no];

    // ldr r8,=.L1
    emit("ldr", rsReg, "=" + name);
}

/// @brief 基址寻址 ldr r0,[fp,#100]
/// @param rsReg 结果寄存器
/// @param base_reg_no 基址寄存器
/// @param disp 偏移
void ILocArm32::load_base(int rs_reg_no, int base_reg_no, int disp)
{
    std::string rsReg = PlatformArm32::regName[rs_reg_no];
    std::string base = PlatformArm32::regName[base_reg_no];

    if (PlatformArm32::isDisp(disp)) {
        // 有效的偏移常量
        if (disp) {
            // [fp,#-16] [fp]
            base += "," + toStr(disp);
        }
    } else {
        // ldr r8,=-4096
        load_imm(rs_reg_no, disp);

        // fp,r8
        base += "," + rsReg;
    }

    // 内存寻址
    base = "[" + base + "]";

    // ldr r8,[fp,#-16]
    // ldr r8,[fp,r8]
    emit("ldr", rsReg, base);
}

/// @brief 基址寻址 str r0,[fp,#100]
/// @param srcReg 源寄存器
/// @param base_reg_no 基址寄存器
/// @param disp 偏移
/// @param tmp_reg_no 可能需要临时寄存器编号
void ILocArm32::store_base(int src_reg_no, int base_reg_no, int disp, int tmp_reg_no)
{
    std::string base = PlatformArm32::regName[base_reg_no];

    if (PlatformArm32::isDisp(disp)) {
        // 有效的偏移常量

        // 若disp为0，则直接采用基址，否则采用基址+偏移
        // [fp,#-16] [fp]
        if (disp) {
            base += "," + toStr(disp);
        }
    } else {
        // 先把立即数赋值给指定的寄存器tmpReg，然后采用基址+寄存器的方式进行

        // ldr r9,=-4096
        load_imm(tmp_reg_no, disp);

        // fp,r9
        base += "," + PlatformArm32::regName[tmp_reg_no];
    }

    // 内存间接寻址
    base = "[" + base + "]";

    // str r8,[fp,#-16]
    // str r8,[fp,r9]
    emit("str", PlatformArm32::regName[src_reg_no], base);
}

/// @brief 寄存器Mov操作
/// @param rs_reg_no 结果寄存器
/// @param src_reg_no 源寄存器
void ILocArm32::mov_reg(int rs_reg_no, int src_reg_no)
{
    emit("mov", PlatformArm32::regName[rs_reg_no], PlatformArm32::regName[src_reg_no]);
}

/// @brief 加载变量到寄存器，保证将变量放到reg中
/// @param rs_reg_no 结果寄存器
/// @param var 源操作数
void ILocArm32::load_var(int rs_reg_no, Value * var)
{
    if (!var) {
        // 无效变量
        return;
    }

    std::string rsReg = PlatformArm32::regName[rs_reg_no];

    if (var->isConst()) {
        // 常量

        // 目前只考虑整数类型 100
        // ldr r8,#100
        load_imm(rs_reg_no, var->intVal);
    } else if(var->_global){
        emit(rsReg, "=" + var->name);
    } else {

        // 不是常量

        if (var->regId != -1) {

            // 寄存器变量

            // 获取寄存器名字
            std::string regName = PlatformArm32::regName[var->regId];

            if (regName != rsReg) {

                // mov r8,r2 | 这里有优化空间——消除r8
                emit("mov", rsReg, regName);
            }
        } else {

            // 目前只考虑局部变量，不考虑数组等
            int off = getAdjustOffset(var);

            // 对于栈内分配的局部数组，可直接在栈指针上进行移动与运算
            // 但对于形参，其保存的是调用函数栈的数组的地址，需要读取出来

            // ldr r8,[sp,#16]
            load_base(rs_reg_no, var->baseRegNo, off);
        }
    }
}

/// @brief 全局变量的加载
/// @param rs_reg_no 结果寄存器
/// @param var 变量
void ILocArm32::load_global(int rs_reg_no, Value * var){
	std::string rsReg = PlatformArm32::regName[rs_reg_no];
    // ldr r3,=b
    emit("ldr", rsReg, "="+var->name);
}
/// @brief 加载变量地址到寄存器
/// @param rs_reg_no
/// @param var
void ILocArm32::lea_var(int rs_reg_no, Value * var)
{
    // 被加载的变量肯定不是常量！
    // 被加载的变量肯定不是寄存器变量！

    // 目前只考虑局部变量

    // 栈帧偏移
    int off = getAdjustOffset(var);

    // lea r8, [fp,#-16]
    leaStack(rs_reg_no, var->baseRegNo, off);
}

/// @brief 保存寄存器到变量，保证将计算结果（r8）保存到变量
/// @param src_reg_no 源寄存器
/// @param var  变量
/// @param tmp_reg_no 第三方寄存器
void ILocArm32::store_var(int src_reg_no, Value * var, int tmp_reg_no)
{
    // 被保存目标变量肯定不是常量

    std::string srcReg = PlatformArm32::regName[src_reg_no];
    std::string tmpReg = PlatformArm32::regName[tmp_reg_no];

    // -1表示非寄存器，其他表示寄存器的索引值
    int id = var->regId;
	if(var->_global){
        emit("str", PlatformArm32::regName[REG_ALLOC_SIMPLE_DST_REG_NO], "="+var->name );
    } else {
        if (id != -1) {
			// 寄存器变量

			std::string regName = PlatformArm32::regName[id];

			if (src_reg_no != id) {

				// mov r2,r8 | 这里有优化空间——消除r8
				emit("mov", PlatformArm32::regName[id], PlatformArm32::regName[src_reg_no]);
			}

		} else {
			// 所有定义的变量,不可能是数组！

			// 对于全局变量，首先把全局变量的左值保存到寄存器中，后间接寻址
			// 对于局部变量，则直接从栈基址+偏移寻址

			// 目前只考虑局部变量

			// 栈帧偏移
			int32_t off = getAdjustOffset(var);

			// 基址寄存器名字
			tmpReg = PlatformArm32::regName[var->baseRegNo];

			// str r8,[r9]
			// str r8, [fp, # - 16]
			store_base(src_reg_no, var->baseRegNo, off, tmp_reg_no);
		}
    }
}

/// @brief 加载栈内变量地址
/// @param rsReg 结果寄存器号
/// @param base_reg_no 基址寄存器
/// @param off 偏移
void ILocArm32::leaStack(int rs_reg_no, int base_reg_no, int off)
{
    std::string rs_reg_name = PlatformArm32::regName[rs_reg_no];
    std::string base_reg_name = PlatformArm32::regName[base_reg_no];

    if (PlatformArm32::constExpr(off))
        // add r8,fp,#-16
        emit("add", rs_reg_name, base_reg_name, toStr(off));
    else {
        // ldr r8,=-257
        load_imm(rs_reg_no, off);

        // add r8,fp,r8
        emit("add", rs_reg_name, base_reg_name, rs_reg_name);
    }
}

/// @brief 函数内栈内空间分配（局部变量、形参变量、函数参数传值，或不能寄存器分配的临时变量等）
/// @param func 函数
/// @param tmp_reg_No
void ILocArm32::allocStack(Function * func, int tmp_reg_no)
{
    // 超过四个的函数调用参数个数，多余4个，则需要栈传值
    int funcCallArgCnt = func->getMaxFuncCallArgCnt() - 4;
    if (funcCallArgCnt < 0) {
        funcCallArgCnt = 0;
    }

    // 计算栈帧大小
    int off = func->getMaxDep();

    off += funcCallArgCnt * 4;

    // 不需要在栈内额外分配空间，则什么都不做
    if (0 == off)
        return;

    if (PlatformArm32::constExpr(off)) {
        // sub sp,sp,#16
        emit("sub", "sp", "sp", toStr(off));
    } else {
        // ldr r8,=257
        load_imm(tmp_reg_no, off);

        // sub sp,sp,r8
        emit("sub", "sp", "sp", PlatformArm32::regName[tmp_reg_no]);
    }

    // 函数调用通过栈传递的基址寄存器设置
    inst("add", PlatformArm32::regName[REG_ALLOC_SIMPLE_FP_REG_NO], "sp", toStr(funcCallArgCnt * 4));
}

/// @brief 调用函数fun
/// @param fun
void ILocArm32::call_fun(std::string name)
{
    // 函数返回值在r0,不需要保护
    emit("bl", name);
}

/// @brief NOP操作
void ILocArm32::nop()
{
    // FIXME 无操作符，要确认是否用nop指令
    emit("");
}

/// @brief 跳转指令
/// @param label 跳转指令
/// @param type 跳转类型
void ILocArm32::jump(Label * label,jump_type type){
	switch (type)
    {
    case jump_type::B:
        emit("b ", label->GetLabelName());
        break;
    case jump_type::BEQ:
        emit("beq ", label->GetLabelName());
        break;
	case jump_type::BNE:
        emit("bne ", label->GetLabelName());
        break;
	case jump_type::BLT:
        emit("blt ", label->GetLabelName());
        break;
	case jump_type::BGT:
        emit("bgt ", label->GetLabelName());
        break;
	case jump_type::BLE:
        emit("ble ", label->GetLabelName());
        break;
	case jump_type::BGE:
        emit("bge ", label->GetLabelName());
        break;
	case jump_type::BL:
        emit("bl ", label->GetLabelName());
        break;
	case jump_type::BX:
		emit("bx ", label->GetLabelName());
        break;
    default:
        emit("b ", label->GetLabelName());
        break;
    }
}

/// @brief 正常跳转指令 cmp r1,r2 | cmp r1,#10
/// @param name Label名字
void ILocArm32::cmp(std::string src1_name, std::string src2_name)
{ 
 	emit("cmp", src1_name, src2_name);
}


/// @brief 构造函数
/// @param symtab 符号表
ILocArm32::ILocArm32(SymbolTable * symtab)
{
    this->symtab = symtab;
}

/// @brief 析构函数
ILocArm32::~ILocArm32()
{
    std::list<ArmInst *>::iterator pIter;

    for (pIter = code.begin(); pIter != code.end(); ++pIter) {
        delete (*pIter);
    }
}

/// @brief 获取局部变量的偏移
/// @param var 变量
/// @return 栈内偏移
int ILocArm32::getAdjustOffset(Value * var)
{
    // 目前暂不支持全局变量，假定全部是局部变量
    if (var->isConst()) {
        // 字面量常量不会保存到栈内，返回-1
        return -1;
    }

    // 获取栈内偏移
    return var->getOffset();
}

/// @brief 删除无用的Label指令
void ILocArm32::deleteUsedLabel()
{
    std::list<ArmInst *> labelInsts;
    for (ArmInst * arm: code) {
        if ((!arm->dead) && (arm->opcode[0] == '.') && (arm->result == ":")) {
            labelInsts.push_back(arm);
        }
    }

    for (ArmInst * labelArm: labelInsts) {
        bool labelUsed = false;

        for (ArmInst * arm: code) {
            // TODO 转移语句的指令标识符根据定义修改判断
            if ((!arm->dead) && (arm->opcode[0] == 'b') && (arm->result == labelArm->opcode)) {
                labelUsed = true;
                break;
            }
        }

        if (!labelUsed) {
            labelArm->setDead();
        }
    }
}

/// @brief 输出汇编
/// @param file 输出的文件指针
/// @param outputEmpty 是否输出空语句
void ILocArm32::outPut(FILE * file, bool outputEmpty)
{
    for (auto arm: code) {
		if(arm->opcode[0] == '.') arm->dead = false;
        std::string s = arm->outPut();
        if (!s.empty()) {
            if (s[0] == '.') {
				s.erase(0, 1);
                // Label符号
                fprintf(file, "%s\n", s.c_str());
            } else {
                fprintf(file, "\t%s\n", s.c_str());
            }
        } else if ((outputEmpty)) {
            fprintf(file, "\n");
        }
    }
}



/// @brief 获取当前的代码序列
/// @return 代码序列
std::list<ArmInst *> & ILocArm32::getCode()
{
    return code;
}
