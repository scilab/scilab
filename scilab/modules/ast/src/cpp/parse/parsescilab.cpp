/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */

 // -*- C++ -*-
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
//#define BUILD_DEBUG_AST

#include <string>
#include <sstream>
#include <list>
#include "all.hxx"
#include "parse.hxx"
#include "parser_private.hxx"
#include "location.hxx"
#include "symbol.hxx"
#include "charEncoding.h"
#include "sci_malloc.h"

//#define DEBUG_RULES
#ifdef DEBUG_RULES
    #include <iomanip>
#endif

static void print_rules(const std::string& _parent, const std::string& _rules)
{
#ifdef DEBUG_RULES
    static std::list<std::pair<std::string, std::string> > rules;
    // add a space to perform a find as whole word of _parent in _rules
    rules.emplace_front(_parent+" ", _rules+" ");

    if(_parent == "program")
    {
        std::list<std::pair<std::string, std::string> > last;
        int spaces = 5; // 5 is the size of "|_./ "

        std::cout <<  "--- RULES ---" << std::endl;
        std::cout <<  "|_./ " << _parent << " : " << _rules << std::endl;

        last.emplace_back(rules.front());
        rules.pop_front();
        for(auto r : rules)
        {
            size_t pos = last.back().second.find(r.first);
            while(pos == std::string::npos)
            {
                spaces -= 2;
                last.pop_back();
                if(last.empty())
                {
                    break;
                }
                pos = last.back().second.find(r.first);
            }

            if(last.empty() == false)
            {
                last.back().second.erase(pos, r.first.length());
            }

            spaces += 2;
            last.emplace_back(r);

            std::setfill(" ");
            std::cout << std::setw(spaces) << "|_./ " << r.first << ": " << r.second << std::endl;
        }

        rules.clear();
    }
#endif
}

static void print_rules(const std::string& _parent, const double _value)
{
#ifdef DEBUG_RULES
    std::stringstream ostr;
    ostr << _value;
    print_rules(_parent, ostr.str());
#endif
}

#define StopOnError()                                           \
    {                                                           \
        if(ParserSingleInstance::stopOnFirstError())            \
        {                                                       \
            ParserSingleInstance::setExitStatus(Parser::ParserStatus::Failed);       \
        }                                                       \
    }

#define SetTree(PTR)                                                \
    {                                                               \
        if(ParserSingleInstance::getExitStatus() == Parser::Failed) \
        {                                                           \
            delete PTR;                                             \
            ParserSingleInstance::setTree(nullptr);                 \
        }                                                           \
        else                                                        \
        {                                                           \
            ParserSingleInstance::setTree(PTR);                     \
        }                                                           \
    }





# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    CONTROLTIMES = 282,
    RDIVIDE = 283,
    DOTRDIVIDE = 284,
    CONTROLRDIVIDE = 285,
    KRONRDIVIDE = 286,
    LDIVIDE = 287,
    DOTLDIVIDE = 288,
    CONTROLLDIVIDE = 289,
    KRONLDIVIDE = 290,
    POWER = 291,
    DOTPOWER = 292,
    EQ = 293,
    NE = 294,
    LT = 295,
    LE = 296,
    GT = 297,
    GE = 298,
    AND = 299,
    ANDAND = 300,
    OR = 301,
    OROR = 302,
    ASSIGN = 303,
    IF = 304,
    THEN = 305,
    ELSE = 306,
    ELSEIF = 307,
    END = 308,
    SELECT = 309,
    SWITCH = 310,
    CASE = 311,
    OTHERWISE = 312,
    FUNCTION = 313,
    ENDFUNCTION = 314,
    FOR = 315,
    WHILE = 316,
    DO = 317,
    BREAK = 318,
    CONTINUE = 319,
    TRY = 320,
    CATCH = 321,
    RETURN = 322,
    FLEX_ERROR = 323,
    STR = 324,
    ID = 325,
    VARINT = 326,
    VARFLOAT = 327,
    NUM = 328,
    PATH = 329,
    COMMENT = 330,
    BLOCKCOMMENT = 331,
    TOPLEVEL = 332,
    HIGHLEVEL = 333,
    UPLEVEL = 334,
    LISTABLE = 335,
    CONTROLBREAK = 336,
    UMINUS = 337,
    UPLUS = 338,
    FUNCTIONCALL = 339
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
#define CONTROLTIMES 282
#define RDIVIDE 283
#define DOTRDIVIDE 284
#define CONTROLRDIVIDE 285
#define KRONRDIVIDE 286
#define LDIVIDE 287
#define DOTLDIVIDE 288
#define CONTROLLDIVIDE 289
#define KRONLDIVIDE 290
#define POWER 291
#define DOTPOWER 292
#define EQ 293
#define NE 294
#define LT 295
#define LE 296
#define GT 297
#define GE 298
#define AND 299
#define ANDAND 300
#define OR 301
#define OROR 302
#define ASSIGN 303
#define IF 304
#define THEN 305
#define ELSE 306
#define ELSEIF 307
#define END 308
#define SELECT 309
#define SWITCH 310
#define CASE 311
#define OTHERWISE 312
#define FUNCTION 313
#define ENDFUNCTION 314
#define FOR 315
#define WHILE 316
#define DO 317
#define BREAK 318
#define CONTINUE 319
#define TRY 320
#define CATCH 321
#define RETURN 322
#define FLEX_ERROR 323
#define STR 324
#define ID 325
#define VARINT 326
#define VARFLOAT 327
#define NUM 328
#define PATH 329
#define COMMENT 330
#define BLOCKCOMMENT 331
#define TOPLEVEL 332
#define HIGHLEVEL 333
#define UPLEVEL 334
#define LISTABLE 335
#define CONTROLBREAK 336
#define UMINUS 337
#define UPLUS 338
#define FUNCTIONCALL 339

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{


  /* Tokens. */
    double                      number;
    std::wstring*               str;
    std::wstring*               path;
    std::wstring*               comment;

    LineBreakStr*               mute;

    ast::exps_t*                t_list_var;
    ast::exps_t*                t_list_exp;
    ast::Exp*                   t_exp;

    ast::SeqExp*                t_seq_exp;
    ast::ReturnExp*             t_return_exp;

    ast::IfExp*                 t_if_exp;
    ast::WhileExp*              t_while_exp;
    ast::ForExp*                t_for_exp;
    ast::TryCatchExp*           t_try_exp;
    ast::SelectExp*             t_select_exp;
    ast::CaseExp*               t_case_exp;
    ast::exps_t*                t_list_case;

    ast::CallExp*               t_call_exp;

    ast::MathExp*               t_math_exp;

    ast::OpExp*                 t_op_exp;
    ast::OpExp::Oper            t_op_exp_oper;
    ast::LogicalOpExp::Oper     t_lop_exp_oper;

    ast::AssignExp*             t_assign_exp;

    ast::StringExp*             t_string_exp;

    ast::ListExp*               t_implicit_list;

    ast::MatrixExp*             t_matrix_exp;
    ast::MatrixLineExp*         t_matrixline_exp;
    ast::exps_t*                t_list_mline;

    ast::CellExp*               t_cell_exp;

    ast::CellCallExp*           t_cell_call_exp;

    ast::FunctionDec*           t_function_dec;

    ast::ArrayListExp*          t_arraylist_exp;
    ast::AssignListExp*         t_assignlist_exp;
    ast::ArrayListVar*          t_arraylist_var;

    ast::SimpleVar*             t_simple_var;


};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  105
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3501

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  392
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  558

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   339

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   403,   403,   404,   405,   414,   429,   433,   439,   446,
     453,   468,   479,   488,   498,   517,   518,   519,   520,   521,
     522,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,   542,   543,   544,   558,   564,   580,   581,
     587,   593,   599,   600,   601,   602,   603,   610,   618,   620,
     631,   632,   633,   634,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   686,   698,
     707,   717,   727,   739,   748,   758,   775,   783,   784,   785,
     793,   799,   812,   813,   814,   815,   816,   824,   830,   845,
     846,   854,   861,   876,   877,   878,   880,   881,   882,   884,
     885,   886,   888,   889,   890,   892,   893,   894,   896,   897,
     898,   900,   901,   902,   904,   905,   906,   908,   909,   910,
     912,   913,   914,   922,   929,   936,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   946,   947,   948,   949,   950,
     951,   960,   961,   963,   964,   966,   967,   968,   969,   970,
     971,   972,   973,   975,   976,   977,   978,   979,   980,   981,
     982,   984,   985,   986,   987,   988,   989,   990,   991,   999,
    1000,  1008,  1009,  1010,  1018,  1019,  1020,  1021,  1022,  1028,
    1029,  1030,  1035,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,  1063,
    1068,  1073,  1079,  1085,  1091,  1103,  1104,  1105,  1110,  1115,
    1121,  1127,  1128,  1137,  1138,  1139,  1140,  1141,  1142,  1143,
    1144,  1152,  1153,  1163,  1164,  1165,  1166,  1174,  1175,  1183,
    1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1199,  1200,
    1208,  1209,  1210,  1211,  1213,  1214,  1216,  1217,  1226,  1227,
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1241,  1250,  1251,
    1262,  1270,  1276,  1291,  1297,  1314,  1315,  1316,  1317,  1318,
    1326,  1327,  1328,  1329,  1330,  1331,  1339,  1340,  1341,  1342,
    1343,  1344,  1352,  1358,  1372,  1388,  1389,  1400,  1401,  1420,
    1421,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1443,  1444,
    1452,  1453,  1454,  1455,  1456,  1464,  1465,  1466,  1467,  1468,
    1469,  1473,  1479,  1494,  1495,  1496,  1497,  1498,  1499,  1500,
    1501,  1502,  1503,  1504,  1505,  1513,  1514,  1522,  1523,  1532,
    1533,  1534,  1535,  1536,  1537,  1538,  1539,  1543,  1549,  1564,
    1572,  1578,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1618,  1619,  1634,  1640,  1646,  1652,  1658,  1666,  1681,  1682,
    1683,  1690,  1691,  1699,  1700,  1708,  1709,  1710,  1711,  1712,
    1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,  1722,
    1723,  1724,  1725
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"line break\"",
  "\"end of line\"", "\"spaces\"", "\"%t or %T\"", "\"%f or %F\"", "\"'\"",
  "\"~ or @\"", "\"$\"", "\",\"", "\":\"", "\";\"", "\"(\"", "\")\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\".\"", "\".'\"", "\"+\"", "\"-\"",
  "\"*\"", "\".*\"", "\".*.\"", "\"*.\"", "\"/\"", "\"./\"", "\"/.\"",
  "\"./.\"", "\"\\\\\"", "\".\\\\\"", "\"\\\\.\"", "\".\\\\.\"",
  "\"** or ^\"", "\".^\"", "\"==\"", "\"<> or ~=\"", "\"<\"", "\"<=\"",
  "\">\"", "\">=\"", "\"&\"", "\"&&\"", "\"|\"", "\"||\"", "\"=\"",
  "\"if\"", "\"then\"", "\"else\"", "\"elseif\"", "\"end\"", "\"select\"",
  "\"switch\"", "\"case\"", "\"otherwise\"", "\"function\"",
  "\"endfunction\"", "\"for\"", "\"while\"", "\"do\"", "\"break\"",
  "\"continue\"", "\"try\"", "\"catch\"", "\"return\"", "FLEX_ERROR",
  "\"string\"", "\"identifier\"", "\"integer\"", "\"float\"", "\"number\"",
  "\"path\"", "\"line comment\"", "\"block comment\"", "TOPLEVEL",
  "HIGHLEVEL", "UPLEVEL", "LISTABLE", "CONTROLBREAK", "UMINUS", "UPLUS",
  "FUNCTIONCALL", "$accept", "program", "expressions",
  "recursiveExpression", "expressionLineBreak", "expression",
  "implicitFunctionCall", "implicitCallable", "functionCall",
  "simpleFunctionCall", "functionArgs", "functionDeclaration",
  "functionDeclarationReturns", "functionDeclarationArguments", "idList",
  "functionDeclarationBreak", "functionBody", "condition", "comparison",
  "rightComparable", "operation", "rightOperand", "listableBegin",
  "listableEnd", "variable", "variableFields", "cell", "matrix",
  "matrixOrCellLines", "matrixOrCellLineBreak", "matrixOrCellLine",
  "matrixOrCellColumns", "matrixOrCellColumnsBreak", "variableDeclaration",
  "assignable", "multipleResults", "ifControl", "thenBody", "elseBody",
  "ifConditionBreak", "then", "else", "elseIfControl", "selectControl",
  "select", "defaultCase", "selectable", "selectConditionBreak",
  "casesControl", "caseBody", "caseControlBreak", "forControl",
  "forIterator", "forConditionBreak", "forBody", "whileControl",
  "whileBody", "whileConditionBreak", "tryControl", "catchBody",
  "returnControl", "comments", "lineEnd", "keywords", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339
};
# endif

#define YYPACT_NINF -461

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-461)))

#define YYTABLE_NINF -368

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     273,  -461,   665,  -461,  -461,  2743,  -461,  -461,  -461,  2743,
     418,  1995,  2743,  2743,  2743,  -461,  -461,    25,     4,  2743,
    -461,  -461,  1120,  2743,  -461,   287,  -461,  -461,  -461,  -461,
      33,  -461,   192,    97,    24,   406,  2881,  -461,  -461,  -461,
    -461,  3002,  -461,  -461,  -461,    22,  -461,  -461,  -461,  2743,
    -461,  -461,  -461,  -461,  -461,   418,   147,     3,   171,  2922,
    2962,   216,   514,  -461,  -461,  3042,  3082,   626,  -461,   302,
    2013,  -461,  2031,   392,   672,   733,   672,   733,  3042,    96,
    3082,    13,   131,    21,    50,   163,  1484,  1705,  1705,  -461,
     128,  3042,  3082,  -461,  -461,  -461,  2137,  2205,  -461,  -461,
    -461,  -461,  -461,  -461,    83,  -461,    27,  -461,  -461,  -461,
    -461,   215,    97,    83,  -461,  2743,  2223,  3409,  -461,  2743,
    2743,  2743,  2743,  2743,  2743,  2743,  2743,  2743,  2743,  2743,
    2743,  2743,  2743,  2743,  2743,  2327,  2345,  2413,  2431,  2449,
    2517,  2535,  2553,  2621,  2639,  2241,  -461,  -461,    94,  -461,
    -461,  2223,  1050,  -461,  2743,  2743,  -461,  -461,  -461,  2309,
    3042,  3082,    39,  1148,  2223,  3431,  2223,  2719,  2743,  -461,
    2743,  -461,  2743,  -461,  -461,  2101,  1303,  -461,  -461,  1775,
    -461,  -461,  -461,    65,  -461,  3042,  3082,    40,  1925,  -461,
    2119,  1795,  -461,  1815,  -461,  -461,   166,   169,   229,   154,
     750,   149,  -461,   195,   198,     6,   150,    69,   174,  2743,
    -461,   213,   230,   265,   277,   266,  1582,  -461,  -461,  -461,
    -461,  1333,  2657,  -461,  -461,   189,  2881,   224,  3002,  -461,
    -461,    38,   232,   215,    97,    97,  3330,  3360,   257,  -461,
    -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,
    -461,  -461,  -461,  -461,  -461,  -461,  -461,   252,   259,  3390,
    3420,  3390,  3420,   672,   733,   672,   733,   672,   733,   672,
     733,   672,   733,   672,   733,   672,   733,   672,   733,   672,
     733,   672,   733,   672,   733,   672,   733,   672,   733,   672,
     733,  -461,  3270,  3300,  -461,  3270,  3300,  -461,  3270,  3300,
    -461,  3270,  3300,  -461,  3270,  3300,  -461,  3270,  3300,  -461,
    3198,  3234,  -461,  3198,  3234,  -461,  3122,  3160,  -461,  3122,
    3160,  -461,  3042,  3082,  -461,  2743,   260,    15,   218,   269,
     281,   672,   733,   672,   733,  -461,  3042,  3082,  -461,  -461,
     328,   359,   365,   -43,  -461,   288,  -461,  -461,   299,   147,
    -461,  -461,  3042,  3082,  3042,  3082,  3042,  3082,  -461,  1885,
    -461,  -461,  -461,  -461,  -461,  -461,  3042,  3082,    40,  -461,
    1905,  -461,  -461,  -461,  -461,  -461,   361,  -461,   298,   300,
     332,   312,  -461,   315,   367,  -461,   381,   390,   391,  1509,
    -461,  2743,  3042,  3082,    43,  -461,  -461,  -461,   393,   396,
    -461,   400,   405,  -461,  -461,   364,   370,  -461,  2881,  3002,
    -461,  2725,  -461,  -461,  -461,    97,   372,  3330,  3360,   378,
     147,   403,  -461,  -461,   -43,  2743,   425,   311,   -14,  -461,
    -461,  -461,  -461,  -461,   375,  2743,  -461,  1607,   377,   367,
     363,  -461,  -461,    69,  -461,  -461,  -461,  -461,   137,   423,
    -461,   435,   439,   440,  1680,  -461,  -461,  -461,  -461,  -461,
    -461,  -461,  2881,  3002,  -461,   402,  2797,  2841,  -461,  -461,
    2743,   379,  -461,  1607,  2743,   441,  -461,   448,   453,    96,
    -461,   415,  -461,    69,   367,  1509,  -461,  -461,    43,  -461,
    -461,  -461,  -461,   416,  -461,  1607,  -461,   467,   468,   380,
    1409,  1409,  2797,  2841,  -461,   470,   477,   429,  2797,  2841,
    -461,  -461,  -461,   750,  -461,  1509,    69,   201,  1680,  -461,
     430,  -461,  -461,  -461,   480,   490,  -461,  -461,  -461,  1409,
    1409,  -461,  -461,  -461,  1409,  1409,   102,   239,  1509,  -461,
    -461,   443,  -461,  -461,  -461,  -461,  -461,  -461,  -461,  1607,
    -461,  -461,  -461,   294,  -461,  -461,  -461,  -461
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,    35,     0,   192,   193,     0,   191,    16,    15,     0,
       0,     0,     0,     0,     0,   289,   290,     0,     0,     0,
      31,    32,     0,   368,   190,   186,   187,   189,   188,    34,
       0,     2,     0,     4,     9,    30,    22,    48,    21,   196,
     185,    29,   184,   183,    23,     0,   254,    24,    25,     0,
      26,    27,    28,    33,     3,     0,   186,   175,   174,     0,
       0,     0,     0,   220,   237,   236,   235,     0,   222,     0,
       0,   212,     0,     0,   128,   127,   126,   125,    89,   275,
      90,     0,    79,     0,     0,     0,     0,     0,     0,   362,
       0,   370,   369,    44,    45,    43,     0,     0,    42,    38,
      39,    41,    40,    47,    37,     1,     7,    20,    19,    18,
      17,    10,    14,    36,   139,     0,     0,     0,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,   124,   173,   182,
     137,     0,     0,   138,     0,     0,    91,   123,   181,     0,
     299,   298,     0,     0,     0,     0,     0,     0,     0,    49,
       0,   194,     0,   195,   219,     0,     0,   213,   221,     0,
     224,   239,   223,   217,   233,   232,   231,   227,     0,   211,
       0,     0,   205,     0,   209,   269,   267,   265,   270,   274,
       0,     0,    81,     0,    76,     0,     0,     0,     0,     0,
     351,   342,   343,   347,   344,     0,     0,   363,   365,   364,
     361,     0,    58,    56,    52,   186,    55,     0,    54,    57,
      53,     0,     0,     8,    11,    13,   170,   169,     0,   375,
     376,   377,   378,   379,   380,   381,   383,   382,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   179,   180,   142,
     141,   144,   143,   146,   145,   148,   147,   150,   149,   152,
     151,   154,   153,   156,   155,   160,   159,   158,   157,   162,
     161,   164,   163,   168,   167,   166,   165,   132,   131,   136,
     135,   107,   106,   105,   110,   109,   108,   116,   115,   114,
     122,   121,   120,   113,   112,   111,   119,   118,   117,    95,
      94,    93,    98,    97,    96,   101,   100,    99,   104,   103,
     102,   245,   243,   242,   247,     0,     0,     0,   176,   178,
     177,   130,   129,   134,   133,   244,   241,   240,   246,   300,
     303,   304,     0,     0,   217,     0,   179,   180,     0,   176,
     178,   177,   202,   203,   204,   201,   200,   199,   214,     0,
     218,   215,   225,   226,   238,   234,   230,   229,   228,   206,
       0,   210,   207,   268,   266,   273,   271,   261,     0,     0,
       0,     0,    78,     0,    79,   373,    85,    83,     0,     0,
      82,     0,   327,   328,   336,   352,   353,   357,   348,   349,
     354,   345,   346,   350,   340,     0,     0,    61,    60,    59,
      62,    63,    50,    51,    46,    12,   198,   172,   171,   197,
       0,     0,   301,   302,     0,     0,     0,     0,     0,   198,
     197,   216,   208,   272,   276,     0,   258,     0,     0,    79,
       0,    80,    77,     0,    86,    84,   374,    87,     0,     0,
     329,   332,   330,   334,     0,   358,   359,   355,   356,   339,
     360,    66,    65,    64,    67,     0,   324,   324,   371,   285,
       0,   292,   291,     0,     0,     0,   279,   277,   278,   275,
     263,     0,   260,     0,    79,     0,    75,    71,   336,   333,
     331,   335,   337,     0,   287,     0,   316,   314,   315,   313,
       0,     0,   324,   324,   295,   293,   294,     0,   324,   324,
     372,   280,   281,     0,   259,     0,     0,     0,     0,   325,
       0,   318,   319,   317,   320,   322,   311,   306,   305,     0,
       0,   296,   297,   286,     0,     0,   282,     0,     0,    72,
      68,     0,   288,   321,   323,   310,   309,   308,   307,     0,
     284,    74,    70,     0,   326,   283,    73,    69
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -461,  -461,     0,  -461,   -22,   465,  -461,   463,    -4,  -461,
     188,  -461,  -461,  -380,   295,  -419,  -460,   -16,  -461,   581,
    -461,   705,  -461,   974,   437,  -461,  -461,  -461,     9,   313,
     -33,     5,  -461,   -70,  -461,  -461,  -461,   -11,  -459,   305,
      26,  -367,   -28,  -461,  -461,    46,  -461,   170,    90,  -352,
    -279,  -461,   125,    29,     1,  -461,  -461,  -461,  -461,   301,
     -97,  -461,  -461,    -9
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    30,   526,    32,    33,    34,    35,   104,    36,    37,
     227,    38,   203,   207,   204,   389,   448,    79,    39,   146,
      40,   147,   148,   149,    41,    61,    42,    43,    67,   187,
      68,    69,   188,    44,    45,    46,    47,   378,   481,   199,
     200,   472,   438,    48,    49,   473,   162,   343,   427,   527,
     500,    50,   394,   454,   493,    51,   405,   216,    52,    90,
      53,   428,   390,   258
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,    57,    54,    85,   443,    59,    65,    65,    74,    76,
      78,   437,   112,   425,   507,    78,    73,   164,    83,    91,
      72,   382,    89,   165,   485,   517,   229,   229,   110,   327,
     201,   110,   426,   105,   178,     7,   520,     8,     7,   178,
       8,    81,   474,   339,   362,   160,   229,   450,   324,   411,
     340,    65,   341,   363,   451,   537,   452,   413,    65,   483,
     163,   475,   338,    65,   515,   185,    65,   176,    65,   185,
     159,   175,   179,   385,    84,   191,   202,   193,   553,   190,
     386,   229,   387,   202,   234,   420,   217,   218,   219,   235,
     555,   208,   226,   226,   229,    82,   229,   538,   209,   111,
     195,   107,   233,   232,   516,   453,   325,   196,   108,   197,
     109,   236,   226,  -257,   342,   259,   261,   263,   265,   267,
     269,   271,   273,   275,   277,   279,   281,   283,   285,   287,
     289,   292,   295,   298,   301,   304,   307,   310,   313,   316,
     319,   322,   178,   330,   388,   205,   198,   226,   329,   528,
     331,   333,   410,   434,   435,   336,   347,   178,   351,   185,
     226,    96,   226,   350,   352,    97,   354,   210,   356,   549,
     373,    65,   185,   374,   211,   185,   212,   545,   546,   206,
     359,   220,   547,   548,   366,   166,    65,   185,   501,   185,
     486,   167,    -6,     1,   221,   370,   487,   379,     3,     4,
     377,     5,     6,    96,   376,   392,     9,    97,    10,   381,
      11,   415,   380,   213,    12,    13,   404,   395,   408,   110,
     384,    89,   391,   529,   530,   214,     7,   172,     8,   534,
     535,   173,    96,   195,   396,   411,    97,  -253,   215,   412,
     196,    14,   197,    -6,    -6,    -6,    15,    16,    -6,    -6,
      17,    -6,    18,    19,   539,    20,    21,    22,    -6,    23,
     540,    24,    25,    26,    27,    28,  -248,    29,   411,   397,
     403,   411,   416,    -5,     1,   419,   398,     2,   399,     3,
       4,   400,     5,     6,     7,   231,     8,     9,   401,    10,
     402,    11,   551,    93,    94,    12,    13,    95,   552,   411,
    -251,    96,   414,   429,   238,    97,   180,  -252,     3,     4,
     411,     5,     6,   181,   430,   182,     9,  -250,    55,   183,
      11,   417,    14,   421,    12,    13,   381,    15,    16,  -249,
     442,    17,   422,    18,    19,  -253,    20,    21,    22,   326,
      23,   464,    24,    25,    26,    27,    28,   556,    29,   434,
     435,   436,   345,   557,   348,   185,    98,    99,   100,   101,
     102,   103,   434,   423,   469,   433,   185,   470,   471,   339,
     439,    24,    56,    26,    27,    28,   340,   184,   341,   476,
     440,   205,   441,   504,   523,   444,   477,   392,   478,   447,
     505,   524,   506,   525,   445,   446,   180,   455,     3,     4,
     456,     5,     6,   181,   457,   182,     9,   462,    55,   458,
      11,   194,    93,    94,    12,    13,    95,   459,   169,   479,
    -256,   466,    62,   460,     3,     4,  -255,     5,     6,   468,
     482,    78,     9,   484,    55,    63,    11,   480,   488,   489,
      12,    13,    58,   490,   491,   510,    60,    66,    66,    75,
      77,    80,   511,   434,   492,   494,    80,   512,   470,   471,
      92,    24,    56,    26,    27,    28,   502,   184,   514,   519,
     508,   521,   522,   480,   531,    98,    99,   100,   101,   102,
     103,   532,   533,   542,   543,   447,   161,    24,    56,    26,
      27,    28,    66,    64,   544,   480,   554,   106,   113,    66,
     383,   368,   536,   375,    66,   513,   186,    66,   550,    66,
     186,   495,   424,   377,   465,   447,   449,   518,   492,   541,
       3,     4,   406,     5,     6,     0,     0,     0,     9,     0,
      55,   174,    11,   228,   228,     0,    12,    13,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,   237,   228,     0,     0,   260,   262,   264,   266,
     268,   270,   272,   274,   276,   278,   280,   282,   284,   286,
     288,   290,   293,   296,   299,   302,   305,   308,   311,   314,
     317,   320,   323,    24,    56,    26,    27,    28,   228,    64,
       0,   332,   334,     0,     0,     0,   337,     0,     0,     0,
     186,   228,     0,   228,     0,   353,     0,   355,     0,   357,
       0,     0,    66,   186,     0,     0,   186,     0,     0,     0,
       0,     0,   156,     0,     0,   367,     0,    66,   186,     0,
     186,     0,     3,     4,     0,     5,     6,     0,     0,   156,
       9,   156,    55,   177,    11,     0,   393,   156,    12,    13,
       0,     0,     0,     0,     0,     0,   156,     0,   156,   409,
       0,   156,     0,     0,     0,   -17,     1,     0,     0,   -17,
       0,     3,     4,   156,     5,     6,   -17,     0,   -17,     9,
     114,    10,     0,    11,     0,     0,   164,    12,    13,     0,
       0,     0,   165,   118,     0,    24,    56,    26,    27,    28,
       0,    64,     0,     0,     0,     0,     0,     0,   133,   134,
       0,     0,     0,     0,    14,     0,     0,     0,     0,    15,
      16,     0,     0,    17,     0,    18,    19,     0,    20,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,     0,
      29,   150,   156,     0,     0,     0,   157,   166,     0,     0,
       0,     1,     0,   167,   153,     0,     3,     4,     0,     5,
       6,     0,   418,   157,     9,   157,    10,   156,    11,   154,
     155,   157,    12,    13,     0,     0,     0,     0,     0,     0,
     157,     0,   157,     0,     0,   157,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   157,     0,    14,
       0,  -262,  -262,  -262,    15,    16,     0,   186,    17,   156,
      18,    19,     0,    20,    21,    22,     0,    23,   156,    24,
      25,    26,    27,    28,     0,    29,     0,     0,   393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,     0,   156,     0,   156,     0,   156,   463,   156,
       0,   156,     0,   156,     0,   156,     0,   156,     0,   156,
       0,   156,   467,   156,     0,   156,   157,   156,     0,   156,
       0,   156,    80,     0,   156,     0,     0,   156,     0,     0,
     156,     0,     0,   156,     0,     0,   156,     0,     0,   156,
       0,   157,   156,     0,     0,   156,     0,     0,   156,     0,
       0,   156,     0,     0,   156,     0,     0,   503,     0,     0,
       0,   509,     0,   156,     0,   156,     0,     0,   156,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,   156,     0,   156,     0,   156,     0,
       0,     0,   157,     0,     0,     0,     0,     0,   156,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,     0,   157,     0,   157,
       0,   157,     0,   157,   156,   157,     0,   157,     0,   157,
       0,   157,     0,   157,     0,   157,     0,   157,     0,   157,
     156,   157,     0,   157,     0,   157,     0,     0,   157,   156,
       0,   157,     0,     0,   157,     0,     0,   157,     0,     0,
     157,     0,     0,   157,     0,   158,   157,     0,     0,   157,
       0,     0,   157,     0,     0,   157,     0,     0,   157,     0,
       0,     0,   158,     0,   158,     0,     0,   157,     0,   157,
     158,     0,   157,     0,   156,     0,     0,     0,   156,   158,
       0,   158,     0,     0,   158,     0,     0,     0,   157,     0,
     157,     0,   157,     0,   327,     0,   158,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   156,     0,     0,     0,     0,     0,
     156,     0,     0,     0,     0,     0,     0,     0,   157,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   157,   254,   255,   256,     0,     0,
     328,     1,     0,   157,    86,     0,     3,     4,     0,     5,
       6,    87,     0,    88,     9,   158,    10,     0,    11,     0,
       0,     0,    12,    13,     0,     0,     0,     0,     0,     0,
       0,     0,   180,     0,     3,     4,     0,     5,     6,   181,
     158,   182,     9,     0,    55,   344,    11,     0,   157,    14,
      12,    13,   157,  -367,    15,    16,     0,     0,    17,     0,
      18,    19,     0,    20,    21,    22,  -367,    23,     0,    24,
      25,    26,    27,    28,     0,    29,     0,     0,     0,     0,
       0,     0,   158,     0,     0,     0,     0,     0,   157,     0,
       0,   158,     0,     0,   157,     0,     0,    24,    56,    26,
      27,    28,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,   158,     0,   158,     0,
     158,     0,   158,     0,   158,     0,   158,     0,   158,     0,
     158,     0,   158,     0,   158,     0,   158,     0,   158,     0,
     158,     0,   158,     0,   158,     0,     0,   158,     0,     0,
     158,     0,     0,   158,     0,     0,   158,     0,     0,   158,
       0,     0,   158,     0,     0,   158,     0,     0,   158,     0,
       0,   158,     0,     0,   158,     0,     0,   158,     0,     0,
       0,     0,     0,     0,     0,     0,   158,   180,   158,     3,
       4,   158,     5,     6,   181,     0,   182,     9,     0,    55,
     360,    11,     0,     0,     0,    12,    13,   158,     0,   158,
       0,   158,     0,     0,     1,     0,     0,    86,     0,     3,
       4,   158,     5,     6,    87,     0,    88,     9,     0,    10,
       0,    11,     0,     0,     0,    12,    13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,     0,     0,
       0,     0,    24,    56,    26,    27,    28,     0,   184,     0,
       0,     0,    14,   158,     0,     0,  -367,    15,    16,     0,
       0,    17,   158,    18,    19,     0,    20,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,     0,    29,     0,
       1,     0,     0,     0,     0,     3,     4,     0,     5,     6,
       0,     0,     0,     9,     0,    10,     0,    11,     0,     0,
       0,    12,    13,     0,     0,     0,     0,   158,     0,     0,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,     0,
    -312,     0,  -312,    15,    16,  -312,  -312,    17,     0,    18,
      19,     0,    20,    21,    22,     0,    23,   158,    24,    25,
      26,    27,    28,   158,    29,     1,     0,     0,     0,     0,
       3,     4,     0,     5,     6,     0,     0,     0,     9,     0,
      10,     0,    11,     0,     0,     0,    12,    13,     0,     0,
       1,     0,     0,     0,     0,     3,     4,     0,     5,     6,
       0,     0,     0,     9,     0,    10,     0,    11,     0,     0,
       0,    12,    13,    14,     0,     0,     0,  -366,    15,    16,
       0,     0,    17,     0,    18,    19,     0,    20,    21,    22,
    -366,    23,     0,    24,    25,    26,    27,    28,    14,    29,
       0,     0,   -88,    15,    16,     0,     0,    17,   -88,    18,
      19,     0,    20,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,     1,    29,     0,     0,     0,     3,     4,
       0,     5,     6,     0,     0,     0,     9,     0,    10,     0,
      11,     0,     0,     0,    12,    13,     0,     0,     1,     0,
       0,     0,     0,     3,     4,     0,     5,     6,     0,     0,
       0,     9,     0,    10,     0,    11,     0,     0,     0,    12,
      13,    14,     0,     0,     0,  -341,    15,    16,     0,     0,
      17,     0,    18,    19,     0,    20,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    14,    29,     0,     0,
    -264,    15,    16,     0,     0,    17,     0,    18,    19,     0,
      20,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,     1,    29,     0,     0,     0,     3,     4,     0,     5,
       6,     0,     0,     0,     9,     0,    10,     0,    11,     0,
       0,     0,    12,    13,     0,     0,     1,     0,     0,     0,
       0,     3,     4,     0,     5,     6,     0,     0,     0,     9,
       0,    10,     0,    11,     0,     0,     0,    12,    13,    14,
       0,     0,     0,  -338,    15,    16,     0,     0,    17,     0,
      18,    19,     0,    20,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    14,    29,     0,     0,     0,    15,
      16,     0,     0,    17,     0,    18,    19,     0,    20,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,   180,
      29,     3,     4,     0,     5,     6,   181,     0,   182,     9,
       0,    55,   361,    11,     0,     0,     0,    12,    13,   180,
       0,     3,     4,     0,     5,     6,   181,     0,   182,     9,
       0,    55,     0,    11,   371,     0,     0,    12,    13,   180,
       0,     3,     4,     0,     5,     6,   181,     0,   182,     9,
       0,    55,     0,    11,   372,     0,     0,    12,    13,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,   180,
     184,     3,     4,     0,     5,     6,   181,     0,   182,     9,
       0,    55,   431,    11,     0,     0,     0,    12,    13,   180,
       0,     3,     4,     0,     5,     6,   181,     0,   182,     9,
       0,    55,     0,    11,   432,     0,     0,    12,    13,   180,
       0,     3,     4,     0,     5,     6,   364,     0,   182,     9,
       0,    55,     0,    11,     0,     0,     0,    12,    13,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,    70,
     365,     3,     4,     0,     5,     6,     0,     0,     0,     9,
       0,    55,     0,    11,    71,     0,     0,    12,    13,     3,
       4,     0,     5,     6,     0,     0,     0,     9,     0,    55,
       0,    11,   189,     0,     0,    12,    13,     3,     4,     0,
       5,     6,     0,     0,     0,     9,     0,    55,     0,    11,
     192,     0,     0,    12,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    56,    26,    27,    28,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    56,    26,    27,    28,     0,    64,     3,     4,     0,
       5,     6,     0,     0,     0,     9,     0,    55,   358,    11,
       0,     0,     0,    12,    13,     3,     4,     0,     5,     6,
       0,     0,     0,     9,     0,    55,     0,    11,   369,     0,
       0,    12,    13,     3,     4,     0,     5,     6,   222,   223,
       0,     9,   224,    10,     0,    11,     0,     0,     0,    12,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    56,    26,    27,    28,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    56,
      26,    27,    28,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,   225,    26,    27,
      28,     3,     4,     0,     5,     6,   222,   223,     0,     9,
       0,    10,     0,    11,   230,     0,     0,    12,    13,     3,
       4,     0,     5,     6,   222,   223,     0,     9,     0,    10,
       0,    11,     0,     0,     0,    12,    13,     3,     4,     0,
       5,     6,     0,   321,     0,     9,     0,    55,     0,    11,
       0,     0,     0,    12,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,   225,    26,    27,    28,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,   225,    26,    27,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
      24,    56,    26,    27,    28,     3,     4,     0,     5,     6,
       0,   335,     0,     9,     0,    55,     0,    11,     0,     0,
       0,    12,    13,     3,     4,     0,     5,     6,     0,   291,
       0,     9,     0,    55,     0,    11,     0,     0,     0,    12,
      13,     3,     4,     0,     5,     6,     0,   294,     0,     9,
       0,    55,     0,    11,     0,     0,     0,    12,    13,     0,
       0,     0,     0,     0,     0,     0,    23,     0,    24,    56,
      26,    27,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    56,    26,    27,
      28,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     3,
       4,     0,     5,     6,     0,   297,     0,     9,     0,    55,
       0,    11,     0,     0,     0,    12,    13,     3,     4,     0,
       5,     6,     0,   300,     0,     9,     0,    55,     0,    11,
       0,     0,     0,    12,    13,     3,     4,     0,     5,     6,
       0,   303,     0,     9,     0,    55,     0,    11,     0,     0,
       0,    12,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    56,    26,    27,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    56,    26,    27,    28,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    56,
      26,    27,    28,     3,     4,     0,     5,     6,     0,   306,
       0,     9,     0,    55,     0,    11,     0,     0,     0,    12,
      13,     3,     4,     0,     5,     6,     0,   309,     0,     9,
       0,    55,     0,    11,     0,     0,     0,    12,    13,     3,
       4,     0,     5,     6,     0,   312,     0,     9,     0,    55,
       0,    11,     0,     0,     0,    12,    13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    56,    26,    27,
      28,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    56,    26,    27,    28,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    56,    26,    27,    28,     3,     4,     0,
       5,     6,     0,   315,     0,     9,     0,    55,     0,    11,
       0,     0,     0,    12,    13,     3,     4,     0,     5,     6,
       0,   318,     0,     9,     0,    55,     0,    11,     0,     0,
       0,    12,    13,     3,     4,     0,     5,     6,     0,   407,
       0,     9,     0,    10,     0,    11,     0,     0,     0,    12,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    56,    26,    27,    28,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    56,
      26,    27,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,   225,    26,    27,
      28,     3,     4,   327,     5,     6,     0,   461,     0,     9,
       0,    10,     0,    11,     0,     0,     0,    12,    13,     3,
       4,     0,     5,     6,     0,     0,     0,     9,     0,    55,
       0,    11,     0,     0,     0,    12,    13,     0,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,     0,   349,
       0,     0,     0,     0,    24,   225,    26,    27,    28,     0,
       0,   496,     0,     0,     0,   114,     0,     0,   497,   115,
     498,   164,    24,    56,    26,    27,    28,   165,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   496,     0,   499,     0,   150,
       0,     0,   497,   115,   498,   166,     0,     0,     0,     0,
       0,   167,   153,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   154,   155,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   114,
       0,   499,     0,   115,     0,   116,     0,     0,     0,     0,
       0,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     114,     0,     0,   168,   115,     0,   164,   169,     0,     0,
       0,     0,   165,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     150,     0,     0,   170,   115,     0,   166,   171,     0,     0,
       0,     0,   167,   153,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   154,   155,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     150,     0,     0,     0,   115,     0,   151,     0,     0,     0,
       0,     0,   152,   153,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   154,   155,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     114,     0,     0,     0,   115,     0,   164,     0,     0,     0,
       0,     0,   165,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     150,     0,     0,     0,   115,     0,   166,     0,     0,     0,
       0,     0,   167,   153,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   154,   155,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     114,     0,     0,     0,   115,     0,   164,     0,     0,     0,
       0,     0,   165,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   150,     0,
       0,     0,   115,     0,   166,     0,     0,     0,     0,     0,
     167,   153,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   154,   155,   135,   136,
     137,   138,   139,   140,   141,   142,   114,     0,     0,     0,
     115,     0,   164,     0,     0,     0,     0,     0,   165,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   150,     0,     0,     0,   115,     0,   166,     0,
       0,     0,     0,     0,   167,   153,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     154,   155,   135,   136,   137,   138,   139,   140,   114,     0,
       0,     0,   115,     0,   164,     0,     0,     0,     0,     0,
     165,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   150,     0,
       0,     0,   115,     0,   166,     0,     0,     0,     0,     0,
     167,   153,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   154,   155,   114,     0,
       0,     0,     0,     0,   164,     0,     0,     0,     0,     0,
     165,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   150,     0,
       0,     0,     0,     0,   166,     0,     0,     0,     0,     0,
     167,   153,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   154,   155,   114,     0,
       0,     0,     0,     0,   164,     0,     0,     0,     0,     0,
     165,   118,     0,     0,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   150,     0,
       0,     0,     0,     0,   166,     0,     0,     0,     0,     0,
     167,   153,     0,     0,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   154,   155,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,     0,   257,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
       0,   346
};

static const yytype_int16 yycheck[] =
{
       0,     5,     2,    19,   384,     9,    10,    11,    12,    13,
      14,   378,    34,    56,   473,    19,    11,    14,    14,    23,
      11,    15,    22,    20,   443,   485,    96,    97,     4,    14,
      17,     4,    75,     0,    67,    11,   495,    13,    11,    72,
      13,    16,    56,     4,     4,    49,   116,     4,   145,    11,
      11,    55,    13,    13,    11,   515,    13,    19,    62,   439,
      55,    75,   159,    67,   483,    69,    70,    62,    72,    73,
      48,    62,    67,     4,    70,    70,    70,    72,   538,    70,
      11,   151,    13,    70,   106,    70,    86,    87,    88,   111,
     549,    70,    96,    97,   164,    70,   166,   516,    48,    75,
       4,     4,    75,    20,   484,    62,    12,    11,    11,    13,
      13,   115,   116,    48,    75,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   175,   152,    75,    14,    50,   151,   152,   501,
     154,   155,   222,    51,    52,   159,   165,   190,   167,   163,
     164,    14,   166,   167,   168,    18,   170,     4,   172,   536,
       4,   175,   176,     4,    11,   179,    13,   529,   530,    48,
     175,    53,   534,   535,   188,    14,   190,   191,   467,   193,
      53,    20,     0,     1,    66,   190,    59,    48,     6,     7,
     200,     9,    10,    14,    50,   209,    14,    18,    16,    11,
      18,   233,    17,    50,    22,    23,   216,     4,   222,     4,
      70,   221,    48,   502,   503,    62,    11,    11,    13,   508,
     509,    15,    14,     4,     4,    11,    18,    48,    75,    15,
      11,    49,    13,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    53,    63,    64,    65,    66,    67,
      59,    69,    70,    71,    72,    73,    48,    75,    11,     4,
       4,    11,    15,     0,     1,    15,    11,     4,    13,     6,
       7,     4,     9,    10,    11,    97,    13,    14,    11,    16,
      13,    18,    53,     6,     7,    22,    23,    10,    59,    11,
      48,    14,    70,    15,   116,    18,     4,    48,     6,     7,
      11,     9,    10,    11,    15,    13,    14,    48,    16,    17,
      18,   325,    49,   327,    22,    23,    11,    54,    55,    48,
      15,    58,     4,    60,    61,    48,    63,    64,    65,   151,
      67,   411,    69,    70,    71,    72,    73,    53,    75,    51,
      52,    53,   164,    59,   166,   359,    69,    70,    71,    72,
      73,    74,    51,     4,    53,     4,   370,    56,    57,     4,
      70,    69,    70,    71,    72,    73,    11,    75,    13,     4,
      48,    14,    70,     4,     4,     4,    11,   391,    13,   389,
      11,    11,    13,    13,     4,     4,     4,     4,     6,     7,
       4,     9,    10,    11,     4,    13,    14,   411,    16,     4,
      18,    19,     6,     7,    22,    23,    10,    53,    15,   435,
      48,   425,     4,    53,     6,     7,    48,     9,    10,     4,
      53,   435,    14,    70,    16,    17,    18,   437,    15,     4,
      22,    23,     5,     4,     4,     4,     9,    10,    11,    12,
      13,    14,     4,    51,   454,    53,    19,     4,    56,    57,
      23,    69,    70,    71,    72,    73,   470,    75,    53,    53,
     474,     4,     4,   473,     4,    69,    70,    71,    72,    73,
      74,     4,    53,    53,     4,   485,    49,    69,    70,    71,
      72,    73,    55,    75,     4,   495,    53,    32,    35,    62,
     205,   188,   513,   198,    67,   479,    69,    70,   536,    72,
      73,   465,   342,   513,   424,   515,   391,   488,   518,   518,
       6,     7,   221,     9,    10,    -1,    -1,    -1,    14,    -1,
      16,    17,    18,    96,    97,    -1,    22,    23,   538,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   549,
      -1,    -1,   115,   116,    -1,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    69,    70,    71,    72,    73,   151,    75,
      -1,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     163,   164,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,   175,   176,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,   188,    -1,   190,   191,    -1,
     193,    -1,     6,     7,    -1,     9,    10,    -1,    -1,    58,
      14,    60,    16,    17,    18,    -1,   209,    66,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,   222,
      -1,    80,    -1,    -1,    -1,     0,     1,    -1,    -1,     4,
      -1,     6,     7,    92,     9,    10,    11,    -1,    13,    14,
       8,    16,    -1,    18,    -1,    -1,    14,    22,    23,    -1,
      -1,    -1,    20,    21,    -1,    69,    70,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    -1,    60,    61,    -1,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    72,    73,    -1,
      75,     8,   161,    -1,    -1,    -1,    41,    14,    -1,    -1,
      -1,     1,    -1,    20,    21,    -1,     6,     7,    -1,     9,
      10,    -1,   325,    58,    14,    60,    16,   186,    18,    36,
      37,    66,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   359,    92,    -1,    49,
      -1,    51,    52,    53,    54,    55,    -1,   370,    58,   228,
      60,    61,    -1,    63,    64,    65,    -1,    67,   237,    69,
      70,    71,    72,    73,    -1,    75,    -1,    -1,   391,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   260,    -1,   262,    -1,   264,    -1,   266,   411,   268,
      -1,   270,    -1,   272,    -1,   274,    -1,   276,    -1,   278,
      -1,   280,   425,   282,    -1,   284,   161,   286,    -1,   288,
      -1,   290,   435,    -1,   293,    -1,    -1,   296,    -1,    -1,
     299,    -1,    -1,   302,    -1,    -1,   305,    -1,    -1,   308,
      -1,   186,   311,    -1,    -1,   314,    -1,    -1,   317,    -1,
      -1,   320,    -1,    -1,   323,    -1,    -1,   470,    -1,    -1,
      -1,   474,    -1,   332,    -1,   334,    -1,    -1,   337,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   228,   353,    -1,   355,    -1,   357,    -1,
      -1,    -1,   237,    -1,    -1,    -1,    -1,    -1,   367,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   260,    -1,   262,    -1,   264,
      -1,   266,    -1,   268,   393,   270,    -1,   272,    -1,   274,
      -1,   276,    -1,   278,    -1,   280,    -1,   282,    -1,   284,
     409,   286,    -1,   288,    -1,   290,    -1,    -1,   293,   418,
      -1,   296,    -1,    -1,   299,    -1,    -1,   302,    -1,    -1,
     305,    -1,    -1,   308,    -1,    41,   311,    -1,    -1,   314,
      -1,    -1,   317,    -1,    -1,   320,    -1,    -1,   323,    -1,
      -1,    -1,    58,    -1,    60,    -1,    -1,   332,    -1,   334,
      66,    -1,   337,    -1,   463,    -1,    -1,    -1,   467,    75,
      -1,    77,    -1,    -1,    80,    -1,    -1,    -1,   353,    -1,
     355,    -1,   357,    -1,    14,    -1,    92,    -1,    -1,    -1,
      -1,    -1,   367,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   503,    -1,    -1,    -1,    -1,    -1,
     509,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   393,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,   409,    65,    66,    67,    -1,    -1,
      70,     1,    -1,   418,     4,    -1,     6,     7,    -1,     9,
      10,    11,    -1,    13,    14,   161,    16,    -1,    18,    -1,
      -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     4,    -1,     6,     7,    -1,     9,    10,    11,
     186,    13,    14,    -1,    16,    17,    18,    -1,   463,    49,
      22,    23,   467,    53,    54,    55,    -1,    -1,    58,    -1,
      60,    61,    -1,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,   228,    -1,    -1,    -1,    -1,    -1,   503,    -1,
      -1,   237,    -1,    -1,   509,    -1,    -1,    69,    70,    71,
      72,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   260,    -1,   262,    -1,   264,    -1,
     266,    -1,   268,    -1,   270,    -1,   272,    -1,   274,    -1,
     276,    -1,   278,    -1,   280,    -1,   282,    -1,   284,    -1,
     286,    -1,   288,    -1,   290,    -1,    -1,   293,    -1,    -1,
     296,    -1,    -1,   299,    -1,    -1,   302,    -1,    -1,   305,
      -1,    -1,   308,    -1,    -1,   311,    -1,    -1,   314,    -1,
      -1,   317,    -1,    -1,   320,    -1,    -1,   323,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   332,     4,   334,     6,
       7,   337,     9,    10,    11,    -1,    13,    14,    -1,    16,
      17,    18,    -1,    -1,    -1,    22,    23,   353,    -1,   355,
      -1,   357,    -1,    -1,     1,    -1,    -1,     4,    -1,     6,
       7,   367,     9,    10,    11,    -1,    13,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   393,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    -1,    75,    -1,
      -1,    -1,    49,   409,    -1,    -1,    53,    54,    55,    -1,
      -1,    58,   418,    60,    61,    -1,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    72,    73,    -1,    75,    -1,
       1,    -1,    -1,    -1,    -1,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,   463,    -1,    -1,
      -1,   467,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    -1,    63,    64,    65,    -1,    67,   503,    69,    70,
      71,    72,    73,   509,    75,     1,    -1,    -1,    -1,    -1,
       6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,
      16,    -1,    18,    -1,    -1,    -1,    22,    23,    -1,    -1,
       1,    -1,    -1,    -1,    -1,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,    49,    -1,    -1,    -1,    53,    54,    55,
      -1,    -1,    58,    -1,    60,    61,    -1,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    49,    75,
      -1,    -1,    53,    54,    55,    -1,    -1,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    72,    73,     1,    75,    -1,    -1,    -1,     6,     7,
      -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    -1,    -1,     1,    -1,
      -1,    -1,    -1,     6,     7,    -1,     9,    10,    -1,    -1,
      -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    49,    -1,    -1,    -1,    53,    54,    55,    -1,    -1,
      58,    -1,    60,    61,    -1,    63,    64,    65,    -1,    67,
      -1,    69,    70,    71,    72,    73,    49,    75,    -1,    -1,
      53,    54,    55,    -1,    -1,    58,    -1,    60,    61,    -1,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    72,
      73,     1,    75,    -1,    -1,    -1,     6,     7,    -1,     9,
      10,    -1,    -1,    -1,    14,    -1,    16,    -1,    18,    -1,
      -1,    -1,    22,    23,    -1,    -1,     1,    -1,    -1,    -1,
      -1,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,
      -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,    49,
      -1,    -1,    -1,    53,    54,    55,    -1,    -1,    58,    -1,
      60,    61,    -1,    63,    64,    65,    -1,    67,    -1,    69,
      70,    71,    72,    73,    49,    75,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    -1,    60,    61,    -1,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    72,    73,     4,
      75,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,     4,
      -1,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,     4,
      -1,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,     4,
      75,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,     4,
      -1,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,     4,
      -1,     6,     7,    -1,     9,    10,    11,    -1,    13,    14,
      -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,     4,
      75,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,
      -1,    18,    19,    -1,    -1,    22,    23,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    14,    -1,    16,    -1,    18,
      19,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    -1,    75,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    14,    -1,    16,    17,    18,
      -1,    -1,    -1,    22,    23,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    14,    -1,    16,    -1,    18,    19,    -1,
      -1,    22,    23,     6,     7,    -1,     9,    10,    11,    12,
      -1,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,     6,     7,    -1,     9,    10,    11,    12,    -1,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,     6,
       7,    -1,     9,    10,    11,    12,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,     6,     7,    -1,
       9,    10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    70,    71,    72,    73,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,     6,     7,    -1,     9,    10,    -1,    12,
      -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,     6,     7,    -1,     9,    10,    -1,    12,    -1,    14,
      -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,     6,
       7,    -1,     9,    10,    -1,    12,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,     6,     7,    -1,
       9,    10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    73,     6,     7,    -1,     9,    10,    -1,    12,
      -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,     6,     7,    -1,     9,    10,    -1,    12,    -1,    14,
      -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,     6,
       7,    -1,     9,    10,    -1,    12,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,     6,     7,    -1,
       9,    10,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      -1,    -1,    -1,    22,    23,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    18,    -1,    -1,
      -1,    22,    23,     6,     7,    -1,     9,    10,    -1,    12,
      -1,    14,    -1,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,     6,     7,    14,     9,    10,    -1,    12,    -1,    14,
      -1,    16,    -1,    18,    -1,    -1,    -1,    22,    23,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,    16,
      -1,    18,    -1,    -1,    -1,    22,    23,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    -1,
      -1,     4,    -1,    -1,    -1,     8,    -1,    -1,    11,    12,
      13,    14,    69,    70,    71,    72,    73,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     4,    -1,    50,    -1,     8,
      -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     8,
      -1,    50,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
       8,    -1,    -1,    11,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       8,    -1,    -1,    11,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     8,    -1,
      -1,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     8,    -1,    -1,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,     8,    -1,    -1,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,     8,    -1,
      -1,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     8,    -1,
      -1,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     8,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     8,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     8,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     8,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    -1,    70,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      -1,    70
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,     6,     7,     9,    10,    11,    13,    14,
      16,    18,    22,    23,    49,    54,    55,    58,    60,    61,
      63,    64,    65,    67,    69,    70,    71,    72,    73,    75,
      86,    87,    88,    89,    90,    91,    93,    94,    96,   103,
     105,   109,   111,   112,   118,   119,   120,   121,   128,   129,
     136,   140,   143,   145,    87,    16,    70,    93,   109,    93,
     109,   110,     4,    17,    75,    93,   109,   113,   115,   116,
       4,    19,   113,   116,    93,   109,    93,   109,    93,   102,
     109,    16,    70,    14,    70,   102,     4,    11,    13,    87,
     144,    93,   109,     6,     7,    10,    14,    18,    69,    70,
      71,    72,    73,    74,    92,     0,    90,     4,    11,    13,
       4,    75,    89,    92,     8,    12,    14,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,   104,   106,   107,   108,
       8,    14,    20,    21,    36,    37,   104,   106,   108,    48,
      93,   109,   131,   116,    14,    20,    14,    20,    11,    15,
      11,    15,    11,    15,    17,   113,   116,    17,   115,   116,
       4,    11,    13,    17,    75,    93,   109,   114,   117,    19,
     113,   116,    19,   116,    19,     4,    11,    13,    50,   124,
     125,    17,    70,    97,    99,    14,    48,    98,    70,    48,
       4,    11,    13,    50,    62,    75,   142,    87,    87,    87,
      53,    66,    11,    12,    15,    70,    93,    95,   109,   118,
      19,    95,    20,    75,    89,    89,    93,   109,    95,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    65,    66,    67,    70,   148,    93,
     109,    93,   109,    93,   109,    93,   109,    93,   109,    93,
     109,    93,   109,    93,   109,    93,   109,    93,   109,    93,
     109,    93,   109,    93,   109,    93,   109,    93,   109,    93,
     109,    12,    93,   109,    12,    93,   109,    12,    93,   109,
      12,    93,   109,    12,    93,   109,    12,    93,   109,    12,
      93,   109,    12,    93,   109,    12,    93,   109,    12,    93,
     109,    12,    93,   109,   145,    12,    95,    14,    70,    93,
     148,    93,   109,    93,   109,    12,    93,   109,   145,     4,
      11,    13,    75,   132,    17,    95,    70,   148,    95,    70,
      93,   148,    93,   109,    93,   109,    93,   109,    17,   116,
      17,    17,     4,    13,    11,    75,    93,   109,   114,    19,
     116,    19,    19,     4,     4,   124,    50,    87,   122,    48,
      17,    11,    15,    99,    70,     4,    11,    13,    75,   100,
     147,    48,    93,   109,   137,     4,     4,     4,    11,    13,
       4,    11,    13,     4,    87,   141,   144,    12,    93,   109,
     118,    11,    15,    19,    70,    89,    15,    93,   109,    15,
      70,    93,     4,     4,   132,    56,    75,   133,   146,    15,
      15,    17,    19,     4,    51,    52,    53,   126,   127,    70,
      48,    70,    15,    98,     4,     4,     4,    87,   101,   137,
       4,    11,    13,    62,   138,     4,     4,     4,     4,    53,
      53,    12,    93,   109,   118,   133,    93,   109,     4,    53,
      56,    57,   126,   130,    56,    75,     4,    11,    13,   102,
      87,   123,    53,    98,    70,   100,    53,    59,    15,     4,
       4,     4,    87,   139,    53,   130,     4,    11,    13,    50,
     135,   135,    93,   109,     4,    11,    13,   123,    93,   109,
       4,     4,     4,   125,    53,   100,    98,   101,   138,    53,
     123,     4,     4,     4,    11,    13,    87,   134,   134,   135,
     135,     4,     4,    53,   135,   135,   122,   101,   100,    53,
      59,   139,    53,     4,     4,   134,   134,   134,   134,   126,
     127,    53,    59,   101,    53,   123,    53,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    86,    86,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    89,    89,    89,    89,    89,
      89,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    93,    93,
      94,    94,    94,    94,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    96,    96,
      96,    96,    96,    96,    96,    96,    97,    98,    98,    98,
      99,    99,   100,   100,   100,   100,   100,   101,   101,   102,
     102,   103,   103,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   107,
     107,   108,   108,   108,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   110,
     110,   110,   110,   110,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   112,   112,   112,   112,   112,   112,   112,
     112,   113,   113,   114,   114,   114,   114,   115,   115,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   117,   117,
     118,   118,   118,   118,   118,   118,   118,   118,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   120,   121,   121,
     121,   122,   122,   123,   123,   124,   124,   124,   124,   124,
     125,   125,   125,   125,   125,   125,   126,   126,   126,   126,
     126,   126,   127,   127,   127,   128,   128,   128,   128,   129,
     129,   130,   130,   130,   130,   130,   130,   130,   131,   131,
     132,   132,   132,   132,   132,   133,   133,   133,   133,   133,
     133,   134,   134,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   136,   136,   137,   137,   138,
     138,   138,   138,   138,   138,   138,   138,   139,   139,   140,
     141,   141,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     143,   143,   144,   144,   144,   144,   144,   144,   145,   145,
     145,   146,   146,   147,   147,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     0,     1,     2,     3,     1,
       2,     3,     4,     3,     2,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     3,
       4,     4,     3,     3,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     8,    10,
       9,     6,     8,    10,     9,     6,     1,     3,     2,     0,
       3,     1,     1,     1,     2,     1,     2,     1,     0,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     1,     2,     2,     3,     3,     3,     3,
       3,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     4,     4,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     5,     3,
       4,     3,     2,     3,     4,     4,     5,     3,     4,     3,
       2,     2,     1,     1,     1,     2,     2,     2,     3,     3,
       3,     2,     2,     2,     3,     1,     1,     1,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     4,     4,     3,     5,     7,
       6,     1,     0,     1,     0,     1,     2,     1,     2,     1,
       1,     2,     3,     2,     1,     0,     1,     2,     2,     2,
       3,     3,     4,     6,     5,     5,     7,     6,     8,     1,
       1,     1,     1,     2,     2,     2,     3,     3,     1,     1,
       1,     2,     2,     1,     1,     4,     4,     5,     5,     5,
       5,     1,     0,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     2,     3,     0,     7,     9,     1,     1,     1,
       1,     2,     1,     2,     1,     2,     0,     1,     0,     5,
       1,     0,     1,     1,     1,     2,     2,     1,     2,     2,
       2,     1,     2,     2,     2,     3,     3,     2,     3,     3,
       5,     3,     1,     2,     2,     2,     1,     0,     1,     2,
       2,     2,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 69: /* "string"  */

      { delete ((*yyvaluep).str); }

        break;

    case 70: /* "identifier"  */

      { delete ((*yyvaluep).str); }

        break;

    case 71: /* "integer"  */

      { }

        break;

    case 72: /* "float"  */

      { }

        break;

    case 73: /* "number"  */

      { }

        break;

    case 74: /* "path"  */

      { delete ((*yyvaluep).path); }

        break;

    case 75: /* "line comment"  */

      { delete ((*yyvaluep).comment); }

        break;

    case 76: /* "block comment"  */

      { delete ((*yyvaluep).comment); }

        break;

    case 87: /* expressions  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 88: /* recursiveExpression  */

      { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }

        break;

    case 89: /* expressionLineBreak  */

      { delete ((*yyvaluep).mute); }

        break;

    case 90: /* expression  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 91: /* implicitFunctionCall  */

      { delete ((*yyvaluep).t_call_exp); }

        break;

    case 92: /* implicitCallable  */

      { delete ((*yyvaluep).t_string_exp); }

        break;

    case 93: /* functionCall  */

      { delete ((*yyvaluep).t_call_exp); }

        break;

    case 94: /* simpleFunctionCall  */

      { delete ((*yyvaluep).t_call_exp); }

        break;

    case 95: /* functionArgs  */

      { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }

        break;

    case 96: /* functionDeclaration  */

      { delete ((*yyvaluep).t_function_dec); }

        break;

    case 97: /* functionDeclarationReturns  */

      { for (auto e : *((*yyvaluep).t_list_var)) delete e; delete ((*yyvaluep).t_list_var); }

        break;

    case 98: /* functionDeclarationArguments  */

      { for (auto e : *((*yyvaluep).t_list_var)) delete e; delete ((*yyvaluep).t_list_var); }

        break;

    case 99: /* idList  */

      { for (auto e : *((*yyvaluep).t_list_var)) delete e; delete ((*yyvaluep).t_list_var); }

        break;

    case 101: /* functionBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 102: /* condition  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 103: /* comparison  */

      { delete ((*yyvaluep).t_op_exp); }

        break;

    case 104: /* rightComparable  */

      { delete ((*yyvaluep).t_op_exp); }

        break;

    case 105: /* operation  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 106: /* rightOperand  */

      { delete ((*yyvaluep).t_op_exp); }

        break;

    case 107: /* listableBegin  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 108: /* listableEnd  */

      { delete ((*yyvaluep).t_implicit_list); }

        break;

    case 109: /* variable  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 110: /* variableFields  */

      { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }

        break;

    case 111: /* cell  */

      { delete ((*yyvaluep).t_cell_exp); }

        break;

    case 112: /* matrix  */

      { delete ((*yyvaluep).t_matrix_exp); }

        break;

    case 113: /* matrixOrCellLines  */

      { for (auto e : *((*yyvaluep).t_list_mline)) delete e; delete ((*yyvaluep).t_list_mline); }

        break;

    case 115: /* matrixOrCellLine  */

      { delete ((*yyvaluep).t_matrixline_exp); }

        break;

    case 116: /* matrixOrCellColumns  */

      { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }

        break;

    case 118: /* variableDeclaration  */

      { delete ((*yyvaluep).t_assign_exp); }

        break;

    case 119: /* assignable  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 120: /* multipleResults  */

      { delete ((*yyvaluep).t_assignlist_exp); }

        break;

    case 121: /* ifControl  */

      { delete ((*yyvaluep).t_if_exp); }

        break;

    case 122: /* thenBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 123: /* elseBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 127: /* elseIfControl  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 128: /* selectControl  */

      { delete ((*yyvaluep).t_select_exp); }

        break;

    case 131: /* selectable  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 133: /* casesControl  */

      { for (auto e : *((*yyvaluep).t_list_case)) delete e; delete ((*yyvaluep).t_list_case); }

        break;

    case 134: /* caseBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 136: /* forControl  */

      { delete ((*yyvaluep).t_for_exp); }

        break;

    case 137: /* forIterator  */

      { delete ((*yyvaluep).t_exp); }

        break;

    case 139: /* forBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 140: /* whileControl  */

      { delete ((*yyvaluep).t_while_exp); }

        break;

    case 141: /* whileBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 143: /* tryControl  */

      { delete ((*yyvaluep).t_try_exp); }

        break;

    case 144: /* catchBody  */

      { delete ((*yyvaluep).t_seq_exp); }

        break;

    case 145: /* returnControl  */

      { delete ((*yyvaluep).t_return_exp); }

        break;

    case 148: /* keywords  */

      { delete ((*yyvaluep).t_simple_var); }

        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { SetTree((yyvsp[0].t_seq_exp)); print_rules("program", "expressions");}

    break;

  case 3:

    { SetTree((yyvsp[0].t_seq_exp)); print_rules("program", "EOL expressions");}

    break;

  case 4:

    {
                                    print_rules("program", "expressionLineBreak");
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty body")));
                                    #endif
                                    SetTree(new ast::SeqExp((yyloc), *tmp));
                                    delete (yyvsp[0].mute);
                                }

    break;

  case 5:

    {
                                    print_rules("program", "Epsilon");
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty body")));
                                    #endif
                                    SetTree(new ast::SeqExp((yyloc), *tmp));
                                }

    break;

  case 6:

    {
                                                  print_rules("expressions", "recursiveExpression");
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[0].t_list_exp));
                                                }

    break;

  case 7:

    {
                                                  print_rules("expressions", "recursiveExpression expression");
                                                  (yyvsp[0].t_exp)->setVerbose(true);
                                                  (yyvsp[-1].t_list_exp)->push_back((yyvsp[0].t_exp));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[-1].t_list_exp));
                                                }

    break;

  case 8:

    {
                                                  print_rules("expressions", "recursiveExpression expression COMMENT");
                                                  (yyvsp[-1].t_exp)->setVerbose(true);
                                                  (yyvsp[-2].t_list_exp)->push_back((yyvsp[-1].t_exp));
                                                  (yyvsp[-2].t_list_exp)->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[-2].t_list_exp));
                                                }

    break;

  case 9:

    {
                                                  print_rules("expressions", "expression");
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  (yyvsp[0].t_exp)->setVerbose(true);
                                                  tmp->push_back((yyvsp[0].t_exp));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                                }

    break;

  case 10:

    {
                                                  print_rules("expressions", "expression COMMENT");
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  (yyvsp[-1].t_exp)->setVerbose(true);
                                                  tmp->push_back((yyvsp[-1].t_exp));
                                                  tmp->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                                }

    break;

  case 11:

    {
                              print_rules("recursiveExpression", "recursiveExpression expression expressionLineBreak");
                              (yyvsp[-1].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              (yyvsp[-2].t_list_exp)->push_back((yyvsp[-1].t_exp));
                              (yyval.t_list_exp) = (yyvsp[-2].t_list_exp);
                              if ((yyvsp[0].mute)->iNbBreaker != 0)
                              {
                                  (yyvsp[-1].t_exp)->getLocation().last_column = (yyvsp[0].mute)->iNbBreaker;
                              }
                              delete (yyvsp[0].mute);
                            }

    break;

  case 12:

    {
                              print_rules("recursiveExpression", "recursiveExpression expression COMMENT expressionLineBreak");
                              (yyvsp[-2].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              (yyvsp[-3].t_list_exp)->push_back((yyvsp[-2].t_exp));
                              (yylsp[-1]).columns((yyvsp[0].mute)->iNbBreaker);
                              (yyvsp[-3].t_list_exp)->push_back(new ast::CommentExp((yylsp[-1]), (yyvsp[-1].comment)));
                              (yyval.t_list_exp) = (yyvsp[-3].t_list_exp);
                              delete (yyvsp[0].mute);
                            }

    break;

  case 13:

    {
                              print_rules("recursiveExpression", "expression COMMENT expressionLineBreak");
                              ast::exps_t* tmp = new ast::exps_t;
                              (yylsp[-1]).columns((yyvsp[0].mute)->iNbBreaker);
                              (yyvsp[-2].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              tmp->push_back((yyvsp[-2].t_exp));
                              tmp->push_back(new ast::CommentExp((yylsp[-1]), (yyvsp[-1].comment)));
                              (yyval.t_list_exp) = tmp;
                              delete (yyvsp[0].mute);
                            }

    break;

  case 14:

    {
                              print_rules("recursiveExpression", "expression expressionLineBreak");
                              ast::exps_t* tmp = new ast::exps_t;
                              (yyvsp[-1].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              tmp->push_back((yyvsp[-1].t_exp));
                              (yyval.t_list_exp) = tmp;
                              if ((yyvsp[0].mute)->iNbBreaker != 0)
                              {
                                  (yyvsp[-1].t_exp)->getLocation().last_column = (yyvsp[0].mute)->iNbBreaker;
                              }
                  delete (yyvsp[0].mute);
                            }

    break;

  case 15:

    { (yyval.mute) = new LineBreakStr(); (yyval.mute)->bVerbose = false; (yyval.mute)->iNbBreaker = (yylsp[0]).last_column;print_rules("expressionLineBreak", "SEMI"); }

    break;

  case 16:

    { (yyval.mute) = new LineBreakStr(); (yyval.mute)->bVerbose = true; (yyval.mute)->iNbBreaker = (yylsp[0]).last_column;print_rules("expressionLineBreak", "COMMA"); }

    break;

  case 17:

    { (yyval.mute) = new LineBreakStr(); (yyval.mute)->bVerbose = true; (yyval.mute)->iNbBreaker = 0;print_rules("expressionLineBreak", "expressionLineBreak SEMI"); }

    break;

  case 18:

    { (yyval.mute) = (yyvsp[-1].mute); (yyval.mute)->bVerbose = false || (yyvsp[-1].mute)->bVerbose; (yyval.mute)->iNbBreaker = (yylsp[0]).last_column;print_rules("expressionLineBreak", "SEMI"); }

    break;

  case 19:

    { (yyval.mute) = (yyvsp[-1].mute); (yyval.mute)->iNbBreaker = (yylsp[0]).last_column;print_rules("expressionLineBreak", "expressionLineBreak COMMA"); }

    break;

  case 20:

    { (yyval.mute) = (yyvsp[-1].mute);print_rules("expressionLineBreak", "expressionLineBreak EOL"); }

    break;

  case 21:

    { (yyval.t_exp) = (yyvsp[0].t_function_dec); print_rules("expression", "functionDeclaration");}

    break;

  case 22:

    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("expression", "functionCall");}

    break;

  case 23:

    { (yyval.t_exp) = (yyvsp[0].t_assign_exp); print_rules("expression", "variableDeclaration");}

    break;

  case 24:

    { (yyval.t_exp) = (yyvsp[0].t_if_exp); print_rules("expression", "ifControl");}

    break;

  case 25:

    { (yyval.t_exp) = (yyvsp[0].t_select_exp); print_rules("expression", "selectControl");}

    break;

  case 26:

    { (yyval.t_exp) = (yyvsp[0].t_for_exp); print_rules("expression", "forControl");}

    break;

  case 27:

    { (yyval.t_exp) = (yyvsp[0].t_while_exp); print_rules("expression", "whileControl");}

    break;

  case 28:

    { (yyval.t_exp) = (yyvsp[0].t_try_exp); print_rules("expression", "tryControl");}

    break;

  case 29:

    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("expression", "variable");}

    break;

  case 30:

    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("expression", "implicitFunctionCall");}

    break;

  case 31:

    { (yyval.t_exp) = new ast::BreakExp((yyloc)); print_rules("expression", "BREAK");}

    break;

  case 32:

    { (yyval.t_exp) = new ast::ContinueExp((yyloc)); print_rules("expression", "CONTINUE");}

    break;

  case 33:

    { (yyval.t_exp) = (yyvsp[0].t_return_exp); print_rules("expression", "returnControl");}

    break;

  case 34:

    { (yyval.t_exp) = new ast::CommentExp((yyloc), (yyvsp[0].comment)); print_rules("expression", "COMMENT");}

    break;

  case 35:

    {
    print_rules("expression", "error");
    (yyval.t_exp) = new ast::CommentExp((yyloc), new std::wstring(L"@@ ERROR RECOVERY @@"));
    StopOnError();
  }

    break;

  case 36:

    {
                          print_rules("implicitFunctionCall", "implicitFunctionCall implicitCallable");
                          (yyvsp[-1].t_call_exp)->addArg((yyvsp[0].t_string_exp));
                          (yyvsp[-1].t_call_exp)->setLocation((yyloc));
                          (yyval.t_call_exp) = (yyvsp[-1].t_call_exp);
                        }

    break;

  case 37:

    {
                          print_rules("implicitFunctionCall", "ID implicitCallable");
                          ast::exps_t* tmp = new ast::exps_t;
                          tmp->push_back((yyvsp[0].t_string_exp));
                          (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[-1]), symbol::Symbol(*(yyvsp[-1].str))), *tmp);
                          delete (yyvsp[-1].str);
                        }

    break;

  case 38:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[0].str)); delete (yyvsp[0].str);print_rules("implicitCallable", "ID");}

    break;

  case 39:

    {
                              print_rules("implicitCallable", (yyvsp[0].number));
                              std::wstringstream tmp;
                              tmp << (yyvsp[0].number);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                        }

    break;

  case 40:

    {
                              print_rules("implicitCallable", (yyvsp[0].number));
                              std::wstringstream tmp;
                              tmp << (yyvsp[0].number);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                        }

    break;

  case 41:

    {
                              print_rules("implicitCallable", (yyvsp[0].number));
                              std::wstringstream tmp;
                              tmp << (yyvsp[0].number);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                        }

    break;

  case 42:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[0].str)); delete (yyvsp[0].str);print_rules("implicitCallable", "STR");}

    break;

  case 43:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), std::wstring(L"$")); print_rules("implicitCallable", "DOLLAR");}

    break;

  case 44:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), std::wstring(L"%t")); print_rules("implicitCallable", "BOOLTRUE");}

    break;

  case 45:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), std::wstring(L"%f")); print_rules("implicitCallable", "BOOLFALSE");}

    break;

  case 46:

    {
                              print_rules("implicitCallable", "implicitCallable DOT ID");
                              std::wstringstream tmp;
                              tmp << (yyvsp[-2].t_string_exp)->getValue() << "." << *(yyvsp[0].str);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                              delete (yyvsp[0].str);
                        }

    break;

  case 47:

    { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[0].path)); delete (yyvsp[0].path);print_rules("implicitCallable", "PATH");}

    break;

  case 48:

    { (yyval.t_call_exp) = (yyvsp[0].t_call_exp); print_rules("functionCall", "simpleFunctionCall");}

    break;

  case 49:

    { (yyval.t_call_exp) = (yyvsp[-1].t_call_exp); print_rules("functionCall", "LPAREN functionCall RPAREN");}

    break;

  case 50:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[-3]), symbol::Symbol(*(yyvsp[-3].str))), *(yyvsp[-1].t_list_exp)); delete (yyvsp[-3].str);print_rules("simpleFunctionCall", "ID LPAREN functionArgs RPAREN");}

    break;

  case 51:

    { (yyval.t_call_exp) = new ast::CellCallExp((yyloc), *new ast::SimpleVar((yylsp[-3]), symbol::Symbol(*(yyvsp[-3].str))), *(yyvsp[-1].t_list_exp)); delete (yyvsp[-3].str);print_rules("simpleFunctionCall", "ID LBRACE functionArgs RBRACE");}

    break;

  case 52:

    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[-2]), symbol::Symbol(*(yyvsp[-2].str))), *new ast::exps_t); delete (yyvsp[-2].str);print_rules("simpleFunctionCall", "ID LPAREN RPAREN");}

    break;

  case 53:

    { (yyval.t_call_exp) = new ast::CellCallExp((yyloc), *new ast::SimpleVar((yylsp[-2]), symbol::Symbol(*(yyvsp[-2].str))), *new ast::exps_t); delete (yyvsp[-2].str);print_rules("simpleFunctionCall", "ID LBRACE RBRACE");}

    break;

  case 54:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_exp));print_rules("functionArgs", "variable");}

    break;

  case 55:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));print_rules("functionArgs", "functionCall");}

    break;

  case 56:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::ColonVar((yylsp[0])));print_rules("functionArgs", "COLON");}

    break;

  case 57:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_assign_exp));print_rules("functionArgs", "variableDeclaration");}

    break;

  case 58:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[0])));(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[0])));print_rules("functionArgs", "COMMA");}

    break;

  case 59:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back((yyvsp[0].t_exp));print_rules("functionArgs", "COMMA variable");}

    break;

  case 60:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));print_rules("functionArgs", "COMMA functionCall");}

    break;

  case 61:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back(new ast::ColonVar((yylsp[0])));print_rules("functionArgs", "COMMA COLON");}

    break;

  case 62:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back((yyvsp[0].t_assign_exp));print_rules("functionArgs", "COMMA variableDeclaration");}

    break;

  case 63:

    {(yyvsp[-1].t_list_exp)->push_back(new ast::NilExp((yylsp[0])));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("functionArgs", "functionArgs COMMA");}

    break;

  case 64:

    {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA variable");}

    break;

  case 65:

    {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_call_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA functionCall");}

    break;

  case 66:

    {(yyvsp[-2].t_list_exp)->push_back(new ast::ColonVar((yylsp[-2])));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA COLON");}

    break;

  case 67:

    {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_assign_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA variableDeclaration");}

    break;

  case 68:

    {
                  print_rules("functionDeclaration", "FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  ast::exps_t* tmp = new ast::exps_t;
                  tmp->push_back(new ast::SimpleVar((yylsp[-6]), symbol::Symbol(*(yyvsp[-6].str))));
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-6]), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-6].str);
                  delete (yyvsp[-4].str);
                }

    break;

  case 69:

    {
                  print_rules("functionDeclaration", "FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-7]) ,*(yyvsp[-7].t_list_var)),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }

    break;

  case 70:

    {
                  print_rules("functionDeclaration", "FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  ast::exps_t* tmp = new ast::exps_t;
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-7]), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }

    break;

  case 71:

    {
                  print_rules("functionDeclaration", "FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  ast::exps_t* tmp = new ast::exps_t;
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yyloc), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }

    break;

  case 72:

    {
                  print_rules("functionDeclaration", "FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END ");
                  ast::exps_t* tmp = new ast::exps_t;
                  tmp->push_back(new ast::SimpleVar((yylsp[-6]), symbol::Symbol(*(yyvsp[-6].str))));
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-6]), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-6].str);
                  delete (yyvsp[-4].str);
                }

    break;

  case 73:

    {
                  print_rules("functionDeclaration", "FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END");
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-7]) ,*(yyvsp[-7].t_list_var)),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }

    break;

  case 74:

    {
                  print_rules("functionDeclaration", "FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END");
                  ast::exps_t* tmp = new ast::exps_t;
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-7]), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }

    break;

  case 75:

    {
                  print_rules("functionDeclaration", "FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END");
                  ast::exps_t* tmp = new ast::exps_t;
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yyloc), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }

    break;

  case 76:

    { (yyval.t_list_var) = (yyvsp[0].t_list_var); print_rules("functionDeclarationReturns", "idList");}

    break;

  case 77:

    { (yyval.t_list_var) = (yyvsp[-1].t_list_var); print_rules("functionDeclarationArguments", "LPAREN idList RPAREN");}

    break;

  case 78:

    { (yyval.t_list_var) = new ast::exps_t;    print_rules("functionDeclarationArguments", "LPAREN RPAREN");}

    break;

  case 79:

    { (yyval.t_list_var) = new ast::exps_t;    print_rules("functionDeclarationArguments", "Epsilon");}

    break;

  case 80:

    {
                    print_rules("idList", "idList COMMA ID");
                    (yyvsp[-2].t_list_var)->push_back(new ast::SimpleVar((yylsp[0]), symbol::Symbol(*(yyvsp[0].str))));
                    delete (yyvsp[0].str);
                    (yyval.t_list_var) = (yyvsp[-2].t_list_var);
                }

    break;

  case 81:

    {
                    print_rules("idList", "ID");
                    (yyval.t_list_var) = new ast::exps_t;
                    (yyval.t_list_var)->push_back(new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))));
                    delete (yyvsp[0].str);
                }

    break;

  case 82:

    { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "lineEnd");}

    break;

  case 83:

    { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "SEMI");}

    break;

  case 84:

    { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "SEMI EOL");}

    break;

  case 85:

    { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "COMMA");}

    break;

  case 86:

    { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "COMMA EOL");}

    break;

  case 87:

    {
                        print_rules("functionBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 88:

    {
                        print_rules("functionBody", "Epsilon");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty function body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }

    break;

  case 89:

    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("condition", "functionCall");}

    break;

  case 90:

    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("condition", "variable");}

    break;

  case 91:

    {
                      print_rules("comparison", "variable rightComparable");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_op_exp) = (yyvsp[0].t_op_exp);
                    }

    break;

  case 92:

    {
                      print_rules("comparison", "functionCall rightComparable");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_call_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_op_exp) = (yyvsp[0].t_op_exp);
                    }

    break;

  case 93:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *(yyvsp[0].t_exp)); print_rules("rightComparable", "AND variable");}

    break;

  case 94:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "AND functionCall");}

    break;

  case 95:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "AND COLON");}

    break;

  case 96:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *(yyvsp[0].t_exp)); print_rules("rightComparable", "ANDAND variable");}

    break;

  case 97:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "ANDAND functionCall");}

    break;

  case 98:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "ANDAND COLON");}

    break;

  case 99:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *(yyvsp[0].t_exp)); print_rules("rightComparable", "OR variable");}

    break;

  case 100:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "OR functionCall");}

    break;

  case 101:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "OR COLON");}

    break;

  case 102:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *(yyvsp[0].t_exp)); print_rules("rightComparable", "OROR variable");}

    break;

  case 103:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "OROR functionCall");}

    break;

  case 104:

    { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "OROR COLON");}

    break;

  case 105:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *(yyvsp[0].t_exp)); print_rules("rightComparable", "EQ variable");}

    break;

  case 106:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "EQ functionCall");}

    break;

  case 107:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "EQ COLON");}

    break;

  case 108:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *(yyvsp[0].t_exp)); print_rules("rightComparable", "NE variable");}

    break;

  case 109:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "NE functionCall");}

    break;

  case 110:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "NE COLON");}

    break;

  case 111:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *(yyvsp[0].t_exp)); print_rules("rightComparable", "GT variable");}

    break;

  case 112:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "GT functionCall");}

    break;

  case 113:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "GT COLON");}

    break;

  case 114:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *(yyvsp[0].t_exp)); print_rules("rightComparable", "LT variable");}

    break;

  case 115:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "LT functionCall");}

    break;

  case 116:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "LT COLON");}

    break;

  case 117:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *(yyvsp[0].t_exp)); print_rules("rightComparable", "GE variable");}

    break;

  case 118:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "GE functionCall");}

    break;

  case 119:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "GE COLON");}

    break;

  case 120:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *(yyvsp[0].t_exp)); print_rules("rightComparable", "LE variable");}

    break;

  case 121:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "LE functionCall");}

    break;

  case 122:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::le, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "LE COLON");}

    break;

  case 123:

    {
                      print_rules("operation", "rightOperand");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_exp) = (yyvsp[0].t_op_exp);
                    }

    break;

  case 124:

    {
                      print_rules("operation", "functionCall rightOperand");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_call_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_exp) = (yyvsp[0].t_op_exp);
                    }

    break;

  case 125:

    { if ((yyvsp[0].t_exp)->isDoubleExp()) { (yyval.t_exp) = (yyvsp[0].t_exp)->getAs<ast::DoubleExp>()->neg();  (yyvsp[0].t_exp)->setLocation((yyloc));} else { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryMinus, *(yyvsp[0].t_exp)); } print_rules("operation", "MINUS variable");}

    break;

  case 126:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryMinus, *(yyvsp[0].t_call_exp)); print_rules("operation", "MINUS functionCall");}

    break;

  case 127:

    { if ((yyvsp[0].t_exp)->isDoubleExp()) { (yyval.t_exp) = (yyvsp[0].t_exp);} else { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryPlus, *(yyvsp[0].t_exp)); } print_rules("operation", "PLUS variable");}

    break;

  case 128:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryPlus, *(yyvsp[0].t_call_exp)); print_rules("operation", "PLUS functionCall");}

    break;

  case 129:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::power, *(yyvsp[0].t_exp)); print_rules("operation", "variable POWER variable");}

    break;

  case 130:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::power, *(yyvsp[0].t_call_exp)); print_rules("operation", "variable POWER functionCall");}

    break;

  case 131:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::power, *(yyvsp[0].t_exp)); print_rules("operation", "functionCall POWER variable");}

    break;

  case 132:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::power, *(yyvsp[0].t_call_exp)); print_rules("operation", "functionCall POWER functionCall");}

    break;

  case 133:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::dotpower, *(yyvsp[0].t_exp)); print_rules("operation", "variable DOTPOWER variable");}

    break;

  case 134:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::dotpower, *(yyvsp[0].t_call_exp)); print_rules("operation", "variable DOTPOWER functionCall");}

    break;

  case 135:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::dotpower, *(yyvsp[0].t_exp)); print_rules("operation", "functionCall DOTPOWER variable");}

    break;

  case 136:

    { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::dotpower, *(yyvsp[0].t_call_exp)); print_rules("operation", "functionCall DOTPOWER functionCall");}

    break;

  case 137:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_exp), ast::TransposeExp::_Conjugate_); print_rules("operation", "variable QUOTE");}

    break;

  case 138:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_exp), ast::TransposeExp::_NonConjugate_); print_rules("operation", "variable DOTQUOTE");}

    break;

  case 139:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_call_exp), ast::TransposeExp::_Conjugate_); print_rules("operation", "functionCall QUOTE");}

    break;

  case 140:

    { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_call_exp), ast::TransposeExp::_NonConjugate_); print_rules("operation", "functionCall DOTQUOTE");}

    break;

  case 141:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *(yyvsp[0].t_exp)); print_rules("rightOperand", "PLUS variable");}

    break;

  case 142:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "PLUS functionCall");}

    break;

  case 143:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *(yyvsp[0].t_exp)); print_rules("rightOperand", "MINUS variable");}

    break;

  case 144:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "MINUS functionCall");}

    break;

  case 145:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *(yyvsp[0].t_exp)); print_rules("rightOperand", "TIMES variable");}

    break;

  case 146:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "TIMES functionCall");}

    break;

  case 147:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[0].t_exp)); print_rules("rightOperand", "DOTTIMES variable");}

    break;

  case 148:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "DOTTIMES functionCall");}

    break;

  case 149:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[0].t_exp)); print_rules("rightOperand", "KRONTIMES variable");}

    break;

  case 150:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "KRONTIMES functionCall");}

    break;

  case 151:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *(yyvsp[0].t_exp)); print_rules("rightOperand", "CONTROLTIMES variable");}

    break;

  case 152:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "CONTROLTIMES functionCall    ");}

    break;

  case 153:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "RDIVIDE variable");}

    break;

  case 154:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "RDIVIDE functionCall");}

    break;

  case 155:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "DOTRDIVIDE variable");}

    break;

  case 156:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "DOTRDIVIDE functionCall");}

    break;

  case 157:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "KRONRDIVIDE variable");}

    break;

  case 158:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "KRONRDIVIDE functionCall");}

    break;

  case 159:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "CONTROLRDIVIDE variable");}

    break;

  case 160:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "CONTROLRDIVIDE functionCall");}

    break;

  case 161:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "LDIVIDE variable");}

    break;

  case 162:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "LDIVIDE functionCall");}

    break;

  case 163:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "DOTLDIVIDE variable");}

    break;

  case 164:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "DOTLDIVIDE functionCall");}

    break;

  case 165:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "KRONLDIVIDE variable");}

    break;

  case 166:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "KRONLDIVIDE functionCall");}

    break;

  case 167:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "CONTROLLDIVIDE variable");}

    break;

  case 168:

    { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "CONTROLLDIVIDE functionCall");}

    break;

  case 169:

    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("listableBegin", "COLON variable");}

    break;

  case 170:

    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("listableBegin", "COLON functionCall");}

    break;

  case 171:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), *(yyvsp[-2].t_exp), *(yyvsp[0].t_exp), true); print_rules("listableEnd", "listableBegin COLON variable");}

    break;

  case 172:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), *(yyvsp[-2].t_exp), *(yyvsp[0].t_call_exp), true); print_rules("listableEnd", "listableBegin COLON functionCall");}

    break;

  case 173:

    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), *new ast::DoubleExp((yyloc), 1.0), *(yyvsp[0].t_exp)); print_rules("listableEnd", "listableBegin ");}

    break;

  case 174:

    { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[0].t_exp)); print_rules("variable", "NOT variable");}

    break;

  case 175:

    { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[0].t_call_exp)); print_rules("variable", "NOT functionCall");}

    break;

  case 176:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str);print_rules("variable", "variable DOT ID");}

    break;

  case 177:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_simple_var)); print_rules("variable", "variable DOT keywords");}

    break;

  case 178:

    {
                              print_rules("variable", "variable DOT functionCall");
                              (yyvsp[0].t_call_exp)->setName(new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), (yyvsp[0].t_call_exp)->getName()));
                              (yyvsp[0].t_call_exp)->setLocation((yyloc));
                              (yyval.t_exp) = (yyvsp[0].t_call_exp);
}

    break;

  case 179:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str); print_rules("variable", "functionCall DOT ID");}

    break;

  case 180:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_simple_var)); print_rules("variable", "functionCall DOT keywords");}

    break;

  case 181:

    {
    print_rules("variable", "variable listableEnd");
    (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[-1].t_exp), *((yyvsp[0].t_implicit_list)->getStep().clone()), *((yyvsp[0].t_implicit_list)->getEnd().clone()), (yyvsp[0].t_implicit_list)->hasExplicitStep());
    delete((yyvsp[0].t_implicit_list));
}

    break;

  case 182:

    {
    print_rules("variable", "functionCall listableEnd");
    (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[-1].t_call_exp), *((yyvsp[0].t_implicit_list)->getStep().clone()), *((yyvsp[0].t_implicit_list)->getEnd().clone()), (yyvsp[0].t_implicit_list)->hasExplicitStep());
    delete((yyvsp[0].t_implicit_list));
}

    break;

  case 183:

    { (yyval.t_exp) = (yyvsp[0].t_matrix_exp); print_rules("variable", "matrix");}

    break;

  case 184:

    { (yyval.t_exp) = (yyvsp[0].t_cell_exp); print_rules("variable", "cell");}

    break;

  case 185:

    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("variable", "operation");}

    break;

  case 186:

    { (yyval.t_exp) = new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))); delete (yyvsp[0].str);print_rules("variable", "ID");}

    break;

  case 187:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[0].number)); print_rules("variable", (yyvsp[0].number));}

    break;

  case 188:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[0].number)); print_rules("variable", (yyvsp[0].number));}

    break;

  case 189:

    { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[0].number)); print_rules("variable", (yyvsp[0].number));}

    break;

  case 190:

    { (yyval.t_exp) = new ast::StringExp((yyloc), *(yyvsp[0].str)); delete (yyvsp[0].str);print_rules("variable", "STR");}

    break;

  case 191:

    { (yyval.t_exp) = new ast::DollarVar((yyloc)); print_rules("variable", "DOLLAR");}

    break;

  case 192:

    { (yyval.t_exp) = new ast::BoolExp((yyloc), true); print_rules("variable", "BOOLTRUE");}

    break;

  case 193:

    { (yyval.t_exp) = new ast::BoolExp((yyloc), false); print_rules("variable", "BOOLFALSE");}

    break;

  case 194:

    { (yyval.t_exp) = (yyvsp[-1].t_exp); print_rules("variable", "LPAREN variable RPAREN");}

    break;

  case 195:

    { (yyval.t_exp) = new ast::ArrayListExp((yyloc), *(yyvsp[-1].t_list_exp)); print_rules("variable", "LPAREN variableFields RPAREN");}

    break;

  case 196:

    { (yyval.t_exp) = (yyvsp[0].t_op_exp); print_rules("variable", "comparison");}

    break;

  case 197:

    { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_list_exp)); print_rules("variable", "variable LPAREN functionArgs RPAREN");}

    break;

  case 198:

    { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_call_exp), *(yyvsp[-1].t_list_exp)); print_rules("variable", "functionCall LPAREN functionArgs RPAREN");}

    break;

  case 199:

    {
                    print_rules("variableFields", "variableFields COMMA variable");
                      (yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_exp));
                      (yyval.t_list_exp) = (yyvsp[-2].t_list_exp);
                    }

    break;

  case 200:

    {
                    print_rules("variableFields", "variableFields COMMA functionCall");
                      (yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_call_exp));
                      (yyval.t_list_exp) = (yyvsp[-2].t_list_exp);
                    }

    break;

  case 201:

    {
                      print_rules("variableFields", "variable COMMA variable");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_exp));
                    }

    break;

  case 202:

    {
                      print_rules("variableFields", "functionCall COMMA functionCall");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_call_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));
                    }

    break;

  case 203:

    {
                      print_rules("variableFields", "functionCall COMMA variable");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_call_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_exp));
                    }

    break;

  case 204:

    {
                      print_rules("variableFields", "variable COMMA functionCall");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));
}

    break;

  case 205:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("cell", "LBRACE matrixOrCellLines RBRACE");}

    break;

  case 206:

    { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("cell", "variable COMMA functionCall");}

    break;

  case 207:

    {
                                  print_rules("cell", "LBRACE matrixOrCellLines matrixOrCellColumns RBRACE");
                                  (yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-2].t_list_mline));
                                }

    break;

  case 208:

    {
                                  print_rules("cell", "LBRACE EOL matrixOrCellLines matrixOrCellColumns RBRACE");
                                  (yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-2].t_list_mline));
                                }

    break;

  case 209:

    {
                                  print_rules("cell", "LBRACE matrixOrCellColumns RBRACE");
                                  ast::exps_t* tmp = new ast::exps_t;
                                  tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
                                }

    break;

  case 210:

    {
                                  print_rules("cell", "LBRACE EOL matrixOrCellColumns RBRACE");
                                  ast::exps_t* tmp = new ast::exps_t;
                                  tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
                                }

    break;

  case 211:

    { ast::exps_t* tmp = new ast::exps_t;(yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp); print_rules("cell", "LBRACE EOL RBRACE");}

    break;

  case 212:

    { ast::exps_t* tmp = new ast::exps_t;(yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp); print_rules("cell", "LBRACE RBRACE");}

    break;

  case 213:

    {(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("matrix", "LBRACK matrixOrCellLines RBRACK");}

    break;

  case 214:

    {(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("matrix", "LBRACK EOL matrixOrCellLines RBRACK");}

    break;

  case 215:

    {(yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-2].t_list_mline));print_rules("matrix", "LBRACK matrixOrCellLines matrixOrCellColumns RBRACK");}

    break;

  case 216:

    {(yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-2].t_list_mline));print_rules("matrix", "BRACK EOL matrixOrCellLines matrixOrCellColumns RBRACK");}

    break;

  case 217:

    {ast::exps_t* tmp = new ast::exps_t;tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);print_rules("matrix", "LBRACK matrixOrCellColumns RBRACK");}

    break;

  case 218:

    {ast::exps_t* tmp = new ast::exps_t;tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);print_rules("matrix", "LBRACK EOL matrixOrCellColumns RBRACK");}

    break;

  case 219:

    {ast::exps_t* tmp = new ast::exps_t;(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp); print_rules("matrix", "LBRACK EOL RBRACK");}

    break;

  case 220:

    {ast::exps_t* tmp = new ast::exps_t;(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp); print_rules("matrix", "LBRACK RBRACK");}

    break;

  case 221:

    {(yyvsp[-1].t_list_mline)->push_back((yyvsp[0].t_matrixline_exp));(yyval.t_list_mline) = (yyvsp[-1].t_list_mline);print_rules("matrixOrCellLines", "matrixOrCellLines matrixOrCellLine");}

    break;

  case 222:

    {(yyval.t_list_mline) = new ast::exps_t;(yyval.t_list_mline)->push_back((yyvsp[0].t_matrixline_exp));print_rules("matrixOrCellLines", "matrixOrCellLine");}

    break;

  case 223:

    { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "SEMI");}

    break;

  case 224:

    { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "EOL");}

    break;

  case 225:

    { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "matrixOrCellLineBreak EOL");}

    break;

  case 226:

    { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "matrixOrCellLineBreak SEMI");}

    break;

  case 227:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[-1].t_list_exp)); print_rules("matrixOrCellLine", "matrixOrCellColumns matrixOrCellLineBreak ");}

    break;

  case 228:

    { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[-2].t_list_exp)); print_rules("matrixOrCellLine", "matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak");}

    break;

  case 229:

    {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak variable");}

    break;

  case 230:

    {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_call_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak functionCall");}

    break;

  case 231:

    {(yyvsp[-1].t_list_exp)->push_back((yyvsp[0].t_exp));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns variable");}

    break;

  case 232:

    {(yyvsp[-1].t_list_exp)->push_back((yyvsp[0].t_call_exp));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns functionCall");}

    break;

  case 233:

    {(yyvsp[-1].t_list_exp)->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns COMMENT");}

    break;

  case 234:

    {(yyvsp[-2].t_list_exp)->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak COMMENT");}

    break;

  case 235:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_exp));print_rules("matrixOrCellColumns", "variable");}

    break;

  case 236:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));print_rules("matrixOrCellColumns", "functionCall");}

    break;

  case 237:

    {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::CommentExp((yyloc), (yyvsp[0].comment)));print_rules("matrixOrCellColumns", "COMMENT");}

    break;

  case 238:

    { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "matrixOrCellColumnsBreak COMMA");}

    break;

  case 239:

    { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "COMMA");}

    break;

  case 240:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_exp)); print_rules("variableDeclaration", "assignable ASSIGN variable");}

    break;

  case 241:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_call_exp)); print_rules("variableDeclaration", "assignable ASSIGN functionCall");}

    break;

  case 242:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_exp)); print_rules("variableDeclaration", "functionCall ASSIGN variable");}

    break;

  case 243:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_call_exp)); print_rules("variableDeclaration", "functionCall ASSIGN functionCall");}

    break;

  case 244:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *new ast::ColonVar((yylsp[0]))); print_rules("variableDeclaration", "assignable ASSIGN COLON");}

    break;

  case 245:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::ColonVar((yylsp[0]))); print_rules("variableDeclaration", "functionCall ASSIGN COLON");}

    break;

  case 246:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_return_exp)); print_rules("variableDeclaration", "assignable ASSIGN returnControl");}

    break;

  case 247:

    { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_return_exp)); print_rules("variableDeclaration", "functionCall ASSIGN returnControl");}

    break;

  case 248:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str);print_rules("assignable", "variable DOT ID");}

    break;

  case 249:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_simple_var)); print_rules("assignable", "variable DOT keywords");}

    break;

  case 250:

    { (yyvsp[0].t_call_exp)->setName(new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), (yyvsp[0].t_call_exp)->getName()));(yyvsp[0].t_call_exp)->setLocation((yyloc));(yyval.t_exp) = (yyvsp[0].t_call_exp);print_rules("assignable", "variable DOT functionCall");}

    break;

  case 251:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str); print_rules("assignable", "functionCall DOT ID");}

    break;

  case 252:

    { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_simple_var)); print_rules("assignable", "functionCall DOT keywords");}

    break;

  case 253:

    { (yyval.t_exp) = new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))); delete (yyvsp[0].str);print_rules("assignable", "ID");}

    break;

  case 254:

    { (yyval.t_exp) = (yyvsp[0].t_assignlist_exp); print_rules("assignable", "multipleResults");}

    break;

  case 255:

    { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_list_exp)); print_rules("assignable", "ariable LPAREN functionArgs RPAREN");}

    break;

  case 256:

    { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_call_exp), *(yyvsp[-1].t_list_exp)); print_rules("assignable", "functionCall LPAREN functionArgs RPAREN");}

    break;

  case 257:

    { (yyval.t_assignlist_exp) = new ast::AssignListExp((yyloc), *(yyvsp[-1].t_list_exp)); print_rules("multipleResults", "LBRACK matrixOrCellColumns RBRACK");}

    break;

  case 258:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_seq_exp)); print_rules("ifControl", "IF condition then thenBody END");}

    break;

  case 259:

    {
    if ((yyvsp[-1].t_seq_exp) != NULL)
    {
        (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_seq_exp), *(yyvsp[-1].t_seq_exp));
    }
    else
    {
       (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_seq_exp));
    }
    print_rules("ifControl", "IF condition then thenBody else elseBody END");
    }

    break;

  case 260:

    { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-2].t_seq_exp), *(yyvsp[-1].t_seq_exp)); print_rules("ifControl", "IF condition then thenBody elseIfControl END");}

    break;

  case 261:

    {
            print_rules("thenBody", "expressions");
            (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
            (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
            (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                }

    break;

  case 262:

    {
    print_rules("thenBody", "Epsilon");
    ast::exps_t* tmp = new ast::exps_t;
    #ifdef BUILD_DEBUG_AST
    tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty then body")));
    #endif
    (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                }

    break;

  case 263:

    {
                        print_rules("elseBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 264:

    {
                        #ifdef BUILD_DEBUG_AST
                            ast::exps_t* tmp = new ast::exps_t;
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty else body")));
                            (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                        #else
                            (yyval.t_seq_exp) = NULL;
                        #endif
                        print_rules("elseBody", "Epsilon");
                    }

    break;

  case 265:

    { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "SEMI");}

    break;

  case 266:

    { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "SEMI EOL");}

    break;

  case 267:

    { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "COMMA");}

    break;

  case 268:

    { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "COMMA EOL");}

    break;

  case 269:

    { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "EOL");}

    break;

  case 270:

    { /* !! Do Nothing !! */ print_rules("then", "THEN");}

    break;

  case 271:

    { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak THEN");}

    break;

  case 272:

    { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak THEN EOL");}

    break;

  case 273:

    { /* !! Do Nothing !! */ print_rules("then", "THEN ifConditionBreak");}

    break;

  case 274:

    { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak");}

    break;

  case 275:

    { /* !! Do Nothing !! */ print_rules("then", "Epsilon");}

    break;

  case 276:

    { /* !! Do Nothing !! */ print_rules("else", "ELSE");}

    break;

  case 277:

    { /* !! Do Nothing !! */ print_rules("else", "ELSE COMMA");}

    break;

  case 278:

    { /* !! Do Nothing !! */ print_rules("else", "ELSE SEMI");}

    break;

  case 279:

    { /* !! Do Nothing !! */ print_rules("else", "ELSE EOL");}

    break;

  case 280:

    { /* !! Do Nothing !! */ print_rules("else", "ELSE COMMA EOL");}

    break;

  case 281:

    { /* !! Do Nothing !! */ print_rules("else", "ELSE SEMI EOL");}

    break;

  case 282:

    {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));
                                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                    }

    break;

  case 283:

    {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody else elseBody");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        if( (yyvsp[0].t_seq_exp) == NULL)
                                        {
                                            tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-2].t_seq_exp)));
                                        }
                                        else
                                        {
                                            tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-2].t_seq_exp), *(yyvsp[0].t_seq_exp)));
                                        }
                                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);

                                    }

    break;

  case 284:

    {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody elseIfControl");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_seq_exp), *(yyvsp[0].t_seq_exp)));
                                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                    }

    break;

  case 285:

    { (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_list_case)); print_rules("selectControl", "select selectable selectConditionBreak casesControl END");}

    break;

  case 286:

    {
                                        if((yyvsp[-1].t_seq_exp) == NULL)
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_list_case));
                                        }
                                        else
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_list_case), *(yyvsp[-1].t_seq_exp));
                                        }
                                        print_rules("selectControl", "select selectable selectConditionBreak casesControl defaultCase elseBody END");
                                    }

    break;

  case 287:

    { (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-1].t_list_case)); delete (yyvsp[-3].comment);print_rules("selectControl", "select selectable COMMENT selectConditionBreak casesControl END");}

    break;

  case 288:

    {
                                        if((yyvsp[-1].t_seq_exp) == NULL)
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-6].t_exp), *(yyvsp[-3].t_list_case));
                                        }
                                        else
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-6].t_exp), *(yyvsp[-3].t_list_case), *(yyvsp[-1].t_seq_exp));
                                        }
                                        delete (yyvsp[-5].comment);
                                        print_rules("selectControl", "select selectable COMMENT selectConditionBreak casesControl defaultCase elseBody END");
                                    }

    break;

  case 289:

    { /* !! Do Nothing !! */ print_rules("select", "SELECT");}

    break;

  case 290:

    { /* !! Do Nothing !! */ print_rules("select", "SWITCH");}

    break;

  case 291:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "else");}

    break;

  case 292:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE");}

    break;

  case 293:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE COMMA");}

    break;

  case 294:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE SEMI");}

    break;

  case 295:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE EOL");}

    break;

  case 296:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE COMMA EOL");}

    break;

  case 297:

    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE SEMI EOL");}

    break;

  case 298:

    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("selectable", "variable");}

    break;

  case 299:

    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("selectable", "functionCall");}

    break;

  case 300:

    { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "EOL");}

    break;

  case 301:

    { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "COMMA EOL");}

    break;

  case 302:

    { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "SEMI EOL");}

    break;

  case 303:

    { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "COMMA");}

    break;

  case 304:

    { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "SEMI");}

    break;

  case 305:

    {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "CASE variable caseControlBreak caseBody");}

    break;

  case 306:

    {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "CASE functionCall caseControlBreak caseBody");}

    break;

  case 307:

    {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "comments CASE variable caseControlBreak caseBody");}

    break;

  case 308:

    {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "comments CASE functionCall caseControlBreak caseBody");}

    break;

  case 309:

    {(yyvsp[-4].t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));(yyval.t_list_case) = (yyvsp[-4].t_list_case);print_rules("casesControl", "casesControl CASE variable caseControlBreak caseBody");}

    break;

  case 310:

    {(yyvsp[-4].t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_seq_exp)));(yyval.t_list_case) = (yyvsp[-4].t_list_case);print_rules("casesControl", "casesControl CASE functionCall caseControlBreak caseBody");}

    break;

  case 311:

    {
                    print_rules("caseBody", "expressions");
                    (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                    (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                    (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                }

    break;

  case 312:

    {
                    print_rules("caseBody", "Epsilon");
                    ast::exps_t* tmp = new ast::exps_t;
                    #ifdef BUILD_DEBUG_AST
                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty case body")));
                    #endif
                    (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                }

    break;

  case 313:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN");}

    break;

  case 314:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "COMMA");}

    break;

  case 315:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "SEMI");}

    break;

  case 316:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "EOL");}

    break;

  case 317:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN EOL");}

    break;

  case 318:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "COMMA EOL");}

    break;

  case 319:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "SEMI EOL");}

    break;

  case 320:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN COMMA");}

    break;

  case 321:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN COMMA EOL");}

    break;

  case 322:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN SEMI");}

    break;

  case 323:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN SEMI EOL");}

    break;

  case 324:

    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "Epsilon");}

    break;

  case 325:

    { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[-4]), symbol::Symbol(*(yyvsp[-5].str)), *(yyvsp[-3].t_exp)), *(yyvsp[-1].t_seq_exp)); delete (yyvsp[-5].str);print_rules("forControl", "FOR ID ASSIGN forIterator forConditionBreak forBody END    ");}

    break;

  case 326:

    { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[-5]), symbol::Symbol(*(yyvsp[-6].str)), *(yyvsp[-4].t_exp)), *(yyvsp[-1].t_seq_exp)); delete (yyvsp[-6].str);print_rules("forControl", "FOR LPAREN ID ASSIGN forIterator RPAREN forConditionBreak forBody END");}

    break;

  case 327:

    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("forIterator", "functionCall");}

    break;

  case 328:

    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("forIterator", "variable");}

    break;

  case 329:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "EOL");}

    break;

  case 330:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "SEMI");}

    break;

  case 331:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "SEMI EOL");}

    break;

  case 332:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "COMMA");}

    break;

  case 333:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "COMMA EOL");}

    break;

  case 334:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "DO");}

    break;

  case 335:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "DO EOL");}

    break;

  case 336:

    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "Epsilon");}

    break;

  case 337:

    {
                    print_rules("forBody", "expressions");
                    (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                    (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                    (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                }

    break;

  case 338:

    {
                    print_rules("forBody", "Epsilon");
                    ast::exps_t* tmp = new ast::exps_t;
                    #ifdef BUILD_DEBUG_AST
                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty for body")));
                    #endif
                    (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                }

    break;

  case 339:

    { (yyval.t_while_exp) = new ast::WhileExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_seq_exp)); print_rules("whileControl", "WHILE condition whileConditionBreak whileBody END");}

    break;

  case 340:

    {
                        print_rules("whileBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 341:

    {
                        print_rules("whileBody", "Epsilon");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty while body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }

    break;

  case 342:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "COMMA");}

    break;

  case 343:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI");}

    break;

  case 344:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO");}

    break;

  case 345:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO COMMA");}

    break;

  case 346:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO SEMI");}

    break;

  case 347:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN");}

    break;

  case 348:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN COMMA");}

    break;

  case 349:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN SEMI");}

    break;

  case 350:

    { delete (yyvsp[-1].comment); print_rules("whileConditionBreak", "COMMENT EOL");}

    break;

  case 351:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "EOL");}

    break;

  case 352:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "COMMA EOL");}

    break;

  case 353:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI EOL");}

    break;

  case 354:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI EOL");}

    break;

  case 355:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO COMMA EOL");}

    break;

  case 356:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO SEMI EOL");}

    break;

  case 357:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN EOL");}

    break;

  case 358:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN COMMA EOL");}

    break;

  case 359:

    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN SEMI EOL");}

    break;

  case 360:

    { (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[-3].t_seq_exp), *(yyvsp[-1].t_seq_exp)); print_rules("tryControl", "TRY catchBody CATCH catchBody END");}

    break;

  case 361:

    {
                                        print_rules("tryControl", "TRY catchBody END");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        #ifdef BUILD_DEBUG_AST
                                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty catch body")));
                                        #endif
                                        (yyval.t_try_exp) = new ast::TryCatchExp((yyloc), *(yyvsp[-1].t_seq_exp), *new ast::SeqExp((yyloc), *tmp));
                                    }

    break;

  case 362:

    {
                        print_rules("catchBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 363:

    {
                        print_rules("catchBody", "EOL expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 364:

    {
                        print_rules("catchBody", "SEMI expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 365:

    {
                        print_rules("catchBody", "COMMA expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }

    break;

  case 366:

    {
                        print_rules("catchBody", "EOL");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty catch body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }

    break;

  case 367:

    {
                        print_rules("catchBody", "Epsilon");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty catch body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }

    break;

  case 368:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc)); print_rules("returnControl", "RETURN");}

    break;

  case 369:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc), (yyvsp[0].t_exp)); print_rules("returnControl", "RETURN variable");}

    break;

  case 370:

    { (yyval.t_return_exp) = new ast::ReturnExp((yyloc), (yyvsp[0].t_call_exp)); print_rules("returnControl", "RETURN functionCall");}

    break;

  case 371:

    { delete (yyvsp[-1].comment); print_rules("comments", "COMMENT EOL");}

    break;

  case 372:

    { delete (yyvsp[-1].comment); print_rules("comments", "comments COMMENT EOL");}

    break;

  case 373:

    { print_rules("lineEnd", "EOL");}

    break;

  case 374:

    { delete (yyvsp[-1].comment); print_rules("lineEnd", "COMMENT EOL");}

    break;

  case 375:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"if"));           print_rules("keywords", "IF");}

    break;

  case 376:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"then"));         print_rules("keywords", "THEN");}

    break;

  case 377:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"else"));         print_rules("keywords", "ELSE");}

    break;

  case 378:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"elseif"));       print_rules("keywords", "ELSEIF");}

    break;

  case 379:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"end"));          print_rules("keywords", "END");}

    break;

  case 380:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"select"));       print_rules("keywords", "SELECT");}

    break;

  case 381:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"switch"));       print_rules("keywords", "SWITCH");}

    break;

  case 382:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"otherwise"));    print_rules("keywords", "OTHERWISE");}

    break;

  case 383:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"case"));         print_rules("keywords", "CASE");}

    break;

  case 384:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"function"));     print_rules("keywords", "FUNCTION");}

    break;

  case 385:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"endfunction"));  print_rules("keywords", "ENDFUNCTION");}

    break;

  case 386:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"for"));          print_rules("keywords", "FOR");}

    break;

  case 387:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"while"));        print_rules("keywords", "WHILE");}

    break;

  case 388:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"do"));           print_rules("keywords", "DO");}

    break;

  case 389:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"break"));        print_rules("keywords", "BREAK");}

    break;

  case 390:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"try"));          print_rules("keywords", "TRY");}

    break;

  case 391:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"catch"));        print_rules("keywords", "CATCH");}

    break;

  case 392:

    { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"return"));       print_rules("keywords", "RETURN");}

    break;



      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}



bool endsWith(const std::string & str, const std::string & end)
{
    if (end.size() > str.size())
    {
    return false;
    }

    return std::equal(end.rbegin(), end.rend(), str.rbegin());
}

void yyerror(std::string msg) {
    if (!endsWith(msg, "FLEX_ERROR") && !ParserSingleInstance::isStrictMode()
       || ParserSingleInstance::getExitStatus() == Parser::Succeded)
    {
        wchar_t* pstMsg = to_wide_string(msg.c_str());
        ParserSingleInstance::PrintError(pstMsg);
        ParserSingleInstance::setExitStatus(Parser::Failed);
    delete ParserSingleInstance::getTree();
        FREE(pstMsg);
    }
}

