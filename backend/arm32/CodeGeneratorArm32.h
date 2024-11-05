/**
 * @file CodeGeneratorArm32.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief ARM32的后端处理头文件
 * @version 0.1
 * @date 2023-09-26
 * @copyright Copyright (c) 2023
 *
 */

#include "CodeGeneratorAsm.h"
#include "PlatformArm32.h"

class CodeGeneratorArm32 : public CodeGeneratorAsm {

public:
    /// @brief 构造函数
    /// @param tab 符号表
    CodeGeneratorArm32(SymbolTable & tab);

    /// @brief 析构函数
    ~CodeGeneratorArm32() override;

protected:
    /// @brief 产生汇编头部分
    void genHeader() override;

    /// @brief 全局变量Section，主要包含初始化的和未初始化过的
    void genDataSection() override;

    /// @brief 针对函数进行汇编指令生成，放到.text代码段中
    /// @param func 要处理的函数
    void genCodeSection(Function * func) override;

    /// @brief 寄存器分配
    /// @param func 要处理的函数
    void registerAllocation(Function * func) override;

    /// @brief 栈空间分配
    /// @param func 要处理的函数
    void stackAlloc(Function * func);

    /// @brief 确保前四个参数之后的参数必须栈内分配
    /// @param func 要处理的函数
    void adjustFormalParamStack(Function * func);

    /// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
    /// @param func 要处理的函数
    void adjustFuncCallInsts(Function * func);

    /// @brief 寄存器分配前对形参指令调整，便于栈内空间分配以及寄存器分配
    /// @param func 要处理的函数
    void adjustFormalParamInsts(Function * func);

private:
    /// @brief 对寄存器R0分配Value，记录位置
    Value * RegVal[PlatformArm32::maxRegNum];
};
