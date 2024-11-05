/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-05-17 08:57:25
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-21 16:34:38
 * @FilePath: /calculator/common/Label.cpp
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
/**
 * @file label.cpp
 * @author zhaozihe (zhaozihe@zhaozihe2002@gmail.com)
 * @brief 跳转标签的实现
 * @version 0.1
 * @date 2024-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <string>
#include <unordered_map>
#include <vector>

#include "Label.h"
#include "SymbolTable.h"

u_int32_t Label::LabelNo = 0;

	/// @brief 析构函数
	/// @param label_name Label'name
Label::Label(const std::string& label_name){
	LabelName = label_name;
	LabelType = Label_Type::NORMAL;
}

	/// @brief 构造函数
	/// @param _LabelType 标签种类
Label::Label(Label_Type _LabelType){
	LabelNo++;
	LabelType = _LabelType;
	switch(LabelType)
	{
		case Label_Type::NORMAL:
		    LabelName = "unknown type label" + std::to_string(LabelNo);
			break;
		case Label_Type::IF:
			LabelName = "if_true" + std::to_string(LabelNo);
			break;
		case Label_Type::ELSE:
			LabelName = "else_true" + std::to_string(LabelNo);
			break;
		case Label_Type::WHILE:
			LabelName = "while_true" + std::to_string(LabelNo);
			break;
		case Label_Type::END:
		    LabelName = "end" + std::to_string(LabelNo);
            break;
		 case Label_Type::WHILE_COND:
            LabelName = "while_cond" + std::to_string(LabelNo);
            break;
        case Label_Type::WHILE_BODY:
            LabelName = "while_body" + std::to_string(LabelNo);
            break;
		case Label_Type::OR_FALESs:
            LabelName = "or_fals" + std::to_string(LabelNo);
			break;
		case Label_Type::AND_TRUEs:
            LabelName = "and_trues" + std::to_string(LabelNo);
			break;
        default:
            LabelName = "unknown type label" + std::to_string(LabelNo);
			break;
	}
}


	/// @brief 获得标签名
	/// @return 标签名
std::string Label::GetLabelName() {
	return LabelName;
}