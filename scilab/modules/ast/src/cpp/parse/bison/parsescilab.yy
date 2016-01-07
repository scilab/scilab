%{ // -*- C++ -*-
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
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

#define StopOnError()                                           \
    {                                                           \
        if(ParserSingleInstance::stopOnFirstError())            \
        {                                                       \
            return ParserSingleInstance::getExitStatus();       \
        }                                                       \
    }

#define SetTree(PTR)					\
    {									\
        if(ParserSingleInstance::getExitStatus() == Parser::Failed)	\
        {								\
            delete PTR;							\
	    ParserSingleInstance::setTree(nullptr);			\
        }								\
	else								\
	{								\
	    ParserSingleInstance::setTree(PTR);				\
	}								\
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
}

%destructor { delete $$; } <*>
%destructor { } <number>
%destructor { for (auto e : *$$) delete e; delete $$; } <t_list_var>
%destructor { for (auto e : *$$) delete e; delete $$; } <t_list_exp>
%destructor { for (auto e : *$$) delete e; delete $$; } <t_list_case>
%destructor { for (auto e : *$$) delete e; delete $$; } <t_list_mline>

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
%token CONTROLTIMES "*."
%token RDIVIDE		"/"
%token DOTRDIVIDE	"./"
%token CONTROLRDIVIDE	"/."
%token KRONRDIVIDE	"./."
%token LDIVIDE		"\\"
%token DOTLDIVIDE	".\\"
%token CONTROLLDIVIDE "\\."
%token KRONLDIVIDE	".\\."

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
%token SWITCH	"switch"
%token CASE		"case"
%token OTHERWISE "otherwise"

%token FUNCTION		"function"
%token ENDFUNCTION	"endfunction"

%token FOR		"for"

%token WHILE		"while"
%token DO		"do"
%token BREAK		"break"
%token CONTINUE     "continue"

%token TRY		"try"
%token CATCH		"catch"
%token RETURN		"return"

%token FLEX_ERROR

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
%type <t_list_exp>	variableFields
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
%type<t_seq_exp>    catchBody

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
 //%type <t_call_exp>	recursiveFunctionCall
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

%type<t_simple_var> keywords

%nonassoc TOPLEVEL
%nonassoc HIGHLEVEL
%nonassoc UPLEVEL
%nonassoc LISTABLE

%nonassoc CONTROLBREAK

%left OR OROR
%left AND ANDAND

%left COLON
%left EQ NE LT LE GT GE
%left MINUS PLUS
%left TIMES DOTTIMES KRONTIMES CONTROLTIMES RDIVIDE DOTRDIVIDE KRONRDIVIDE CONTROLRDIVIDE LDIVIDE DOTLDIVIDE KRONLDIVIDE CONTROLLDIVIDE
%left UMINUS
%right POWER DOTPOWER

%left QUOTE DOTQUOTE

%left NOT

%left DOT

%nonassoc FUNCTIONCALL
%nonassoc BOOLTRUE BOOLFALSE
%nonassoc LPAREN LBRACE

%start program


%%
/*
** -*- PROGRAM -*-
*/
/* Root of the Abstract Syntax Tree */
program:
expressions                     { SetTree($1); }
| EOL expressions 		{ SetTree($2); }
| expressionLineBreak           {
                                  ast::exps_t* tmp = new ast::exps_t;
                                  #ifdef BUILD_DEBUG_AST
                                      tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty body");
                                  #endif
                                  SetTree(new ast::SeqExp(@$, *tmp));
				  delete $1;
                                }
| /* Epsilon */                 {
                                  ast::exps_t* tmp = new ast::exps_t;
                                  #ifdef BUILD_DEBUG_AST
                                      tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty body")));
                                  #endif
                                  SetTree(new ast::SeqExp(@$, *tmp));
                                }
;

/*
** -*- EXPRESSIONS -*-
*/
/* List of expression or single instruction */
expressions :
recursiveExpression                             {
                                                  $$ = new ast::SeqExp(@$, *$1);
                                                }
| recursiveExpression expression                {
                                                  $2->setVerbose(true);
                                                  $1->push_back($2);
                                                  $$ = new ast::SeqExp(@$, *$1);
                                                }
| recursiveExpression expression COMMENT        {
                                                  $2->setVerbose(true);
                                                  $1->push_back($2);
                                                  $1->push_back(new ast::CommentExp(@3, $3));
                                                  $$ = new ast::SeqExp(@$, *$1);
                                                }
| expression                                    {
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  $1->setVerbose(true);
                                                  tmp->push_back($1);
                                                  $$ = new ast::SeqExp(@$, *tmp);
                                                }
| expression COMMENT                            {
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  $1->setVerbose(true);
                                                  tmp->push_back($1);
                                                  tmp->push_back(new ast::CommentExp(@2, $2));
                                                  $$ = new ast::SeqExp(@$, *tmp);
                                                }
;

/*
** -*- RECURSIVE EXPRESSION -*-
*/
/* List of instructions. _MUST_BE_ left recursive Rule */
recursiveExpression :
recursiveExpression expression expressionLineBreak	{
							  $2->setVerbose($3->bVerbose);
							  $1->push_back($2);
							  $$ = $1;
                              if ($3->iNbBreaker != 0)
                              {
                                  $2->getLocation().last_column = $3->iNbBreaker;
                              }
			      delete $3;
							}
| recursiveExpression expression COMMENT expressionLineBreak {
							  $2->setVerbose($4->bVerbose);
							  $1->push_back($2);
                              @3.columns($4->iNbBreaker);
							  $1->push_back(new ast::CommentExp(@3, $3));
							  $$ = $1;
							  delete $4;
							}
| expression COMMENT expressionLineBreak		{
							  ast::exps_t* tmp = new ast::exps_t;
                              @2.columns($3->iNbBreaker);
							  $1->setVerbose($3->bVerbose);
							  tmp->push_back($1);
							  tmp->push_back(new ast::CommentExp(@2, $2));
							  $$ = tmp;
							  delete $3;
							}
| expression expressionLineBreak			{
							  ast::exps_t* tmp = new ast::exps_t;
							  $1->setVerbose($2->bVerbose);
							  tmp->push_back($1);
							  $$ = tmp;
                              if ($2->iNbBreaker != 0)
                              {
                                  $1->getLocation().last_column = $2->iNbBreaker;
                              }
			      delete $2;
							}
;

/*
** -*- EXPRESSION LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is the end of an instruction. */
expressionLineBreak :
SEMI                            { $$ = new LineBreakStr(); $$->bVerbose = false; $$->iNbBreaker = @1.last_column; }
| COMMA                         { $$ = new LineBreakStr(); $$->bVerbose = true; $$->iNbBreaker = @1.last_column; }
| EOL                           { $$ = new LineBreakStr(); $$->bVerbose = true; $$->iNbBreaker = 0; }
| expressionLineBreak SEMI      { $$ = $1; $$->bVerbose = false || $1->bVerbose; $$->iNbBreaker = @2.last_column; }
| expressionLineBreak COMMA     { $$ = $1; $$->iNbBreaker = @2.last_column; }
| expressionLineBreak EOL       { $$ = $1; }
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
| CONTINUE					{ $$ = new ast::ContinueExp(@$); }
| returnControl					{ $$ = $1; }
| COMMENT					{ $$ = new ast::CommentExp(@$, $1); }
| error					        {
  $$ = new ast::CommentExp(@$, new std::wstring(L"@@ ERROR RECOVERY @@"));
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
						  $1->addArg($2);
						  $1->setLocation(@$);
                          $$ = $1;
						}
| ID implicitCallable				{
						  ast::exps_t* tmp = new ast::exps_t;
						  tmp->push_back($2);
						  $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *tmp);
						  delete $1;
						}
;

/*
** -*- IMPLICIT CALLABLE -*-
*/
/* Bash-like : foo bar titi <=> foo('bar', 'titi').
** Describe 'bar', 'titi' that can be more complex.
*/
implicitCallable :
ID						{ $$ = new ast::StringExp(@$, *$1); delete $1;}
| VARINT					{
						  std::wstringstream tmp;
						  tmp << $1;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| NUM						{
						  std::wstringstream tmp;
						  tmp << $1;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| VARFLOAT					{
						  std::wstringstream tmp;
						  tmp << $1;
						  $$ = new ast::StringExp(@$, tmp.str());
						}
| STR						{ $$ = new ast::StringExp(@$, *$1); delete $1;}
| DOLLAR					{ $$ = new ast::StringExp(@$, std::wstring(L"$")); }
| BOOLTRUE					{ $$ = new ast::StringExp(@$, std::wstring(L"%t")); }
| BOOLFALSE					{ $$ = new ast::StringExp(@$, std::wstring(L"%f")); }
| implicitCallable DOT ID			{
						  std::wstringstream tmp;
						  tmp << $1->getValue() << "." << *$3;
						  $$ = new ast::StringExp(@$, tmp.str());
						  delete $3;
						}
| PATH						{ $$ = new ast::StringExp(@$, *$1); delete $1;}
;

/*
** -*- FUNCTION CALL -*-
*/
/* How to call a function or a cell extraction */
functionCall :
simpleFunctionCall			{ $$ = $1; }
//| recursiveFunctionCall		%prec FUNCTIONCALL	{ $$ = $1; }
| LPAREN functionCall RPAREN	{ $$ = $2; }
;

/*
** -*- SIMPLE FUNCTION CALL -*-
*/
/* Usual way to call functions foo(arg1, arg2, arg3)
** or extract cell values foo{arg1, arg2, arg3}
*/
simpleFunctionCall :

ID LPAREN functionArgs RPAREN				{ $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *$3); delete $1;}
| ID LBRACE functionArgs RBRACE				{ $$ = new ast::CellCallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *$3); delete $1;}
| ID LPAREN RPAREN				            { $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *new ast::exps_t); delete $1;}
| ID LBRACE RBRACE				            { $$ = new ast::CellCallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *new ast::exps_t); delete $1;}
;

/*
** -*- RECURSIVE FUNCTION CALL -*-
*/
/* To manage foo(a)(b)(c) <=> ((foo(a))(b))(c)
** foo{a}{b}{c} <=> ((foo{a}){b}){c}
** foo{a}(b){c} <=> ((foo{a})(b)){c}
** foo(a){b}(c) <=> ((foo(a)){b})(c)
*/
//recursiveFunctionCall :
//simpleFunctionCall LPAREN functionArgs RPAREN		{ $$ = new ast::CallExp(@$, *$1, *$3); }
//| recursiveFunctionCall LPAREN functionArgs RPAREN	{ $$ = new ast::CallExp(@$, *$1, *$3); }
//| simpleFunctionCall LBRACE functionArgs RBRACE		{ $$ = new ast::CellCallExp(@$, *$1, *$3); }
//| recursiveFunctionCall LBRACE functionArgs RBRACE	{ $$ = new ast::CellCallExp(@$, *$1, *$3); }
//;

/*
** -*- FUNCTION ARGS -*-
*/
/* What can be use in a function call */
functionArgs :
variable			{
				  $$ = new ast::exps_t;
				  $$->push_back($1);
				}
| functionCall			{
				  $$ = new ast::exps_t;
				  $$->push_back($1);
				}
| COLON				{
				  $$ = new ast::exps_t;
				  $$->push_back(new ast::ColonVar(@1));
				}
| variableDeclaration		{
				  $$ = new ast::exps_t;
				  $$->push_back($1);
				}
| COMMA {
                  $$ = new ast::exps_t;
				  $$->push_back(new ast::NilExp(@1));
				  $$->push_back(new ast::NilExp(@1));
                  }
| COMMA variable	{
				  $$ = new ast::exps_t;
				  $$->push_back(new ast::NilExp(@1));
                  $$->push_back($2);
				}
| COMMA functionCall {
				  $$ = new ast::exps_t;
				  $$->push_back(new ast::NilExp(@1));
                  $$->push_back($2);
				}
| COMMA COLON	{
				  $$ = new ast::exps_t;
				  $$->push_back(new ast::NilExp(@1));
                  $$->push_back(new ast::ColonVar(@2));
				}
| COMMA variableDeclaration {
				  $$ = new ast::exps_t;
				  $$->push_back(new ast::NilExp(@1));
                  $$->push_back($2);
				}
| functionArgs COMMA {
                  $1->push_back(new ast::NilExp(@2));
				  $$ = $1;
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
				  $1->push_back(new ast::ColonVar(@1));
			      $$ = $1;
				}
| functionArgs COMMA variableDeclaration {
				  $1->push_back($3);
				  $$ = $1;
				}
//| functionArgs COMMA {
//                  $1->push_back(new ast::NilExp(@2));
//				  $$ = $1;
//				}
//| COMMA functionArgs {
//                  $2->insert($2->begin(), new ast::NilExp(@1));
//				  $$ = $2;
//				}
;

/*
** -*- FUNCTION DECLARATION -*-
*/
/* How to declare a function */
functionDeclaration :
FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  ast::exps_t* tmp = new ast::exps_t;
				  tmp->push_back(new ast::SimpleVar(@2, symbol::Symbol(*$2)));
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$4),
							    *new ast::ArrayListVar(@5, *$5),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$7);
				  delete $2;
				  delete $4;
				}
| FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3 ,*$3),
							    *$9);
				  delete $6;
				}
| FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  ast::exps_t* tmp = new ast::exps_t;
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$8);
				  delete $5;
				}
| FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
				  ast::exps_t* tmp = new ast::exps_t;
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$2),
							    *new ast::ArrayListVar(@3, *$3),
							    *new ast::ArrayListVar(@$, *tmp),
							    *$5);
				  delete $2;
				}
| FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  ast::exps_t* tmp = new ast::exps_t;
				  tmp->push_back(new ast::SimpleVar(@2, symbol::Symbol(*$2)));
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$4),
							    *new ast::ArrayListVar(@5, *$5),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$7);
				  delete $2;
				  delete $4;
				}
| FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$6),
							    *new ast::ArrayListVar(@7, *$7),
							    *new ast::ArrayListVar(@3 ,*$3),
							    *$9);
				  delete $6;
				}
| FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  ast::exps_t* tmp = new ast::exps_t;
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$5),
							    *new ast::ArrayListVar(@6, *$6),
							    *new ast::ArrayListVar(@2, *tmp),
							    *$8);
				  delete $5;
				}
| FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END {
				  ast::exps_t* tmp = new ast::exps_t;
				  $$ = new ast::FunctionDec(@$,
							    symbol::Symbol(*$2),
							    *new ast::ArrayListVar(@3, *$3),
							    *new ast::ArrayListVar(@$, *tmp),
							    *$5);
				  delete $2;
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
| LPAREN RPAREN			{ $$ = new ast::exps_t;	}
| /* Epsilon */			{ $$ = new ast::exps_t;	}
;

/*
** -*- ID LIST -*-
*/
/* ID (,ID)* */
idList:
idList COMMA ID			{
				  $1->push_back(new ast::SimpleVar(@3, symbol::Symbol(*$3)));
				  delete $3;
				  $$ = $1;
				}
| ID				{
				  $$ = new ast::exps_t;
				  $$->push_back(new ast::SimpleVar(@$, symbol::Symbol(*$1)));
				  delete $1;
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
expressions			{
                        $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                        $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                        $$ = $1;
                    }
| /* Epsilon */		{
				  ast::exps_t* tmp = new ast::exps_t;
				  #ifdef BUILD_DEBUG_AST
				    tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty function body")));
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
					  delete &($2->getLeft());
					  $2->setLeft(*$1);
					  $2->setLocation(@$);
					  $$ = $2;
					}
| functionCall rightComparable		{
					  delete &($2->getLeft());
					  $2->setLeft(*$1);
					  $2->setLocation(@$);
					  $$ = $2;
					}
;

/*
** -*- RIGHT COMPARABLE -*-
*/
/* rightComparable for comparison */
rightComparable :
/* & */
AND variable				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *$2); }
| AND functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *$2); }
| AND COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, * new ast::ColonVar(@$)); }
/* && */
| ANDAND variable			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *$2); }
| ANDAND functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *$2); }
| ANDAND COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, * new ast::ColonVar(@$)); }
/* | */
| OR variable				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *$2); }
| OR functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *$2); }
| OR COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, * new ast::ColonVar(@$)); }
/* || */
| OROR variable				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *$2); }
| OROR functionCall			{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *$2); }
| OROR COLON				{ $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, * new ast::ColonVar(@$)); }
/* == */
| EQ variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *$2); }
| EQ functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *$2); }
| EQ COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, * new ast::ColonVar(@$)); }
/* ~= */
| NE variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *$2); }
| NE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *$2); }
| NE COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, * new ast::ColonVar(@$)); }
/* > */
| GT variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *$2); }
| GT functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *$2); }
| GT COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, * new ast::ColonVar(@$)); }
/* < */
| LT variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *$2); }
| LT functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *$2); }
| LT COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, * new ast::ColonVar(@$)); }
/* >= */
| GE variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *$2); }
| GE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *$2); }
| GE COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, * new ast::ColonVar(@$)); }
/* <= */
| LE variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *$2); }
| LE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *$2); }
| LE COLON				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::le, * new ast::ColonVar(@$)); }
;

/*
** -*- OPERATIONS -*-
*/
/* Operations */
operation :
variable rightOperand			{
					  delete &($2->getLeft());
					  $2->setLeft(*$1);
					  $2->setLocation(@$);
					  $$ = $2;
					}
| functionCall rightOperand		{
					  delete &($2->getLeft());
					  $2->setLeft(*$1);
					  $2->setLocation(@$);
					  $$ = $2;
					}
| MINUS variable        %prec UMINUS    { if ($2->isDoubleExp()) { $$ = $2->getAs<ast::DoubleExp>()->neg();  $2->setLocation(@$);} else { $$ = new ast::OpExp(@$, *new ast::DoubleExp(@$, 0.0), ast::OpExp::unaryMinus, *$2); } }
| MINUS functionCall    %prec UMINUS    { $$ = new ast::OpExp(@$, *new ast::DoubleExp(@$, 0.0), ast::OpExp::unaryMinus, *$2); }
| PLUS variable				            { $$ = $2; }
| PLUS functionCall			            { $$ = $2; }
| variable POWER variable		        { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| variable POWER functionCall		    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| functionCall POWER variable		    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| functionCall POWER functionCall	    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); }
| variable DOTPOWER variable		    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| variable DOTPOWER functionCall	    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| functionCall DOTPOWER variable	    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| functionCall DOTPOWER functionCall	{ $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); }
| variable QUOTE			            { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_Conjugate_); }
| variable DOTQUOTE			            { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_NonConjugate_); }
| functionCall QUOTE			        { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_Conjugate_); }
| functionCall DOTQUOTE			        { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_NonConjugate_); }
;

/*
** -*- RIGHT OPERAND -*-
*/
/* rightOperand for operation */
rightOperand :
/*   '+'   '.+'   '.+.'?   */
PLUS variable				{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *$2); }
| PLUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *$2); }
/*   '-'   '.-'   */
| MINUS variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *$2); }
| MINUS functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *$2); }
/*   '*'   '.*'   '.*.'   '*.'   */
| TIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *$2); }
| TIMES functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *$2); }
| DOTTIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *$2); }
| DOTTIMES functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *$2); }
| KRONTIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *$2); }
| KRONTIMES functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *$2); }
| CONTROLTIMES variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *$2); }
| CONTROLTIMES functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *$2); }
/*   '/'   './'   './.'   '/.'   */
| RDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *$2); }
| RDIVIDE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *$2); }
| DOTRDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *$2); }
| DOTRDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *$2); }
| KRONRDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *$2); }
| KRONRDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *$2); }
| CONTROLRDIVIDE variable		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *$2); }
| CONTROLRDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *$2); }
/*   '\'   '.\'   '.\.'   '\.'   */
| LDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *$2); }
| LDIVIDE functionCall			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *$2); }
| DOTLDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *$2); }
| DOTLDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *$2); }
| KRONLDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *$2); }
| KRONLDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *$2); }
| CONTROLLDIVIDE variable			{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *$2); }
| CONTROLLDIVIDE functionCall		{ $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *$2); }
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
listableBegin COLON variable		{ $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), *$1, *$3, true); }
| listableBegin COLON functionCall	{ $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), *$1, *$3, true); }
| listableBegin %prec LISTABLE		{ $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), *new ast::DoubleExp(@$, 1.0), *$1); }
;

/*
** -*- VARIABLE -*-
*/
/* Variables */
variable :
NOT variable				%prec NOT	{ $$ = new ast::NotExp(@$, *$2); }
| NOT functionCall			%prec NOT	{ $$ = new ast::NotExp(@$, *$2); }
| variable DOT ID			%prec UPLEVEL	{ $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, symbol::Symbol(*$3))); delete $3;}
| variable DOT keywords 	%prec UPLEVEL	{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| variable DOT functionCall				{
							  $3->setName(new ast::FieldExp(@$, *$1, $3->getName()));
							  $3->setLocation(@$);
							  $$ = $3;
}
| functionCall DOT variable				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT keywords				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT functionCall				{
							  $3->setName(new ast::FieldExp(@$, *$1, $3->getName()));
							  $3->setLocation(@$);
							  $$ = $3;
}
| variable listableEnd					{
    $$ = new ast::ListExp(@$, *$1, *($2->getStep().clone()), *($2->getEnd().clone()), $2->hasExplicitStep());
    delete($2);
}
| functionCall listableEnd		%prec UPLEVEL	{
    $$ = new ast::ListExp(@$, *$1, *($2->getStep().clone()), *($2->getEnd().clone()), $2->hasExplicitStep());
    delete($2);
}
| matrix						{ $$ = $1; }
| cell							{ $$ = $1; }
| operation				%prec UPLEVEL		{ $$ = $1; }
| ID					%prec LISTABLE	{ $$ = new ast::SimpleVar(@$, symbol::Symbol(*$1)); delete $1;}
| VARINT				%prec LISTABLE	{ $$ = new ast::DoubleExp(@$, $1); }
| NUM					%prec LISTABLE	{ $$ = new ast::DoubleExp(@$, $1); }
| VARFLOAT						{ $$ = new ast::DoubleExp(@$, $1); }
| STR							{ $$ = new ast::StringExp(@$, *$1); delete $1;}
| DOLLAR						{ $$ = new ast::DollarVar(@$); }
| BOOLTRUE				%prec BOOLTRUE	{ $$ = new ast::BoolExp(@$, true); }
| BOOLFALSE				%prec BOOLFALSE	{ $$ = new ast::BoolExp(@$, false); }
| LPAREN variable RPAREN				{ $$ = $2; }
| LPAREN variableFields RPAREN				{ $$ = new ast::ArrayListExp(@$, *$2); }
| comparison						{ $$ = $1; }
| variable LPAREN functionArgs RPAREN { $$ = new ast::CallExp(@$, *$1, *$3); }
| functionCall LPAREN functionArgs RPAREN { $$ = new ast::CallExp(@$, *$1, *$3); }
;

/*
** -*- VARIABLE FIELDS -*-
*/
/* variable (, variable)+ */
variableFields :
variableFields COMMA variable		{
					  $1->push_back($3);
					  $$ = $1;
					}
| variableFields COMMA functionCall	{
					  $1->push_back($3);
					  $$ = $1;
					}
| variable COMMA variable		{
					  $$ = new ast::exps_t;
					  $$->push_back($1);
					  $$->push_back($3);
					}
| functionCall COMMA functionCall	{
					  $$ = new ast::exps_t;
					  $$->push_back($1);
					  $$->push_back($3);
					}
| functionCall COMMA variable		{
					  $$ = new ast::exps_t;
					  $$->push_back($1);
					  $$->push_back($3);
					}
| variable COMMA functionCall		{
					  $$ = new ast::exps_t;
					  $$->push_back($1);
					  $$->push_back($3);
}
;

/*
** -*- CELL -*-
*/
cell :
LBRACE matrixOrCellLines RBRACE					{ $$ = new ast::CellExp(@$, *$2); }
| LBRACE EOL matrixOrCellLines RBRACE			{ $$ = new ast::CellExp(@$, *$3); }
| LBRACE matrixOrCellLines matrixOrCellColumns RBRACE			{
								  $2->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::CellExp(@$, *$2);
								}
| LBRACE EOL matrixOrCellLines matrixOrCellColumns RBRACE		{
								  $3->push_back(new ast::MatrixLineExp(@4, *$4));
								  $$ = new ast::CellExp(@$, *$3);
								}
| LBRACE matrixOrCellColumns RBRACE					{
								  ast::exps_t* tmp = new ast::exps_t;
								  tmp->push_back(new ast::MatrixLineExp(@2, *$2));
								  $$ = new ast::CellExp(@$, *tmp);
								}
| LBRACE EOL matrixOrCellColumns RBRACE				{
								  ast::exps_t* tmp = new ast::exps_t;
								  tmp->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::CellExp(@$, *tmp);
                                }
| LBRACE EOL RBRACE						{ ast::exps_t* tmp = new ast::exps_t;$$ = new ast::CellExp(@$, *tmp); }
| LBRACE RBRACE							{ ast::exps_t* tmp = new ast::exps_t;$$ = new ast::CellExp(@$, *tmp); }
;


/*
** -*- MATRIX -*-
*/
/* How Matrix are written */
matrix :
LBRACK matrixOrCellLines RBRACK					{ $$ = new ast::MatrixExp(@$, *$2); }
| LBRACK EOL matrixOrCellLines RBRACK				{ $$ = new ast::MatrixExp(@$, *$3); }
| LBRACK matrixOrCellLines matrixOrCellColumns RBRACK			{
								  $2->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *$2);
								}
| LBRACK EOL matrixOrCellLines matrixOrCellColumns RBRACK		{
								  $3->push_back(new ast::MatrixLineExp(@4, *$4));
								  $$ = new ast::MatrixExp(@$, *$3);
								}
| LBRACK matrixOrCellColumns RBRACK					{
								  ast::exps_t* tmp = new ast::exps_t;
								  tmp->push_back(new ast::MatrixLineExp(@2, *$2));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACK EOL matrixOrCellColumns RBRACK				{
								  ast::exps_t* tmp = new ast::exps_t;
								  tmp->push_back(new ast::MatrixLineExp(@3, *$3));
								  $$ = new ast::MatrixExp(@$, *tmp);
								}
| LBRACK EOL RBRACK						{ ast::exps_t* tmp = new ast::exps_t;$$ = new ast::MatrixExp(@$, *tmp); }
| LBRACK RBRACK							{ ast::exps_t* tmp = new ast::exps_t;$$ = new ast::MatrixExp(@$, *tmp); }
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
					  $$ = new ast::exps_t;
					  $$->push_back($1);
					}
//| matrixOrCellLines lineEnd {}
//| COMMENT EOL {}
;

/*
** -*- MATRIX OR CELL LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is a line ending in a Matrix/Cell */
matrixOrCellLineBreak :
SEMI                            { /* !! Do Nothing !! */ }
| EOL                           { /* !! Do Nothing !! */ }
| matrixOrCellLineBreak EOL     { /* !! Do Nothing !! */ }
| matrixOrCellLineBreak SEMI    { /* !! Do Nothing !! */ }
;

/*
** -*- MATRIX OR CELL LINE -*-
*/
/* Some matrix/cell columns with a special matrix/cell line break at the end */
matrixOrCellLine :
matrixOrCellColumns matrixOrCellLineBreak                               { $$ = new ast::MatrixLineExp(@$, *$1); }
| matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak	{ $$ = new ast::MatrixLineExp(@$, *$1); }
;

/*
** -*- MATRIX OR CELL COLUMNS -*-
*/
/* Matrix or Cell Columns : [variable|functinoCall] ([,|][variable|functionCall])* */
matrixOrCellColumns :
matrixOrCellColumns matrixOrCellColumnsBreak variable       %prec HIGHLEVEL {
                                                                                $1->push_back($3);
                                                                                $$ = $1;
                                                                            }
| matrixOrCellColumns matrixOrCellColumnsBreak functionCall	%prec HIGHLEVEL {
                                                                                $1->push_back($3);
                                                                                $$ = $1;
                                                                            }
| matrixOrCellColumns variable                              %prec HIGHLEVEL {
                                                                                $1->push_back($2);
                                                                                $$ = $1;
                                                                            }
| matrixOrCellColumns functionCall                          %prec HIGHLEVEL {
                                                                                $1->push_back($2);
                                                                                $$ = $1;
                                                                            }
| matrixOrCellColumns COMMENT                               %prec HIGHLEVEL {
                                                                                $1->push_back(new ast::CommentExp(@2, $2));
                                                                                $$ = $1;
                                                                            }
| variable                                                  %prec HIGHLEVEL {
                                                                                $$ = new ast::exps_t;
                                                                                $$->push_back($1);
                                                                            }
| functionCall                                              %prec HIGHLEVEL {
                                                                                $$ = new ast::exps_t;
                                                                                $$->push_back($1);
                                                                            }
| COMMENT                                                                   {
                                                                                $$ = new ast::exps_t;
                                                                                $$->push_back(new ast::CommentExp(@$, $1));
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
variable DOT ID			%prec UPLEVEL		{ $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, symbol::Symbol(*$3))); delete $3;}
| variable DOT keywords	%prec UPLEVEL		{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| variable DOT functionCall                 {
                                                $3->setName(new ast::FieldExp(@$, *$1, $3->getName()));
                                                $3->setLocation(@$);
                                                $$ = $3;
                                            }
| functionCall DOT variable				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT keywords				{ $$ = new ast::FieldExp(@$, *$1, *$3); }
| functionCall DOT functionCall				{
							  $3->setName(new ast::FieldExp(@$, *$1, $3->getName()));
							  $3->setLocation(@$);
							  $$ = $3;
                                            }
| ID					%prec LISTABLE	{ $$ = new ast::SimpleVar(@$, symbol::Symbol(*$1)); delete $1;}
| multipleResults					{ $$ = $1; }
| variable LPAREN functionArgs RPAREN { $$ = new ast::CallExp(@$, *$1, *$3); }
| functionCall LPAREN functionArgs RPAREN { $$ = new ast::CallExp(@$, *$1, *$3); }
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
IF condition then thenBody END                          { $$ = new ast::IfExp(@$, *$2, *$4); }
| IF condition then thenBody else elseBody END          {
    if ($6 != NULL)
    {
        $$ = new ast::IfExp(@$, *$2, *$4, *$6);
    }
    else
    {
       $$ = new ast::IfExp(@$, *$2, *$4);
    }
    }
| IF condition then thenBody elseIfControl END          { $$ = new ast::IfExp(@$, *$2, *$4, *$5); }
;

/*
** -*- THEN BODY -*-
*/
/* Instructions that can be managed inside THEN */
thenBody :
expressions     {
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                }
| /* Epsilon */ {
    ast::exps_t* tmp = new ast::exps_t;
    #ifdef BUILD_DEBUG_AST
    tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty then body")));
    #endif
    $$ = new ast::SeqExp(@$, *tmp);
                }
;

/*
** -*- ELSE BODY -*-
*/
/* Instructions that can be managed inside ELSE */
elseBody :
expressions         {
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                    }
| /* Epsilon */                         {
                                        #ifdef BUILD_DEBUG_AST
                                            ast::exps_t* tmp = new ast::exps_t;
                                            tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty else body")));
                                            $$ = new ast::SeqExp(@$, *tmp);
                                        #else
                                            $$ = NULL;
                                        #endif
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
										ast::exps_t* tmp = new ast::exps_t;
										tmp->push_back(new ast::IfExp(@$, *$2, *$4));
										$$ = new ast::SeqExp(@$, *tmp);
									}
| ELSEIF condition then thenBody else elseBody				{
										ast::exps_t* tmp = new ast::exps_t;
										if( $6 == NULL)
                                        {
                                            tmp->push_back(new ast::IfExp(@$, *$2, *$4));
                                        }
                                        else
                                        {
                                            tmp->push_back(new ast::IfExp(@$, *$2, *$4, *$6));
                                        }
										$$ = new ast::SeqExp(@$, *tmp);

									}
| ELSEIF condition then thenBody elseIfControl				{
										ast::exps_t* tmp = new ast::exps_t;
										tmp->push_back(new ast::IfExp(@$, *$2, *$4, *$5));
										$$ = new ast::SeqExp(@$, *tmp);
									}
;

//| ELSEIF condition then thenBody else elseBody elseIfControl		{ $$ = new ast::CommentExp(@$, new std::wstring("!! FIXME !! Elseif additionnal control ??")); }


/*
** -*- SELECT CONTROL -*-
*/
/* Select Case Then End control block */
selectControl :
select selectable selectConditionBreak casesControl END									{ $$ = new ast::SelectExp(@$, *$2, *$4); }
| select selectable selectConditionBreak casesControl defaultCase elseBody END			{
                                        if($6 == NULL)
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$4);
                                        }
                                        else
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$4, *$6);
                                        }
                                    }
| select selectable COMMENT selectConditionBreak casesControl END						{ $$ = new ast::SelectExp(@$, *$2, *$5); delete $3;}
| select selectable COMMENT selectConditionBreak casesControl defaultCase elseBody END	{
                                        if($7 == NULL)
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$5);
                                        }
                                        else
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$5, *$7);
                                        }
                                        delete $3;
                                    }
;

/*
** -*- SELECT -*-
*/
/* Fake Rule : Only for lazy syntax */
select :
SELECT					{ /* !! Do Nothing !! */ }
| SWITCH				{ /* !! Do Nothing !! */ }
;

/*
** -*- defaultCase -*-
*/
/* Fake Rule : Only for lazy syntax */
defaultCase :
else								{ /* !! Do Nothing !! */ }
| OTHERWISE							{ /* !! Do Nothing !! */ }
| OTHERWISE COMMA					{ /* !! Do Nothing !! */ }
| OTHERWISE SEMI					{ /* !! Do Nothing !! */ }
| OTHERWISE EOL						{ /* !! Do Nothing !! */ }
| OTHERWISE COMMA EOL				{ /* !! Do Nothing !! */ }
| OTHERWISE SEMI EOL				{ /* !! Do Nothing !! */ }
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
EOL								{ /* !! Do Nothing !! */ }
| COMMA EOL						{ /* !! Do Nothing !! */ }
| SEMI EOL						{ /* !! Do Nothing !! */ }
| COMMA							{ /* !! Do Nothing !! */ }
| SEMI							{ /* !! Do Nothing !! */ }
;

/*
** -*- CASE CONTROL -*-
*/
/* (Case ... Then ...)+ control block */
casesControl :
CASE variable caseControlBreak caseBody							{
																  $$ = new ast::exps_t;
																  $$->push_back(new ast::CaseExp(@$, *$2, *$4));
																}
| CASE functionCall caseControlBreak caseBody					{
																  $$ = new ast::exps_t;
																  $$->push_back(new ast::CaseExp(@$, *$2, *$4));
																}
| comments CASE variable caseControlBreak caseBody				{
																  $$ = new ast::exps_t;
																  $$->push_back(new ast::CaseExp(@$, *$3, *$5));
																}
| comments CASE functionCall caseControlBreak caseBody          {
																  $$ = new ast::exps_t;
																  $$->push_back(new ast::CaseExp(@$, *$3, *$5));
																}
| casesControl CASE variable caseControlBreak caseBody			{
																  $1->push_back(new ast::CaseExp(@$, *$3, *$5));
																  $$ = $1;
																}
| casesControl CASE functionCall caseControlBreak caseBody		{
																  $1->push_back(new ast::CaseExp(@$, *$3, *$5));
																  $$ = $1;
																}
;

caseBody :
expressions				{
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                        }
| /* Epsilon */			{
                            ast::exps_t* tmp = new ast::exps_t;
                            #ifdef BUILD_DEBUG_AST
                                tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty case body")));
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
| THEN COMMA				{ /* !! Do Nothing !! */ }
| THEN COMMA EOL			{ /* !! Do Nothing !! */ }
| THEN SEMI					{ /* !! Do Nothing !! */ }
| THEN SEMI EOL				{ /* !! Do Nothing !! */ }
| /* Epsilon */		%prec CONTROLBREAK		{ /* !! Do Nothing !! */ }
;

/*
** -*- FOR CONTROL -*-
*/
/* For ... End control block */
forControl :
FOR ID ASSIGN forIterator forConditionBreak forBody END			{ $$ = new ast::ForExp(@$, *new ast::VarDec(@3, symbol::Symbol(*$2), *$4), *$6); delete $2;}
| FOR LPAREN ID ASSIGN forIterator RPAREN forConditionBreak forBody END { $$ = new ast::ForExp(@$, *new ast::VarDec(@4, symbol::Symbol(*$3), *$5), *$8); delete $3;}
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
expressions			{
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                    }
| /* Epsilon */			{
                    ast::exps_t* tmp = new ast::exps_t;
                    #ifdef BUILD_DEBUG_AST
                        tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty for body")));
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
expressions             {
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                        }
| /* Epsilon */			{
                            ast::exps_t* tmp = new ast::exps_t;
                            #ifdef BUILD_DEBUG_AST
                                tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty while body")));
                            #endif
                            $$ = new ast::SeqExp(@$, *tmp);
                        }
;

/*
** -*- WHILE CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'while' condition ending. */
whileConditionBreak :
COMMA                   { /* !! Do Nothing !! */ }
| SEMI                  { /* !! Do Nothing !! */ }
| DO                    { /* !! Do Nothing !! */ }
| DO COMMA              { /* !! Do Nothing !! */ }
| DO SEMI               { /* !! Do Nothing !! */ }
| THEN                  { /* !! Do Nothing !! */ }
| THEN COMMA            { /* !! Do Nothing !! */ }
| THEN SEMI             { /* !! Do Nothing !! */ }
| COMMENT EOL           { delete $1;/* !! Do Nothing !! */ }
| EOL                   { /* !! Do Nothing !! */ }
| COMMA EOL             { /* !! Do Nothing !! */ }
| SEMI EOL              { /* !! Do Nothing !! */ }
| DO EOL                { /* !! Do Nothing !! */ }
| DO COMMA EOL          { /* !! Do Nothing !! */ }
| DO SEMI EOL           { /* !! Do Nothing !! */ }
| THEN EOL              { /* !! Do Nothing !! */ }
| THEN COMMA EOL        { /* !! Do Nothing !! */ }
| THEN SEMI EOL         { /* !! Do Nothing !! */ }
;

/*
** -*- TRY CONTROL -*-
*/
/* try ... catch ... end control block. */
tryControl :
TRY catchBody CATCH catchBody END               { $$ =new ast::TryCatchExp(@$, *$2, *$4); }
| TRY catchBody END                             {
                                                    ast::exps_t* tmp = new ast::exps_t;
                                                    #ifdef BUILD_DEBUG_AST
                                                        tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty catch body")));
                                                    #endif
                                                    $$ = new ast::TryCatchExp(@$, *$2, *new ast::SeqExp(@$, *tmp));
                                                }
;

/*
** -*- CATCH BODY -*-
*/
/* Wich instructions can be used in a catch control. */
catchBody :
expressions                     {
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                                }
| EOL expressions               {
            $2->getLocation().last_line = $2->getExps().back()->getLocation().last_line;
            $2->getLocation().last_column = $2->getExps().back()->getLocation().last_column;
            $$ = $2;
                                }
| SEMI expressions              {
            $2->getLocation().last_line = $2->getExps().back()->getLocation().last_line;
            $2->getLocation().last_column = $2->getExps().back()->getLocation().last_column;
            $$ = $2;
                                }
| COMMA expressions             {
            $2->getLocation().last_line = $2->getExps().back()->getLocation().last_line;
            $2->getLocation().last_column = $2->getExps().back()->getLocation().last_column;
            $$ = $2;
                                }
| EOL                           {
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty catch body")));
                                    #endif
                                    $$ = new ast::SeqExp(@$, *tmp);
                                }
| /* Epsilon */                 {
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty catch body")));
                                    #endif
                                    $$ = new ast::SeqExp(@$, *tmp);
                                }
;

/*
** -*- RETURN CONTROL -*-
*/
/* Make a break in a function or make the variable getting one scope up. */
returnControl :
RETURN				{ $$ = new ast::ReturnExp(@$); }
| RETURN variable   { $$ = new ast::ReturnExp(@$, $2); }
| RETURN functionCall   { $$ = new ast::ReturnExp(@$, $2); }
;

/*
** -*- COMMENTS -*-
*/
comments :
COMMENT EOL { delete $1; }
| comments COMMENT EOL { delete $2; }
;

/*
** -*- LINE END -*-
*/
/* Comment can be added also to end a line */
lineEnd :
EOL
| COMMENT EOL { delete $1; }
;

/*
** -*- KEYWORDS -*-
*/
/* Some token can be used as fields var.if var.then */
keywords :
IF              { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"if")); }
| THEN          { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"then")); }
| ELSE          { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"else")); }
| ELSEIF        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"elseif")); }
| END           { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"end")); }
| SELECT        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"select")); }
| SWITCH        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"switch")); }
| OTHERWISE     { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"otherwise")); }
| CASE          { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"case")); }
| FUNCTION      { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"function")); }
| ENDFUNCTION   { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"endfunction")); }
| FOR           { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"for")); }
| WHILE         { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"while")); }
| DO            { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"do")); }
| BREAK         { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"break")); }
| TRY           { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"try")); }
| CATCH         { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"catch")); }
| RETURN        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"return")); }
;

%%

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
