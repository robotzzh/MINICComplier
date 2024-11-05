%{
#include <cstdio>
#include <cstring>
#include<stdio.h>
#include<stdlib.h>

// 词法分析头文件
#include "FlexLexer.h"

// bison生成的头文件
#include "BisonParser.h"

// 抽象语法树函数定义原型头文件
#include "AST.h"

#include "Label.h"

// LR分析失败时所调用函数的原型声明
void yyerror(char * msg);
struct cpmplex_error{
	char * msg;
	int code;
};
char * errormsg(struct cpmplex_error * err);
/*
global_type == 1 means that type is i32
global_type == 2 means that type is float
global_type == 3 means that type is i32 arr
global_type == 4 means that type is pointer i32
*/
int global_type = 0;
/*
指针偏移量
*/

u_int32_t pointer_offset = 0;

int is_global = 0; // 定义的变量是是全局变量
int is_const = 0; //常量表达式
%}

// 联合体声明，用于后续终结符和非终结符号属性指定使用
%union {
    class ast_node * node;

    struct digit_int_attr integer_num;
    struct digit_real_attr float_num;
    struct var_id_attr var_id;
	struct digit_bool_attr bool_num;
};

// 文法的开始符号
%start  CompileUnit

// 指定文法的终结符号，<>可指定文法属性
// 对于单个字符的算符或者分隔符，在词法分析时可直返返回对应的字符即可
%token <integer_num> T_DIGIT
%token <float_num> T_FLOAT
%token <var_id> T_ID
%token <bool_num> T_Boolean
%token T_FUNC T_RETURN T_ADD T_SUB T_MUL T_DIV T_REM 
%token T_EQ T_NE T_LE T_LN T_GE T_GN
%token T_LAND T_LOR  
%token T_IF T_ELSE T_WHILE T_BREAK T_CONTINUE T_GOTO
%token T_VOID 



%token E_IF




%token T_CONST INT FLOAT T_CONST T_POINTER 
%left T_SUB /* 一元负号的优先级 */
%left T_ADD  /* 一元正号的优先级 */
%left T_NOT   /* 一元逻辑非的优先级 */
%type <node> CompileUnit

// 指定文法的非终结符号，<>可指定文法属性
%type <node> FuncDef
%type <node> FuncFormalParams
%type <node> Block

%type <node> FuncFormalParam
%type <node> FuncBasicParam

%type <node> BlockItemList
%type <node> BlockItem

%type <node> Statement
%type <node> Expr  Assign
%type <node> MulExp AddExp UnaryExp LVal
%type <node> RelExp EqExp LAndExp LorExp Cond
%type <node> PrimaryExp ArrayExp
%type <node> RealParamList

%type <node> Decl  VarDecl 
%type<node> ArrayParams ArrayParam
%%

/* 编译单元可包含若干个函数，main函数作为程序的入口，必须存在 EqExp LAndExp LorExp*/
CompileUnit : FuncDef {
        $$ = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT, $1);
        ast_root = $$;
    }
    | CompileUnit FuncDef {
        $$ = insert_ast_node($1, $2);
    }
	| Decl ';'{
		$$ = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT, $1);
        ast_root = $$;
			for(int i=0;i<$1->sons.size();i++){
				if($1->sons[i]->sons.size() == 0) $1->sons[i]->is_global = 1;
				else{
					if($1->sons[i]->sons[0]->sons.size() == 0) {
						$1->sons[i]->sons[0]->is_global = 1;
					}
					else {
						$1->sons[i]->sons[0]->sons[0]->is_global = 1;
					}
				}
			}
		}
	| CompileUnit Decl ';'{
		$$ = insert_ast_node($1, $2);
		for(int i=0;i<$2->sons.size();i++){
				if($2->sons[i]->sons.size() == 0) $2->sons[i]->is_global = 1;
				else{
					if($2->sons[i]->sons[0]->sons.size() == 0) {
						$2->sons[i]->sons[0]->is_global = 1;
					}
					else {
						$2->sons[i]->sons[0]->sons[0]->is_global = 1;
					}
				}
			}
	}
    ;

// 函数定义
FuncDef : 
	T_VOID T_ID '(' ')' Block{
		// 无返回值函数构造，调用创建函数定义类型（非终结符），默认形参为空
        $$ = create_func_def($2.lineno, $2.id, $5, nullptr,0);
    }
	| T_VOID T_ID '(' FuncFormalParams ')' Block {
	// 含参数函数构造，调用创建函数定义类型（非终结符），默认形参为$4
        $$ = create_func_def($2.lineno, $2.id, $6, $4,0);
    }
	| INT T_ID '(' ')' Block{
		// 无返回值函数构造，调用创建函数定义类型（非终结符），默认形参为空
        $$ = create_func_def($2.lineno, $2.id, $5, nullptr, 1);
    }
	| INT T_ID '(' FuncFormalParams ')' Block {
	// 含参数函数构造，调用创建函数定义类型（非终结符），默认形参为$4
        $$ = create_func_def($2.lineno, $2.id, $6, $4, 1);
    }
	| FLOAT T_ID '(' ')' Block{
		// 无返回值函数构造，调用创建函数定义类型（非终结符），默认形参为空
        $$ = create_func_def($2.lineno, $2.id, $5, nullptr,2);
    }
	| FLOAT T_ID '(' FuncFormalParams ')' Block {
	// 含参数函数构造，调用创建函数定义类型（非终结符），默认形参为$4
        $$ = create_func_def($2.lineno, $2.id, $6, $4,2);
    } 
    ;

// 函数参数
FuncFormalParams : FuncFormalParam  {
		// 第一个/单参数时，创建参数列表指针域
        $$ = create_contain_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS, $1);
    }
    | FuncFormalParams ',' FuncFormalParam {
		// 多参数时，在第一个参数，在参数列表指针域内添加子节点，参数指针域为父节点，参数为子节点
        $$ = insert_ast_node($1, $3);
    }
    ;

// 函数参数，目前只支持基本类型参数
FuncFormalParam : FuncBasicParam  {
        $$ = $1;
    } | 
    ;

// 基本类型函数参数，默认整型
FuncBasicParam : T_ID {
        $$ = create_func_formal_param($1.lineno, $1.id);
    }
	| INT T_ID{
		$$ = create_func_formal_param($2.lineno, $2.id,1);
	}
	| FLOAT T_ID{
		$$ = create_func_formal_param($2.lineno, $2.id,2);
	}
	| INT '*' T_ID{
		$$ = create_func_formal_param($3.lineno, $3.id,3);
	}
    ;

// 语句块
Block : '{' '}' {
        // 语句块没有语句
        $$ = nullptr;
    }
    | '{' BlockItemList '}' {
        // 语句块含有语句
        $$ = $2;
    }
    ;

// 语句块内语句列表
BlockItemList : BlockItem {
        // 第一个左侧的孩子节点归约成Block父节点，后续语句可不断作为孩子追加到block中
        // 创建一个AST_OP_BLOCK类型的中间节点，孩子为Statement($1)
        $$ = new_ast_node(ast_operator_type::AST_OP_BLOCK, $1, nullptr);
    }
    | BlockItemList BlockItem  {
        // 采用左递归的文法产生式，可以使得Block节点在上个产生式创建，后续递归追加孩子节点
        // 请注意，不要采用右递归，左递归翻遍孩子的追加
        // BlockItem($2)作为Block($1)的孩子 
        $$ = insert_ast_node($1,$2);
    } 
	| BlockItemList ',' Assign ';'{
		$$ = insert_ast_node($1,$3);
	}
	| BlockItemList ',' Assign{
		$$ = insert_ast_node($1,$3);
	}
    ;

// 目前语句块内项目只能是语句
BlockItem :
	Statement  {
        $$ = $1;
    }
    ;

/* 语句 */
Statement : 
	 Assign 
	{
		$$ = $1;
	} 
	| Assign ';'
	{
		$$ = $1;
	} 
	| Block{
        $$ = $1;
	}
	| Statement Block {
		$$ = $2;
	}
    ;
    | Expr ';' {
        // Expr归约到Statement时要执行的语义动作程序
        // 表达式语句，不显示表达式的值

        // 创建一个AST_OP_EXPR类型的中间节点，孩子为Expr($1)
        $$ = new_ast_node(ast_operator_type::AST_OP_EXPR, $1, nullptr);
    }
    | Expr {
        // Expr归约到Statement时要执行的语义动作程序
        // 表达式语句，需要显示表达式的值

        // 创建一个AST_OP_EXPR_SHOW类型的中间节点，孩子为Expr($1)
        $$ = new_ast_node(ast_operator_type::AST_OP_EXPR_SHOW, $1, nullptr);
    }
    | T_RETURN Expr ';' {
        // 返回语句
        $$ = new_ast_node(ast_operator_type::AST_OP_RETURN_STATEMENT, $2, nullptr);
    } 
	| T_IF Expr ';' {
		$$ = new_ast_node(ast_operator_type::AST_OP_ONLY_IF,$2,nullptr);
	}
	| T_IF Expr Statement {
		$$ = new_ast_node(ast_operator_type::AST_OP_ONLY_IF,$2,$3,nullptr);
	}
	| T_IF Expr Statement T_ELSE Statement {
		$$ = new_ast_node(ast_operator_type::AST_OP_ONLY_IF_ELSE,$2,$3,$5,nullptr);
	}
	| T_WHILE Expr Statement{
		$$ = new_ast_node(ast_operator_type::AST_OP_WHILE,$2,$3,nullptr);
	}
	| T_BREAK ';'{
		$$ = new_ast_node(ast_operator_type::AST_OP_BREAK,nullptr,nullptr);
    }
	| T_CONTINUE ';'{
		$$ = new_ast_node(ast_operator_type::AST_OP_CONTINUE,nullptr,nullptr);
    }
	| T_GOTO T_ID ';' {
		$$ = new_ast_node(ast_operator_type::AST_OP_GOTO,nullptr,nullptr);
		$$->label = new Label($2.id);
	}
	| T_ID ':' 
	{
		$$ = new_ast_node(ast_operator_type::AST_OP_LABEL,nullptr,nullptr);
		$$->label = new Label($1.id);
	}
	| Decl ';'{
		$$ = $1;
	}
	| E_IF Expr ';' {
		struct cpmplex_error struct_E_IF1{"Expr error: if syntax error",0};
		yyerror(errormsg(&struct_E_IF1));
	}
	| E_IF Expr Statement {
		struct cpmplex_error struct_E_IF1{"Expr error: if syntax error",1};
		yyerror(errormsg(&struct_E_IF1));
	}
	| E_IF Expr Statement T_ELSE Statement {
		struct cpmplex_error struct_E_IF1{"Expr error: if syntax error",2};
		yyerror(errormsg(&struct_E_IF1));
	}
    ;

Assign : T_ID '=' Expr {

		ast_node * id_node = new_ast_leaf_node(var_id_attr{$1.id, $1.lineno,is_global,is_const});
		free($1.id);
		$$ = new_ast_node(ast_operator_type::AST_OP_ASSIGN, id_node, $3, nullptr);

	} | T_ID ArrayParams '=' Expr
	{ 
		ast_node * id_node = new_ast_leaf_node(var_id_attr{$1.id, $1.lineno,is_global,is_const});
		ast_node * arr_node = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,$2,nullptr);
		pointer_offset = 4;
		$$ = new_ast_node(ast_operator_type::AST_OP_ASSIGN, arr_node, $4, nullptr);
		free($1.id);
	} 

Decl : VarDecl {
		switch (global_type){
			case 1:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,$1);
			break;
			case 2:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_FLOAT,$1);
			break;
			case 3:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_ARR_I32,$1);
			$$->pointer_offset = pointer_offset;
			break;
			default:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,$1);
			break;
		}
		is_const = 0;
		global_type = 1;
	}
	| T_CONST VarDecl 
	{
		if($2->sons.size() == 0) $2->is_const = 1;
		else{
			for(int i=0;i<$2->sons.size();i++){
				if($2->sons[i]->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) $2->sons[i]->is_const = 1;
			}
		}
		switch (global_type){
			case 1:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,$2);
			break;
			case 2:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_FLOAT,$2);
			break;
			case 3:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_ARR_I32,$2);
			$$->pointer_offset = pointer_offset;
			break;
			default:
			$$ = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,$2);
			break;
		}
		is_const = 0;
		global_type = 1;
	}
	| Decl ',' T_ID  {
		// 每个生命的是否为常量由第一个变量决定
		if($1->sons[0]->sons.size() == 0) is_const = $1->sons[0]->is_const;
		else{
			for(int i=0;$1->sons[0]->sons.size();i++){
				if($1->sons[i]->sons[i]->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) 
				{
					is_const = $1->sons[0]->sons[i]->is_const;
					break;
				}
			}
		}
		ast_node * leaf = new_ast_leaf_node(var_id_attr{$3.id, $3.lineno,is_global,is_const});
		free($3.id);
		$$ = insert_ast_node($1,leaf);
		is_const = 0;
	}
	| Decl ',' T_ID '=' Expr {
		// 每个生命的是否为常量由第一个变量决定
		if($1->sons[0]->sons.size() == 0) is_const = $1->sons[0]->is_const;
		else{
			for(int i=0;$1->sons[0]->sons.size();i++){
				if($1->sons[i]->sons[i]->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) 
				{
					is_const = $1->sons[0]->sons[i]->is_const;
					break;
				}
			}
		}
		ast_node * leaf = new_ast_leaf_node(var_id_attr{$3.id, $3.lineno,is_global,is_const});
		free($3.id);
		ast_node * assign = new_ast_node(ast_operator_type::AST_OP_ASSIGN,leaf,$5,nullptr);
		$$ = insert_ast_node($1,assign);
		is_const = 0;
	};
	

VarDecl : 
	INT T_ID 
	{
		// 变量作为声明类型的子节点
		$$ = new_ast_leaf_node(var_id_attr{$2.id, $2.lineno,is_global,is_const});
		free($2.id);
		global_type = 1;
	} 
	| INT T_ID '=' Expr{
		 ast_node * id_node = new_ast_leaf_node(var_id_attr{$2.id, $2.lineno,is_global,is_const});
		free($2.id);
        $$ = new_ast_node(ast_operator_type::AST_OP_ASSIGN, id_node, $4, nullptr);
		global_type = 1;

	}
	
	| FLOAT T_ID {
        // 变量作为声明类型的子节点
		$$ = new_ast_leaf_node(var_id_attr{$2.id, $2.lineno,is_global,is_const});
		free($2.id);
		global_type = 2;
	}
	| INT T_ID ArrayParams 
	{
		ast_node * id_node = new_ast_leaf_node(var_id_attr{$2.id, $2.lineno,is_global,is_const});
		$$ = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,$3,nullptr);
		pointer_offset = 4;
		free($2.id);
		global_type = 3;
	}
	| INT T_ID ArrayParams '=' Expr 
	{	
		ast_node * id_node = new_ast_leaf_node(var_id_attr{$2.id, $2.lineno,is_global,is_const});
		ast_node * arr_node = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,$3,nullptr);
		pointer_offset = 4;
		$$ = new_ast_node(ast_operator_type::AST_OP_ASSIGN, arr_node, $5, nullptr);
		free($2.id);
		global_type = 3;
	}

;

/*expr是不含有括号的*/
Expr : AddExp {
        $$ = $1;
    }| Cond{
		$$ = $1;
	}| "(" Expr ")"{
		$$ = $2;
	}
    ;

Cond : LorExp{
		$$ = $1;
	}
	;

LorExp : LAndExp {
	    $$ = $1;
	}| LorExp T_LOR LAndExp {
		$$ = new_ast_node(ast_operator_type::AST_OP_LOR,$1,$3,nullptr);
	}
	;

LAndExp : EqExp{
		$$ = $1;
	}| LAndExp T_LAND EqExp {
		$$ = new_ast_node(ast_operator_type::AST_OP_LAND,$1,$3,nullptr);
	}
	;

EqExp : RelExp {
		$$ = $1;
	} | EqExp T_EQ RelExp {
		$$ = new_ast_node(ast_operator_type::AST_OP_EQ,$1,$3,nullptr);
	} | EqExp T_NE RelExp {
		$$ = new_ast_node(ast_operator_type::AST_OP_NE,$1,$3,nullptr);
	}
	;


RelExp : RelExp T_GN AddExp{
				$$ = new_ast_node(ast_operator_type::AST_OP_GN,$1,$3,nullptr);
		} | RelExp T_LN AddExp{
				$$ = new_ast_node(ast_operator_type::AST_OP_LN,$1,$3,nullptr);
		} | RelExp T_LE AddExp{
				$$ = new_ast_node(ast_operator_type::AST_OP_LE,$1,$3,nullptr);
		} | RelExp T_GE AddExp{
				$$ = new_ast_node(ast_operator_type::AST_OP_GE,$1,$3,nullptr);
		} | AddExp {
				$$ = $1;
		}
		;
/* 加法表达式 
优先度倒数第一
第一接近root
加法，减法
*/

AddExp : AddExp T_ADD MulExp {
        /* Expr = Expr + Term */

        // 创建一个AST_OP_ADD类型的中间节点，孩子为Expr($1)和Term($3)
        $$ = new_ast_node(ast_operator_type::AST_OP_ADD, $1, $3, nullptr);
    }
    | AddExp T_SUB MulExp {
        /* Expr = Expr + Term */

        // 创建一个AST_OP_ADD类型的中间节点，孩子为Expr($1)和Term($3)
        $$ = new_ast_node(ast_operator_type::AST_OP_SUB, $1, $3, nullptr);
    }
    | MulExp {
        /* Expr = Term */
        $$ = $1;
    }
    ;

/* 乘法表达式 written by zhaozihe 4-16
优先度倒数第二
第二接近root
乘，除，取模
*/
MulExp: MulExp '*' UnaryExp{
	/* Expr = Expr * Term */
		$$ = new_ast_node(ast_operator_type::AST_OP_MUL, $1, $3, nullptr);
	} |	MulExp T_DIV UnaryExp{
		$$ = new_ast_node(ast_operator_type::AST_OP_DIV, $1, $3, nullptr);

	} |	MulExp T_REM UnaryExp{
		$$ = new_ast_node(ast_operator_type::AST_OP_REM, $1, $3, nullptr);
	}
	 | UnaryExp{
		$$ = $1;
	}
	;




// 单元式


UnaryExp : PrimaryExp {
        $$ = $1;
    }
    | T_ID '(' ')' {
        // 用户自定义的不含实参的函数调用
        $$ = create_func_call($1.lineno, $1.id, nullptr);
    }
    | T_ID '(' RealParamList ')' {
        // 用户自定义的含有实参的参数调用
        $$ = create_func_call($1.lineno, $1.id, $3);
    }
	| T_SUB UnaryExp %prec T_SUB{
        $$ = new_ast_node(ast_operator_type::AST_OP_NEG_I, $2, nullptr);
	}
	| T_ADD UnaryExp %prec T_ADD{
        $$ = $2;
	}
	| T_ADD UnaryExp %prec T_ADD{
		$$ = new_ast_node(ast_operator_type::AST_OP_ADD, $2, nullptr);
	}
	| T_NOT UnaryExp %prec T_NOT{
		$$ = new_ast_node(ast_operator_type::AST_OP_NOT, $2, nullptr);
	}
	| UnaryExp{
		$$ = $1;
	}


PrimaryExp :  '(' Expr ')' {
	
        /* PrimaryExp = Expr */
        $$ = $2;
    }
    | T_DIGIT {
        // 无符号整数识别

        // 终结符作为抽象语法树的叶子节点进行创建
        $$ = new_ast_leaf_node(digit_int_attr{$1.val, $1.lineno});
    }
    | LVal  {
        // 左值
        $$ = $1;
    }
	| T_FLOAT{
		// 浮点数识别

		// 终结符作为抽象语法树的叶子节点进行创建
		$$ = new_ast_leaf_node(digit_real_attr{$1.val, $1.lineno})
	}
	| T_Boolean{
		// bool类型识别

		$$ = new_ast_leaf_node(digit_bool_attr{$1.val, $1.lineno})
	}
	| '{' ArrayExp '}'
	{
		$$ = $2;
	}
	| T_ID ArrayParams
	{	
		ast_node * id_node = new_ast_leaf_node(var_id_attr{$1.id, $1.lineno,is_global,is_const});
		$$ = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,$2,nullptr);
		pointer_offset = 4;
		free($1.id);
	}
    ;

ArrayExp : PrimaryExp  {
		// 第一个/单参数时，创建参数列表指针域
        $$ = create_contain_node(ast_operator_type::AST_OP_ArrayExp, $1);
    }
    | ArrayExp ',' PrimaryExp {
		// 多参数时，在第一个参数，在参数列表指针域内添加子节点，参数指针域为父节点，参数为子节点
        $$ = insert_ast_node($1, $3);
    }
    ;

/*基础数据类型int/float*/

LVal : T_ID {
        // 终结符作为抽象语法树的叶子节点进行创建
        $$ = new_ast_leaf_node(var_id_attr{$1.id, $1.lineno,is_global,is_const});

		// 对于字符型字面量的字符串空间需要释放，因词法用到了strdup进行了字符串复制
		free($1.id);
		is_const = 0;
    }
/* 实参列表 */
RealParamList : Expr {
        $$ = create_contain_node(ast_operator_type::AST_OP_FUNC_REAL_PARAMS, $1);
    }
    | RealParamList ',' Expr {
        $$ = insert_ast_node($1, $3);
    }
    ;

//数组参数
ArrayParams : ArrayParam  {
	//当有一维时，创建数组维数列表
	    $$ = create_contain_node(ast_operator_type::AST_OP_ARRAY_PARAMS, $1);
    }
    | ArrayParams  ArrayParam {
	//多维度时
	$$ = insert_ast_node($1, $2);
}

ArrayParam : '[' Expr ']'{
	$$ = $2;
}


%%

// 语法识别错误要调用函数的定义
void yyerror(char * msg)
{
    printf("Line %d: %s\n", yylineno, msg);
}


char * errormsg(struct cpmplex_error * err){
	std::string msg = "\t";
	msg = msg + err->msg + "\neror code:\t" + std::to_string(err->code) + "\n";
	static char buffer[3000];
	strcpy(buffer, msg.c_str());
	return buffer;
}