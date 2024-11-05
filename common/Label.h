/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-05-17 08:57:16
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-05 20:22:46
 * @FilePath: /calculator/common/Label.h
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
/**
 * @file label.h
 * @author zhaozihe (zhaozihe@zhaozihe2002@gmail.com)
 * @brief 跳转标签的定义
 * @version 0.1
 * @date 2024-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/// @brief label可能的种类
enum class Label_Type{
	NORMAL,
	IF,
    ELSE,
    WHILE,
	END,
	WHILE_COND,
	WHILE_BODY,
	OR_FALESs, // 只有or的
	AND_TRUEs,
};

class Label{
	public:
	/// @brief 标签名字
	std::string LabelName;

	/// @brief 标签种类
	Label_Type LabelType;

	/// @brief 标签编号
	static u_int32_t LabelNo;

	/// @brief 析构函数
	/// @param label_name Label'name
	Label(const std::string& label_name);

	/// @brief 构造函数
	/// @param label_name Label'name
	Label(Label_Type _LabelType);


	/// @brief 获得标签名
	/// @return 标签名
	std::string GetLabelName();

};