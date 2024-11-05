/**
 * @file AttrType.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 语法分析过程中符号的属性类型
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <cstdint>
#include <string>

/// @brief 词法与语法通信的无符号整数字面量属性
typedef struct digit_int_attr {
    int32_t val;   // 整数值
    int32_t lineno; // 行号
} digit_int_attr;

/// @brief 词法与语法通信的单精度浮点数字面量属性
typedef struct digit_real_attr {
    float val;      // 实数值
    int32_t lineno; // 行号
} digit_real_attr;

/// @brief 词法与语法通信的标识符（变量名、函数名等）
typedef struct var_id_attr {
    char * id;      // 标识符名称
    int32_t lineno; // 行号
	int32_t is_global; // 是否是全局变量
	int32_t is_const; // 
} var_id_attr;

/// @brief 词法与语法通信的单精度布尔类型字面量属性
typedef struct digit_bool_attr{
    char val;      // bool值
    int32_t lineno; // 行号
} digit_bool_attr; 
