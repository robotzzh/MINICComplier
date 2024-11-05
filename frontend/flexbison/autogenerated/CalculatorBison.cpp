/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_DIGIT = 258,
     T_FLOAT = 259,
     T_ID = 260,
     T_Boolean = 261,
     T_FUNC = 262,
     T_RETURN = 263,
     T_ADD = 264,
     T_SUB = 265,
     T_MUL = 266,
     T_DIV = 267,
     T_REM = 268,
     T_EQ = 269,
     T_NE = 270,
     T_LE = 271,
     T_LN = 272,
     T_GE = 273,
     T_GN = 274,
     T_LAND = 275,
     T_LOR = 276,
     T_IF = 277,
     T_ELSE = 278,
     T_WHILE = 279,
     T_BREAK = 280,
     T_CONTINUE = 281,
     T_GOTO = 282,
     T_VOID = 283,
     E_IF = 284,
     T_CONST = 285,
     INT = 286,
     FLOAT = 287,
     T_POINTER = 288,
     T_NOT = 289
   };
#endif
/* Tokens.  */
#define T_DIGIT 258
#define T_FLOAT 259
#define T_ID 260
#define T_Boolean 261
#define T_FUNC 262
#define T_RETURN 263
#define T_ADD 264
#define T_SUB 265
#define T_MUL 266
#define T_DIV 267
#define T_REM 268
#define T_EQ 269
#define T_NE 270
#define T_LE 271
#define T_LN 272
#define T_GE 273
#define T_GN 274
#define T_LAND 275
#define T_LOR 276
#define T_IF 277
#define T_ELSE 278
#define T_WHILE 279
#define T_BREAK 280
#define T_CONTINUE 281
#define T_GOTO 282
#define T_VOID 283
#define E_IF 284
#define T_CONST 285
#define INT 286
#define FLOAT 287
#define T_POINTER 288
#define T_NOT 289




/* Copy the first part of user declarations.  */
#line 1 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 43 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
{
    class ast_node * node;

    struct digit_int_attr integer_num;
    struct digit_real_attr float_num;
    struct var_id_attr var_id;
	struct digit_bool_attr bool_num;
}
/* Line 193 of yacc.c.  */
#line 214 "/Users/zzh/compiler/April/calculator/frontend/flexbison/autogenerated/CalculatorBison.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 227 "/Users/zzh/compiler/April/calculator/frontend/flexbison/autogenerated/CalculatorBison.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   382

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNRULES -- Number of states.  */
#define YYNSTATES  187

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,    37,    39,     2,    38,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    42,    35,
       2,    43,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    46,     2,    47,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      44,    45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    15,    21,    28,    34,
      41,    47,    54,    56,    60,    62,    63,    65,    68,    71,
      75,    78,    82,    84,    87,    92,    96,    98,   100,   103,
     105,   108,   111,   113,   117,   121,   125,   131,   135,   138,
     141,   145,   148,   151,   155,   159,   165,   169,   174,   176,
     179,   183,   189,   192,   197,   200,   204,   210,   212,   214,
     218,   220,   222,   226,   228,   232,   234,   238,   242,   246,
     250,   254,   258,   260,   264,   268,   270,   274,   278,   282,
     284,   286,   290,   295,   298,   301,   304,   307,   309,   313,
     315,   317,   319,   321,   325,   328,   330,   334,   336,   338,
     342,   344,   347
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      49,     0,    -1,    50,    -1,    49,    50,    -1,    59,    35,
      -1,    49,    59,    35,    -1,    28,     5,    36,    37,    54,
      -1,    28,     5,    36,    51,    37,    54,    -1,    31,     5,
      36,    37,    54,    -1,    31,     5,    36,    51,    37,    54,
      -1,    32,     5,    36,    37,    54,    -1,    32,     5,    36,
      51,    37,    54,    -1,    52,    -1,    51,    38,    52,    -1,
      53,    -1,    -1,     5,    -1,    31,     5,    -1,    32,     5,
      -1,    31,    39,     5,    -1,    40,    41,    -1,    40,    55,
      41,    -1,    56,    -1,    55,    56,    -1,    55,    38,    58,
      35,    -1,    55,    38,    58,    -1,    57,    -1,    58,    -1,
      58,    35,    -1,    54,    -1,    57,    54,    -1,    61,    35,
      -1,    61,    -1,     8,    61,    35,    -1,    22,    61,    35,
      -1,    22,    61,    57,    -1,    22,    61,    57,    23,    57,
      -1,    24,    61,    57,    -1,    25,    35,    -1,    26,    35,
      -1,    27,     5,    35,    -1,     5,    42,    -1,    59,    35,
      -1,    29,    61,    35,    -1,    29,    61,    57,    -1,    29,
      61,    57,    23,    57,    -1,     5,    43,    61,    -1,     5,
      74,    43,    61,    -1,    60,    -1,    30,    60,    -1,    59,
      38,     5,    -1,    59,    38,     5,    43,    61,    -1,    31,
       5,    -1,    31,     5,    43,    61,    -1,    32,     5,    -1,
      31,     5,    74,    -1,    31,     5,    74,    43,    61,    -1,
      67,    -1,    62,    -1,    44,    61,    45,    -1,    63,    -1,
      64,    -1,    63,    21,    64,    -1,    65,    -1,    64,    20,
      65,    -1,    66,    -1,    65,    14,    66,    -1,    65,    15,
      66,    -1,    66,    19,    67,    -1,    66,    17,    67,    -1,
      66,    16,    67,    -1,    66,    18,    67,    -1,    67,    -1,
      67,     9,    68,    -1,    67,    10,    68,    -1,    68,    -1,
      68,    39,    69,    -1,    68,    12,    69,    -1,    68,    13,
      69,    -1,    69,    -1,    70,    -1,     5,    36,    37,    -1,
       5,    36,    73,    37,    -1,    10,    69,    -1,     9,    69,
      -1,     9,    69,    -1,    34,    69,    -1,    69,    -1,    36,
      61,    37,    -1,     3,    -1,    72,    -1,     4,    -1,     6,
      -1,    40,    71,    41,    -1,     5,    74,    -1,    70,    -1,
      71,    38,    70,    -1,     5,    -1,    61,    -1,    73,    38,
      61,    -1,    75,    -1,    74,    75,    -1,    46,    61,    47,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   103,   103,   107,   110,   125,   143,   147,   151,   155,
     159,   163,   170,   174,   181,   183,   187,   190,   193,   196,
     202,   206,   213,   218,   224,   227,   234,   241,   245,   249,
     252,   256,   263,   270,   274,   277,   280,   283,   286,   289,
     292,   296,   301,   304,   308,   312,   318,   324,   333,   352,
     378,   395,   416,   423,   431,   437,   445,   458,   460,   462,
     467,   472,   474,   479,   481,   486,   488,   490,   496,   498,
     500,   502,   504,   514,   520,   526,   537,   540,   543,   546,
     557,   560,   564,   568,   571,   574,   577,   580,   585,   590,
     596,   600,   606,   611,   615,   624,   628,   636,   645,   648,
     654,   658,   663
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_DIGIT", "T_FLOAT", "T_ID",
  "T_Boolean", "T_FUNC", "T_RETURN", "T_ADD", "T_SUB", "T_MUL", "T_DIV",
  "T_REM", "T_EQ", "T_NE", "T_LE", "T_LN", "T_GE", "T_GN", "T_LAND",
  "T_LOR", "T_IF", "T_ELSE", "T_WHILE", "T_BREAK", "T_CONTINUE", "T_GOTO",
  "T_VOID", "E_IF", "T_CONST", "INT", "FLOAT", "T_POINTER", "T_NOT", "';'",
  "'('", "')'", "','", "'*'", "'{'", "'}'", "':'", "'='", "\"(\"", "\")\"",
  "'['", "']'", "$accept", "CompileUnit", "FuncDef", "FuncFormalParams",
  "FuncFormalParam", "FuncBasicParam", "Block", "BlockItemList",
  "BlockItem", "Statement", "Assign", "Decl", "VarDecl", "Expr", "Cond",
  "LorExp", "LAndExp", "EqExp", "RelExp", "AddExp", "MulExp", "UnaryExp",
  "PrimaryExp", "ArrayExp", "LVal", "RealParamList", "ArrayParams",
  "ArrayParam", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    59,    40,    41,    44,    42,
     123,   125,    58,    61,   290,   291,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    49,    49,    49,    50,    50,    50,    50,
      50,    50,    51,    51,    52,    52,    53,    53,    53,    53,
      54,    54,    55,    55,    55,    55,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    58,    58,    59,    59,
      59,    59,    60,    60,    60,    60,    60,    61,    61,    61,
      62,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      66,    66,    66,    67,    67,    67,    68,    68,    68,    68,
      69,    69,    69,    69,    69,    69,    69,    69,    70,    70,
      70,    70,    70,    70,    70,    71,    71,    72,    73,    73,
      74,    74,    75
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     3,     5,     6,     5,     6,
       5,     6,     1,     3,     1,     0,     1,     2,     2,     3,
       2,     3,     1,     2,     4,     3,     1,     1,     2,     1,
       2,     2,     1,     3,     3,     3,     5,     3,     2,     2,
       3,     2,     2,     3,     3,     5,     3,     4,     1,     2,
       3,     5,     2,     4,     2,     3,     5,     1,     1,     3,
       1,     1,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       1,     3,     4,     2,     2,     2,     2,     1,     3,     1,
       1,     1,     1,     3,     2,     1,     3,     1,     1,     3,
       1,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     2,     0,    48,     0,
       0,     0,    49,    52,    54,     1,     3,     0,     4,     0,
      15,    52,    54,    15,     0,     0,    55,   100,    15,     5,
      50,    16,     0,     0,     0,     0,    12,    14,     0,     0,
      89,    91,    97,    92,     0,     0,     0,     0,     0,     0,
      53,    58,    60,    61,    63,    65,    57,    75,    79,    80,
      90,     0,     0,   101,     0,     0,     0,    17,     0,    18,
       0,     6,     0,    15,     8,     0,     0,    94,    84,    83,
      86,     0,    97,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
      56,    10,     0,    51,    19,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    29,     0,    22,    26,    27,
       0,    32,     7,    13,     9,    81,    98,     0,    88,     0,
      93,    59,    62,    72,    64,    66,    67,    70,    69,    71,
      68,    73,    74,    77,    78,    76,    11,    41,     0,    94,
       0,     0,     0,    38,    39,     0,     0,    80,     0,    21,
      23,    30,    28,    42,    31,    82,     0,    96,    46,     0,
      33,    34,    35,    37,    40,    43,    44,     0,    25,    99,
      47,     0,     0,     0,    24,    36,    45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,     6,    35,    36,    37,   115,   116,   117,   118,
     119,   120,     8,   121,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    84,    60,   127,    26,    27
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -54
static const yytype_int16 yypact[] =
{
     106,    10,   183,    12,    33,    34,   -54,    20,   -54,   -29,
      35,    44,   -54,    61,    32,   -54,   -54,   116,   -54,    73,
      15,    98,   -54,    40,   338,   338,   112,   -54,    49,   -54,
      39,   -54,    -2,    88,    65,   124,   -54,   -54,    65,   179,
     -54,   -54,   -30,   -54,    23,    23,    23,   338,     8,   338,
     -54,   -54,    93,    99,   204,   130,   336,    -4,   -54,   -54,
     -54,    62,   338,   -54,    65,   184,   338,   -54,   128,   -54,
     161,   -54,    65,    56,   -54,    65,   296,    75,   -54,   -54,
     -54,   113,    75,   -54,   118,   123,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    23,   -54,
     -54,   -54,    65,   -54,   -54,    89,   338,   338,   338,   137,
     147,   189,   338,   161,   -54,   -54,    86,   -54,    65,   154,
     146,   163,   -54,   -54,   -54,   -54,   -54,   186,   -54,     8,
     -54,   -54,    99,   231,   204,   130,   130,   231,   231,   231,
     231,    -4,    -4,   -54,   -54,   -54,   -54,   -54,   338,   153,
     169,   203,   287,   -54,   -54,   175,   245,   -54,   215,   -54,
     -54,   -54,   -54,   -54,   -54,   -54,   338,   -54,   -54,   338,
     -54,   -54,   -19,    65,   -54,   -54,    13,   157,   191,   -54,
     -54,   287,   287,   153,   -54,    65,    65
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -54,   -54,   226,     2,   173,   -54,   -33,   -54,   120,   -53,
      84,    79,   250,    -6,   -54,   -54,   172,   174,   156,    87,
     162,   -22,   -46,   -54,   -54,   -54,   -32,   -26
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -73
static const yytype_int16 yytable[] =
{
      63,    71,    83,    67,   181,    74,    76,    20,    96,    97,
      77,    40,    41,    82,    43,     9,    25,    13,    50,    61,
      31,    70,    78,    79,    80,    39,    40,    41,    42,    43,
      65,   101,    44,    45,    15,    98,   182,    68,    14,   122,
      21,    81,   124,    85,    47,    31,    32,    33,    48,    22,
      77,    63,    34,    70,    31,    18,   100,    46,    19,    47,
     103,    31,     1,    48,     2,     3,     4,   157,    28,   146,
     126,    32,    33,   149,   143,   144,   145,    38,    30,     7,
      32,    33,    66,   167,    17,   161,    64,    32,    33,    40,
      41,   105,    43,    69,   106,    44,    45,    23,   172,   173,
     150,   151,   152,   176,    24,    70,   156,    25,   107,    99,
     108,   109,   110,   111,    86,   112,     2,    10,    11,    87,
      46,    25,    47,    63,   158,    76,   113,   159,   185,   186,
      49,   147,   148,   104,     1,    25,     2,     3,     4,   161,
     161,    24,   168,   161,    25,   183,    90,    91,    92,    93,
     128,    29,   161,   161,    19,    62,   129,    63,    25,   130,
     179,    72,    73,   180,    40,    41,   105,    43,   131,   106,
      44,    45,   153,   133,   133,   133,   133,   137,   138,   139,
     140,   163,   154,   107,    19,   108,   109,   110,   111,   162,
     112,     2,    10,    11,   155,    46,   169,    47,   164,    25,
     148,   113,   114,    25,   170,    49,    40,    41,   105,    43,
     174,   106,    44,    45,    10,    11,    75,    73,    88,    89,
     177,   102,    73,   165,   166,   107,   184,   108,   109,   110,
     111,    16,   112,     2,    10,    11,   160,    46,   171,    47,
      94,    95,   178,   113,   135,   136,   123,    49,    40,    41,
     105,    43,    12,   106,    44,    45,   141,   142,   132,     0,
       0,   134,     0,     0,     0,     0,     0,   107,     0,   108,
     109,   110,   111,     0,   112,     2,    10,    11,     0,    46,
     175,    47,     0,     0,     0,   113,     0,     0,     0,    49,
      40,    41,   105,    43,     0,   106,    44,    45,     0,    40,
      41,    42,    43,     0,     0,    44,    45,     0,     0,   107,
       0,   108,   109,   110,   111,     0,   112,     2,    10,    11,
       0,    46,     0,    47,     0,     0,     0,   113,     0,     0,
      46,    49,    47,   125,     0,     0,    48,     0,     0,     0,
      49,    40,    41,    42,    43,    94,    95,    44,    45,     0,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,     0,     0,    48,     0,
       0,     0,    49
};

static const yytype_int16 yycheck[] =
{
      26,    34,    48,     5,    23,    38,    36,    36,    12,    13,
      42,     3,     4,     5,     6,     5,    46,     5,    24,    25,
       5,    40,    44,    45,    46,    23,     3,     4,     5,     6,
      28,    64,     9,    10,     0,    39,    23,    39,     5,    72,
       5,    47,    75,    49,    36,     5,    31,    32,    40,     5,
      82,    77,    37,    40,     5,    35,    62,    34,    38,    36,
      66,     5,    28,    40,    30,    31,    32,   113,    36,   102,
      76,    31,    32,   105,    96,    97,    98,    37,     5,     0,
      31,    32,    43,   129,     5,   118,    37,    31,    32,     3,
       4,     5,     6,     5,     8,     9,    10,    36,   151,   152,
     106,   107,   108,   156,    43,    40,   112,    46,    22,    47,
      24,    25,    26,    27,    21,    29,    30,    31,    32,    20,
      34,    46,    36,   149,    38,    36,    40,    41,   181,   182,
      44,    42,    43,     5,    28,    46,    30,    31,    32,   172,
     173,    43,   148,   176,    46,   177,    16,    17,    18,    19,
      37,    35,   185,   186,    38,    43,    38,   183,    46,    41,
     166,    37,    38,   169,     3,     4,     5,     6,    45,     8,
       9,    10,    35,    86,    87,    88,    89,    90,    91,    92,
      93,    35,    35,    22,    38,    24,    25,    26,    27,    35,
      29,    30,    31,    32,     5,    34,    43,    36,    35,    46,
      43,    40,    41,    46,    35,    44,     3,     4,     5,     6,
      35,     8,     9,    10,    31,    32,    37,    38,    14,    15,
       5,    37,    38,    37,    38,    22,    35,    24,    25,    26,
      27,     5,    29,    30,    31,    32,   116,    34,    35,    36,
       9,    10,   158,    40,    88,    89,    73,    44,     3,     4,
       5,     6,     2,     8,     9,    10,    94,    95,    86,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    22,    -1,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    -1,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,
       3,     4,     5,     6,    -1,     8,     9,    10,    -1,     3,
       4,     5,     6,    -1,    -1,     9,    10,    -1,    -1,    22,
      -1,    24,    25,    26,    27,    -1,    29,    30,    31,    32,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      34,    44,    36,    37,    -1,    -1,    40,    -1,    -1,    -1,
      44,     3,     4,     5,     6,     9,    10,     9,    10,    -1,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    28,    30,    31,    32,    49,    50,    59,    60,     5,
      31,    32,    60,     5,     5,     0,    50,    59,    35,    38,
      36,     5,     5,    36,    43,    46,    74,    75,    36,    35,
       5,     5,    31,    32,    37,    51,    52,    53,    37,    51,
       3,     4,     5,     6,     9,    10,    34,    36,    40,    44,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      72,    61,    43,    75,    37,    51,    43,     5,    39,     5,
      40,    54,    37,    38,    54,    37,    36,    74,    69,    69,
      69,    61,     5,    70,    71,    61,    21,    20,    14,    15,
      16,    17,    18,    19,     9,    10,    12,    13,    39,    47,
      61,    54,    37,    61,     5,     5,     8,    22,    24,    25,
      26,    27,    29,    40,    41,    54,    55,    56,    57,    58,
      59,    61,    54,    52,    54,    37,    61,    73,    37,    38,
      41,    45,    64,    67,    65,    66,    66,    67,    67,    67,
      67,    68,    68,    69,    69,    69,    54,    42,    43,    74,
      61,    61,    61,    35,    35,     5,    61,    70,    38,    41,
      56,    54,    35,    35,    35,    37,    38,    70,    61,    43,
      35,    35,    57,    57,    35,    35,    57,     5,    58,    61,
      61,    23,    23,    74,    35,    57,    57
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 103 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT, (yyvsp[(1) - (1)].node));
        ast_root = (yyval.node);
    ;}
    break;

  case 3:
#line 107 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = insert_ast_node((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node));
    ;}
    break;

  case 4:
#line 110 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT, (yyvsp[(1) - (2)].node));
        ast_root = (yyval.node);
			for(int i=0;i<(yyvsp[(1) - (2)].node)->sons.size();i++){
				if((yyvsp[(1) - (2)].node)->sons[i]->sons.size() == 0) (yyvsp[(1) - (2)].node)->sons[i]->is_global = 1;
				else{
					if((yyvsp[(1) - (2)].node)->sons[i]->sons[0]->sons.size() == 0) {
						(yyvsp[(1) - (2)].node)->sons[i]->sons[0]->is_global = 1;
					}
					else {
						(yyvsp[(1) - (2)].node)->sons[i]->sons[0]->sons[0]->is_global = 1;
					}
				}
			}
		;}
    break;

  case 5:
#line 125 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = insert_ast_node((yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node));
		for(int i=0;i<(yyvsp[(2) - (3)].node)->sons.size();i++){
				if((yyvsp[(2) - (3)].node)->sons[i]->sons.size() == 0) (yyvsp[(2) - (3)].node)->sons[i]->is_global = 1;
				else{
					if((yyvsp[(2) - (3)].node)->sons[i]->sons[0]->sons.size() == 0) {
						(yyvsp[(2) - (3)].node)->sons[i]->sons[0]->is_global = 1;
					}
					else {
						(yyvsp[(2) - (3)].node)->sons[i]->sons[0]->sons[0]->is_global = 1;
					}
				}
			}
	;}
    break;

  case 6:
#line 143 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 无返回值函数构造，调用创建函数定义类型（非终结符），默认形参为空
        (yyval.node) = create_func_def((yyvsp[(2) - (5)].var_id).lineno, (yyvsp[(2) - (5)].var_id).id, (yyvsp[(5) - (5)].node), nullptr,0);
    ;}
    break;

  case 7:
#line 147 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	// 含参数函数构造，调用创建函数定义类型（非终结符），默认形参为$4
        (yyval.node) = create_func_def((yyvsp[(2) - (6)].var_id).lineno, (yyvsp[(2) - (6)].var_id).id, (yyvsp[(6) - (6)].node), (yyvsp[(4) - (6)].node),0);
    ;}
    break;

  case 8:
#line 151 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 无返回值函数构造，调用创建函数定义类型（非终结符），默认形参为空
        (yyval.node) = create_func_def((yyvsp[(2) - (5)].var_id).lineno, (yyvsp[(2) - (5)].var_id).id, (yyvsp[(5) - (5)].node), nullptr, 1);
    ;}
    break;

  case 9:
#line 155 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	// 含参数函数构造，调用创建函数定义类型（非终结符），默认形参为$4
        (yyval.node) = create_func_def((yyvsp[(2) - (6)].var_id).lineno, (yyvsp[(2) - (6)].var_id).id, (yyvsp[(6) - (6)].node), (yyvsp[(4) - (6)].node), 1);
    ;}
    break;

  case 10:
#line 159 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 无返回值函数构造，调用创建函数定义类型（非终结符），默认形参为空
        (yyval.node) = create_func_def((yyvsp[(2) - (5)].var_id).lineno, (yyvsp[(2) - (5)].var_id).id, (yyvsp[(5) - (5)].node), nullptr,2);
    ;}
    break;

  case 11:
#line 163 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	// 含参数函数构造，调用创建函数定义类型（非终结符），默认形参为$4
        (yyval.node) = create_func_def((yyvsp[(2) - (6)].var_id).lineno, (yyvsp[(2) - (6)].var_id).id, (yyvsp[(6) - (6)].node), (yyvsp[(4) - (6)].node),2);
    ;}
    break;

  case 12:
#line 170 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 第一个/单参数时，创建参数列表指针域
        (yyval.node) = create_contain_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS, (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 13:
#line 174 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 多参数时，在第一个参数，在参数列表指针域内添加子节点，参数指针域为父节点，参数为子节点
        (yyval.node) = insert_ast_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 14:
#line 181 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 16:
#line 187 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = create_func_formal_param((yyvsp[(1) - (1)].var_id).lineno, (yyvsp[(1) - (1)].var_id).id);
    ;}
    break;

  case 17:
#line 190 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = create_func_formal_param((yyvsp[(2) - (2)].var_id).lineno, (yyvsp[(2) - (2)].var_id).id,1);
	;}
    break;

  case 18:
#line 193 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = create_func_formal_param((yyvsp[(2) - (2)].var_id).lineno, (yyvsp[(2) - (2)].var_id).id,2);
	;}
    break;

  case 19:
#line 196 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = create_func_formal_param((yyvsp[(3) - (3)].var_id).lineno, (yyvsp[(3) - (3)].var_id).id,3);
	;}
    break;

  case 20:
#line 202 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 语句块没有语句
        (yyval.node) = nullptr;
    ;}
    break;

  case 21:
#line 206 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 语句块含有语句
        (yyval.node) = (yyvsp[(2) - (3)].node);
    ;}
    break;

  case 22:
#line 213 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 第一个左侧的孩子节点归约成Block父节点，后续语句可不断作为孩子追加到block中
        // 创建一个AST_OP_BLOCK类型的中间节点，孩子为Statement($1)
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_BLOCK, (yyvsp[(1) - (1)].node), nullptr);
    ;}
    break;

  case 23:
#line 218 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 采用左递归的文法产生式，可以使得Block节点在上个产生式创建，后续递归追加孩子节点
        // 请注意，不要采用右递归，左递归翻遍孩子的追加
        // BlockItem($2)作为Block($1)的孩子 
        (yyval.node) = insert_ast_node((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
    ;}
    break;

  case 24:
#line 224 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = insert_ast_node((yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));
	;}
    break;

  case 25:
#line 227 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = insert_ast_node((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));
	;}
    break;

  case 26:
#line 234 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 27:
#line 242 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 28:
#line 246 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (2)].node);
	;}
    break;

  case 29:
#line 249 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 30:
#line 252 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(2) - (2)].node);
	;}
    break;

  case 31:
#line 256 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // Expr归约到Statement时要执行的语义动作程序
        // 表达式语句，不显示表达式的值

        // 创建一个AST_OP_EXPR类型的中间节点，孩子为Expr($1)
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_EXPR, (yyvsp[(1) - (2)].node), nullptr);
    ;}
    break;

  case 32:
#line 263 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // Expr归约到Statement时要执行的语义动作程序
        // 表达式语句，需要显示表达式的值

        // 创建一个AST_OP_EXPR_SHOW类型的中间节点，孩子为Expr($1)
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_EXPR_SHOW, (yyvsp[(1) - (1)].node), nullptr);
    ;}
    break;

  case 33:
#line 270 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 返回语句
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_RETURN_STATEMENT, (yyvsp[(2) - (3)].node), nullptr);
    ;}
    break;

  case 34:
#line 274 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ONLY_IF,(yyvsp[(2) - (3)].node),nullptr);
	;}
    break;

  case 35:
#line 277 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ONLY_IF,(yyvsp[(2) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
	;}
    break;

  case 36:
#line 280 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ONLY_IF_ELSE,(yyvsp[(2) - (5)].node),(yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].node),nullptr);
	;}
    break;

  case 37:
#line 283 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_WHILE,(yyvsp[(2) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
	;}
    break;

  case 38:
#line 286 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_BREAK,nullptr,nullptr);
    ;}
    break;

  case 39:
#line 289 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_CONTINUE,nullptr,nullptr);
    ;}
    break;

  case 40:
#line 292 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_GOTO,nullptr,nullptr);
		(yyval.node)->label = new Label((yyvsp[(2) - (3)].var_id).id);
	;}
    break;

  case 41:
#line 297 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_LABEL,nullptr,nullptr);
		(yyval.node)->label = new Label((yyvsp[(1) - (2)].var_id).id);
	;}
    break;

  case 42:
#line 301 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (2)].node);
	;}
    break;

  case 43:
#line 304 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		struct cpmplex_error struct_E_IF1{"Expr error: if syntax error",0};
		yyerror(errormsg(&struct_E_IF1));
	;}
    break;

  case 44:
#line 308 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		struct cpmplex_error struct_E_IF1{"Expr error: if syntax error",1};
		yyerror(errormsg(&struct_E_IF1));
	;}
    break;

  case 45:
#line 312 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		struct cpmplex_error struct_E_IF1{"Expr error: if syntax error",2};
		yyerror(errormsg(&struct_E_IF1));
	;}
    break;

  case 46:
#line 318 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {

		ast_node * id_node = new_ast_leaf_node(var_id_attr{(yyvsp[(1) - (3)].var_id).id, (yyvsp[(1) - (3)].var_id).lineno,is_global,is_const});
		free((yyvsp[(1) - (3)].var_id).id);
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ASSIGN, id_node, (yyvsp[(3) - (3)].node), nullptr);

	;}
    break;

  case 47:
#line 325 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    { 
		ast_node * id_node = new_ast_leaf_node(var_id_attr{(yyvsp[(1) - (4)].var_id).id, (yyvsp[(1) - (4)].var_id).lineno,is_global,is_const});
		ast_node * arr_node = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,(yyvsp[(2) - (4)].node),nullptr);
		pointer_offset = 4;
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ASSIGN, arr_node, (yyvsp[(4) - (4)].node), nullptr);
		free((yyvsp[(1) - (4)].var_id).id);
	;}
    break;

  case 48:
#line 333 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		switch (global_type){
			case 1:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,(yyvsp[(1) - (1)].node));
			break;
			case 2:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_FLOAT,(yyvsp[(1) - (1)].node));
			break;
			case 3:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_ARR_I32,(yyvsp[(1) - (1)].node));
			(yyval.node)->pointer_offset = pointer_offset;
			break;
			default:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,(yyvsp[(1) - (1)].node));
			break;
		}
		is_const = 0;
		global_type = 1;
	;}
    break;

  case 49:
#line 353 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		if((yyvsp[(2) - (2)].node)->sons.size() == 0) (yyvsp[(2) - (2)].node)->is_const = 1;
		else{
			for(int i=0;i<(yyvsp[(2) - (2)].node)->sons.size();i++){
				if((yyvsp[(2) - (2)].node)->sons[i]->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) (yyvsp[(2) - (2)].node)->sons[i]->is_const = 1;
			}
		}
		switch (global_type){
			case 1:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,(yyvsp[(2) - (2)].node));
			break;
			case 2:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_FLOAT,(yyvsp[(2) - (2)].node));
			break;
			case 3:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_ARR_I32,(yyvsp[(2) - (2)].node));
			(yyval.node)->pointer_offset = pointer_offset;
			break;
			default:
			(yyval.node) = create_contain_node(ast_operator_type::AST_OP_DECLARE_INT,(yyvsp[(2) - (2)].node));
			break;
		}
		is_const = 0;
		global_type = 1;
	;}
    break;

  case 50:
#line 378 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 每个生命的是否为常量由第一个变量决定
		if((yyvsp[(1) - (3)].node)->sons[0]->sons.size() == 0) is_const = (yyvsp[(1) - (3)].node)->sons[0]->is_const;
		else{
			for(int i=0;(yyvsp[(1) - (3)].node)->sons[0]->sons.size();i++){
				if((yyvsp[(1) - (3)].node)->sons[i]->sons[i]->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) 
				{
					is_const = (yyvsp[(1) - (3)].node)->sons[0]->sons[i]->is_const;
					break;
				}
			}
		}
		ast_node * leaf = new_ast_leaf_node(var_id_attr{(yyvsp[(3) - (3)].var_id).id, (yyvsp[(3) - (3)].var_id).lineno,is_global,is_const});
		free((yyvsp[(3) - (3)].var_id).id);
		(yyval.node) = insert_ast_node((yyvsp[(1) - (3)].node),leaf);
		is_const = 0;
	;}
    break;

  case 51:
#line 395 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 每个生命的是否为常量由第一个变量决定
		if((yyvsp[(1) - (5)].node)->sons[0]->sons.size() == 0) is_const = (yyvsp[(1) - (5)].node)->sons[0]->is_const;
		else{
			for(int i=0;(yyvsp[(1) - (5)].node)->sons[0]->sons.size();i++){
				if((yyvsp[(1) - (5)].node)->sons[i]->sons[i]->node_type == ast_operator_type::AST_OP_LEAF_VAR_ID) 
				{
					is_const = (yyvsp[(1) - (5)].node)->sons[0]->sons[i]->is_const;
					break;
				}
			}
		}
		ast_node * leaf = new_ast_leaf_node(var_id_attr{(yyvsp[(3) - (5)].var_id).id, (yyvsp[(3) - (5)].var_id).lineno,is_global,is_const});
		free((yyvsp[(3) - (5)].var_id).id);
		ast_node * assign = new_ast_node(ast_operator_type::AST_OP_ASSIGN,leaf,(yyvsp[(5) - (5)].node),nullptr);
		(yyval.node) = insert_ast_node((yyvsp[(1) - (5)].node),assign);
		is_const = 0;
	;}
    break;

  case 52:
#line 417 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 变量作为声明类型的子节点
		(yyval.node) = new_ast_leaf_node(var_id_attr{(yyvsp[(2) - (2)].var_id).id, (yyvsp[(2) - (2)].var_id).lineno,is_global,is_const});
		free((yyvsp[(2) - (2)].var_id).id);
		global_type = 1;
	;}
    break;

  case 53:
#line 423 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		 ast_node * id_node = new_ast_leaf_node(var_id_attr{(yyvsp[(2) - (4)].var_id).id, (yyvsp[(2) - (4)].var_id).lineno,is_global,is_const});
		free((yyvsp[(2) - (4)].var_id).id);
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_ASSIGN, id_node, (yyvsp[(4) - (4)].node), nullptr);
		global_type = 1;

	;}
    break;

  case 54:
#line 431 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 变量作为声明类型的子节点
		(yyval.node) = new_ast_leaf_node(var_id_attr{(yyvsp[(2) - (2)].var_id).id, (yyvsp[(2) - (2)].var_id).lineno,is_global,is_const});
		free((yyvsp[(2) - (2)].var_id).id);
		global_type = 2;
	;}
    break;

  case 55:
#line 438 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		ast_node * id_node = new_ast_leaf_node(var_id_attr{(yyvsp[(2) - (3)].var_id).id, (yyvsp[(2) - (3)].var_id).lineno,is_global,is_const});
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,(yyvsp[(3) - (3)].node),nullptr);
		pointer_offset = 4;
		free((yyvsp[(2) - (3)].var_id).id);
		global_type = 3;
	;}
    break;

  case 56:
#line 446 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {	
		ast_node * id_node = new_ast_leaf_node(var_id_attr{(yyvsp[(2) - (5)].var_id).id, (yyvsp[(2) - (5)].var_id).lineno,is_global,is_const});
		ast_node * arr_node = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,(yyvsp[(3) - (5)].node),nullptr);
		pointer_offset = 4;
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ASSIGN, arr_node, (yyvsp[(5) - (5)].node), nullptr);
		free((yyvsp[(2) - (5)].var_id).id);
		global_type = 3;
	;}
    break;

  case 57:
#line 458 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 58:
#line 460 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 59:
#line 462 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(2) - (3)].node);
	;}
    break;

  case 60:
#line 467 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 61:
#line 472 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	    (yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 62:
#line 474 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_LOR,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
	;}
    break;

  case 63:
#line 479 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 64:
#line 481 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_LAND,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
	;}
    break;

  case 65:
#line 486 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 66:
#line 488 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_EQ,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
	;}
    break;

  case 67:
#line 490 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_NE,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
	;}
    break;

  case 68:
#line 496 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
				(yyval.node) = new_ast_node(ast_operator_type::AST_OP_GN,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
		;}
    break;

  case 69:
#line 498 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
				(yyval.node) = new_ast_node(ast_operator_type::AST_OP_LN,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
		;}
    break;

  case 70:
#line 500 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
				(yyval.node) = new_ast_node(ast_operator_type::AST_OP_LE,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
		;}
    break;

  case 71:
#line 502 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
				(yyval.node) = new_ast_node(ast_operator_type::AST_OP_GE,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),nullptr);
		;}
    break;

  case 72:
#line 504 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
				(yyval.node) = (yyvsp[(1) - (1)].node);
		;}
    break;

  case 73:
#line 514 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        /* Expr = Expr + Term */

        // 创建一个AST_OP_ADD类型的中间节点，孩子为Expr($1)和Term($3)
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_ADD, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), nullptr);
    ;}
    break;

  case 74:
#line 520 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        /* Expr = Expr + Term */

        // 创建一个AST_OP_ADD类型的中间节点，孩子为Expr($1)和Term($3)
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_SUB, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), nullptr);
    ;}
    break;

  case 75:
#line 526 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        /* Expr = Term */
        (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 76:
#line 537 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	/* Expr = Expr * Term */
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_MUL, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), nullptr);
	;}
    break;

  case 77:
#line 540 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_DIV, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), nullptr);

	;}
    break;

  case 78:
#line 543 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_REM, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), nullptr);
	;}
    break;

  case 79:
#line 546 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 80:
#line 557 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 81:
#line 560 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 用户自定义的不含实参的函数调用
        (yyval.node) = create_func_call((yyvsp[(1) - (3)].var_id).lineno, (yyvsp[(1) - (3)].var_id).id, nullptr);
    ;}
    break;

  case 82:
#line 564 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 用户自定义的含有实参的参数调用
        (yyval.node) = create_func_call((yyvsp[(1) - (4)].var_id).lineno, (yyvsp[(1) - (4)].var_id).id, (yyvsp[(3) - (4)].node));
    ;}
    break;

  case 83:
#line 568 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = new_ast_node(ast_operator_type::AST_OP_NEG_I, (yyvsp[(2) - (2)].node), nullptr);
	;}
    break;

  case 84:
#line 571 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = (yyvsp[(2) - (2)].node);
	;}
    break;

  case 85:
#line 574 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ADD, (yyvsp[(2) - (2)].node), nullptr);
	;}
    break;

  case 86:
#line 577 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_NOT, (yyvsp[(2) - (2)].node), nullptr);
	;}
    break;

  case 87:
#line 580 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(1) - (1)].node);
	;}
    break;

  case 88:
#line 585 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	
        /* PrimaryExp = Expr */
        (yyval.node) = (yyvsp[(2) - (3)].node);
    ;}
    break;

  case 89:
#line 590 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 无符号整数识别

        // 终结符作为抽象语法树的叶子节点进行创建
        (yyval.node) = new_ast_leaf_node(digit_int_attr{(yyvsp[(1) - (1)].integer_num).val, (yyvsp[(1) - (1)].integer_num).lineno});
    ;}
    break;

  case 90:
#line 596 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 左值
        (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 91:
#line 600 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 浮点数识别

		// 终结符作为抽象语法树的叶子节点进行创建
		(yyval.node) = new_ast_leaf_node(digit_real_attr{(yyvsp[(1) - (1)].float_num).val, (yyvsp[(1) - (1)].float_num).lineno})
	;}
    break;

  case 92:
#line 606 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// bool类型识别

		(yyval.node) = new_ast_leaf_node(digit_bool_attr{(yyvsp[(1) - (1)].bool_num).val, (yyvsp[(1) - (1)].bool_num).lineno})
	;}
    break;

  case 93:
#line 612 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		(yyval.node) = (yyvsp[(2) - (3)].node);
	;}
    break;

  case 94:
#line 616 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {	
		ast_node * id_node = new_ast_leaf_node(var_id_attr{(yyvsp[(1) - (2)].var_id).id, (yyvsp[(1) - (2)].var_id).lineno,is_global,is_const});
		(yyval.node) = new_ast_node(ast_operator_type::AST_OP_ARRAY,id_node,(yyvsp[(2) - (2)].node),nullptr);
		pointer_offset = 4;
		free((yyvsp[(1) - (2)].var_id).id);
	;}
    break;

  case 95:
#line 624 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 第一个/单参数时，创建参数列表指针域
        (yyval.node) = create_contain_node(ast_operator_type::AST_OP_ArrayExp, (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 96:
#line 628 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
		// 多参数时，在第一个参数，在参数列表指针域内添加子节点，参数指针域为父节点，参数为子节点
        (yyval.node) = insert_ast_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 97:
#line 636 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        // 终结符作为抽象语法树的叶子节点进行创建
        (yyval.node) = new_ast_leaf_node(var_id_attr{(yyvsp[(1) - (1)].var_id).id, (yyvsp[(1) - (1)].var_id).lineno,is_global,is_const});

		// 对于字符型字面量的字符串空间需要释放，因词法用到了strdup进行了字符串复制
		free((yyvsp[(1) - (1)].var_id).id);
		is_const = 0;
    ;}
    break;

  case 98:
#line 645 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = create_contain_node(ast_operator_type::AST_OP_FUNC_REAL_PARAMS, (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 99:
#line 648 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
        (yyval.node) = insert_ast_node((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 100:
#line 654 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	//当有一维时，创建数组维数列表
	    (yyval.node) = create_contain_node(ast_operator_type::AST_OP_ARRAY_PARAMS, (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 101:
#line 658 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	//多维度时
	(yyval.node) = insert_ast_node((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node));
;}
    break;

  case 102:
#line 663 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"
    {
	(yyval.node) = (yyvsp[(2) - (3)].node);
;}
    break;


/* Line 1267 of yacc.c.  */
#line 2514 "/Users/zzh/compiler/April/calculator/frontend/flexbison/autogenerated/CalculatorBison.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 668 "/Users/zzh/compiler/April/calculator/frontend/flexbison/Calculator.y"


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
