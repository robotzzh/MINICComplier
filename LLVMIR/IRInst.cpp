/**
 * @file IRInst.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief IR指令类
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "IRInst.h"
#include "Value.h"
#include "Label.h"

static std::string array_dims(std::vector<int> dims,int id){
    if(id == dims.size()-1) 
	return "["+std::to_string(dims[id])+" x i32]";
    return "["+std::to_string(dims[id])+" x "+array_dims(dims, id+1)+"]";
}

/// @brief 构造函数
IRInst::IRInst()
{
    // 未知指令
    op = IRInstOperator::IRINST_OP_MAX;
}

/// @brief 构造函数
/// @param op
/// @param result
/// @param srcVal1
/// @param srcVal2
IRInst::IRInst(IRInstOperator _op, Value * _result) : op(_op), dstValue(_result)
{}

/// @brief 获取指令操作码
/// @return 指令操作码
IRInstOperator IRInst::getOp()
{
    return op;
}

/// @brief 获取源操作数列表
/// @return 源操作数列表
std::vector<Value *> & IRInst::getSrc()
{
    return srcValues;
}

/// @brief 获取目的操作数，或者结果操作数
/// @return 目的操作数，或者结果操作数
Value * IRInst::getDst()
{
    return dstValue;
}

/// @brief 取得源操作数1
/// @return
Value * IRInst::getSrc1()
{
    return srcValues[0];
}

/// @brief 取得源操作数1的寄存器号
/// @return 寄存器号，可能为-1，表示在内存或立即数
int IRInst::getSrc1RegId()
{
    return srcValues[0]->regId;
}

bool IRInst::setdstValue(Value * value){
	dstValue = value;
    return true;
}
/// @brief 取得源操作数2
/// @return
Value * IRInst::getSrc2()
{
    return srcValues[1];
}

/// @brief 取得源操作数2的寄存器号
/// @return 寄存器号，可能为-1，表示在内存或立即数
int IRInst::getSrc2RegId()
{
    return srcValues[1]->regId;
}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void IRInst::toString(std::string & str)
{
    // 未知指令
    str = "Unkown IR Instruction";
}

/// @brief 构造函数
LabelIRInst::LabelIRInst() : IRInst(IRInstOperator::IRINST_OP_LABEL)
{
    // TODO 这里先设置为空字符串，实际上必须是唯一的Label名字
    // 处理方式：(1) 全局唯一 (2) 函数内唯一
    labelName = createLabelName();
}

/// @brief 构造函数
/// @param name Label名字，要确保函数内唯一
LabelIRInst::LabelIRInst(std::string name) : IRInst(IRInstOperator::IRINST_OP_LABEL)
{
    labelName = name;
}

/// @brief 析构函数
LabelIRInst::~LabelIRInst()
{}

/// @brief 转换成字符串
/// @param str 返回指令字符串
void LabelIRInst::toString(std::string & str)
{
	//TODO:暂时删除
    str = labelName + ":";
}

/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal1 源操作数1
/// @param _srcVal2 源操作数2
BinaryIRInst::BinaryIRInst(IRInstOperator _op, Value * _result, Value * _srcVal1, Value * _srcVal2)
    : IRInst(_op, _result)
{
    srcValues.push_back(_srcVal1);
    srcValues.push_back(_srcVal2);
}

/// @brief 析构函数
BinaryIRInst::~BinaryIRInst()
{}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void BinaryIRInst::toString(std::string & str)
{

    Value *src1 = srcValues[0], *src2 = srcValues[1], *result = dstValue;
	std::string src1String, src2String;
	if(src1->_const)
        src1String = std::to_string(src1->intVal);
	else
        src1String = src1->toString();
    if (src2->_const)
        src2String = std::to_string(src2->intVal);
	else
        src2String = src2->toString();
    switch (op) {
        case IRInstOperator::IRINST_OP_ADD_I:

            // 加法指令，二元运算
            str = result->getName() + " = add nsw " + src1String + ", " + src2String;
            break;
        case IRInstOperator::IRINST_OP_SUB_I:

            // 减法指令，二元运算
            str = result->getName() + " = sub nsw " + src1String + ", " + src2String;
            break;

		// written by zhao 4_18
		case IRInstOperator::IRINST_OP_MUL_I:

			// 乘法指令，二元运算
			str = result->getName() + " = mul " + src1String + ", " + src2String;
			break;

		case IRInstOperator::IRINST_OP_DIV_I:

			// 除法指令，二元运算
			str = result->getName() + " = sdiv " + src1String + ", " + src2String;
			break;

		case IRInstOperator::IRINST_OP_MOD:

			// 取模指令，二元运算
			str = result->getName() + " = srem " + src1String + ", " + src2String;
            break;

        case IRInstOperator::IRINST_OP_OR:

            // 逻辑或指令，二元运算
			str = result->getName() + " = or i32 " + src1String + ", " + src2String;
			break;
		case IRInstOperator::IRINST_OP_AND:

			// 逻辑与指令，二元运算
			str = result->getName() + " = and i32 " + src1String + ", " + src2String;
			break;
        default:
            // 未知指令
            IRInst::toString(str);
            break;
    }
}

/// @brief 构造函数
/// @param _op 操作符
/// @param _result 结果操作数
/// @param _srcVal1 源操作数1
OddIRInst::OddIRInst(IRInstOperator _op, Value * _result, Value * _srcVal1)
    : IRInst(_op, _result)
{
	switch (op) {
        case IRInstOperator::IRINST_OP_NEG_I:

            // 负号指令，单元运算
            this->UnaryMode = unary_mode::Neg;
            break;
        case IRInstOperator::IRINST_OP_NOT:
			this->UnaryMode = unary_mode::Neg;
            break;
        default:
            // 未知指令
            this->UnaryMode = unary_mode::Neg;
            break;
    }
    srcValues.push_back(_srcVal1);
}

/// @brief 析构函数
OddIRInst::~OddIRInst()
{}

/// @brief  返回单元运算符种类
/// @return 返回单元运算符种类
unary_mode OddIRInst::getmode(){
    return UnaryMode;
}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void OddIRInst::toString(std::string & str)
{

    Value *src1 = srcValues[0], *result = dstValue;

    switch (op) {
        case IRInstOperator::IRINST_OP_NEG_I:

            // 负号指令，单元运算
            str = result->getName() + " = sub nsw " +src1->toString() + ", ";
            break;
		case IRInstOperator::IRINST_OP_NOT:
			str = result->getName() + " = xor i1 " +src1->getName() + ", true";
            break;
        default:
            // 未知指令
            IRInst::toString(str);
            break;
    }
}



/// @brief 无参数的函数调用
/// @param name 函数名
/// @param result 保存返回值的Value
FuncCallIRInst::FuncCallIRInst(std::string _name) : IRInst(IRInstOperator::IRINST_OP_FUNC_CALL, nullptr), name(_name)
{}

/// @brief 含有参数的函数调用
/// @param _srcVal1 函数的实参Value
/// @param result 保存返回值的Value
FuncCallIRInst::FuncCallIRInst(std::string _name, Value * _srcVal1, Value * _result)
    : IRInst(IRInstOperator::IRINST_OP_FUNC_CALL, _result), name(_name)
{
    srcValues.push_back(_srcVal1);
}

/// @brief 含有参数的函数调用
/// @param srcVal 函数的实参Value
/// @param result 保存返回值的Value
FuncCallIRInst::FuncCallIRInst(std::string _name, std::vector<Value *> & _srcVal, Value * _result)
    : IRInst(IRInstOperator::IRINST_OP_FUNC_CALL, _result), name(_name)
{
    // 实参拷贝
    srcValues = _srcVal;
}

/// @brief 析构函数
FuncCallIRInst::~FuncCallIRInst()
{}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void FuncCallIRInst::toString(std::string & str)
{
    Value * result = dstValue;

    // TODO 这里应该根据函数名查找函数定义或者声明获取函数的类型
    // 这里假定所有函数返回类型要么是i32，要么是void
    // 函数参数的类型是i32

    if (!dstValue) {

        // 函数没有返回值设置
        str = "call void @" + name + "(";
    } else {

        // 函数有返回值要设置到结果变量中
        str = result->getName() + " = call i32 @" + name + "(";
    }

    for (size_t k = 0; k < srcValues.size(); ++k) {

        str += srcValues[k]->toString();

        if (k != (srcValues.size() - 1)) {
            str += ", ";
        }
    }

    str += ")";
}

/// @brief 赋值IR指令
/// @param _result
/// @param _srcVal1
AssignIRInst::AssignIRInst(Value * _result, Value * _srcVal1) : IRInst(IRInstOperator::IRINST_OP_ASSIGN, _result)
{
    srcValues.push_back(_srcVal1);
}
/// @brief 析构函数
AssignIRInst::~AssignIRInst()
{}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void AssignIRInst::toString(std::string & str)
{
    Value *src1 = srcValues[0], *result = dstValue;

    //str = result->getName() + " = " + src1->toString();
	str = "store "+ src1->toString()+", ptr "+ result->getName() + ", align 4";
}

/// @brief return语句指令
/// @param _result 返回结果值
ExitIRInst::ExitIRInst(Value * _result,int pattern,int _instantNumber) : IRInst(IRInstOperator::IRINST_OP_EXIT, nullptr)
{
    if (_result != nullptr) {
        srcValues.push_back(_result);
    }
	instantNumber = _instantNumber;
    this->labelName = "exit";
    this->pattern = pattern;
}

/// @brief 析构函数
ExitIRInst::~ExitIRInst()
{}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void ExitIRInst::toString(std::string & str)
{
	switch(this->getPattern()){
		case 1:
			str = "ret i32 "+ std::to_string(instantNumber);
			break;
		default:
			if (srcValues.empty()) {
				str = "ret void";
			} else {
				Value * src1 = srcValues[0];
				str = "ret " + src1->toString();
			}
			break;
	}
}
/*
/// @brief return语句指令
ReturnInst::ReturnIRInst() : IRInst(IRInstOperator::IRINST_OP_RETURN, nullptr)
{
    if (_result != nullptr) {
        srcValues.push_back(_result);
    }
}

/// @brief 析构函数
ReturnIRInst::~ReturnIRInst()
{}
*/


/// @brief entry语句指令
EntryIRInst::EntryIRInst():IRInst(IRInstOperator::IRINST_OP_ENTRY){
    labelName = "Entry";
}

    /// @brief 析构函数
EntryIRInst::~EntryIRInst(){

}

/// @brief 转换成字符串
void EntryIRInst::toString(std::string & str)
{
    str = ";basic block begin \nentry: ";
}


/// @brief return语句指令
/// @param target 跳转目标
GotoIRInst::GotoIRInst(IRInst * target) : IRInst(IRInstOperator::IRINST_OP_GOTO, nullptr)
{
    // 真假目标一样，则无条件跳转
    trueInst = falseInst = target;
}

/// @brief 析构函数
GotoIRInst::~GotoIRInst()
{}

/// @brief 转换成字符串
void GotoIRInst::toString(std::string & str)
{
    str = ";basic block end goto " + trueInst->getLabelName();
}

/// @brief 构造函数
/// @param result
DeclareIRInst::DeclareIRInst(IRInstOperator _op,Value * _result,BasicType _type,u_int32_t _size):IRInst(_op, _result)
{
	type = _type;
    size = _size;
}

/// @brief 构造函数
/// @brief _op操作符
/// @param result
/// @param _dim 维数
DeclareIRInst::DeclareIRInst(IRInstOperator _op,Value * _result,BasicType _type,std::vector<int> _dim)
:IRInst(_op, _result)
{
	type = _type;
    DIMS = _dim;
}

    /// @brief 析构函数
DeclareIRInst::~DeclareIRInst() 
{}

/// @brief  特色功能
/// @param dims 
/// @param id 
/// @return 
std::string DeclareIRInst::to_str(std::vector<int> dims,int id){
    if(id == dims.size()-1) 
	return "["+std::to_string(dims[id])+" x i32]";
    return "["+std::to_string(dims[id])+" x "+to_str(dims, id+1)+"]";
}


    /// @brief 转换成字符串
void DeclareIRInst::toString(std::string & str) {
	Value *result = dstValue;
	switch(type){
		case BasicType::TYPE_INT:
            str = result->getName() + " = alloca i32, align 4 ";
            break;
        case BasicType::TYPE_FLOAT:
            str = result->getName() + " = alloca float, align 4 ";
            break;
        case BasicType::TYPE_POINTER_i32:
            str = result->getName() + " = alloca ptr, align 4 ";
            break;
		case BasicType::TYPE_ARRAY_i32:
			if(!DIMS.empty())
				str = result->getName() + " = alloca "+ to_str(DIMS,0) +", align 4 ";
            break;
        default:
            str = result->getName() + " = alloca i32, align 4 ";
            break;
	}
    //str =  result->getName()+" = alloca i32, align 4 ";
	//str = str + "\n\t;向栈中申请开辟一块内存";
}

	/// @brief 返回定义的偏移量
	/// @return 返回定义的偏移量
u_int32_t DeclareIRInst::getSize() {
    return size;
}

// load部分

/// @brief 构造函数
/// @param result
/// @param srcVal1
LoadIRInst::LoadIRInst(Value * _result, Value * _srcVal1) : IRInst(IRInstOperator::IRINST_OP_LOAD, _result){
	srcValues.push_back(_srcVal1);
    
}

/// @brief 析构函数
LoadIRInst::~LoadIRInst() {

}

/// @brief 转换成字符串
void LoadIRInst::toString(std::string & str) {

	Value *src1 = srcValues[0], *result = dstValue;

    //str = result->getName() + " = " + src1->toString();
	str = result->getName() + " = load "+dstValue->type.toString()+" , ptr "+ src1->getName() ;
}




/// @brief 构造函数
/// @param _pattern 模式0 开始， 模式1，结束
BlockborderInst::BlockborderInst(int _pattern){
	this->pattern = _pattern;
}

/// @brief 析构函数
BlockborderInst::~BlockborderInst(){

}

/// @brief 转换成字符串
void BlockborderInst::toString(std::string & str) {
	if(!this->pattern){
		str = "; this is block begining";
	}
	else{
        str = "; this is block ending";
    }
}

// ICMP
	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _result 结果操作数
    /// @param _srcVal1 源操作数1
    /// @param _srcVal2 源操作数2
	/// @param _predicate 比较方式
ICMPInst::ICMPInst(IRInstOperator _op, Value * _result,Value * _srcVal1, Value * _srcVal2, predicate _predicate): 
IRInst(_op, _result)
{
    srcValues.push_back(_srcVal1);
    srcValues.push_back(_srcVal2);
	this->predicate_type = _predicate;
}

    /// @brief 析构函数
ICMPInst::~ICMPInst()
{}

    /// @brief 转换成字符串
void ICMPInst::toString(std::string & str) {
	Value *src1 = srcValues[0], *src2 = srcValues[1], *result = dstValue;
		switch(this->predicate_type){
			case predicate::EQ:
				str = result->getName() + " = icmp eq i32 " + src1->getName() + ", " + src2->getName();
				break;
			case predicate::NE:
				str = result->getName() + " = icmp ne i32 " + src1->getName() + ", " + src2->getName();
				break;
			case predicate::SLT:
				str = result->getName() + " = icmp slt i32 " + src1->getName() + ", " + src2->getName();
				break;
			case predicate::SLE:
				str = result->getName() + " = icmp sle i32 " + src1->getName() + ", " + src2->getName();
				break;
			case predicate::SGT:
				str = result->getName() + " = icmp sgt i32 " + src1->getName() + ", " + src2->getName();
				break;
			case predicate::SGE:
				str = result->getName() + " = icmp sge i32 " + src1->getName() + ", " + src2->getName();
				break;
			default:
				str = result->getName() + " = icmp unknown_predicate i32 " + src1->getName() + ", " + src2->getName();
				break;
		}
}

	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _result 结果操作数
    /// @param _srcVal1 源操作数1
	/// @param _extend_mode 扩展模式
ExtInst::ExtInst(IRInstOperator _op, 
			Value * _result, 
			Value * _srcVal1, 
			extend_mode _extend_mode)
			: IRInst(_op, _result)
			{
				srcValues.push_back(_srcVal1);
    			this->extend_mode_ = _extend_mode;
			}

	/// @brief 析构函数
ExtInst::~ExtInst() 
{}

	/// @brief 转换成字符串
void ExtInst::toString(std::string & str) {
	Value *src1 = srcValues[0] , *result = dstValue;
	switch(this->extend_mode_){
		case extend_mode::ZEXT:
			str = result->getName()+ " = zext i1 "+ src1->getName() +" to i32"; 
			break;
		case extend_mode::SEXT:
			str = result->getName()+ " = sext i1 "+ src1->getName() +" to i32"; 
			break;
		case extend_mode::TRUNC:
			str = result->getName()+ " = trunc i1 "+ src1->getName() +" to i32"; 
			break;
		default:
			str = result->getName()+ " = zext i1 "+ src1->getName() +" to i32"; 
			break;
	}
}


	/// @brief 构造函数
    /// @param _op 操作符
    /// @param _srcLabel1 源操作数label1
	/// @param _jump_mode 跳转模式
	/// @param _srcLabel2 源操作数label2
	/// @param _condition 跳转条件
BrInst::BrInst(IRInstOperator _op, 
			Label * _srcLabel1,
			jump_mode  _jump_mode ,
			Label * _srcLabel2,
			Value * _condition
		  ):IRInst(_op)
		  {
			TOGO_Label = _srcLabel1 ;
			Option_Label = _srcLabel2;
			jump_mode_ = _jump_mode;
			condition = _condition;
		  };

    /// @brief 析构函数
BrInst::~BrInst() {};

bool BrInst::is_Condition(){
    return jump_mode_ == jump_mode::conditional_jump;
}

    /// @brief 转换成字符串
void BrInst::toString(std::string & str) 
{
	switch(jump_mode_){
		case jump_mode::hard_jump:
            str = "br label %" + TOGO_Label->GetLabelName();
            break;
		case jump_mode::conditional_jump:
		    str = "br i1 " +condition->name + ", label %"+ TOGO_Label->GetLabelName() + ", label %" + Option_Label->GetLabelName();
            break;
        default:
            str = "br label %" + TOGO_Label->GetLabelName();
			break;
	}
};




/// @brief 构造函数
/// @param _op 操作符
/// @param _srcValue1 参数1
/// @param _func_type 函数类型
/// @param _srcValue2 参数2
LLVM_FUNC_INST::LLVM_FUNC_INST(
					IRInstOperator _op,
                       std::string _srcValue1,
                       func_type _func_type,
                       u_int32_t _size):IRInst(_op)
{
			arg1=_srcValue1;
			FUNC_TYPE = _func_type;
			size = _size;
}
/// @brief 构造函数
/// @param _op 操作符
/// @param _srcValue1 参数1
/// @param _func_type 函数类型
/// @param _srcValue2 参数2
LLVM_FUNC_INST::LLVM_FUNC_INST(
					IRInstOperator _op,
                       std::string _SrcValue1,
                       func_type _func_type,
                       u_int32_t _size,
					   std::string _srcValue2,
					   std::string _srcValue3):IRInst(_op)
{
			arg1=_SrcValue1;
			FUNC_TYPE = _func_type;
			size = _size;
			arg2 =_srcValue2;
			arg3 = _srcValue3;
}

    /// @brief 析构函数
LLVM_FUNC_INST::~LLVM_FUNC_INST() {
}

    /// @brief 转换成字符串
void LLVM_FUNC_INST::toString(std::string & str) {
	std::string Memset =  
			"call void @llvm.memset.p0.i32(ptr align 4 %" + arg1
			+ ", i8 0, i32 "
			+std::to_string(size)
			+", i1 false)";
    std::string Getelementptr = arg2 + " = "
			"getelementptr inbounds ["
			+std::to_string(size)
			+" x i32], ptr %"
			+arg1
			+", i32 0, i32 "
			+arg3;
    switch (FUNC_TYPE) {
        case func_type::memset:
            str = Memset;
            break;
		case func_type::getelementptr:
            str = Getelementptr;
            break;
        default:
            str = Memset;
            break;
    }
}

	/// @brief 构造函数
	/// @param _op  操作符
	/// @param val 开辟变量的名字
	/// @param _B 开辟变量的类型
	/// @param size 要为变量开辟的空间
GLOBAL_DECLARE_Inst::GLOBAL_DECLARE_Inst(IRInstOperator _op, 
	Value * val,
	int32_t _value,
	bool _is_const,
	BasicType _B ,
	uint32_t _size ,
	std::vector<int> _dims):IRInst(_op){
		srcValues.push_back(val);
        value = _value;
        is_const = _is_const;
        B = _B;
        size = _size;
		dims = _dims;
}

    /// @brief 析构函数
GLOBAL_DECLARE_Inst::~GLOBAL_DECLARE_Inst() 
{

}
	
/// @brief 转换成字符串
void GLOBAL_DECLARE_Inst::toString(std::string & str) {
    std::string global_constant;
    global_constant = is_const == 0 ? " global" : " constant";
    Value * val = srcValues[0];
    switch(B){
		case BasicType::TYPE_INT:
        case BasicType::TYPE_FLOAT:
            str = "@" + val->name + " = dso_local"+
			global_constant
			+
			" i32 "
			+
			std::to_string(value)
			+", align "+
			std::to_string(size)
			+" ";
            break;
		case BasicType::TYPE_ARRAY_i32:
			str = val->getName() + " = dso_local global"+array_dims(dims,0)+" zeroinitializer, align 4 ";
            break;
        default:
            str = val->getName() + " = alloca i32, align 4 ";
            break;
	}
}

