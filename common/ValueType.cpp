/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-04-13 12:48:12
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-06 11:50:32
 * @FilePath: /calculator/common/ValueType.cpp
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
/**
 * @file type.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 变量类型管理
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ValueType.h"

/// @brief 构造函数，类型不存在
ValueType::ValueType() : type(BasicType::TYPE_NONE)
{}

/// @brief 构造函数
/// @param _type 指定类型
ValueType::ValueType(BasicType _type) : type(_type)
{}

/// @brief 转换字符串
/// @return 字符串
std::string ValueType::toString()
{
    std::string str;
    switch (type) {
        case BasicType::TYPE_INT:
            str = "i32";
            break;
        case BasicType::TYPE_FLOAT:
            str = "float";
            break;
        case BasicType::TYPE_VOID:
            str = "void";
            break;
        case BasicType::TYPE_NONE:
            str = "None";
            break;
		case BasicType::TYPE_Boolean:
			str = "boolean";
			break;
		case BasicType::TYPE_POINTER_i32:
            str = "ptr";
            break;
        default:
            str = "Unknown";
    }

    return str;
}



/// @brief 构造函数，类型不存在
PointerType::PointerType() : type(ComplexType::TYPE_NULLPTR), offset(0)
{}


/// @brief 构造函数
/// @param _type 指定类型
/// @param _offset 指针所需开辟的空间大小，单位:字节(8bits)
PointerType::PointerType(ComplexType _type,u_int32_t _offset) : type(_type), offset(_offset)
{}



/// @brief 字节变为bit
/// @return 返回比特
u_int32_t PointerType::to_bits(){
	return 8 * offset;
}

/// @brief 转换字符串
/// @return 字符串
std::string PointerType::toString(){
	std::string str;
	switch (type) {
		case ComplexType::TYPE_NULLPTR:
			str = "NULL ptr memory offset" + std::to_string(offset) + "bytes";
			break;
		case ComplexType::TYPE_POINTER_i32:
			str = "Integer ptr memory offset" + std::to_string(offset) + "bytes";
			break;
		case ComplexType::TYPE_ARRAY_i32:
			str = "Integer Array ptr memory offset" + std::to_string(offset) + "bytes";
			break;
		default:
			str = "NULL ptr";
			break;
	}
	return str;
}