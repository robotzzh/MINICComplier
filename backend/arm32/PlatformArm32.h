/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-04-13 12:48:12
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-05 13:57:12
 * @FilePath: /calculator/backend/arm32/PlatformArm32.h
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
/**
 * @file PlatformArm32.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief ARM32平台相关约定函数的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>

// load指令的目的寄存器计为1
// 二元操作时寄存器分配为源操作数1用REG_ALLOC_SIMPLE_SRC1_REG_NO，
//   源操作数2用REG_ALLOC_SIMPLE_SRC2_REG_NO
//   结果用REG_ALLOC_SIMPLE_DST_REG_NO
//   乘法/除法结果用REG_MUL_DIV_NO
#define REG_ALLOC_SIMPLE_LOAD 1
#define REG_ALLOC_SIMPLE_SRC1_REG_NO 2
#define REG_ALLOC_SIMPLE_SRC2_REG_NO 3
#define REG_ALLOC_SIMPLE_DST_REG_NO 3
#define REG_ADD_SUB_NO 4
#define REG_MUL_DIV_NO 5


// 在操作过程中临时借助的寄存器为REG_ALLOC_SIMPLE_TMP_REG_NO
#define REG_ALLOC_SIMPLE_TMP_REG_NO 10

// 栈寄存器SP和FP
#define REG_ALLOC_SIMPLE_SP_REG_NO 13
#define REG_ALLOC_SIMPLE_FP_REG_NO 11

// 函数跳转寄存器LX
#define REG_ALLOC_SIMPLE_LX_REG_NO 14

/// @brief ARM32平台信息
class PlatformArm32 {

    /// @brief 循环左移两位
    /// @param num
    static void roundLeftShiftTwoBit(unsigned int & num);

    /// @brief 判断num是否是常数表达式，8位数字循环右移偶数位得到
    /// @param num
    /// @return
    static bool __constExpr(int num);

public:
    /// @brief 同时处理正数和负数
    /// @param num
    /// @return
    static bool constExpr(int num);

    /// @brief 判定是否是合法的偏移
    /// @param num
    /// @return
    static bool isDisp(int num);

    /// @brief 判断是否是合法的寄存器名
    /// @param name 寄存器名字
    /// @return 是否是
    static bool isReg(std::string name);

    /// @brief 最大寄存器数目
    static const int maxRegNum = 16;

    /// @brief 可使用的通用寄存器的个数r0-r10
    static const int regNum = 11;

    /// @brief 寄存器的名字，r0-r15
    static const std::string regName[maxRegNum];
};
