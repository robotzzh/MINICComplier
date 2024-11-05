/*
 * @Author: git config zhaozihe && git config weiersili2021@163.com
 * @Date: 2024-05-27 10:54:54
 * @LastEditors: zzh weiersili2021@163.com
 * @LastEditTime: 2024-07-06 18:14:51
 * @FilePath: /calculator/Optimizer/Optimizer.cpp
 * @@brief: 
 * 
 * Copyright (c) NPU by zhao, All Rights Reserved. 
 */
#include "SymbolTable.h"
#include "Flow_Graph.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include "Optimizer.h"
#include "IRCode.h"
#include "Function.h"
#include "IRInst.h"


/// @brief 优化器构造函数
/// @param symtab 引用符号表传入，修改完毕后返回
Optimizer::Optimizer(SymbolTable & symtab){
    std::cout << " launch op\n";
    this->origin_symtab = symtab;
    this->optimized_symtab = symtab;

    // TODO:  optimize sectors
    Optimizer::Separator();
	for (auto BB: this->origin_basic_blocks) {
        std::cout << BB->to_string();
    }

    Optimizer::Linear_to_graph();

	for(auto BB : this->origin_basic_blocks) {
        merge_const(BB);
    }

    /// 将origin_basic_blocks压缩到origin_symtab当中
    Optimizer::Compress();

    symtab = this->origin_symtab;
}

/// @brief 分解symtab变成基本块表
/// @return 是否分解成功
bool Optimizer::Separator(){
    int temp = 0;
    std::vector<Function *> FuncVector = this->origin_symtab.getFunctionList();
    for(auto & Func : FuncVector){
        std::vector<IRInst *> & Insts = Func->getInterCode().getInsts();
        BasicBlock * current_Block = nullptr;
        std::vector<int> infunc_blocks;
        // 现在Insts是一个函数内部的所有指令
        // TDDO: 分割基本块
        for(auto & Inst: Insts){
			if(Inst->getOp() == IRInstOperator::IRINST_OP_ENTRY){
				current_Block = new BasicBlock(std::to_string(temp)+": BB",temp,"");

                entry_id.insert(std::pair<std::string,int>(Inst->getLabelName(), temp));
                infunc_blocks.push_back(temp);

                temp++;
                current_Block->Add_Entry(Inst->getLabelName());
				current_Block->addInst(Inst);
                continue;
            }
			if(Inst->getOp() == IRInstOperator::IRINST_OP_JUMP){
                BrInst * BRI = (BrInst * )Inst;
                if(BRI->is_Condition()){
                    current_Block->addInst(BRI);
                    current_Block->Add_Exit(BRI->TOGO_Label->GetLabelName());
					current_Block->Add_Exit(BRI->Option_Label->GetLabelName());
                } else {
					current_Block->addInst(BRI);
                    current_Block->Add_Exit(BRI->TOGO_Label->GetLabelName());
                }
				origin_basic_blocks.emplace_back(current_Block);
				current_Block = nullptr;
				continue;
            }
			if(Inst->getOp() == IRInstOperator::IRINST_OP_EXIT){
				current_Block->addInst(Inst);
				current_Block->Add_Exit(Inst->getLabelName());
                origin_basic_blocks.emplace_back(current_Block);

                current_Block = nullptr;
				continue;
            }
            if(Inst->getOp() == IRInstOperator::IRINST_OP_LABEL){
			    current_Block = nullptr;

                current_Block = new BasicBlock(std::to_string(temp) + ": BB",temp, "");

				entry_id.insert(std::pair<std::string,int>(Inst->getLabelName(), temp));
				infunc_blocks.push_back(temp);
				
                temp++;
                current_Block->Add_Entry(Inst->getLabelName());
                current_Block->addInst(Inst);
				continue;
            }
            current_Block->addInst(Inst); 
        }
        this->func_block.push_back(infunc_blocks);
    }
    return true;
}

/// @brief 基本块表转化为控制流图
/// @return 是否分解成功
bool Optimizer::Linear_to_graph(){
    flow_graph.reserve(origin_basic_blocks.size());
    for (int i = 0; i < origin_basic_blocks.size(); i++) {
        std::vector<int> temp;
        for (auto & exit: origin_basic_blocks[i]->Exit) {
			if(exit == "exit"){
				/// -1代表exit
                temp.push_back(-1);
            }else{
				auto element = entry_id.find(exit);
				temp.push_back(element->second);
			}
        }
        flow_graph.push_back(temp);
    }

    return true;
}


/// @brief 将基本块表压缩回symtab当中
/// @return 是否构造symtab成功成功
bool Optimizer::Compress(){
    std::vector<Function *> FuncVector = this->origin_symtab.funcVector;
    int fun_length = FuncVector.size();	
    for (int i = 0; i < fun_length;i++){
		InterCode * LongevityInsts = new InterCode();
            
            for (auto blockid: func_block[i]) {
                {
                    BasicBlock * b = origin_basic_blocks[blockid];

					for(auto inst: b->BasicBlock_codes){
                        LongevityInsts->addInst(inst);
						
                    }
                }
            }
            FuncVector[i]->code.clear_code();
            for (auto Inst: LongevityInsts->getInsts()) {
                FuncVector[i]->code.addInst(Inst);
            }
            LongevityInsts = nullptr;
    }
    return true;
}

/// @brief 常数的合并
/// @param B 基本块指针
/// @return 是否合并成功
bool Optimizer::merge_const(BasicBlock * B){
    int index = 0;
    for (; index < B->BasicBlock_codes.size();) {
		auto Inst = B->BasicBlock_codes[index];
        if(Inst->getOp() == IRInstOperator::IRINST_OP_ADD_I
		||Inst->getOp()==IRInstOperator::IRINST_OP_SUB_I 
		|| Inst->getOp()==IRInstOperator::IRINST_OP_MUL_I 
		|| Inst->getOp()==IRInstOperator::IRINST_OP_DIV_I){
            Value * src1 = Inst->getSrc1();
			Value * src2 = Inst->getSrc2();
			Value * dest = Inst->getDst();
			if(src1->_const && src2->_const){
                int answer = 0;
                switch (Inst->getOp()) {
                    case IRInstOperator::IRINST_OP_ADD_I:
                        answer = src1->intVal + src2->intVal;
                        break;
					case IRInstOperator::IRINST_OP_SUB_I:
                        answer = src1->intVal - src2->intVal;
                        break;
					case IRInstOperator::IRINST_OP_MUL_I:
                        answer = src1->intVal * src2->intVal;
                        break;
					case IRInstOperator::IRINST_OP_DIV_I:
                        answer = src1->intVal / src2->intVal;
                        break;
                    default:
                        answer = src1->intVal + src2->intVal;
                        break;
                };
                dest->intVal = answer;
                dest->name = std::to_string(answer);
                dest->_const = true;
                B->delete_column(index);
                index--;
            }
        }
        index++;
    }
    return true;
}