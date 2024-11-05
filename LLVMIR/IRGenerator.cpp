/**
 * @file IRGenerator.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief AST遍历产生线性IR
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include "AST.h"
#include "IRCode.h"
#include "IRGenerator.h"
#include "IRInst.h"
#include "SymbolTable.h"
#include "Value.h"
#include "ValueType.h"
#include "Label.h"

bool is_in_or = false;
bool is_in_and = false;

/// @brief 构造函数
/// @param _root AST的根
/// @param _symtab 符号表
IRGenerator::IRGenerator(ast_node * _root, SymbolTable * _symtab) : root(_root), symtab(_symtab)
{
    /* 叶子节点 */
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_UINT] = &IRGenerator::ir_leaf_node_uint;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT] = &IRGenerator::ir_leaf_node_float;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_VAR_ID] = &IRGenerator::ir_leaf_node_var_id;

    /* 表达式运算， 加减乘除 */
    ast2ir_handlers[ast_operator_type::AST_OP_SUB] = &IRGenerator::ir_sub;
    ast2ir_handlers[ast_operator_type::AST_OP_ADD] = &IRGenerator::ir_add;
	ast2ir_handlers[ast_operator_type::AST_OP_MUL] = &IRGenerator::ir_mul;
	ast2ir_handlers[ast_operator_type::AST_OP_DIV] = &IRGenerator::ir_div;
	ast2ir_handlers[ast_operator_type::AST_OP_REM] = &IRGenerator::ir_mod;
	ast2ir_handlers[ast_operator_type::AST_OP_LOR] = &IRGenerator::ir_LOR;
	ast2ir_handlers[ast_operator_type::AST_OP_LAND] = &IRGenerator::ir_LAND;
	ast2ir_handlers[ast_operator_type::AST_OP_GOTO] = &IRGenerator::ir_GOTO;
	ast2ir_handlers[ast_operator_type::AST_OP_LABEL] = &IRGenerator::ir_LABEL;


	/* 比较语句 */
	ast2ir_handlers[ast_operator_type::AST_OP_EQ] = &IRGenerator::ir_ICMP;
	ast2ir_handlers[ast_operator_type::AST_OP_NE] = &IRGenerator::ir_ICMP;
	ast2ir_handlers[ast_operator_type::AST_OP_GE] = &IRGenerator::ir_ICMP;
	ast2ir_handlers[ast_operator_type::AST_OP_GN] = &IRGenerator::ir_ICMP;
	ast2ir_handlers[ast_operator_type::AST_OP_LE] = &IRGenerator::ir_ICMP;
	ast2ir_handlers[ast_operator_type::AST_OP_LN] = &IRGenerator::ir_ICMP;



    /* 语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_EXPR] = &IRGenerator::ir_expr_noshow;
    ast2ir_handlers[ast_operator_type::AST_OP_EXPR_SHOW] = &IRGenerator::ir_expr_show;
    ast2ir_handlers[ast_operator_type::AST_OP_ASSIGN] = &IRGenerator::ir_assign;
	ast2ir_handlers[ast_operator_type::AST_OP_NEG_I] = &IRGenerator::ir_odd;
    ast2ir_handlers[ast_operator_type::AST_OP_NOT] = &IRGenerator::ir_not;
    ast2ir_handlers[ast_operator_type::AST_OP_ONLY_IF] = &IRGenerator::ir_if;
    ast2ir_handlers[ast_operator_type::AST_OP_ONLY_IF_ELSE] = &IRGenerator::ir_if;
	ast2ir_handlers[ast_operator_type::AST_OP_WHILE] = &IRGenerator::ir_while;
    ast2ir_handlers[ast_operator_type::AST_OP_BREAK] = &IRGenerator::ir_break;
	ast2ir_handlers[ast_operator_type::AST_OP_CONTINUE] = &IRGenerator::ir_continue;
    ast2ir_handlers[ast_operator_type::AST_OP_ArrayExp] = &IRGenerator::ir_arrayexp;
	ast2ir_handlers[ast_operator_type::AST_OP_ARRAY] = &IRGenerator::ir_array;
    ast2ir_handlers[ast_operator_type::AST_OP_ARRAY_PARAMS] = &IRGenerator::ir_dims;

    // TODO:默认定义i32
	ast2ir_handlers[ast_operator_type::AST_OP_DECLARE_INT] = &IRGenerator::ir_declare;
	ast2ir_handlers[ast_operator_type::AST_OP_DECLARE_FLOAT] = &IRGenerator::ir_declare;
	ast2ir_handlers[ast_operator_type::AST_OP_DECLARE_ARR_I32] = &IRGenerator::ir_declare;
    ast2ir_handlers[ast_operator_type::AST_OP_RETURN_STATEMENT] = &IRGenerator::ir_return;

    /* 函数调用 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_CALL] = &IRGenerator::ir_function_call;

    /* 函数定义 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_DEF] = &IRGenerator::ir_function_define;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS] = &IRGenerator::ir_function_formal_params;

    /* 语句块 */
    ast2ir_handlers[ast_operator_type::AST_OP_BLOCK] = &IRGenerator::ir_block;

    /* 编译单元 */
    ast2ir_handlers[ast_operator_type::AST_OP_COMPILE_UNIT] = &IRGenerator::ir_compile_unit;

	/* 无用节点单元 */
	ast2ir_handlers[ast_operator_type::AST_OP_RETURN_INT] = &IRGenerator::ir_useless;
}

/// @brief 编译单元AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_compile_unit(ast_node * node)
{
    // 新建main函数并默认设置当前函数为main函数
    //symtab->mainFunc = symtab->newFunction("main", BasicType::TYPE_INT);
    //symtab->currentFunc = symtab->mainFunc;

    for (auto son: node->sons) {

        // 遍历编译单元，要么是函数定义，要么是语句
        ast_node * son_node = ir_visit_ast_node(son);
        if (!son_node) {
            return false;
        }
		node->DeclareBlock.addInst(son_node->DeclareBlock);
        node->blockInsts.addInst(son_node->blockInsts);
    }


    // 获取函数的IR代码列表，用于后面追加指令用，注意这里用的是引用传值
    //InterCode & irCode = symtab->currentFunc->getInterCode();

    // 创建并加入Entry入口指令
    //irCode.addInst(new EntryIRInst());

    // 创建出口指令并不加入出口指令，等函数内的指令处理完毕后加入出口指令
	//TODO: 暂时改成ret
    //IRInst * exitLabelInst = new LabelIRInst();
    //symtab->currentFunc->setExitLabel(exitLabelInst);

    // 新建一个Value，用于保存函数的返回值，如果没有返回值可不用申请，
    // 目前不需要
	/*
#if 0
    Value * retValue = symtab->currentFunc->newVarValue(BasicType::TYPE_INT);
    symtab->currentFunc->setReturnValue(retValue);
#endif

    // 除了函数定义的指令外都加入到main函数的指令当中
    irCode.addInst(node->blockInsts);

    // 添加函数出口Label指令，主要用于return语句跳转到这里进行函数的退出
    //irCode.addInst(exitLabelInst);

    // 尾部追加一个return 0指令，使得main函数的格式正确
    irCode.addInst(new ExitIRInst(new ConstValue(0)));

    // main函数移动到列表的尾部，以便后续简便处理
    symtab->moveFunctionEnd(symtab->mainFunc);
*/
    // 设置成空，使得后续访问该变量出错。
    symtab->currentFunc = nullptr;

    return true;
}

/// @brief 函数定义AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_define(ast_node * node)
{
    // 创建一个函数，用于当前函数处理
    current_entry.push(new Label("entry"));
	current_exit.push(new Label("exit"));
    if (symtab->currentFunc != symtab->mainFunc) {
        // 函数中嵌套定义函数，这是不允许的，错误退出
        // TODO 自行追加语义错误处理
        return false;
    }
    ast_node * returntype = node->sons[0];
	if(returntype->node_type == ast_operator_type::AST_OP_RETURN_INT){
		symtab->currentFunc = new Function(node->name, BasicType::TYPE_INT);
	}

    // 创建一个新的函数定义，函数的返回类型设置为VOID，待定，必须等return时才能确定，目前可以是VOID或者INT类型
    // TODO:解决函数定义时的问题symtab->currentFunc = new Function(node->name, BasicType::TYPE_VOID);
    bool result = symtab->insertFunction(symtab->currentFunc);
    if (!result) {
        // 清理资源
        delete symtab->currentFunc;

        // 恢复当前函数指向main函数
        symtab->currentFunc = symtab->mainFunc;

        // 函数已经定义过了，不能重复定义，语义错误：出错返回。
        // TODO 自行追加语义错误处理
        return false;
    }

    // 获取函数的IR代码列表，用于后面追加指令用，注意这里用的是引用传值
    InterCode & irCode = symtab->currentFunc->getInterCode();

    // 这里也可增加一个函数入口Label指令，便于后续基本块划分

    // 创建并加入Entry入口指令
    irCode.addInst(new EntryIRInst());

    // 创建出口指令并不加入出口指令，等函数内的指令处理完毕后加入出口指令
    Label * exitLabelInst = new Label("exit");

    // 函数出口指令保存到函数信息中，因为在语义分析函数体时return语句需要跳转到函数尾部，需要这个label指令
    symtab->currentFunc->setExitLabel(exitLabelInst);

    // 新建一个Value，用于保存函数的返回值，如果没有返回值可不用申请，
    // 目前未知，先创建一个，不用后续可释放
    Value * retValue = symtab->currentFunc->newVarValue("ret",BasicType::TYPE_INT);

    // 保存函数返回值变量到函数信息中，在return语句翻译时需要设置值到这个变量中
    symtab->currentFunc->setReturnValue(retValue);

    // 遍历函数体内的每个语句
    for (auto son: node->sons) {
        // 遍历函数定义，孩子要么是形式参数，要么是block
        ast_node * son_node = ir_visit_ast_node(son);
        if (!son_node) {

            // 对函数体内的语句进行语义分析时出现错误
            return false;
        }
		node->DeclareBlock.addInst(son_node->DeclareBlock);
        // IR指令追加到当前的节点中
        node->blockInsts.addInst(son_node->blockInsts);
    }

	int temp = 0;
    for (auto DeclItem: node->DeclareBlock.getInsts()) {
        DeclareIRInst * declItem = (DeclareIRInst *)DeclItem;
        temp += declItem->getSize();
    }
    symtab->currentFunc->setMaxDep(temp);
    // 此时，所有指令都加入到当前函数中，也就是node->blockInsts


	// node节点中先放入定义部分
	irCode.addInst(node->DeclareBlock);
    // node节点的指令移动到函数的IR指令列表中
    irCode.addInst(node->blockInsts);
    

    // 添加函数出口Label指令，主要用于return语句跳转到这里进行函数的退出
    // irCode.addInst(exitLabelInst);

    // 检查函数是否有返回值类型，则需要设置返回值，否则不设置
    if (symtab->currentFunc->getReturnType().type != BasicType::TYPE_VOID) {
        // 函数出口指令
		if(symtab->currentFunc->getReturnValue()->isConst()){
            irCode.addInst(new ExitIRInst(retValue, 1, symtab->currentFunc->getReturnValue()->intVal));
        } else {
            irCode.addInst(new ExitIRInst(symtab->currentFunc->getReturnValue(),2));
        }
        //irCode.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP,exitLabelInst));
		//irCode.addInst(new LabelIRInst(exitLabelInst->GetLabelName()));
    } else {
        // 清理资源恢复原状
        symtab->currentFunc->deleteVarValue(retValue);
        symtab->currentFunc->setReturnValue(nullptr);
        delete retValue;

        // 函数出口指令
        irCode.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP,exitLabelInst));
    }
	symtab->currentFunc->resetTempNo();

    // 恢复成指向main函数
    symtab->currentFunc = symtab->mainFunc;

    return true;
}

/// @brief 形式参数AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_formal_params(ast_node * node)
{
    // 获取当前要保存函数的形式参数清单
    auto & params = symtab->currentFunc->getParams();

    // 遍历形式参数列表，孩子是叶子节点
    for (auto son: node->sons) {

        // 创建变量，默认整型
		if(son->node_type == ast_operator_type::AST_OP_FUNC_FORMAL_PARAM_INT)
		{
			Value * var = symtab->currentFunc->newVarValue(son->name, BasicType::TYPE_INT);
			params.emplace_back(son->name, BasicType::TYPE_INT, var);
		}
		if(son->node_type == ast_operator_type::AST_OP_FUNC_FORMAL_PARAM_FLOAT)
		{
			Value * var = symtab->currentFunc->newVarValue(son->name, BasicType::TYPE_FLOAT);
			params.emplace_back(son->name, BasicType::TYPE_FLOAT, var);
		}
		if(son->node_type == ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS_POINTER)
		{
			Value * var = symtab->currentFunc->newVarValue(son->name, BasicType::TYPE_POINTER_i32);
			params.emplace_back(son->name, BasicType::TYPE_POINTER_i32, var);
		}

        // 默认是整数类型
    }

    return true;
}

/// @brief 函数调用AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_call(ast_node * node)
{
    std::vector<Value *> realParams;

    // 根据函数名查找函数，看是否存在。若不存在则出错
    // 这里约定函数必须先定义后使用
    auto pFunction = symtab->findFunction(node->name);
    if (nullptr == pFunction) {
        // TODO 这里输出错误信息
        return false;
    }

    // 设置存在函数调用
    symtab->currentFunc->setExistFuncCall(true);

    // 如果没有孩子，也认为是没有参数
    if (!node->sons.empty()) {

        // 只有一个节点，实际参数列表
        auto paramsNode = node->sons[0];

        int argsCount = (int) paramsNode->sons.size();

        // 设置最大函数调用参数个数
        if (argsCount > symtab->currentFunc->getMaxFuncCallArgCnt()) {
            symtab->currentFunc->setMaxFuncCallArgCnt(argsCount);
        }

        // 遍历参数列表，孩子是表达式
        // 这里自左往右计算表达式
        for (auto son: paramsNode->sons) {

            // 遍历Block的每个语句，进行显示或者运算
            ast_node * temp = ir_visit_ast_node(son);
            if (!temp) {
                return false;
            }

            realParams.push_back(temp->val);
			node->DeclareBlock.addInst(temp->DeclareBlock);
            node->blockInsts.addInst(temp->blockInsts);
        }
    }

    // 创建临时变量，用于保存函数调用的结果
    Value * resultVal = nullptr;

    // 返回调用有返回值，则需要分配临时变量
    if (pFunction->getReturnType().type == BasicType::TYPE_INT) {
        resultVal = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
    }
	if (pFunction->getReturnType().type == BasicType::TYPE_FLOAT) {
        resultVal = symtab->currentFunc->newTempValue(BasicType::TYPE_FLOAT);
    }
	if (pFunction->getReturnType().type == BasicType::TYPE_ARRAY_i32) {
        resultVal = symtab->currentFunc->newTempValue(BasicType::TYPE_ARRAY_i32);
    }

    // 创建函数调用指令
    node->blockInsts.addInst(new FuncCallIRInst(node->name, realParams, resultVal));

    // 设置存在函数调用，后面要根据是否函数调用进行调整栈分配策略
    symtab->currentFunc->setExistFuncCall(true);

    // 函数调用结果保存到node中，用于外部使用
    node->val = resultVal;

    return true;
}

/// @brief 语句块（含函数体）AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_block(ast_node * node)
{
	node->blockInsts.addInst(new BlockborderInst(0));
    std::vector<ast_node *>::iterator pIter;
    for (pIter = node->sons.begin(); pIter != node->sons.end(); ++pIter) {

        // 遍历Block的每个语句，进行显示或者运算
        ast_node * temp = ir_visit_ast_node(*pIter);
        if (!temp) {
            return false;
        }
		node->DeclareBlock.addInst(temp->DeclareBlock);
        node->blockInsts.addInst(temp->blockInsts);
    }
	node->blockInsts.addInst(new BlockborderInst(1));
    return true;
}

/// @brief 表达式语句ST节点翻译成线性中间IR的共同函数
/// @param node AST节点
/// @param show 是否显示值，true：显示，false：不显示
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_show_internal(ast_node * node, bool show)
{
    // TODO real number print

    ast_node * src1_node = node->sons[0];

    ast_node * result = ir_visit_ast_node(src1_node);
    if (!result) {
        // 解析错误
        return false;
    }
	node->DeclareBlock.addInst(result->DeclareBlock);
    node->blockInsts.addInst(result->blockInsts);

    if (show && (result->val != nullptr)) {

        // 创建函数调用指令
        node->blockInsts.addInst(new FuncCallIRInst("putint", result->val));

        // 设置存在函数调用，后面要根据是否函数调用进行调整栈分配策略
        symtab->currentFunc->setExistFuncCall(true);
    }

    node->val = nullptr;

    return true;
}

/// @brief 不显示表达式AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_expr_noshow(ast_node * node)
{
    return ir_show_internal(node, false);
}

/// @brief 显示表达式AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_expr_show(ast_node * node)
{
    return ir_show_internal(node, true);
}

/// @brief 整数加法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_add(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

	

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    // TODO real number add


    // 创建临时变量保存IR的值，以及线性IR指令
	node->DeclareBlock.addInst(left->DeclareBlock);
	node->DeclareBlock.addInst(right->DeclareBlock);
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
	if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
	Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
    node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_ADD_I, resultValue, left->val, right->val));
    node->val = resultValue;

    return true;
}

/// @brief 整数减法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_sub(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }



    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    // TODO real number add

    

    // 创建临时变量保存IR的值，以及线性IR指令
	node->DeclareBlock.addInst(left->DeclareBlock);
	node->DeclareBlock.addInst(right->DeclareBlock);
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
	if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
	Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
    node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_SUB_I, resultValue, left->val, right->val));
    node->val = resultValue;

    return true;
}

	/// @brief 整数乘法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mul(ast_node * node){
		ast_node * src1_node = node->sons[0];
    	ast_node * src2_node = node->sons[1];

    	// 乘法法节点，左结合，先计算左节点，后计算右节点

    	// 乘法的左边操作数
		ast_node * left = ir_visit_ast_node(src1_node);
		if (!left) {
			// 某个变量没有定值
			return false;
		}

		// 乘法的右边操作数
		ast_node * right = ir_visit_ast_node(src2_node);
		if (!right) {
			// 某个变量没有定值
			return false;
		}

		

		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    	// TODO real number mul
		
	

		// 创建临时变量保存IR的值，以及线性IR指令
		node->DeclareBlock.addInst(left->DeclareBlock);
		node->DeclareBlock.addInst(right->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		node->blockInsts.addInst(right->blockInsts);
		if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
		Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_MUL_I, resultValue, left->val, right->val));
		node->val = resultValue;

		return true;
	}

	/// @brief 整数除法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_div(ast_node * node){
		ast_node * src1_node = node->sons[0];
    	ast_node * src2_node = node->sons[1];

    	// 乘法法节点，左结合，先计算左节点，后计算右节点

    	// 乘法的左边操作数
		ast_node * left = ir_visit_ast_node(src1_node);
		if (!left) {
			// 某个变量没有定值
			return false;
		}

		// 乘法的右边操作数
		ast_node * right = ir_visit_ast_node(src2_node);
		if (!right) {
			// 某个变量没有定值
			return false;
		}

	
		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    	// TODO real number mul


		// 创建临时变量保存IR的值，以及线性IR指令
		node->DeclareBlock.addInst(left->DeclareBlock);
		node->DeclareBlock.addInst(right->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		node->blockInsts.addInst(right->blockInsts);
		if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
		Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_DIV_I, resultValue, left->val, right->val));
		node->val = resultValue;

		return true;
	}

/// @brief 取模AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mod(ast_node * node){
	ast_node * src1_node = node->sons[0];
    	ast_node * src2_node = node->sons[1];

    	// 乘法法节点，左结合，先计算左节点，后计算右节点

    	// 乘法的左边操作数
		ast_node * left = ir_visit_ast_node(src1_node);
		if (!left) {
			// 某个变量没有定值
			return false;
		}

		// 乘法的右边操作数
		ast_node * right = ir_visit_ast_node(src2_node);
		if (!right) {
			// 某个变量没有定值
			return false;
		}



		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    	// TODO real number mul


		// 创建临时变量保存IR的值，以及线性IR指令
		node->DeclareBlock.addInst(left->DeclareBlock);
		node->DeclareBlock.addInst(right->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		node->blockInsts.addInst(right->blockInsts);
		if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
		Value * DIV_result = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		Value * MUL_result = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_DIV_I, DIV_result, left->val, right->val));
		node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_MUL_I, MUL_result, DIV_result, right->val));
		node->blockInsts.addInst(new BinaryIRInst(IRInstOperator::IRINST_OP_SUB_I, resultValue, left->val, MUL_result));
		node->val = resultValue;

		return true;
}


static bool is_cmp(ast_operator_type ast_type){
	switch(ast_type){
        case ast_operator_type::AST_OP_EQ:
        case ast_operator_type::AST_OP_GE:
        case ast_operator_type::AST_OP_GN:
        case ast_operator_type::AST_OP_LE:
        case ast_operator_type::AST_OP_LN:
        case ast_operator_type::AST_OP_NE:
            return true;
        default:
            return false;
    }
}

	/// @brief 逻辑或 AST节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_LOR(ast_node * node){
    is_in_or = true;
    is_in_and = false;
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];
    Label * fals = symtab->currentFunc->newLabel("", Label_Type::OR_FALESs);
    false_exit.push(fals);
    // 二元项节点，左结合，先计算左节点，后计算右节点
    ast_node * left = ir_visit_ast_node(src1_node);
		if (!left) {
			// 某个变量没有定值
			return false;
		}
	false_exit.pop();
		// 乘法的右边操作数
		ast_node * right = ir_visit_ast_node(src2_node);
		if (!right) {
			// 某个变量没有定值
			return false;
		}


		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    	// TODO real number mul
	

		// 创建临时变量保存IR的值，以及线性IR指令
		node->DeclareBlock.addInst(left->DeclareBlock);
		node->DeclareBlock.addInst(right->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		node->blockInsts.addInst(new LabelIRInst(fals->GetLabelName()));	
		node->blockInsts.addInst(right->blockInsts);
		/*
		if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
		Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		node->blockInsts.addInst(
			new BinaryIRInst(
				IRInstOperator::IRINST_OP_OR, 
				resultValue,
				left->val, 
				right->val
			)
		);
		node->val = resultValue;
		*/
        is_in_or = false;
        return true;
}


	/// @brief 逻辑与 AST节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_LAND(ast_node * node){
	is_in_or = false;
    is_in_and = true;
	Label * trues = symtab->currentFunc->newLabel("", Label_Type::AND_TRUEs);
    true_exit.push(trues);
	ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    	// 二元项节点，左结合，先计算左节点，后计算右节点

	ast_node * left = ir_visit_ast_node(src1_node);
		if (!left) {
			// 某个变量没有定值
			return false;
		}
	true_exit.pop();
		// 乘法的右边操作数
		ast_node * right = ir_visit_ast_node(src2_node);
		if (!right) {
			// 某个变量没有定值
			return false;
		}

		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    	// TODO real number mul
	

		// 创建临时变量保存IR的值，以及线性IR指令
		node->DeclareBlock.addInst(left->DeclareBlock);
		node->DeclareBlock.addInst(right->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		node->blockInsts.addInst(new LabelIRInst(trues->GetLabelName()));
		node->blockInsts.addInst(right->blockInsts);
		/*
		if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
	if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
		Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		node->blockInsts.addInst(
			new BinaryIRInst(
				IRInstOperator::IRINST_OP_AND, 
				resultValue,
				left->val, 
				right->val
			)
		);
		node->val = resultValue;
		*/
		is_in_and = false;
		return true;
}

	/// @brief 整形AST节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ICMP(ast_node * node){
	ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    	// 二元项节点，左结合，先计算左节点，后计算右节点

		ast_node * left = ir_visit_ast_node(src1_node);
		if (!left) {
			// 某个变量没有定值
			return false;
		}

		// 乘法的右边操作数
		ast_node * right = ir_visit_ast_node(src2_node);
		if (!right) {
			// 某个变量没有定值
			return false;
		}
		
		// 创建临时变量保存IR的值，以及线性IR指令
		node->DeclareBlock.addInst(left->DeclareBlock);
		node->DeclareBlock.addInst(right->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		node->blockInsts.addInst(right->blockInsts);

		// 如果是变量，从栈当中直接获取
		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    	// TODO real number mul
		if(right->is_variable){
			Value * LoadRight = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadRight , right->val)
			);
			right->val = LoadRight;
		}
		if(left->is_variable){
			Value * LoadLeft = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
			node->blockInsts.addInst(
				new LoadIRInst(LoadLeft , left->val)
			);
			left->val = LoadLeft;
		}
	
		Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
		switch (node->node_type){
			case ast_operator_type::AST_OP_EQ:
				// 等于
				node->blockInsts.addInst(
				new ICMPInst(IRInstOperator::IRINST_OP_ICMP, resultValue,left->val, right->val,predicate::EQ)
				);
				break;
			case ast_operator_type::AST_OP_NE:
				// 不等于
				node->blockInsts.addInst(
				new ICMPInst(
					IRInstOperator::IRINST_OP_ICMP, 
					resultValue,
					left->val, 
					right->val,
					predicate::NE
					)
				);
				break;
			case ast_operator_type::AST_OP_GN:
				// 大于
				node->blockInsts.addInst(
				new ICMPInst(IRInstOperator::IRINST_OP_ICMP, resultValue,left->val, right->val,predicate::SGT)
				);
				break;
			case ast_operator_type::AST_OP_GE:
				// 大于等于
				node->blockInsts.addInst(
				new ICMPInst(IRInstOperator::IRINST_OP_ICMP, resultValue, left->val, right->val,predicate::SGE)
				);
				break;
			case ast_operator_type::AST_OP_LN:
				// 小于
				node->blockInsts.addInst(
				new ICMPInst(IRInstOperator::IRINST_OP_ICMP, resultValue,left->val, right->val,predicate::SLT)
				);
				break;
			case ast_operator_type::AST_OP_LE:
				// 小于等于
				node->blockInsts.addInst(
				new ICMPInst(IRInstOperator::IRINST_OP_ICMP, resultValue,left->val, right->val,predicate::SLE)
				);
				break;
			default:
				node->blockInsts.addInst(
				new ICMPInst(IRInstOperator::IRINST_OP_ICMP, resultValue,left->val, right->val,predicate::EQ)
				);
				break;
		}
        Label * nextLabel1 = nullptr;
        Label * nextLabel2 = nullptr;
        if (true_exit.size() > 1){
            nextLabel1 = true_exit.top();
        }
        else{
			nextLabel1 = true_exit.top();
		}
        if (false_exit.size() > 1) {
            nextLabel2 = false_exit.top();
        }else{
			nextLabel2 = false_exit.top();
        }
        node->blockInsts.addInst(
                new BrInst(IRInstOperator::IRINST_OP_JUMP, nextLabel1, jump_mode::conditional_jump, nextLabel2, resultValue));
        node->val = resultValue;

		return true;
}


/// @brief 赋值AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_assign(ast_node * node)
{
    ast_node * son1_node = node->sons[0];
    ast_node * son2_node = node->sons[1];

    // 赋值节点，自右往左运算

    // 赋值运算符的左侧操作数
    ast_node * left = ir_visit_ast_node(son1_node);
    if (!left) {
        // 某个变量没有定值
        // 这里缺省设置变量不存在则创建，因此这里不会错误
        return false;
    }

    // 赋值运算符的右侧操作数
    ast_node * right;
        right = ir_visit_ast_node(son2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    // TODO real number add

    // 创建临时变量保存IR的值，以及线性IR指令
	node->DeclareBlock.addInst(right->DeclareBlock);
	node->DeclareBlock.addInst(left->DeclareBlock);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(left->blockInsts);
	if(right->is_global){
		// 全局部分
	}else{
		// 局部部分

	}
	if(right->is_variable && left->dims.empty()){
        Value * temp = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
        node->blockInsts.addInst(new LoadIRInst(temp, right->val));
		node->blockInsts.addInst(new AssignIRInst(left->val, temp));

    }
	else if(!left->dims.empty()){
	}
	else{
    	node->blockInsts.addInst(new AssignIRInst(left->val, right->val));
	}

    // 这里假定赋值的类型是一致的
    if (!right->val){
        right->val->type = left->val->type;
    }else{
		left->val->type = right->val->type;
	}
    node->val = left->val;
    node->val->intVal = right->val->intVal;
    node->is_const = left->is_const;
	node->is_global = left->is_global;
    std::string tempname = left->val->name;
    if (left->val->_global)
        left->val->intVal = right->val->intVal;
    if(left->is_global && left->is_const){
        node->is_variable = 0;
        node->is_const = 1;
        node->is_global = 1;
    }
	if(!left->dims.empty()){
        node->dims = left->dims;
    }
    return true;
}

/// @brief 定义AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_declare(ast_node * node){
	for(auto son1_node : node->sons){
		//ast_node * son2_node = node->sons[1];

		// 赋值节点，自右往左运算

		// 赋值运算符的左侧操作数
		ast_node * left = ir_visit_ast_node(son1_node);
		if (!left) {
			// 某个变量没有定值
			// 这里缺省设置变量不存在则创建，因此这里不会错误
			return false;
		}


		// 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
		// TODO real number add


		node->DeclareBlock.addInst(left->DeclareBlock);
		node->blockInsts.addInst(left->blockInsts);
		BasicType Type;
		int size = 4;
		if (node->node_type == ast_operator_type::AST_OP_DECLARE_INT) {
			left->val->setValueType(BasicType::TYPE_INT);
			Type = BasicType::TYPE_INT;
		}
		if(node->node_type == ast_operator_type::AST_OP_DECLARE_FLOAT){
			left->val->setValueType(BasicType::TYPE_FLOAT);
			Type = BasicType::TYPE_FLOAT;
		}
		if(node->node_type == ast_operator_type::AST_OP_DECLARE_ARR_I32){
			left->val->setValueType(BasicType::TYPE_POINTER_i32);
			Type = BasicType::TYPE_ARRAY_i32;
		}

		/// 目前只支持整数大小的定义
		if(!son1_node->is_global){
			if(son1_node->dims.empty())
				node->DeclareBlock.addInst(
					new DeclareIRInst(IRInstOperator::IRINST_OP_DECLARE,left->val,Type,size));
			else{
                node->DeclareBlock.addInst(
                    new DeclareIRInst(IRInstOperator::IRINST_OP_DECLARE, left->val, Type, son1_node->dims));
            }
            symtab->currentFunc->newVarValue(left->val->name, BasicType::TYPE_INT);
        } else {
			if(son1_node->node_type != ast_operator_type::AST_OP_ARRAY)
			{
            symtab->global_inst.addInst(new GLOBAL_DECLARE_Inst(IRInstOperator::IRINST_OP_GLOBAL_DECLARE,
                                                                son1_node->val,
                                                                son1_node->val->intVal,
                                                                (bool) son1_node->is_const,
                                                                BasicType::TYPE_INT,
                                                                4));
			}else{
			symtab->global_inst.addInst(new GLOBAL_DECLARE_Inst(IRInstOperator::IRINST_OP_GLOBAL_DECLARE,
                                                                son1_node->val,
                                                                son1_node->val->intVal,
                                                                (bool) son1_node->is_const,
                                                                BasicType::TYPE_ARRAY_i32,
                                                                4,
																son1_node->sons[1]->dims));	
			}
        }
        //node->blockInsts.addInst(new AssignIRInst(left->val, right->val));
		//left->val->type = right->val->type;


		node->val = left->val;
	}

    return true;
}

/// @brief IF系列节点节点类型的节点处理如返回值类型等
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_if(ast_node * node){

	int node_length = node->sons.size();
    std::vector<Label *> labels(node->sons.size());


	if(node_length == 2){
		labels[0] = symtab->currentFunc->newLabel("", Label_Type::IF);
		labels[1] = symtab->currentFunc->newLabel("",Label_Type::END);
        if(true_exit.empty())
            true_exit.push(labels[0]);
		else{
            true_exit.pop();
			true_exit.push(labels[0]);
        }
		if(false_exit.empty())
            false_exit.push(labels[1]);
		else{
            false_exit.pop();
			false_exit.push(labels[1]);
        }
    }

    if(node_length == 3){
			labels[0] = symtab->currentFunc->newLabel("", Label_Type::IF);
            labels[1] = symtab->currentFunc->newLabel("", Label_Type::ELSE);
            labels[2] = symtab->currentFunc->newLabel("", Label_Type::END);
		if(true_exit.empty())
            true_exit.push(labels[0]);
		else{
            true_exit.pop();
			true_exit.push(labels[0]);
        }
		if(false_exit.empty())
            false_exit.push(labels[1]);
		else{
            false_exit.pop();
			false_exit.push(labels[1]);
        }
			
	}

        ast_node * condition = ir_visit_ast_node(node->sons[0]);
        if(!condition) return false;
		node->DeclareBlock.addInst(condition->DeclareBlock);
		node->blockInsts.addInst(condition->blockInsts);

		/*
		node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP,
                                                labels[0],
                                                jump_mode::conditional_jump,
                                                labels[1],
                                                condition->val));
												*/

        for (int i = 1; i < node_length;i++){
			
			node->blockInsts.addInst(new LabelIRInst(labels[i-1]->GetLabelName()));
			ast_node * if_block_node = ir_visit_ast_node(node->sons[i]);
			if(!if_block_node) return false;

			node->DeclareBlock.addInst(if_block_node->DeclareBlock);
			node->blockInsts.addInst(if_block_node->blockInsts);

			node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP, labels[node_length-1]));
		}
		node->blockInsts.addInst(new LabelIRInst(labels[node_length-1]->GetLabelName()));
		return true;
}

/// @brief break运算单元节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_break(ast_node * node){
	node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP, current_exit.top()));
    return true;
}

/// @brief continue运算单元节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_continue(ast_node * node){
    node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP, current_entry.top()));
    return true;
}

/// @brief return节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_return(ast_node * node)
{
    ast_node * right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {

        ast_node * son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {

            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    // 创建临时变量保存IR的值，以及线性IR指令
	node->DeclareBlock.addInst(right->DeclareBlock);
    node->blockInsts.addInst(right->blockInsts);

    // 返回值赋值到函数返回值变量上，然后跳转到函数的尾部
    if (!right->sons.empty()) {
        symtab->currentFunc->setReturnValue(right->val);
    }else{
		if(!right->val->_const)
        	node->blockInsts.addInst(new LoadIRInst(symtab->currentFunc->getReturnValue(),right->val));
    }
    // 跳转到函数的尾部出口指令上
    //node->blockInsts.addInst(new GotoIRInst(symtab->currentFunc->getExitLabel()));

    node->val = right->val;

    // 这里设置返回值类型
    ValueType & returnType = symtab->currentFunc->getReturnType();
	if(node->val->isConst()){
        symtab->currentFunc->setReturnValue(right->val);
    }
    if(returnType.type == BasicType::TYPE_INT){
		returnType.type = BasicType::TYPE_INT;
	}
    if (returnType.type == BasicType::TYPE_VOID) {
        // 设置类型
        returnType.type = right->val->type.type;
    } else if (returnType.type != right->val->type.type) {
        // 两者类型不一致，要出错显示
        // 或者隐式转换成更高的类型
        // TODO 这里目前什么都不做
    }

    return true;
}

/// @brief 单元节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_odd(ast_node * node){
	ast_node * right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {

        ast_node * son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {

            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(right->blockInsts);
	node->DeclareBlock.addInst(right->DeclareBlock);
	Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
    node->blockInsts.addInst(new OddIRInst(IRInstOperator::IRINST_OP_NEG_I,resultValue,right->val));
	node->val = resultValue;
	return true;

}

/// @brief 标识符叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_var_id(ast_node * node)
{
    Value * val;

    // 新建一个ID型Value

    // 变量，则需要在符号表中查找对应的值
    // 若变量之前没有有定值，则采用默认的值为0
	if(!node->is_global)
	{
		val = symtab->currentFunc->findValue(node->name, false);
        
        if (!val) {
			// 变量在域内不存在，则在全局中搜索
			val = symtab->findValue(node->name, false);
			
			if(!val){
            	// 变量在域内，全局也不存在，则创建一个变量
				val = symtab->currentFunc->newVarValue(node->name,BasicType::TYPE_INT);
			}
        }
    }
	else{
        val = symtab->findValue(node->name, false);
		if(!val){
			val = symtab->newVarValue(node->name,BasicType::TYPE_INT);
			val->_global = true;
			if(node->is_const) val->_const = true;
		}
    }

    node->val = val;
	if(val->_const)
        node->is_variable = 0;

    return true;
}

/// @brief 无符号整数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_uint(ast_node * node)
{
    Value * val;

    // 新建一个整数常量Value
    val = symtab->newConstValue((int32_t) node->integer_val);

    node->val = val;

    return true;
}

/// @brief float数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_float(ast_node * node)
{
    Value * val = nullptr;

    // 新建一个实数型常量Value
    val = symtab->newConstValue(node->float_val);

    node->val = val;

    return true;
}

/// @brief 未知节点类型的节点处理
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_default(ast_node * node)
{
    // 未知的节点
    printf("Unkown node(%d)\n", (int) node->node_type);
    return true;
}

/// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
/// @param node AST节点
/// @return 成功返回node节点，否则返回nullptr
ast_node * IRGenerator::ir_visit_ast_node(ast_node * node)
{
    // 空节点
    if (nullptr == node) {
        return nullptr;
    }

    bool result;

    std::unordered_map<ast_operator_type, ast2ir_handler_t>::const_iterator pIter;
    pIter = ast2ir_handlers.find(node->node_type);
    if (pIter == ast2ir_handlers.end()) {
        // 没有找到，则说明当前不支持
        result = (this->ir_default)(node);
    } else {
        result = (this->*(pIter->second))(node);
    }

    if (!result) {
        // 语义解析错误，则出错返回
        node = nullptr;
    }

    return node;
}

/// @brief 非运算单元节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_not(ast_node * node){
	ast_node * right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {

        ast_node * son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {

            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(right->blockInsts);
	node->DeclareBlock.addInst(right->DeclareBlock);
	Value * resultValue = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);
    node->blockInsts.addInst(new OddIRInst(IRInstOperator::IRINST_OP_NOT,resultValue,right->val));
	node->val = resultValue;
    return true;
}

	/// @brief 数组表达式翻译
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool IRGenerator::ir_array(ast_node * node){
        ast_node * id_node;
		ast_node * dims_node;
        id_node = ir_visit_ast_node(node->sons[0]);
		if(!id_node)
            return false;
		
        dims_node = ir_visit_ast_node(node->sons[1]);
		if(!dims_node)
            return false;

        node->name = id_node->name;
        node->is_global = id_node->is_global;
        node->is_const = id_node->is_const;
        node->val = id_node->val;
        node->dims = dims_node->dims;

        return true;
    }

    /// @brief 数组维数翻译
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool IRGenerator::ir_dims(ast_node * node){
		for(auto son : node->sons){
			ast_node * dim_node = ir_visit_ast_node(son);
            if(!dim_node)
                return false;
            node->dims.push_back(dim_node->val->intVal);
		}
        return true;
    }

/// @brief WHILE系列节点节点类型的节点处理如返回值类型等
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_while(ast_node * node){

    std::vector<Label *> labels(3);

        labels[0] = symtab->currentFunc->newLabel("", Label_Type::WHILE_COND);
        labels[1] = symtab->currentFunc->newLabel("", Label_Type::WHILE_BODY);
        labels[2] = symtab->currentFunc->newLabel("", Label_Type::END);

		if(true_exit.empty())
            true_exit.push(labels[1]);
		else{
            true_exit.pop();
			true_exit.push(labels[1]);
        }
		if(false_exit.empty())
            false_exit.push(labels[2]);
		else{
            false_exit.pop();
			false_exit.push(labels[2]);
        }

        //node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP, labels[0]));
        //cond
        node->blockInsts.addInst(new LabelIRInst(labels[0]->GetLabelName()));
        ast_node * while_cond_node = ir_visit_ast_node(node->sons[0]);
        if(!while_cond_node) return false;

        node->DeclareBlock.addInst(while_cond_node->DeclareBlock);
        node->blockInsts.addInst(while_cond_node->blockInsts);
		/*
        node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP,
                                                labels[1],
                                                jump_mode::conditional_jump,
                                                labels[2],
                                                while_cond_node->val));
		*/
        //body
        current_entry.push(labels[1]);
		current_exit.push(labels[2]);
        node->blockInsts.addInst(new LabelIRInst(labels[1]->GetLabelName()));
        ast_node * while_body_node = ir_visit_ast_node(node->sons[1]);
        if(!while_body_node) return false;

        node->DeclareBlock.addInst(while_body_node->DeclareBlock);
        node->blockInsts.addInst(while_body_node->blockInsts);

        node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP, labels[0]));

        //end
        node->blockInsts.addInst(new LabelIRInst(labels[2]->GetLabelName()));
        current_entry.pop();
        current_exit.pop();
        return true;

}

/// @brief 非运算单元节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_global(ast_node * node){
    return true;
}


/// @brief 数组表达式类型的节点处理如返回值类型等
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_arrayexp(ast_node * node){
    ast_node * node1 = node->parent->sons[0];
    std::string name = node1->name;
    u_int32_t size = 1;
	for(auto dim : node1->dims)
        size = dim * size;
    size = size * 4;
    Value * val;

    val = symtab->currentFunc->findValue(name, false);
    if (!val) {

        // 变量不存在，则创建一个变量
        val = symtab->currentFunc->newVarValue(name,BasicType::TYPE_ARRAY_i32);
    }
	node->val = val;
    node->blockInsts.addInst(new LLVM_FUNC_INST(
		IRInstOperator::IRINST_OP_MEMSET, 
		name,
		func_type::memset,
		size));
    int i = 0;
    for (auto son: node->sons) {
        Value * elementval;
    	elementval = symtab->newConstValue((int32_t) son->integer_val);
        Value * tempval = symtab->currentFunc->newTempValue(BasicType::TYPE_INT);

		// 使用llvmIR的内部函数getelementptr，提供数组名，地址，一个寄存器
		node->blockInsts.addInst(new LLVM_FUNC_INST(
			IRInstOperator::IRINST_OP_GETELEMENTPTR,
			name,
			func_type::getelementptr,
			size,
			tempval->getName(),
			std::to_string(i)
		));

        node->blockInsts.addInst(new AssignIRInst(elementval, tempval));
    	i++;
    }
    return true;
}
/// @brief goto类型的节点处理如返回值类型等
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_GOTO(ast_node * node){
	node->blockInsts.addInst(new BrInst(IRInstOperator::IRINST_OP_JUMP, node->label));
    return true;
}

    /// @brief label类型的节点处理如返回值类型等
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_LABEL(ast_node * node){
    Label * lab1;

    // 新建一个ID型Value

    // 变量，则需要在符号表中查找对应的值
    // 若变量之前没有有定值，则采用默认的值为0

	lab1 = symtab->currentFunc->findLabel(node->label->GetLabelName());
    if (!lab1) {

        // 变量不存在，则创建一个变量
        lab1 = symtab->currentFunc->newLabel(node->label->GetLabelName());
    }
    node->blockInsts.addInst(new LabelIRInst(lab1->GetLabelName()));

    return true;
}
/// @brief 无用节点类型的节点处理如返回值类型等
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_useless(ast_node * node){
	return true;
}






/// @brief 遍历抽象语法树产生线性IR，保存到IRCode中
/// @param root 抽象语法树
/// @param IRCode 线性IR
/// @return true: 成功 false: 失败
bool IRGenerator::run()
{
    ast_node * node;

    // 从根节点进行遍历
    node = ir_visit_ast_node(root);

    return node != nullptr;
}
