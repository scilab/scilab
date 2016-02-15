%{ // -*- C++ -*-
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

%token YYEOF    0       "end of file"

%token DOTS             "line break"

%token EOL              "end of line"
%token SPACES           "spaces"

%token BOOLTRUE         "%t or %T"
%token BOOLFALSE        "%f or %F"

%token QUOTE            "'"
%token NOT              "~ or @"
%token DOLLAR           "$"
%token COMMA            ","
%token COLON            ":"
%token SEMI             ";"
%token LPAREN           "("
%token RPAREN           ")"
%token LBRACK           "["
%token RBRACK           "]"
%token LBRACE           "{"
%token RBRACE           "}"
%token DOT              "."

%token DOTQUOTE         ".'"

%token PLUS             "+"
%token MINUS            "-"
%token TIMES            "*"
%token DOTTIMES         ".*"
%token KRONTIMES        ".*."
%token CONTROLTIMES     "*."
%token RDIVIDE          "/"
%token DOTRDIVIDE       "./"
%token CONTROLRDIVIDE   "/."
%token KRONRDIVIDE      "./."
%token LDIVIDE          "\\"
%token DOTLDIVIDE       ".\\"
%token CONTROLLDIVIDE   "\\."
%token KRONLDIVIDE      ".\\."

%token POWER            "** or ^"
%token DOTPOWER         ".^"

%token EQ               "=="
%token NE               "<> or ~="
%token LT               "<"
%token LE               "<="
%token GT               ">"
%token GE               ">="
%token AND              "&"
%token ANDAND           "&&"
%token OR               "|"
%token OROR             "||"
%token ASSIGN           "="

%token IF               "if"
%token THEN             "then"
%token ELSE             "else"
%token ELSEIF           "elseif"
%token END              "end"

%token SELECT           "select"
%token SWITCH           "switch"
%token CASE             "case"
%token OTHERWISE        "otherwise"

%token FUNCTION         "function"
%token ENDFUNCTION      "endfunction"

%token FOR              "for"

%token WHILE            "while"
%token DO               "do"
%token BREAK            "break"
%token CONTINUE         "continue"

%token TRY              "try"
%token CATCH            "catch"
%token RETURN           "return"

%token FLEX_ERROR

%token <str>        STR             "string"
%token <str>        ID              "identifier"
%token <number>     VARINT          "integer"
%token <number>     VARFLOAT        "float"
%token <number>     NUM             "number"

%token <path>       PATH            "path"

%token <comment>    COMMENT         "line comment"
%token <comment>    BLOCKCOMMENT    "block comment"

%type <t_seq_exp>           expressions
%type <t_list_exp>          recursiveExpression

%type <t_exp>               assignable
%type <t_assignlist_exp>    multipleResults
%type <t_exp>               variable
%type <t_list_exp>          variableFields
%type <t_exp>               expression

%type <t_op_exp>            comparison
%type <t_op_exp>            rightComparable
%type <t_exp>               operation
%type <t_op_exp>            rightOperand

 // IF Control
%type <t_if_exp>            ifControl
%type <t_exp>               condition
%type <t_seq_exp>           thenBody
%type <t_seq_exp>           elseBody
%type <t_seq_exp>           elseIfControl

 // WHILE Control
%type <t_while_exp>         whileControl
%type <t_seq_exp>           whileBody

 // FOR Control
%type <t_for_exp>           forControl
%type <t_exp>               forIterator
%type <t_seq_exp>           forBody

 // RETURN Control
%type <t_return_exp>        returnControl

 // TRY Control
%type<t_try_exp>            tryControl
%type<t_seq_exp>            catchBody

 // SELECT Control
%type<t_select_exp>         selectControl
%type<t_exp>                selectable
%type<t_list_case>          casesControl
%type <t_seq_exp>           caseBody

 // Implicit Function Call
%type <t_call_exp>          implicitFunctionCall
%type <t_string_exp>        implicitCallable

 // Function Call
%type <t_call_exp>          functionCall
 //%type <t_call_exp>       recursiveFunctionCall
%type <t_call_exp>          simpleFunctionCall
%type <t_list_exp>          functionArgs
%type <t_seq_exp>           functionBody

 // Function Declaration
%type <t_function_dec>      functionDeclaration
%type <t_list_var>          functionDeclarationReturns
%type <t_list_var>          functionDeclarationArguments
%type <t_list_var>          idList

 // Variable Declaration
%type <t_assign_exp>        variableDeclaration

 // Implicit List
%type <t_implicit_list>     listableEnd
%type <t_exp>               listableBegin

 // Matrix & Cell
%type<t_matrix_exp>         matrix
%type<t_cell_exp>           cell
%type<t_list_exp>           matrixOrCellColumns
%type<t_matrixline_exp>     matrixOrCellLine
%type<t_list_mline>         matrixOrCellLines

%type<mute>                 expressionLineBreak

%type<t_simple_var>         keywords

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
expressions                     { SetTree($1); print_rules("program", "expressions");}
| EOL expressions               { SetTree($2); print_rules("program", "EOL expressions");}
| expressionLineBreak           {
                                    print_rules("program", "expressionLineBreak");
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty body")));
                                    #endif
                                    SetTree(new ast::SeqExp(@$, *tmp));
                                    delete $1;
                                }
| /* Epsilon */                 {
                                    print_rules("program", "Epsilon");
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
                                                  print_rules("expressions", "recursiveExpression");
                                                  $$ = new ast::SeqExp(@$, *$1);
                                                }
| recursiveExpression expression                {
                                                  print_rules("expressions", "recursiveExpression expression");
                                                  $2->setVerbose(true);
                                                  $1->push_back($2);
                                                  $$ = new ast::SeqExp(@$, *$1);
                                                }
| recursiveExpression expression COMMENT        {
                                                  print_rules("expressions", "recursiveExpression expression COMMENT");
                                                  $2->setVerbose(true);
                                                  $1->push_back($2);
                                                  $1->push_back(new ast::CommentExp(@3, $3));
                                                  $$ = new ast::SeqExp(@$, *$1);
                                                }
| expression                                    {
                                                  print_rules("expressions", "expression");
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  $1->setVerbose(true);
                                                  tmp->push_back($1);
                                                  $$ = new ast::SeqExp(@$, *tmp);
                                                }
| expression COMMENT                            {
                                                  print_rules("expressions", "expression COMMENT");
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
recursiveExpression expression expressionLineBreak    {
                              print_rules("recursiveExpression", "recursiveExpression expression expressionLineBreak");
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
                              print_rules("recursiveExpression", "recursiveExpression expression COMMENT expressionLineBreak");
                              $2->setVerbose($4->bVerbose);
                              $1->push_back($2);
                              @3.columns($4->iNbBreaker);
                              $1->push_back(new ast::CommentExp(@3, $3));
                              $$ = $1;
                              delete $4;
                            }
| expression COMMENT expressionLineBreak        {
                              print_rules("recursiveExpression", "expression COMMENT expressionLineBreak");
                              ast::exps_t* tmp = new ast::exps_t;
                              @2.columns($3->iNbBreaker);
                              $1->setVerbose($3->bVerbose);
                              tmp->push_back($1);
                              tmp->push_back(new ast::CommentExp(@2, $2));
                              $$ = tmp;
                              delete $3;
                            }
| expression expressionLineBreak            {
                              print_rules("recursiveExpression", "expression expressionLineBreak");
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
SEMI                            { $$ = new LineBreakStr(); $$->bVerbose = false; $$->iNbBreaker = @1.last_column;print_rules("expressionLineBreak", "SEMI"); }
| COMMA                         { $$ = new LineBreakStr(); $$->bVerbose = true; $$->iNbBreaker = @1.last_column;print_rules("expressionLineBreak", "COMMA"); }
| EOL                           { $$ = new LineBreakStr(); $$->bVerbose = true; $$->iNbBreaker = 0;print_rules("expressionLineBreak", "expressionLineBreak SEMI"); }
| expressionLineBreak SEMI      { $$ = $1; $$->bVerbose = false || $1->bVerbose; $$->iNbBreaker = @2.last_column;print_rules("expressionLineBreak", "SEMI"); }
| expressionLineBreak COMMA     { $$ = $1; $$->iNbBreaker = @2.last_column;print_rules("expressionLineBreak", "expressionLineBreak COMMA"); }
| expressionLineBreak EOL       { $$ = $1;print_rules("expressionLineBreak", "expressionLineBreak EOL"); }
;

/*
** -*- EXPRESSION -*-
*/
/* Expression or Instruction : quite similar. */
expression :
functionDeclaration                         { $$ = $1; print_rules("expression", "functionDeclaration");}
| functionCall            %prec TOPLEVEL    { $$ = $1; print_rules("expression", "functionCall");}
| variableDeclaration                       { $$ = $1; print_rules("expression", "variableDeclaration");}
| ifControl                                 { $$ = $1; print_rules("expression", "ifControl");}
| selectControl                             { $$ = $1; print_rules("expression", "selectControl");}
| forControl                                { $$ = $1; print_rules("expression", "forControl");}
| whileControl                              { $$ = $1; print_rules("expression", "whileControl");}
| tryControl                                { $$ = $1; print_rules("expression", "tryControl");}
| variable                %prec TOPLEVEL    { $$ = $1; print_rules("expression", "variable");}
| implicitFunctionCall  %prec TOPLEVEL      { $$ = $1; print_rules("expression", "implicitFunctionCall");}
| BREAK                                     { $$ = new ast::BreakExp(@$); print_rules("expression", "BREAK");}
| CONTINUE                                  { $$ = new ast::ContinueExp(@$); print_rules("expression", "CONTINUE");}
| returnControl                             { $$ = $1; print_rules("expression", "returnControl");}
| COMMENT                                   { $$ = new ast::CommentExp(@$, $1); print_rules("expression", "COMMENT");}
| error                            {
    print_rules("expression", "error");
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
implicitFunctionCall implicitCallable        {
                          print_rules("implicitFunctionCall", "implicitFunctionCall implicitCallable");
                          $1->addArg($2);
                          $1->setLocation(@$);
                          $$ = $1;
                        }
| ID implicitCallable                {
                          print_rules("implicitFunctionCall", "ID implicitCallable");
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
ID                      { $$ = new ast::StringExp(@$, *$1); delete $1;print_rules("implicitCallable", "ID");}
| VARINT                {
                              print_rules("implicitCallable", $1);
                              std::wstringstream tmp;
                              tmp << $1;
                              $$ = new ast::StringExp(@$, tmp.str());
                        }
| NUM                   {
                              print_rules("implicitCallable", $1);
                              std::wstringstream tmp;
                              tmp << $1;
                              $$ = new ast::StringExp(@$, tmp.str());
                        }
| VARFLOAT              {
                              print_rules("implicitCallable", $1);
                              std::wstringstream tmp;
                              tmp << $1;
                              $$ = new ast::StringExp(@$, tmp.str());
                        }
| STR                   { $$ = new ast::StringExp(@$, *$1); delete $1;print_rules("implicitCallable", "STR");}
| DOLLAR                { $$ = new ast::StringExp(@$, std::wstring(L"$")); print_rules("implicitCallable", "DOLLAR");}
| BOOLTRUE              { $$ = new ast::StringExp(@$, std::wstring(L"%t")); print_rules("implicitCallable", "BOOLTRUE");}
| BOOLFALSE             { $$ = new ast::StringExp(@$, std::wstring(L"%f")); print_rules("implicitCallable", "BOOLFALSE");}
| implicitCallable DOT ID   {
                              print_rules("implicitCallable", "implicitCallable DOT ID");
                              std::wstringstream tmp;
                              tmp << $1->getValue() << "." << *$3;
                              $$ = new ast::StringExp(@$, tmp.str());
                              delete $3;
                        }
| PATH                  { $$ = new ast::StringExp(@$, *$1); delete $1;print_rules("implicitCallable", "PATH");}
;

/*
** -*- FUNCTION CALL -*-
*/
/* How to call a function or a cell extraction */
functionCall :
simpleFunctionCall              { $$ = $1; print_rules("functionCall", "simpleFunctionCall");}
//| recursiveFunctionCall        %prec FUNCTIONCALL    { $$ = $1; print_rules("functionCall", "recursiveFunctionCall");}
| LPAREN functionCall RPAREN    { $$ = $2; print_rules("functionCall", "LPAREN functionCall RPAREN");}
;

/*
** -*- SIMPLE FUNCTION CALL -*-
*/
/* Usual way to call functions foo(arg1, arg2, arg3)
** or extract cell values foo{arg1, arg2, arg3}
*/
simpleFunctionCall :

ID LPAREN functionArgs RPAREN       { $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *$3); delete $1;print_rules("simpleFunctionCall", "ID LPAREN functionArgs RPAREN");}
| ID LBRACE functionArgs RBRACE     { $$ = new ast::CellCallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *$3); delete $1;print_rules("simpleFunctionCall", "ID LBRACE functionArgs RBRACE");}
| ID LPAREN RPAREN                  { $$ = new ast::CallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *new ast::exps_t); delete $1;print_rules("simpleFunctionCall", "ID LPAREN RPAREN");}
| ID LBRACE RBRACE                  { $$ = new ast::CellCallExp(@$, *new ast::SimpleVar(@1, symbol::Symbol(*$1)), *new ast::exps_t); delete $1;print_rules("simpleFunctionCall", "ID LBRACE RBRACE");}
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
//simpleFunctionCall LPAREN functionArgs RPAREN         { $$ = new ast::CallExp(@$, *$1, *$3); }
//| recursiveFunctionCall LPAREN functionArgs RPAREN    { $$ = new ast::CallExp(@$, *$1, *$3); }
//| simpleFunctionCall LBRACE functionArgs RBRACE       { $$ = new ast::CellCallExp(@$, *$1, *$3); }
//| recursiveFunctionCall LBRACE functionArgs RBRACE    { $$ = new ast::CellCallExp(@$, *$1, *$3); }
//;

/*
** -*- FUNCTION ARGS -*-
*/
/* What can be use in a function call */
functionArgs :
variable                                    {$$ = new ast::exps_t;$$->push_back($1);print_rules("functionArgs", "variable");}
| functionCall                              {$$ = new ast::exps_t;$$->push_back($1);print_rules("functionArgs", "functionCall");}
| COLON                                     {$$ = new ast::exps_t;$$->push_back(new ast::ColonVar(@1));print_rules("functionArgs", "COLON");}
| variableDeclaration                       {$$ = new ast::exps_t;$$->push_back($1);print_rules("functionArgs", "variableDeclaration");}
| COMMA                                     {$$ = new ast::exps_t;$$->push_back(new ast::NilExp(@1));$$->push_back(new ast::NilExp(@1));print_rules("functionArgs", "COMMA");}
| COMMA variable                            {$$ = new ast::exps_t;$$->push_back(new ast::NilExp(@1));$$->push_back($2);print_rules("functionArgs", "COMMA variable");}
| COMMA functionCall                        {$$ = new ast::exps_t;$$->push_back(new ast::NilExp(@1));$$->push_back($2);print_rules("functionArgs", "COMMA functionCall");}
| COMMA COLON                               {$$ = new ast::exps_t;$$->push_back(new ast::NilExp(@1));$$->push_back(new ast::ColonVar(@2));print_rules("functionArgs", "COMMA COLON");}
| COMMA variableDeclaration                 {$$ = new ast::exps_t;$$->push_back(new ast::NilExp(@1));$$->push_back($2);print_rules("functionArgs", "COMMA variableDeclaration");}
| functionArgs COMMA                        {$1->push_back(new ast::NilExp(@2));$$ = $1;print_rules("functionArgs", "functionArgs COMMA");}
| functionArgs COMMA variable               {$1->push_back($3);$$ = $1;print_rules("functionArgs", "functionArgs COMMA variable");}
| functionArgs COMMA functionCall           {$1->push_back($3);$$ = $1;print_rules("functionArgs", "functionArgs COMMA functionCall");}
| functionArgs COMMA COLON                  {$1->push_back(new ast::ColonVar(@1));$$ = $1;print_rules("functionArgs", "functionArgs COMMA COLON");}
| functionArgs COMMA variableDeclaration    {$1->push_back($3);$$ = $1;print_rules("functionArgs", "functionArgs COMMA variableDeclaration");}
//| functionArgs COMMA {
//                  $1->push_back(new ast::NilExp(@2));
//                  $$ = $1;
//                }
//| COMMA functionArgs {
//                  $2->insert($2->begin(), new ast::NilExp(@1));
//                  $$ = $2;
//                }
;

/*
** -*- FUNCTION DECLARATION -*-
*/
/* How to declare a function */
functionDeclaration :
FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
                  print_rules("functionDeclaration", "FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
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
                  print_rules("functionDeclaration", "FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  $$ = new ast::FunctionDec(@$,
                                symbol::Symbol(*$6),
                                *new ast::ArrayListVar(@7, *$7),
                                *new ast::ArrayListVar(@3 ,*$3),
                                *$9);
                  delete $6;
                }
| FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
                  print_rules("functionDeclaration", "FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  ast::exps_t* tmp = new ast::exps_t;
                  $$ = new ast::FunctionDec(@$,
                                symbol::Symbol(*$5),
                                *new ast::ArrayListVar(@6, *$6),
                                *new ast::ArrayListVar(@2, *tmp),
                                *$8);
                  delete $5;
                }
| FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION {
                  print_rules("functionDeclaration", "FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody ENDFUNCTION");
                  ast::exps_t* tmp = new ast::exps_t;
                  $$ = new ast::FunctionDec(@$,
                                symbol::Symbol(*$2),
                                *new ast::ArrayListVar(@3, *$3),
                                *new ast::ArrayListVar(@$, *tmp),
                                *$5);
                  delete $2;
                }
| FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
                  print_rules("functionDeclaration", "FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END ");
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
                  print_rules("functionDeclaration", "FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END");
                  $$ = new ast::FunctionDec(@$,
                                symbol::Symbol(*$6),
                                *new ast::ArrayListVar(@7, *$7),
                                *new ast::ArrayListVar(@3 ,*$3),
                                *$9);
                  delete $6;
                }
| FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END {
                  print_rules("functionDeclaration", "FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody END");
                  ast::exps_t* tmp = new ast::exps_t;
                  $$ = new ast::FunctionDec(@$,
                                symbol::Symbol(*$5),
                                *new ast::ArrayListVar(@6, *$6),
                                *new ast::ArrayListVar(@2, *tmp),
                                *$8);
                  delete $5;
                }
| FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END {
                  print_rules("functionDeclaration", "FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody END");
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
idList  { $$ = $1; print_rules("functionDeclarationReturns", "idList");}
;

/*
** -*- FUNCTION DECLARATION ARGUMENTS -*-
*/
/* Arguments passed to a function in it's declaration. */
functionDeclarationArguments :
LPAREN idList RPAREN        { $$ = $2; print_rules("functionDeclarationArguments", "LPAREN idList RPAREN");}
| LPAREN RPAREN             { $$ = new ast::exps_t;    print_rules("functionDeclarationArguments", "LPAREN RPAREN");}
| /* Epsilon */             { $$ = new ast::exps_t;    print_rules("functionDeclarationArguments", "Epsilon");}
;

/*
** -*- ID LIST -*-
*/
/* ID (,ID)* */
idList:
idList COMMA ID {
                    print_rules("idList", "idList COMMA ID");
                    $1->push_back(new ast::SimpleVar(@3, symbol::Symbol(*$3)));
                    delete $3;
                    $$ = $1;
                }
| ID            {
                    print_rules("idList", "ID");
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
lineEnd         { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "lineEnd");}
| SEMI          { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "SEMI");}
| SEMI EOL      { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "SEMI EOL");}
| COMMA         { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "COMMA");}
| COMMA EOL     { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "COMMA EOL");}
;

/*
** -*- FUNCTION BODY -*-
*/
/* What may content a function */
functionBody :
expressions         {
                        print_rules("functionBody", "expressions");
                        $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                        $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                        $$ = $1;
                    }
| /* Epsilon */     {
                        print_rules("functionBody", "Epsilon");
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
functionCall    %prec HIGHLEVEL     { $$ = $1; print_rules("condition", "functionCall");}
| variable      %prec HIGHLEVEL     { $$ = $1; print_rules("condition", "variable");}
;

/*
** -*- COMPARISON -*-
*/
/* a way to compare two expressions */
comparison :
variable rightComparable        {
                      print_rules("comparison", "variable rightComparable");
                      delete &($2->getLeft());
                      $2->setLeft(*$1);
                      $2->setLocation(@$);
                      $$ = $2;
                    }
| functionCall rightComparable        {
                      print_rules("comparison", "functionCall rightComparable");
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
AND variable            { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *$2); print_rules("rightComparable", "AND variable");}
| AND functionCall      { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *$2); print_rules("rightComparable", "AND functionCall");}
| AND COLON             { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, * new ast::ColonVar(@$)); print_rules("rightComparable", "AND COLON");}
/* && */
| ANDAND variable       { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *$2); print_rules("rightComparable", "ANDAND variable");}
| ANDAND functionCall   { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *$2); print_rules("rightComparable", "ANDAND functionCall");}
| ANDAND COLON          { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, * new ast::ColonVar(@$)); print_rules("rightComparable", "ANDAND COLON");}
/* | */
| OR variable           { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *$2); print_rules("rightComparable", "OR variable");}
| OR functionCall       { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *$2); print_rules("rightComparable", "OR functionCall");}
| OR COLON              { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, * new ast::ColonVar(@$)); print_rules("rightComparable", "OR COLON");}
/* || */
| OROR variable         { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *$2); print_rules("rightComparable", "OROR variable");}
| OROR functionCall     { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *$2); print_rules("rightComparable", "OROR functionCall");}
| OROR COLON            { $$ = new ast::LogicalOpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, * new ast::ColonVar(@$)); print_rules("rightComparable", "OROR COLON");}
/* == */
| EQ variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *$2); print_rules("rightComparable", "EQ variable");}
| EQ functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *$2); print_rules("rightComparable", "EQ functionCall");}
| EQ COLON              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, * new ast::ColonVar(@$)); print_rules("rightComparable", "EQ COLON");}
/* ~= */
| NE variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *$2); print_rules("rightComparable", "NE variable");}
| NE functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *$2); print_rules("rightComparable", "NE functionCall");}
| NE COLON              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, * new ast::ColonVar(@$)); print_rules("rightComparable", "NE COLON");}
/* > */
| GT variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *$2); print_rules("rightComparable", "GT variable");}
| GT functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *$2); print_rules("rightComparable", "GT functionCall");}
| GT COLON              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, * new ast::ColonVar(@$)); print_rules("rightComparable", "GT COLON");}
/* < */
| LT variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *$2); print_rules("rightComparable", "LT variable");}
| LT functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *$2); print_rules("rightComparable", "LT functionCall");}
| LT COLON              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, * new ast::ColonVar(@$)); print_rules("rightComparable", "LT COLON");}
/* >= */
| GE variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *$2); print_rules("rightComparable", "GE variable");}
| GE functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *$2); print_rules("rightComparable", "GE functionCall");}
| GE COLON              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, * new ast::ColonVar(@$)); print_rules("rightComparable", "GE COLON");}
/* <= */
| LE variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *$2); print_rules("rightComparable", "LE variable");}
| LE functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *$2); print_rules("rightComparable", "LE functionCall");}
| LE COLON              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::le, * new ast::ColonVar(@$)); print_rules("rightComparable", "LE COLON");}
;

/*
** -*- OPERATIONS -*-
*/
/* Operations */
operation :
variable rightOperand            {
                      print_rules("operation", "rightOperand");
                      delete &($2->getLeft());
                      $2->setLeft(*$1);
                      $2->setLocation(@$);
                      $$ = $2;
                    }
| functionCall rightOperand        {
                      print_rules("operation", "functionCall rightOperand");
                      delete &($2->getLeft());
                      $2->setLeft(*$1);
                      $2->setLocation(@$);
                      $$ = $2;
                    }
| MINUS variable        %prec UMINUS    { if ($2->isDoubleExp()) { $$ = $2->getAs<ast::DoubleExp>()->neg();  $2->setLocation(@$);} else { $$ = new ast::OpExp(@$, *new ast::DoubleExp(@$, 0.0), ast::OpExp::unaryMinus, *$2); } print_rules("operation", "MINUS variable");}
| MINUS functionCall    %prec UMINUS    { $$ = new ast::OpExp(@$, *new ast::DoubleExp(@$, 0.0), ast::OpExp::unaryMinus, *$2); print_rules("operation", "MINUS functionCall");}
| PLUS variable                         { $$ = $2; print_rules("operation", "PLUS variable");}
| PLUS functionCall                     { $$ = $2; print_rules("operation", "PLUS functionCall");}
| variable POWER variable               { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); print_rules("operation", "variable POWER variable");}
| variable POWER functionCall           { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); print_rules("operation", "variable POWER functionCall");}
| functionCall POWER variable           { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); print_rules("operation", "functionCall POWER variable");}
| functionCall POWER functionCall       { $$ = new ast::OpExp(@$, *$1, ast::OpExp::power, *$3); print_rules("operation", "functionCall POWER functionCall");}
| variable DOTPOWER variable            { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); print_rules("operation", "variable DOTPOWER variable");}
| variable DOTPOWER functionCall        { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); print_rules("operation", "variable DOTPOWER functionCall");}
| functionCall DOTPOWER variable        { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); print_rules("operation", "functionCall DOTPOWER variable");}
| functionCall DOTPOWER functionCall    { $$ = new ast::OpExp(@$, *$1, ast::OpExp::dotpower, *$3); print_rules("operation", "functionCall DOTPOWER functionCall");}
| variable QUOTE                        { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_Conjugate_); print_rules("operation", "variable QUOTE");}
| variable DOTQUOTE                     { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_NonConjugate_); print_rules("operation", "variable DOTQUOTE");}
| functionCall QUOTE                    { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_Conjugate_); print_rules("operation", "functionCall QUOTE");}
| functionCall DOTQUOTE                 { $$ = new ast::TransposeExp(@$, *$1, ast::TransposeExp::_NonConjugate_); print_rules("operation", "functionCall DOTQUOTE");}
;

/*
** -*- RIGHT OPERAND -*-
*/
/* rightOperand for operation */
rightOperand :
/*   '+'   '.+'   '.+.'?   */
PLUS variable                   { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *$2); print_rules("rightOperand", "PLUS variable");}
| PLUS functionCall             { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *$2); print_rules("rightOperand", "PLUS functionCall");}
/*   '-'   '.-'   */
| MINUS variable                { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *$2); print_rules("rightOperand", "MINUS variable");}
| MINUS functionCall            { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *$2); print_rules("rightOperand", "MINUS functionCall");}
/*   '*'   '.*'   '.*.'   '*.'   */
| TIMES variable                { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *$2); print_rules("rightOperand", "TIMES variable");}
| TIMES functionCall            { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *$2); print_rules("rightOperand", "TIMES functionCall");}
| DOTTIMES variable             { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *$2); print_rules("rightOperand", "DOTTIMES variable");}
| DOTTIMES functionCall         { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *$2); print_rules("rightOperand", "DOTTIMES functionCall");}
| KRONTIMES variable            { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *$2); print_rules("rightOperand", "KRONTIMES variable");}
| KRONTIMES functionCall        { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *$2); print_rules("rightOperand", "KRONTIMES functionCall");}
| CONTROLTIMES variable         { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *$2); print_rules("rightOperand", "CONTROLTIMES variable");}
| CONTROLTIMES functionCall     { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *$2); print_rules("rightOperand", "CONTROLTIMES functionCall    ");}
/*   '/'   './'   './.'   '/.'   */
| RDIVIDE variable              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *$2); print_rules("rightOperand", "RDIVIDE variable");}
| RDIVIDE functionCall          { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *$2); print_rules("rightOperand", "RDIVIDE functionCall");}
| DOTRDIVIDE variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *$2); print_rules("rightOperand", "DOTRDIVIDE variable");}
| DOTRDIVIDE functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *$2); print_rules("rightOperand", "DOTRDIVIDE functionCall");}
| KRONRDIVIDE variable          { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *$2); print_rules("rightOperand", "KRONRDIVIDE variable");}
| KRONRDIVIDE functionCall      { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *$2); print_rules("rightOperand", "KRONRDIVIDE functionCall");}
| CONTROLRDIVIDE variable       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *$2); print_rules("rightOperand", "CONTROLRDIVIDE variable");}
| CONTROLRDIVIDE functionCall   { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *$2); print_rules("rightOperand", "CONTROLRDIVIDE functionCall");}
/*   '\'   '.\'   '.\.'   '\.'   */
| LDIVIDE variable              { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *$2); print_rules("rightOperand", "LDIVIDE variable");}
| LDIVIDE functionCall          { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *$2); print_rules("rightOperand", "LDIVIDE functionCall");}
| DOTLDIVIDE variable           { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *$2); print_rules("rightOperand", "DOTLDIVIDE variable");}
| DOTLDIVIDE functionCall       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *$2); print_rules("rightOperand", "DOTLDIVIDE functionCall");}
| KRONLDIVIDE variable          { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *$2); print_rules("rightOperand", "KRONLDIVIDE variable");}
| KRONLDIVIDE functionCall      { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *$2); print_rules("rightOperand", "KRONLDIVIDE functionCall");}
| CONTROLLDIVIDE variable       { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *$2); print_rules("rightOperand", "CONTROLLDIVIDE variable");}
| CONTROLLDIVIDE functionCall   { $$ = new ast::OpExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *$2); print_rules("rightOperand", "CONTROLLDIVIDE functionCall");}
;

/*
** -*- LISTABLE BEGIN -*-
*/
/* May have no stride in the list, assume it is 1. */
listableBegin :
COLON variable          { $$ = $2; print_rules("listableBegin", "COLON variable");}
| COLON functionCall    { $$ = $2; print_rules("listableBegin", "COLON functionCall");}
;

/*
** -*- LISTABLE END -*-
*/
/* Stride parameter or not. */
listableEnd :
listableBegin COLON variable        { $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), *$1, *$3, true); print_rules("listableEnd", "listableBegin COLON variable");}
| listableBegin COLON functionCall  { $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), *$1, *$3, true); print_rules("listableEnd", "listableBegin COLON functionCall");}
| listableBegin %prec LISTABLE      { $$ = new ast::ListExp(@$, *new ast::CommentExp(@$, new std::wstring(L"Should not stay in that state")), *new ast::DoubleExp(@$, 1.0), *$1); print_rules("listableEnd", "listableBegin ");}
;

/*
** -*- VARIABLE -*-
*/
/* Variables */
variable :
NOT variable                %prec NOT       { $$ = new ast::NotExp(@$, *$2); print_rules("variable", "NOT variable");}
| NOT functionCall          %prec NOT       { $$ = new ast::NotExp(@$, *$2); print_rules("variable", "NOT functionCall");}
| variable DOT ID           %prec UPLEVEL   { $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, symbol::Symbol(*$3))); delete $3;print_rules("variable", "variable DOT ID");}
| variable DOT keywords     %prec UPLEVEL   { $$ = new ast::FieldExp(@$, *$1, *$3); print_rules("variable", "variable DOT keywords");}
| variable DOT functionCall                 {
                              print_rules("variable", "variable DOT functionCall");
                              $3->setName(new ast::FieldExp(@$, *$1, $3->getName()));
                              $3->setLocation(@$);
                              $$ = $3;
}
| functionCall DOT ID                       { $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, symbol::Symbol(*$3))); delete $3; print_rules("variable", "functionCall DOT ID");}
| functionCall DOT keywords                 { $$ = new ast::FieldExp(@$, *$1, *$3); print_rules("variable", "functionCall DOT keywords");}
| variable listableEnd                      {
    print_rules("variable", "variable listableEnd");
    $$ = new ast::ListExp(@$, *$1, *($2->getStep().clone()), *($2->getEnd().clone()), $2->hasExplicitStep());
    delete($2);
}
| functionCall listableEnd        %prec UPLEVEL    {
    print_rules("variable", "functionCall listableEnd");
    $$ = new ast::ListExp(@$, *$1, *($2->getStep().clone()), *($2->getEnd().clone()), $2->hasExplicitStep());
    delete($2);
}
| matrix                                    { $$ = $1; print_rules("variable", "matrix");}
| cell                                      { $$ = $1; print_rules("variable", "cell");}
| operation             %prec UPLEVEL       { $$ = $1; print_rules("variable", "operation");}
| ID                    %prec LISTABLE      { $$ = new ast::SimpleVar(@$, symbol::Symbol(*$1)); delete $1;print_rules("variable", "ID");}
| VARINT                %prec LISTABLE      { $$ = new ast::DoubleExp(@$, $1); print_rules("variable", $1);}
| NUM                   %prec LISTABLE      { $$ = new ast::DoubleExp(@$, $1); print_rules("variable", $1);}
| VARFLOAT                                  { $$ = new ast::DoubleExp(@$, $1); print_rules("variable", $1);}
| STR                                       { $$ = new ast::StringExp(@$, *$1); delete $1;print_rules("variable", "STR");}
| DOLLAR                                    { $$ = new ast::DollarVar(@$); print_rules("variable", "DOLLAR");}
| BOOLTRUE              %prec BOOLTRUE      { $$ = new ast::BoolExp(@$, true); print_rules("variable", "BOOLTRUE");}
| BOOLFALSE             %prec BOOLFALSE     { $$ = new ast::BoolExp(@$, false); print_rules("variable", "BOOLFALSE");}
| LPAREN variable RPAREN                    { $$ = $2; print_rules("variable", "LPAREN variable RPAREN");}
| LPAREN variableFields RPAREN              { $$ = new ast::ArrayListExp(@$, *$2); print_rules("variable", "LPAREN variableFields RPAREN");}
| comparison                                { $$ = $1; print_rules("variable", "comparison");}
| variable LPAREN functionArgs RPAREN       { $$ = new ast::CallExp(@$, *$1, *$3); print_rules("variable", "variable LPAREN functionArgs RPAREN");}
| functionCall LPAREN functionArgs RPAREN   { $$ = new ast::CallExp(@$, *$1, *$3); print_rules("variable", "functionCall LPAREN functionArgs RPAREN");}
;

/*
** -*- VARIABLE FIELDS -*-
*/
/* variable (, variable)+ */
variableFields :
variableFields COMMA variable        {
                    print_rules("variableFields", "variableFields COMMA variable");
                      $1->push_back($3);
                      $$ = $1;
                    }
| variableFields COMMA functionCall    {
                    print_rules("variableFields", "variableFields COMMA functionCall");
                      $1->push_back($3);
                      $$ = $1;
                    }
| variable COMMA variable        {
                      print_rules("variableFields", "variable COMMA variable");
                      $$ = new ast::exps_t;
                      $$->push_back($1);
                      $$->push_back($3);
                    }
| functionCall COMMA functionCall    {
                      print_rules("variableFields", "functionCall COMMA functionCall");
                      $$ = new ast::exps_t;
                      $$->push_back($1);
                      $$->push_back($3);
                    }
| functionCall COMMA variable        {
                      print_rules("variableFields", "functionCall COMMA variable");
                      $$ = new ast::exps_t;
                      $$->push_back($1);
                      $$->push_back($3);
                    }
| variable COMMA functionCall        {
                      print_rules("variableFields", "variable COMMA functionCall");
                      $$ = new ast::exps_t;
                      $$->push_back($1);
                      $$->push_back($3);
}
;

/*
** -*- CELL -*-
*/
cell :
LBRACE matrixOrCellLines RBRACE                             { $$ = new ast::CellExp(@$, *$2); print_rules("cell", "LBRACE matrixOrCellLines RBRACE");}
| LBRACE EOL matrixOrCellLines RBRACE                       { $$ = new ast::CellExp(@$, *$3); print_rules("cell", "variable COMMA functionCall");}
| LBRACE matrixOrCellLines matrixOrCellColumns RBRACE       {
                                  print_rules("cell", "LBRACE matrixOrCellLines matrixOrCellColumns RBRACE");
                                  $2->push_back(new ast::MatrixLineExp(@3, *$3));
                                  $$ = new ast::CellExp(@$, *$2);
                                }
| LBRACE EOL matrixOrCellLines matrixOrCellColumns RBRACE   {
                                  print_rules("cell", "LBRACE EOL matrixOrCellLines matrixOrCellColumns RBRACE");
                                  $3->push_back(new ast::MatrixLineExp(@4, *$4));
                                  $$ = new ast::CellExp(@$, *$3);
                                }
| LBRACE matrixOrCellColumns RBRACE                         {
                                  print_rules("cell", "LBRACE matrixOrCellColumns RBRACE");
                                  ast::exps_t* tmp = new ast::exps_t;
                                  tmp->push_back(new ast::MatrixLineExp(@2, *$2));
                                  $$ = new ast::CellExp(@$, *tmp);
                                }
| LBRACE EOL matrixOrCellColumns RBRACE                     {
                                  print_rules("cell", "LBRACE EOL matrixOrCellColumns RBRACE");
                                  ast::exps_t* tmp = new ast::exps_t;
                                  tmp->push_back(new ast::MatrixLineExp(@3, *$3));
                                  $$ = new ast::CellExp(@$, *tmp);
                                }
| LBRACE EOL RBRACE             { ast::exps_t* tmp = new ast::exps_t;$$ = new ast::CellExp(@$, *tmp); print_rules("cell", "LBRACE EOL RBRACE");}
| LBRACE RBRACE                 { ast::exps_t* tmp = new ast::exps_t;$$ = new ast::CellExp(@$, *tmp); print_rules("cell", "LBRACE RBRACE");}
;


/*
** -*- MATRIX -*-
*/
/* How Matrix are written */
matrix :
LBRACK matrixOrCellLines RBRACK                                 {$$ = new ast::MatrixExp(@$, *$2); print_rules("matrix", "LBRACK matrixOrCellLines RBRACK");}
| LBRACK EOL matrixOrCellLines RBRACK                           {$$ = new ast::MatrixExp(@$, *$3); print_rules("matrix", "LBRACK EOL matrixOrCellLines RBRACK");}
| LBRACK matrixOrCellLines matrixOrCellColumns RBRACK           {$2->push_back(new ast::MatrixLineExp(@3, *$3));$$ = new ast::MatrixExp(@$, *$2);print_rules("matrix", "LBRACK matrixOrCellLines matrixOrCellColumns RBRACK");}
| LBRACK EOL matrixOrCellLines matrixOrCellColumns RBRACK       {$3->push_back(new ast::MatrixLineExp(@4, *$4));$$ = new ast::MatrixExp(@$, *$3);print_rules("matrix", "BRACK EOL matrixOrCellLines matrixOrCellColumns RBRACK");}
| LBRACK matrixOrCellColumns RBRACK                             {ast::exps_t* tmp = new ast::exps_t;tmp->push_back(new ast::MatrixLineExp(@2, *$2));$$ = new ast::MatrixExp(@$, *tmp);print_rules("matrix", "LBRACK matrixOrCellColumns RBRACK");}
| LBRACK EOL matrixOrCellColumns RBRACK                         {ast::exps_t* tmp = new ast::exps_t;tmp->push_back(new ast::MatrixLineExp(@3, *$3));$$ = new ast::MatrixExp(@$, *tmp);print_rules("matrix", "LBRACK EOL matrixOrCellColumns RBRACK");}
| LBRACK EOL RBRACK                                             {ast::exps_t* tmp = new ast::exps_t;$$ = new ast::MatrixExp(@$, *tmp); print_rules("matrix", "LBRACK EOL RBRACK");}
| LBRACK RBRACK                                                 {ast::exps_t* tmp = new ast::exps_t;$$ = new ast::MatrixExp(@$, *tmp); print_rules("matrix", "LBRACK RBRACK");}
;

/*
** -*- MATRIX ORC ELL LINES -*-
*/
/* Matrix or Cell Lines : matrixOrCellLine (matrixOrCellline)* */
matrixOrCellLines :
matrixOrCellLines matrixOrCellLine  {$1->push_back($2);$$ = $1;print_rules("matrixOrCellLines", "matrixOrCellLines matrixOrCellLine");}
| matrixOrCellLine                  {$$ = new ast::exps_t;$$->push_back($1);print_rules("matrixOrCellLines", "matrixOrCellLine");}
//| matrixOrCellLines lineEnd {}
//| COMMENT EOL {}
;

/*
** -*- MATRIX OR CELL LINE BREAK -*-
*/
/* Fake Rule : How can we be sure this is a line ending in a Matrix/Cell */
matrixOrCellLineBreak :
SEMI                            { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "SEMI");}
| EOL                           { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "EOL");}
| matrixOrCellLineBreak EOL     { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "matrixOrCellLineBreak EOL");}
| matrixOrCellLineBreak SEMI    { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "matrixOrCellLineBreak SEMI");}
;

/*
** -*- MATRIX OR CELL LINE -*-
*/
/* Some matrix/cell columns with a special matrix/cell line break at the end */
matrixOrCellLine :
matrixOrCellColumns matrixOrCellLineBreak                               { $$ = new ast::MatrixLineExp(@$, *$1); print_rules("matrixOrCellLine", "matrixOrCellColumns matrixOrCellLineBreak ");}
| matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak    { $$ = new ast::MatrixLineExp(@$, *$1); print_rules("matrixOrCellLine", "matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak");}
;

/*
** -*- MATRIX OR CELL COLUMNS -*-
*/
/* Matrix or Cell Columns : [variable|functinoCall] ([,|][variable|functionCall])* */
matrixOrCellColumns :
matrixOrCellColumns matrixOrCellColumnsBreak variable       %prec HIGHLEVEL {$1->push_back($3);$$ = $1;print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak variable");}
| matrixOrCellColumns matrixOrCellColumnsBreak functionCall %prec HIGHLEVEL {$1->push_back($3);$$ = $1;print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak functionCall");}
| matrixOrCellColumns variable                              %prec HIGHLEVEL {$1->push_back($2);$$ = $1;print_rules("matrixOrCellColumns", "matrixOrCellColumns variable");}
| matrixOrCellColumns functionCall                          %prec HIGHLEVEL {$1->push_back($2);$$ = $1;print_rules("matrixOrCellColumns", "matrixOrCellColumns functionCall");}
| matrixOrCellColumns COMMENT                               %prec HIGHLEVEL {$1->push_back(new ast::CommentExp(@2, $2));$$ = $1;print_rules("matrixOrCellColumns", "matrixOrCellColumns COMMENT");}
| variable                                                  %prec HIGHLEVEL {$$ = new ast::exps_t;$$->push_back($1);print_rules("matrixOrCellColumns", "variable");}
| functionCall                                              %prec HIGHLEVEL {$$ = new ast::exps_t;$$->push_back($1);print_rules("matrixOrCellColumns", "functionCall");}
| COMMENT                                                                   {$$ = new ast::exps_t;$$->push_back(new ast::CommentExp(@$, $1));print_rules("matrixOrCellColumns", "COMMENT");}
;

/*
** -*- MATRIX OR CELL COLUMNS BREAK -*-
*/
/* How to tell the column is now ended. */
matrixOrCellColumnsBreak :
matrixOrCellColumnsBreak COMMA  { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "matrixOrCellColumnsBreak COMMA");}
| COMMA                         { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "COMMA");}
;

/*
** -*- VARIABLE DECLARATION -*-
*/
/* How to declare a new variable */
variableDeclaration :
assignable ASSIGN variable              %prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); print_rules("variableDeclaration", "assignable ASSIGN variable");}
| assignable ASSIGN functionCall        %prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); print_rules("variableDeclaration", "assignable ASSIGN functionCall");}
| functionCall ASSIGN variable          %prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); print_rules("variableDeclaration", "functionCall ASSIGN variable");}
| functionCall ASSIGN functionCall      %prec HIGHLEVEL { $$ = new ast::AssignExp(@$, *$1, *$3); print_rules("variableDeclaration", "functionCall ASSIGN functionCall");}
// --> Sugar Syntax for ':' meaning eye .* 1
| assignable ASSIGN COLON                               { $$ = new ast::AssignExp(@$, *$1, *new ast::ColonVar(@3)); print_rules("variableDeclaration", "assignable ASSIGN COLON");}
| functionCall ASSIGN COLON                             { $$ = new ast::AssignExp(@$, *$1, *new ast::ColonVar(@3)); print_rules("variableDeclaration", "functionCall ASSIGN COLON");}
// --> Strange Syntax : a = return (x)
| assignable ASSIGN returnControl                       { $$ = new ast::AssignExp(@$, *$1, *$3); print_rules("variableDeclaration", "assignable ASSIGN returnControl");}
| functionCall ASSIGN returnControl                     { $$ = new ast::AssignExp(@$, *$1, *$3); print_rules("variableDeclaration", "functionCall ASSIGN returnControl");}
;


/*
** -*- ASSIGNABLE -*-
*/
/* What we can assign something to. */
assignable :
variable DOT ID             %prec UPLEVEL       { $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, symbol::Symbol(*$3))); delete $3;print_rules("assignable", "variable DOT ID");}
| variable DOT keywords     %prec UPLEVEL       { $$ = new ast::FieldExp(@$, *$1, *$3); print_rules("assignable", "variable DOT keywords");}
| variable DOT functionCall                     { $3->setName(new ast::FieldExp(@$, *$1, $3->getName()));$3->setLocation(@$);$$ = $3;print_rules("assignable", "variable DOT functionCall");}
| functionCall DOT ID                           { $$ = new ast::FieldExp(@$, *$1, *new ast::SimpleVar(@$, symbol::Symbol(*$3))); delete $3; print_rules("assignable", "functionCall DOT ID");}
| functionCall DOT keywords                     { $$ = new ast::FieldExp(@$, *$1, *$3); print_rules("assignable", "functionCall DOT keywords");}
| ID                        %prec LISTABLE      { $$ = new ast::SimpleVar(@$, symbol::Symbol(*$1)); delete $1;print_rules("assignable", "ID");}
| multipleResults                               { $$ = $1; print_rules("assignable", "multipleResults");}
| variable LPAREN functionArgs RPAREN           { $$ = new ast::CallExp(@$, *$1, *$3); print_rules("assignable", "ariable LPAREN functionArgs RPAREN");}
| functionCall LPAREN functionArgs RPAREN       { $$ = new ast::CallExp(@$, *$1, *$3); print_rules("assignable", "functionCall LPAREN functionArgs RPAREN");}
;

/*
** -*- MULTIPLE RESULTS -*-
*/
multipleResults :
LBRACK matrixOrCellColumns RBRACK   { $$ = new ast::AssignListExp(@$, *$2); print_rules("multipleResults", "LBRACK matrixOrCellColumns RBRACK");}
;


/*
** -*- IF CONTROL -*-
*/
/* If Then Else End control block */
ifControl :
IF condition then thenBody END                          { $$ = new ast::IfExp(@$, *$2, *$4); print_rules("ifControl", "IF condition then thenBody END");}
| IF condition then thenBody else elseBody END          {
    if ($6 != NULL)
    {
        $$ = new ast::IfExp(@$, *$2, *$4, *$6);
    }
    else
    {
       $$ = new ast::IfExp(@$, *$2, *$4);
    }
    print_rules("ifControl", "IF condition then thenBody else elseBody END");
    }
| IF condition then thenBody elseIfControl END          { $$ = new ast::IfExp(@$, *$2, *$4, *$5); print_rules("ifControl", "IF condition then thenBody elseIfControl END");}
;

/*
** -*- THEN BODY -*-
*/
/* Instructions that can be managed inside THEN */
thenBody :
expressions     {
            print_rules("thenBody", "expressions");
            $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
            $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
            $$ = $1;
                }
| /* Epsilon */ {
    print_rules("thenBody", "Epsilon");
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
                        print_rules("elseBody", "expressions");
                        $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                        $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                        $$ = $1;
                    }
| /* Epsilon */     {
                        #ifdef BUILD_DEBUG_AST
                            ast::exps_t* tmp = new ast::exps_t;
                            tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty else body")));
                            $$ = new ast::SeqExp(@$, *tmp);
                        #else
                            $$ = NULL;
                        #endif
                        print_rules("elseBody", "Epsilon");
                    }
;

/*
** -*- IF CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'if' condition ending */
ifConditionBreak :
SEMI            { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "SEMI");}
| SEMI EOL      { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "SEMI EOL");}
| COMMA         { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "COMMA");}
| COMMA EOL     { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "COMMA EOL");}
| EOL           { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "EOL");}
;

/*
** -*- THEN -*-
*/
/* Fake Rule : Only for lazy syntax */
then :
THEN                            { /* !! Do Nothing !! */ print_rules("then", "THEN");}
| ifConditionBreak THEN         { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak THEN");}
| ifConditionBreak THEN EOL     { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak THEN EOL");}
| THEN ifConditionBreak         { /* !! Do Nothing !! */ print_rules("then", "THEN ifConditionBreak");}
| ifConditionBreak              { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak");}
| /* Epsilon */                 { /* !! Do Nothing !! */ print_rules("then", "Epsilon");}
;

/*
** -*- ELSE -*-
*/
/* Fake Rule : Only for lazy syntax */
else :
ELSE                { /* !! Do Nothing !! */ print_rules("else", "ELSE");}
| ELSE COMMA        { /* !! Do Nothing !! */ print_rules("else", "ELSE COMMA");}
| ELSE SEMI         { /* !! Do Nothing !! */ print_rules("else", "ELSE SEMI");}
| ELSE EOL          { /* !! Do Nothing !! */ print_rules("else", "ELSE EOL");}
| ELSE COMMA EOL    { /* !! Do Nothing !! */ print_rules("else", "ELSE COMMA EOL");}
| ELSE SEMI EOL     { /* !! Do Nothing !! */ print_rules("else", "ELSE SEMI EOL");}
;

/*
** -*- ELSEIF CONTROL
*/
/* else if ... then ... */
elseIfControl :
ELSEIF condition then thenBody      {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        tmp->push_back(new ast::IfExp(@$, *$2, *$4));
                                        $$ = new ast::SeqExp(@$, *tmp);
                                    }
| ELSEIF condition then thenBody else elseBody                {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody else elseBody");
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
| ELSEIF condition then thenBody elseIfControl                {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody elseIfControl");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        tmp->push_back(new ast::IfExp(@$, *$2, *$4, *$5));
                                        $$ = new ast::SeqExp(@$, *tmp);
                                    }
;

//| ELSEIF condition then thenBody else elseBody elseIfControl        { $$ = new ast::CommentExp(@$, new std::wstring("!! FIXME !! Elseif additionnal control ??"));}


/*
** -*- SELECT CONTROL -*-
*/
/* Select Case Then End control block */
selectControl :
select selectable selectConditionBreak casesControl END                         { $$ = new ast::SelectExp(@$, *$2, *$4); print_rules("selectControl", "select selectable selectConditionBreak casesControl END");}
| select selectable selectConditionBreak casesControl defaultCase elseBody END  {
                                        if($6 == NULL)
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$4);
                                        }
                                        else
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$4, *$6);
                                        }
                                        print_rules("selectControl", "select selectable selectConditionBreak casesControl defaultCase elseBody END");
                                    }
| select selectable COMMENT selectConditionBreak casesControl END               { $$ = new ast::SelectExp(@$, *$2, *$5); delete $3;print_rules("selectControl", "select selectable COMMENT selectConditionBreak casesControl END");}
| select selectable COMMENT selectConditionBreak casesControl defaultCase elseBody END    {
                                        if($7 == NULL)
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$5);
                                        }
                                        else
                                        {
                                            $$ = new ast::SelectExp(@$, *$2, *$5, *$7);
                                        }
                                        delete $3;
                                        print_rules("selectControl", "select selectable COMMENT selectConditionBreak casesControl defaultCase elseBody END");
                                    }
;

/*
** -*- SELECT -*-
*/
/* Fake Rule : Only for lazy syntax */
select :
SELECT      { /* !! Do Nothing !! */ print_rules("select", "SELECT");}
| SWITCH    { /* !! Do Nothing !! */ print_rules("select", "SWITCH");}
;

/*
** -*- defaultCase -*-
*/
/* Fake Rule : Only for lazy syntax */
defaultCase :
else                    { /* !! Do Nothing !! */ print_rules("defaultCase", "else");}
| OTHERWISE             { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE");}
| OTHERWISE COMMA       { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE COMMA");}
| OTHERWISE SEMI        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE SEMI");}
| OTHERWISE EOL         { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE EOL");}
| OTHERWISE COMMA EOL   { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE COMMA EOL");}
| OTHERWISE SEMI EOL    { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE SEMI EOL");}
;

/*
** -*- SELECTABLE -*-
*/
/* On what can a select bloc be switch. */
selectable :
variable        { $$ = $1; print_rules("selectable", "variable");}
| functionCall  { $$ = $1; print_rules("selectable", "functionCall");}
;

/*
** -*- SELECT CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'select' condition ending. */
selectConditionBreak :
EOL             { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "EOL");}
| COMMA EOL     { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "COMMA EOL");}
| SEMI EOL      { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "SEMI EOL");}
| COMMA         { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "COMMA");}
| SEMI          { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "SEMI");}
;

/*
** -*- CASE CONTROL -*-
*/
/* (Case ... Then ...)+ control block */
casesControl :
CASE variable caseControlBreak caseBody                     {$$ = new ast::exps_t;$$->push_back(new ast::CaseExp(@$, *$2, *$4));print_rules("casesControl", "CASE variable caseControlBreak caseBody");}
| CASE functionCall caseControlBreak caseBody               {$$ = new ast::exps_t;$$->push_back(new ast::CaseExp(@$, *$2, *$4));print_rules("casesControl", "CASE functionCall caseControlBreak caseBody");}
| comments CASE variable caseControlBreak caseBody          {$$ = new ast::exps_t;$$->push_back(new ast::CaseExp(@$, *$3, *$5));print_rules("casesControl", "comments CASE variable caseControlBreak caseBody");}
| comments CASE functionCall caseControlBreak caseBody      {$$ = new ast::exps_t;$$->push_back(new ast::CaseExp(@$, *$3, *$5));print_rules("casesControl", "comments CASE functionCall caseControlBreak caseBody");}
| casesControl CASE variable caseControlBreak caseBody      {$1->push_back(new ast::CaseExp(@$, *$3, *$5));$$ = $1;print_rules("casesControl", "casesControl CASE variable caseControlBreak caseBody");}
| casesControl CASE functionCall caseControlBreak caseBody  {$1->push_back(new ast::CaseExp(@$, *$3, *$5));$$ = $1;print_rules("casesControl", "casesControl CASE functionCall caseControlBreak caseBody");}
;

caseBody :
expressions     {
                    print_rules("caseBody", "expressions");
                    $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                    $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                    $$ = $1;
                }
| /* Epsilon */ {
                    print_rules("caseBody", "Epsilon");
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
THEN                                { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN");}
| COMMA                             { /* !! Do Nothing !! */ print_rules("caseControlBreak", "COMMA");}
| SEMI                              { /* !! Do Nothing !! */ print_rules("caseControlBreak", "SEMI");}
| EOL                               { /* !! Do Nothing !! */ print_rules("caseControlBreak", "EOL");}
| THEN EOL                          { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN EOL");}
| COMMA EOL                         { /* !! Do Nothing !! */ print_rules("caseControlBreak", "COMMA EOL");}
| SEMI EOL                          { /* !! Do Nothing !! */ print_rules("caseControlBreak", "SEMI EOL");}
| THEN COMMA                        { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN COMMA");}
| THEN COMMA EOL                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN COMMA EOL");}
| THEN SEMI                         { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN SEMI");}
| THEN SEMI EOL                     { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN SEMI EOL");}
| /* Epsilon */ %prec CONTROLBREAK  { /* !! Do Nothing !! */ print_rules("caseControlBreak", "Epsilon");}
;

/*
** -*- FOR CONTROL -*-
*/
/* For ... End control block */
forControl :
FOR ID ASSIGN forIterator forConditionBreak forBody END                 { $$ = new ast::ForExp(@$, *new ast::VarDec(@3, symbol::Symbol(*$2), *$4), *$6); delete $2;print_rules("forControl", "FOR ID ASSIGN forIterator forConditionBreak forBody END    ");}
| FOR LPAREN ID ASSIGN forIterator RPAREN forConditionBreak forBody END { $$ = new ast::ForExp(@$, *new ast::VarDec(@4, symbol::Symbol(*$3), *$5), *$8); delete $3;print_rules("forControl", "FOR LPAREN ID ASSIGN forIterator RPAREN forConditionBreak forBody END");}
;

/*
** -*- FOR ITERATOR -*-
*/
/* For loop variable to tell the number of iterations. */
forIterator :
functionCall    %prec UPLEVEL   { $$ = $1; print_rules("forIterator", "functionCall");}
| variable      %prec UPLEVEL   { $$ = $1; print_rules("forIterator", "variable");}
;


/*
** -*- FOR CONDITION BREAK -*-
*/
/* Fake Rule : How can we be sure this is the 'for' condition ending. */
forConditionBreak :
EOL                 { /* !! Do Nothing !! */ print_rules("forConditionBreak", "EOL");}
| SEMI              { /* !! Do Nothing !! */ print_rules("forConditionBreak", "SEMI");}
| SEMI EOL          { /* !! Do Nothing !! */ print_rules("forConditionBreak", "SEMI EOL");}
| COMMA             { /* !! Do Nothing !! */ print_rules("forConditionBreak", "COMMA");}
| COMMA EOL         { /* !! Do Nothing !! */ print_rules("forConditionBreak", "COMMA EOL");}
| DO                { /* !! Do Nothing !! */ print_rules("forConditionBreak", "DO");}
| DO EOL            { /* !! Do Nothing !! */ print_rules("forConditionBreak", "DO EOL");}
| /* Epsilon */     { /* !! Do Nothing !! */ print_rules("forConditionBreak", "Epsilon");}
;

forBody :
expressions     {
                    print_rules("forBody", "expressions");
                    $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                    $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                    $$ = $1;
                }
| /* Epsilon */ {
                    print_rules("forBody", "Epsilon");
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
WHILE condition whileConditionBreak whileBody END   { $$ = new ast::WhileExp(@$, *$2, *$4); print_rules("whileControl", "WHILE condition whileConditionBreak whileBody END");}
;

/*
** -*- WHILE BODY -*-
*/
/* Which instructions can be used in a while loop. */
whileBody :
expressions         {
                        print_rules("whileBody", "expressions");
                        $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                        $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                        $$ = $1;
                    }
| /* Epsilon */     {
                        print_rules("whileBody", "Epsilon");
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
COMMA                   { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "COMMA");}
| SEMI                  { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI");}
| DO                    { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO");}
| DO COMMA              { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO COMMA");}
| DO SEMI               { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO SEMI");}
| THEN                  { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN");}
| THEN COMMA            { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN COMMA");}
| THEN SEMI             { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN SEMI");}
| COMMENT EOL           { delete $1; print_rules("whileConditionBreak", "COMMENT EOL");}
| EOL                   { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "EOL");}
| COMMA EOL             { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "COMMA EOL");}
| SEMI EOL              { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI EOL");}
| DO EOL                { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI EOL");}
| DO COMMA EOL          { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO COMMA EOL");}
| DO SEMI EOL           { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO SEMI EOL");}
| THEN EOL              { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN EOL");}
| THEN COMMA EOL        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN COMMA EOL");}
| THEN SEMI EOL         { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN SEMI EOL");}
;

/*
** -*- TRY CONTROL -*-
*/
/* try ... catch ... end control block. */
tryControl :
TRY catchBody CATCH catchBody END   { $$ =new ast::TryCatchExp(@$, *$2, *$4); print_rules("tryControl", "TRY catchBody CATCH catchBody END");}
| TRY catchBody END                 {
                                        print_rules("tryControl", "TRY catchBody END");
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
expressions         {
                        print_rules("catchBody", "expressions");
                        $1->getLocation().last_line = $1->getExps().back()->getLocation().last_line;
                        $1->getLocation().last_column = $1->getExps().back()->getLocation().last_column;
                        $$ = $1;
                    }
| EOL expressions   {
                        print_rules("catchBody", "EOL expressions");
                        $2->getLocation().last_line = $2->getExps().back()->getLocation().last_line;
                        $2->getLocation().last_column = $2->getExps().back()->getLocation().last_column;
                        $$ = $2;
                    }
| SEMI expressions  {
                        print_rules("catchBody", "SEMI expressions");
                        $2->getLocation().last_line = $2->getExps().back()->getLocation().last_line;
                        $2->getLocation().last_column = $2->getExps().back()->getLocation().last_column;
                        $$ = $2;
                    }
| COMMA expressions {
                        print_rules("catchBody", "COMMA expressions");
                        $2->getLocation().last_line = $2->getExps().back()->getLocation().last_line;
                        $2->getLocation().last_column = $2->getExps().back()->getLocation().last_column;
                        $$ = $2;
                    }
| EOL               {
                        print_rules("catchBody", "EOL");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp(@$, new std::wstring(L"Empty catch body")));
                        #endif
                        $$ = new ast::SeqExp(@$, *tmp);
                    }
| /* Epsilon */     {
                        print_rules("catchBody", "Epsilon");
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
RETURN                  { $$ = new ast::ReturnExp(@$); print_rules("returnControl", "RETURN");}
| RETURN variable       { $$ = new ast::ReturnExp(@$, $2); print_rules("returnControl", "RETURN variable");}
| RETURN functionCall   { $$ = new ast::ReturnExp(@$, $2); print_rules("returnControl", "RETURN functionCall");}
;

/*
** -*- COMMENTS -*-
*/
comments :
COMMENT EOL             { delete $1; print_rules("comments", "COMMENT EOL");}
| comments COMMENT EOL  { delete $2; print_rules("comments", "comments COMMENT EOL");}
;

/*
** -*- LINE END -*-
*/
/* Comment can be added also to end a line */
lineEnd :
EOL             { print_rules("lineEnd", "EOL");}
| COMMENT EOL   { delete $1; print_rules("lineEnd", "COMMENT EOL");}
;

/*
** -*- KEYWORDS -*-
*/
/* Some token can be used as fields var.if var.then */
keywords :
IF              { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"if"));           print_rules("keywords", "IF");}
| THEN          { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"then"));         print_rules("keywords", "THEN");}
| ELSE          { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"else"));         print_rules("keywords", "ELSE");}
| ELSEIF        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"elseif"));       print_rules("keywords", "ELSEIF");}
| END           { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"end"));          print_rules("keywords", "END");}
| SELECT        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"select"));       print_rules("keywords", "SELECT");}
| SWITCH        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"switch"));       print_rules("keywords", "SWITCH");}
| OTHERWISE     { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"otherwise"));    print_rules("keywords", "OTHERWISE");}
| CASE          { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"case"));         print_rules("keywords", "CASE");}
| FUNCTION      { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"function"));     print_rules("keywords", "FUNCTION");}
| ENDFUNCTION   { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"endfunction"));  print_rules("keywords", "ENDFUNCTION");}
| FOR           { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"for"));          print_rules("keywords", "FOR");}
| WHILE         { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"while"));        print_rules("keywords", "WHILE");}
| DO            { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"do"));           print_rules("keywords", "DO");}
| BREAK         { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"break"));        print_rules("keywords", "BREAK");}
| TRY           { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"try"));          print_rules("keywords", "TRY");}
| CATCH         { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"catch"));        print_rules("keywords", "CATCH");}
| RETURN        { $$ = new ast::SimpleVar(@$, symbol::Symbol(L"return"));       print_rules("keywords", "RETURN");}
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

