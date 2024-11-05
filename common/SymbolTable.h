/**
 * @file symboltable.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 符号表管理：变量、函数等管理的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <unordered_map>
#include <vector>

#include "Function.h"
#include "IRCode.h"
#include "Value.h"
#include "Label.h"

class Function;

/// @brief 符号表管理类
class SymbolTable {

public:
    /// @brief 构造函数
    SymbolTable();

    /// @brief 根据函数名查找函数信息
    /// @param name 函数名
    /// @return 函数信息
    Function * findFunction(std::string name);

    /// @brief 插入函数。如果函数存在，则返回false，否则返回true
    /// @param func 函数信息
    /// @return true: 可以插入函数 false: 不能插入函数
    bool insertFunction(Function * func);

    /// @brief 移动到函数列表的尾部
    /// @param func
    void moveFunctionEnd(Function * func);

    /// @brief 输出线性IR指令列表
    /// @param filePath
    void outputIR(const std::string & filePath);

    /// @brief 获得函数列表
    std::vector<Function *> & getFunctionList()
    {
        return funcVector;
    }

    /// @brief 根据变量名获取当前符号（只管理全局变量）
    /// \param name 变量名
    /// \param create 变量查找不到时若为true则自动创建，否则不创建
    /// \return 变量对应的值
    Value * findValue(std::string name, bool create = false);

    /// @brief 新建变量型Value，全局变量
    /// @param name 变量ID
    /// @param type 变量类型
    Value * newVarValue(std::string name, 
	BasicType type = BasicType::TYPE_INT,
	bool is_global = false,
	bool is_const = false);

    /// @brief 新建一个整型数值的Value，并加入到符号表，用于后续释放空间
    /// \param intVal 整数值
    /// \return 临时Value
    Value * newConstValue(int32_t intVal);

    /// @brief 新建一个实数数值的Value，并加入到符号表，用于后续释放空间
    /// \param intVal 整数值
    /// \return 临时Value
    Value * newConstValue(float realVal);

    /// @brief 清理注册的所有Value资源
    void freeValues();

    /// @brief 新建函数并放到函数列表中
    /// @param name 函数名
    /// @param returnType 返回值类型
    /// @param params 形参列表
    /// @param builtin 是否内置函数
    /// @return 新建的函数对象实例
    Function *
    newFunction(std::string name, BasicType returnType, std::vector<FuncFormalParam> params, bool builtin = false);

    /// @brief 新建函数并放到函数列表中
    /// @param name 函数名
    /// @param returnType 返回值类型
    /// @param builtin 是否内置函数
    /// @return 新建的函数对象实例
    Function * newFunction(std::string name, BasicType returnType, bool builtin = false);
 
	/*
	///	@brief 创建新的标签并放到标签列表中
	/// @param name 标签名
	/// @param _Label_Type 标签类型（可选）
	/// @return 新建的标签对象实例
	Label * newLabel(std::string name, Label_Type _Label_Type = Label_Type::NORMAL);

	/// @brief 插入标签。如果标签存在，则返回false，否则返回true
    /// @param func 函数信息
    /// @return true: 可以插入函数 false: 不能插入函数
	bool insertLabel(Label * lab);

	/// @brief 根据函数名查找函数信息
    /// @param name 函数名
    /// @return 函数信息
    Label * findLabel(std::string name);
	*/


protected:
    /// @brief Value插入到符号表中
    /// @param val Value信息
    void insertValue(Value * val);

public:
    /// @brief main函数
    Function * mainFunc = nullptr;

    /// 遍历抽象树过程中的当前处理函数
    Function * currentFunc = nullptr;

	
private:
    /// @brief 变量名映射表，变量名-变量，只保存全局变量以及常量
    std::unordered_map<std::string, Value *> varsMap;

    /// @brief 只保存全局变量以及常量
    std::vector<Value *> varsVector;

    /// @brief 函数映射表，函数名-函数，便于检索
    std::unordered_map<std::string, Function *> funcMap;
public:
    /// @brief  函数列表
    std::vector<Function *> funcVector;

	/// @brief 
    InterCode global_inst;

private:
	/// @brief 标签列表
	std::vector<Label *> labelVector;

	/// @brief 标签映射表
	std::unordered_map<std::string, Label *> labelMap;
};
