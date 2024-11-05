/**
 * @file function.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 函数形参与函数管理的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "IRCode.h"
#include "Value.h"
#include "Label.h"

class SymbolTable;

/// @brief 描述函数形参类
class FuncFormalParam {

public:
    /// @brief 参数名字，可为空
    std::string name;

    /// @brief 形参类型
    ValueType type;

    /// @brief 形参变量
    Value * val = nullptr;

    /// @brief 默认整型参数
    /// @param _name 形参的名字
    FuncFormalParam(std::string _name);

    /// @brief 基本类型的参数
    /// @param _name 形参的名字
    /// @param _type 基本类型
    /// @param _val Value
    FuncFormalParam(std::string _name, BasicType _type, Value * _val = nullptr);

    /// @brief 输出字符串
    /// @param str
    std::string toString();
};

/// @brief 描述函数信息的类
class Function {

public:
    /// @brief 匿名构造函数
    Function();

    /// @brief 指定有函数类型与名字的构造函数
    /// @param name
    /// @param type
    Function(std::string _name, BasicType _type = BasicType::TYPE_INT, bool _builtin = false);

    /// @brief 指定函数名字、函数返回类型以及函数形式参数的构造函数
    /// @param _name
    /// @param _type
    /// @param _param
    Function(std::string _name, BasicType _type, FuncFormalParam _param, bool _builtin = false);

    /// @brief 取得函数名字
    /// @return 函数名字
    std::string & getName();

    /// @brief 获取函数返回类型
    /// @return 返回类型
    ValueType & getReturnType();

    /// @brief 获取函数的形参列表
    /// @return 形参列表
    std::vector<FuncFormalParam> & getParams();

    /// @brief 获取函数内的IR指令代码
    /// @return IR指令代码
    InterCode & getInterCode();

    /// @brief 判断该函数是否是内置函数
    /// @return true: 内置函数，false：用户自定义
    bool isBuiltin();

    /// @brief 设置函数是否是内置或用户自定义
    /// @param _builtin true: 内置函数 false: 用户自定义函数
    void setBuiltin(bool _builtin);

    /// @brief 函数指令信息输出
    /// @param str 函数指令
    void toString(std::string & str);

    /// @brief 获取下一个Label名字
    /// @return 下一个Label名字
    std::string getNextLabelName();

    /// @brief 获取下一个临时变量名字
    /// @return 下一个临时变量名字
    std::string getNextTempVarName();

    /// @brief 设置函数出口指令
    /// @param inst 出口Label指令
    void setExitLabel(Label * _exitLabel);

    /// @brief 获取函数出口指令
    /// @return 出口Label指令
    Label * getExitLabel();

    /// @brief 设置函数返回值变量
    /// @param val 返回值变量，要求必须是局部变量，不能是临时变量
    void setReturnValue(Value * val);

    /// @brief 获取函数返回值变量
    /// @return 返回值变量
    Value * getReturnValue();

    /// @brief 从函数内的局部变量中删除
    /// @param val 变量Value
    void deleteVarValue(Value * val);

    /// @brief 获取函数内变量清单
    /// @return 函数内变量清单
    std::vector<Value *> & getVarValues()
    {
        return varsVector;
    }

    /// @brief 获取最大栈帧深度
    /// @return 栈帧深度
    int getMaxDep();

    /// @brief 设置最大栈帧深度
    /// @param dep 栈帧深度
    void setMaxDep(int dep);

    /// @brief 获取函数调用栈空间大小而引入的栈空间大小
    /// @return 栈空间大小
    int getExtraStackSize();

    /// @brief 设置函数调用栈空间大小而引入的栈空间大小
    /// @param size 栈空间大小
    void setExtraStackSize(int size);

    /// @brief 获取函数调用参数个数的最大值
    /// @return 函数调用参数个数的最大值
    int getMaxFuncCallArgCnt();

    /// @brief 设置函数调用参数个数的最大值
    /// @param count 函数调用参数个数的最大值
    void setMaxFuncCallArgCnt(int count);

    /// @brief 函数内是否存在函数调用
    bool getExistFuncCall();

    /// @brief 设置函数是否存在函数调用
    /// @param exist true: 存在 false: 不存在
    void setExistFuncCall(bool exist);

    /// @brief 获取本函数需要保护的寄存器
    /// @return 要保护的寄存器
    std::vector<int32_t> & getProtectedReg();

    /// @brief 获取本函数需要保护的寄存器字符串
    /// @return 要保护的寄存器
    std::string & getProtectedRegStr();

    /// 根据变量名取得当前符号的值。若变量不存在，则说明变量之前没有定值，则创建一个未知类型的值，初值为0
    /// \param name 变量名
    /// \param create true: 不存在返回nullptr；false：不存在则不创建
    /// \return 变量对应的值
    Value * findValue(std::string name, bool create = false);

    /// @brief 新建变量型Value
    /// @param name 变量ID
    /// @param type 变量类型
    Value * newVarValue(std::string name, BasicType type = BasicType::TYPE_INT);

    /// @brief 新建变量型Value
    /// @param type 变量类型
    Value * newVarValue(BasicType type);

    /// @brief 新建一个临时型的Value，并加入到符号表，用于后续释放空间
    /// \param type 变量类型
    /// \return 临时变量Value
    Value * newTempValue(BasicType type);

    /// @brief 设置符号表，以便全局符号查找
    void setSymtab(SymbolTable * _symtab);

	/// @brief 重制临时编号名称
	void resetTempNo();

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

protected:
    /// @brief Value插入到符号表中
    /// @param val Value信息
    void insertValue(Value * val);

private:
    /// @brief 函数名
    std::string name;

    /// @brief 函数返回类型
    ValueType returnType;

    /// @brief 形式参数列表
    std::vector<FuncFormalParam> params;

    // @brief 是否是内置函数
    bool builtIn = false;

public:
    /// @brief 线性IR指令块，可包含多条IR指令
    InterCode code;

private:
    /// @brief 全局符号表
    SymbolTable * symtab = nullptr;

    /// @brief 下一个可用的临时变量编号，用于输出中间代码用
    int32_t nextTempVarNo = 0;

    /// @brief 下一个可用的Label编号，用户输出中间代表用
    int32_t nextLabelNo = 0;

    /// @brief 函数内变量的映射表，提高查询的效率
    std::unordered_map<std::string, Value *> varsMap;

    /// @brief 函数内变量的向量表
    std::vector<Value *> varsVector;

    /// @brief 函数出口Label指令
    Label * exitLabel = nullptr;

    /// @brief 函数返回值变量，不能是临时变量，必须是局部变量
    Value * returnValue = nullptr;

    /// @brief 由于局部变量、前4个形参需站内空间分配而导致的栈帧大小
    int maxDepth = 0;

    /// @brief 由于函数调用需要栈传递而导致的栈空间大小
    int maxExtraStackSize = 0;

    /// @brief 是否存在函数调用
    bool funcCallExist = false;

    /// @brief 本函数内函数调用的参数个数最大值
    int maxFuncCallArgCnt = 0;

    /// @brief 函数是否需要重定位，栈帧发生变化
    bool relocated = false;

    /// @brief 被保护的寄存器编号
    std::vector<int32_t> protectedRegs;

    /// @brief 被保护寄存器字符串
    std::string protectedRegStr;

	///@brief 每个函数内部变量的寄存器/变量名编号
	unsigned int function_temp_variable = 0;

	/// @brief 标签列表(限定函数内部)
	std::vector<Label *> FUN_labelVector;

	/// @brief 标签映射表
	std::unordered_map<std::string, Label *> FUN_labelMap;
};
