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
#define YYLSP_NEEDED 1



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     YYEOF = 0,
     DOTS = 258,
     EOL = 259,
     SPACES = 260,
     BOOLTRUE = 261,
     BOOLFALSE = 262,
     QUOTE = 263,
     NOT = 264,
     DOLLAR = 265,
     COMMA = 266,
     COLON = 267,
     SEMI = 268,
     LPAREN = 269,
     RPAREN = 270,
     LBRACK = 271,
     RBRACK = 272,
     LBRACE = 273,
     RBRACE = 274,
     DOT = 275,
     DOTQUOTE = 276,
     PLUS = 277,
     MINUS = 278,
     TIMES = 279,
     DOTTIMES = 280,
     KRONTIMES = 281,
     DIVIDE = 282,
     DOTDIVIDE = 283,
     CONTROLDIVIDE = 284,
     KRONDIVIDE = 285,
     RDIVIDE = 286,
     DOTRDIVIDE = 287,
     KRONRDIVIDE = 288,
     POWER = 289,
     DOTPOWER = 290,
     EQ = 291,
     NE = 292,
     LT = 293,
     LE = 294,
     GT = 295,
     GE = 296,
     AND = 297,
     OR = 298,
     ASSIGN = 299,
     IF = 300,
     THEN = 301,
     ELSE = 302,
     ELSEIF = 303,
     END = 304,
     SELECT = 305,
     CASE = 306,
     FUNCTION = 307,
     ENDFUNCTION = 308,
     FOR = 309,
     WHILE = 310,
     DO = 311,
     BREAK = 312,
     TRY = 313,
     CATCH = 314,
     RETURN = 315,
     STR = 316,
     ID = 317,
     VARINT = 318,
     VARFLOAT = 319,
     NUM = 320,
     PATH = 321,
     COMMENT = 322,
     BLOCKCOMMENT = 323,
     TOPLEVEL = 324,
     HIGHLEVEL = 325,
     UPLEVEL = 326,
     LISTABLE = 327,
     FUNCTIONCALL = 328
   };
#endif
/* Tokens.  */
#define YYEOF 0
#define DOTS 258
#define EOL 259
#define SPACES 260
#define BOOLTRUE 261
#define BOOLFALSE 262
#define QUOTE 263
#define NOT 264
#define DOLLAR 265
#define COMMA 266
#define COLON 267
#define SEMI 268
#define LPAREN 269
#define RPAREN 270
#define LBRACK 271
#define RBRACK 272
#define LBRACE 273
#define RBRACE 274
#define DOT 275
#define DOTQUOTE 276
#define PLUS 277
#define MINUS 278
#define TIMES 279
#define DOTTIMES 280
#define KRONTIMES 281
#define DIVIDE 282
#define DOTDIVIDE 283
#define CONTROLDIVIDE 284
#define KRONDIVIDE 285
#define RDIVIDE 286
#define DOTRDIVIDE 287
#define KRONRDIVIDE 288
#define POWER 289
#define DOTPOWER 290
#define EQ 291
#define NE 292
#define LT 293
#define LE 294
#define GT 295
#define GE 296
#define AND 297
#define OR 298
#define ASSIGN 299
#define IF 300
#define THEN 301
#define ELSE 302
#define ELSEIF 303
#define END 304
#define SELECT 305
#define CASE 306
#define FUNCTION 307
#define ENDFUNCTION 308
#define FOR 309
#define WHILE 310
#define DO 311
#define BREAK 312
#define TRY 313
#define CATCH 314
#define RETURN 315
#define STR 316
#define ID 317
#define VARINT 318
#define VARFLOAT 319
#define NUM 320
#define PATH 321
#define COMMENT 322
#define BLOCKCOMMENT 323
#define TOPLEVEL 324
#define HIGHLEVEL 325
#define UPLEVEL 326
#define LISTABLE 327
#define FUNCTIONCALL 328




/* Copy the first part of user declarations.  */

 // -*- C++ -*-
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2009 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#define YYERROR_VERBOSE 1

#define YYDEBUG 1

#define YYLTYPE Location

/*
** This build the tree in verbose mode
** for instance adding CommentExp
** where nothing is needed.
*/
#define BUILD_DEBUG_AST

#include <string>
#include <sstream>
#include <list>
#include "all.hxx"
#include "parse.hxx"
#include "parser.hxx"
#include "location.hxx"

#define StopOnError()					\
  {							\
    if(Parser::getInstance()->stopOnFirstError())	\
      {							\
	return Parser::getInstance()->getExitStatus();	\
      }							\
 }




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

{
  /* Tokens. */
  double		number;
  std::string*		str;
  std::string*		path;
  std::string*		comment;

  bool			mute;

  ast::vars_t*		t_list_var;
  ast::exps_t*		t_list_exp;
  ast::Exp*		t_exp;

  ast::SeqExp*		t_seq_exp;
  ast::ReturnExp*	t_return_exp;

  ast::IfExp*		t_if_exp;
  ast::WhileExp*	t_while_exp;
  ast::ForExp*		t_for_exp;
  ast::TryCatchExp*	t_try_exp;

  ast::CallExp*		t_call_exp;

  ast::MathExp*		t_math_exp;

  ast::OpExp*		t_op_exp;
  ast::OpExp::Oper	t_op_exp_oper;

  ast::AssignExp*	t_assign_exp;

  ast::StringExp*	t_string_exp;

  ast::ListExp*		t_implicit_list;

  ast::MatrixExp*	t_matrix_exp;
  ast::MatrixLineExp*	t_matrixline_exp;
  std::list<ast::MatrixLineExp *>* \
			t_list_mline;

  ast::CellExp*		t_cell_exp;

  ast::FunctionDec*	t_function_dec;

  ast::ArrayListExp*	t_arraylist_exp;
  ast::AssignListExp*	t_assignlist_exp;
  ast::ArrayListVar*	t_arraylist_var;
}
/* Line 187 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


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
# if YYENABLE_NLS
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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  104
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2544

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  320
/* YYNRULES -- Number of states.  */
#define YYNSTATES  501

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   328

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    17,    19,    22,
      26,    31,    35,    38,    40,    42,    44,    47,    50,    53,
      55,    57,    59,    61,    63,    65,    67,    69,    71,    73,
      75,    77,    79,    81,    84,    87,    89,    91,    93,    95,
      97,    99,   101,   103,   107,   111,   113,   116,   118,   120,
     122,   124,   128,   131,   136,   141,   146,   151,   156,   158,
     160,   162,   164,   165,   169,   173,   177,   181,   184,   193,
     204,   214,   221,   223,   227,   230,   231,   235,   237,   239,
     241,   244,   246,   249,   251,   252,   254,   256,   260,   264,
     268,   272,   274,   276,   278,   280,   282,   285,   288,   291,
     294,   297,   300,   304,   308,   312,   316,   320,   324,   328,
     332,   335,   338,   341,   344,   347,   350,   353,   356,   359,
     362,   365,   368,   371,   374,   377,   380,   383,   386,   389,
     392,   395,   398,   401,   404,   407,   410,   413,   416,   419,
     422,   426,   430,   432,   435,   439,   443,   447,   451,   454,
     457,   459,   461,   463,   465,   467,   469,   471,   473,   475,
     477,   479,   483,   487,   489,   493,   497,   501,   505,   509,
     513,   517,   522,   527,   533,   537,   542,   545,   549,   554,
     559,   565,   569,   574,   577,   580,   582,   584,   586,   589,
     592,   596,   601,   605,   610,   616,   620,   624,   627,   630,
     632,   634,   637,   639,   643,   647,   651,   655,   659,   663,
     667,   671,   675,   679,   683,   687,   689,   691,   695,   701,
     709,   716,   718,   719,   721,   722,   724,   727,   729,   732,
     734,   736,   739,   743,   746,   748,   749,   751,   754,   757,
     760,   764,   768,   773,   780,   786,   792,   800,   807,   816,
     818,   820,   822,   825,   828,   833,   839,   845,   852,   858,
     865,   867,   868,   870,   872,   874,   876,   879,   882,   885,
     888,   892,   900,   910,   912,   914,   916,   918,   921,   923,
     926,   928,   931,   932,   934,   935,   941,   943,   944,   946,
     948,   950,   953,   956,   958,   961,   964,   966,   969,   972,
     975,   979,   983,   986,   990,   994,  1002,  1011,  1020,  1030,
    1032,  1035,  1037,  1039,  1041,  1043,  1045,  1047,  1050,  1054,
    1056
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      75,     0,    -1,    76,    -1,     4,    76,    -1,    77,    -1,
      77,    79,    -1,    77,    79,    67,    -1,    79,    -1,    79,
      67,    -1,    77,    79,    78,    -1,    77,    79,    67,    78,
      -1,    79,    67,    78,    -1,    79,    78,    -1,    13,    -1,
      11,    -1,     4,    -1,    78,    13,    -1,    78,    11,    -1,
      78,     4,    -1,    87,    -1,    82,    -1,   110,    -1,   113,
      -1,   120,    -1,   126,    -1,   130,    -1,   133,    -1,   101,
      -1,    80,    -1,    57,    -1,   134,    -1,    67,    -1,     1,
      -1,    80,    81,    -1,    62,    81,    -1,    62,    -1,    63,
      -1,    65,    -1,    64,    -1,    61,    -1,    10,    -1,     6,
      -1,     7,    -1,    81,    20,    62,    -1,    81,    20,    82,
      -1,    84,    -1,    81,    98,    -1,    66,    -1,    84,    -1,
      83,    -1,    85,    -1,    14,    82,    15,    -1,     9,    82,
      -1,     6,    14,    86,    15,    -1,     7,    14,    86,    15,
      -1,    62,    14,    86,    15,    -1,    84,    14,    86,    15,
      -1,    85,    14,    86,    15,    -1,   101,    -1,    82,    -1,
      12,    -1,   110,    -1,    -1,    86,    11,   101,    -1,    86,
      11,    82,    -1,    86,    11,    12,    -1,    86,    11,   110,
      -1,    86,    11,    -1,    52,    62,    44,    62,    89,    91,
      92,    53,    -1,    52,    16,    88,    17,    44,    62,    89,
      91,    92,    53,    -1,    52,    16,    17,    44,    62,    89,
      91,    92,    53,    -1,    52,    62,    89,    91,    92,    53,
      -1,    90,    -1,    14,    90,    15,    -1,    14,    15,    -1,
      -1,    90,    11,    62,    -1,    62,    -1,   137,    -1,    13,
      -1,    13,     4,    -1,    11,    -1,    11,     4,    -1,    76,
      -1,    -1,    82,    -1,   101,    -1,   101,   135,    96,    -1,
      82,   135,    96,    -1,   101,    95,    96,    -1,    82,    95,
      96,    -1,    42,    -1,    43,    -1,   101,    -1,    82,    -1,
      12,    -1,   101,    98,    -1,    82,    98,    -1,    23,   101,
      -1,    23,    82,    -1,    22,   101,    -1,    22,    82,    -1,
     101,    34,   101,    -1,   101,    34,    82,    -1,    82,    34,
     101,    -1,    82,    34,    82,    -1,   101,    35,   101,    -1,
     101,    35,    82,    -1,    82,    35,   101,    -1,    82,    35,
      82,    -1,   101,     8,    -1,   101,    21,    -1,    82,     8,
      -1,    82,    21,    -1,    22,   101,    -1,    22,    82,    -1,
      23,   101,    -1,    23,    82,    -1,    24,   101,    -1,    24,
      82,    -1,    25,   101,    -1,    25,    82,    -1,    26,   101,
      -1,    26,    82,    -1,    27,   101,    -1,    27,    82,    -1,
      28,   101,    -1,    28,    82,    -1,    30,   101,    -1,    30,
      82,    -1,    29,   101,    -1,    29,    82,    -1,    31,   101,
      -1,    31,    82,    -1,    32,   101,    -1,    32,    82,    -1,
      33,   101,    -1,    33,    82,    -1,    12,   101,    -1,    12,
      82,    -1,    99,    12,   101,    -1,    99,    12,    82,    -1,
      99,    -1,     9,   101,    -1,   101,    20,    62,    -1,   101,
      20,    82,    -1,    82,    20,   101,    -1,    82,    20,    82,
      -1,   101,   100,    -1,    82,   100,    -1,   104,    -1,   103,
      -1,    97,    -1,    62,    -1,    63,    -1,    65,    -1,    64,
      -1,    61,    -1,    10,    -1,     6,    -1,     7,    -1,    14,
     101,    15,    -1,    14,   102,    15,    -1,    94,    -1,   102,
      11,   101,    -1,   102,    11,    82,    -1,   101,    11,   101,
      -1,    82,    11,    82,    -1,    82,    11,   101,    -1,   101,
      11,    82,    -1,    18,   105,    19,    -1,    18,   137,   105,
      19,    -1,    18,   105,   108,    19,    -1,    18,   137,   105,
     108,    19,    -1,    18,   108,    19,    -1,    18,   137,   108,
      19,    -1,    18,    19,    -1,    16,   105,    17,    -1,    16,
     137,   105,    17,    -1,    16,   105,   108,    17,    -1,    16,
     137,   105,   108,    17,    -1,    16,   108,    17,    -1,    16,
     137,   108,    17,    -1,    16,    17,    -1,   105,   107,    -1,
     107,    -1,    13,    -1,     4,    -1,    13,     4,    -1,   108,
     106,    -1,   108,    67,     4,    -1,   108,    13,    67,     4,
      -1,   108,   109,   106,    -1,   108,   109,    67,     4,    -1,
     108,   109,    13,    67,     4,    -1,   108,   109,   101,    -1,
     108,   109,    82,    -1,   108,   101,    -1,   108,    82,    -1,
     101,    -1,    82,    -1,   109,    11,    -1,    11,    -1,   111,
      44,   101,    -1,   111,    44,    82,    -1,    82,    44,   101,
      -1,    82,    44,    82,    -1,   111,    44,    12,    -1,    82,
      44,    12,    -1,   111,    44,   134,    -1,    82,    44,   134,
      -1,   101,    20,    62,    -1,   101,    20,    82,    -1,    82,
      20,   101,    -1,    82,    20,    82,    -1,    62,    -1,   112,
      -1,    16,   108,    17,    -1,    45,    93,   117,   114,    49,
      -1,    45,    93,   117,   114,   118,   115,    49,    -1,    45,
      93,   117,   114,   119,    49,    -1,    76,    -1,    -1,    76,
      -1,    -1,    13,    -1,    13,     4,    -1,    11,    -1,    11,
       4,    -1,     4,    -1,    46,    -1,   116,    46,    -1,   116,
      46,     4,    -1,    46,   116,    -1,   116,    -1,    -1,    47,
      -1,    47,    11,    -1,    47,    13,    -1,    47,     4,    -1,
      47,    11,     4,    -1,    47,    13,     4,    -1,    48,    93,
     117,   114,    -1,    48,    93,   117,   114,   118,   115,    -1,
      48,    93,   117,   114,   119,    -1,    50,   121,   122,   123,
      49,    -1,    50,   121,   122,   123,   118,   115,    49,    -1,
      50,   121,    67,   122,   123,    49,    -1,    50,   121,    67,
     122,   123,   118,   115,    49,    -1,   101,    -1,    82,    -1,
       4,    -1,    11,     4,    -1,    13,     4,    -1,    51,   101,
     125,   124,    -1,   136,    51,   101,   125,   124,    -1,    51,
     101,    67,     4,   124,    -1,   136,    51,   101,    67,     4,
     124,    -1,   123,    51,   101,   125,   124,    -1,   123,    51,
     101,    67,     4,   124,    -1,    76,    -1,    -1,    46,    -1,
      11,    -1,    13,    -1,     4,    -1,    46,     4,    -1,    11,
       4,    -1,    13,     4,    -1,    46,    11,    -1,    46,    11,
       4,    -1,    54,    62,    44,   127,   128,   129,    49,    -1,
      54,    14,    62,    44,   127,    15,   128,   129,    49,    -1,
      82,    -1,   101,    -1,     4,    -1,    13,    -1,    13,     4,
      -1,    11,    -1,    11,     4,    -1,    56,    -1,    56,     4,
      -1,    -1,    76,    -1,    -1,    55,    93,   132,   131,    49,
      -1,    76,    -1,    -1,    11,    -1,    13,    -1,    56,    -1,
      56,    11,    -1,    56,    13,    -1,    46,    -1,    46,    11,
      -1,    46,    13,    -1,   137,    -1,    11,     4,    -1,    13,
       4,    -1,    56,     4,    -1,    56,    11,     4,    -1,    56,
      13,     4,    -1,    46,     4,    -1,    46,    11,     4,    -1,
      46,    13,     4,    -1,    58,     4,    76,    59,     4,    76,
      49,    -1,    58,    67,     4,    76,    59,     4,    76,    49,
      -1,    58,     4,    76,    59,    67,     4,    76,    49,    -1,
      58,    67,     4,    76,    59,    67,     4,    76,    49,    -1,
      60,    -1,    60,   101,    -1,    36,    -1,    37,    -1,    38,
      -1,    39,    -1,    40,    -1,    41,    -1,    67,     4,    -1,
     136,    67,     4,    -1,     4,    -1,    67,     4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   292,   292,   293,   301,   304,   309,   315,   321,   335,
     340,   346,   353,   366,   367,   368,   369,   370,   371,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   405,   409,   424,   425,   430,   435,   440,
     441,   442,   443,   444,   449,   450,   451,   452,   460,   461,
     462,   463,   464,   472,   473,   481,   489,   490,   498,   502,
     506,   510,   514,   517,   521,   525,   529,   533,   543,   552,
     559,   566,   580,   588,   589,   590,   598,   602,   613,   614,
     615,   616,   617,   625,   626,   640,   641,   649,   650,   651,
     652,   656,   657,   664,   665,   666,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,   700,   701,   703,   704,   706,   707,
     708,   709,   710,   711,   713,   714,   715,   716,   717,   718,
     719,   720,   722,   723,   724,   725,   726,   727,   735,   736,
     744,   745,   746,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,   774,   782,   788,   794,   800,   806,   812,
     824,   825,   826,   830,   834,   839,   844,   853,   854,   855,
     859,   863,   868,   873,   881,   885,   896,   897,   898,   906,
     907,   908,   909,   910,   911,   919,   923,   927,   931,   935,
     939,   950,   951,   959,   960,   961,   962,   964,   965,   967,
     968,   977,   978,   979,   980,   981,   982,   989,   998,   999,
    1000,  1008,  1009,  1023,  1024,  1038,  1039,  1040,  1041,  1042,
    1050,  1051,  1052,  1053,  1054,  1055,  1063,  1064,  1065,  1066,
    1067,  1068,  1076,  1081,  1087,  1103,  1104,  1105,  1106,  1114,
    1115,  1123,  1124,  1125,  1134,  1135,  1136,  1137,  1138,  1139,
    1143,  1144,  1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,
    1160,  1168,  1169,  1177,  1178,  1187,  1188,  1189,  1190,  1191,
    1192,  1193,  1194,  1198,  1199,  1213,  1221,  1222,  1236,  1237,
    1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,
    1248,  1249,  1250,  1251,  1252,  1260,  1261,  1265,  1269,  1281,
    1282,  1290,  1291,  1292,  1293,  1294,  1295,  1302,  1303,  1311,
    1312
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"line break\"",
  "\"end of line\"", "\"spaces\"", "\"%t or %T\"", "\"%f or %F\"", "\"'\"",
  "\"~ or @\"", "\"$\"", "\",\"", "\":\"", "\";\"", "\"(\"", "\")\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\".\"", "\".'\"", "\"+\"", "\"-\"",
  "\"*\"", "\".*\"", "\".*.\"", "\"/\"", "\"./\"", "\"/.\"", "\"./.\"",
  "\"\\\\\"", "\".\\\\\"", "\".\\\\.\"", "\"** or ^\"", "\".^\"", "\"==\"",
  "\"<> or ~=\"", "\"<\"", "\"<=\"", "\">\"", "\">=\"", "\"&\"", "\"|\"",
  "\"=\"", "\"if\"", "\"then\"", "\"else\"", "\"elseif\"", "\"end\"",
  "\"select\"", "\"case\"", "\"function\"", "\"endfunction\"", "\"for\"",
  "\"while\"", "\"do\"", "\"break\"", "\"try\"", "\"catch\"", "\"return\"",
  "\"string\"", "\"identifier\"", "\"integer\"", "\"float\"", "\"number\"",
  "\"path\"", "\"line comment\"", "\"block comment\"", "TOPLEVEL",
  "HIGHLEVEL", "UPLEVEL", "LISTABLE", "FUNCTIONCALL", "$accept", "program",
  "expressions", "recursiveExpression", "expressionLineBreak",
  "expression", "implicitFunctionCall", "implicitCallable", "functionCall",
  "specificFunctionCall", "simpleFunctionCall", "recursiveFunctionCall",
  "functionArgs", "functionDeclaration", "functionDeclarationReturns",
  "functionDeclarationArguments", "idList", "functionDeclarationBreak",
  "functionBody", "condition", "comparison", "logicalComparators",
  "comparable", "operation", "rightOperand", "listableBegin",
  "listableEnd", "variable", "variableFields", "cell", "matrix",
  "matrixOrCellLines", "matrixOrCellLineBreak", "matrixOrCellLine",
  "matrixOrCellColumns", "matrixOrCellColumnsBreak", "variableDeclaration",
  "assignable", "multipleResults", "ifControl", "thenBody", "elseBody",
  "ifConditionBreak", "then", "else", "elseIfControl", "selectControl",
  "selectable", "selectConditionBreak", "casesControl", "caseBody",
  "caseControlBreak", "forControl", "forIterator", "forConditionBreak",
  "forBody", "whileControl", "whileBody", "whileConditionBreak",
  "tryControl", "returnControl", "comparators", "comments", "lineEnd", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    75,    76,    76,    76,    76,    76,    77,
      77,    77,    77,    78,    78,    78,    78,    78,    78,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    80,    80,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    82,
      82,    82,    82,    83,    83,    84,    85,    85,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    87,    87,
      87,    87,    88,    89,    89,    89,    90,    90,    91,    91,
      91,    91,    91,    92,    92,    93,    93,    94,    94,    94,
      94,    95,    95,    96,    96,    96,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    99,    99,
     100,   100,   100,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   102,   102,   102,   102,   102,   102,
     103,   103,   103,   103,   103,   103,   103,   104,   104,   104,
     104,   104,   104,   104,   105,   105,   106,   106,   106,   107,
     107,   107,   107,   107,   107,   108,   108,   108,   108,   108,
     108,   109,   109,   110,   110,   110,   110,   110,   110,   110,
     110,   111,   111,   111,   111,   111,   111,   112,   113,   113,
     113,   114,   114,   115,   115,   116,   116,   116,   116,   116,
     117,   117,   117,   117,   117,   117,   118,   118,   118,   118,
     118,   118,   119,   119,   119,   120,   120,   120,   120,   121,
     121,   122,   122,   122,   123,   123,   123,   123,   123,   123,
     124,   124,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   126,   126,   127,   127,   128,   128,   128,   128,   128,
     128,   128,   128,   129,   129,   130,   131,   131,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   133,   133,   133,   133,   134,
     134,   135,   135,   135,   135,   135,   135,   136,   136,   137,
     137
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     3,     1,     2,     3,
       4,     3,     2,     1,     1,     1,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     2,     1,     1,     1,
       1,     3,     2,     4,     4,     4,     4,     4,     1,     1,
       1,     1,     0,     3,     3,     3,     3,     2,     8,    10,
       9,     6,     1,     3,     2,     0,     3,     1,     1,     1,
       2,     1,     2,     1,     0,     1,     1,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     1,     2,     3,     3,     3,     3,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     5,     3,     4,     2,     3,     4,     4,
       5,     3,     4,     2,     2,     1,     1,     1,     2,     2,
       3,     4,     3,     4,     5,     3,     3,     2,     2,     1,
       1,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     3,     5,     7,
       6,     1,     0,     1,     0,     1,     2,     1,     2,     1,
       1,     2,     3,     2,     1,     0,     1,     2,     2,     2,
       3,     3,     4,     6,     5,     5,     7,     6,     8,     1,
       1,     1,     2,     2,     4,     5,     5,     6,     5,     6,
       1,     0,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     7,     9,     1,     1,     1,     1,     2,     1,     2,
       1,     2,     0,     1,     0,     5,     1,     0,     1,     1,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     2,
       3,     3,     2,     3,     3,     7,     8,     8,     9,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     3,     1,
       2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,    32,     0,   159,   160,     0,   158,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   309,
     157,   153,   154,   156,   155,    31,     0,     2,     0,     7,
      28,    20,    49,    48,    50,    19,   163,   152,    27,   151,
     150,    21,     0,   216,    22,    23,    24,    25,    26,    30,
       3,    62,    62,     0,   153,    52,   143,     0,     0,     0,
     319,   183,     0,   200,   199,     0,   185,     0,     0,   176,
       0,     0,     0,   101,   100,    99,    98,    85,   235,    86,
     250,   249,     0,     0,    75,     0,     0,     0,     0,     0,
       0,   310,    41,    42,    40,    62,    39,    35,    36,    38,
      37,    47,    34,    45,     1,     5,    15,    14,    13,     8,
      12,    33,   112,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   312,   313,   314,   315,   316,    91,    92,     0,     0,
      97,   142,   149,     0,    62,    62,   110,     0,   111,     0,
       0,     0,    96,   148,     0,     0,    60,   153,    59,     0,
      58,    61,     0,     0,     0,     0,     0,    51,     0,   161,
       0,   162,   320,   177,   184,     0,   187,   202,   186,   181,
       0,   198,   197,   189,     0,     0,     0,   170,     0,   174,
       0,     0,   229,   227,   225,   230,   234,     0,   251,     0,
       0,     0,     0,     0,    77,     0,    72,     0,     0,     0,
       0,     0,   288,   289,   293,   290,     0,   296,     0,     0,
       0,     0,    46,     6,     9,    11,    18,    17,    16,   139,
     138,   147,   146,   115,   114,   117,   116,   119,   118,   121,
     120,   123,   122,   125,   124,   127,   126,   131,   130,   129,
     128,   133,   132,   135,   134,   137,   136,   105,   104,   109,
     108,   208,   206,   205,   210,    95,    94,    90,    93,     0,
      88,     0,     0,     0,     0,     0,     0,   144,   145,   103,
     102,   107,   106,    89,    87,   207,   204,   203,   209,    67,
      53,    54,   181,   147,   146,   144,   145,   167,   168,   169,
     166,   165,   164,   179,   188,     0,   190,   201,   186,     0,
     196,   195,   192,   178,     0,   182,   172,   171,     0,   175,
     228,   226,   233,   231,   221,     0,   252,   253,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,    81,
      79,     0,    78,     0,   273,   274,   282,   297,   298,   302,
     294,   295,   299,   291,   292,   286,     0,     0,     0,    55,
      43,    44,    10,   141,   140,    56,    57,     0,    52,     0,
      65,    64,    63,    66,   191,     0,   193,   180,   173,   232,
     236,     0,   218,     0,     0,     0,     0,   317,   245,     0,
       0,     0,     0,    75,     0,    76,    73,     0,    82,    80,
      83,     0,     0,   275,   278,   276,   280,     0,   303,   304,
     300,   301,   285,     0,     0,     0,   194,   239,   237,   238,
     235,   223,     0,   220,   247,     0,   265,   263,   264,   262,
       0,     0,     0,     0,     0,   318,     0,    75,     0,    71,
     282,   279,   277,   281,   283,     0,     0,     0,     0,     0,
     240,   241,     0,   219,     0,   267,   268,   266,   269,     0,
     260,   254,     0,     0,   246,     0,     0,     0,     0,     0,
       0,   271,   305,     0,     0,     0,   242,   248,   270,   256,
       0,   258,     0,   255,     0,     0,    68,     0,   307,   306,
       0,     0,   244,   259,   257,    70,     0,   272,   308,   243,
      69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,   460,    28,   110,    29,    30,   102,    31,    32,
      33,    34,   159,    35,   205,   209,   206,   341,   401,    78,
      36,   151,   267,    37,   152,   141,   153,    38,    59,    39,
      40,    65,   183,    66,    67,   184,    41,    42,    43,    44,
     325,   422,   196,   197,   383,   384,    45,    82,   202,   331,
     461,   431,    46,   346,   407,   445,    47,   356,   216,    48,
      49,   154,   332,   342
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -382
static const yytype_int16 yypact[] =
{
     676,  -382,  1333,    61,    79,  2231,  -382,  2231,    36,  1881,
    2231,  2231,  2231,  2231,    23,    27,  2231,  -382,    15,  2231,
    -382,   738,  -382,  -382,  -382,  -382,   108,  -382,   239,    19,
     416,  2236,  -382,    97,   139,  -382,  -382,  -382,  2361,  -382,
    -382,  -382,   123,  -382,  -382,  -382,  -382,  -382,  -382,  -382,
    -382,  1979,  1979,    36,   163,  -382,  -382,  2289,  2325,   158,
    -382,  -382,   218,  2397,  2433,  2039,  -382,   214,  2231,  -382,
    2057,  1532,  2231,   921,  1337,   921,  1337,  2397,   222,  2433,
    2397,  2433,    25,   114,   130,   211,   230,   144,  1333,   276,
    2397,  2433,  -382,  -382,  -382,  1979,  -382,   163,  -382,  -382,
    -382,  -382,   939,  -382,  -382,    65,  -382,  -382,  -382,   256,
     333,   939,  -382,  2231,  2231,  -382,  2231,  2231,  2231,  2231,
    2231,  2231,  2231,  2231,  2231,  2231,  2231,  2231,  2231,  2231,
    -382,  -382,  -382,  -382,  -382,  -382,  -382,  -382,  1943,  2075,
    -382,   283,  -382,  2075,  1979,  1979,  -382,   182,  -382,  2231,
    2231,  2075,  -382,  -382,  2075,  1961,  -382,   135,  2236,   193,
    2361,  -382,   223,  1553,  2231,   196,  2231,  -382,  2231,  -382,
    2231,  -382,  -382,  -382,  -382,  1615,  -382,  -382,    17,   264,
     307,  2397,  2433,  -382,  1861,  2135,  1635,  -382,  1697,  -382,
    2153,  1717,  -382,   313,   323,   368,   311,   306,  -382,   336,
     338,   408,     5,   318,  -382,   342,   354,   113,   316,    91,
     340,  2231,   378,   384,   420,   423,  1114,  -382,   330,  1333,
     248,   312,  -382,   256,   333,   333,  -382,  -382,  -382,  2469,
    2503,   348,   349,   921,  1337,   921,  1337,   315,   383,   315,
     383,   315,   383,   315,   383,   315,   383,   315,   383,   315,
     383,   315,   383,   315,   383,   315,   383,   221,   262,   221,
     262,  -382,  2397,  2433,  -382,  -382,  2397,  -382,  2433,  2231,
    -382,   294,   337,    61,    79,   324,   324,   151,   351,   221,
     262,   221,   262,  -382,  -382,  -382,  2397,  2433,  -382,  2171,
    -382,  -382,  -382,  -382,  -382,   163,  -382,  2397,  2433,  2397,
    2433,  2397,  2433,  -382,  -382,   402,  -382,  -382,    29,   405,
    2397,  2433,  -382,  -382,  1779,  -382,  -382,  -382,  1799,  -382,
    -382,  -382,  -382,   406,  -382,   165,  -382,  -382,     5,  2231,
     407,   205,   101,   352,   371,   358,  -382,   365,   415,   412,
     426,  1179,  -382,  2231,  2397,  2433,   159,  -382,  -382,  -382,
     428,   435,  -382,   439,   440,  -382,   396,    66,   387,  -382,
     163,  -382,   333,  2469,  2503,  -382,  -382,   163,  -382,   434,
    -382,  2236,  2361,  -382,  -382,   446,  -382,  -382,  -382,  -382,
     424,  2231,  -382,  1244,   404,   296,  1395,  -382,  -382,  2231,
    1244,  2231,   447,   415,   393,  -382,  -382,    91,  -382,  -382,
    -382,   409,   443,  -382,   459,   460,   461,  1268,  -382,  -382,
    -382,  -382,  -382,  1333,   462,    69,  -382,  -382,   464,   465,
     222,  -382,   422,  -382,  -382,  1244,  -382,   468,   469,   174,
     470,   605,  1443,   427,  1491,  -382,    91,   415,  1179,  -382,
     159,  -382,  -382,  -382,  -382,   437,   438,  1333,  1333,   479,
    -382,  -382,   306,  -382,   441,  -382,  -382,  -382,   484,   605,
    -382,  -382,   485,   605,  -382,   488,   605,  1179,    91,   442,
    1268,  -382,  -382,   444,   445,  1333,   145,  -382,  -382,  -382,
     605,  -382,   605,  -382,   448,  1179,  -382,   449,  -382,  -382,
     450,  1244,  -382,  -382,  -382,  -382,   451,  -382,  -382,  -382,
    -382
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -382,  -382,     0,  -382,   -85,   472,  -382,   466,    -4,  -382,
      14,  -382,   -15,  -382,  -382,  -310,   290,  -381,  -284,    -6,
    -382,   616,    55,  -382,   530,  -382,   762,   389,  -382,  -382,
    -382,    22,   335,   -48,     9,  -382,   -38,  -382,  -382,  -382,
      68,  -365,   326,   102,  -320,    47,  -382,  -382,   325,   197,
    -216,  -100,  -382,   181,    89,    60,  -382,  -382,  -382,  -382,
    -104,   848,  -382,    18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -288
static const yytype_int16 yytable[] =
{
      27,    55,    50,    57,    63,    63,    73,    75,    77,    80,
      87,   390,    77,   161,   161,    90,   438,   174,    71,    88,
     224,   304,   174,   106,   225,   433,    68,    72,   397,   198,
     107,    70,   108,   304,   264,   103,   199,   162,   200,    83,
      60,    85,     3,     4,   103,     5,     6,   158,   158,    63,
       7,   288,    53,    61,     9,   467,   329,   161,    10,    11,
     454,    63,   163,   181,    63,   425,    63,   181,    63,   106,
     413,    68,   330,   448,   175,    51,   107,   186,   108,   188,
     220,   191,    89,   436,   305,    84,   109,   485,   218,    86,
     185,   158,   201,    52,   190,    60,   375,    20,    54,    22,
      23,    24,   339,    62,   340,   217,   161,   161,   104,   229,
     231,   144,   233,   235,   237,   239,   241,   243,   245,   247,
     249,   251,   253,   255,   257,   259,   499,   468,   336,   271,
     272,   203,   223,   414,   262,   266,   449,   174,   362,   266,
     158,   158,   174,   278,   207,   279,   281,   266,    60,    95,
     266,   286,   391,   145,   469,   212,   491,   213,    62,   181,
     293,   296,   297,   403,   299,    95,   301,   155,   392,   170,
     404,   181,   405,   171,   208,   204,   204,    95,   457,  -215,
     310,    63,   181,   484,   181,   458,    63,   181,   273,   274,
     214,   275,   380,   381,   314,  -211,   276,   324,   270,   318,
     215,   496,   273,   274,   289,   275,   283,   344,   290,   284,
     276,    62,   380,   381,   382,   406,   355,   361,   176,   358,
       3,     4,   172,     5,     6,   177,   192,   178,     7,   112,
      53,   179,     9,   193,   289,   194,    10,    11,   291,    -4,
       1,   164,   115,   479,   277,     3,     4,   481,     5,     6,
     483,   373,   380,     7,   388,     8,   389,     9,   295,   289,
     106,    10,    11,   359,   493,   363,   494,   107,   195,   108,
     146,   368,   369,   210,   211,    20,    54,    22,    23,    24,
     219,   180,   165,   148,    12,   371,    -4,    -4,    -4,    13,
      -4,    14,    -4,    15,    16,   269,    17,    18,    -4,    19,
      20,    21,    22,    23,    24,   289,    25,     1,  -217,   365,
     181,   306,     3,     4,   181,     5,     6,   320,   273,   274,
       7,   275,     8,   112,     9,    90,   276,   321,    10,    11,
     273,   274,   463,   275,   466,   164,   115,   226,   276,   344,
     326,   400,   327,   380,   227,   424,   228,   389,   289,   128,
     129,    12,   366,  -222,  -222,  -222,    13,   323,    14,   334,
      15,    16,   333,    17,    18,   335,    19,    20,    21,    22,
      23,    24,   192,    25,   360,   420,   335,    77,   338,   193,
     396,   194,   347,   421,   343,    90,   367,    90,   348,   357,
     421,   146,  -214,  -213,    56,  -212,    58,    64,    64,    74,
      76,    79,    81,   165,   148,    79,   374,   444,    91,   376,
     379,   387,   198,   446,   393,   394,   398,   149,   150,   199,
     395,   200,    92,    93,   349,   421,    94,   352,   417,   207,
     399,   350,   408,   351,   353,   418,   354,   419,   400,   409,
     160,   160,    64,   410,   411,   412,   415,   473,   474,   167,
     416,   435,   324,   423,    64,   437,   182,    64,   440,    64,
     182,    64,   439,   441,   442,   443,   447,   400,   450,   451,
     444,   453,   455,   456,   459,   490,   464,    96,    97,    98,
      99,   100,   101,   475,   160,   400,   471,   472,   478,   480,
     477,   421,   482,   488,   489,   486,   111,   337,   497,   498,
     105,   495,   230,   232,   500,   234,   236,   238,   240,   242,
     244,   246,   248,   250,   252,   254,   256,   258,   260,   312,
     476,   322,   452,   492,   402,   385,   328,   263,   268,   470,
     487,     0,   268,   160,   160,     0,     0,     0,   280,   282,
     268,     0,     0,   268,   287,     0,     0,     0,     0,     0,
       0,     0,   182,   294,     0,   298,     0,   300,     0,   302,
       0,   140,     0,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,   311,    64,   182,     0,   182,     0,    64,
     182,     0,     0,     0,     0,   140,     0,   140,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
     345,     0,     0,   140,     0,   140,     1,   140,     0,     0,
     140,     3,     4,     0,     5,     6,     0,     0,     0,     7,
     140,     8,     0,     9,     0,     0,     0,    10,    11,     0,
       0,     0,   222,     0,     0,     0,     0,     0,     0,     0,
       0,   222,     0,     0,     0,     0,     0,   139,     0,     0,
      12,     0,  -261,     0,  -261,    13,  -261,    14,   364,    15,
      16,     0,    17,    18,     0,    19,    20,    21,    22,    23,
      24,   139,    25,   139,     0,     0,     0,     1,   372,   139,
       2,     0,     3,     4,     0,     5,     6,     0,   140,   139,
       7,   139,     8,   139,     9,     0,   139,     0,    10,    11,
       0,     0,     0,   182,     0,     0,   139,   182,     0,     0,
       0,   140,     0,     0,     0,     0,     0,     0,   386,     0,
       0,    12,     0,     0,     0,     0,    13,     0,    14,     0,
      15,    16,   345,    17,    18,     0,    19,    20,    21,    22,
      23,    24,     0,    25,    92,    93,     0,     0,    94,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,   140,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
      79,   140,     0,   140,   139,   140,     0,   140,   432,   140,
     434,   140,  -215,   140,     0,   140,     0,   140,     0,   140,
       0,     0,   140,   142,     0,     0,   140,   139,     0,    96,
      97,    98,    99,   100,   101,     0,     0,     0,     0,   140,
       0,   140,     0,     0,     0,     0,   140,   142,     0,   142,
       0,     0,     0,   140,     0,   142,     0,   140,     0,   140,
       0,   140,     0,     0,     0,   142,     0,   142,     0,   142,
     140,     0,   142,     0,     0,   139,     0,   139,     0,   139,
       0,   139,   142,   139,     0,   139,     0,   139,     0,   139,
       0,   139,     0,   139,     0,   139,     0,   139,     0,   139,
       0,   139,     0,   139,   140,   139,     0,     0,   139,   143,
       0,     0,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,     0,   139,     0,   139,     0,     0,
       0,   140,   139,   143,     0,   143,     0,     0,     0,   139,
       0,   143,     0,   139,     0,   139,     0,   139,     0,     0,
     142,   143,     0,   143,     0,   143,   139,     0,   143,   112,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,   164,   115,   142,     0,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,     0,     0,   221,
     139,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,   139,     0,     0,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,   143,   142,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,     0,   142,     0,     0,     0,   142,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   142,     0,   142,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,     0,   142,     0,     0,     0,   142,
       0,   142,     0,   142,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,     0,     0,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,   142,   143,     0,     0,
     143,     0,     0,     0,   143,     1,     0,     0,     0,     0,
       3,     4,     0,     5,     6,   142,     0,   143,     7,   143,
       8,     0,     9,   142,   143,     0,    10,    11,     0,     0,
       0,   143,     0,     0,     0,   143,     0,   143,     0,   143,
       0,     0,     0,     0,     0,     0,     0,     0,   143,    12,
       0,     0,     0,  -287,    13,     0,    14,     0,    15,    16,
       0,    17,    18,     0,    19,    20,    21,    22,    23,    24,
       1,    25,     0,     0,     0,     3,     4,     0,     5,     6,
       0,     0,   143,     7,     0,     8,     0,     9,     0,     0,
       0,    10,    11,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,     0,    12,     0,     0,     0,     0,    13,
       0,    14,   -84,    15,    16,     0,    17,    18,     0,    19,
      20,    21,    22,    23,    24,     1,    25,     0,     0,     0,
       3,     4,     0,     5,     6,     0,     0,     0,     7,     0,
       8,     0,     9,     0,     0,     0,    10,    11,     0,     1,
       0,     0,     0,     0,     3,     4,     0,     5,     6,     0,
       0,     0,     7,     0,     8,     0,     9,     0,     0,    12,
      10,    11,     0,  -224,    13,     0,    14,     0,    15,    16,
       0,    17,    18,     0,    19,    20,    21,    22,    23,    24,
       0,    25,     0,    12,     0,     0,     0,  -284,    13,     0,
      14,     0,    15,    16,     0,    17,    18,     0,    19,    20,
      21,    22,    23,    24,     1,    25,     0,     0,     0,     3,
       4,     0,     5,     6,     0,   146,     0,     7,     0,     8,
       0,     9,     0,     0,     0,    10,    11,   165,   148,     0,
       0,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   149,   150,     0,     0,     0,     0,     0,    12,     0,
       0,     0,     0,    13,     0,    14,     0,    15,    16,     0,
      17,    18,     0,    19,    20,    21,    22,    23,    24,   426,
      25,     0,     0,   146,     0,     0,   427,   113,   428,     0,
       0,     0,     0,     0,     0,   165,   148,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   149,
     150,   130,   131,   132,   133,   134,   135,   136,   137,     0,
       0,   429,     0,     0,     0,     0,     0,   426,     0,     0,
       0,   146,     0,     0,   427,   113,   428,     0,     0,     0,
       0,     0,   430,   165,   148,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   149,   150,   130,
     131,   132,   133,   134,   135,   136,   137,     0,     0,   429,
       0,     0,     0,     0,     0,   426,     0,     0,     0,   146,
       0,     0,   427,   113,   428,     0,     0,     0,     0,     0,
     462,   165,   148,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   149,   150,   130,   131,   132,
     133,   134,   135,   136,   137,     0,   176,   429,     3,     4,
       0,     5,     6,   177,     0,   178,     7,     0,    53,     0,
       9,   189,     0,     0,    10,    11,     0,   176,   465,     3,
       4,     0,     5,     6,   177,     0,   178,     7,     0,    53,
     292,     9,     0,     0,     0,    10,    11,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,    54,    22,    23,    24,     0,   180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    54,    22,    23,    24,   176,
     180,     3,     4,     0,     5,     6,   177,     0,   178,     7,
       0,    53,   303,     9,     0,     0,     0,    10,    11,   176,
       0,     3,     4,     0,     5,     6,   177,     0,   178,     7,
       0,    53,   315,     9,     0,     0,     0,    10,    11,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,    54,    22,    23,
      24,     0,   180,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,    54,    22,    23,
      24,   176,   180,     3,     4,     0,     5,     6,   177,     0,
     178,     7,     0,    53,     0,     9,   316,     0,     0,    10,
      11,   176,     0,     3,     4,     0,     5,     6,   177,     0,
     178,     7,     0,    53,     0,     9,   319,     0,     0,    10,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    54,
      22,    23,    24,     0,   180,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    54,
      22,    23,    24,   176,   180,     3,     4,     0,     5,     6,
     177,     0,   178,     7,     0,    53,   377,     9,     0,     0,
       0,    10,    11,   176,     0,     3,     4,     0,     5,     6,
     177,     0,   178,     7,     0,    53,     0,     9,   378,     0,
       0,    10,    11,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,    54,    22,    23,    24,     0,   180,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,    54,    22,    23,    24,   176,   180,     3,     4,     0,
       5,     6,   307,     0,   308,     7,     0,    53,     0,     9,
       0,     0,     0,    10,    11,    60,     0,     3,     4,     0,
       5,     6,     0,     0,     0,     7,     0,    53,     0,     9,
      69,     0,     0,    10,    11,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    54,    22,    23,    24,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    54,    22,    23,    24,     0,    62,     3,
       4,     0,     5,     6,     0,   261,     0,     7,     0,    53,
       0,     9,     0,     0,     0,    10,    11,     3,     4,     0,
       5,     6,     0,   285,     0,     7,     0,    53,     0,     9,
       0,     0,     0,    10,    11,     3,     4,     0,     5,     6,
       0,   156,     0,     7,     0,     8,     0,     9,     0,     0,
       0,    10,    11,    19,    20,    54,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    19,    20,    54,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,   157,    22,    23,    24,     3,     4,     0,     5,     6,
       0,     0,     0,     7,     0,    53,   173,     9,     0,     0,
       0,    10,    11,     3,     4,     0,     5,     6,     0,     0,
       0,     7,     0,    53,     0,     9,   187,     0,     0,    10,
      11,     3,     4,     0,     5,     6,     0,   265,     0,     7,
       0,    53,     0,     9,     0,     0,     0,    10,    11,     0,
      20,    54,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    54,
      22,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,    54,    22,    23,
      24,     3,     4,     0,     5,     6,     0,     0,     0,     7,
       0,    53,   313,     9,     0,     0,     0,    10,    11,     3,
       4,     0,     5,     6,     0,     0,     0,     7,     0,    53,
       0,     9,   317,     0,     0,    10,    11,     3,     4,     0,
       5,     6,     0,   370,     0,     7,     0,     8,     0,     9,
       0,     0,     0,    10,    11,     0,    20,    54,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    54,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,   157,    22,    23,    24,     3,     4,     0,
       5,     6,     0,     0,   112,     7,     0,    53,   113,     9,
       0,     0,     0,    10,    11,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    54,    22,    23,    24,   112,     0,     0,
     166,   113,     0,     0,   167,     0,     0,     0,     0,   164,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   146,     0,     0,   168,   113,     0,     0,
     169,     0,     0,     0,     0,   165,   148,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   149,
     150,   130,   131,   132,   133,   134,   135,   136,   137,   146,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,   147,   148,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   149,   150,   130,   131,   132,
     133,   134,   135,   136,   137,   112,     0,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,   164,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   146,     0,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,   165,   148,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   149,   150,   130,
     131,   132,   133,   134,   135,   136,   137,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   165,   148,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   149,   150,   130,
     131,   132,   133,   134,   135
};

static const yytype_int16 yycheck[] =
{
       0,     5,     2,     7,     8,     9,    10,    11,    12,    13,
      16,   331,    16,    51,    52,    19,   397,    65,     9,     4,
     105,     4,    70,     4,   109,   390,     8,     9,   338,     4,
      11,     9,    13,     4,   138,    21,    11,    52,    13,    16,
       4,    14,     6,     7,    30,     9,    10,    51,    52,    53,
      14,   155,    16,    17,    18,   436,    51,    95,    22,    23,
     425,    65,    53,    67,    68,   385,    70,    71,    72,     4,
       4,    53,    67,     4,    65,    14,    11,    68,    13,    70,
      95,    72,    67,   393,    67,    62,    67,   468,    88,    62,
      68,    95,    67,    14,    72,     4,    67,    61,    62,    63,
      64,    65,    11,    67,    13,    87,   144,   145,     0,   113,
     114,    14,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   491,   437,    15,   144,
     145,    17,    67,    67,   138,   139,    67,   185,   223,   143,
     144,   145,   190,   147,    14,   149,   150,   151,     4,    14,
     154,   155,    51,    14,   438,    11,   476,    13,    67,   163,
     164,   165,   166,     4,   168,    14,   170,    44,    67,    11,
      11,   175,    13,    15,    44,    62,    62,    14,     4,    44,
     184,   185,   186,   467,   188,    11,   190,   191,     6,     7,
      46,     9,    47,    48,   185,    44,    14,   197,   143,   190,
      56,   485,     6,     7,    11,     9,   151,   211,    15,   154,
      14,    67,    47,    48,    49,    56,   216,   221,     4,   219,
       6,     7,     4,     9,    10,    11,     4,    13,    14,     8,
      16,    17,    18,    11,    11,    13,    22,    23,    15,     0,
       1,    20,    21,   459,    62,     6,     7,   463,     9,    10,
     466,   289,    47,    14,    49,    16,    51,    18,    62,    11,
       4,    22,    23,    15,   480,   269,   482,    11,    46,    13,
       8,   275,   276,    62,    44,    61,    62,    63,    64,    65,
       4,    67,    20,    21,    45,   289,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    12,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    11,    67,     1,    44,    15,
     314,     4,     6,     7,   318,     9,    10,     4,     6,     7,
      14,     9,    16,     8,    18,   329,    14,     4,    22,    23,
       6,     7,   432,     9,   434,    20,    21,     4,    14,   343,
       4,   341,     4,    47,    11,    49,    13,    51,    11,    34,
      35,    45,    15,    47,    48,    49,    50,    46,    52,    17,
      54,    55,    44,    57,    58,    11,    60,    61,    62,    63,
      64,    65,     4,    67,    62,   381,    11,   381,    62,    11,
      15,    13,     4,   383,    44,   389,    62,   391,     4,    59,
     390,     8,    44,    44,     5,    44,     7,     8,     9,    10,
      11,    12,    13,    20,    21,    16,     4,   407,    19,     4,
       4,     4,     4,   413,    62,    44,     4,    34,    35,    11,
      62,    13,     6,     7,     4,   425,    10,     4,     4,    14,
       4,    11,     4,    13,    11,    11,    13,    13,   438,     4,
      51,    52,    53,     4,     4,    49,    59,   447,   448,    15,
       4,     4,   452,    49,    65,    62,    67,    68,    15,    70,
      71,    72,    53,     4,     4,     4,     4,   467,     4,     4,
     470,    49,     4,     4,     4,   475,    49,    61,    62,    63,
      64,    65,    66,     4,    95,   485,    49,    49,     4,     4,
      49,   491,     4,    49,    49,    53,    30,   207,    49,    49,
      28,    53,   113,   114,    53,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   184,
     452,   195,   420,   476,   343,   328,   201,   138,   139,   440,
     470,    -1,   143,   144,   145,    -1,    -1,    -1,   149,   150,
     151,    -1,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,    31,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,   185,   186,    -1,   188,    -1,   190,
     191,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
     211,    -1,    -1,    73,    -1,    75,     1,    77,    -1,    -1,
      80,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,
      90,    16,    -1,    18,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      45,    -1,    47,    -1,    49,    50,    51,    52,   269,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    55,    67,    57,    -1,    -1,    -1,     1,   289,    63,
       4,    -1,     6,     7,    -1,     9,    10,    -1,   158,    73,
      14,    75,    16,    77,    18,    -1,    80,    -1,    22,    23,
      -1,    -1,    -1,   314,    -1,    -1,    90,   318,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,   329,    -1,
      -1,    45,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      54,    55,   343,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,     6,     7,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,   229,
      -1,   231,    -1,   233,    -1,   235,    -1,   237,    -1,   239,
     381,   241,    -1,   243,   158,   245,    -1,   247,   389,   249,
     391,   251,    44,   253,    -1,   255,    -1,   257,    -1,   259,
      -1,    -1,   262,    31,    -1,    -1,   266,   181,    -1,    61,
      62,    63,    64,    65,    66,    -1,    -1,    -1,    -1,   279,
      -1,   281,    -1,    -1,    -1,    -1,   286,    55,    -1,    57,
      -1,    -1,    -1,   293,    -1,    63,    -1,   297,    -1,   299,
      -1,   301,    -1,    -1,    -1,    73,    -1,    75,    -1,    77,
     310,    -1,    80,    -1,    -1,   229,    -1,   231,    -1,   233,
      -1,   235,    90,   237,    -1,   239,    -1,   241,    -1,   243,
      -1,   245,    -1,   247,    -1,   249,    -1,   251,    -1,   253,
      -1,   255,    -1,   257,   344,   259,    -1,    -1,   262,    31,
      -1,    -1,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   363,    -1,   279,    -1,   281,    -1,    -1,
      -1,   371,   286,    55,    -1,    57,    -1,    -1,    -1,   293,
      -1,    63,    -1,   297,    -1,   299,    -1,   301,    -1,    -1,
     158,    73,    -1,    75,    -1,    77,   310,    -1,    80,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    20,    21,   181,    -1,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    20,
     344,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   363,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,    -1,    -1,
      -1,   229,    -1,   231,    -1,   233,    -1,   235,    -1,   237,
      -1,   239,    -1,   241,    -1,   243,   158,   245,    -1,   247,
      -1,   249,    -1,   251,    -1,   253,    -1,   255,    -1,   257,
      -1,   259,    -1,    -1,   262,    -1,    -1,    -1,   266,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   279,    -1,   281,    -1,    -1,    -1,    -1,   286,    -1,
      -1,    -1,    -1,    -1,    -1,   293,    -1,    -1,    -1,   297,
      -1,   299,    -1,   301,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,   229,    -1,   231,
      -1,   233,    -1,   235,    -1,   237,    -1,   239,    -1,   241,
      -1,   243,    -1,   245,    -1,   247,    -1,   249,    -1,   251,
      -1,   253,    -1,   255,    -1,   257,   344,   259,    -1,    -1,
     262,    -1,    -1,    -1,   266,     1,    -1,    -1,    -1,    -1,
       6,     7,    -1,     9,    10,   363,    -1,   279,    14,   281,
      16,    -1,    18,   371,   286,    -1,    22,    23,    -1,    -1,
      -1,   293,    -1,    -1,    -1,   297,    -1,   299,    -1,   301,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    45,
      -1,    -1,    -1,    49,    50,    -1,    52,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
       1,    67,    -1,    -1,    -1,     6,     7,    -1,     9,    10,
      -1,    -1,   344,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   363,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    50,
      -1,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,     1,    67,    -1,    -1,    -1,
       6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,
      16,    -1,    18,    -1,    -1,    -1,    22,    23,    -1,     1,
      -1,    -1,    -1,    -1,     6,     7,    -1,     9,    10,    -1,
      -1,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,    45,
      22,    23,    -1,    49,    50,    -1,    52,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    -1,    45,    -1,    -1,    -1,    49,    50,    -1,
      52,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,     1,    67,    -1,    -1,    -1,     6,
       7,    -1,     9,    10,    -1,     8,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,    20,    21,    -1,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    50,    -1,    52,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,     4,
      67,    -1,    -1,     8,    -1,    -1,    11,    12,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,     8,    -1,    -1,    11,    12,    13,    -1,    -1,    -1,
      -1,    -1,    67,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,     8,
      -1,    -1,    11,    12,    13,    -1,    -1,    -1,    -1,    -1,
      67,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,     4,    46,     6,     7,
      -1,     9,    10,    11,    -1,    13,    14,    -1,    16,    -1,
      18,    19,    -1,    -1,    22,    23,    -1,     4,    67,     6,
       7,    -1,     9,    10,    11,    -1,    13,    14,    -1,    16,
      17,    18,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,     4,
      67,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,     4,
      -1,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      65,     4,    67,     6,     7,    -1,     9,    10,    11,    -1,
      13,    14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,
      23,     4,    -1,     6,     7,    -1,     9,    10,    11,    -1,
      13,    14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,     4,    67,     6,     7,    -1,     9,    10,
      11,    -1,    13,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    22,    23,     4,    -1,     6,     7,    -1,     9,    10,
      11,    -1,    13,    14,    -1,    16,    -1,    18,    19,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,     4,    67,     6,     7,    -1,
       9,    10,    11,    -1,    13,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,     4,    -1,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,    18,
      19,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    65,    -1,    67,     6,
       7,    -1,     9,    10,    -1,    12,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,     6,     7,    -1,
       9,    10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    22,    23,     6,     7,    -1,     9,    10,    -1,    -1,
      -1,    14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,
      23,     6,     7,    -1,     9,    10,    -1,    12,    -1,    14,
      -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,    -1,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      65,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,
      -1,    18,    19,    -1,    -1,    22,    23,     6,     7,    -1,
       9,    10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,    -1,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    65,     6,     7,    -1,
       9,    10,    -1,    -1,     8,    14,    -1,    16,    12,    18,
      -1,    -1,    -1,    22,    23,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    65,     8,    -1,    -1,
      11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,     8,    -1,    -1,    11,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,     6,     7,     9,    10,    14,    16,    18,
      22,    23,    45,    50,    52,    54,    55,    57,    58,    60,
      61,    62,    63,    64,    65,    67,    75,    76,    77,    79,
      80,    82,    83,    84,    85,    87,    94,    97,   101,   103,
     104,   110,   111,   112,   113,   120,   126,   130,   133,   134,
      76,    14,    14,    16,    62,    82,   101,    82,   101,   102,
       4,    17,    67,    82,   101,   105,   107,   108,   137,    19,
     105,   108,   137,    82,   101,    82,   101,    82,    93,   101,
      82,   101,   121,    16,    62,    14,    62,    93,     4,    67,
      82,   101,     6,     7,    10,    14,    61,    62,    63,    64,
      65,    66,    81,    84,     0,    79,     4,    11,    13,    67,
      78,    81,     8,    12,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    95,
      98,    99,   100,   135,    14,    14,     8,    20,    21,    34,
      35,    95,    98,   100,   135,    44,    12,    62,    82,    86,
     101,   110,    86,   108,    20,    20,    11,    15,    11,    15,
      11,    15,     4,    17,   107,   108,     4,    11,    13,    17,
      67,    82,   101,   106,   109,   105,   108,    19,   108,    19,
     105,   108,     4,    11,    13,    46,   116,   117,     4,    11,
      13,    67,   122,    17,    62,    88,    90,    14,    44,    89,
      62,    44,    11,    13,    46,    56,   132,   137,    76,     4,
      86,    20,    98,    67,    78,    78,     4,    11,    13,    82,
     101,    82,   101,    82,   101,    82,   101,    82,   101,    82,
     101,    82,   101,    82,   101,    82,   101,    82,   101,    82,
     101,    82,   101,    82,   101,    82,   101,    82,   101,    82,
     101,    12,    82,   101,   134,    12,    82,    96,   101,    12,
      96,    86,    86,     6,     7,     9,    14,    62,    82,    82,
     101,    82,   101,    96,    96,    12,    82,   101,   134,    11,
      15,    15,    17,    82,   101,    62,    82,    82,   101,    82,
     101,    82,   101,    17,     4,    67,     4,    11,    13,    67,
      82,   101,   106,    17,   108,    17,    19,    19,   108,    19,
       4,     4,   116,    46,    76,   114,     4,     4,   122,    51,
      67,   123,   136,    44,    17,    11,    15,    90,    62,    11,
      13,    91,   137,    44,    82,   101,   127,     4,     4,     4,
      11,    13,     4,    11,    13,    76,   131,    59,    76,    15,
      62,    82,    78,    82,   101,    15,    15,    62,    82,    82,
      12,    82,   101,   110,     4,    67,     4,    17,    19,     4,
      47,    48,    49,   118,   119,   123,   101,     4,    49,    51,
     118,    51,    67,    62,    44,    62,    15,    89,     4,     4,
      76,    92,   127,     4,    11,    13,    56,   128,     4,     4,
       4,     4,    49,     4,    67,    59,     4,     4,    11,    13,
      93,    76,   115,    49,    49,   118,     4,    11,    13,    46,
      67,   125,   101,   115,   101,     4,    89,    62,    91,    53,
      15,     4,     4,     4,    76,   129,    76,     4,     4,    67,
       4,     4,   117,    49,   115,     4,     4,     4,    11,     4,
      76,   124,    67,   125,    49,    67,   125,    91,    89,    92,
     128,    49,    49,    76,    76,     4,   114,    49,     4,   124,
       4,   124,     4,   124,    92,    91,    53,   129,    49,    49,
      76,   118,   119,   124,   124,    53,    92,    49,    49,   115,
      53
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
# if YYLTYPE_IS_TRIVIAL
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

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
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



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

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

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
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { Parser::getInstance()->setTree((yyvsp[(1) - (1)].t_seq_exp)); }
    break;

  case 3:

    { Parser::getInstance()->setTree((yyvsp[(2) - (2)].t_seq_exp)); }
    break;

  case 4:

    {
						  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[(1) - (1)].t_list_exp));
						}
    break;

  case 5:

    {
						  (yyvsp[(2) - (2)].t_exp)->set_verbose(true);
						  (yyvsp[(1) - (2)].t_list_exp)->push_back((yyvsp[(2) - (2)].t_exp));
						  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[(1) - (2)].t_list_exp));
						}
    break;

  case 6:

    {
						  (yyvsp[(2) - (3)].t_exp)->set_verbose(true);
						  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(2) - (3)].t_exp));
						  (yyvsp[(1) - (3)].t_list_exp)->push_back(new ast::CommentExp((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].comment)));
						  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[(1) - (3)].t_list_exp));
						}
    break;

  case 7:

    {
						  ast::exps_t *tmp = new ast::exps_t;
						  (yyvsp[(1) - (1)].t_exp)->set_verbose(true);
						  tmp->push_front((yyvsp[(1) - (1)].t_exp));
						  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
						}
    break;

  case 8:

    {
						  ast::exps_t *tmp = new ast::exps_t;
						  (yyvsp[(1) - (2)].t_exp)->set_verbose(true);
						  tmp->push_front(new ast::CommentExp((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].comment)));
						  tmp->push_front((yyvsp[(1) - (2)].t_exp));
						  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
						}
    break;

  case 9:

    {
							  (yyvsp[(2) - (3)].t_exp)->set_verbose((yyvsp[(3) - (3)].mute));
							  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(2) - (3)].t_exp));
							  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
							}
    break;

  case 10:

    {
							  (yyvsp[(2) - (4)].t_exp)->set_verbose((yyvsp[(4) - (4)].mute));
							  (yyvsp[(1) - (4)].t_list_exp)->push_back((yyvsp[(2) - (4)].t_exp));
							  (yyvsp[(1) - (4)].t_list_exp)->push_back(new ast::CommentExp((yylsp[(3) - (4)]), (yyvsp[(3) - (4)].comment)));
							  (yyval.t_list_exp) = (yyvsp[(1) - (4)].t_list_exp);
							}
    break;

  case 11:

    {
							  ast::exps_t *tmp = new ast::exps_t;
							  tmp->push_front(new ast::CommentExp((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].comment)));
							  (yyvsp[(1) - (3)].t_exp)->set_verbose((yyvsp[(3) - (3)].mute));
							  tmp->push_front((yyvsp[(1) - (3)].t_exp));
							  (yyval.t_list_exp) = tmp;
							}
    break;

  case 12:

    {
							  ast::exps_t *tmp = new ast::exps_t;
							  (yyvsp[(1) - (2)].t_exp)->set_verbose((yyvsp[(2) - (2)].mute));
							  tmp->push_front((yyvsp[(1) - (2)].t_exp));
							  (yyval.t_list_exp) = tmp;
							}
    break;

  case 13:

    { (yyval.mute) = false; }
    break;

  case 14:

    { (yyval.mute) = true; }
    break;

  case 15:

    { (yyval.mute) = true; }
    break;

  case 16:

    { (yyval.mute) = false; }
    break;

  case 17:

    { (yyval.mute) = true; }
    break;

  case 18:

    { (yyval.mute) = (yyvsp[(1) - (2)].mute); }
    break;

  case 19:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_function_dec); }
    break;

  case 20:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 21:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_assign_exp); }
    break;

  case 22:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_if_exp); }
    break;

  case 23:

    { (yyval.t_exp) = new ast::CommentExp((yyloc), new std::string("!! FIXME : Select Control Sequence !!")); }
    break;

  case 24:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_for_exp); }
    break;

  case 25:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_while_exp); }
    break;

  case 26:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_try_exp); }
    break;

  case 27:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 28:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 29:

    { (yyval.t_exp) = new ast::BreakExp((yyloc)); }
    break;

  case 30:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_return_exp); }
    break;

  case 31:

    { (yyval.t_exp) = new ast::CommentExp((yyloc), (yyvsp[(1) - (1)].comment)); }
    break;

  case 32:

    {
  (yyval.t_exp) = new ast::CommentExp((yyloc), new std::string("@@ ERROR RECOVERY @@"));
  StopOnError();
  }
    break;

  case 33:

    {
						  (yyvsp[(1) - (2)].t_call_exp)->args_get().push_back((yyvsp[(2) - (2)].t_string_exp));
						  (yyval.t_call_exp) = (yyvsp[(1) - (2)].t_call_exp);
						}
    break;

  case 34:

    {
						  ast::exps_t *tmp = new ast::exps_t;
						  tmp->push_front((yyvsp[(2) - (2)].t_string_exp));
						  (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (2)]), *new symbol::Symbol(*(yyvsp[(1) - (2)].str))), *tmp);
						}
    break;

  case 35:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].str)); }
    break;

  case 36:

    {
						  std::stringstream tmp;
						  tmp << (yyvsp[(1) - (1)].number);
						  (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
						}
    break;

  case 37:

    {
						  std::stringstream tmp;
						  tmp << (yyvsp[(1) - (1)].number);
						  (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
						}
    break;

  case 38:

    {
						  std::stringstream tmp;
						  tmp << (yyvsp[(1) - (1)].number);
						  (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
						}
    break;

  case 39:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].str)); }
    break;

  case 40:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("$")); }
    break;

  case 41:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("%t")); }
    break;

  case 42:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("%f")); }
    break;

  case 43:

    {
						  std::stringstream tmp;
						  tmp << (yyvsp[(1) - (3)].t_string_exp)->value_get() << "." << *(yyvsp[(3) - (3)].str);
						  (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
						}
    break;

  case 44:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("!! FIXME : implicitCallable implicitCallable DOT functionCall !!")); }
    break;

  case 45:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("!! FIXME : implicitCallable simpleFunctionCall !!")); }
    break;

  case 46:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("!! FIXME : implicitCallable implicitCallable rightOperand !!")); }
    break;

  case 47:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].path)); }
    break;

  case 48:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 49:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 50:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 51:

    { (yyval.t_exp) = (yyvsp[(2) - (3)].t_exp); }
    break;

  case 52:

    { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 53:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (4)]), *new symbol::Symbol("%t")), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 54:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (4)]), *new symbol::Symbol("%f")), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 55:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (4)]), *new symbol::Symbol(*(yyvsp[(1) - (4)].str))), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 56:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *(yyvsp[(1) - (4)].t_call_exp), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 57:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *(yyvsp[(1) - (4)].t_call_exp), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 58:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
				}
    break;

  case 59:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
				}
    break;

  case 60:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				  (yyval.t_list_exp)->push_front(new ast::ColonVar((yyloc)));
				}
    break;

  case 61:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_assign_exp));
				}
    break;

  case 62:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				}
    break;

  case 63:

    {
				  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
				  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 64:

    {
				  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
				  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 65:

    {
				  (yyvsp[(1) - (3)].t_list_exp)->push_back(new ast::ColonVar((yyloc)));
			          (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 66:

    {
				  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_assign_exp));
				  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 67:

    {
				  (yyval.t_list_exp) = (yyvsp[(1) - (2)].t_list_exp);
				}
    break;

  case 68:

    {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar((yylsp[(2) - (8)]), *new symbol::Symbol(*(yyvsp[(2) - (8)].str))));
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(4) - (8)].str)),
							    *new ast::ArrayListVar((yylsp[(5) - (8)]), *(yyvsp[(5) - (8)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(2) - (8)]), *tmp),
							    *(yyvsp[(7) - (8)].t_seq_exp));
				}
    break;

  case 69:

    {
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(6) - (10)].str)),
							    *new ast::ArrayListVar((yylsp[(7) - (10)]), *(yyvsp[(7) - (10)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(3) - (10)]) ,*(yyvsp[(3) - (10)].t_list_var)),
							    *(yyvsp[(9) - (10)].t_seq_exp));
				}
    break;

  case 70:

    {
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(5) - (9)].str)),
							    *new ast::ArrayListVar((yylsp[(6) - (9)]), *(yyvsp[(6) - (9)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(2) - (9)]), *new ast::vars_t),
							    *(yyvsp[(8) - (9)].t_seq_exp));
				}
    break;

  case 71:

    {
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(2) - (6)].str)),
							    *new ast::ArrayListVar((yylsp[(3) - (6)]), *(yyvsp[(3) - (6)].t_list_var)),
							    *new ast::ArrayListVar((yyloc), *new ast::vars_t),
							    *(yyvsp[(5) - (6)].t_seq_exp));
				}
    break;

  case 72:

    { (yyval.t_list_var) = (yyvsp[(1) - (1)].t_list_var); }
    break;

  case 73:

    { (yyval.t_list_var) = (yyvsp[(2) - (3)].t_list_var); }
    break;

  case 74:

    { (yyval.t_list_var) = new ast::vars_t;	}
    break;

  case 75:

    { (yyval.t_list_var) = new ast::vars_t;	}
    break;

  case 76:

    {
				  (yyvsp[(1) - (3)].t_list_var)->push_back(new ast::SimpleVar((yylsp[(3) - (3)]), *new symbol::Symbol(*(yyvsp[(3) - (3)].str))));
				  (yyval.t_list_var) = (yyvsp[(1) - (3)].t_list_var);
				}
    break;

  case 77:

    {
				  (yyval.t_list_var) = new ast::vars_t;
				  (yyval.t_list_var)->push_front(new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(1) - (1)].str))));
				}
    break;

  case 78:

    { /* !! Do Nothing !! */ }
    break;

  case 79:

    { /* !! Do Nothing !! */ }
    break;

  case 80:

    { /* !! Do Nothing !! */ }
    break;

  case 81:

    { /* !! Do Nothing !! */ }
    break;

  case 82:

    { /* !! Do Nothing !! */ }
    break;

  case 83:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 84:

    {
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty function body")));
				  #endif
				  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
				}
    break;

  case 85:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 86:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 87:

    { (yyval.t_math_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), (yyvsp[(2) - (3)].t_op_exp_oper), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 88:

    { (yyval.t_math_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), (yyvsp[(2) - (3)].t_op_exp_oper), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 89:

    { (yyval.t_math_exp) = new ast::LogicalOpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), (yyvsp[(2) - (3)].t_op_exp_oper), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 90:

    { (yyval.t_math_exp) = new ast::LogicalOpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), (yyvsp[(2) - (3)].t_op_exp_oper), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 91:

    { (yyval.t_op_exp_oper) = ast::OpExp::binaryAnd; }
    break;

  case 92:

    { (yyval.t_op_exp_oper) = ast::OpExp::binaryOr; }
    break;

  case 93:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 94:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 95:

    { (yyval.t_exp) = new ast::ColonVar((yyloc)); }
    break;

  case 96:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_op_exp)->oper_get(), (yyvsp[(2) - (2)].t_op_exp)->right_get()); }
    break;

  case 97:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_op_exp)->oper_get(), (yyvsp[(2) - (2)].t_op_exp)->right_get()); }
    break;

  case 98:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 99:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 100:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 101:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 102:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 103:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 104:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 105:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 106:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 107:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 108:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 109:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 110:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_Conjugate_); }
    break;

  case 111:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_NonConjugate_); }
    break;

  case 112:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_Conjugate_); }
    break;

  case 113:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_NonConjugate_); }
    break;

  case 114:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::plus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 115:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::plus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 116:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 117:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 118:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::times, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 119:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::times, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 120:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 121:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 122:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 123:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 124:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::divide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 125:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::divide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 126:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 127:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 128:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krondivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 129:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krondivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 130:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::controldivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 131:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::controldivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 132:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 133:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 134:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 135:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 136:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 137:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 138:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 139:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 140:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 141:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 142:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), *new ast::DoubleExp((yyloc), 1.0), *(yyvsp[(1) - (1)].t_exp)); }
    break;

  case 143:

    { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 144:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(3) - (3)].str)))); }
    break;

  case 145:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 146:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 147:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 148:

    { (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_implicit_list)->step_get(), (yyvsp[(2) - (2)].t_implicit_list)->end_get()); }
    break;

  case 149:

    { (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_implicit_list)->step_get(), (yyvsp[(2) - (2)].t_implicit_list)->end_get()); }
    break;

  case 150:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_matrix_exp); }
    break;

  case 151:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_cell_exp); }
    break;

  case 152:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 153:

    { (yyval.t_exp) = new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(1) - (1)].str))); }
    break;

  case 154:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[(1) - (1)].number)); }
    break;

  case 155:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[(1) - (1)].number)); }
    break;

  case 156:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[(1) - (1)].number)); }
    break;

  case 157:

    { (yyval.t_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].str)); }
    break;

  case 158:

    { (yyval.t_exp) = new ast::DollarVar((yyloc)); }
    break;

  case 159:

    { (yyval.t_exp) = new ast::BoolExp((yyloc), true); }
    break;

  case 160:

    { (yyval.t_exp) = new ast::BoolExp((yyloc), false); }
    break;

  case 161:

    { (yyval.t_exp) = (yyvsp[(2) - (3)].t_exp); }
    break;

  case 162:

    { (yyval.t_exp) = (yyvsp[(2) - (3)].t_arraylist_exp); }
    break;

  case 163:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_math_exp); }
    break;

  case 164:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_arraylist_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 165:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_arraylist_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 166:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 167:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 168:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 169:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
}
    break;

  case 170:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(2) - (3)].t_list_mline)); }
    break;

  case 171:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(3) - (4)].t_list_mline)); }
    break;

  case 172:

    {
								  (yyvsp[(2) - (4)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(2) - (4)].t_list_mline));
								}
    break;

  case 173:

    {
								  (yyvsp[(3) - (5)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(4) - (5)]), *(yyvsp[(4) - (5)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(3) - (5)].t_list_mline));
								}
    break;

  case 174:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(2) - (3)]), *(yyvsp[(2) - (3)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
								}
    break;

  case 175:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
}
    break;

  case 176:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *new std::list<ast::MatrixLineExp *>); }
    break;

  case 177:

    { (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(2) - (3)].t_list_mline)); }
    break;

  case 178:

    { (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(3) - (4)].t_list_mline)); }
    break;

  case 179:

    {
								  (yyvsp[(2) - (4)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(2) - (4)].t_list_mline));
								}
    break;

  case 180:

    {
								  (yyvsp[(3) - (5)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(4) - (5)]), *(yyvsp[(4) - (5)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(3) - (5)].t_list_mline));
								}
    break;

  case 181:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(2) - (3)]), *(yyvsp[(2) - (3)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);
								}
    break;

  case 182:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);
								}
    break;

  case 183:

    { (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *new std::list<ast::MatrixLineExp *>); }
    break;

  case 184:

    {
					  (yyvsp[(1) - (2)].t_list_mline)->push_back((yyvsp[(2) - (2)].t_matrixline_exp));
					  (yyval.t_list_mline) = (yyvsp[(1) - (2)].t_list_mline);
					}
    break;

  case 185:

    {
					  (yyval.t_list_mline) = new std::list<ast::MatrixLineExp *>;
					  (yyval.t_list_mline)->push_front((yyvsp[(1) - (1)].t_matrixline_exp));
					}
    break;

  case 186:

    { /* !! Do Nothing !! */ }
    break;

  case 187:

    { /* !! Do Nothing !! */ }
    break;

  case 188:

    { /* !! Do Nothing !! */ }
    break;

  case 189:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (2)].t_list_exp)); }
    break;

  case 190:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (3)].t_list_exp)); }
    break;

  case 191:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (4)].t_list_exp)); }
    break;

  case 192:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (3)].t_list_exp)); }
    break;

  case 193:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (4)].t_list_exp)); }
    break;

  case 194:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (5)].t_list_exp)); }
    break;

  case 195:

    {
								  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
								}
    break;

  case 196:

    {
								  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
								}
    break;

  case 197:

    {
								  (yyvsp[(1) - (2)].t_list_exp)->push_back((yyvsp[(2) - (2)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (2)].t_list_exp);
								}
    break;

  case 198:

    {
								  (yyvsp[(1) - (2)].t_list_exp)->push_back((yyvsp[(2) - (2)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (2)].t_list_exp);
								}
    break;

  case 199:

    {
								  (yyval.t_list_exp) = new ast::exps_t;
								  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
								}
    break;

  case 200:

    {
								  (yyval.t_list_exp) = new ast::exps_t;
								  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
								}
    break;

  case 201:

    { /* !! Do Nothing !! */ }
    break;

  case 202:

    { /* !! Do Nothing !! */ }
    break;

  case 203:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 204:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 205:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 206:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 207:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::ColonVar((yylsp[(3) - (3)]))); }
    break;

  case 208:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::ColonVar((yylsp[(3) - (3)]))); }
    break;

  case 209:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_return_exp)); }
    break;

  case 210:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_return_exp)); }
    break;

  case 211:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(3) - (3)].str)))); }
    break;

  case 212:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 213:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 214:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 215:

    { (yyval.t_exp) = new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(1) - (1)].str))); }
    break;

  case 216:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_assignlist_exp); }
    break;

  case 217:

    { (yyval.t_assignlist_exp) = new ast::AssignListExp((yyloc), *(yyvsp[(2) - (3)].t_list_exp)); }
    break;

  case 218:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[(2) - (5)].t_exp), *(yyvsp[(4) - (5)].t_seq_exp)); }
    break;

  case 219:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[(2) - (7)].t_exp), *(yyvsp[(4) - (7)].t_seq_exp), *(yyvsp[(6) - (7)].t_seq_exp)); }
    break;

  case 220:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[(2) - (6)].t_exp), *(yyvsp[(4) - (6)].t_seq_exp), *(yyvsp[(5) - (6)].t_seq_exp)); }
    break;

  case 221:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 222:

    {
							  ast::exps_t *tmp = new ast::exps_t;
							  #ifdef BUILD_DEBUG_AST
							  tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty then body")));
							  #endif
							  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
							}
    break;

  case 223:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 224:

    {
							  ast::exps_t *tmp = new ast::exps_t;
							  #ifdef BUILD_DEBUG_AST
							  tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty else body")));
							  #endif
							  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
							}
    break;

  case 225:

    { /* !! Do Nothing !! */ }
    break;

  case 226:

    { /* !! Do Nothing !! */ }
    break;

  case 227:

    { /* !! Do Nothing !! */ }
    break;

  case 228:

    { /* !! Do Nothing !! */ }
    break;

  case 229:

    { /* !! Do Nothing !! */ }
    break;

  case 230:

    { /* !! Do Nothing !! */ }
    break;

  case 231:

    { /* !! Do Nothing !! */ }
    break;

  case 232:

    { /* !! Do Nothing !! */ }
    break;

  case 233:

    { /* !! Do Nothing !! */ }
    break;

  case 234:

    { /* !! Do Nothing !! */ }
    break;

  case 235:

    { /* !! Do Nothing !! */ }
    break;

  case 236:

    { /* !! Do Nothing !! */ }
    break;

  case 237:

    { /* !! Do Nothing !! */ }
    break;

  case 238:

    { /* !! Do Nothing !! */ }
    break;

  case 239:

    { /* !! Do Nothing !! */ }
    break;

  case 240:

    { /* !! Do Nothing !! */ }
    break;

  case 241:

    { /* !! Do Nothing !! */ }
    break;

  case 242:

    {
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp((yyloc), *(yyvsp[(2) - (4)].t_exp), *(yyvsp[(4) - (4)].t_seq_exp)) );
										(yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
									}
    break;

  case 243:

    {
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp((yyloc), *(yyvsp[(2) - (6)].t_exp), *(yyvsp[(4) - (6)].t_seq_exp), *(yyvsp[(6) - (6)].t_seq_exp)) );
										(yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);

									}
    break;

  case 244:

    {
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp((yyloc), *(yyvsp[(2) - (5)].t_exp), *(yyvsp[(5) - (5)].t_seq_exp)) );
										(yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
									}
    break;

  case 251:

    { /* !! Do Nothing !! */ }
    break;

  case 252:

    { /* !! Do Nothing !! */ }
    break;

  case 253:

    { /* !! Do Nothing !! */ }
    break;

  case 262:

    { /* !! Do Nothing !! */ }
    break;

  case 263:

    { /* !! Do Nothing !! */ }
    break;

  case 264:

    { /* !! Do Nothing !! */ }
    break;

  case 265:

    { /* !! Do Nothing !! */ }
    break;

  case 266:

    { /* !! Do Nothing !! */ }
    break;

  case 267:

    { /* !! Do Nothing !! */ }
    break;

  case 268:

    { /* !! Do Nothing !! */ }
    break;

  case 269:

    { /* !! Do Nothing !! */ }
    break;

  case 270:

    { /* !! Do Nothing !! */ }
    break;

  case 271:

    { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[(3) - (7)]), *new symbol::Symbol(*(yyvsp[(2) - (7)].str)), *(yyvsp[(4) - (7)].t_exp)), *(yyvsp[(6) - (7)].t_seq_exp)); }
    break;

  case 272:

    { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[(4) - (9)]), *new symbol::Symbol(*(yyvsp[(3) - (9)].str)), *(yyvsp[(5) - (9)].t_exp)), *(yyvsp[(8) - (9)].t_seq_exp)); }
    break;

  case 273:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 274:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 275:

    { /* !! Do Nothing !! */ }
    break;

  case 276:

    { /* !! Do Nothing !! */ }
    break;

  case 277:

    { /* !! Do Nothing !! */ }
    break;

  case 278:

    { /* !! Do Nothing !! */ }
    break;

  case 279:

    { /* !! Do Nothing !! */ }
    break;

  case 280:

    { /* !! Do Nothing !! */ }
    break;

  case 281:

    { /* !! Do Nothing !! */ }
    break;

  case 282:

    { /* !! Do Nothing !! */ }
    break;

  case 283:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 284:

    {
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty for body")));
				  #endif
				  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
				}
    break;

  case 285:

    { (yyval.t_while_exp) = new ast::WhileExp((yyloc), *(yyvsp[(2) - (5)].t_exp), *(yyvsp[(4) - (5)].t_seq_exp)); }
    break;

  case 286:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 287:

    {
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty while body")));
				  #endif
				  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
				}
    break;

  case 288:

    { /* !! Do Nothing !! */ }
    break;

  case 289:

    { /* !! Do Nothing !! */ }
    break;

  case 290:

    { /* !! Do Nothing !! */ }
    break;

  case 291:

    { /* !! Do Nothing !! */ }
    break;

  case 292:

    { /* !! Do Nothing !! */ }
    break;

  case 293:

    { /* !! Do Nothing !! */ }
    break;

  case 294:

    { /* !! Do Nothing !! */ }
    break;

  case 295:

    { /* !! Do Nothing !! */ }
    break;

  case 296:

    { /* !! Do Nothing !! */ }
    break;

  case 297:

    { /* !! Do Nothing !! */ }
    break;

  case 298:

    { /* !! Do Nothing !! */ }
    break;

  case 299:

    { /* !! Do Nothing !! */ }
    break;

  case 300:

    { /* !! Do Nothing !! */ }
    break;

  case 301:

    { /* !! Do Nothing !! */ }
    break;

  case 302:

    { /* !! Do Nothing !! */ }
    break;

  case 303:

    { /* !! Do Nothing !! */ }
    break;

  case 304:

    { /* !! Do Nothing !! */ }
    break;

  case 305:

    { (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(3) - (7)].t_seq_exp), *(yyvsp[(6) - (7)].t_seq_exp)); }
    break;

  case 306:

    {
								  (yyvsp[(4) - (8)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(2) - (8)]), (yyvsp[(2) - (8)].comment)));
								  (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(4) - (8)].t_seq_exp), *(yyvsp[(7) - (8)].t_seq_exp));
								}
    break;

  case 307:

    {
								  (yyvsp[(7) - (8)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(5) - (8)]), (yyvsp[(5) - (8)].comment)));
								  (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(3) - (8)].t_seq_exp), *(yyvsp[(7) - (8)].t_seq_exp));
								}
    break;

  case 308:

    {
								  (yyvsp[(4) - (9)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(2) - (9)]), (yyvsp[(2) - (9)].comment)));
								  (yyvsp[(8) - (9)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(6) - (9)]), (yyvsp[(6) - (9)].comment)));
								  (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(4) - (9)].t_seq_exp), *(yyvsp[(8) - (9)].t_seq_exp));
								}
    break;

  case 309:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc)); }
    break;

  case 310:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc), (yyvsp[(2) - (2)].t_exp)); }
    break;

  case 311:

    { (yyval.t_op_exp_oper) = ast::OpExp::eq; }
    break;

  case 312:

    { (yyval.t_op_exp_oper) = ast::OpExp::ne; }
    break;

  case 313:

    { (yyval.t_op_exp_oper) = ast::OpExp::lt; }
    break;

  case 314:

    { (yyval.t_op_exp_oper) = ast::OpExp::le; }
    break;

  case 315:

    { (yyval.t_op_exp_oper) = ast::OpExp::gt; }
    break;

  case 316:

    { (yyval.t_op_exp_oper) = ast::OpExp::ge; }
    break;


/* Line 1267 of yacc.c.  */

      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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




void yyerror(std::string msg) {
  if(!Parser::getInstance()->isStrictMode()
     || Parser::getInstance()->getExitStatus() == Parser::Succeded)
    {
      Parser::PrintError(msg);
      Parser::getInstance()->setExitStatus(Parser::Failed);
    }
}

