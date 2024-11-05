/**
 * @file IRInst.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief IR指令类的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <vector>

#include "Value.h"
#include "Label.h"

/// @brief IR指令操作码
enum class IRInstOperator {

    /// @brief 函数入口指令，对应函数的prologue，用户栈空间分配、寄存器保护等
    IRINST_OP_ENTRY,

    /// @brief 函数出口指令，对应函数的epilogue，用于栈空间的恢复与清理、寄存器恢复等
    IRINST_OP_EXIT,

    /// @brief Label指令，用于语句的跳转
    IRINST_OP_LABEL,

    /// @brief 无条件跳转指令
    IRINST_OP_GOTO,

    /// @brief 整数的加法指令，二元运算
    IRINST_OP_ADD_I,

    /// @brief 整数的减法指令，二元运算
    IRINST_OP_SUB_I,

	/// @brief 整数的乘法指令，二元运算
    IRINST_OP_MUL_I,

	/// @brief 整数的除法指令，二元运算
    IRINST_OP_DIV_I,

	/// @brief 	取模指令，二元运算
	IRINST_OP_MOD,

	/// @brief 整数的逻辑或指令，二元运算
	IRINST_OP_OR,

	/// @brief 整数的逻辑与指令，二元运算
	IRINST_OP_AND,

	/// @brief 整数的逻辑非指令，二元运算
	IRINST_OP_NOT,

    /// @brief 赋值指令，一元运算
    IRINST_OP_ASSIGN,

	/// @brief 整数的取负运算，一元运算
	IRINST_OP_NEG_I,

    /// @brief 函数调用，多目运算，个数不限
    IRINST_OP_FUNC_CALL,

    /* 后续可追加其他的IR指令 */
    /// @brief 函数定义:
	IRINST_OP_FUNC_DEFINE,

	/// @brief 整数比较
	IRINST_OP_ICMP,

	/// @brief 整数扩展
	IRINST_OP_EXT,

	/// @brief 函数声明:
	IRINST_OP_FUNC_DECLARE,
    /// @brief 最大指令码，也是无效指令
    IRINST_OP_MAX,
	///@brief 赋值指令
	IRINST_OP_DECLARE,
	///@brief 赋值指令
	IRINST_OP_LOAD,
	/// @brief 仅仅只有if
	IRINST_OP_ONLY_IF,
	/// @brief 跳转指令
	IRINST_OP_JUMP,

	/// @brief memset函数
	IRINST_OP_MEMSET,

	/// @brief getelementptr函数
	IRINST_OP_GETELEMENTPTR,

	/// @brief global declare 
	IRINST_OP_GLOBAL_DECLARE,
};

/// @brief 比较条件,有符号
enum class predicate{
	EQ,
	NE,
    SLT,
    SLE,
    SGT,
    SGE,
};

/// @brief 扩展模式
enum class extend_mode{
    ZEXT,
    SEXT,
    TRUNC,
};

/// @brief 跳转模式
enum class jump_mode{
	hard_jump,
	conditional_jump,
};

enum class func_type {
	memset,
	getelementptr,
};

/// @brief 单元运算符种类
enum class unary_mode{
	Xor,
	Neg,
};

/// @brief IR指令的基类
class IRInst {

public:
    /// @brief 构造函数
    IRInst();

    /// @brief 构造函数
    /// @param op
    /// @param result
    IRInst(IRInstOperator op, Value * result = nullptr);

    /// @brief 析构函数
    virtual ~IRInst() = default;

    /// @brief 获取指令操作码
    /// @return 指令操作码
    IRInstOperator getOp();

    /// @brief 获取源操作数列表
    /// @return 源操作数列表
    std::vector<Value *> & getSrc();

    /// @brief 获取目的操作数，或者结果操作数
    /// @return 目的操作数，或者结果操作数
    Value * getDst();

    /// @brief 取得源操作数1
    /// @return 源操作数1
    Value * getSrc1();

    /// @brief 取得源操作数1的寄存器号
    /// @return 寄存器号，可能为-1，表示在内存或立即数
    int getSrc1RegId();

    /// @brief 取得源操作数2
    /// @return 源操作数2
    Value * getSrc2();

    /// @brief 取得源操作数2的寄存器号
    /// @return 寄存器号，可能为-1，表示在内存或立即数
    int getSrc2RegId();

    /// @brief 转换成字符串
    virtual void toString(std::string & str);

    /// @brief 是否是Dead指令
    bool isDead()
    {
        return dead;
    }

    /// @brief 设置指令是否是Dead指令
    /// @param _dead 是否是Dead指令，true：Dead, false: 非Dead
    void setDead(bool _dead = true)
    {
        dead = _dead;
    }

    /// @brief 获取Label指令的命令
    /// @return Label名字
    std::string getLabelName()
    {
        return labelName;
    }

	/// @brief 设置返回值
    /// @param 设置返回值
    bool setdstValue(Value * value);

public:
    /// @brief IR指令操作码
    enum IRInstOperator op;

    /// @brief 源操作树
    std::vector<Value *> srcValues;

    /// @brief 目的操作数或结果或跳转指令的目标
    Value * dstValue;

    /// @brief 是否是Dead指令
    bool dead = false;

    /// @brief Label指令的名字
    std::string labelName;

    /// @brief 目标真出口指令，指向Label指令，主要用于有条件跳转
    IRInst * trueInst;

    /// @brief 目标假出口指令，指向Label指令，主要用于有条件跳转
    IRInst * falseInst;
};

/// @brief Label指令
class LabelIRInst : public IRInst {

public:
    /// @brief 构造函数
    LabelIRInst();

	/// @brief create a if 

    /// @brief 构造函数
    /// @param name Label名字，要确保函数内唯一
    LabelIRInst(std::string name);

    /// @brief 析构函数
    virtual ~LabelIRInst();

    /// @brief 转换成字符串
    /// @param str 返回指令字符串
    void toString(std::string & str) override;

protected:
    /// @brief 新建Label名字，目前采用全局唯一编号，实际上函数内唯一即可
    /// \return Label名字
    static std::string createLabelName()
    {
        static uint64_t tempLabelCount = 0; // 常量计数，默认从0开始

        return "L" + std::to_string(tempLabelCount++);
    }
};

/// @brief 单元运算指令
class OddIRInst : public IRInst {

public:
    /// @brief 构造函数
    /// @param _op 操作符
    /// @param _result 结果操作数
    /// @param _srcVal1 源操作数1
    OddIRInst(IRInstOperator _op, Value * _result, Value * _srcVal1);

    /// @brief 析构函数
    virtual ~OddIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;

    /// @brief  返回单元运算符种类
    /// @return 返回单元运算符种类
    unary_mode getmode();

    /// @brief 单元运算符的种类
    unary_mode UnaryMode;
};

/// @brief 二元运算指令
class BinaryIRInst : public IRInst {

public:
    /// @brief 构造函数
    /// @param _op 操作符
    /// @param _result 结果操作数
    /// @param _srcVal1 源操作数1
    /// @param _srcVal2 源操作数2
    BinaryIRInst(IRInstOperator _op, Value * _result, Value * _srcVal1, Value * _srcVal2);

    /// @brief 析构函数
    virtual ~BinaryIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};

/// @brief 函数调用指令
class FuncCallIRInst : public IRInst {

public:
    /// @brief 函数名
    std::string name;

public:
    /// @brief 无参数并且没有要设置的返回值的函数调用
    /// @param name 函数名
    FuncCallIRInst(std::string _name);

    /// @brief 含有参数的函数调用
    /// @param _srcVal1 函数的实参Value
    /// @param result 保存返回值的Value
    FuncCallIRInst(std::string _name, Value * _srcVal1, Value * _result = nullptr);

    /// @brief 含有参数的函数调用
    /// @param srcVal 函数的实参Value
    /// @param result 保存返回值的Value
    FuncCallIRInst(std::string _name, std::vector<Value *> & _srcVal, Value * _result = nullptr);

    /// @brief 析构函数
    virtual ~FuncCallIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};

/// @brief 赋值指令或者说复制指令
class AssignIRInst : public IRInst {

public:
    /// @brief 构造函数
    /// @param result
    /// @param srcVal1
    AssignIRInst(Value * result, Value * srcVal1);


    /// @brief 析构函数
    virtual ~AssignIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};

class EntryIRInst : public IRInst {

public:
    /// @brief return语句指令
    EntryIRInst();

    /// @brief 析构函数
    virtual ~EntryIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};

/// @brief return语句指令
class ExitIRInst : public IRInst {

public:
    /// @brief return语句指令
    /// @param _result 返回结果值
	/// @param pattern 选择返回语句类型
	/// @param _instantNumber 立即数
    ExitIRInst(Value * result = nullptr,int pattern = 0,int _instantNumber = 0);

    /// @brief 析构函数
    virtual ~ExitIRInst() override;

    /// @brief 转换成字符串
	/// @param pattern 选择返回语句类型
    void toString(std::string & str) override;

	/// @brief 获取返回类型模式
	/// @return 返回类型模式
	int getPattern()
	{
		return pattern;
	}

protected:
	/// @brief 返回类型模式
	int pattern;
public:
    /// @brief 立即数
    int instantNumber;
};

class GotoIRInst : public IRInst {

public:
    /// @brief return语句指令
    /// @param target 跳转目标
    GotoIRInst(IRInst * target);

    /// @brief 析构函数
    virtual ~GotoIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};

// TODO:实现构造部分
class DeclareIRInst : public IRInst {
public:
	/// @brief 构造函数
	/// @brief _op操作符
    /// @param result
	/// @param _size 分配大小
    DeclareIRInst(IRInstOperator _op,Value * _result,BasicType _type,u_int32_t _size);

    std::string to_str(std::vector<int> dims, int id);	
    /// @brief 构造函数
    /// @brief _op操作符
    /// @param result
    /// @param _dim 维数
    DeclareIRInst(IRInstOperator _op,Value * _result,BasicType _type,std::vector<int> _dim);

    /// @brief 析构函数
    virtual ~DeclareIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;

	/// @brief 返回定义的偏移量
	/// @return 返回定义的偏移量
    u_int32_t getSize();

protected:
	/// @brief分配大小
    int size;

	/// @brief 声明类型
    BasicType type;

	/// @brief 维数
    std::vector<int> DIMS;
};

// load部分
class LoadIRInst : public IRInst {
public:
	/// @brief 构造函数
    /// @param result
    LoadIRInst(Value * _result, Value * _srcVal1);

    /// @brief 析构函数
    virtual ~LoadIRInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};

// block边界部分
class BlockborderInst : public IRInst {
public:
	/// @brief 构造函数
    /// @param _pattern
    BlockborderInst(int _pattern);

    /// @brief 析构函数
    virtual ~BlockborderInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;

protected:
	int pattern;
};


// 整数比较部分
class ICMPInst : public IRInst {
public:
	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _result 结果操作数
    /// @param _srcVal1 源操作数1
    /// @param _srcVal2 源操作数2
	/// @param _predicate 比较方式
    ICMPInst(IRInstOperator _op, 
			Value * _result, 
			Value * _srcVal1, 
			Value * _srcVal2,
			predicate _predicate);

    /// @brief 析构函数
    virtual ~ICMPInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
public:
	predicate predicate_type;
};

// 整数比较部分
class ExtInst : public IRInst {
public:
	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _result 结果操作数
    /// @param _srcVal1 源操作数1
	/// @param _extend_mode 扩展模式
    ExtInst(IRInstOperator _op, 
			Value * _result, 
			Value * _srcVal1, 
			extend_mode _extend_mode);

    /// @brief 析构函数
    virtual ~ExtInst() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;
protected:
	extend_mode extend_mode_;
};

/// @brief	强制跳转
class BrInst : public IRInst {
public:
	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _srcLabel1 源操作数label1
	/// @param _jump_mode 跳转模式
	/// @param _srcLabel2 源操作数label2
	/// @param _condition 跳转条件
    BrInst(IRInstOperator _op, 
			Label * _srcLabel1,
			jump_mode  _jump_mode = jump_mode::hard_jump,
			Label * _srcLabel2 = nullptr,
			Value * _condition = nullptr
		  );

    /// @brief 析构函数
    virtual ~BrInst() override;

	///	@brief 是否是条件跳转
    bool is_Condition();

    /// @brief 转换成字符串
    void toString(std::string & str) override;

	

public:
	Label * TOGO_Label;
	Label * Option_Label;
	jump_mode jump_mode_;
	Value * condition;
};

/// @brief 一些llvm内部的函数
class LLVM_FUNC_INST : public IRInst{
public:
	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _srcValue1 参数名
	/// @param _func_type 函数类型
	/// @param _size memset专用
    LLVM_FUNC_INST(IRInstOperator _op,
                                   std::string _srcValue1,
                                   func_type _func_type,
                                   u_int32_t _size);

	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _srcValue1 参数名
	/// @param _func_type 函数类型
	/// @param _size 专用
    LLVM_FUNC_INST(IRInstOperator _op,
                       std::string _SrcValue1 ,
                       func_type _func_type,
                       u_int32_t _size,
					   std::string _srcValue2 ,
					   std::string _srcValue3 );


    /// @brief 析构函数
    virtual ~LLVM_FUNC_INST() override;

    /// @brief 转换成字符串
    void toString(std::string & str) override;

    protected:
        func_type FUNC_TYPE;

        u_int32_t size;

        std::string arg1;

        std::string arg2;

		std::string arg3;
};

class GLOBAL_DECLARE_Inst : public IRInst{
public:
	/// @brief 构造函数
	/// @param _op  操作符
	/// @param val 开辟变量的名字
	/// @param value 开辟变量的值，默认为0
	/// @param _B 开辟变量的类型
	/// @param size 要为变量开辟的空间
	/// @param _dims 维数空间
    
	GLOBAL_DECLARE_Inst(IRInstOperator _op, 
	Value * val,
	int32_t _value,
	bool _is_const = false,
	BasicType _B = BasicType::TYPE_INT,
	uint32_t _size=4,
	std::vector<int> _dims = std::vector<int>()
	);


    /// @brief 析构函数
    virtual ~GLOBAL_DECLARE_Inst() override;

	/// @brief 转换成字符串
    void toString(std::string & str) override;

public:
    /// @brief 	全局变量类型
    BasicType B;
	/// @brief 是否为常量
	bool is_const;
	/// @brief 全局变量大小
	int32_t size;
    /// @brief 值
    u_int32_t value;
	/// @brief 维数空间
    std::vector<int> dims;
	/// @brief 数组内容
	std::vector<int> array_values;
};
