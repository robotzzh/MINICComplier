/**
 * @file symboltable.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 符号表管理：变量、函数等管理
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "Function.h"
#include "SymbolTable.h"
#include "Value.h"
#include "ValueType.h"
#include "Label.h"

/// @brief 构造函数
SymbolTable::SymbolTable()
{
    // 加入内置函数
    (void) newFunction("putint",                    // 函数名
                       BasicType::TYPE_VOID,        // 返回值类型
                       {{"", BasicType::TYPE_INT}}, // 形参类型
                       true                         // 内置函数
    );
}

/// @brief 根据函数名查找函数信息
/// @param name 函数名
/// @return 函数信息
Function * SymbolTable::findFunction(std::string name)
{
    // 根据名字查找
    auto pIter = funcMap.find(name);
    if (pIter != funcMap.end()) {
        // 查找到
        return pIter->second;
    }

    return nullptr;
}

/// @brief 插入函数。如果函数存在，则返回false，否则返回true
/// @param func 函数信息
/// @return true: 可以插入函数 false: 不能插入函数
bool SymbolTable::insertFunction(Function * func)
{
    bool result = false;
    std::string name;
    name = func->getName();

    auto pFunction = findFunction(name);
    if (pFunction == nullptr) {
        // 该函数不存在，则加入到函数中

        funcMap.insert({name, func});
        funcVector.emplace_back(func);

        result = true;
    }

    return result;
}

/// @brief 移动到函数列表的尾部
/// @param func
void SymbolTable::moveFunctionEnd(Function * func)
{
    // 在向量中查找函数
    auto pIter = std::find(funcVector.begin(), funcVector.end(), func);
    if (pIter != funcVector.end()) {
        // 找到了，则从列中中删除，并移动到尾部
        funcVector.erase(pIter);
        funcVector.push_back(func);
    }
}

/// @brief 文本输出线性IR指令
/// @param filePath 输出文件路径
void SymbolTable::outputIR(const std::string & filePath)
{
    // 这里使用C的文件操作，也可以使用C++的文件操作

    FILE * fp = fopen(filePath.c_str(), "w");
    if (nullptr == fp) {
        printf("fopen() failed\n");
        return;
    }
	for(auto inst: global_inst.getInsts()){
		std::string instStr;
        inst->toString(instStr);
        instStr += "\n";
        fprintf(fp, "%s", instStr.c_str());
    }

    // 遍历所有的线性IR指令，文本输出
    for (auto func: funcVector) {

        std::string instStr;
        func->toString(instStr);
        fprintf(fp, "%s", instStr.c_str());
    }

    fclose(fp);
}

/// @brief 新建全局变量型Value
/// @param name 变量ID
/// @param type 变量类型
Value * SymbolTable::newVarValue
	(std::string name, 
	BasicType type,
	bool is_global ,
	bool is_const )
{
    Value * retVal = nullptr;

    auto pIter = varsMap.find(name);
    if (pIter != varsMap.end()) {

        // 已存在的Value
        retVal = pIter->second;

        // 符号表中存在，则只是更新值
        pIter->second->type = type;

        if (type == BasicType::TYPE_INT) {
            pIter->second->intVal = 0;
        } else {
            pIter->second->realVal = 0;
        }
    } else {
		if(!is_global && !is_const)
		{
        	retVal = new VarValue(name, type);
		}
        insertValue(retVal);
    }

    return retVal;
}

/// @brief Value插入到符号表中
/// @param name Value的名称
/// @param val Value信息
void SymbolTable::insertValue(Value * val)
{
    varsMap.emplace(val->name, val);
    varsVector.push_back(val);
}

/// @brief 新建一个整型数值的Value，并加入到符号表，用于后续释放空间
/// @param intVal 整数值
/// @return 常量Value
Value * SymbolTable::newConstValue(int32_t intVal)
{
    // 整数变字符串
    std::string name = std::to_string(intVal);

    // 查找整数字符串
    Value * val = findValue(name, false);
    if (!val) {

        // 不存在，则创建整数常量Value
        val = new ConstValue(intVal);
        insertValue(val);
    }

    return val;
}

/// @brief 新建一个实数数值的Value，并加入到符号表，用于后续释放空间
/// @param realVal 浮点数值
/// @return 常量Value
Value * SymbolTable::newConstValue(float realVal)
{
    // 浮点数变字符串
    std::string name = std::to_string(realVal);

    // 查找常量
    Value * val = findValue(name, false);
    if (!val) {

        // 不存在，则新建
        val = new ConstValue(realVal);
        insertValue(val);
    }

    return val;
}

/// @brief 根据变量名获取当前符号(只管理全局变量和常量)
/// @param name 变量名或者常量名
/// @param create 变量查找不到时若为true则自动创建变量型Value，否则不创建
/// @return 变量对应的值
Value * SymbolTable::findValue(std::string name, bool create)
{
    Value * temp = nullptr;

    auto pIter = varsMap.find(name);
    if (pIter == varsMap.end()) {

        // 变量名没有找到
        if (create) {
            temp = newVarValue(name);
        }
    } else {
        temp = pIter->second;
    }

    return temp;
}

/// @brief 清理注册的所有Value资源
void SymbolTable::freeValues()
{
    // 清理Vector表中的所有创建的Value
    for (auto var: varsVector) {
        delete var;
    }

    // 清空
    varsVector.clear();
}

/// @brief 新建函数并放到函数列表中
/// @param name 函数名
/// @param returnType 返回值类型
/// @param params 形参列表
/// @param builtin 是否内置函数
/// @return 新建的函数对象实例
Function *
SymbolTable::newFunction(std::string name, BasicType returnType, std::vector<FuncFormalParam> params, bool builtin)
{
    Function * func = new Function(name, returnType, builtin);
    func->getParams().assign(params.begin(), params.end());
    insertFunction(func);

    return func;
}

/// @brief 新建函数并放到函数列表中
/// @param name 函数名
/// @param returnType 返回值类型
/// @param builtin 是否内置函数
/// @return 新建的函数对象实例
Function * SymbolTable::newFunction(std::string name, BasicType returnType, bool builtin)
{
    Function * func = new Function(name, returnType, builtin);
    insertFunction(func);
    return func;
}




///	@brief 创建新的标签并放到标签列表中
/// @param name 标签名
/// @param _Label_Type 标签类型（可选）
/// @return 新建的标签对象实例
/*
Label * SymbolTable::newLabel(std::string name, 
							Label_Type _Label_Type)
							{
								Label * label;
								if (_Label_Type ==  Label_Type::NORMAL)	label = new Label(name);
								else{
									label = new Label(_Label_Type);
								}
                                insertLabel(label);
                                return label;
							}

	/// @brief 插入标签。如果标签存在，则返回false，否则返回true
    /// @param func 标签信息
    /// @return true: 可以插入函数 false: 不能插入函数
bool SymbolTable::insertLabel(Label * lab){
	bool result = false;
    std::string name = lab->GetLabelName();

    auto pFunction = findLabel(name);
    if (pFunction == nullptr) {
        // 该函数不存在，则加入到函数中

        labelMap.insert({name, lab});
        labelVector.emplace_back(lab);

        result = true;
    }

    return result;
}

Label * SymbolTable::findLabel(std::string name){
	// 根据名字查找
    auto pIter = labelMap.find(name);
    if (pIter != labelMap.end()) {
        // 查找到
        return pIter->second;
    }

    return nullptr;
}
*/