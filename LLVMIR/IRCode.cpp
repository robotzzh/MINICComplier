/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-04-13 12:48:12
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-14 16:52:43
 * @FilePath: /calculator/LLVMIR/IRCode.cpp
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
/**
 * @file IRCode.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief IR指令序列管理类
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <vector>

#include "IRCode.h"
#include "IRInst.h"

/// @brief 析构函数
InterCode::~InterCode()
{
    // 资源清理
    for (auto inst: code) {
        delete inst;
    }
    code.clear();
}

/// @brief 添加一个指令块，添加到尾部，并清除原来指令块的内容
/// @param block 指令块，请注意加入后会自动清空block的指令
void InterCode::addInst(InterCode & block)
{
    std::vector<IRInst *> & insert = block.getInsts();

    code.insert(code.end(), insert.begin(), insert.end());

    // InterCode析构会清理资源，因此移动指令到code中后必须清理，否则会释放多次导致程序例外
    // 当然，这里也可不清理，但InterCode的析构函数不能清理，需专门的函数清理即可。
    insert.clear();
}

/// @brief 添加一条中间指令
/// @param inst IR指令
void InterCode::addInst(IRInst * inst)
{
    code.emplace_back(inst);
}

/// @brief 在开头添加一条中间指令
/// @param inst IR指令
void InterCode::add_Begin_Inst(IRInst * inst){
	code.insert(code.begin(), inst);
}

/// @brief 获取指令序列
/// @return 指令序列
std::vector<IRInst *> & InterCode::getInsts()
{
    return code;
}

/// @brief 将code里所有变量变成nullptr
void InterCode::clear_code(){
	std::fill(code.begin(), code.end(), nullptr);
    code.resize(0);
}

