
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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
     ANDAND = 298,
     OR = 299,
     OROR = 300,
     ASSIGN = 301,
     IF = 302,
     THEN = 303,
     ELSE = 304,
     ELSEIF = 305,
     END = 306,
     SELECT = 307,
     SWITCH = 308,
     CASE = 309,
     OTHERWISE = 310,
     FUNCTION = 311,
     ENDFUNCTION = 312,
     HIDDENFUNCTION = 313,
     HIDDEN = 314,
     FOR = 315,
     WHILE = 316,
     DO = 317,
     BREAK = 318,
     TRY = 319,
     CATCH = 320,
     RETURN = 321,
     STR = 322,
     ID = 323,
     VARINT = 324,
     VARFLOAT = 325,
     NUM = 326,
     PATH = 327,
     COMMENT = 328,
     BLOCKCOMMENT = 329,
     TOPLEVEL = 330,
     HIGHLEVEL = 331,
     UPLEVEL = 332,
     LISTABLE = 333,
     FUNCTIONCALL = 334
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
#define ANDAND 298
#define OR 299
#define OROR 300
#define ASSIGN 301
#define IF 302
#define THEN 303
#define ELSE 304
#define ELSEIF 305
#define END 306
#define SELECT 307
#define SWITCH 308
#define CASE 309
#define OTHERWISE 310
#define FUNCTION 311
#define ENDFUNCTION 312
#define HIDDENFUNCTION 313
#define HIDDEN 314
#define FOR 315
#define WHILE 316
#define DO 317
#define BREAK 318
#define TRY 319
#define CATCH 320
#define RETURN 321
#define STR 322
#define ID 323
#define VARINT 324
#define VARFLOAT 325
#define NUM 326
#define PATH 327
#define COMMENT 328
#define BLOCKCOMMENT 329
#define TOPLEVEL 330
#define HIGHLEVEL 331
#define UPLEVEL 332
#define LISTABLE 333
#define FUNCTIONCALL 334




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
  ast::SelectExp*	t_select_exp;
  ast::CaseExp*		t_case_exp;
  ast::cases_t*		t_list_case;

  ast::CallExp*		t_call_exp;

  ast::MathExp*		t_math_exp;

  ast::OpExp*		t_op_exp;
  ast::OpExp::Oper	t_op_exp_oper;
  ast::LogicalOpExp::Oper	t_lop_exp_oper;

  ast::AssignExp*	t_assign_exp;

  ast::StringExp*	t_string_exp;

  ast::ListExp*		t_implicit_list;

  ast::MatrixExp*	t_matrix_exp;
  ast::MatrixLineExp*	t_matrixline_exp;
  std::list<ast::MatrixLineExp *>* \
			t_list_mline;

  ast::CellExp*		t_cell_exp;

  ast::CellCallExp*	t_cell_call_exp;

  ast::FunctionDec*	t_function_dec;

  ast::ArrayListExp*	t_arraylist_exp;
  ast::AssignListExp*	t_assignlist_exp;
  ast::ArrayListVar*	t_arraylist_var;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

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

extern YYLTYPE yylloc;

