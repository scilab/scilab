%{ // -*- C++ -*-
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


%}

//%pure-parser
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

  ast::FunctionDec*	t_function_dec;

  ast::ArrayListExp*	t_arraylist_exp;
  ast::AssignListExp*	t_assignlist_exp;
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
%token MINUS		"-"
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
%token ANDAND		"&&"
%token OR		"|"
%token OROR		"||"
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
%token HIDDENFUNCTION	"#function"
%token HIDDEN		"hidden"


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

%type <t_exp>		assignable
%type <t_assignlist_exp>multipleResults
%type <t_exp>		variable
%type <t_arraylist_exp>	variableFields
%type <t_exp>		expression

%type <t_op_exp>	comparison
%type <t_op_exp>	rightComparable
%type <t_exp>		operation
%type <t_op_exp>	rightOperand

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

 // SELECT Control
%type<t_select_exp> selectControl
%type<t_exp>		selectable
%type<t_list_case>  casesControl
%type <t_seq_exp>	caseBody

 // Implicit Function Call
%type <t_call_exp>	implicitFunctionCall
%type <t_string_exp>	implicitCallable

 // Function Call
%type <t_call_exp>	functionCall
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

 // Matrix & Cell
%type<t_matrix_exp>	matrix
%type<t_cell_exp>	cell
%type<t_list_exp>	matrixOrCellColumns
%type<t_matrixline_exp>	matrixOrCellLine
%type<t_list_mline>	matrixOrCellLines

%type<mute>		expressionLineBreak

%nonassoc TOPLEVEL
%nonassoc HIGHLEVEL
%nonassoc UPLEVEL

%nonassoc LISTABLE

%nonassoc FUNCTIONCALL
%nonassoc BOOLTRUE BOOLFALSE
%nonassoc LPAREN

%left OR OROR
%left AND ANDAND

%left COLON
%nonassoc EQ NE LT LE GT GE
%left MINUS PLUS
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
| EOL expressions 				{ Parser::getInstance()->setTree($2); }
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
						  $2->set_verbose(true);
						  $1->push_back($2);
						  $$ = new ast::SeqExp(@$, *$1);
						}
| recursiveExpression expression COMMENT	{
						  $2->set_verbose(true);
						  $1->push_back($2);
						  $1->push_back(new ast::CommentExp(@3, $3));
						  $$ = new ast::SeqExp(@$, *$1);
						}
| expression					{
						  ast::exps_t *tmp = new ast::exps_t;
						  $1->set_verbose(true);
						  tmp->push_front($1);
						  $$ = new ast::SeqExp(@$, *tmp);
						}
| expression COMMENT				{
						  ast::exps_t *tmp = new ast::exps_t;
						  $1->set_verbose(true);
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
							  $2->set_verbose($3);
							  $1->push_back($2);
							  $$ = $1;
							}
| recursiveExpression expression COMMENT expressionLineBreak {
							  $2->set_verbose($4);
							  $1->push_back($2);
							  $1->push_back(new ast::CommentExp(@3, $3));
							  $$ = $1;
							}
| expression COMMENT expressionLineBreak		{
							  ast::exps_t *tmp = new ast::exps_t;
							  tmp->push_front(new ast::CommentExp(@2, $2));
							  $1->set_verbose($3);
							  tmp->push_front($1);
							  $$ = tmp;
							}
| expression expressionLineBreak			{
							  ast::exps_t *tmp = new ast::exps_t;
							  $1->set_verbose($2);
							  tmp->push_front($1);
							  $$ = tmp;
							}
;

/*
** -*- EXPRESSION LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is the end of an instruction. */
expressionLineBreak :
SEMI						{ $$ = false; }
| COMMA						{ $$ = true; }
| EOL						{ $$ = true; }
| expressionLineBreak SEMI			{ $$ = false; }
| expressionLineBreak COMMA			{ $$ = true; }
| expressionLineBreak EOL			{ $$ = $1; }
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
| selectControl					{ $$ = $1; }
| forControl					{ $$ = $1; }
| whileControl					{ $$ = $1; }
| tryControl					{ $$ = $1; }
| variable			%prec TOPLEVEL	{ $$ = $1; }
| implicitFunctionCall		%prec TOPLEVEL	{ $$ = $1; }
| BREAK						{ $$ = new ast::BreakExp(@$); }
| returnControl					{ $$ = $1; }
| COMMENT					{ $$ = new ast::CommentExp(@$, $1); }
| error						{
  $$ = new ast::CommentExp(@$, new std::string("@@ ERROR RECOVERY @@"));
  StopOnError();
  }
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
| VARINT					{
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
| FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar(@2, *new symbol::Symbol(*$2)));
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$4),
							    *new ast::ArrayListVar(@5, *$5),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$7);
				}
| FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3 ,*$3),
							    *$9);
				}
| FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *new ast::vars_t),
							    *$8);
				}
| FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$2),
							    *new ast::ArrayListVar(@3, *$3),
							    *new ast::ArrayListVar(@$, *new ast::vars_t),
							    *$5);
				}
| HIDDENFUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar(@2, *new symbol::Symbol(*$2)));
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$4),
							    *new ast::ArrayListVar(@5, *$5),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$7);
				}
| HIDDENFUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3 ,*$3),
							    *$9);
				}
| HIDDENFUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *new ast::vars_t),
							    *$8);
				}
| HIDDENFUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$2),
							    *new ast::ArrayListVar(@3, *$3),
							    *new ast::ArrayListVar(@$, *new ast::vars_t),
							    *$5);
				}
| HIDDENFUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar(@2, *new symbol::Symbol(*$2)));
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$4),
							    *new ast::ArrayListVar(@5, *$5),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$7);
				}
| HIDDENFUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3 ,*$3),
							    *$9);
				}
| HIDDENFUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *new ast::vars_t),
							    *$8);
				}
| HIDDENFUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$2),
							    *new ast::ArrayListVar(@3, *$3),
							    *new ast::ArrayListVar(@$, *new ast::vars_t),
							    *$5);
				}
| HIDDEN FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar(@2, *new symbol::Symbol(*$3)));
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@3, *tmp),
							    *$8);
				}
| HIDDEN FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$7),
							    *new ast::ArrayListVar(@8, *$8),
							    *new ast::ArrayListVar(@4 ,*$4),
							    *$10);
				}
| HIDDEN FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3, *new ast::vars_t),
							    *$9);
				}
| HIDDEN FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$3),
							    *new ast::ArrayListVar(@4, *$4),
							    *new ast::ArrayListVar(@$, *new ast::vars_t),
							    *$6);
				}
| HIDDEN FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  ast::vars_t *tmp = new ast::vars_t;
				  tmp->push_front(new ast::SimpleVar(@3, *new symbol::Symbol(*$3)));
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$8);
				}
| HIDDEN FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$7),
							    *new ast::ArrayListVar(@8, *$8),
							    *new ast::ArrayListVar(@4 ,*$4),
							    *$10);
				}
| HIDDEN FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3, *new ast::vars_t),
							    *$9);
				}
| HIDDEN FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    *new symbol::Symbol(*$3),
							    *new ast::ArrayListVar(@4, *$4),
							    *new ast::ArrayListVar(@$, *new ast::vars_t),
							    *$6);
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
variable rightComparable		{ 
					  delete &($2->left_get());
					  $2->left_set(*$1);
					  $$ = $2;
					}
| functionCall rightComparable		{ 
					  delete &($2->left_get());
					  $2->left_set(*$1);
					  $$ = $2;
					}
;

/*
** -*- RIGHT COMPARABLE -*-
*/
/* rightComparable for comparison */
rightComparable :
/* & */
AND variable				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *$2); }
| AND functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *$2); }
| AND COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalAnd, * new ast::ColonVar(@$)); }
/* && */
| ANDAND variable			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *$2); }
| ANDAND functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *$2); }
| ANDAND COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, * new ast::ColonVar(@$)); }
/* | */
| OR variable				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalOr, *$2); }
| OR functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalOr, *$2); }
| OR COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalOr, * new ast::ColonVar(@$)); }
/* || */				
| OROR variable				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *$2); }
| OROR functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *$2); }
| OROR COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, * new ast::ColonVar(@$)); }
/* == */
| EQ variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::eq, *$2); }
| EQ functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::eq, *$2); }
| EQ COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::eq, * new ast::ColonVar(@$)); }
/* ~= */
| NE variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::ne, *$2); }
| NE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::ne, *$2); }
| NE COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::ne, * new ast::ColonVar(@$)); }
/* > */
| GT variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::gt, *$2); }
| GT functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::gt, *$2); }
| GT COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::gt, * new ast::ColonVar(@$)); }
/* < */
| LT variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::lt, *$2); }
| LT functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::lt, *$2); }
| LT COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::lt, * new ast::ColonVar(@$)); }
/* >= */
| GE variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::ge, *$2); }
| GE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::ge, *$2); }
| GE COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::ge, * new ast::ColonVar(@$)); }
/* <= */
| LE variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::le, *$2); }
| LE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::le, *$2); }
| LE COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::le, * new ast::ColonVar(@$)); }
;

/*
** -*- OPERATIONS -*-
*/
/* Operations */
operation :
variable rightOperand			{ 
					  delete &($2->left_get());
					  $2->left_set(*$1);
					  $2->location_set(@$);
					  $$ = $2;
					}
| functionCall rightOperand		{ 
					  delete &($2->left_get());
					  $2->left_set(*$1);
					  $2->location_set(@$);
					  $$ = $2;
					}
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
/*   '-'   '.-'   '.-.'?   */
| MINUS variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::minus, *$2); }
| MINUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::string("Should not stay in that state")), ast::OpExp::minus, *$2); }
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
| NOT functionCall			%prec NOT	{ $$ = new ast::NotExp(@$, *$2); }
| variable DOT ID			%prec UPLEVEL	{ $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, *new symbol::Symbol(*$3))); }
| variable DOT functionCall				{ 
							  $3->name_set(new ast::FieldExp(@$, *$1, $3->name_get())); 
							  $3->location_set(@$);
							  $$ = $3;
}
| functionCall DOT variable				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT functionCall				{ 
							  $3->name_set(new ast::FieldExp(@$, *$1, $3->name_get())); 
							  $3->location_set(@$);
							  $$ = $3;
}
| variable listableEnd					{ $$ = new ast::ListExp(@$, *$1, $2->step_get(), $2->end_get()); }
| functionCall listableEnd		%prec UPLEVEL	{ $$ = new ast::ListExp(@$, *$1, $2->step_get(), $2->end_get()); }
| matrix						{ $$ = $1; }
| cell							{ $$ = $1; }
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
					  $1->exps_get().push_back($3);
					  $$ = $1;
					}
| variableFields COMMA functionCall	{
					  $1->exps_get().push_back($3);
					  $$ = $1;
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
** -*- CELL -*-
*/
cell :
LBRACE matrixOrCellLines RBRACE					{ $$ = new ast::CellExp(@$, *$2); }
| LBRACE lineEnd matrixOrCellLines RBRACE				{ $$ = new ast::CellExp(@$, *$3); }
| LBRACE matrixOrCellLines matrixOrCellColumns RBRACE			{
								  $2->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::CellExp(@$, *$2);
								}
| LBRACE lineEnd matrixOrCellLines matrixOrCellColumns RBRACE		{
								  $3->push_back(new ast::MatrixLineExp(@4, *$4));
								  $$ = new ast::CellExp(@$, *$3);
								}
| LBRACE matrixOrCellColumns RBRACE					{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@2, *$2));
								  $$ = new ast::CellExp(@$, *tmp);
								}
| LBRACE lineEnd matrixOrCellColumns RBRACE				{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::CellExp(@$, *tmp);
}
| LBRACE RBRACE							{ $$ = new ast::CellExp(@$, *new std::list<ast::MatrixLineExp *>); }
;


/*
** -*- MATRIX -*-
*/
/* How Matrix are written */
matrix :
LBRACK matrixOrCellLines RBRACK					{ $$ = new ast::MatrixExp(@$, *$2); }
| LBRACK lineEnd matrixOrCellLines RBRACK				{ $$ = new ast::MatrixExp(@$, *$3); }
| LBRACK matrixOrCellLines matrixOrCellColumns RBRACK			{
								  $2->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *$2);
								}
| LBRACK lineEnd matrixOrCellLines matrixOrCellColumns RBRACK		{
								  $3->push_back(new ast::MatrixLineExp(@4, *$4));
								  $$ = new ast::MatrixExp(@$, *$3);
								}
| LBRACK matrixOrCellColumns RBRACK					{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@2, *$2));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACK lineEnd matrixOrCellColumns RBRACK				{
								  std::list<ast::MatrixLineExp *> *tmp = new std::list<ast::MatrixLineExp *>;
								  tmp->push_front(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACK RBRACK							{ $$ = new ast::MatrixExp(@$, *new std::list<ast::MatrixLineExp *>); }
;

/*
** -*- MATRIX ORC ELL LINES -*-
*/
/* Matrix or Cell Lines : matrixOrCellLine (matrixOrCellline)* */
matrixOrCellLines :
matrixOrCellLines matrixOrCellLine	{
					  $1->push_back($2);
					  $$ = $1;
					}
| matrixOrCellLine			{
					  $$ = new std::list<ast::MatrixLineExp *>;
					  $$->push_front($1);
					}
;

/*
** -*- MATRIX OR CELL LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is a line ending in a Matrix/Cell */
matrixOrCellLineBreak :
SEMI							{ /* !! Do Nothing !! */ }
| EOL							{ /* !! Do Nothing !! */ }
| SEMI EOL						{ /* !! Do Nothing !! */ }
;

/*
** -*- MATRIX OR CELL LINE -*-
*/
/* Some matrix/cell columns with a special matrix/cell line break at the end */
matrixOrCellLine :
matrixOrCellColumns matrixOrCellLineBreak				{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixOrCellColumns COMMENT EOL					{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixOrCellColumns SEMI COMMENT EOL					{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak	{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixOrCellColumns matrixOrCellColumnsBreak COMMENT EOL		{ $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixOrCellColumns matrixOrCellColumnsBreak SEMI COMMENT EOL		{ $$ = new ast::MatrixLineExp(@$, *$1); }
;

/*
** -*- MATRIX OR CELL COLUMNS -*-
*/
/* Matrix or Cell Columns : [variable|functinoCall] ([,|][variable|functionCall])* */
matrixOrCellColumns :
matrixOrCellColumns matrixOrCellColumnsBreak variable		%prec HIGHLEVEL {
								  $1->push_back($3);
								  $$ = $1;
								}
| matrixOrCellColumns matrixOrCellColumnsBreak functionCall	%prec HIGHLEVEL {
								  $1->push_back($3);
								  $$ = $1;
								}
| matrixOrCellColumns variable					%prec HIGHLEVEL {
								  $1->push_back($2);
								  $$ = $1;
								}
| matrixOrCellColumns functionCall				%prec HIGHLEVEL {
								  $1->push_back($2);
								  $$ = $1;
								}
| variable							%prec HIGHLEVEL {
								  $$ = new ast::exps_t;
								  $$->push_front($1);
								}
| functionCall							%prec HIGHLEVEL {
								  $$ = new ast::exps_t;
								  $$->push_front($1);
								}
;

/*
** -*- MATRIX OR CELL COLUMNS BREAK -*-
*/
/* How to tell the column is now ended. */
matrixOrCellColumnsBreak :
matrixOrCellColumnsBreak COMMA					{ /* !! Do Nothing !! */ }
| COMMA								{ /* !! Do Nothing !! */ }
;

/*
** -*- VARIABLE DECLARATION -*-
*/
/* How to declare a new variable */
variableDeclaration :
assignable ASSIGN variable		%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
| assignable ASSIGN functionCall	%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
| functionCall ASSIGN variable		%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
| functionCall ASSIGN functionCall	%prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); }
// --> Sugar Syntax for ':' meaning eye .* 1
| assignable ASSIGN COLON				{ $$ = new ast::AssignExp(@$, *$1, *new ast::ColonVar(@3)); }
| functionCall ASSIGN COLON				{ $$ = new ast::AssignExp(@$, *$1, *new ast::ColonVar(@3)); }
// --> Strange Syntax : a = return (x)
| assignable ASSIGN returnControl			{ $$ = new ast::AssignExp(@$, *$1, *$3); }
| functionCall ASSIGN returnControl			{ $$ = new ast::AssignExp(@$, *$1, *$3); }
;


/*
** -*- ASSIGNABLE -*-
*/
/* What we can assign something to. */
assignable :
variable DOT ID			%prec UPLEVEL		{ $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, *new symbol::Symbol(*$3))); }
| variable DOT functionCall				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT variable				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT functionCall				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| ID					%prec LISTABLE	{ $$ = new ast::SimpleVar(@$, *new symbol::Symbol(*$1)); }
| multipleResults					{ $$ = $1; }
;

/*
** -*- MULTIPLE RESULTS -*-
*/
multipleResults :
LBRACK matrixOrCellColumns RBRACK			{ $$ = new ast::AssignListExp(@$, *$2); }
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
selectControl :
SELECT selectable selectConditionBreak casesControl END									{ $$ = new ast::SelectExp(@$, *$2, *$4); }
| SELECT selectable selectConditionBreak casesControl else elseBody END					{ $$ = new ast::SelectExp(@$, *$2, *$4, *$6); }
| SELECT selectable COMMENT selectConditionBreak casesControl END						{ $$ = new ast::SelectExp(@$, *$2, *$5); }
| SELECT selectable COMMENT selectConditionBreak casesControl else elseBody END			{ $$ = new ast::SelectExp(@$, *$2, *$5, *$7); }
;

/*
** -*- SELECTABLE -*-
*/
/* On what can a select bloc be switch. */
selectable :
variable				{ $$ = $1; }
| functionCall			{ $$ = $1; }
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
CASE variable caseControlBreak caseBody							{ 
																  $$ = new ast::cases_t;
																  $$->push_back(new ast::CaseExp(@$, *$2, *$4));
																}
| comments CASE variable caseControlBreak caseBody				{
																  $$ = new ast::cases_t;
																  $$->push_back(new ast::CaseExp(@$, *$3, *$5));
																}
| CASE variable COMMENT EOL caseBody							{
																  $$ = new ast::cases_t;
																  $$->push_back(new ast::CaseExp(@$, *$2, *$5));
																}
| comments CASE variable COMMENT EOL caseBody					{
																  $$ = new ast::cases_t;
																  $$->push_back(new ast::CaseExp(@$, *$3, *$6));
																}
| casesControl CASE variable caseControlBreak caseBody			{
																  $1->push_back(new ast::CaseExp(@$, *$3, *$5));
																  $$ = $1;
																}
| casesControl CASE variable COMMENT EOL caseBody				{
																  $1->push_back(new ast::CaseExp(@$, *$3, *$6));
																  $$ = $1;
																}
;

caseBody :
expressions				{ $$ = $1; }
| /* Epsilon */			{
						  ast::exps_t *tmp = new ast::exps_t;
						#ifdef BUILD_DEBUG_AST
						  tmp->push_front(new ast::CommentExp(@$, new std::string("Empty case body")));
						#endif
						  $$ = new ast::SeqExp(@$, *tmp);
						}
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
  if(!Parser::getInstance()->isStrictMode()
     || Parser::getInstance()->getExitStatus() == Parser::Succeded)
    {
      Parser::PrintError(msg);
      Parser::getInstance()->setExitStatus(Parser::Failed);
    }
}
