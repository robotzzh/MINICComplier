/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-05-26 11:39:47
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-14 20:33:27
 * @FilePath: /calculator/Optimizer/BB_separator.cpp
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */

#include<string>
#include<vector>
#include "BB_separator.h"
#include "Label.h"

/// @brief  构造函数
/// @param name 
/// @param BasicBlock_comment Optional
BasicBlock::BasicBlock(std::string name,
					int _id,
				   std::string _BasicBlock_comment
				   )
				   {
					   BasicBlock_comment = _BasicBlock_comment;
                       id = _id;
                       BasicBlock_name = name;
                   }

BasicBlock::~BasicBlock()
	{
	}

		/// @brief 在末尾增加指令
bool BasicBlock::addInst(IRInst *Inst){
        BasicBlock_codes.push_back(Inst);
        return true;
    }

	    /// @brief 设置入口
bool BasicBlock::Add_Entry(std::string _Entry){
	Entry.emplace_back(_Entry);
    return true;
}

        /// @brief 设置出口
bool BasicBlock::Add_Exit(std::string _Exit){
	Exit.emplace_back(_Exit);
    return true;
}

std::string BasicBlock::to_string(){
            std::string str = "";
			str += "BasicBlock_name: " + BasicBlock_name + "\n";
			
            str += "BB_Entry :\n{ ";
            for (auto entry: Entry) {
                str += "\t" + entry + "\n";
            }
            str += "}\n";

			for(auto BasicBlock_code : BasicBlock_codes){
                std::string instStr;
        		BasicBlock_code->toString(instStr);
				str += "\t" + instStr + "\n";
            }
			str += "Comment: "+ BasicBlock_comment +"\n";

			str += "BB_Exit :\n{ ";
			for (auto exit: Exit) {
                str += "\t" + exit + "\n";
            }
            str += "}\n";

			return str;
        }


/// @brief  删除一行
/// @param _id 行号
/// @return 是否删除成功
bool BasicBlock::delete_column(int _id){
    this->BasicBlock_codes.erase(this->BasicBlock_codes.begin() + _id);
    return true;
}
