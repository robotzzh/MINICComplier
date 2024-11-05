/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-05-27 10:54:44
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-06-14 18:54:01
 * @FilePath: /calculator/Optimizer/Optimizer.h
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
#include "SymbolTable.h"
#include "BB_separator.h"
#include "Flow_Graph.h"
#include <vector>
#include <string>
#include <map>
#include <set>


class Optimizer{

public:
    /// @brief  未优化前的符号表
    SymbolTable origin_symtab;

	/// @brief  未优化前的基本块
    std::vector<BasicBlock *> origin_basic_blocks;

	/// @brief 入口——id映射表
	std::map<std::string, int> entry_id;


    /// @brief  优化后的符号表
	SymbolTable optimized_symtab;

	/// @brief 控制流图，临接表,入口永远是第一个flow_graph[0] ,当flow_graph[i][j]=-1,即第i个基本块的第j个出口为exit
	/*e.g. 1 - 2，3
		   2 - 4
		   3 - 5
		   4 - 6
		   5 - 6
		   6 - -1
		   基本块1有块2，3两个出口，2有4这个出口，6只有-1这一个出口-1即为exit
	
	*/
    std::vector<std::vector<int>> flow_graph;

	/// func_block[i].size == 0 无意义
	/// @brief 函数基本块表
	std::vector<std::vector<int>> func_block;

public:
	/// @brief 输出分割后的基本块
	/// @param filePath 输出的文件路径
    void OUTPUT_SB(const std::string & filePath);

	/// @brief 输出基本块的控制流图
	/// @param filePath 输出的文件路径
    void OUTPUT_FG(const std::string & filePath);

    /// @brief 优化器构造函数
	/// @param symtab 引用符号表传入，修改完毕后返回
    Optimizer(SymbolTable & symtab);

	

	/// @brief 分解symtab变成基本块表
	/// @return 是否分解成功
    bool Separator();

	/// @brief 基本块表转化为控制流图
	/// @return 是否分解成功
    bool Linear_to_graph();

	/// @brief 将基本块表压缩回symtab当中
	/// @return 是否构造symtab成功成功
    bool Compress();

    /// @brief 常数的合并
	/// @param B 基本块指针
    /// @return 是否合并成功
    bool merge_const(BasicBlock * B);
};