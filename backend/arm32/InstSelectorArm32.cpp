/**
 * @file InstSelectorArm32.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 指令选择器-ARM32
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <cstdio>

#include "Function.h"
#include "ILocArm32.h"
#include "IRInst.h"
#include "InstSelectorArm32.h"
#include "PlatformArm32.h"
#include "Value.h"


/// @brief 反转了
/// @param 跳转指令
/// @return 相反的跳转指令
static jump_type jump_reverse(jump_type a){
	switch (a)
	{
	case jump_type::BEQ:
        return jump_type::BNE;
        break;
	case jump_type::BNE:
        return jump_type::BEQ;
        break;
	case jump_type::BGT:
        return jump_type::BLE;
        break;
	case jump_type::BLE:
        return jump_type::BGT;
        break;
	case jump_type::BGE:
        return jump_type::BLT;
        break;
	case jump_type::BLT:
        return jump_type::BGE;
        break;
	default:
		return jump_type::BNE;
		break;
	}
}

static jump_type global_jump_type = jump_type::B;
/// @brief 构造函数
/// @param _irCode 指令
/// @param _iloc ILoc
/// @param _func 函数
InstSelectorArm32::InstSelectorArm32(vector<IRInst *> & _irCode, ILocArm32 & _iloc, Function * _func)
    : ir(_irCode), iloc(_iloc), func(_func)
{
	/// 生成IR的无效指令
    translator_handlers[IRInstOperator::IRINST_OP_MAX] = &InstSelectorArm32::translate_useless_ir;
	translator_handlers[IRInstOperator::IRINST_OP_JUMP] = &InstSelectorArm32::translate_jump;
    translator_handlers[IRInstOperator::IRINST_OP_ENTRY] = &InstSelectorArm32::translate_entry;
    translator_handlers[IRInstOperator::IRINST_OP_EXIT] = &InstSelectorArm32::translate_exit;

    translator_handlers[IRInstOperator::IRINST_OP_LABEL] = &InstSelectorArm32::translate_label;
    translator_handlers[IRInstOperator::IRINST_OP_GOTO] = &InstSelectorArm32::translate_goto;
    translator_handlers[IRInstOperator::IRINST_OP_DECLARE] = &InstSelectorArm32::translate_declare;
    translator_handlers[IRInstOperator::IRINST_OP_LOAD] = &InstSelectorArm32::translate_load;

    translator_handlers[IRInstOperator::IRINST_OP_ASSIGN] = &InstSelectorArm32::translate_assign;

    translator_handlers[IRInstOperator::IRINST_OP_ADD_I] = &InstSelectorArm32::translate_add_int32;
    translator_handlers[IRInstOperator::IRINST_OP_SUB_I] = &InstSelectorArm32::translate_sub_int32;
	translator_handlers[IRInstOperator::IRINST_OP_NEG_I] = &InstSelectorArm32::translate_neg_int32;
    translator_handlers[IRInstOperator::IRINST_OP_MUL_I] = &InstSelectorArm32::translate_mul_int32;
    translator_handlers[IRInstOperator::IRINST_OP_DIV_I] = &InstSelectorArm32::translate_sdiv_int32;
	translator_handlers[IRInstOperator::IRINST_OP_MOD] = &InstSelectorArm32::translate_mod_int32;
	translator_handlers[IRInstOperator::IRINST_OP_AND] = &InstSelectorArm32::translate_and_int32;
	translator_handlers[IRInstOperator::IRINST_OP_OR] = &InstSelectorArm32::translate_orr_int32;
	translator_handlers[IRInstOperator::IRINST_OP_NOT] = &InstSelectorArm32::translate_not_int32;

	translator_handlers[IRInstOperator::IRINST_OP_ICMP] = &InstSelectorArm32::translate_cmp_int32;

    translator_handlers[IRInstOperator::IRINST_OP_FUNC_CALL] = &InstSelectorArm32::translate_call;
}

/// @brief 指令选择执行
void InstSelectorArm32::run()
{
    for (auto inst: ir) {

        // 逐个指令进行翻译
        if (!inst->isDead()) {
            translate(inst);
        }
    }
}

/// @brief NOP翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_nop(IRInst * inst)
{
    (void) inst;
    iloc.nop();
}

/// @brief Label指令指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_label(IRInst * inst)
{
	LabelIRInst * label = (LabelIRInst *) inst;
    iloc.label(label->getLabelName());
}

/// @brief goto指令指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_goto(IRInst * inst)
{}

/// @brief 函数入口指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_entry(IRInst * inst)
{
    // 查看保护的寄存器
    auto & protectedRegNo = func->getProtectedReg();
    auto & protectedRegStr = func->getProtectedRegStr();

    bool first = true;
    for (auto regno: protectedRegNo) {
        if (first) {
            protectedRegStr = PlatformArm32::regName[regno];
            first = false;
        } else if (!first) {
            protectedRegStr += "," + PlatformArm32::regName[regno];
        }
    }

    if (!protectedRegStr.empty()) {
        iloc.inst("push", "{" + protectedRegStr + "}");
    }

    // 为fun分配栈帧，含局部变量、函数调用值传递的空间等
    iloc.allocStack(func, REG_ALLOC_SIMPLE_TMP_REG_NO);
}

/// @brief 函数出口指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_exit(IRInst * inst)
{
	ExitIRInst * exit = (ExitIRInst *) inst;
    if (!inst->getSrc().empty() && exit->getPattern() != 1) {
        // 存在返回值
        Value * retVal = inst->getSrc1();

        // 赋值给寄存器R0
        iloc.load_var(0, retVal);
    }
	if(exit->getPattern() == 1){
        iloc.inst("mov", "r0", "#" + std::to_string(exit->instantNumber));
    }

    auto & protectedRegStr = func->getProtectedRegStr();

    // 恢复栈空间
    iloc.inst("add",
              PlatformArm32::regName[REG_ALLOC_SIMPLE_FP_REG_NO],
              PlatformArm32::regName[REG_ALLOC_SIMPLE_FP_REG_NO],
              iloc.toStr(func->getMaxDep()));

    iloc.inst("mov", "sp", PlatformArm32::regName[REG_ALLOC_SIMPLE_FP_REG_NO]);

    // 保护寄存器的恢复
    if (!protectedRegStr.empty()) {
        iloc.inst("pop", "{" + protectedRegStr + "}");
    }

    iloc.inst("bx", "lr");
}

/// @brief 无用指令翻译成ARM32汇编
/// @param inst 
void InstSelectorArm32::translate_useless_ir(IRInst * inst){
}
/// @brief 赋值指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_assign(IRInst * inst)
{
    Value * rs = inst->getDst();
    Value * arg1 = inst->getSrc1();

    if (arg1->regId != -1) {
        // 寄存器 => 内存
        // 寄存器 => 寄存器

        // r8 -> rs 可能用到r9
        iloc.store_var(arg1->regId, rs, 9);

    } else if (rs->regId != -1) {
        // 内存变量 => 寄存器

        iloc.load_var(rs->regId, arg1);

    } else {
        // 内存变量 => 内存变量

        // arg1 -> r8
        iloc.load_var(REG_ALLOC_SIMPLE_SRC1_REG_NO, arg1);

        // r8 -> rs 可能用到r9
        iloc.store_var(REG_ALLOC_SIMPLE_SRC1_REG_NO, rs, 9);
    }
}

/// @brief 单元操作指令翻译成ARM32汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数1寄存器号
void InstSelectorArm32::translate_single_operator(IRInst * inst,
                                string operator_name,
                                int rs_reg_no,
                                int op1_reg_no)
{
    Value * rs = inst->getDst();
    Value * arg1 = inst->getSrc1();

    std::string arg1_reg_name;
    int arg1_reg_no = arg1->regId;

    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {
        // arg1 -> r8
        iloc.load_var(op1_reg_no, arg1);
    } else if (arg1_reg_no != op1_reg_no) {
        // 已分配的操作数1的寄存器和操作数2的缺省寄存器一致，这样会使得操作数2的值设置到一个寄存器上
        // 缺省寄存器  2    3
        // 实际寄存器  3    -1   有问题
        // 实际寄存器  3    3    有问题
        // 实际寄存器  3    4    无问题
            op1_reg_no = arg1_reg_no;
    }

    arg1_reg_name = PlatformArm32::regName[op1_reg_no];

    // 看结果变量是否是寄存器，若不是则采用参数指定的寄存器rs_reg_name
    if (rs->regId != -1) {
        rs_reg_no = rs->regId;
    } else if (rs->isTemp()) {
        // 临时变量
        rs->regId = rs_reg_no;
    }

    std::string rs_reg_name = PlatformArm32::regName[rs_reg_no];
	if(operator_name == "neg"){
		iloc.inst("rsb", rs_reg_name, arg1_reg_name,"#0");
	}
	else if(operator_name == "not"){
		if(arg1->intVal == 0)
            iloc.inst("eor", rs_reg_name, "#1", arg1_reg_name);
    } else {
        iloc.inst(operator_name, rs_reg_name, arg1_reg_name);
    }

    // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    if (rs->regId == -1) {
        // r8 -> rs 可能用到r9
		// TODO:未实现
        iloc.store_var(rs_reg_no, rs, op1_reg_no);
    }	

	is_r2_used = 0;
	// 目前将寄存器归零，防止之后使用已经被修改了值的寄存器
    arg1->regId = -1;

}
/// @brief 二元操作指令翻译成ARM32汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数1寄存器号
/// @param op2_reg_no 源操作数2寄存器号
void InstSelectorArm32::translate_two_operator(IRInst * inst,
                                               string operator_name,
                                               int rs_reg_no,
                                               int op1_reg_no,
                                               int op2_reg_no)
{
    Value * rs = inst->getDst();
    Value * arg1 = inst->getSrc1();
    Value * arg2 = inst->getSrc2();

    std::string arg1_reg_name, arg2_reg_name;
    int arg1_reg_no = arg1->regId, arg2_reg_no = arg2->regId;

    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {
        // arg1 -> r8
        iloc.load_var(op1_reg_no, arg1);
    } else if (arg1_reg_no != op1_reg_no) {
        // 已分配的操作数1的寄存器和操作数2的缺省寄存器一致，这样会使得操作数2的值设置到一个寄存器上
        // 缺省寄存器  2    3
        // 实际寄存器  3    -1   有问题
        // 实际寄存器  3    3    有问题
        // 实际寄存器  3    4    无问题
        if ((arg1_reg_no == op2_reg_no) && ((arg2_reg_no == -1) || (arg2_reg_no == op2_reg_no))) {
            iloc.mov_reg(op1_reg_no, arg1_reg_no);
        } else {
            op1_reg_no = arg1_reg_no;
        }
    }

    arg1_reg_name = PlatformArm32::regName[op1_reg_no];

    // 看arg2是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg2_reg_no == -1) {
        // arg1 -> r8
        iloc.load_var(op2_reg_no, arg2);
    } else if (arg2_reg_no != op2_reg_no) {
        // 已分配的操作数2的寄存器和操作数1的缺省寄存器一致，这样会使得操作数2的值设置到一个寄存器上
        // 缺省寄存器  2    3
        // 实际寄存器  -1   2   有问题
        // 实际寄存器  2    2    有问题
        // 实际寄存器  4    2    无问题
        if ((arg2_reg_no == op1_reg_no) && ((arg1_reg_no == -1) || (arg1_reg_no == op1_reg_no))) {
            iloc.mov_reg(op2_reg_no, arg2_reg_no);
        } else {
            op2_reg_no = arg2_reg_no;
        }
    }

    arg2_reg_name = PlatformArm32::regName[op2_reg_no];

    // 看结果变量是否是寄存器，若不是则采用参数指定的寄存器rs_reg_name
    if (rs->regId != -1) {
        rs_reg_no = rs->regId;
    } else if (rs->isTemp()) {
        // 临时变量
        rs->regId = rs_reg_no;
    }
	if(operator_name == "add" || operator_name == "sub") {
        rs->regId = REG_ADD_SUB_NO;
    }
    if(operator_name == "mul" || operator_name == "div"){
		rs->regId = REG_MUL_DIV_NO;
	}

    std::string rs_reg_name = PlatformArm32::regName[rs->regId];

    iloc.inst(operator_name, rs_reg_name, arg1_reg_name, arg2_reg_name);

    // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    if (rs->regId == -1) {
        // r8 -> rs 可能用到r9
        iloc.store_var(rs_reg_no, rs, op2_reg_no);
    }

	is_r2_used = 0;
	// 目前将寄存器归零，防止之后使用已经被修改了值的寄存器
    arg1->regId = -1;
    arg2->regId = -1;
}

/// @brief 整数加法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_add_int32(IRInst * inst)
{
    translate_two_operator(inst, "add");
}

/// @brief 整数减法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_sub_int32(IRInst * inst)
{
    translate_two_operator(inst, "sub");
}

/// @brief 取负指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_neg_int32(IRInst * inst)
{
    translate_single_operator(inst, "neg");
}

/// @brief 整数乘法法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_mul_int32(IRInst * inst)
{
	translate_two_operator(inst, "mul");
}

/// @brief 有符号除法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_sdiv_int32(IRInst * inst)
{
	translate_two_operator(inst, "sdiv");
}

/// @brief 有符号除法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_udiv_int32(IRInst * inst)
{
	translate_two_operator(inst, "udiv");
}

/// @brief AND指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_and_int32(IRInst * inst)
{
	translate_two_operator(inst, "and");
}

/// @brief AND指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_orr_int32(IRInst * inst)
{
	translate_two_operator(inst, "orr");
}

/// @brief EOR指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_eor_int32(IRInst * inst)
{
	translate_two_operator(inst, "eor");
}

/// @brief MVN指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_not_int32(IRInst * inst)
{
    translate_single_operator(inst, "not");
}

/// @brief 整数取模指令翻译成arm32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_mod_int32(IRInst * inst){

}

/// @brief 函数调用指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_call(IRInst * inst)
{
    FuncCallIRInst * callInst = dynamic_cast<FuncCallIRInst *>(inst);
    iloc.call_fun(callInst->name);
}

void InstSelectorArm32::translate_declare(IRInst * inst){

}

/// @brief load指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_load(IRInst * inst)
{
    Value * dst = inst->dstValue;

	Value * src = inst->srcValues[0];
	if(src->_global){
        iloc.load_global(REG_ALLOC_SIMPLE_LOAD, src);
        dst->realVal = REG_ALLOC_SIMPLE_LOAD;
    }
    if(dst -> regId == -1 && !is_r2_used && !src->_global){
		dst->offset = src->offset;
		dst->baseRegNo = src->baseRegNo;
        iloc.load_base(REG_ALLOC_SIMPLE_LOAD, dst->baseRegNo, dst->offset);
		iloc.mov_reg(REG_ALLOC_SIMPLE_SRC1_REG_NO, REG_ALLOC_SIMPLE_LOAD);
		dst->regId = REG_ALLOC_SIMPLE_SRC1_REG_NO;
        is_r2_used = 1;
    }
	if(dst -> regId == -1 && is_r2_used && !src->_global){
		dst->offset = src->offset;
		dst->baseRegNo = src->baseRegNo;
        iloc.load_base(REG_ALLOC_SIMPLE_LOAD, dst->baseRegNo, dst->offset);
		iloc.mov_reg(REG_ALLOC_SIMPLE_SRC2_REG_NO, REG_ALLOC_SIMPLE_LOAD);
		dst->regId = REG_ALLOC_SIMPLE_SRC2_REG_NO;
        is_r2_used = 0;
    }
}

/// @brief 跳转指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_jump(IRInst * inst){
	BrInst * brInst = (BrInst *) inst;
	if(brInst->jump_mode_ == jump_mode::hard_jump){
        Label * label = brInst->TOGO_Label;
        iloc.jump(label,jump_type::B);
    }
	if(brInst->jump_mode_ == jump_mode::conditional_jump){
		Label * lab1 = brInst->TOGO_Label;
        Label * lab2 = brInst->Option_Label;
        iloc.jump(lab2, jump_reverse(global_jump_type));
        iloc.jump(lab1, jump_type::B);
    }
}

void InstSelectorArm32::translate_cmp_int32(IRInst * inst){
    int op1_reg_no = REG_ALLOC_SIMPLE_SRC1_REG_NO;
    int op2_reg_no = REG_ALLOC_SIMPLE_SRC2_REG_NO;
    ICMPInst * icmpInst = (ICMPInst *) inst;
    Value * arg1 = icmpInst->srcValues[0];
    Value * arg2 = icmpInst->srcValues[1];
	switch (icmpInst->predicate_type)
	{
        case predicate::EQ:
		 	global_jump_type = jump_type::BEQ; 
			break;
		case predicate::NE:
		 	global_jump_type = jump_type::BNE; 
			break;
		case predicate::SGT:
		 	global_jump_type = jump_type::BGT; 
			break;
		case predicate::SGE:
		 	global_jump_type = jump_type::BGE; 
			break;
		case predicate::SLT:
		 	global_jump_type = jump_type::BLT; 
			break;
		case predicate::SLE:
		 	global_jump_type = jump_type::BLE; 
			break;
        default:
			global_jump_type = jump_type::BEQ;
            break;
	}
	

	std::string arg1_reg_name, arg2_reg_name;
    int arg1_reg_no = arg1->regId, arg2_reg_no = arg2->regId;

    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {
        // arg1 -> r8
        iloc.load_var(op1_reg_no, arg1);
    } else if (arg1_reg_no != op1_reg_no) {
        // 已分配的操作数1的寄存器和操作数2的缺省寄存器一致，这样会使得操作数2的值设置到一个寄存器上
        // 缺省寄存器  2    3
        // 实际寄存器  3    -1   有问题
        // 实际寄存器  3    3    有问题
        // 实际寄存器  3    4    无问题
        if ((arg1_reg_no == op2_reg_no) && ((arg2_reg_no == -1) || (arg2_reg_no == op2_reg_no))) {
            iloc.mov_reg(op1_reg_no, arg1_reg_no);
        } else {
            op1_reg_no = arg1_reg_no;
        }
    }

    arg1_reg_name = PlatformArm32::regName[op1_reg_no];

    // 看arg2是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg2_reg_no == -1) {
        // arg1 -> r8
        iloc.load_var(op2_reg_no, arg2);
    } else if (arg2_reg_no != op2_reg_no) {
        // 已分配的操作数2的寄存器和操作数1的缺省寄存器一致，这样会使得操作数2的值设置到一个寄存器上
        // 缺省寄存器  2    3
        // 实际寄存器  -1   2   有问题
        // 实际寄存器  2    2    有问题
        // 实际寄存器  4    2    无问题
        if ((arg2_reg_no == op1_reg_no) && ((arg1_reg_no == -1) || (arg1_reg_no == op1_reg_no))) {
            iloc.mov_reg(op2_reg_no, arg2_reg_no);
        } else {
            op2_reg_no = arg2_reg_no;
        }
    }

    arg2_reg_name = PlatformArm32::regName[op2_reg_no];
    iloc.cmp(arg1_reg_name, arg2_reg_name);
}

/// @brief 指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate(IRInst * inst)
{
    // 操作符
    IRInstOperator op = inst->getOp();

    map<IRInstOperator, translate_handler>::const_iterator pIter;
    pIter = translator_handlers.find(op);
    if (pIter == translator_handlers.end()) {
        // 没有找到，则说明当前不支持
        printf("Translate: Operator(%d) not support", (int) op);
        return;
    }

    (this->*(pIter->second))(inst);
}


