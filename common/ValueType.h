/**
 * @file type.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 变量类型管理的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>

/// @brief 基本类型枚举类
enum class BasicType : int {
    TYPE_NONE,  // 节点不存在类型
    TYPE_VOID,  // void型，仅用于函数返回值
    TYPE_INT,   // 整型
    TYPE_FLOAT, // Float类型
	TYPE_Boolean, // boolean类型
	TYPE_NULLPTR, // nullptr
	TYPE_POINTER_i32, // Pointer i32
	TYPE_ARRAY_i32, // Array i32
    TYPE_MAX,   // 其它类型，未知类型
};

enum class ComplexType {
	TYPE_NULLPTR, // nullptr
	TYPE_POINTER_i32, // Pointer i32
	TYPE_ARRAY_i32, // Array i32
};

/// @brief 目前只考虑基本类型，数组类型后面可定义
class ValueType {

public:
    /// @brief 值的类型
    BasicType type; 

	/// @brief 字节变为bit
	/// @return 返回比特
	u_int32_t to_bits();

    /// @brief 构造函数，类型不存在
    ValueType();

    /// @brief 构造函数
    /// @param _type 指定类型
    ValueType(BasicType _type);

    /// @brief 转换字符串
    /// @return 字符串
    std::string toString();
};

class PointerType {
	/// @brief 值的类型
	ComplexType type;

	/// @brief 指针所需开辟的空间大小，单位:字节(8bits)
	u_int32_t offset;

	/// @brief 构造函数，类型不存在
    PointerType();

    /// @brief 构造函数
    /// @param _type 指定类型
	/// @param _offset 指针所需开辟的空间大小，单位:字节(8bits)
    PointerType(ComplexType _type,u_int32_t _offset);


    /// @brief 转换字符串
    /// @return 字符串
    std::string toString();

	/// @brief 字节变为bit
	/// @return 返回比特
	u_int32_t to_bits();
};