/**
 * @file ast.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 抽象语法树AST管理的源文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "AttrType.h"
#include "IRCode.h"
#include "Value.h"
#include "Label.h"

/// @brief AST节点的类型
enum class ast_operator_type : int {

    /* 以下为AST的叶子节点 */

    /// @brief 无符号整数字面量叶子节点
    AST_OP_LEAF_LITERAL_UINT,

    /// @brief  无符号浮点数数字面量叶子节点
    AST_OP_LEAF_LITERAL_FLOAT,

    /// @brief  无符号浮点数数字面量叶子节点
    AST_OP_LEAF_LITERAL_Boolean,

    /// @brief 变量ID叶子节点
    AST_OP_LEAF_VAR_ID,

    /// @brief 复杂类型的节点
    AST_OP_LEAF_TYPE,

    /* 以下为AST的内部节点，含根节点 */
    /// @brief 二元运算符+
    AST_OP_ADD,

    /// @brief 二元运算符-
    AST_OP_SUB,

    /// @brief 二元运算符*
    AST_OP_MUL, //

    /// @brief 二元运算符/
    AST_OP_DIV, //

    /// @brief 二元运算符%
    AST_OP_REM,

    /// @brief Uint强制类型转化
    AST_OP_ALERT_UINT,

    /// @brief Float强制类型转化
    AST_OP_ALERT_FLOAT,

    /// @brief 比较运算>大于
    AST_OP_GN,

    /// @brief 比较运算>=大于
    AST_OP_GE,

    /// @brief 比较运算<小于
    AST_OP_LN,

    /// @brief 比较运算<=小于等于
    AST_OP_LE,

    /// @brief 比较运算==等于
    AST_OP_EQ,

    /// @brief 比较运算!=不等于
    AST_OP_NE,

    /// @brief 逻辑运算与 &&
    AST_OP_LAND,

    /// @brief 逻辑运算或 ｜｜
    AST_OP_LOR,

    /// @brief 逻辑运算或 !
    AST_OP_NOT,

    /// @brief 只有一个if onlyif
    AST_OP_ONLY_IF,

    /// @brief  only if + else
    AST_OP_ONLY_IF_ELSE,

    /// @brief if else if  else if else if else
    AST_OP_MUL_IF_ELSE,

    /// @brief 打断 break
    AST_OP_BREAK,

    /// @brief 持续 continue
    AST_OP_CONTINUE,

    /// @brief 循环 while
    AST_OP_WHILE,

    /// @brief 多个语句组成的块运算符
    AST_OP_BLOCK,

    /// @brief 赋值语句运算符
    AST_OP_ASSIGN,

    /// @brief 表达式语句运算符，不显示表达式的值
    AST_OP_EXPR,

    /// @brief 表达式显示语句运算符，需要显示表达式的值
    AST_OP_EXPR_SHOW,

    /// @brief return语句运算符
    AST_OP_RETURN_STATEMENT,

    /// @brief 函数定义运算符，函数名和返回值类型作为节点的属性，自左到右孩子：AST_OP_FUNC_FORMAL_PARAMS、AST_OP_BLOCK
    AST_OP_FUNC_DEF,

    /// @brief 形式参数列表运算符，可包含多个孩子：AST_OP_FUNC_FORMAL_PARAM
    AST_OP_FUNC_FORMAL_PARAMS,

    /// @brief 形参运算符，属性包含名字与类型，复杂类型时可能要包含孩子
    AST_OP_FUNC_FORMAL_PARAM,

    /// @brief 函数调用运算符，函数名作为节点属性，孩子包含AST_OP_FUNC_REAL_PARAMS
    AST_OP_FUNC_CALL,

    /// @brief 实际参数列表运算符，可包含多个表达式AST_OP_EXPR
    AST_OP_FUNC_REAL_PARAMS,

    /// @brief 文件编译单元运算符，可包含函数定义、语句块等孩子
    AST_OP_COMPILE_UNIT,

    // TODO 抽象语法树其它内部节点运算符追加
    /// @brief if条件语句
    AST_OP_IF,

    /// @brief 二元运算符取余%
    AST_OP_REMA,

    /// @brief 最大标识符，表示非法运算符
    AST_OP_MAX,

    /// @brief 浮点定义型
    AST_OP_FLOAT,

    /// @brief 声明操作int
    AST_OP_DECLARE_INT,

    /// @brief 声明操作float
    AST_OP_DECLARE_FLOAT,

    /// @brief 整数定义型
    AST_OP_INT,

    /// @brief 无返回值
    AST_OP_RETURN_VOID,

    /// @brief 返回整形
    AST_OP_RETURN_INT,

    /// @brief 返回浮点型
    AST_OP_RETURN_FLOAT,

    /// @brief 参数整形型
    AST_OP_FUNC_FORMAL_PARAM_INT,

    /// @brief 参数浮点型
    AST_OP_FUNC_FORMAL_PARAM_FLOAT,

    /// @brief 整数取负
    AST_OP_NEG_I,

    /// @brief 数组表示
    AST_OP_ArrayExp,

    /// @brief 整形数组表示
    AST_OP_DECLARE_ARR_I32,

    /// @brief 整形指针表示
    AST_OP_DECLARE_POINTER_I32,

    /// @brief goto指令
    AST_OP_GOTO,

    /// @brief 标签
    AST_OP_LABEL,

    /// @brief 形参指针
    AST_OP_FUNC_FORMAL_PARAMS_POINTER,

    /// @brief 单独的数组元素，如a[10]/b[0]这类的
    AST_OP_array_element,

    /// @brief int a , b ,c 这类的b,c
    AST_OP_others,

    /// @brief 数组维数
    AST_OP_ARRAY_PARAMS,

    ///	@brief 数组
    AST_OP_ARRAY,

};

/// @brief 抽象语法树AST的节点描述类
class ast_node {
public:

	/// @brief 维数，不是数组是长度恒为0
    std::vector<int> dims;
    /// @brief 父节点
    ast_node * parent;

    /// @brief 孩子节点
    std::vector<ast_node *> sons;

    /// @brief 节点类型
    ast_operator_type node_type;

    /// @brief 行号信息，主要针对叶子节点有用
    uint32_t line_no;

    /// @brief 节点值的类型，可用于函数返回值类型
    ValueType type;

    /// @brief 无符号整数字面量值
    uint32_t integer_val;

    /// @brief float类型字面量值
    float float_val;

	/// @brief boolean类型字面量值
    char bool_val;

    /// @brief 变量名，或者函数名
    std::string name;

    /// @brief 线性IR指令块，可包含多条IR指令，用于线性IR指令产生用
    InterCode blockInsts;

	/// @brief 线性IR指令块，只包括定义部分
	InterCode DeclareBlock;

	/// @brief 线性IR指令块，除定义部分以外其他部分
	InterCode OtherBlock;

    /// @brief 线性IR指令或者运行产生的Value，用于线性IR指令产生用
    Value * val;

	/// @brief 指针的偏移量
	int pointer_offset = 0;

	/// @brief label指针
	Label * label;

	///	@brief 是否为变量 variable
	int is_variable = 0;

	/// @brief 是否为全局
	int is_global = 0;

	/// @brief 是否为常量
	int is_const = 0;

    /// @brief 构造函数
    /// @param _type 节点值的类型
    /// @param line_no 行号
    ast_node(ValueType _type, int32_t _line_no);

    /// @brief 针对无符号整数字面量的构造函数
    /// @param attr 无符号整数字面量
    ast_node(digit_int_attr attr);

    /// @brief 针对float字面量的构造函数
    /// @param attr float型实数字面量
    ast_node(digit_real_attr attr);

	/// @brief 针对bool字面量的构造函数
    /// @param attr bool型实数字面量
    ast_node(digit_bool_attr attr);

    /// @brief 针对标识符ID的叶子构造函数
    /// @param attr 字符型标识符
    ast_node(var_id_attr attr);

    /// @brief 创建指定节点类型的节点
    /// @param _node_type 节点类型
    /// @param _line_no 行号
    ast_node(ast_operator_type _node_type, int32_t _line_no = -1);
};

/// @brief 判断是否是叶子节点
/// @param type 节点类型
/// @return true：是叶子节点 false：内部节点
bool isLeafNode(ast_operator_type type);

/// @brief 创建指定节点类型的节点
/// @param type 节点类型
/// @param  可变参数，最后一个孩子节点必须指定为nullptr。如果没有孩子，则指定为nullptr
/// @return 创建的节点
ast_node * new_ast_node(ast_operator_type type, ...);

/// @brief 向父节点插入一个节点
/// @param parent 父节点
/// @param node 节点
ast_node * insert_ast_node(ast_node * parent, ast_node * node);

/// @brief 创建无符号整数的叶子节点
/// @param val 词法值
/// @param line_no 行号
ast_node * new_ast_leaf_node(digit_int_attr attr);

/// @brief 创建实数的叶子节点
/// @param val 词法值
/// @param line_no 行号
ast_node * new_ast_leaf_node(digit_real_attr attr);

/// @brief 创建标识符的叶子节点
/// @param val 词法值
/// @param line_no 行号
ast_node * new_ast_leaf_node(var_id_attr attr);

/// @brief 创建bool的叶子节点
/// @param val bool值
/// @param line_no 行号
ast_node * new_ast_leaf_node(digit_bool_attr attr);


/// @brief 创建具备指定类型的节点
/// @param type 节点值类型
/// @param line_no 行号
/// @return 创建的节点
ast_node * new_ast_leaf_node(BasicType type, int32_t line_no);

/// @brief AST资源清理
void free_ast();

/// @brief抽象语法树的根节点指针
extern ast_node * ast_root;

/// @brief 创建函数定义类型的内部AST节点
/// @param line_no 行号
/// @param func_name 函数名
/// @param block 函数体语句块
/// @param params 函数形参，可以没有参数
/// @return 创建的节点
/// @return 创建的节点
ast_node * create_func_def(uint32_t line_no, const char * func_name, ast_node * block, ast_node * params = nullptr,int32_t return_type = 0);

/// @brief 创建函数形式参数的节点
/// @param line_no 行号
/// @param param_name 参数名
/// @param params_type 参数类型
/// @return 创建的节点
ast_node * create_func_formal_param(uint32_t line_no, const char * param_name , int32_t param_type = 1);

/// @brief 创建AST的内部节点
/// @param node_type 节点类型
/// @param first_param 第一个孩子节点
/// @return 创建的节点
ast_node * create_contain_node(ast_operator_type node_type, ast_node * first_param = nullptr);

/// @brief 创建函数调用的节点
/// @param line_no 行号
/// @param func_name 被调用的函数名
/// @param params 实参节点
/// @return 创建的节点
ast_node * create_func_call(uint32_t line_no, const char * func_name, ast_node * params = nullptr);
