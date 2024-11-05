/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑     永不宕机     永无BUG
 */




/*
 *                                |~~~~~~~|
 *                                |       |
 *                                |       |
 *                                |       |
 *                                |       |
 *                                |       |
 *     |~.\\\_\~~~~~~~~~~~~~~xx~~~         ~~~~~~~~~~~~~~~~~~~~~/_//;~|
 *     |  \  o \_         ,XXXXX),                         _..-~ o /  |
 *     |    ~~\  ~-.     XXXXX`)))),                 _.--~~   .-~~~   |
 *      ~~~~~~~`\   ~\~~~XXX' _/ ';))     |~~~~~~..-~     _.-~ ~~~~~~~
 *               `\   ~~--`_\~\, ;;;\)__.---.~~~      _.-~
 *                 ~-.       `:;;/;; \          _..-~~
 *                    ~-._      `''        /-~-~
 *                        `\              /  /
 *                          |         ,   | |
 *                           |  '        /  |
 *                            \/;          |
 *                             ;;          |
 *                             `;   .       |
 *                             |~~~-----.....|
 *                            | \             \
 *                           | /\~~--...__    |
 *                           (|  `\       __-\|
 *                           ||    \_   /~    |
 *                           |)     \~-'      |
 *                            |      | \      '
 *                            |      |  \    :
 *                             \     |  |    |
 *                              |    )  (    )
 *                               \  /;  /\  |
 *                               |    |/   |
 *                               |    |   |
 *                                \  .'  ||
 *                                |  |  | |
 *                                (  | |  |
 *                                |   \ \ |
 *                                || o `.)|
 *                                |`\\) |
 *                                |       |
 *                                |       |
 */

/**
 * @file main.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 主程序文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
 

#include <iostream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "AST.h"
#include "CodeGenerator.h"
#include "CodeGeneratorArm32.h"
#include "FlexBisonExecutor.h"
#include "FrontEndExecutor.h"
#include "Graph.h"
#include "IRGenerator.h"
#include "SymbolTable.h"
#include "getopt-port.h"

#include "Optimizer/Optimizer.h"

/// @brief 是否显示帮助信息
bool gShowHelp = false;

/// @brief 显示抽象语法树，非线性IR
int gShowAST = 0;

/// @brief 产生线性IR，线性IR，默认输出
int gShowLineIR = 0;

/// @brief 产生优化IR
int gOIR = 0; 

/// @brief 打印优化IR
int gShowOIR = 0;

/// @brief 显示汇编
int gShowASM = 0;

/// @brief 直接运行，默认运行
int gDirectRun = 0;

/// @brief 输出中间IR，含汇编或者自定义IR等，默认输出线性IR
int gShowSymbol = 0;

/// @brief 前端分析器，默认选Flex和Bison
bool gFrontEndFlexBison = true;

/// @brief 前端分析器Antlr4，是否选中
bool gFrontEndAntlr4 = false;

/// @brief 前端分析器用递归下降分析法，是否选中
bool gFrontEndRecursiveDescentParsing = false;

/// @brief 输入源文件
std::string gInputFile;

/// @brief 输出文件，不同的选项输出的内容不同
std::string gOutputFile;

/// @brief 显示帮助
/// @param exeName
void showHelp(const std::string & exeName)
{
    std::cout << exeName + " -S [-a | -I| -O] [-o output] source\n";
    std::cout << exeName + " -R [-A | -D] source\n";
}

/// @brief 参数解析与有效性检查
/// @param argc
/// @param argv
/// @return
int ArgsAnalysis(int argc, char * argv[])
{
    int ch;

    // 指定参数解析的选项，可识别-h、-o、-S、-a、-I、-R、-A、-D选项，并且-o要求必须要有附加参数
    const char options[] = "ho:SaIROX";

    opterr = 1;

lb_check:
    while ((ch = getopt(argc, argv, options)) != -1) {
        switch (ch) {
            case 'h':
                gShowHelp = true;
                break;
            case 'o':
                gOutputFile = optarg;
                break;
			case 'O':
                gOIR = 1;
                gShowOIR = 1;
                break;
			case 'X':
                gOIR = 1;
                gShowOIR = 0;
                break;
            case 'S':
                gShowSymbol = 1;
                break;
            case 'a':
                gShowAST = 1;
                break;
            case 'I':
                // 产生中间IR
                gShowLineIR = 1;
                break;
            case 'R':
                // 直接运行，默认运行
                gDirectRun = 1;
                break;
            default:
                return -1;
                break; /* no break */
        }
    }

    argc -= optind;
    argv += optind;

    if (argc >= 1) {

        // 第一次设置
        if (gInputFile.empty()) {

            gInputFile = argv[0];
        } else {
            // 重复设置则出错
            return -1;
        }

        if (argc > 1) {
            // 多余一个参数，则说明输入的源文件后仍然有参数要解析
            optind = 0;
            goto lb_check;
        }
    }

    // 必须指定输入文件和输出文件
    if (gInputFile.length() == 0) {
        return -1;
    }

    // 这三者只能指定一个
    int flag = gShowSymbol + gDirectRun;
    if (flag != 1) {
        // 运行与中间IR只能同时选择一个
        return -1;
    }

    flag = gShowLineIR + gShowAST;

    if (gShowSymbol) {

        if (flag == 0) {
            // 没有指定，则输出汇编指令
            gShowASM = 1;
        } else if (flag != 1) {
            // 线性中间IR、抽象语法树只能同时选择一个
            return -1;
        }
    } else {
        // 如果-S没有指定，但指定了-a等选项时，则失败
        if (flag != 0) {
            return -1;
        }
    }

    if (gOutputFile.empty()) {

        // 默认文件名
        if (gShowAST) {
            gOutputFile = "ast.png";
        } else if (gShowLineIR) {
            gOutputFile = "ir.txt";
        } else {
            gOutputFile = "asm.s";
        }
    } else {

        // 直接运行时，文件不能指定
        if (gDirectRun) {
            return -1;
        }
    }

    return 0;
}

/// @brief 主程序
/// @param argc
/// @param argv
/// @return
int main(int argc, char * argv[])
{
    // 函数返回值，默认-1


    int result = -1;

    // 内部函数调用返回值保存变量
    int subResult;

#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif

    // 这里采用do {} while(0)架构的目的是如果处理出错可通过break退出循环，出口唯一
    // 在编译器编译优化时会自动去除，因为while恒假的缘故
    do {

        // 参数解析
        subResult = ArgsAnalysis(argc, argv);
        if (subResult < 0) {

            // 在终端显示程序帮助信息
            showHelp(argv[0]);

            // 错误不用设置，因此result默认值为-1
            break;
        }

        // 显示帮助
        if (gShowHelp) {

            // 在终端显示程序帮助信息
            showHelp(argv[0]);

            // 这里必须设置返回值，因默认值为-1
            result = 0;

            break;
        }

        // 创建词法语法分析器
        FrontEndExecutor * frontEndExecutor;
		/*
        if (gFrontEndAntlr4) {
            // Antlr4
            frontEndExecutor = new Antlr4Executor(gInputFile);
        } else if (gFrontEndRecursiveDescentParsing) {
            // 递归下降分析法
            frontEndExecutor = new RecursiveDescentExecutor(gInputFile);
        } else {
            // 默认为Flex+Bison
        }
		*/
		frontEndExecutor = new FlexBisonExecutor(gInputFile);

        // 前端执行：词法分析、语法分析后产生抽象语法树，其root为全局变量ast_root
        subResult = frontEndExecutor->run();
        if (!subResult) {

            std::cout << "FrontEnd's analysis failed\n";

            // 退出循环
            break;
        }

        // 清理前端资源
        delete frontEndExecutor;

        // 这里可进行非线性AST的优化

        if (gShowAST) {

            // 遍历抽象语法树，生成抽象语法树图片
            OutputAST(ast_root, gOutputFile);

            // 清理抽象语法树
            free_ast();

            // 设置返回结果：正常
            result = 0;

            break;
        }

        // 输出线性中间IR、计算器模拟解释执行、输出汇编指令
        // 都需要遍历AST转换成线性IR指令

        // 符号表，保存所有的变量以及函数等信息
        SymbolTable symtab;

        // 遍历抽象语法树产生线性IR，相关信息保存到符号表中
        IRGenerator ast2IR(ast_root, &symtab);
        subResult = ast2IR.run();
        if (!subResult) {

            // 输出错误信息
            std::cout << "GenIR failed\n";

            break;
        }

        // 清理抽象语法树
        free_ast();

        if (gShowLineIR && !gShowOIR) {

            // 输出无优化IR
            symtab.outputIR(gOutputFile);

            // 设置返回结果：正常
            result = 0;

            break;
        }

        // IR优化器部分
		/*
		if(gOIR)
		{
			Optimizer Optimizer(symtab);
			if(gShowOIR){
				symtab.outputIR(gOutputFile);

            	// 设置返回结果：正常
            	result = 0;

            	break;
			}
		}*/
        // 后端处理，体系结果相关的操作
        // 这里提供两种模式：第一种是解释执行器CodeSimulator；第二种为面向ARM32的汇编产生器CodeGeneratorArm32
        // 需要时可根据需要修改或追加新的目标体系架构

        CodeGenerator * generator;

        if (gShowASM) {

            // 输出面向ARM32的汇编指令
            generator = new CodeGeneratorArm32(symtab);
            generator->run(gOutputFile);
            delete generator;
        } 

        // 清理符号表
        symtab.freeValues();

        result = 0;
    } while (false);

    return result;
}
