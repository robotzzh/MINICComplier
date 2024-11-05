/**
 * @file InstSelectorArm32.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 指令选择器-ARM32
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <map>
#include <vector>

#include "Function.h"
#include "ILocArm32.h"
#include "IRInst.h"
#include "PlatformArm32.h"

using namespace std;

static int is_r2_used = 0;

/// @brief 指令选择器-ARM32
class InstSelectorArm32 {

    /// @brief 所有的IR指令
    std::vector<IRInst *> & ir;

    /// @brief 指令变换
    ILocArm32 & iloc;

    /// @brief 要处理的函数
    Function * func;

    /// @brief 形参列表
    std::vector<Value *> realArgs;

protected:
    /// @brief 指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate(IRInst * inst);

    /// @brief NOP翻译成ARM32汇编
    /// @param inst IR指令
    void translate_nop(IRInst * inst);

    /// @brief 函数入口指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_entry(IRInst * inst);

    /// @brief 函数出口指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_exit(IRInst * inst);

    /// @brief 赋值指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_assign(IRInst * inst);

    /// @brief Label指令指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_label(IRInst * inst);

    /// @brief goto指令指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_goto(IRInst * inst);

    /// @brief 整数加法指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_add_int32(IRInst * inst);

    /// @brief 整数减法指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_sub_int32(IRInst * inst);

	/// @brief 声明指令翻译成ARM32汇编
    /// @param inst IR指令
	void translate_declare(IRInst * inst);

    /// @brief 二元操作指令翻译成ARM32汇编
    /// @param inst IR指令
    /// @param operator_name 操作码
    /// @param rs_reg_no 结果寄存器号
    /// @param op1_reg_no 源操作数1寄存器号
    /// @param op2_reg_no 源操作数2寄存器号
    void translate_two_operator(IRInst * inst,
                                string operator_name,
                                int rs_reg_no = REG_ALLOC_SIMPLE_DST_REG_NO,
                                int op1_reg_no = REG_ALLOC_SIMPLE_SRC1_REG_NO,
                                int op2_reg_no = REG_ALLOC_SIMPLE_SRC2_REG_NO);

    /// @brief 函数调用指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_call(IRInst * inst);

    /// @brief IR翻译动作函数原型
    typedef void (InstSelectorArm32::*translate_handler)(IRInst *);

    /// @brief IR动作处理函数清单
    map<IRInstOperator, translate_handler> translator_handlers;

	// 在这里新增指令翻译 --------------------------------

	/// @brief 整数乘法法指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_mul_int32(IRInst * inst);

	/// @brief 有符号除法指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_sdiv_int32(IRInst * inst);

	/// @brief 无符号除法指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_udiv_int32(IRInst * inst);

	/// @brief 整数取模指令翻译成arm32汇编
	/// @param inst IR指令
    void translate_mod_int32(IRInst * inst);

    void translate_and_int32(IRInst * inst);

	void translate_orr_int32(IRInst * inst);

	void translate_eor_int32(IRInst * inst);

	void translate_not_int32(IRInst * inst);

	void translate_cmp_int32(IRInst * inst);

	void translate_bne_int32(IRInst * inst);

	void translate_beq_int32(IRInst * inst);

	void translate_useless_ir(IRInst * inst);

	/// @brief 单元操作指令翻译成ARM32汇编
    /// @param inst IR指令
    /// @param operator_name 操作码
    /// @param rs_reg_no 结果寄存器号
    /// @param op1_reg_no 源操作数1寄存器号
    void translate_single_operator(IRInst * inst,
                                string operator_name,
                                int rs_reg_no = REG_ALLOC_SIMPLE_DST_REG_NO,
                                int op1_reg_no = REG_ALLOC_SIMPLE_SRC1_REG_NO);

    /// @brief load指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_load(IRInst * inst);

	/// @brief 取负指令翻译成ARM32汇编
	/// @param inst IR指令
	void translate_neg_int32(IRInst * inst);

	/// @brief 跳转指令翻译成ARM32汇编
	/// @param inst IR指令
    void translate_jump(IRInst * inst);
    // 新增部分结束 -------

public:
    /// @brief 构造函数
    /// @param _irCode IR指令
    /// @param _func 函数
    /// @param _iloc 后端指令
    InstSelectorArm32(std::vector<IRInst *> & _irCode, ILocArm32 & _iloc, Function * _func);

    /// @brief 指令选择
    void run();
};
