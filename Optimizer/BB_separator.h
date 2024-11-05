/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-05-26 11:39:01
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-14 20:32:23
 * @FilePath: /calculator/Optimizer/BB_separator.h
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
// basic block sepatator
#include<vector>
#include<string>
#include<map>
#include<set>
#include "IRInst.h"
#include "SymbolTable.h"
#include "Label.h"
class BasicBlock{

	public:
		/// @brief 基本块携带的代码
		std::vector<IRInst *> BasicBlock_codes;
	    /// @brief 基本块名字
	    std::string BasicBlock_name;
		/// @brief 基本块携带的注释
		std::string BasicBlock_comment;
        /// @brief 基本块入口
        std::vector<std::string> Entry;
		/// @brief 基本块出口
		std::vector<std::string>  Exit;
        /// @brief 基本块ID
        int id;
		/// @brief 入口，id映射表

    public:
        /// @brief  构造函数
        /// @param name
        /// @param BasicBlock_comment Optional
		/// @param _id id
        BasicBlock(std::string name, int _id ,std::string _BasicBlock_comment = "");

        /// @brief  xigou函数
        ~BasicBlock();

	
		/// @brief  删除一行
		/// @param _id 行号
		/// @return 是否删除成功
		bool delete_column(int _id);

		/// @brief 在末尾增加指令
        bool addInst(IRInst * Inst);

        /// @brief 设置入口
        bool Add_Entry(std::string _Entry);

		/// @brief 设置出口
        bool Add_Exit(std::string _Exit);

        /// @brief 基本块字符化
        /// @return 字符化的基本块 
        std::string to_string();

		/// @brief 文本输出线性基本块分割结果
		/// @param filePath 输出文件路径
        void outputSB(const std::string & filePath);
};

