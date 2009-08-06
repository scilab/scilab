
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



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
     CLASS = 309,
     ENDCLASS = 310,
     PROPERTY = 311,
     FOR = 312,
     WHILE = 313,
     DO = 314,
     BREAK = 315,
     TRY = 316,
     CATCH = 317,
     RETURN = 318,
     STR = 319,
     ID = 320,
     VARINT = 321,
     VARFLOAT = 322,
     NUM = 323,
     PATH = 324,
     COMMENT = 325,
     BLOCKCOMMENT = 326,
     TOPLEVEL = 327,
     HIGHLEVEL = 328,
     UPLEVEL = 329,
     LISTABLE = 330,
     FUNCTIONCALL = 331
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
#define CLASS 309
#define ENDCLASS 310
#define PROPERTY 311
#define FOR 312
#define WHILE 313
#define DO 314
#define BREAK 315
#define TRY 316
#define CATCH 317
#define RETURN 318
#define STR 319
#define ID 320
#define VARINT 321
#define VARFLOAT 322
#define NUM 323
#define PATH 324
#define COMMENT 325
#define BLOCKCOMMENT 326
#define TOPLEVEL 327
#define HIGHLEVEL 328
#define UPLEVEL 329
#define LISTABLE 330
#define FUNCTIONCALL 331




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
  
  ast::ClassDec*	t_class_dec;
  ast::SlotDec*		t_slot_dec;
  ast::slots_t*		t_list_slots;

  ast::ArrayListExp*	t_arraylist_exp;
  ast::AssignListExp*	t_assignlist_exp;
  ast::ArrayListVar*	t_arraylist_var;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  107
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2673

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  69
/* YYNRULES -- Number of rules.  */
#define YYNRULES  333
/* YYNRULES -- Number of states.  */
#define YYNSTATES  543

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   331

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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    17,    19,    22,
      26,    31,    35,    38,    40,    42,    44,    47,    50,    53,
      55,    57,    59,    61,    63,    65,    67,    69,    71,    73,
      75,    77,    79,    81,    83,    86,    89,    91,    93,    95,
      97,    99,   101,   103,   105,   109,   113,   115,   118,   120,
     122,   124,   126,   130,   133,   138,   143,   148,   153,   158,
     160,   162,   164,   166,   167,   171,   175,   179,   183,   186,
     195,   206,   216,   223,   225,   229,   232,   233,   237,   239,
     241,   243,   246,   248,   251,   253,   254,   262,   268,   272,
     275,   276,   278,   280,   290,   302,   313,   321,   327,   331,
     335,   338,   339,   341,   343,   347,   351,   353,   355,   357,
     360,   363,   366,   369,   372,   375,   379,   383,   387,   391,
     395,   399,   403,   407,   410,   413,   416,   419,   422,   425,
     428,   431,   434,   437,   440,   443,   446,   449,   452,   455,
     458,   461,   464,   467,   470,   473,   476,   479,   482,   485,
     488,   491,   494,   497,   501,   505,   507,   510,   514,   518,
     521,   524,   526,   528,   530,   532,   534,   536,   538,   540,
     542,   544,   546,   550,   554,   556,   560,   564,   568,   572,
     576,   580,   584,   589,   594,   600,   604,   609,   612,   616,
     621,   626,   632,   636,   641,   644,   647,   649,   651,   653,
     656,   659,   663,   668,   672,   677,   683,   687,   691,   694,
     697,   699,   701,   704,   706,   710,   714,   718,   722,   726,
     730,   734,   738,   742,   746,   750,   754,   756,   758,   762,
     768,   776,   783,   785,   786,   788,   789,   791,   794,   796,
     799,   801,   803,   806,   810,   813,   815,   816,   818,   821,
     824,   827,   831,   835,   840,   847,   853,   859,   867,   874,
     883,   885,   887,   889,   892,   895,   900,   906,   912,   919,
     925,   932,   934,   935,   937,   939,   941,   943,   946,   949,
     952,   955,   959,   967,   977,   979,   981,   983,   985,   988,
     990,   993,   995,   998,   999,  1001,  1002,  1008,  1010,  1011,
    1013,  1015,  1017,  1020,  1023,  1025,  1028,  1031,  1033,  1036,
    1039,  1042,  1046,  1050,  1053,  1057,  1061,  1069,  1078,  1087,
    1097,  1099,  1102,  1104,  1106,  1108,  1110,  1112,  1114,  1116,
    1118,  1121,  1125,  1127
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      78,     0,    -1,    79,    -1,     4,    79,    -1,    80,    -1,
      80,    82,    -1,    80,    82,    70,    -1,    82,    -1,    82,
      70,    -1,    80,    82,    81,    -1,    80,    82,    70,    81,
      -1,    82,    70,    81,    -1,    82,    81,    -1,    13,    -1,
      11,    -1,     4,    -1,    81,    13,    -1,    81,    11,    -1,
      81,     4,    -1,    90,    -1,    96,    -1,    85,    -1,   118,
      -1,   121,    -1,   128,    -1,   134,    -1,   138,    -1,   141,
      -1,   109,    -1,    83,    -1,    60,    -1,   142,    -1,    70,
      -1,     1,    -1,    83,    84,    -1,    65,    84,    -1,    65,
      -1,    66,    -1,    68,    -1,    67,    -1,    64,    -1,    10,
      -1,     6,    -1,     7,    -1,    84,    20,    65,    -1,    84,
      20,    85,    -1,    87,    -1,    84,   106,    -1,    69,    -1,
      87,    -1,    86,    -1,    88,    -1,    14,    85,    15,    -1,
       9,    85,    -1,     6,    14,    89,    15,    -1,     7,    14,
      89,    15,    -1,   109,    14,    89,    15,    -1,    87,    14,
      89,    15,    -1,    88,    14,    89,    15,    -1,   109,    -1,
      85,    -1,    12,    -1,   118,    -1,    -1,    89,    11,   109,
      -1,    89,    11,    85,    -1,    89,    11,    12,    -1,    89,
      11,   118,    -1,    89,    11,    -1,    52,    65,    44,    65,
      92,    94,    95,    53,    -1,    52,    16,    91,    17,    44,
      65,    92,    94,    95,    53,    -1,    52,    16,    17,    44,
      65,    92,    94,    95,    53,    -1,    52,    65,    92,    94,
      95,    53,    -1,    93,    -1,    14,    93,    15,    -1,    14,
      15,    -1,    -1,    93,    11,    65,    -1,    65,    -1,   145,
      -1,    13,    -1,    13,     4,    -1,    11,    -1,    11,     4,
      -1,    79,    -1,    -1,    54,    65,    38,    65,    94,    97,
      55,    -1,    54,    65,    94,    97,    55,    -1,    97,    98,
      94,    -1,    97,    94,    -1,    -1,    99,    -1,   100,    -1,
      52,   101,    65,    44,    65,    92,    94,    95,    53,    -1,
      52,   101,    16,    91,    17,    44,    65,    92,    94,    95,
      53,    -1,    52,   101,    16,    17,    44,    65,    92,    94,
      95,    53,    -1,    52,   101,    65,    92,    94,    95,    53,
      -1,    56,   101,    65,    44,   109,    -1,    56,   101,    65,
      -1,    14,    93,    15,    -1,    14,    15,    -1,    -1,    85,
      -1,   109,    -1,   109,   143,   104,    -1,    85,   143,   104,
      -1,   109,    -1,    85,    -1,    12,    -1,   109,   106,    -1,
      85,   106,    -1,    23,   109,    -1,    23,    85,    -1,    22,
     109,    -1,    22,    85,    -1,   109,    34,   109,    -1,   109,
      34,    85,    -1,    85,    34,   109,    -1,    85,    34,    85,
      -1,   109,    35,   109,    -1,   109,    35,    85,    -1,    85,
      35,   109,    -1,    85,    35,    85,    -1,   109,     8,    -1,
     109,    21,    -1,    85,     8,    -1,    85,    21,    -1,    22,
     109,    -1,    22,    85,    -1,    23,   109,    -1,    23,    85,
      -1,    24,   109,    -1,    24,    85,    -1,    25,   109,    -1,
      25,    85,    -1,    26,   109,    -1,    26,    85,    -1,    27,
     109,    -1,    27,    85,    -1,    28,   109,    -1,    28,    85,
      -1,    30,   109,    -1,    30,    85,    -1,    29,   109,    -1,
      29,    85,    -1,    31,   109,    -1,    31,    85,    -1,    32,
     109,    -1,    32,    85,    -1,    33,   109,    -1,    33,    85,
      -1,    12,   109,    -1,    12,    85,    -1,   107,    12,   109,
      -1,   107,    12,    85,    -1,   107,    -1,     9,   109,    -1,
     109,    20,    65,    -1,    85,    20,   109,    -1,   109,   108,
      -1,    85,   108,    -1,   112,    -1,   111,    -1,   105,    -1,
      65,    -1,    66,    -1,    68,    -1,    67,    -1,    64,    -1,
      10,    -1,     6,    -1,     7,    -1,    14,   109,    15,    -1,
      14,   110,    15,    -1,   103,    -1,   110,    11,   109,    -1,
     110,    11,    85,    -1,   109,    11,   109,    -1,    85,    11,
      85,    -1,    85,    11,   109,    -1,   109,    11,    85,    -1,
      18,   113,    19,    -1,    18,   145,   113,    19,    -1,    18,
     113,   116,    19,    -1,    18,   145,   113,   116,    19,    -1,
      18,   116,    19,    -1,    18,   145,   116,    19,    -1,    18,
      19,    -1,    16,   113,    17,    -1,    16,   145,   113,    17,
      -1,    16,   113,   116,    17,    -1,    16,   145,   113,   116,
      17,    -1,    16,   116,    17,    -1,    16,   145,   116,    17,
      -1,    16,    17,    -1,   113,   115,    -1,   115,    -1,    13,
      -1,     4,    -1,    13,     4,    -1,   116,   114,    -1,   116,
      70,     4,    -1,   116,    13,    70,     4,    -1,   116,   117,
     114,    -1,   116,   117,    70,     4,    -1,   116,   117,    13,
      70,     4,    -1,   116,   117,   109,    -1,   116,   117,    85,
      -1,   116,   109,    -1,   116,    85,    -1,   109,    -1,    85,
      -1,   117,    11,    -1,    11,    -1,   119,    44,   109,    -1,
     119,    44,    85,    -1,    85,    44,   109,    -1,    85,    44,
      85,    -1,   119,    44,    12,    -1,    85,    44,    12,    -1,
     119,    44,   142,    -1,    85,    44,   142,    -1,   109,    20,
      65,    -1,   109,    20,    85,    -1,    85,    20,   109,    -1,
      85,    20,    85,    -1,    65,    -1,   120,    -1,    16,   116,
      17,    -1,    45,   102,   125,   122,    49,    -1,    45,   102,
     125,   122,   126,   123,    49,    -1,    45,   102,   125,   122,
     127,    49,    -1,    79,    -1,    -1,    79,    -1,    -1,    13,
      -1,    13,     4,    -1,    11,    -1,    11,     4,    -1,     4,
      -1,    46,    -1,   124,    46,    -1,   124,    46,     4,    -1,
      46,   124,    -1,   124,    -1,    -1,    47,    -1,    47,    11,
      -1,    47,    13,    -1,    47,     4,    -1,    47,    11,     4,
      -1,    47,    13,     4,    -1,    48,   102,   125,   122,    -1,
      48,   102,   125,   122,   126,   123,    -1,    48,   102,   125,
     122,   127,    -1,    50,   129,   130,   131,    49,    -1,    50,
     129,   130,   131,   126,   123,    49,    -1,    50,   129,    70,
     130,   131,    49,    -1,    50,   129,    70,   130,   131,   126,
     123,    49,    -1,   109,    -1,    85,    -1,     4,    -1,    11,
       4,    -1,    13,     4,    -1,    51,   109,   133,   132,    -1,
     144,    51,   109,   133,   132,    -1,    51,   109,    70,     4,
     132,    -1,   144,    51,   109,    70,     4,   132,    -1,   131,
      51,   109,   133,   132,    -1,   131,    51,   109,    70,     4,
     132,    -1,    79,    -1,    -1,    46,    -1,    11,    -1,    13,
      -1,     4,    -1,    46,     4,    -1,    11,     4,    -1,    13,
       4,    -1,    46,    11,    -1,    46,    11,     4,    -1,    57,
      65,    44,   135,   136,   137,    49,    -1,    57,    14,    65,
      44,   135,    15,   136,   137,    49,    -1,    85,    -1,   109,
      -1,     4,    -1,    13,    -1,    13,     4,    -1,    11,    -1,
      11,     4,    -1,    59,    -1,    59,     4,    -1,    -1,    79,
      -1,    -1,    58,   102,   140,   139,    49,    -1,    79,    -1,
      -1,    11,    -1,    13,    -1,    59,    -1,    59,    11,    -1,
      59,    13,    -1,    46,    -1,    46,    11,    -1,    46,    13,
      -1,   145,    -1,    11,     4,    -1,    13,     4,    -1,    59,
       4,    -1,    59,    11,     4,    -1,    59,    13,     4,    -1,
      46,     4,    -1,    46,    11,     4,    -1,    46,    13,     4,
      -1,    61,     4,    79,    62,     4,    79,    49,    -1,    61,
      70,     4,    79,    62,     4,    79,    49,    -1,    61,     4,
      79,    62,    70,     4,    79,    49,    -1,    61,    70,     4,
      79,    62,    70,     4,    79,    49,    -1,    63,    -1,    63,
     109,    -1,    36,    -1,    37,    -1,    38,    -1,    39,    -1,
      40,    -1,    41,    -1,    42,    -1,    43,    -1,    70,     4,
      -1,   144,    70,     4,    -1,     4,    -1,    70,     4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   305,   305,   306,   314,   317,   322,   328,   334,   348,
     353,   359,   366,   379,   380,   381,   382,   383,   384,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   419,   423,   438,   439,   444,   449,
     454,   455,   456,   457,   458,   463,   464,   465,   466,   474,
     475,   476,   477,   478,   486,   487,   495,   503,   504,   512,
     516,   520,   524,   528,   531,   535,   539,   543,   547,   557,
     566,   573,   580,   594,   602,   603,   604,   612,   616,   627,
     628,   629,   630,   631,   639,   640,   653,   656,   665,   668,
     669,   676,   677,   685,   695,   703,   711,   725,   731,   743,
     744,   745,   753,   754,   762,   763,   771,   772,   773,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   797,   798,   807,   808,   810,
     811,   813,   814,   815,   816,   817,   818,   820,   821,   822,
     823,   824,   825,   826,   827,   829,   830,   831,   832,   833,
     834,   842,   843,   851,   852,   853,   861,   862,   863,   864,
     865,   866,   867,   868,   869,   870,   871,   872,   873,   874,
     875,   876,   877,   878,   879,   887,   893,   899,   905,   911,
     917,   929,   930,   931,   935,   939,   944,   949,   958,   959,
     960,   964,   968,   973,   978,   986,   990,  1001,  1002,  1003,
    1011,  1012,  1013,  1014,  1015,  1016,  1024,  1028,  1032,  1036,
    1040,  1044,  1055,  1056,  1064,  1065,  1066,  1067,  1069,  1070,
    1072,  1073,  1082,  1083,  1084,  1085,  1086,  1087,  1094,  1103,
    1104,  1105,  1113,  1114,  1128,  1129,  1143,  1144,  1145,  1146,
    1147,  1155,  1156,  1157,  1158,  1159,  1160,  1168,  1169,  1170,
    1171,  1172,  1173,  1181,  1186,  1192,  1208,  1209,  1210,  1211,
    1219,  1220,  1228,  1229,  1230,  1239,  1240,  1241,  1242,  1243,
    1244,  1248,  1249,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1273,  1274,  1282,  1283,  1292,  1293,  1294,  1295,
    1296,  1297,  1298,  1299,  1303,  1304,  1318,  1326,  1327,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1353,  1354,  1355,  1356,  1357,  1365,  1366,  1370,  1374,
    1386,  1387,  1395,  1396,  1397,  1398,  1399,  1400,  1401,  1402,
    1409,  1410,  1418,  1419
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
  "\"select\"", "\"case\"", "\"function\"", "\"endfunction\"", "\"class\"",
  "\"endclass\"", "\"property\"", "\"for\"", "\"while\"", "\"do\"",
  "\"break\"", "\"try\"", "\"catch\"", "\"return\"", "\"string\"",
  "\"identifier\"", "\"integer\"", "\"float\"", "\"number\"", "\"path\"",
  "\"line comment\"", "\"block comment\"", "TOPLEVEL", "HIGHLEVEL",
  "UPLEVEL", "LISTABLE", "FUNCTIONCALL", "$accept", "program",
  "expressions", "recursiveExpression", "expressionLineBreak",
  "expression", "implicitFunctionCall", "implicitCallable", "functionCall",
  "specificFunctionCall", "simpleFunctionCall", "recursiveFunctionCall",
  "functionArgs", "functionDeclaration", "functionDeclarationReturns",
  "functionDeclarationArguments", "idList", "functionDeclarationBreak",
  "functionBody", "classDeclaration", "slotsDeclaration",
  "slotDeclaration", "methodDeclaration", "propertyDeclaration",
  "slotAttributes", "condition", "comparison", "comparable", "operation",
  "rightOperand", "listableBegin", "listableEnd", "variable",
  "variableFields", "cell", "matrix", "matrixOrCellLines",
  "matrixOrCellLineBreak", "matrixOrCellLine", "matrixOrCellColumns",
  "matrixOrCellColumnsBreak", "variableDeclaration", "assignable",
  "multipleResults", "ifControl", "thenBody", "elseBody",
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
     325,   326,   327,   328,   329,   330,   331
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    77,    78,    78,    79,    79,    79,    79,    79,    80,
      80,    80,    80,    81,    81,    81,    81,    81,    81,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    83,    83,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    85,
      85,    85,    85,    85,    86,    86,    87,    88,    88,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    90,
      90,    90,    90,    91,    92,    92,    92,    93,    93,    94,
      94,    94,    94,    94,    95,    95,    96,    96,    97,    97,
      97,    98,    98,    99,    99,    99,    99,   100,   100,   101,
     101,   101,   102,   102,   103,   103,   104,   104,   104,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   107,   107,   108,   108,   108,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   110,   110,   110,   110,   110,
     110,   111,   111,   111,   111,   111,   111,   111,   112,   112,
     112,   112,   112,   112,   112,   113,   113,   114,   114,   114,
     115,   115,   115,   115,   115,   115,   116,   116,   116,   116,
     116,   116,   117,   117,   118,   118,   118,   118,   118,   118,
     118,   118,   119,   119,   119,   119,   119,   119,   120,   121,
     121,   121,   122,   122,   123,   123,   124,   124,   124,   124,
     124,   125,   125,   125,   125,   125,   125,   126,   126,   126,
     126,   126,   126,   127,   127,   127,   128,   128,   128,   128,
     129,   129,   130,   130,   130,   131,   131,   131,   131,   131,
     131,   132,   132,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   134,   134,   135,   135,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   137,   138,   139,   139,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   141,   141,   141,   141,
     142,   142,   143,   143,   143,   143,   143,   143,   143,   143,
     144,   144,   145,   145
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     3,     1,     2,     3,
       4,     3,     2,     1,     1,     1,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     2,     1,     1,
       1,     1,     3,     2,     4,     4,     4,     4,     4,     1,
       1,     1,     1,     0,     3,     3,     3,     3,     2,     8,
      10,     9,     6,     1,     3,     2,     0,     3,     1,     1,
       1,     2,     1,     2,     1,     0,     7,     5,     3,     2,
       0,     1,     1,     9,    11,    10,     7,     5,     3,     3,
       2,     0,     1,     1,     3,     3,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     1,     2,     3,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     5,     3,     4,     2,     3,     4,
       4,     5,     3,     4,     2,     2,     1,     1,     1,     2,
       2,     3,     4,     3,     4,     5,     3,     3,     2,     2,
       1,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     5,
       7,     6,     1,     0,     1,     0,     1,     2,     1,     2,
       1,     1,     2,     3,     2,     1,     0,     1,     2,     2,
       2,     3,     3,     4,     6,     5,     5,     7,     6,     8,
       1,     1,     1,     2,     2,     4,     5,     5,     6,     5,
       6,     1,     0,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     7,     9,     1,     1,     1,     1,     2,     1,
       2,     1,     2,     0,     1,     0,     5,     1,     0,     1,
       1,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       2,     3,     3,     2,     3,     3,     7,     8,     8,     9,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,    33,     0,   170,   171,     0,   169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    30,     0,
     320,   168,   164,   165,   167,   166,    32,     0,     2,     0,
       7,    29,    21,    50,    49,    51,    19,    20,   174,   163,
      28,   162,   161,    22,     0,   227,    23,    24,    25,    26,
      27,    31,     3,    63,    63,     0,   164,    53,   156,     0,
       0,     0,   332,   194,     0,   211,   210,     0,   196,     0,
       0,   187,     0,     0,     0,   114,   113,   112,   111,   102,
     246,   103,   261,   260,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,   321,    42,    43,    41,    40,    36,
      37,    39,    38,    48,    35,    46,     0,     1,     5,    15,
      14,    13,     8,    12,    34,   125,     0,     0,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,   323,   324,   325,   326,   327,   328,
     329,     0,   110,   155,   160,     0,    63,    63,   123,    63,
       0,   124,     0,     0,   109,   159,     0,     0,    61,   164,
      60,     0,    59,    62,     0,     0,     0,     0,     0,    52,
       0,   172,     0,   173,   333,   188,   195,     0,   198,   213,
     197,   192,     0,   209,   208,   200,     0,     0,     0,   181,
       0,   185,     0,     0,   240,   238,   236,   241,   245,     0,
     262,     0,     0,     0,     0,     0,    78,     0,    73,     0,
       0,     0,    82,    80,     0,    90,    79,     0,     0,   299,
     300,   304,   301,     0,   307,     0,     0,     0,    47,     6,
       9,    11,    18,    17,    16,   152,   151,   225,   158,   128,
     127,   130,   129,   132,   131,   134,   133,   136,   135,   138,
     137,   140,   139,   144,   143,   142,   141,   146,   145,   148,
     147,   150,   149,   118,   117,   122,   121,   219,   217,   216,
     221,     0,   108,   107,   105,   106,     0,     0,     0,   157,
     223,   116,   115,   120,   119,   104,   218,   215,   214,   220,
      68,    54,    55,   192,   158,   157,   178,   179,   180,   177,
     176,   175,   190,   199,     0,   201,   212,   197,     0,   207,
     206,   203,   189,     0,   193,   183,   182,     0,   186,   239,
     237,   244,   242,   232,     0,   263,   264,     0,     0,     0,
       0,     0,     0,     0,     0,    75,     0,    76,     0,    83,
      81,     0,     0,     0,   284,   285,   293,   308,   309,   313,
     305,   306,   310,   302,   303,   297,     0,     0,     0,    44,
      45,    10,   154,   153,    57,    58,    56,    66,    65,    64,
      67,   202,     0,   204,   191,   184,   243,   247,     0,   229,
       0,     0,     0,     0,   330,   256,     0,     0,     0,     0,
      76,     0,    77,    74,     0,    84,     0,    90,   101,    87,
     101,    89,     0,    91,    92,     0,   286,   289,   287,   291,
       0,   314,   315,   311,   312,   296,     0,     0,     0,   205,
     250,   248,   249,   246,   234,     0,   231,   258,     0,   276,
     274,   275,   273,     0,     0,     0,     0,     0,   331,     0,
      76,     0,    72,     0,     0,     0,     0,    88,   293,   290,
     288,   292,   294,     0,     0,     0,     0,     0,   251,   252,
       0,   230,     0,   278,   279,   277,   280,     0,   271,   265,
       0,     0,   257,     0,     0,     0,     0,     0,    86,   100,
       0,     0,    76,    98,     0,   282,   316,     0,     0,     0,
     253,   259,   281,   267,     0,   269,     0,   266,     0,     0,
      69,    99,     0,     0,     0,     0,     0,     0,   318,   317,
       0,     0,   255,   270,   268,    71,     0,     0,     0,    76,
       0,    97,   283,   319,   254,    70,    76,     0,     0,     0,
       0,    76,     0,    96,     0,     0,     0,     0,     0,    93,
      95,     0,    94
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    27,   395,    29,   113,    30,    31,   104,    32,    33,
      34,    35,   161,    36,   207,   211,   208,   401,   396,    37,
     342,   402,   403,   404,   445,    80,    38,   274,    39,   154,
     143,   155,    40,    61,    41,    42,    67,   185,    68,    69,
     186,    43,    44,    45,    46,   324,   425,   198,   199,   380,
     381,    47,    84,   204,   330,   469,   434,    48,   346,   410,
     453,    49,   356,   223,    50,    51,   156,   331,   216
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -356
static const yytype_int16 yypact[] =
{
     338,  -356,  1322,    47,    50,  2057,  -356,  2057,   197,  1724,
    2057,  2057,  2057,  2057,    24,    -7,    42,  2057,  -356,    18,
    2057,  -356,   814,  -356,  -356,  -356,  -356,    79,  -356,   974,
     158,  1742,  2187,  -356,    78,    90,  -356,  -356,  -356,  -356,
    2296,  -356,  -356,  -356,   128,  -356,  -356,  -356,  -356,  -356,
    -356,  -356,  -356,  1841,  1841,   197,  -356,  -356,  -356,  2260,
    2224,   146,  -356,  -356,   156,  2368,  2332,  1859,  -356,    32,
    2057,  -356,  1922,   656,  2057,  2610,  2638,  2610,  2638,  2368,
     309,  2332,  2368,  2332,   223,     9,    13,    72,   122,   174,
     305,  1322,   217,  2368,  2332,   830,   930,  2129,  2404,  2414,
    2450,  2460,  2496,  -356,   661,  1440,  2332,  -356,   267,  -356,
    -356,  -356,   154,   306,   661,  -356,  2057,  2057,  -356,  2057,
    2057,  2057,  2057,  2057,  2057,  2057,  2057,  2057,  2057,  2057,
    2057,  2057,  2057,  -356,  -356,  -356,  -356,  -356,  -356,  -356,
    -356,  1760,  -356,   246,  -356,  1940,  1841,  1841,  -356,  1841,
    2120,  -356,  2057,  2057,  -356,  -356,  1940,  1823,  -356,   185,
    2187,   167,  2296,  -356,   194,  1428,  2057,   170,  2057,  -356,
    2057,  -356,  2057,  -356,  -356,  -356,  -356,  1449,  -356,  -356,
      20,   188,   310,  2368,  2332,  -356,  1659,  1958,  1514,  -356,
    1534,  -356,  2021,  1554,  -356,   324,   326,   355,   292,   237,
    -356,   339,   354,   359,   132,   222,  -356,   329,   362,    29,
     314,   295,   377,   381,   328,  -356,  -356,   350,  2057,   393,
     403,   378,   407,  1206,  -356,   347,  1322,  2138,  -356,   154,
     306,   306,  -356,  -356,  -356,  2542,  2576,  2368,   373,  2610,
    2638,  2610,  2638,   315,   392,   315,   392,   315,   392,   315,
     392,   315,   392,   315,   392,   315,   392,   315,   392,   315,
     392,   315,   392,   304,   313,   304,   313,  -356,  2368,  2332,
    -356,  2057,  -356,  2368,  -356,  2332,   262,   281,   316,   375,
    2368,   304,   313,   304,   313,  -356,  -356,  2368,  2332,  -356,
    2039,  -356,  -356,  -356,  -356,  -356,  2368,  2332,  2368,  2332,
    2368,  2332,  -356,  -356,   417,  -356,  -356,    33,   419,  2368,
    2332,  -356,  -356,  1619,  -356,  -356,  -356,  1639,  -356,  -356,
    -356,  -356,   421,  -356,   -14,  -356,  -356,   132,  2057,   425,
     203,   138,   365,   387,   367,  -356,   342,   430,   694,  -356,
    -356,   295,    82,  2057,  2368,  2332,   270,  -356,  -356,  -356,
     438,   442,  -356,   443,   445,  -356,   402,    43,   390,  2414,
    2506,   306,  2542,  2576,  -356,  -356,  -356,  -356,  2187,  2296,
    -356,  -356,   449,  -356,  -356,  -356,  -356,   411,  2057,  -356,
    1230,   405,   320,   495,  -356,  -356,  2057,  1230,  2057,   453,
     430,   396,  -356,  -356,   295,  -356,   409,  -356,   444,  -356,
     444,  -356,   295,  -356,  -356,   448,  -356,   460,   461,   465,
    1298,  -356,  -356,  -356,  -356,  -356,  1322,   466,    83,  -356,
    -356,   468,   471,   309,  -356,   427,  -356,  -356,  1230,  -356,
     473,   474,   226,   475,  1138,  1044,   431,  1387,  -356,   295,
     430,   694,  -356,   220,    44,    46,   420,  -356,   270,  -356,
    -356,  -356,  -356,   437,   439,  1322,  1322,   483,  -356,  -356,
     237,  -356,   441,  -356,  -356,  -356,   487,  1138,  -356,  -356,
     488,  1138,  -356,   489,  1138,   694,   295,   452,  -356,  -356,
     363,   116,   102,   469,  1298,  -356,  -356,   463,   490,  1322,
     209,  -356,  -356,  -356,  1138,  -356,  1138,  -356,   457,   694,
    -356,  -356,   470,   523,   477,   295,  2057,   494,  -356,  -356,
     497,  1230,  -356,  -356,  -356,  -356,   508,   498,   518,   430,
     694,  2332,  -356,  -356,  -356,  -356,   430,   499,   295,   513,
     295,   430,   694,  -356,   694,   295,   514,   515,   694,  -356,
    -356,   517,  -356
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -356,  -356,     0,  -356,   -89,   542,  -356,   541,    -2,  -356,
      38,  -356,    30,  -356,    95,  -309,  -205,   -62,  -340,  -356,
     182,  -356,  -356,  -356,   183,   -16,  -356,   426,  -356,   554,
    -356,   650,   428,  -356,  -356,  -356,   123,   401,   -51,     8,
    -356,   -41,  -356,  -356,  -356,   129,  -355,   391,   168,  -316,
     100,  -356,  -356,   389,   268,  -226,  -195,  -356,   254,   151,
     117,  -356,  -356,  -356,  -356,    55,   870,  -356,    22
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -299
static const yytype_int16 yytable[] =
{
      28,    90,    52,    57,   336,    59,    65,    65,    75,    77,
      79,    82,   163,   163,   387,    79,   176,    73,    93,   230,
      93,   176,    91,   231,   303,   215,   205,   209,   394,    93,
      70,    74,   436,   377,   378,   379,   178,   303,     3,     4,
      85,     5,     6,   179,   335,   180,     7,   416,    55,   181,
       9,   160,   160,    65,    10,    11,    88,   210,    87,   479,
     105,    53,   481,   165,    54,    65,   428,   183,    65,   105,
      65,   183,    65,   462,   206,   177,    62,    70,   188,   107,
     190,   439,   193,   212,   164,   213,    62,   456,    92,    86,
     304,   225,   146,   212,   206,   213,    21,    56,    23,    24,
      25,   477,   182,   372,   147,   163,   163,    89,   163,   206,
     214,   482,   224,   417,   235,   237,   209,   239,   241,   243,
     245,   247,   249,   251,   253,   255,   257,   259,   261,   263,
     265,   476,    72,   502,   398,   498,   176,   399,   400,   268,
     361,   176,    64,   273,   160,   160,   504,   160,   280,   338,
     281,   283,    64,   457,   273,   287,   524,   172,   109,   516,
     174,   173,   109,   183,    93,   110,   296,   111,   298,   110,
     300,   111,   157,   505,   511,   183,   276,   277,   290,   278,
     529,   206,   291,   328,   309,    65,   183,   217,   183,   388,
      65,   183,   536,   187,   537,   313,   270,   192,   541,   323,
     317,    62,   329,     3,     4,   290,     5,     6,   389,   292,
     528,     7,   289,    55,    63,     9,   344,   530,   218,    10,
      11,   226,   535,   355,    62,   360,   358,   200,   112,  -226,
     465,   212,  -228,   213,   201,   295,   202,   466,     1,   480,
     471,   493,   474,     3,     4,   495,     5,     6,   497,   370,
     377,     7,   385,     8,   386,     9,   377,   378,   271,    10,
      11,    21,    56,    23,    24,    25,   332,    64,   513,   362,
     514,   109,   398,   290,   406,   478,   400,   364,   110,   397,
     111,   407,    12,   408,  -233,  -233,  -233,    13,   368,    14,
      64,    15,   290,   203,    16,    17,   365,    18,    19,    62,
      20,    21,    22,    23,    24,    25,   212,    26,   213,    62,
     232,   183,   115,   194,   305,   183,   219,   233,   220,   234,
     195,   148,   196,   115,   166,   118,    93,   290,   319,   409,
     320,   366,   441,   167,   151,   166,   118,   229,   322,     1,
     447,   344,     2,   325,     3,     4,   333,     5,     6,   131,
     132,   221,     7,   334,     8,   197,     9,   393,   326,   194,
      10,    11,   423,   200,   222,    64,   195,   377,   196,   427,
     201,   386,   202,   334,   334,    64,    79,   475,   501,   337,
     424,   339,   349,    12,    93,   340,    93,   424,    13,   350,
      14,   351,    15,   341,   343,    16,    17,   347,    18,    19,
     148,    20,    21,    22,    23,    24,    25,   348,    26,   357,
     452,   352,   167,   151,   499,   420,   454,  -224,   353,  -222,
     354,   371,   421,   373,   422,   376,   152,   153,   424,   384,
     390,   391,   392,    58,   468,    60,    66,    66,    76,    78,
      81,    83,   411,   520,   209,    81,   412,   413,    94,   414,
     106,   415,   418,   419,   426,   487,   488,   438,   444,   106,
     323,   440,   442,   448,   449,   450,   532,   468,   534,   451,
     455,   468,   458,   538,   468,   459,   461,   463,   464,   467,
     472,   162,   162,    66,   452,   483,   485,   489,   486,   510,
     491,   492,   494,   496,   468,    66,   468,   184,    66,   429,
      66,   184,    66,   148,    93,   500,   430,   116,   431,   149,
     515,   424,   508,   506,   517,   167,   151,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   152,
     153,   133,   134,   135,   136,   137,   138,   139,   140,   509,
     518,   432,   519,   522,   236,   238,   523,   240,   242,   244,
     246,   248,   250,   252,   254,   256,   258,   260,   262,   264,
     266,   525,   527,   526,   531,   433,   533,   539,   540,   269,
     542,   108,   114,   275,   162,   162,   503,   162,   106,   443,
     282,   284,   285,   446,   275,   288,   142,   311,   321,   490,
     512,   460,   327,   184,   294,   382,   297,   405,   299,   484,
     301,   507,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   142,     0,   142,   310,    66,   184,     0,   184,   142,
      66,   184,     0,     0,     0,     0,     0,     0,     0,   142,
       0,   142,     0,   142,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,   142,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,   228,     0,
     178,     0,     3,     4,     0,     5,     6,   179,   228,   180,
       7,     0,    55,     0,     9,   191,     0,     0,    10,    11,
       0,   227,   144,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,     1,     0,     0,     0,   363,
       3,     4,     0,     5,     6,     0,     0,   144,     7,   144,
       8,     0,     9,     0,   142,   144,    10,    11,   369,     0,
      21,    56,    23,    24,    25,   144,   182,   144,     0,   144,
       0,     0,   144,     0,     0,     0,     0,   142,     0,    12,
       0,   184,     0,   144,    13,   184,    14,   -85,    15,     0,
       0,    16,    17,     0,    18,    19,   383,    20,    21,    22,
      23,    24,    25,     0,    26,     0,     0,     0,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,    81,   142,     0,   142,
     144,   142,     0,   142,   435,   142,   437,   142,     0,   142,
      95,    96,   142,     5,    97,     0,     0,   142,     0,     0,
      55,     0,     9,   144,   142,   142,     0,   142,  -170,     0,
       0,   142,  -170,     0,    53,     0,     0,     0,     0,     0,
     142,  -170,   142,     0,   142,     0,     0,     0,  -226,     0,
       0,     0,     0,   142,  -170,  -170,  -170,  -170,  -170,  -170,
    -170,  -170,  -170,  -170,     0,     0,     0,     0,    98,    99,
     100,   101,   102,   103,     0,   144,     0,   144,     0,   144,
       0,   144,     0,   144,     0,   144,     0,   144,   142,   144,
       0,   144,   145,   144,     0,   144,     0,   144,     0,   144,
       0,   144,     0,   144,   142,   144,   142,     0,   144,     0,
       0,     0,   142,   144,     0,     0,     0,   145,     0,   145,
     144,   144,     0,   144,   521,   145,     0,   144,  -171,     0,
       0,     0,  -171,     0,    54,   145,   144,   145,   144,   145,
     144,  -171,   145,     0,     0,     0,     0,     0,     0,   144,
       0,     0,     0,   145,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,    -4,     1,     0,     0,     0,     0,
       3,     4,     0,     5,     6,     0,     0,     0,     7,     0,
       8,     0,     9,     0,   144,     0,    10,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,     0,   144,     0,     0,     0,     0,     0,   144,    12,
       0,    -4,    -4,    -4,    13,    -4,    14,    -4,    15,     0,
     145,    16,    17,     0,    18,    19,    -4,    20,    21,    22,
      23,    24,    25,     0,    26,     0,     0,     0,   429,     0,
       0,     0,   148,   145,     0,   430,   116,   431,   149,     0,
       0,     0,     0,     0,   167,   151,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   152,   153,
     133,   134,   135,   136,   137,   138,   139,   140,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,   145,     0,   145,
       0,   145,     0,   145,   470,   145,     0,   145,     0,   145,
       0,   145,     0,   145,     0,   145,     0,   145,     0,   145,
       0,   145,     0,   145,     0,   145,     0,     0,   145,     1,
       0,     0,     0,   145,     3,     4,     0,     5,     6,     0,
     145,   145,     7,   145,     8,     0,     9,   145,     0,     0,
      10,    11,     0,     0,     0,     0,   145,     0,   145,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,    12,     0,  -272,     0,  -272,    13,  -272,
      14,     0,    15,     0,     0,    16,    17,     0,    18,    19,
       0,    20,    21,    22,    23,    24,    25,     1,    26,     0,
       0,     0,     3,     4,   145,     5,     6,     0,     0,     0,
       7,     0,     8,     0,     9,     0,     0,     0,    10,    11,
     145,     1,   145,     0,     0,     0,     3,     4,   145,     5,
       6,     0,     0,     0,     7,     0,     8,     0,     9,     0,
       0,    12,    10,    11,     0,  -298,    13,     0,    14,     0,
      15,     0,     0,    16,    17,     0,    18,    19,     0,    20,
      21,    22,    23,    24,    25,    12,    26,     0,     0,  -235,
      13,     0,    14,     0,    15,     0,     0,    16,    17,     0,
      18,    19,     0,    20,    21,    22,    23,    24,    25,     1,
      26,     0,     0,     0,     3,     4,     0,     5,     6,     0,
       0,     0,     7,     0,     8,     0,     9,     0,     0,     0,
      10,    11,     0,     1,     0,     0,     0,     0,     3,     4,
       0,     5,     6,     0,     0,     0,     7,     0,     8,     0,
       9,     0,     0,    12,    10,    11,     0,  -295,    13,     0,
      14,     0,    15,     0,     0,    16,    17,     0,    18,    19,
       0,    20,    21,    22,    23,    24,    25,    12,    26,     0,
       0,     0,    13,     0,    14,     0,    15,     0,     0,    16,
      17,     0,    18,    19,     0,    20,    21,    22,    23,    24,
      25,   429,    26,     0,     0,   148,     0,     0,   430,   116,
     431,   149,     0,     0,     0,     0,     0,   167,   151,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   152,   153,   133,   134,   135,   136,   137,   138,   139,
     140,     0,   178,   432,     3,     4,     0,     5,     6,   179,
       0,   180,     7,     0,    55,   293,     9,     0,   -49,     0,
      10,    11,   -49,   178,   146,     3,     4,   473,     5,     6,
     179,   -49,   180,     7,     0,    55,   302,     9,     0,     0,
       0,    10,    11,     0,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    56,    23,    24,    25,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    56,    23,    24,    25,   178,   182,
       3,     4,     0,     5,     6,   179,     0,   180,     7,     0,
      55,   314,     9,     0,     0,     0,    10,    11,   178,     0,
       3,     4,     0,     5,     6,   179,     0,   180,     7,     0,
      55,     0,     9,   315,     0,     0,    10,    11,   178,     0,
       3,     4,     0,     5,     6,   179,     0,   180,     7,     0,
      55,     0,     9,   318,     0,     0,    10,    11,    21,    56,
      23,    24,    25,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    56,
      23,    24,    25,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    56,
      23,    24,    25,   178,   182,     3,     4,     0,     5,     6,
     179,     0,   180,     7,     0,    55,   374,     9,     0,     0,
       0,    10,    11,   178,     0,     3,     4,     0,     5,     6,
     179,     0,   180,     7,     0,    55,     0,     9,   375,     0,
       0,    10,    11,   178,     0,     3,     4,     0,     5,     6,
     306,     0,   307,     7,     0,    55,     0,     9,     0,     0,
       0,    10,    11,    21,    56,    23,    24,    25,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    56,    23,    24,    25,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    56,    23,    24,    25,    62,   308,
       3,     4,     0,     5,     6,     0,     0,     0,     7,     0,
      55,     0,     9,    71,     0,     0,    10,    11,    95,    96,
       0,     5,    97,     0,     0,     0,     7,     0,    55,     0,
       9,     0,     0,     0,    10,    11,     3,     4,     0,     5,
       6,     0,   267,     0,     7,     0,    55,     0,     9,     0,
       0,     0,    10,    11,     0,     0,     0,     0,    21,    56,
      23,    24,    25,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,    99,   100,   101,
     102,   103,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,    21,    56,    23,    24,    25,     3,
       4,     0,     5,     6,     0,   286,     0,     7,     0,    55,
       0,     9,     0,     0,     0,    10,    11,     3,     4,     0,
       5,     6,     0,   158,     0,     7,     0,     8,     0,     9,
       0,     0,     0,    10,    11,     3,     4,     0,     5,     6,
       0,     0,     0,     7,     0,    55,   175,     9,     0,     0,
       0,    10,    11,     0,     0,     0,    20,    21,    56,    23,
      24,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,   159,    23,    24,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    56,    23,    24,    25,     3,     4,
       0,     5,     6,     0,     0,     0,     7,     0,    55,     0,
       9,   189,     0,     0,    10,    11,     3,     4,     0,     5,
       6,     0,   272,     0,     7,     0,    55,     0,     9,     0,
       0,     0,    10,    11,     3,     4,     0,     5,     6,     0,
       0,     0,     7,     0,    55,   312,     9,     0,     0,     0,
      10,    11,     0,     0,     0,     0,    21,    56,    23,    24,
      25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    56,    23,    24,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    56,    23,    24,    25,     3,     4,     0,
       5,     6,     0,     0,     0,     7,     0,    55,     0,     9,
     316,     0,     0,    10,    11,     3,     4,     0,     5,     6,
       0,   367,     0,     7,     0,     8,     0,     9,     0,     0,
       0,    10,    11,     3,     4,     0,     5,     6,     0,     0,
       0,     7,     0,    55,     0,     9,     0,     0,     0,    10,
      11,     0,     0,     0,     0,    21,    56,    23,    24,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,   159,    23,    24,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,    56,    23,    24,    25,     3,     4,     0,     5,
       6,     0,     0,     0,     7,     0,    55,  -169,     9,     0,
       0,  -169,    10,    11,     3,     4,     0,     5,     6,     0,
    -169,     0,     7,     0,    55,     0,     9,     0,     0,     0,
      10,    11,     0,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,   279,    23,    24,    25,     0,
       0,     0,     0,     0,     0,   115,     0,     0,     0,   116,
       0,     0,    21,   359,    23,    24,    25,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   148,     0,     0,   170,   116,     0,   149,   171,
       0,     0,     0,     0,   167,   151,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   152,   153,
     133,   134,   135,   136,   137,   138,   139,   140,   115,     0,
       0,   168,   116,     0,     0,   169,     0,     0,     0,     0,
     166,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   148,     0,     0,     0,   116,     0,
     149,     0,     0,     0,     0,     0,   150,   151,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     152,   153,   133,   134,   135,   136,   137,   138,   139,   140,
     148,     0,     0,     0,   116,     0,   149,     0,     0,     0,
       0,     0,   167,   151,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   152,   153,   133,   134,
     135,   136,   137,   138,   139,   140,   115,     0,     0,     0,
     116,     0,     0,     0,     0,     0,     0,     0,   166,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,  -168,     0,     0,     0,  -168,     0,     0,     0,
       0,     0,  -164,     0,     0,  -168,  -164,     0,     0,     0,
       0,     0,     0,     0,     0,  -164,     0,     0,  -168,  -168,
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -164,  -164,
    -164,  -164,  -164,  -164,  -164,  -164,  -164,  -164,  -165,     0,
       0,     0,  -165,     0,     0,     0,     0,     0,  -167,     0,
       0,  -165,  -167,     0,     0,     0,     0,     0,     0,     0,
       0,  -167,     0,     0,  -165,  -165,  -165,  -165,  -165,  -165,
    -165,  -165,  -165,  -165,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,  -167,  -166,     0,     0,     0,  -166,     0,
       0,     0,     0,     0,   115,     0,     0,  -166,   116,     0,
       0,     0,     0,     0,     0,     0,     0,   118,     0,     0,
    -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     115,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   167,   151,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     152,   153,   133,   134,   135,   136,   137,   138,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   118,     0,     0,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   167,   151,
       0,     0,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   152,   153
};

static const yytype_int16 yycheck[] =
{
       0,    17,     2,     5,   209,     7,     8,     9,    10,    11,
      12,    13,    53,    54,   330,    17,    67,     9,    20,   108,
      22,    72,     4,   112,     4,    87,    17,    14,   337,    31,
       8,     9,   387,    47,    48,    49,     4,     4,     6,     7,
      16,     9,    10,    11,    15,    13,    14,     4,    16,    17,
      18,    53,    54,    55,    22,    23,    14,    44,    65,    15,
      22,    14,    16,    55,    14,    67,   382,    69,    70,    31,
      72,    73,    74,   428,    65,    67,     4,    55,    70,     0,
      72,   390,    74,    11,    54,    13,     4,     4,    70,    65,
      70,    91,    14,    11,    65,    13,    64,    65,    66,    67,
      68,   441,    70,    70,    14,   146,   147,    65,   149,    65,
      38,    65,    90,    70,   116,   117,    14,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   440,     9,    17,    52,   475,   187,    55,    56,   141,
     229,   192,    70,   145,   146,   147,    44,   149,   150,   211,
     152,   153,    70,    70,   156,   157,   511,    11,     4,   499,
       4,    15,     4,   165,   166,    11,   168,    13,   170,    11,
     172,    13,    44,   482,   490,   177,   146,   147,    11,   149,
     520,    65,    15,    51,   186,   187,   188,    65,   190,    51,
     192,   193,   532,    70,   534,   187,   141,    74,   538,   199,
     192,     4,    70,     6,     7,    11,     9,    10,    70,    15,
     519,    14,   157,    16,    17,    18,   218,   526,    44,    22,
      23,     4,   531,   223,     4,   227,   226,     4,    70,    44,
       4,    11,    44,    13,    11,    65,    13,    11,     1,   444,
     435,   467,   437,     6,     7,   471,     9,    10,   474,   290,
      47,    14,    49,    16,    51,    18,    47,    48,    12,    22,
      23,    64,    65,    66,    67,    68,    44,    70,   494,   271,
     496,     4,    52,    11,     4,    55,    56,    15,    11,   341,
      13,    11,    45,    13,    47,    48,    49,    50,   290,    52,
      70,    54,    11,    70,    57,    58,    15,    60,    61,     4,
      63,    64,    65,    66,    67,    68,    11,    70,    13,     4,
       4,   313,     8,     4,     4,   317,    11,    11,    13,    13,
      11,     8,    13,     8,    20,    21,   328,    11,     4,    59,
       4,    15,   394,    20,    21,    20,    21,    70,    46,     1,
     402,   343,     4,     4,     6,     7,    17,     9,    10,    34,
      35,    46,    14,    11,    16,    46,    18,    15,     4,     4,
      22,    23,   378,     4,    59,    70,    11,    47,    13,    49,
      11,    51,    13,    11,    11,    70,   378,   439,    15,    65,
     380,     4,     4,    45,   386,     4,   388,   387,    50,    11,
      52,    13,    54,    65,    44,    57,    58,     4,    60,    61,
       8,    63,    64,    65,    66,    67,    68,     4,    70,    62,
     410,     4,    20,    21,   476,     4,   416,    44,    11,    44,
      13,     4,    11,     4,    13,     4,    34,    35,   428,     4,
      65,    44,    65,     5,   434,     7,     8,     9,    10,    11,
      12,    13,     4,   505,    14,    17,     4,     4,    20,     4,
      22,    49,    62,     4,    49,   455,   456,     4,    14,    31,
     460,    65,    53,    15,     4,     4,   528,   467,   530,     4,
       4,   471,     4,   535,   474,     4,    49,     4,     4,     4,
      49,    53,    54,    55,   484,    65,    49,     4,    49,   489,
      49,     4,     4,     4,   494,    67,   496,    69,    70,     4,
      72,    73,    74,     8,   506,    53,    11,    12,    13,    14,
      53,   511,    49,    44,    44,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    49,
      17,    46,    65,    49,   116,   117,    49,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    53,    44,    65,    65,    70,    53,    53,    53,   141,
      53,    29,    31,   145,   146,   147,   481,   149,   150,   397,
     152,   153,   156,   400,   156,   157,    32,   186,   197,   460,
     490,   423,   203,   165,   166,   327,   168,   343,   170,   448,
     172,   484,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    59,   186,   187,   188,    -1,   190,    65,
     192,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,    -1,    -1,   104,    -1,
       4,    -1,     6,     7,    -1,     9,    10,    11,   114,    13,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,
      -1,    20,    32,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     1,    -1,    -1,    -1,   271,
       6,     7,    -1,     9,    10,    -1,    -1,    57,    14,    59,
      16,    -1,    18,    -1,   160,    65,    22,    23,   290,    -1,
      64,    65,    66,    67,    68,    75,    70,    77,    -1,    79,
      -1,    -1,    82,    -1,    -1,    -1,    -1,   183,    -1,    45,
      -1,   313,    -1,    93,    50,   317,    52,    53,    54,    -1,
      -1,    57,    58,    -1,    60,    61,   328,    63,    64,    65,
      66,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
      -1,   237,    -1,   239,    -1,   241,    -1,   243,    -1,   245,
      -1,   247,    -1,   249,    -1,   251,   378,   253,    -1,   255,
     160,   257,    -1,   259,   386,   261,   388,   263,    -1,   265,
       6,     7,   268,     9,    10,    -1,    -1,   273,    -1,    -1,
      16,    -1,    18,   183,   280,   281,    -1,   283,     8,    -1,
      -1,   287,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
     296,    21,   298,    -1,   300,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,   309,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    69,    -1,   235,    -1,   237,    -1,   239,
      -1,   241,    -1,   243,    -1,   245,    -1,   247,   344,   249,
      -1,   251,    32,   253,    -1,   255,    -1,   257,    -1,   259,
      -1,   261,    -1,   263,   360,   265,   362,    -1,   268,    -1,
      -1,    -1,   368,   273,    -1,    -1,    -1,    57,    -1,    59,
     280,   281,    -1,   283,   506,    65,    -1,   287,     8,    -1,
      -1,    -1,    12,    -1,    14,    75,   296,    77,   298,    79,
     300,    21,    82,    -1,    -1,    -1,    -1,    -1,    -1,   309,
      -1,    -1,    -1,    93,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     0,     1,    -1,    -1,    -1,    -1,
       6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,
      16,    -1,    18,    -1,   344,    -1,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     360,    -1,   362,    -1,    -1,    -1,    -1,    -1,   368,    45,
      -1,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
     160,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    -1,    -1,    -1,     4,    -1,
      -1,    -1,     8,   183,    -1,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,    -1,   237,    -1,   239,
      -1,   241,    -1,   243,    70,   245,    -1,   247,    -1,   249,
      -1,   251,    -1,   253,    -1,   255,    -1,   257,    -1,   259,
      -1,   261,    -1,   263,    -1,   265,    -1,    -1,   268,     1,
      -1,    -1,    -1,   273,     6,     7,    -1,     9,    10,    -1,
     280,   281,    14,   283,    16,    -1,    18,   287,    -1,    -1,
      22,    23,    -1,    -1,    -1,    -1,   296,    -1,   298,    -1,
     300,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,
      -1,    -1,    -1,    45,    -1,    47,    -1,    49,    50,    51,
      52,    -1,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,     1,    70,    -1,
      -1,    -1,     6,     7,   344,     9,    10,    -1,    -1,    -1,
      14,    -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,
     360,     1,   362,    -1,    -1,    -1,     6,     7,   368,     9,
      10,    -1,    -1,    -1,    14,    -1,    16,    -1,    18,    -1,
      -1,    45,    22,    23,    -1,    49,    50,    -1,    52,    -1,
      54,    -1,    -1,    57,    58,    -1,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    45,    70,    -1,    -1,    49,
      50,    -1,    52,    -1,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,     1,
      70,    -1,    -1,    -1,     6,     7,    -1,     9,    10,    -1,
      -1,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,
      22,    23,    -1,     1,    -1,    -1,    -1,    -1,     6,     7,
      -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,
      18,    -1,    -1,    45,    22,    23,    -1,    49,    50,    -1,
      52,    -1,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    45,    70,    -1,
      -1,    -1,    50,    -1,    52,    -1,    54,    -1,    -1,    57,
      58,    -1,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,     4,    70,    -1,    -1,     8,    -1,    -1,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,     4,    46,     6,     7,    -1,     9,    10,    11,
      -1,    13,    14,    -1,    16,    17,    18,    -1,     8,    -1,
      22,    23,    12,     4,    14,     6,     7,    70,     9,    10,
      11,    21,    13,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    68,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,     4,    70,
       6,     7,    -1,     9,    10,    11,    -1,    13,    14,    -1,
      16,    17,    18,    -1,    -1,    -1,    22,    23,     4,    -1,
       6,     7,    -1,     9,    10,    11,    -1,    13,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    22,    23,     4,    -1,
       6,     7,    -1,     9,    10,    11,    -1,    13,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    22,    23,    64,    65,
      66,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,     4,    70,     6,     7,    -1,     9,    10,
      11,    -1,    13,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    22,    23,     4,    -1,     6,     7,    -1,     9,    10,
      11,    -1,    13,    14,    -1,    16,    -1,    18,    19,    -1,
      -1,    22,    23,     4,    -1,     6,     7,    -1,     9,    10,
      11,    -1,    13,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,    64,    65,    66,    67,    68,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,     4,    70,
       6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    22,    23,     6,     7,
      -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,     6,     7,    -1,     9,
      10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,
      -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    67,    68,     6,
       7,    -1,     9,    10,    -1,    12,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,     6,     7,    -1,
       9,    10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    63,    64,    65,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,     6,     7,
      -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,
      18,    19,    -1,    -1,    22,    23,     6,     7,    -1,     9,
      10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,
      -1,    -1,    22,    23,     6,     7,    -1,     9,    10,    -1,
      -1,    -1,    14,    -1,    16,    17,    18,    -1,    -1,    -1,
      22,    23,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    68,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,    18,
      19,    -1,    -1,    22,    23,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,     6,     7,    -1,     9,    10,    -1,    -1,
      -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,    68,     6,     7,    -1,     9,
      10,    -1,    -1,    -1,    14,    -1,    16,     8,    18,    -1,
      -1,    12,    22,    23,     6,     7,    -1,     9,    10,    -1,
      21,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,
      22,    23,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    -1,    -1,    12,
      -1,    -1,    64,    65,    66,    67,    68,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,     8,    -1,    -1,    11,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,     8,    -1,
      -1,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     8,    -1,    -1,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
       8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    -1,    21,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      -1,    21,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    -1,    21,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,     6,     7,     9,    10,    14,    16,    18,
      22,    23,    45,    50,    52,    54,    57,    58,    60,    61,
      63,    64,    65,    66,    67,    68,    70,    78,    79,    80,
      82,    83,    85,    86,    87,    88,    90,    96,   103,   105,
     109,   111,   112,   118,   119,   120,   121,   128,   134,   138,
     141,   142,    79,    14,    14,    16,    65,    85,   109,    85,
     109,   110,     4,    17,    70,    85,   109,   113,   115,   116,
     145,    19,   113,   116,   145,    85,   109,    85,   109,    85,
     102,   109,    85,   109,   129,    16,    65,    65,    14,    65,
     102,     4,    70,    85,   109,     6,     7,    10,    64,    65,
      66,    67,    68,    69,    84,    87,   109,     0,    82,     4,
      11,    13,    70,    81,    84,     8,    12,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,   106,   107,   108,   143,    14,    14,     8,    14,
      20,    21,    34,    35,   106,   108,   143,    44,    12,    65,
      85,    89,   109,   118,    89,   116,    20,    20,    11,    15,
      11,    15,    11,    15,     4,    17,   115,   116,     4,    11,
      13,    17,    70,    85,   109,   114,   117,   113,   116,    19,
     116,    19,   113,   116,     4,    11,    13,    46,   124,   125,
       4,    11,    13,    70,   130,    17,    65,    91,    93,    14,
      44,    92,    11,    13,    38,    94,   145,    65,    44,    11,
      13,    46,    59,   140,   145,    79,     4,    20,   106,    70,
      81,    81,     4,    11,    13,    85,   109,    85,   109,    85,
     109,    85,   109,    85,   109,    85,   109,    85,   109,    85,
     109,    85,   109,    85,   109,    85,   109,    85,   109,    85,
     109,    85,   109,    85,   109,    85,   109,    12,    85,   109,
     142,    12,    12,    85,   104,   109,    89,    89,    89,    65,
      85,    85,   109,    85,   109,   104,    12,    85,   109,   142,
      11,    15,    15,    17,   109,    65,    85,   109,    85,   109,
      85,   109,    17,     4,    70,     4,    11,    13,    70,    85,
     109,   114,    17,   116,    17,    19,    19,   116,    19,     4,
       4,   124,    46,    79,   122,     4,     4,   130,    51,    70,
     131,   144,    44,    17,    11,    15,    93,    65,    94,     4,
       4,    65,    97,    44,    85,   109,   135,     4,     4,     4,
      11,    13,     4,    11,    13,    79,   139,    62,    79,    65,
      85,    81,    85,   109,    15,    15,    15,    12,    85,   109,
     118,     4,    70,     4,    17,    19,     4,    47,    48,    49,
     126,   127,   131,   109,     4,    49,    51,   126,    51,    70,
      65,    44,    65,    15,    92,    79,    95,    94,    52,    55,
      56,    94,    98,    99,   100,   135,     4,    11,    13,    59,
     136,     4,     4,     4,     4,    49,     4,    70,    62,     4,
       4,    11,    13,   102,    79,   123,    49,    49,   126,     4,
      11,    13,    46,    70,   133,   109,   123,   109,     4,    92,
      65,    94,    53,    97,    14,   101,   101,    94,    15,     4,
       4,     4,    79,   137,    79,     4,     4,    70,     4,     4,
     125,    49,   123,     4,     4,     4,    11,     4,    79,   132,
      70,   133,    49,    70,   133,    94,    92,    95,    55,    15,
      93,    16,    65,    65,   136,    49,    49,    79,    79,     4,
     122,    49,     4,   132,     4,   132,     4,   132,    95,    94,
      53,    15,    17,    91,    44,    92,    44,   137,    49,    49,
      79,   126,   127,   132,   132,    53,    95,    44,    17,    65,
      94,   109,    49,    49,   123,    53,    65,    44,    92,    95,
      92,    65,    94,    53,    94,    92,    95,    95,    94,    53,
      53,    95,    53
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
  yylloc.first_column = yylloc.last_column = 1;
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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_class_dec); }
    break;

  case 21:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 22:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_assign_exp); }
    break;

  case 23:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_if_exp); }
    break;

  case 24:

    { (yyval.t_exp) = new ast::CommentExp((yyloc), new std::string("!! FIXME : Select Control Sequence !!")); }
    break;

  case 25:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_for_exp); }
    break;

  case 26:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_while_exp); }
    break;

  case 27:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_try_exp); }
    break;

  case 28:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 29:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 30:

    { (yyval.t_exp) = new ast::BreakExp((yyloc)); }
    break;

  case 31:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_return_exp); }
    break;

  case 32:

    { (yyval.t_exp) = new ast::CommentExp((yyloc), (yyvsp[(1) - (1)].comment)); }
    break;

  case 33:

    {
  (yyval.t_exp) = new ast::CommentExp((yyloc), new std::string("@@ ERROR RECOVERY @@"));
  StopOnError();
  }
    break;

  case 34:

    {
						  (yyvsp[(1) - (2)].t_call_exp)->args_get().push_back((yyvsp[(2) - (2)].t_string_exp));
						  (yyval.t_call_exp) = (yyvsp[(1) - (2)].t_call_exp);
						}
    break;

  case 35:

    {
						  ast::exps_t *tmp = new ast::exps_t;
						  tmp->push_front((yyvsp[(2) - (2)].t_string_exp));
						  (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (2)]), *new symbol::Symbol(*(yyvsp[(1) - (2)].str))), *tmp);
						}
    break;

  case 36:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].str)); }
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

    {
						  std::stringstream tmp;
						  tmp << (yyvsp[(1) - (1)].number);
						  (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
						}
    break;

  case 40:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].str)); }
    break;

  case 41:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("$")); }
    break;

  case 42:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("%t")); }
    break;

  case 43:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("%f")); }
    break;

  case 44:

    {
						  std::stringstream tmp;
						  tmp << (yyvsp[(1) - (3)].t_string_exp)->value_get() << "." << *(yyvsp[(3) - (3)].str);
						  (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
						}
    break;

  case 45:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("!! FIXME : implicitCallable implicitCallable DOT functionCall !!")); }
    break;

  case 46:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("!! FIXME : implicitCallable simpleFunctionCall !!")); }
    break;

  case 47:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *new std::string("!! FIXME : implicitCallable implicitCallable rightOperand !!")); }
    break;

  case 48:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].path)); }
    break;

  case 49:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 50:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 51:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_call_exp); }
    break;

  case 52:

    { (yyval.t_exp) = (yyvsp[(2) - (3)].t_exp); }
    break;

  case 53:

    { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 54:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (4)]), *new symbol::Symbol("%t")), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 55:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[(1) - (4)]), *new symbol::Symbol("%f")), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 56:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *(yyvsp[(1) - (4)].t_exp), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 57:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *(yyvsp[(1) - (4)].t_call_exp), *(yyvsp[(3) - (4)].t_list_exp)); }
    break;

  case 58:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *(yyvsp[(1) - (4)].t_call_exp), *(yyvsp[(3) - (4)].t_list_exp)); }
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
				  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
				}
    break;

  case 61:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				  (yyval.t_list_exp)->push_front(new ast::ColonVar((yyloc)));
				}
    break;

  case 62:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
				  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_assign_exp));
				}
    break;

  case 63:

    {
				  (yyval.t_list_exp) = new ast::exps_t;
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
				  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
				  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 66:

    {
				  (yyvsp[(1) - (3)].t_list_exp)->push_back(new ast::ColonVar((yyloc)));
			          (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 67:

    {
				  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_assign_exp));
				  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
				}
    break;

  case 68:

    {
				  (yyval.t_list_exp) = (yyvsp[(1) - (2)].t_list_exp);
				}
    break;

  case 69:

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

  case 70:

    {
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(6) - (10)].str)),
							    *new ast::ArrayListVar((yylsp[(7) - (10)]), *(yyvsp[(7) - (10)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(3) - (10)]) ,*(yyvsp[(3) - (10)].t_list_var)),
							    *(yyvsp[(9) - (10)].t_seq_exp));
				}
    break;

  case 71:

    {
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(5) - (9)].str)),
							    *new ast::ArrayListVar((yylsp[(6) - (9)]), *(yyvsp[(6) - (9)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(2) - (9)]), *new ast::vars_t),
							    *(yyvsp[(8) - (9)].t_seq_exp));
				}
    break;

  case 72:

    {
				  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(2) - (6)].str)),
							    *new ast::ArrayListVar((yylsp[(3) - (6)]), *(yyvsp[(3) - (6)].t_list_var)),
							    *new ast::ArrayListVar((yyloc), *new ast::vars_t),
							    *(yyvsp[(5) - (6)].t_seq_exp));
				}
    break;

  case 73:

    { (yyval.t_list_var) = (yyvsp[(1) - (1)].t_list_var); }
    break;

  case 74:

    { (yyval.t_list_var) = (yyvsp[(2) - (3)].t_list_var); }
    break;

  case 75:

    { (yyval.t_list_var) = new ast::vars_t;	}
    break;

  case 76:

    { (yyval.t_list_var) = new ast::vars_t;	}
    break;

  case 77:

    {
				  (yyvsp[(1) - (3)].t_list_var)->push_back(new ast::SimpleVar((yylsp[(3) - (3)]), *new symbol::Symbol(*(yyvsp[(3) - (3)].str))));
				  (yyval.t_list_var) = (yyvsp[(1) - (3)].t_list_var);
				}
    break;

  case 78:

    {
				  (yyval.t_list_var) = new ast::vars_t;
				  (yyval.t_list_var)->push_front(new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(1) - (1)].str))));
				}
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

    { /* !! Do Nothing !! */ }
    break;

  case 84:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 85:

    {
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty function body")));
				  #endif
				  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
				}
    break;

  case 86:

    {
				  (yyval.t_class_dec) = new ast::ClassDec((yyloc), new symbol::Symbol(*(yyvsp[(2) - (7)].str)), new symbol::Symbol(*(yyvsp[(4) - (7)].str)), *(yyvsp[(6) - (7)].t_list_slots));
				}
    break;

  case 87:

    {
				  (yyval.t_class_dec) = new ast::ClassDec((yyloc), new symbol::Symbol(*(yyvsp[(2) - (5)].str)), NULL, *(yyvsp[(4) - (5)].t_list_slots));
				}
    break;

  case 88:

    {
				  (yyval.t_list_slots) = (yyvsp[(1) - (3)].t_list_slots); (yyval.t_list_slots)->push_back((yyvsp[(2) - (3)].t_slot_dec));
				}
    break;

  case 89:

    { (yyval.t_list_slots) = (yyvsp[(1) - (2)].t_list_slots); }
    break;

  case 90:

    { (yyval.t_list_slots) = new ast::slots_t; }
    break;

  case 91:

    { (yyval.t_slot_dec) = (yyvsp[(1) - (1)].t_slot_dec); }
    break;

  case 92:

    { (yyval.t_slot_dec) = (yyvsp[(1) - (1)].t_slot_dec); }
    break;

  case 93:

    {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar((yylsp[(3) - (9)]), *new symbol::Symbol(*(yyvsp[(3) - (9)].str))));
				  (yyval.t_slot_dec) = new ast::MethodDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(5) - (9)].str)),
							    *new ast::ArrayListVar((yylsp[(2) - (9)]), *(yyvsp[(2) - (9)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(6) - (9)]), *(yyvsp[(6) - (9)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(3) - (9)]), *tmp),
							    *(yyvsp[(8) - (9)].t_seq_exp));
				}
    break;

  case 94:

    {
				  (yyval.t_slot_dec) = new ast::MethodDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(7) - (11)].str)),
							    *new ast::ArrayListVar((yylsp[(2) - (11)]), *(yyvsp[(2) - (11)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(8) - (11)]), *(yyvsp[(8) - (11)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(4) - (11)]) ,*(yyvsp[(4) - (11)].t_list_var)),
							    *(yyvsp[(10) - (11)].t_seq_exp));
				}
    break;

  case 95:

    {
				  (yyval.t_slot_dec) = new ast::MethodDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(6) - (10)].str)),
							    *new ast::ArrayListVar((yylsp[(2) - (10)]), *(yyvsp[(2) - (10)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(7) - (10)]), *(yyvsp[(7) - (10)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(3) - (10)]), *new ast::vars_t),
							    *(yyvsp[(9) - (10)].t_seq_exp));
				}
    break;

  case 96:

    {
				  (yyval.t_slot_dec) = new ast::MethodDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(3) - (7)].str)),
							    *new ast::ArrayListVar((yylsp[(2) - (7)]), *(yyvsp[(2) - (7)].t_list_var)),
							    *new ast::ArrayListVar((yylsp[(4) - (7)]), *(yyvsp[(4) - (7)].t_list_var)),
							    *new ast::ArrayListVar((yyloc), *new ast::vars_t),
							    *(yyvsp[(6) - (7)].t_seq_exp));
				}
    break;

  case 97:

    {
				  (yyval.t_slot_dec) = new ast::PropertyDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(3) - (5)].str)),
							    *new ast::ArrayListVar((yylsp[(2) - (5)]), *(yyvsp[(2) - (5)].t_list_var)),
							    (yyvsp[(5) - (5)].t_exp));
				}
    break;

  case 98:

    {
				  (yyval.t_slot_dec) = new ast::PropertyDec((yyloc),
							    *new symbol::Symbol(*(yyvsp[(3) - (3)].str)),
							    *new ast::ArrayListVar((yylsp[(2) - (3)]), *(yyvsp[(2) - (3)].t_list_var)),
							    NULL);
				}
    break;

  case 99:

    { (yyval.t_list_var) = (yyvsp[(2) - (3)].t_list_var); }
    break;

  case 100:

    { (yyval.t_list_var) = new ast::vars_t; }
    break;

  case 101:

    { (yyval.t_list_var) = new ast::vars_t; }
    break;

  case 102:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 103:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 104:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), (yyvsp[(2) - (3)].t_op_exp_oper), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 105:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), (yyvsp[(2) - (3)].t_op_exp_oper), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 106:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 107:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 108:

    { (yyval.t_exp) = new ast::ColonVar((yyloc)); }
    break;

  case 109:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_op_exp)->oper_get(), (yyvsp[(2) - (2)].t_op_exp)->right_get()); }
    break;

  case 110:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_op_exp)->oper_get(), (yyvsp[(2) - (2)].t_op_exp)->right_get()); }
    break;

  case 111:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 112:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 113:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 114:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 115:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 116:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 117:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 118:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::power, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 119:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 120:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 121:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 122:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[(1) - (3)].t_exp), ast::OpExp::dotpower, *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 123:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_Conjugate_); }
    break;

  case 124:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_NonConjugate_); }
    break;

  case 125:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_Conjugate_); }
    break;

  case 126:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[(1) - (2)].t_exp), ast::TransposeExp::_NonConjugate_); }
    break;

  case 127:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::plus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 128:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::plus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 129:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 130:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::minus, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 131:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::times, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 132:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::times, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 133:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 134:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 135:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 136:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 137:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::divide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 138:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::divide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 139:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 140:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 141:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krondivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 142:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::krondivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 143:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::controldivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 144:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::controldivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 145:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 146:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 147:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 148:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 149:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 150:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 151:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 152:

    { (yyval.t_exp) = (yyvsp[(2) - (2)].t_exp); }
    break;

  case 153:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 154:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 155:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::string("Should not stay in that state")), *new ast::DoubleExp((yyloc), 1.0), *(yyvsp[(1) - (1)].t_exp)); }
    break;

  case 156:

    { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[(2) - (2)].t_exp)); }
    break;

  case 157:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(3) - (3)].str)))); }
    break;

  case 158:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 159:

    { (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_implicit_list)->step_get(), (yyvsp[(2) - (2)].t_implicit_list)->end_get()); }
    break;

  case 160:

    { (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[(1) - (2)].t_exp), (yyvsp[(2) - (2)].t_implicit_list)->step_get(), (yyvsp[(2) - (2)].t_implicit_list)->end_get()); }
    break;

  case 161:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_matrix_exp); }
    break;

  case 162:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_cell_exp); }
    break;

  case 163:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 164:

    { (yyval.t_exp) = new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(1) - (1)].str))); }
    break;

  case 165:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[(1) - (1)].number)); }
    break;

  case 166:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[(1) - (1)].number)); }
    break;

  case 167:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[(1) - (1)].number)); }
    break;

  case 168:

    { (yyval.t_exp) = new ast::StringExp((yyloc), *(yyvsp[(1) - (1)].str)); }
    break;

  case 169:

    { (yyval.t_exp) = new ast::DollarVar((yyloc)); }
    break;

  case 170:

    { (yyval.t_exp) = new ast::BoolExp((yyloc), true); }
    break;

  case 171:

    { (yyval.t_exp) = new ast::BoolExp((yyloc), false); }
    break;

  case 172:

    { (yyval.t_exp) = (yyvsp[(2) - (3)].t_exp); }
    break;

  case 173:

    { (yyval.t_exp) = (yyvsp[(2) - (3)].t_arraylist_exp); }
    break;

  case 174:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_op_exp); }
    break;

  case 175:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_arraylist_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 176:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_arraylist_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 177:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 178:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 179:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
					}
    break;

  case 180:

    {
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back((yyvsp[(1) - (3)].t_exp));
					  tmp->push_back((yyvsp[(3) - (3)].t_exp));
					  (yyval.t_arraylist_exp) = new ast::ArrayListExp((yyloc), *tmp);
}
    break;

  case 181:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(2) - (3)].t_list_mline)); }
    break;

  case 182:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(3) - (4)].t_list_mline)); }
    break;

  case 183:

    {
								  (yyvsp[(2) - (4)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(2) - (4)].t_list_mline));
								}
    break;

  case 184:

    {
								  (yyvsp[(3) - (5)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(4) - (5)]), *(yyvsp[(4) - (5)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[(3) - (5)].t_list_mline));
								}
    break;

  case 185:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(2) - (3)]), *(yyvsp[(2) - (3)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
								}
    break;

  case 186:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
}
    break;

  case 187:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *new std::list<ast::MatrixLineExp *>); }
    break;

  case 188:

    { (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(2) - (3)].t_list_mline)); }
    break;

  case 189:

    { (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(3) - (4)].t_list_mline)); }
    break;

  case 190:

    {
								  (yyvsp[(2) - (4)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(2) - (4)].t_list_mline));
								}
    break;

  case 191:

    {
								  (yyvsp[(3) - (5)].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[(4) - (5)]), *(yyvsp[(4) - (5)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[(3) - (5)].t_list_mline));
								}
    break;

  case 192:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(2) - (3)]), *(yyvsp[(2) - (3)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);
								}
    break;

  case 193:

    {
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp((yylsp[(3) - (4)]), *(yyvsp[(3) - (4)].t_list_exp)));
								  (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);
								}
    break;

  case 194:

    { (yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *new std::list<ast::MatrixLineExp *>); }
    break;

  case 195:

    {
					  (yyvsp[(1) - (2)].t_list_mline)->push_back((yyvsp[(2) - (2)].t_matrixline_exp));
					  (yyval.t_list_mline) = (yyvsp[(1) - (2)].t_list_mline);
					}
    break;

  case 196:

    {
					  (yyval.t_list_mline) = new std::list<ast::MatrixLineExp *>;
					  (yyval.t_list_mline)->push_front((yyvsp[(1) - (1)].t_matrixline_exp));
					}
    break;

  case 197:

    { /* !! Do Nothing !! */ }
    break;

  case 198:

    { /* !! Do Nothing !! */ }
    break;

  case 199:

    { /* !! Do Nothing !! */ }
    break;

  case 200:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (2)].t_list_exp)); }
    break;

  case 201:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (3)].t_list_exp)); }
    break;

  case 202:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (4)].t_list_exp)); }
    break;

  case 203:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (3)].t_list_exp)); }
    break;

  case 204:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (4)].t_list_exp)); }
    break;

  case 205:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[(1) - (5)].t_list_exp)); }
    break;

  case 206:

    {
								  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
								}
    break;

  case 207:

    {
								  (yyvsp[(1) - (3)].t_list_exp)->push_back((yyvsp[(3) - (3)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (3)].t_list_exp);
								}
    break;

  case 208:

    {
								  (yyvsp[(1) - (2)].t_list_exp)->push_back((yyvsp[(2) - (2)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (2)].t_list_exp);
								}
    break;

  case 209:

    {
								  (yyvsp[(1) - (2)].t_list_exp)->push_back((yyvsp[(2) - (2)].t_exp));
								  (yyval.t_list_exp) = (yyvsp[(1) - (2)].t_list_exp);
								}
    break;

  case 210:

    {
								  (yyval.t_list_exp) = new ast::exps_t;
								  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
								}
    break;

  case 211:

    {
								  (yyval.t_list_exp) = new ast::exps_t;
								  (yyval.t_list_exp)->push_front((yyvsp[(1) - (1)].t_exp));
								}
    break;

  case 212:

    { /* !! Do Nothing !! */ }
    break;

  case 213:

    { /* !! Do Nothing !! */ }
    break;

  case 214:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 215:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 216:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 217:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 218:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::ColonVar((yylsp[(3) - (3)]))); }
    break;

  case 219:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::ColonVar((yylsp[(3) - (3)]))); }
    break;

  case 220:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_return_exp)); }
    break;

  case 221:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_return_exp)); }
    break;

  case 222:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(3) - (3)].str)))); }
    break;

  case 223:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 224:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 225:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[(1) - (3)].t_exp), *(yyvsp[(3) - (3)].t_exp)); }
    break;

  case 226:

    { (yyval.t_exp) = new ast::SimpleVar((yyloc), *new symbol::Symbol(*(yyvsp[(1) - (1)].str))); }
    break;

  case 227:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_assignlist_exp); }
    break;

  case 228:

    { (yyval.t_assignlist_exp) = new ast::AssignListExp((yyloc), *(yyvsp[(2) - (3)].t_list_exp)); }
    break;

  case 229:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[(2) - (5)].t_exp), *(yyvsp[(4) - (5)].t_seq_exp)); }
    break;

  case 230:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[(2) - (7)].t_exp), *(yyvsp[(4) - (7)].t_seq_exp), *(yyvsp[(6) - (7)].t_seq_exp)); }
    break;

  case 231:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[(2) - (6)].t_exp), *(yyvsp[(4) - (6)].t_seq_exp), *(yyvsp[(5) - (6)].t_seq_exp)); }
    break;

  case 232:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 233:

    {
							  ast::exps_t *tmp = new ast::exps_t;
							  #ifdef BUILD_DEBUG_AST
							  tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty then body")));
							  #endif
							  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
							}
    break;

  case 234:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 235:

    {
							  ast::exps_t *tmp = new ast::exps_t;
							  #ifdef BUILD_DEBUG_AST
							  tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty else body")));
							  #endif
							  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
							}
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

    { /* !! Do Nothing !! */ }
    break;

  case 243:

    { /* !! Do Nothing !! */ }
    break;

  case 244:

    { /* !! Do Nothing !! */ }
    break;

  case 245:

    { /* !! Do Nothing !! */ }
    break;

  case 246:

    { /* !! Do Nothing !! */ }
    break;

  case 247:

    { /* !! Do Nothing !! */ }
    break;

  case 248:

    { /* !! Do Nothing !! */ }
    break;

  case 249:

    { /* !! Do Nothing !! */ }
    break;

  case 250:

    { /* !! Do Nothing !! */ }
    break;

  case 251:

    { /* !! Do Nothing !! */ }
    break;

  case 252:

    { /* !! Do Nothing !! */ }
    break;

  case 253:

    {
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp((yyloc), *(yyvsp[(2) - (4)].t_exp), *(yyvsp[(4) - (4)].t_seq_exp)) );
										(yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
									}
    break;

  case 254:

    {
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp((yyloc), *(yyvsp[(2) - (6)].t_exp), *(yyvsp[(4) - (6)].t_seq_exp), *(yyvsp[(6) - (6)].t_seq_exp)) );
										(yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);

									}
    break;

  case 255:

    {
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp((yyloc), *(yyvsp[(2) - (5)].t_exp), *(yyvsp[(5) - (5)].t_seq_exp)) );
										(yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
									}
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

  case 273:

    { /* !! Do Nothing !! */ }
    break;

  case 274:

    { /* !! Do Nothing !! */ }
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

    { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[(3) - (7)]), *new symbol::Symbol(*(yyvsp[(2) - (7)].str)), *(yyvsp[(4) - (7)].t_exp)), *(yyvsp[(6) - (7)].t_seq_exp)); }
    break;

  case 283:

    { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[(4) - (9)]), *new symbol::Symbol(*(yyvsp[(3) - (9)].str)), *(yyvsp[(5) - (9)].t_exp)), *(yyvsp[(8) - (9)].t_seq_exp)); }
    break;

  case 284:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 285:

    { (yyval.t_exp) = (yyvsp[(1) - (1)].t_exp); }
    break;

  case 286:

    { /* !! Do Nothing !! */ }
    break;

  case 287:

    { /* !! Do Nothing !! */ }
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

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 295:

    {
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty for body")));
				  #endif
				  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
				}
    break;

  case 296:

    { (yyval.t_while_exp) = new ast::WhileExp((yyloc), *(yyvsp[(2) - (5)].t_exp), *(yyvsp[(4) - (5)].t_seq_exp)); }
    break;

  case 297:

    { (yyval.t_seq_exp) = (yyvsp[(1) - (1)].t_seq_exp); }
    break;

  case 298:

    {
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp((yyloc), new std::string("Empty while body")));
				  #endif
				  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
				}
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

    { /* !! Do Nothing !! */ }
    break;

  case 306:

    { /* !! Do Nothing !! */ }
    break;

  case 307:

    { /* !! Do Nothing !! */ }
    break;

  case 308:

    { /* !! Do Nothing !! */ }
    break;

  case 309:

    { /* !! Do Nothing !! */ }
    break;

  case 310:

    { /* !! Do Nothing !! */ }
    break;

  case 311:

    { /* !! Do Nothing !! */ }
    break;

  case 312:

    { /* !! Do Nothing !! */ }
    break;

  case 313:

    { /* !! Do Nothing !! */ }
    break;

  case 314:

    { /* !! Do Nothing !! */ }
    break;

  case 315:

    { /* !! Do Nothing !! */ }
    break;

  case 316:

    { (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(3) - (7)].t_seq_exp), *(yyvsp[(6) - (7)].t_seq_exp)); }
    break;

  case 317:

    {
								  (yyvsp[(4) - (8)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(2) - (8)]), (yyvsp[(2) - (8)].comment)));
								  (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(4) - (8)].t_seq_exp), *(yyvsp[(7) - (8)].t_seq_exp));
								}
    break;

  case 318:

    {
								  (yyvsp[(7) - (8)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(5) - (8)]), (yyvsp[(5) - (8)].comment)));
								  (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(3) - (8)].t_seq_exp), *(yyvsp[(7) - (8)].t_seq_exp));
								}
    break;

  case 319:

    {
								  (yyvsp[(4) - (9)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(2) - (9)]), (yyvsp[(2) - (9)].comment)));
								  (yyvsp[(8) - (9)].t_seq_exp)->exps_get().push_front(new ast::CommentExp((yylsp[(6) - (9)]), (yyvsp[(6) - (9)].comment)));
								  (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[(4) - (9)].t_seq_exp), *(yyvsp[(8) - (9)].t_seq_exp));
								}
    break;

  case 320:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc)); }
    break;

  case 321:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc), (yyvsp[(2) - (2)].t_exp)); }
    break;

  case 322:

    { (yyval.t_op_exp_oper) = ast::OpExp::eq; }
    break;

  case 323:

    { (yyval.t_op_exp_oper) = ast::OpExp::ne; }
    break;

  case 324:

    { (yyval.t_op_exp_oper) = ast::OpExp::lt; }
    break;

  case 325:

    { (yyval.t_op_exp_oper) = ast::OpExp::le; }
    break;

  case 326:

    { (yyval.t_op_exp_oper) = ast::OpExp::gt; }
    break;

  case 327:

    { (yyval.t_op_exp_oper) = ast::OpExp::ge; }
    break;

  case 328:

    { (yyval.t_op_exp_oper) = ast::OpExp::binaryAnd; }
    break;

  case 329:

    { (yyval.t_op_exp_oper) = ast::OpExp::binaryOr; }
    break;



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
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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

