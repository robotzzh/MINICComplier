/**
 * @file IRGenerator.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief AST遍历产生线性IR的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <unordered_map>
#include <stack>
#include "AST.h"
#include "SymbolTable.h"



/// @brief AST遍历产生线性IR类
class IRGenerator
{

public:
    /// @brief 构造函数
    /// @param root
    /// @param symtab
    IRGenerator(ast_node * root, SymbolTable * symtab);

    /// @brief 析构函数
    ~IRGenerator() = default;

    /// @brief 运行产生IR
    bool run();

protected:
    /// @brief 编译单元AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_compile_unit(ast_node * node);

    /// @brief 函数定义AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_function_define(ast_node * node);

    /// @brief 形式参数AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_function_formal_params(ast_node * node);

    /// @brief 函数调用AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_function_call(ast_node * node);

    /// @brief 语句块（含函数体）AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_block(ast_node * node);

    /// @brief 表达式语句ST节点翻译成线性中间IR的共同函数
    /// @param node AST节点
    /// @param show 是否显示值，true：显示，false：不显示
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_show_internal(ast_node * node, bool show);

    /// @brief 不显示表达式AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_expr_noshow(ast_node * node);

    /// @brief 显示表达式AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_expr_show(ast_node * node);

    /// @brief 整数加法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_add(ast_node * node);

    /// @brief 整数减法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_sub(ast_node * node);

    /// @brief 整数乘法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_mul(ast_node * node);

	/// @brief 整数除法AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_div(ast_node * node);

	/// @brief 取模AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_mod(ast_node * node);

	/// @brief 逻辑或AST节点翻译成线性IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_LOR(ast_node * node);

	/// @brief 逻辑与AST节点翻译成线性IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_LAND(ast_node * node);

	/// @brief 整数比较AST节点翻译成线性IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_ICMP(ast_node * node);

    /// @brief 赋值AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_assign(ast_node * node);

	/// @brief AST节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_declare(ast_node * node);

    /// @brief return节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_return(ast_node * node);

	/// @brief 单元节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_odd(ast_node * node);

	/// @brief 非运算单元节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_not(ast_node * node);


	/// @brief 非运算单元节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_global(ast_node * node);

	/// @brief while运算单元节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_while(ast_node * node);
	
	/// @brief break运算单元节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_break(ast_node * node);

	/// @brief continue运算单元节点翻译成线性中间IR
	/// @param node AST节点
	/// @return 翻译是否成功，true：成功，false：失败
	bool ir_continue(ast_node * node);

    /// @brief 标识符叶子节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_leaf_node_var_id(ast_node * node);

    /// @brief 无符号整数字面量叶子节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_leaf_node_uint(ast_node * node);

    /// @brief float数字面量叶子节点翻译成线性中间IR
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_leaf_node_float(ast_node * node);

    /// @brief 未知节点类型的节点处理
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_default(ast_node * node);

    /// @brief 无用节点类型的节点处理如返回值类型等
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_useless(ast_node * node);

	/// @brief IF系列节点节点类型的节点处理如返回值类型等
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
	bool ir_if(ast_node * node);


	/// @brief 数组表达式类型的节点处理如返回值类型等
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_arrayexp(ast_node * node);

	/// @brief goto类型的节点处理如返回值类型等
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_GOTO(ast_node * node);

	/// @brief label类型的节点处理如返回值类型等
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_LABEL(ast_node * node);

	/// @brief 数组表达式翻译
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_array(ast_node * node);

	/// @brief 数组维数翻译
    /// @param node AST节点
    /// @return 翻译是否成功，true：成功，false：失败
    bool ir_dims(ast_node * node);

    /// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
    /// @param node AST节点
    /// @return 成功返回node节点，否则返回nullptr
    ast_node * ir_visit_ast_node(ast_node * node);

    /// @brief AST的节点操作函数
    typedef bool (IRGenerator::*ast2ir_handler_t)(ast_node *);

    /// @brief AST节点运算符与动作函数关联的映射表
    std::unordered_map<ast_operator_type, ast2ir_handler_t> ast2ir_handlers;

private:
    /// @brief 抽象语法树的根
    ast_node * root;

	/// @brief 静态入口别用
	Label * static_entry = new Label("entry");

	/// @brief 静态入口别用
	Label * static_exit = new Label("exit");
	/// @brief 当前入口
    std::stack<Label *> current_entry ;

    /// @brief 当前出口
	std::stack<Label *> current_exit ;

	/// @brief 如果为真的出口
	std::stack<Label *>  true_exit ;

	/// @brief 如果为假的出口
	std::stack<Label *>   false_exit;


    /// @brief 符号表
    SymbolTable * symtab;
};

