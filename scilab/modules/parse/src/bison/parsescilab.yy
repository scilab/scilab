%{ // -*- C++ -*-
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

%}

//%pure-parser
%define "location_type" "Location"
%locations
%verbose
%debug
%defines

%union
{
  /* Tokens. */
  double		number;
  std::string*		str;
  std::string*		path;
  std::string*		comment;

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

  ast::FunctionDec*	t_function_dec;
  ast::ArrayListExp*	t_arraylist_exp;
  ast::ArrayListVar*	t_arraylist_var;
}

%token YYEOF    0	"end of file"

%token DOTS		"line break"

%token EOL		"end of line"
%token SPACES		"spaces"

%token BOOLTRUE		"%t or %T"
%token BOOLFALSE	"%f or %F"

%token QUOTE		"'"
%token NOT		"~ or @"
%token DOLLAR		"$"
%token COMMA		","
%token COLON		":"
%token SEMI		";"
%token LPAREN		"("
%token RPAREN		")"
%token LBRACK		"["
%token RBRACK		"]"
%token LBRACE		"{"
%token RBRACE		"}"
%token DOT		"."

%token DOTQUOTE		".'"

%token PLUS		"+"
%token DOTPLUS		".+"
%token MINUS		"-"
%token DOTMINUS		".-"
%token TIMES		"*"
%token DOTTIMES		".*"
%token KRONTIMES	".*."
%token DIVIDE		"/"
%token DOTDIVIDE	"./"
%token CONTROLDIVIDE	"/."
%token KRONDIVIDE	"./."
%token RDIVIDE		"\\"
%token DOTRDIVIDE	".\\"
%token KRONRDIVIDE	".\\."

%token POWER		"** or ^"
%token DOTPOWER		".^"

%token EQ		"=="
%token NE		"<> or ~="
%token LT		"<"
%token LE		"<="
%token GT		">"
%token GE		">="
%token AND		"&"
%token OR		"|"
%token ASSIGN		"="

%token IF		"if"
%token THEN		"then"
%token ELSE		"else"
%token ELSEIF		"elseif"
%token END		"end"

%token SELECT		"select"
%token CASE		"case"

%token FUNCTION		"function"
%token ENDFUNCTION	"endfunction"

%token FOR		"for"

%token WHILE		"while"
%token DO		"do"
%token BREAK		"break"

%token TRY		"try"
%token CATCH		"catch"
%token RETURN		"return"

%token <str>		STR	"string"
%token <str>		ID	"identifier"
%token <number>		VARINT	"integer"
%token <number>		VARFLOAT"float"
%token <number>		NUM	"number"

%token <path>		PATH	"path"

%token <comment>	COMMENT "line comment"
%token <comment>	BLOCKCOMMENT "block comment"

%type <t_seq_exp>	expressions
%type <t_list_exp>	recursiveExpression

%type <t_exp>		variable
%type <t_arraylist_exp>	variableFields
%type <t_exp>		expression

%type <t_op_exp>	comparison
%type <t_exp>		comparable
%type <t_exp>		operation
%type <t_op_exp>	rightOperand
%type <t_op_exp_oper>	comparators

 // IF Control
%type <t_if_exp>	ifControl
%type <t_exp>		condition
%type <t_seq_exp>	thenBody
%type <t_seq_exp>	elseBody
%type <t_seq_exp>	elseIfControl

 // WHILE Control
%type <t_while_exp>	whileControl
%type <t_seq_exp>	whileBody

 // FOR Control
%type <t_for_exp>	forControl
%type <t_exp>		forIterator
%type <t_seq_exp>	forBody

 // RETURN Control
%type <t_return_exp>	returnControl

 // TRY Control
%type<t_try_exp>	tryControl

 // Implicit Function Call
%type <t_call_exp>	implicitFunctionCall
%type <t_string_exp>	implicitCallable

 // Function Call
%type <t_exp>		functionCall
%type <t_call_exp>	specificFunctionCall
%type <t_call_exp>	recursiveFunctionCall
%type <t_call_exp>	simpleFunctionCall
%type <t_list_exp>	functionArgs
%type <t_seq_exp>	functionBody

 // Function Declaration
%type <t_function_dec>	functionDeclaration
%type <t_list_var>	functionDeclarationReturns
%type <t_list_var>	functionDeclarationArguments
%type <t_list_var>	idList

 // Variable Declaration
%type <t_assign_exp>	variableDeclaration

 // Implicit List
%type <t_implicit_list> listableEnd
%type <t_exp>		listableBegin

 // Matrix
%type<t_matrix_exp>		matrix
%type<t_list_exp>	matrixColumns
%type<t_matrixline_exp>	matrixLine
%type<t_list_mline>	matrixLines

%nonassoc TOPLEVEL
%nonassoc HIGHLEVEL
%nonassoc UPLEVEL

%nonassoc LISTABLE

%nonassoc FUNCTIONCALL
%nonassoc BOOLTRUE BOOLFALSE
%nonassoc LPAREN

%left OR
%left AND

%left COLON
%nonassoc EQ NE LT LE GT GE
%left MINUS DOTMINUS PLUS DOTPLUS
%left TIMES DOTTIMES KRONTIMES DIVIDE DOTDIVIDE KRONDIVIDE RDIVIDE DOTRDIVIDE KRONRDIVIDE CONTROLDIVIDE
%left POWER DOTPOWER

%left QUOTE DOTQUOTE

%left DOT

%left NOT

%start program


%%
/*
** -*- PROGRAM -*-
*/
/* Root of the Abstract Syntax Tree */
program:
expressions					{ Parser::getInstance()->setTree($1); }
| EOL expressions				{ Parser::getInstance()->setTree($2); }
;

/*
** -*- EXPRESSIONS -*-
*/
/* List of expression or single instruction */
expressions :
recursiveExpression				{
						  $$ = new ast::SeqExp(@$, *$1);
						}
| recursiveExpression expression		{
						  $1->push_back($2);
						  $$ = new ast::SeqExp(@$, *$1);
						}
| recursiveExpression expression COMMENT	{
						  $1->push_back($2);
						  $1->push_back(new ast::CommentExp(@3, $3));
						  $$ = new ast::SeqExp(@$, *$1);
						}
| expression					{
						  ast::exps_t *tmp = new ast::exps_t;
						  tmp->push_front($1);
						  $$ = new ast::SeqExp(@$, *tmp);
						}
| expression COMMENT				{
						  ast::exps_t *tmp = new ast::exps_t;
						  tmp->push_front(new ast::CommentExp(@2, $2));
						  tmp->push_front($1);
						  $$ = new ast::SeqExp(@$, *tmp);
						}
;

/*
** -*- RECURSIVE EXPRESSION -*-
*/
/* List of instructions. _MUST_BE_ left recursive Rule */
recursiveExpression :
recursiveExpression expression expressionLineBreak	{
							  $1->push_back($2);
							  $$ = $1;
							}
| recursiveExpression expression COMMENT expressionLineBreak {
							  $1->push_back($2);
							  $1->push_back(new ast::CommentExp(@3, $3));
							  $$ = $1;
							}
| expression COMMENT expressionLineBreak		{
							  ast::exps_t *tmp = new ast::exps_t;
							  tmp->push_front(new ast::CommentExp(@2, $2));
							  tmp->push_front($1);
							  $$ = tmp;
							}
| expression expressionLineBreak			{
							  ast::exps_t *tmp = new ast::exps_t;
							  tmp->push_front($1);
							  $$ = tmp;
							}
;

/*
** -*- EXPRESSION LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is the end of an instruction. */
expressionLineBreak :
SEMI						{ /* !! Do Nothing !! */ }
| COMMA						{ /* !! Do Nothing !! */ }
| EOL						{ /* !! Do Nothing !! */ }
| expressionLineBreak SEMI			{ /* !! Do Nothing !! */ }
| expressionLineBreak COMMA			{ /* !! Do Nothing !! */ }
| expressionLineBreak EOL			{ /* !! Do Nothing !! */ }
;

/*
** -*- EXPRESSION -*-
*/
/* Expression or Instruction : quite similar. */
expression :
functionDeclaration				{ $$ = $1; }
| functionCall			%prec TOPLEVEL	{ $$ = $1; }
| variableDeclaration				{ $$ = $1; }
| ifControl					{ $$ = $1; }
| selectControl					{ $$ = new ast::CommentExp(@$, new std::string("!! FIXME : Select Control Sequence !!")); }
| forControl					{ $$ = $1; }
| whileControl					{ $$ = $1; }
| tryControl					{ $$ = $1; }
| variable			%prec TOPLEVEL	{ $$ = $1; }
| implicitFunctionCall		%prec TOPLEVEL	{ $$ = $1; }
| BREAK						{ $$ = new ast::BreakExp(@$); }
| returnControl					{ $$ = $1; }
| COMMENT					{ $$ = new ast::CommentExp(@$, $1); }
;

/*
** -*- IMPLICIT FUNCTION CALL -*-
*/
/* FIXME : Must be improved */
/* Bash-like : foo bar titi <=> foo('bar', 'titi'). */
implicitFunctionCall :
/* FIXME : Add arguments to call */
implicitFunctionCall implicitCallable		{
						  $1->args_get().push_back($2);
						  $$ = $1;
						}
| ID implicitCallable				{
						  ast::exps_t *tmp = new ast::exps_t;
						  tmp->push_front($2);
						  $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, *new symbol::Symbol(*$1)), *tmp);
						}
;

/*
** -*- IMPLICIT CALLABLE -*-
*/
/* FIXME : Must be improved */
/* Bash-like : foo bar titi <=> foo('bar', 'titi').
** Describe 'bar', 'titi' that can be more complex.
*/
implicitCallable :
ID						{ $$ = new ast::StringExp(@$, *$1); }
| VARINT						{
						  std::stringstream tmp;
						  tmp << $1;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| NUM						{
						  std::stringstream tmp;
						  tmp << $1;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| VARFLOAT					{
						  std::stringstream tmp;
						  tmp << $1;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| STR						{ $$ = new ast::StringExp(@$, *$1); }
| DOLLAR					{ $$ = new ast::StringExp(@$, *new std::string("$")); }
| BOOLTRUE					{ $$ = new ast::StringExp(@$, *new std::string("%t")); }
| BOOLFALSE					{ $$ = new ast::StringExp(@$, *new std::string("%f")); }
| implicitCallable DOT ID			{
						  std::stringstream tmp;
						  tmp << $1->value_get() << "." << *$3;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| implicitCallable DOT functionCall		{ $$ = new ast::StringExp(@$, *new std::string("!! FIXME : implicitCallable implicitCallable DOT functionCall !!")); }
| simpleFunctionCall				{ $$ = new ast::StringExp(@$, *new std::string("!! FIXME : implicitCallable simpleFunctionCall !!")); }
| implicitCallable rightOperand			{ $$ = new ast::StringExp(@$, *new std::string("!! FIXME : implicitCallable implicitCallable rightOperand !!")); }
| PATH						{ $$ = new ast::StringExp(@$, *$1); }
;

/*
** -*- FUNCTION CALL -*-
*/
/* How to call a function */
functionCall :
simpleFunctionCall		%prec FUNCTIONCALL	{ $$ = $1; }
| specificFunctionCall		%prec FUNCTIONCALL	{ $$ = $1; }
| recursiveFunctionCall		%prec FUNCTIONCALL	{ $$ = $1; }
| LPAREN functionCall RPAREN	%prec FUNCTIONCALL	{ $$ = $2; }
| NOT functionCall					{ $$ = new ast::NotExp(@$, *$2); }
;

/*
** -*- SPECIFIC FUNCTION CALL -*-
*/
/* To manage %t(a, b) and %f(a, b) */
specificFunctionCall :
BOOLTRUE LPAREN functionArgs RPAREN			{ $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, *new symbol::Symbol("%t")), *$3); }
| BOOLFALSE LPAREN functionArgs RPAREN			{ $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, *new symbol::Symbol("%f")), *$3); }
;

/*
** -*- SIMPLE FUNCTION CALL -*-
*/
/* Usual way to call functions foo(arg1, arg2, arg3) */
simpleFunctionCall :
ID LPAREN functionArgs RPAREN				{ $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, *new symbol::Symbol(*$1)), *$3); }
;

/*
** -*- RECURSIVE FUNCTION CALL -*-
*/
/* To manage foo(a)(b)(c) <=> ((foo(a))(b))(c)*/
recursiveFunctionCall :
simpleFunctionCall LPAREN functionArgs RPAREN		{ $$ = new ast::CallExp(@$, *$1, *$3); }
| recursiveFunctionCall LPAREN functionArgs RPAREN	{ $$ = new ast::CallExp(@$, *$1, *$3); }
;

/*
** -*- FUNCTION ARGS -*-
*/
/* What can be use in a function call */
functionArgs :
variable			{
				  $$ = new ast::exps_t;
				  $$->push_front($1);
				}
| functionCall			{
				  $$ = new ast::exps_t;
				  $$->push_front($1);
				}
| COLON				{
				  $$ = new ast::exps_t;
				  $$->push_front(new ast::ColonVar(@$));
				}
| variableDeclaration		{
				  $$ = new ast::exps_t;
				  $$->push_front($1);
				}
| /* Epsilon */			{
				  $$ = new ast::exps_t;
				}
| functionArgs COMMA variable	{
				  $1->push_back($3);
				  $$ = $1;
				}
| functionArgs COMMA functionCall {
				  $1->push_back($3);
				  $$ = $1;
				}
| functionArgs COMMA COLON	{
				  $1->push_back(new ast::ColonVar(@$));
			          $$ = $1;
				}
| functionArgs COMMA variableDeclaration {
				  $1->push_back($3);
				  $$ = $1;
				}
| functionArgs COMMA		{
				  $$ = $1;
				}
;

/*
** -*- FUNCTION DECLARATION -*-
*/
/* How to declare a function */
functionDeclaration :
FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar(@2, *new symbol::Symbol(*$2)));
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$4),
							    *new ast::ArrayListVar(@5, *$5),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$7);
				}
| FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3 ,*$3),
							    *$9);
				}
| FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *new ast::vars_t),
							    *$8);
				}
| FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$2),
							    *new ast::ArrayListVar(@3, *$3),
							    *new ast::ArrayListVar(@$, *new ast::vars_t),
							    *$5);
				}
;

/*
** -*- FUNCTION DECLARATION RETURNS -*-
*/
/* Simple Forward to idList */
functionDeclarationReturns :
idList				{ $$ = $1; }
;

/*
** -*- FUNCTION DECLARATION ARGUMENTS -*-
*/
/* Arguments passed to a function in it's declaration. */
functionDeclarationArguments :
LPAREN idList RPAREN		{ $$ = $2; }
| LPAREN RPAREN			{ $$ = new ast::vars_t;	}
| /* Epsilon */			{ $$ = new ast::vars_t;	}
;

/*
** -*- ID LIST -*-
*/
/* ID (,ID)* */
idList:
idList COMMA ID			{
				  $1->push_back(new ast::SimpleVar(@3, *new symbol::Symbol(*$3)));
				  $$ = $1;
				}
| ID				{
				  $$ = new ast::vars_t;
				  $$->push_front(new ast::SimpleVar(@$, *new symbol::Symbol(*$1)));
				}
;

/*
** -*- FUNCTION DECLARATION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'function' prototype ending */
functionDeclarationBreak :
lineEnd				{ /* !! Do Nothing !! */ }
| SEMI				{ /* !! Do Nothing !! */ }
| SEMI EOL			{ /* !! Do Nothing !! */ }
| COMMA				{ /* !! Do Nothing !! */ }
| COMMA EOL			{ /* !! Do Nothing !! */ }
;

/*
** -*- FUNCTION BODY -*-
*/
/* What may content a function */
functionBody :
expressions			{ $$ = $1; }
| /* Epsilon */			{
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp(@$, new std::string("Empty function body")));
				  #endif
				  $$ = new ast::SeqExp(@$, *tmp);
				}
;

/*
** -*- CONDITION -*-
*/
/* Condition for tests in Control Loop */
condition :
functionCall	%prec HIGHLEVEL		{ $$ = $1; }
| variable	%prec HIGHLEVEL		{ $$ = $1; }
;

/*
** -*- COMPARISON -*-
*/
/* a way to compare two expressions */
comparison :
variable comparators comparable			{ $$ = new ast::OpExp(@$, *$1, $2, *$3); }
| functionCall comparators comparable		{ $$ = new ast::OpExp(@$, *$1, $2, *$3); }
;

/*
** -*- COMPARABLE -*-
*/
/* what can be evolved in a comparison */
comparable :
variable	%prec HIGHLEVEL		{ $$ = $1; }
| functionCall	%prec HIGHLEVEL		{ $$ = $1; }
| COLON					{ $$ = new ast::ColonVar(@$); }
;

/*
** -*- OPERATIONS -*-
*/
/* Operations */
operation :
variable rightOperand			{ $$ = new ast::OpExp(@$, *$1, $2->oper_get(), $2->right_get()); }
| functionCall rightOperand		{ $$ = new ast::OpExp(@$, *$1, $2->oper_get(), $2->right_get()); }
| MINUS variable			{ $$ = new ast::OpExp(@$, *new ast::DoubleExp(@$, 0.0), ast::OpExp::minus, *$2); }
| MINUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::DoubleExp(@$, 0.0), ast::OpExp::minus, *$2); }
| PLUS variable				{ $$ = $2; }
| PLUS functionCall			{ $$ = $2; }
| variable POWER variable		{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| variable POWER functionCall		{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| functionCall POWER variable		{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| functionCall POWER functionCall	{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| variable DOTPOWER variable		{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| variable DOTPOWER functionCall	{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| functionCall DOTPOWER variable	{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| functionCall DOTPOWER functionCall	{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| variable QUOTE			{ $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_Conjugate_); }
| variable DOTQUOTE			{ $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_NonConjugate_); }
| functionCall QUOTE			{ $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_Conjugate_); }
| functionCall DOTQUOTE			{ $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_NonConjugate_); }
;

/*
** -*- RIGHT OPERAND -*-
*/
/* rightOperand for operation */
rightOperand :
/*   '+'   '.+'   '.+.'?   */
PLUS variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::plus, *$2); }
| PLUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::plus, *$2); }
| DOTPLUS variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotplus, *$2); }
| DOTPLUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotplus, *$2); }
/*   '-'   '.-'   '.-.'?   */
| MINUS variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::minus, *$2); }
| MINUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::minus, *$2); }
| DOTMINUS variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotminus, *$2); }
| DOTMINUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotminus, *$2); }
/*   '*'   '.*'   '.*.'   */
| TIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::times, *$2); }
| TIMES functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::times, *$2); }
| DOTTIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dottimes, *$2); }
| DOTTIMES functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dottimes, *$2); }
| KRONTIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::krontimes, *$2); }
| KRONTIMES functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::krontimes, *$2); }
/*   '/'   './'   './.'  '/.' */
| DIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::divide, *$2); }
| DIVIDE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::divide, *$2); }
| DOTDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotdivide, *$2); }
| DOTDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotdivide, *$2); }
| KRONDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::krondivide, *$2); }
| KRONDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::krondivide, *$2); }
| CONTROLDIVIDE variable		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::controldivide, *$2); }
| CONTROLDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::controldivide, *$2); }
/*   '\'   '.\'   '.\.'?   */
| RDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::rdivide, *$2); }
| RDIVIDE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::rdivide, *$2); }
| DOTRDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotrdivide, *$2); }
| DOTRDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::dotrdivide, *$2); }
| KRONRDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::kronrdivide, *$2); }
| KRONRDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::kronrdivide, *$2); }
;

/*
** -*- LISTABLE BEGIN -*-
*/
/* May have no stride in the list, assume it is 1. */
listableBegin :
COLON variable				{ $$ = $2; }
| COLON functionCall			{ $$ = $2; }
;

/*
** -*- LISTABLE END -*-
*/
/* Stride parameter or not. */
listableEnd :
listableBegin COLON variable		{ $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), *$1, *$3); }
| listableBegin COLON functionCall	{ $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), *$1, *$3); }
| listableBegin %prec LISTABLE		{ $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), *new ast::DoubleExp(@$, 1.0), *$1); }
;

/*
** -*- VARIABLE -*-
*/
/* Variables */
variable :
NOT variable				%prec NOT	{ $$ = new ast::NotExp(@$, *$2); }
| variable DOT ID			%prec UPLEVEL	{ $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, *new symbol::Symbol(*$3))); }
| variable DOT functionCall				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT variable				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT functionCall				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| variable listableEnd					{ $$ = new ast::ListExp(@$, *$1, $2->step_get(), $2->end_get()); }
| functionCall listableEnd		%prec UPLEVEL	{ $$ = new ast::ListExp(@$, *$1, $2->step_get(), $2->end_get()); }
| matrix						{ $$ = $1; }
| operation						{ $$ = $1; }
| ID					%prec LISTABLE	{ $$ = new ast::SimpleVar(@$, *new symbol::Symbol(*$1)); }
| VARINT				%prec LISTABLE	{ $$ = new ast::DoubleExp(@$, $1); }
| NUM					%prec LISTABLE	{ $$ = new ast::DoubleExp(@$, $1); }
| VARFLOAT						{ $$ = new ast::DoubleExp(@$, $1); }
| STR							{ $$ = new ast::StringExp(@$, *$1); }
| DOLLAR						{ $$ = new ast::DollarVar(@$); }
| BOOLTRUE				%prec BOOLTRUE	{ $$ = new ast::BoolExp(@$, true); }
| BOOLFALSE				%prec BOOLFALSE	{ $$ = new ast::BoolExp(@$, false); }
| LPAREN variable RPAREN				{ $$ = $2; }
| LPAREN variableFields RPAREN				{ $$ = $2; }
| comparison						{ $$ = $1; }
;

/*
** -*- VARIABLE FIELDS -*-
*/
/* variable (, variable)+ */
variableFields :
variableFields COMMA variable		{
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back($1);
					  tmp->push_back($3);
					  $$ = new ast::ArrayListExp(@$, *tmp);
					}
| variableFields COMMA functionCall	{
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back($1);
					  tmp->push_back($3);
					  $$ = new ast::ArrayListExp(@$, *tmp);
					}
| variable COMMA variable		{
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back($1);
					  tmp->push_back($3);
					  $$ = new ast::ArrayListExp(@$, *tmp);
					}
| functionCall COMMA functionCall	{
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back($1);
					  tmp->push_back($3);
					  $$ = new ast::ArrayListExp(@$, *tmp);
					}
| functionCall COMMA variable		{
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back($1);
					  tmp->push_back($3);
					  $$ = new ast::ArrayListExp(@$, *tmp);
					}
| variable COMMA functionCall		{
					  ast::exps_t *tmp = new ast::exps_t;
					  tmp->push_back($1);
					  tmp->push_back($3);
					  $$ = new ast::ArrayListExp(@$, *tmp);
}
;

/*
** -*- MATRIX -*-
*/
/* How Matrix are written */
matrix :
LBRACK matrixLines RBRACK					{ $$ = new ast::MatrixExp(@$, *$2); }
| LBRACE matrixLines RBRACE					{ $$ = new ast::MatrixExp(@$, *$2); }
| LBRACK lineEnd matrixLines RBRACK				{ $$ = new ast::MatrixExp(@$, *$3); }
| LBRACE lineEnd matrixLines RBRACE				{ $$ = new ast::MatrixExp(@$, *$3); }
| LBRACK matrixLines matrixColumns RBRACK			{
								  $2->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *$2);
								}
| LBRACE matrixLines matrixColumns RBRACE			{
								  $2->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *$2);
								}
| LBRACK lineEnd matrixLines matrixColumns RBRACK		{
								  $3->push_back(new ast::MatrixLineExp(@4, *$4));
								  $$ = new ast::MatrixExp(@$, *$3);
								}
| LBRACE lineEnd matrixLines matrixColumns RBRACE		{
								  $3->push_back(new ast::MatrixLineExp(@4, *$4));
								  $$ = new ast::MatrixExp(@$, *$3);
								}
| LBRACK matrixColumns RBRACK					{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@2, *$2));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACE matrixColumns RBRACE					{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@2, *$2));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACK lineEnd matrixColumns RBRACK				{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACE lineEnd matrixColumns RBRACE				{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *tmp);
}
| LBRACK RBRACK							{ $$ = new ast::MatrixExp(@$, *new std::list<ast::MatrixLineExp *>); }
| LBRACE RBRACE							{ $$ = new ast::MatrixExp(@$, *new std::list<ast::MatrixLineExp *>); }
;

/*
** -*- MATRIX LINES -*-
*/
/* Matrix Lines : matrixLine (matrixline)* */
matrixLines :
matrixLines matrixLine			{
					  $1->push_back($2);
					  $$ = $1;
					}
| matrixLine				{
					  $$ = new std::list<ast::MatrixLineExp *>;
					  $$->push_front($1);
					}
;

/*
** -*- MATRIX LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is a line ending in a Matrix */
matrixLineBreak :
SEMI							{ /* !! Do Nothing !! */ }
| EOL							{ /* !! Do Nothing !! */ }
| SEMI EOL						{ /* !! Do Nothing !! */ }
;

/*
** -*- MATRIX LINE -*-
*/
/* Some matrix columns with a special matrix line break at the end */
matrixLine :
matrixColumns matrixLineBreak				{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixColumns COMMENT EOL				{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixColumns SEMI COMMENT EOL			{ $$ = new ast::MatrixLineExp(@$, *$1); }
;

/*
** -*- MATRIX COLUMNS -*-
*/
/* Matrix Columns : [variable|functinoCall] ([,|][variable|functionCall])* */
matrixColumns :
matrixColumns matrixColumnsBreak variable	%prec HIGHLEVEL {
								  $1->push_back($3);
								  $$ = $1;
								}
| matrixColumns matrixColumnsBreak functionCall	%prec HIGHLEVEL {
								  $1->push_back($3);
								  $$ = $1;
								}
| matrixColumns variable			%prec HIGHLEVEL {
								  $1->push_back($2);
								  $$ = $1;
								}
| matrixColumns functionCall			%prec HIGHLEVEL {
								  $1->push_back($2);
								  $$ = $1;
								}
| variable					%prec HIGHLEVEL {
								  $$ = new ast::exps_t;
								  $$->push_front($1);
								}
| functionCall					%prec HIGHLEVEL {
								  $$ = new ast::exps_t;
								  $$->push_front($1);
								}
;

/*
** -*- MATRIX COLUMNS BREAK -*-
*/
/* How to tell the column is now ended. */
matrixColumnsBreak :
matrixColumnsBreak COMMA					{ /* !! Do Nothing !! */ }
| COMMA								{ /* !! Do Nothing !! */ }
;

/*
** -*- VARIABLE DECLARATION -*-
*/
/* How to declare a new variable */
variableDeclaration :
variable ASSIGN variable		%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
| variable ASSIGN functionCall		%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
| functionCall ASSIGN variable		%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
| functionCall ASSIGN functionCall	%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
// --> Sugar Syntax for ':' meaning eye .* 1
| variable ASSIGN COLON					{ $$ = new ast::AssignExp(@$, *$1, *new ast::ColonVar(@3)); }
| functionCall ASSIGN COLON				{ $$ = new ast::AssignExp(@$, *$1, *new ast::ColonVar(@3)); }
// --> Strange Syntax : a = return (x)
| variable ASSIGN returnControl				{ $$ = new ast::AssignExp(@$, *$1, *$3); }
| functionCall ASSIGN returnControl			{ $$ = new ast::AssignExp(@$, *$1, *$3); }
;


/*
** -*- IF CONTROL -*-
*/
/* If Then Else End control block */
ifControl :
IF condition then thenBody END				{ $$ = new ast::IfExp(@$, *$2, *$4); }
| IF condition then thenBody else elseBody END		{ $$ = new ast::IfExp(@$, *$2, *$4, *$6); }
| IF condition then thenBody elseIfControl END		{ $$ = new ast::IfExp(@$, *$2, *$4, *$5); }
;

/*
** -*- THEN BODY -*-
*/
/* Instructions that can be managed inside THEN */
thenBody :
expressions						{ $$ = $1; }
| /* Epsilon */						{
							  ast::exps_t *tmp = new ast::exps_t;
							  #ifdef BUILD_DEBUG_AST
							  tmp->push_front(new ast::CommentExp(@$, new std::string("Empty then body")));
							  #endif
							  $$ = new ast::SeqExp(@$, *tmp);
							}
;

/*
** -*- ELSE BODY -*-
*/
/* Instructions that can be managed inside ELSE */
elseBody :
expressions						{ $$ = $1; }
| /* Epsilon */						{
							  ast::exps_t *tmp = new ast::exps_t;
							  #ifdef BUILD_DEBUG_AST
							  tmp->push_front(new ast::CommentExp(@$, new std::string("Empty else body")));
							  #endif
							  $$ = new ast::SeqExp(@$, *tmp);
							}
;

/*
** -*- IF CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'if' condition ending */
ifConditionBreak :
SEMI							{ /* !! Do Nothing !! */ }
| SEMI EOL						{ /* !! Do Nothing !! */ }
| COMMA							{ /* !! Do Nothing !! */ }
| COMMA EOL						{ /* !! Do Nothing !! */ }
| EOL							{ /* !! Do Nothing !! */ }
;

/*
** -*- THEN -*-
*/
/* Fake Rule : Only for lazy syntax */
then :
THEN							{ /* !! Do Nothing !! */ }
| ifConditionBreak THEN					{ /* !! Do Nothing !! */ }
| ifConditionBreak THEN EOL				{ /* !! Do Nothing !! */ }
| THEN ifConditionBreak					{ /* !! Do Nothing !! */ }
| ifConditionBreak					{ /* !! Do Nothing !! */ }
| /* Epsilon */						{ /* !! Do Nothing !! */ }
;

/*
** -*- ELSE -*-
*/
/* Fake Rule : Only for lazy syntax */
else :
ELSE							{ /* !! Do Nothing !! */ }
| ELSE COMMA						{ /* !! Do Nothing !! */ }
| ELSE SEMI						{ /* !! Do Nothing !! */ }
| ELSE EOL						{ /* !! Do Nothing !! */ }
| ELSE COMMA EOL					{ /* !! Do Nothing !! */ }
| ELSE SEMI EOL						{ /* !! Do Nothing !! */ }
;

/*
** -*- ELSEIF CONTROL
*/
/* else if ... then ... */
elseIfControl :
ELSEIF condition then thenBody						{
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp(@$, *$2, *$4) );
										$$ = new ast::SeqExp(@$, *tmp);
									}
| ELSEIF condition then thenBody else elseBody				{
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp(@$, *$2, *$4, *$6) );
										$$ = new ast::SeqExp(@$, *tmp);

									}
| ELSEIF condition then thenBody elseIfControl				{
										ast::exps_t *tmp = new ast::exps_t;
										tmp->push_front( new ast::IfExp(@$, *$2, *$5) );
										$$ = new ast::SeqExp(@$, *tmp);
									}
;

//| ELSEIF condition then thenBody else elseBody elseIfControl		{ $$ = new ast::CommentExp(@$, new std::string("!! FIXME !! Elseif additionnal control ??")); }


/*
** -*- SELECT CONTROL -*-
*/
/* Select Case Then End control block */
/* FIXME : Add the corresponding AST !!! */
selectControl :
SELECT selectable selectConditionBreak casesControl END
| SELECT selectable selectConditionBreak casesControl else elseBody END
| SELECT selectable COMMENT selectConditionBreak casesControl END
| SELECT selectable COMMENT selectConditionBreak casesControl else elseBody END
;

/*
** -*- SELECTABLE -*-
*/
/* On what can a select bloc be switch. */
selectable :
variable
| functionCall
;

/*
** -*- SELECT CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'select' condition ending. */
selectConditionBreak :
EOL									{ /* !! Do Nothing !! */ }
| COMMA EOL								{ /* !! Do Nothing !! */ }
| SEMI EOL								{ /* !! Do Nothing !! */ }

;

/*
** -*- CASE CONTROL -*-
*/
/* (Case ... Then ...)+ control block */
casesControl :
CASE variable caseControlBreak caseBody
| comments CASE variable caseControlBreak caseBody
| CASE variable COMMENT EOL caseBody
| comments CASE variable COMMENT EOL caseBody
| casesControl CASE variable caseControlBreak caseBody
| casesControl CASE variable COMMENT EOL caseBody
;

caseBody :
expressions
| /* Epsilon */
;

/*
** -*- CASE CONTROL BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'case' ending */
caseControlBreak :
THEN						{ /* !! Do Nothing !! */ }
| COMMA						{ /* !! Do Nothing !! */ }
| SEMI						{ /* !! Do Nothing !! */ }
| EOL						{ /* !! Do Nothing !! */ }
| THEN EOL					{ /* !! Do Nothing !! */ }
| COMMA EOL					{ /* !! Do Nothing !! */ }
| SEMI EOL					{ /* !! Do Nothing !! */ }
| THEN COMMA					{ /* !! Do Nothing !! */ }
| THEN COMMA EOL				{ /* !! Do Nothing !! */ }
;

/*
** -*- FOR CONTROL -*-
*/
/* For ... End control block */
forControl :
FOR ID ASSIGN forIterator forConditionBreak forBody END			{ $$ = new ast::ForExp(@$, *new ast::VarDec(@3, *new symbol::Symbol(*$2), *$4), *$6); }
| FOR LPAREN ID ASSIGN forIterator RPAREN forConditionBreak forBody END { $$ = new ast::ForExp(@$, *new ast::VarDec(@4, *new symbol::Symbol(*$3), *$5), *$8); }
;

/*
** -*- FOR ITERATOR -*-
*/
/* For loop variable to tell the number of iterations. */
forIterator :
functionCall		%prec UPLEVEL		{ $$ = $1; }
| variable		%prec UPLEVEL		{ $$ = $1; }
;


/*
** -*- FOR CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'for' condition ending. */
forConditionBreak :
EOL						{ /* !! Do Nothing !! */ }
| SEMI						{ /* !! Do Nothing !! */ }
| SEMI EOL					{ /* !! Do Nothing !! */ }
| COMMA						{ /* !! Do Nothing !! */ }
| COMMA EOL					{ /* !! Do Nothing !! */ }
| DO						{ /* !! Do Nothing !! */ }
| DO EOL					{ /* !! Do Nothing !! */ }
| /* Epsilon */					{ /* !! Do Nothing !! */ }
;

forBody :
expressions			{ $$ = $1; }
| /* Epsilon */			{
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp(@$, new std::string("Empty for body")));
				  #endif
				  $$ = new ast::SeqExp(@$, *tmp);
				}
;

/*
** -*- WHILE CONTROL -*-
*/
/* while ... End control block. */
whileControl :
WHILE condition whileConditionBreak whileBody END	{ $$ = new ast::WhileExp(@$, *$2, *$4); }
;

/*
** -*- WHILE BODY -*-
*/
/* Which instructions can be used in a while loop. */
whileBody :
expressions			{ $$ = $1; }
| /* Epsilon */			{
				  ast::exps_t *tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_front(new ast::CommentExp(@$, new std::string("Empty while body")));
				  #endif
				  $$ = new ast::SeqExp(@$, *tmp);
				}
;

/*
** -*- WHILE CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'while' condition ending. */
whileConditionBreak :
COMMA				{ /* !! Do Nothing !! */ }
| SEMI				{ /* !! Do Nothing !! */ }
| DO				{ /* !! Do Nothing !! */ }
| DO COMMA			{ /* !! Do Nothing !! */ }
| DO SEMI			{ /* !! Do Nothing !! */ }
| THEN				{ /* !! Do Nothing !! */ }
| THEN COMMA			{ /* !! Do Nothing !! */ }
| THEN SEMI			{ /* !! Do Nothing !! */ }
| lineEnd			{ /* !! Do Nothing !! */ }
| COMMA EOL			{ /* !! Do Nothing !! */ }
| SEMI EOL			{ /* !! Do Nothing !! */ }
| DO EOL			{ /* !! Do Nothing !! */ }
| DO COMMA EOL			{ /* !! Do Nothing !! */ }
| DO SEMI EOL			{ /* !! Do Nothing !! */ }
| THEN EOL			{ /* !! Do Nothing !! */ }
| THEN COMMA EOL		{ /* !! Do Nothing !! */ }
| THEN SEMI EOL			{ /* !! Do Nothing !! */ }
;

/*
** -*- TRY CONTROL -*-
*/
/* try ... catch ... end control block. */
tryControl :
TRY EOL expressions CATCH EOL expressions END			{ $$ =new ast::TryCatchExp(@$, *$3, *$6); }
| TRY COMMENT EOL expressions CATCH EOL expressions END		{
								  $4->exps_get().push_front(new ast::CommentExp(@2, $2));
								  $$ =new ast::TryCatchExp(@$, *$4, *$7);
								}
| TRY EOL expressions CATCH COMMENT EOL expressions END		{
								  $7->exps_get().push_front(new ast::CommentExp(@5, $5));
								  $$ =new ast::TryCatchExp(@$, *$3, *$7);
								}
| TRY COMMENT EOL expressions CATCH COMMENT EOL expressions END {
								  $4->exps_get().push_front(new ast::CommentExp(@2, $2));
								  $8->exps_get().push_front(new ast::CommentExp(@6, $6));
								  $$ =new ast::TryCatchExp(@$, *$4, *$8);
								}
;

/*
** -*- RETURN CONTROL -*-
*/
/* Make a break in a function or make the variable getting one scope up. */
returnControl :
RETURN				{ $$ = new ast::ReturnExp(@$); }
| RETURN variable		{ $$ = new ast::ReturnExp(@$, $2); }
;

/*
** -*- COMPARATORS -*-
*/
/* Comparators token == <>  etc... */
comparators :
EQ				{ $$ = ast::OpExp::eq; }
| NE				{ $$ = ast::OpExp::ne; }
| LT				{ $$ = ast::OpExp::lt; }
| LE				{ $$ = ast::OpExp::le; }
| GT				{ $$ = ast::OpExp::gt; }
| GE				{ $$ = ast::OpExp::ge; }
| AND				{ $$ = ast::OpExp::binaryAnd; }
| OR				{ $$ = ast::OpExp::binaryOr; }
;

/*
** -*- COMMENTS -*-
*/
comments :
COMMENT EOL
| comments COMMENT EOL
;

/*
** -*- LINE END -*-
*/
/* Comment can be added also to end a line */
lineEnd :
EOL
| COMMENT EOL
;

%%
void yyerror(std::string msg) {
  Parser::PrintError(msg);
  exit(PARSE_ERROR);
}
