/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-04-13 12:48:12
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-14 15:31:42
 * @FilePath: /calculator/LLVMIR/IRCode.h
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
/**
 * @file IRCode.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 中间IR指令序列管理类的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <vector>
#include <memory>

#include "IRInst.h"

/// @brief 中间IR指令序列管理类
class InterCode {

protected:
    /// @brief 指令块的指令序列
    std::vector<IRInst *> code;

public:
    /// @brief 构造函数
    InterCode() = default;

    /// @brief 析构函数
    ~InterCode();

    /// @brief 添加一个指令块，添加到尾部，并清除原来指令块的内容
    /// @param block 指令块，请注意加入后会自动清空block的指令
    void addInst(InterCode & block);

    /// @brief 添加一条中间指令
    /// @param inst IR指令
    void addInst(IRInst * inst);

	/// @brief 在开头添加一条中间指令
    /// @param inst IR指令
	void add_Begin_Inst(IRInst * inst);

	/// @brief 将code里所有变量变成nullptr
    void clear_code();

	/// @brief IRcode复制构造
    void setCode(InterCode & other);

    /// @brief 获取指令序列
    /// @return 指令序列
    std::vector<IRInst *> & getInsts();

};
