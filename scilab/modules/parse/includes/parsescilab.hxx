/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C

      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
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
    HIDDENFUNCTION = 315,
    HIDDEN = 316,
    FOR = 317,
    WHILE = 318,
    DO = 319,
    BREAK = 320,
    CONTINUE = 321,
    TRY = 322,
    CATCH = 323,
    RETURN = 324,
    STR = 325,
    ID = 326,
    VARINT = 327,
    VARFLOAT = 328,
    NUM = 329,
    PATH = 330,
    COMMENT = 331,
    BLOCKCOMMENT = 332,
    TOPLEVEL = 333,
    HIGHLEVEL = 334,
    UPLEVEL = 335,
    LISTABLE = 336,
    CONTROLBREAK = 337,
    FUNCTIONCALL = 338
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
#define HIDDENFUNCTION 315
#define HIDDEN 316
#define FOR 317
#define WHILE 318
#define DO 319
#define BREAK 320
#define CONTINUE 321
#define TRY 322
#define CATCH 323
#define RETURN 324
#define STR 325
#define ID 326
#define VARINT 327
#define VARFLOAT 328
#define NUM 329
#define PATH 330
#define COMMENT 331
#define BLOCKCOMMENT 332
#define TOPLEVEL 333
#define HIGHLEVEL 334
#define UPLEVEL 335
#define LISTABLE 336
#define CONTROLBREAK 337
#define FUNCTIONCALL 338



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    /* Tokens. */
    double                      number;
    std::wstring*               str;
    std::wstring*               path;
    std::wstring*               comment;

    LineBreakStr*               mute;

    ast::vars_t*                t_list_var;
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
    ast::cases_t*               t_list_case;

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
    std::list<ast::MatrixLineExp *>*            \
    t_list_mline;

    ast::CellExp*               t_cell_exp;

    ast::CellCallExp*           t_cell_call_exp;

    ast::FunctionDec*           t_function_dec;

    ast::ArrayListExp*          t_arraylist_exp;
    ast::AssignListExp*         t_assignlist_exp;
    ast::ArrayListVar*          t_arraylist_var;

    ast::SimpleVar*             t_simple_var;



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

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
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

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
