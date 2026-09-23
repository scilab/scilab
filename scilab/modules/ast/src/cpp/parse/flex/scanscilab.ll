%{
/* -*- C++ -*- */
/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2012 - Scilab Enterprises - Bruno JOFRET
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2018 - Dirk Reusch, Kybernetik Dr. Reusch
 * Copyright (C) 2023 - Dassault Systèmes S.E. - Bruno JOFRET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <stack>

#include "isatty.hxx"
#include "parse.hxx"
#include "parser_private.hxx"

#include "context.hxx"

extern "C"
{
#include "charEncoding.h"
#include "sci_malloc.h"
}

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#endif

static std::stack<int> paren_levels;
static std::stack<int> lambda_levels;
static int classdef_inner_level = 0;

static int comment_level = 0;
static int last_token = 0;
static int linebreak_stored_token = 0;
static bool linebreak_stored_space = FALSE;
static int str_opener_column = 0;
static std::string current_file;
static std::string program_name;

static std::string pstBuffer;

extern void yyerror(std::string);

#define YY_USER_ACTION                          \
    yylloc.first_column = yylloc.last_column; yylloc.last_column += yyleng;
//yylloc.last_column += yyleng;

/* -*- Verbose Special Debug -*- */
// #define DEV
// #define TOKENDEV

#ifdef DEV
#define DEBUG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
#define DEBUG(x) /* Nothing */
#endif

#ifdef DEV
std::string token_to_string(int);
#endif

%}

%option stack
%option noyywrap

%x SIMPLESTRING
%x DOUBLESTRING
%x REGIONCOMMENT
%x LINECOMMENT
%x LINEBREAK

%x MATRIX

%x SHELLMODE
%x BEGINID

%x CLASSDEC

spaces            [ \t\v\f]+
integer           [0-9]+
number            [0-9]+[\.][0-9]*
little            \.[0-9]+
bom               \xEF\xBB\xBF

floating_D		    ({little}|{number}|{integer})[dD][+-]?{integer}
floating_E	  	  ({little}|{number}|{integer})[eE][+-]?{integer}
complexNumber     ({little}|{number}|{integer}|{floating_D}|{floating_E})[ij]
incorrect_number  ({little}|{integer}|{number}|{floating_D}|{floating_E}){id}

hex               [0]x[0-9a-fA-F]+
oct               [0]o[0-7]+


utf2              ([\xC2-\xDF][\x80-\xBF])
utf31             ([\xE0][\xA0-\xBF][\x80-\xBF])
utf32             ([\xE1-\xEC][\x80-\xBF][\x80-\xBF])
utf33             ([\xED][\x80-\x9F][\x80-\xBF])
utf34             ([\xEE-\xEF][\x80-\xBF][\x80-\xBF])
utf41             ([\xF0][\x90-\xBF][\x80-\xBF][\x80-\xBF])
utf42             ([\xF1-\xF3][\x80-\xBF][\x80-\xBF][\x80-\xBF])
utf43             ([\xF4][\x80-\x8F][\x80-\xBF][\x80-\xBF])

utf3              ({utf31}|{utf32}|{utf33}|{utf34})
utf4              ({utf41}|{utf42}|{utf43})

utf               ({utf2}|{utf3}|{utf4})
alpha_id          ([a-zA-Z_!#?]|{utf})([a-zA-Z_0-9!#?$]|{utf})*
dollar_id         ([$]([a-zA-Z_0-9!#?$]|{utf})+)
percent_id        ([%]([a-zA-Z_0-9!#?$]|{utf})*)
id                ({alpha_id}|{dollar_id}|{percent_id})

beginid           ^{spaces}*{id}{spaces}([^ \t\v\f(=<>~@,;]|([~@]{spaces}*[^=]?))

newline           ("\r"|"\n"|"\r\n")
blankline         {spaces}+{newline}
emptyline         {newline}({spaces}|[,;])+{newline}
next              \.\.+
char_in_line_comment    [^\r\n]*
char_in_comment         [^\r\n\/*]*

boolnot           ("@"|"~")
booltrue          ("%t"|"%T")
boolfalse         ("%f"|"%F")
booland           ("&")
boolandand        ("&&")
boolor            ("|")
booloror          ("||")

lbrack            "["
rbrack            "]"

lparen            "("
rparen            ")"

lbrace            "{"
rbrace            "}"

dollar            "$"

semicolon         ";"
comma             ","
colon             ":"

startcomment      "//"
startblockcomment "/*"
endblockcomment   "*/"

dquote            "\""
quote             "'"
in_string         [^\"\'\r\n\.]*

dot               "."
dotquote          ".'"
dottimes          ".*"
dotrdivide        "./"
dotldivide        ".\\"
dotpower          (".^"|".**")

plus              "+"
minus             "-"
rdivide           "/"
ldivide           "\\"
times             "*"
power             ("^"|"**")

equal             "="{spaces}*"="
notequal          ("~"{spaces}*"="|"@"{spaces}*"="|"<"{spaces}*">")
lowerthan         "<"
greaterthan       ">"
lowerequal        "<"{spaces}*"="
greaterequal      ">"{spaces}*"="

krontimes         ".*."
kronrdivide       "./."
kronldivide       ".\\."

controltimes      ("*."[^0-9])
controlrdivide    ("/."[^0-9])
controlldivide    ("\\."[^0-9])

assign            "="

arrow             "->"
sharp             "#"

%%
<INITIAL>{bom}/.* {
// BOM found ==> ignored
}

<INITIAL,BEGINID,CLASSDEC>{booltrue}     {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(BOOLTRUE);
}
<INITIAL,BEGINID,CLASSDEC>{boolfalse}    {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(BOOLFALSE);
}

<INITIAL,BEGINID,CLASSDEC>"classdef"    {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinClassdef);
  }
  if (YY_START == BEGINID) yy_pop_state();
  classdef_inner_level = 0;
  yy_push_state(CLASSDEC);
  return scan_throw(CLASSDEF);
}

<CLASSDEC>"enumeration"    {
  if (ParserSingleInstance::getControlStatus() != Parser::WithinClassdef)
  {
    // Should be within a function
    wchar_t *pwText = to_wide_string(yytext);
    if (yytext != NULL && pwText == NULL)
    {
      std::string str = "Can\'t convert \'";
      str += yytext;
      str += "\' to UTF-8";
      BEGIN(INITIAL);
      yyerror(str);
      return scan_throw(FLEX_ERROR);
    }
    yylval.str = new std::wstring(pwText);
    FREE(pwText);
    return scan_throw(ID);
  }
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinEnumeration);
  }
  if (YY_START == BEGINID) yy_pop_state();
  classdef_inner_level++;
  return scan_throw(ENUMERATION);
}

<CLASSDEC>"properties"    {
    if (ParserSingleInstance::getControlStatus() != Parser::WithinClassdef)
  {
    // Should be within a function
    wchar_t *pwText = to_wide_string(yytext);
    if (yytext != NULL && pwText == NULL)
    {
      std::string str = "Can\'t convert \'";
      str += yytext;
      str += "\' to UTF-8";
      BEGIN(INITIAL);
      yyerror(str);
      return scan_throw(FLEX_ERROR);
    }
    yylval.str = new std::wstring(pwText);
    FREE(pwText);
    return scan_throw(ID);
  }
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinProperties);
  }
  if (YY_START == BEGINID) yy_pop_state();
  classdef_inner_level++;
  return scan_throw(PROPERTIES);
}

<CLASSDEC>"methods"    {
    if (ParserSingleInstance::getControlStatus() != Parser::WithinClassdef)
  {
    // Should be within a function
    wchar_t *pwText = to_wide_string(yytext);
    if (yytext != NULL && pwText == NULL)
    {
      std::string str = "Can\'t convert \'";
      str += yytext;
      str += "\' to UTF-8";
      BEGIN(INITIAL);
      yyerror(str);
      return scan_throw(FLEX_ERROR);
    }
    yylval.str = new std::wstring(pwText);
    FREE(pwText);
    return scan_throw(ID);
  }
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinMethods);
  }
  if (YY_START == BEGINID) yy_pop_state();
  classdef_inner_level++;
  return scan_throw(METHODS);
}

<INITIAL,BEGINID,CLASSDEC>"arguments"    {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinArguments);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(ARGUMENTS);
}

<INITIAL,BEGINID,CLASSDEC>"if"            {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinIf);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(IF);
}

<INITIAL,BEGINID,CLASSDEC>"then"          {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(THEN);
}

<INITIAL,BEGINID,CLASSDEC>"else"          {
  if (last_token != DOT)
  {
    // Pop to step out IF
    ParserSingleInstance::popControlStatus();
    ParserSingleInstance::pushControlStatus(Parser::WithinElse);
  }
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(ELSE);
}

<INITIAL,BEGINID,CLASSDEC>"elseif" {
  if (last_token != DOT)
  {
    ParserSingleInstance::popControlStatus();
    ParserSingleInstance::pushControlStatus(Parser::WithinElseIf);
  }
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(ELSEIF);
}

<INITIAL,BEGINID>"end"        {
  if (YY_START == BEGINID) yy_pop_state();
  
  if (paren_levels.size() != 0 && /* within parenthesis */
    last_token != DOT &&
    ((lambda_levels.size() != 0 && paren_levels.top() > 1) /* within lambda */
    || (lambda_levels.size() == 0 && paren_levels.top() > 0)) /*outside lambda */)
  {
    return scan_throw(DOLLAR);  
  }

  if (last_token != DOT)
  {
    ParserSingleInstance::popControlStatus();
  }
  return scan_throw(END);
}

<MATRIX>"end" {
  if (paren_levels.size() != 0 && last_token != DOT)
  {
    return scan_throw(DOLLAR);
  }
  return scan_throw(END);
}

<CLASSDEC>"end" {
  classdef_inner_level--;
  if (classdef_inner_level < 0) yy_pop_state();
  ParserSingleInstance::popControlStatus();
  return scan_throw(END);
}

<INITIAL,BEGINID,CLASSDEC>"select"    {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinSelect);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(SELECT);
}

<INITIAL,BEGINID,CLASSDEC>"switch"    {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinSwitch);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(SWITCH);
}

<INITIAL,BEGINID,CLASSDEC>"otherwise" {
  if (last_token != DOT)
  {
    ParserSingleInstance::popControlStatus();
    ParserSingleInstance::pushControlStatus(Parser::WithinOtherwise);
  }
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(OTHERWISE);
}

<INITIAL,BEGINID,CLASSDEC>"case"        {
  if (last_token != DOT)
  {
    ParserSingleInstance::popControlStatus();
    ParserSingleInstance::pushControlStatus(Parser::WithinCase);
  }
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(CASE);
}

<INITIAL,BEGINID,CLASSDEC>"function" {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinFunction);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(FUNCTION);
}

<INITIAL,BEGINID,CLASSDEC>"endfunction" {
  if (last_token != DOT)
  {
    ParserSingleInstance::popControlStatus();
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level--;
  return scan_throw(ENDFUNCTION);
}

<INITIAL,BEGINID,CLASSDEC>"for" {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinFor);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(FOR);
}

<INITIAL,BEGINID,CLASSDEC>"while"    {
  if (last_token != DOT)
  {
    ParserSingleInstance::pushControlStatus(Parser::WithinWhile);
  }
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(WHILE);
}

<INITIAL,BEGINID,CLASSDEC>"do"        {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(DO);
}

<INITIAL,BEGINID,CLASSDEC>"break"        {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(BREAK);
}

<INITIAL,BEGINID,CLASSDEC>"continue"        {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(CONTINUE);
}

<INITIAL,BEGINID,CLASSDEC>"try" {
  ParserSingleInstance::pushControlStatus(Parser::WithinTry);
  if (YY_START == BEGINID) yy_pop_state();
  if (YY_START == CLASSDEC) classdef_inner_level++;
  return scan_throw(TRY);
}

<INITIAL,BEGINID,CLASSDEC>"catch" {
  // Pop to step out TRY
  ParserSingleInstance::popControlStatus();
  ParserSingleInstance::pushControlStatus(Parser::WithinCatch);
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(CATCH);
}

<INITIAL,BEGINID,CLASSDEC>"return"    {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(RETURN);
}

<INITIAL,BEGINID,CLASSDEC>"resume"    {
  if (YY_START == BEGINID) yy_pop_state();
  return scan_throw(RETURN);
}

<INITIAL>{beginid} {
  DEBUG("yy_push_state(BEGINID)");
  yy_push_state(BEGINID);

  // rewind enforced to avoid "Variable trailing context rule"
  // skip {spaces} and go to the beginning of {id}
  int skipped_prefix_space = 0;
  while (yytext[skipped_prefix_space] == ' ' ||
         yytext[skipped_prefix_space] == '\t' ||
         yytext[skipped_prefix_space] == '\v' ||
         yytext[skipped_prefix_space] == '\f')
  {
    skipped_prefix_space++;
  }
  yyless(skipped_prefix_space);
  yylloc.last_column = skipped_prefix_space + 1;
}

<BEGINID>{id}                        {
  wchar_t *pwText = to_wide_string(yytext);
  if (yytext != NULL && pwText == NULL)
  {
    std::string str = "Can\'t convert \'";
    str += yytext;
    str += "\' to UTF-8";
    BEGIN(INITIAL);
    yyerror(str);
    return scan_throw(FLEX_ERROR);
  }
  yylval.str = new std::wstring(pwText);
  FREE(pwText);
  types::InternalType * pIT = ParserSingleInstance::useRuntimeContext()
    ? symbol::Context::getInstance()->get(symbol::Symbol(*yylval.str)) : nullptr;
  if (pIT && pIT->isCallable() && ParserSingleInstance::getControlStatus() != Parser::WithinArguments)
  {
    DEBUG("yy_push_state(SHELLMODE)");
    yy_push_state(SHELLMODE);
  }
  else
  {
    DEBUG("yy_pop_state()");
    yy_pop_state();
  }
  #ifdef TOKENDEV
  std::cout << "--> [DEBUG] ID : " << yytext << std::endl;
  #endif
  return scan_throw(ID);
}

<INITIAL,MATRIX,CLASSDEC>{boolnot}        return scan_throw(NOT);
<INITIAL,MATRIX,CLASSDEC>{dollar}         return scan_throw(DOLLAR);
<INITIAL,MATRIX,CLASSDEC>{arrow} {
  ParserSingleInstance::pushControlStatus(Parser::WithinLambda);
  lambda_levels.push(0);
  return scan_throw(ARROW);
}
<INITIAL,MATRIX,CLASSDEC>{sharp}          return scan_throw(SHARP);
<INITIAL,MATRIX,CLASSDEC>{booltrue}       return scan_throw(BOOLTRUE);
<INITIAL,MATRIX,CLASSDEC>{boolfalse}      return scan_throw(BOOLFALSE);
<INITIAL,MATRIX,CLASSDEC>{booland}        return scan_throw(AND);
<INITIAL,MATRIX,CLASSDEC>{boolandand}     return scan_throw(ANDAND);
<INITIAL,MATRIX,CLASSDEC>{boolor}         return scan_throw(OR);
<INITIAL,MATRIX,CLASSDEC>{booloror}       return scan_throw(OROR);

<INITIAL>{lparen} {
  if(paren_levels.size() == 0) {
    paren_levels.push(0);
  }
  ++paren_levels.top();

  if (lambda_levels.size()) {
    ++lambda_levels.top();
  }
  return scan_throw(LPAREN);
}
<CLASSDEC>{lparen}    return scan_throw(LPAREN);

<INITIAL>{rparen} {
  // Let Bison report an unmatched closing parenthesis as a syntax error.
  // Calling top() here without a corresponding opening parenthesis invokes
  // deque::back() on the empty container used by std::stack.
  if (paren_levels.empty() || paren_levels.top() == 0) {
    return scan_throw(RPAREN);
  }

  --paren_levels.top();

  if (lambda_levels.size()) {
    --lambda_levels.top();
    if (lambda_levels.top() == 0) {
      ParserSingleInstance::popControlStatus();
      lambda_levels.pop();
    }
  }
  return scan_throw(RPAREN);
}
<CLASSDEC>{rparen}    return scan_throw(RPAREN);

<INITIAL,MATRIX,CLASSDEC>{semicolon}      return scan_throw(SEMI);
<INITIAL,MATRIX,CLASSDEC>{comma}          return scan_throw(COMMA);
<INITIAL,MATRIX,CLASSDEC>{colon}          return scan_throw(COLON);

<INITIAL,MATRIX,CLASSDEC>{lbrace}         {
  yy_push_state(MATRIX);
  paren_levels.push(0);
  ParserSingleInstance::pushControlStatus(Parser::WithinCell);
  return scan_throw(LBRACE);
}

<INITIAL,CLASSDEC>{rbrace}                return scan_throw(RBRACE);

<INITIAL,MATRIX,CLASSDEC>{dotquote}       return scan_throw(DOTQUOTE);
<INITIAL,MATRIX,CLASSDEC>{dottimes}       return scan_throw(DOTTIMES);
<INITIAL,MATRIX,CLASSDEC>{dotrdivide}     return scan_throw(DOTRDIVIDE);
<INITIAL,MATRIX,CLASSDEC>{dotldivide}     return scan_throw(DOTLDIVIDE);
<INITIAL,MATRIX,CLASSDEC>{dotpower}       return scan_throw(DOTPOWER);
<INITIAL,CLASSDEC>{minus}                 return scan_throw(MINUS);
<INITIAL,CLASSDEC>{plus}                  return scan_throw(PLUS);
<INITIAL,MATRIX,CLASSDEC>{times}          return scan_throw(TIMES);
<INITIAL,MATRIX,CLASSDEC>{rdivide}        return scan_throw(RDIVIDE);
<INITIAL,MATRIX,CLASSDEC>{ldivide}        return scan_throw(LDIVIDE);
<INITIAL,MATRIX,CLASSDEC>{power}          return scan_throw(POWER);
<INITIAL,MATRIX,CLASSDEC>{krontimes}      return scan_throw(KRONTIMES);
<INITIAL,MATRIX,CLASSDEC>{kronrdivide}    return scan_throw(KRONRDIVIDE);
<INITIAL,MATRIX,CLASSDEC>{kronldivide}    return scan_throw(KRONLDIVIDE);

<INITIAL,MATRIX,CLASSDEC>{controltimes}   {
  unput(yytext[yyleng - 1]);
  yylloc.last_column--;
  return scan_throw(CONTROLTIMES);
}

<INITIAL,MATRIX,CLASSDEC>{controlrdivide} {
  unput(yytext[yyleng - 1]);
  yylloc.last_column--;
  return scan_throw(CONTROLRDIVIDE);
}

<INITIAL,MATRIX,CLASSDEC>{controlldivide} {
  unput(yytext[yyleng - 1]);
  yylloc.last_column--;
  return scan_throw(CONTROLLDIVIDE);
}

<INITIAL,MATRIX,CLASSDEC>{equal}          return scan_throw(EQ);
<INITIAL,MATRIX,CLASSDEC>{notequal}       return scan_throw(NE);
<INITIAL,MATRIX,CLASSDEC>{lowerthan}      return scan_throw(LT);
<INITIAL,MATRIX,CLASSDEC>{greaterthan}    return scan_throw(GT);
<INITIAL,MATRIX,CLASSDEC>{lowerequal}     return scan_throw(LE);
<INITIAL,MATRIX,CLASSDEC>{greaterequal}   return scan_throw(GE);
<INITIAL,MATRIX,CLASSDEC>{assign}         return scan_throw(ASSIGN);

<INITIAL,MATRIX,CLASSDEC>{lbrack}         {
  DEBUG("yy_push_state(MATRIX)");
  yy_push_state(MATRIX);
  paren_levels.push(0);
  ParserSingleInstance::pushControlStatus(Parser::WithinMatrix);
  return scan_throw(LBRACK);
}

<INITIAL,CLASSDEC>{rbrack}                return scan_throw(RBRACK);
<INITIAL,MATRIX,CLASSDEC>{dot}            return scan_throw(DOT);

<INITIAL,CLASSDEC>{next}                  {
  ParserSingleInstance::pushControlStatus(Parser::WithinDots);
  yy_push_state(LINEBREAK);
}

<INITIAL,MATRIX,CLASSDEC>{integer}        {
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] INTEGER : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(VARINT);
}

<INITIAL,MATRIX,CLASSDEC>{floating_D}     {
  scan_exponent_convert(yytext);
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
  //scan_step();
  return scan_throw(VARFLOAT);
}

<INITIAL,MATRIX,CLASSDEC>{floating_E}     {
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
  //scan_step();
  return scan_throw(VARFLOAT);
}

<INITIAL,MATRIX,CLASSDEC>{complexNumber}		{
  scan_complex_convert(yytext);
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] COMPLEXNUM : " << yytext << std::endl;
#endif
  //scan_step();
  return scan_throw(COMPLEXNUM);
}

<INITIAL,MATRIX,CLASSDEC>[0-9]+[\.]/[\*^\\\/]		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] NUMBER WITH DOT AS LAST CHARACTER : " << yytext << std::endl;
#endif
//  scan_step();
  unput('.');
  yylloc.last_column--;
  return scan_throw(NUM);
}

<INITIAL,MATRIX,CLASSDEC>{number}         {
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] NUMBER : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(NUM);
}

<INITIAL,MATRIX,CLASSDEC>{little}         {
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] LITTLE : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(NUM);
}

<INITIAL,MATRIX,CLASSDEC>{id}             {
  wchar_t *pwText = to_wide_string(yytext);
  if (yytext != NULL && pwText == NULL)
  {
    std::string str = "Can\'t convert \'";
    str += yytext;
    str += "\' to UTF-8";
    BEGIN(INITIAL);
    yyerror(str);
    return scan_throw(FLEX_ERROR);
  }
  yylval.str = new std::wstring(pwText);
  FREE(pwText);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] ID : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(ID);
}

<INITIAL,MATRIX,CLASSDEC>{incorrect_number}   {
  std::string str = "Can\'t convert \'";
  str += yytext;
  str += "\' to a valid number nor identifier";
  yyerror(str);
  return scan_throw(FLEX_ERROR);
}

<INITIAL,MATRIX,SHELLMODE,CLASSDEC>{startblockcomment}    {
  yylval.comment = new std::wstring();
  comment_level = 1;
  ParserSingleInstance::pushControlStatus(Parser::WithinBlockComment);
  yy_push_state(REGIONCOMMENT);
}

<INITIAL,MATRIX,SHELLMODE,CLASSDEC>{startcomment}        {
  pstBuffer.clear();
  yy_push_state(LINECOMMENT);
}

<INITIAL,MATRIX,SHELLMODE,CLASSDEC>{dquote}         {
  pstBuffer.clear();
  str_opener_column = yylloc.first_column;
  #ifdef TOKENDEV
    std::cout << "--> Push State DOUBLESTRING" << std::endl;
  #endif
  yy_push_state(DOUBLESTRING);
}

<INITIAL,MATRIX,SHELLMODE,CLASSDEC>{spaces}{quote}  {
  /*
  ** Can not be Matrix Transposition
  ** Pushing SIMPLESTRING
  */
  pstBuffer.clear();
  /*
  ** The quote is the last byte of the matched text: the leading blanks
  ** are not part of the string.
  */
  str_opener_column = yylloc.last_column - 1;
  #ifdef TOKENDEV
  std::cout << "--> Push State SIMPLESTRING" << std::endl;
  #endif
  yy_push_state(SIMPLESTRING);
}

<INITIAL,MATRIX,SHELLMODE,CLASSDEC>{quote}          {
  /*
  ** Matrix Transposition special behaviour
  ** ID' []' toto()' are transposition call
  */
  if (last_token == ID
      || last_token == RBRACK
      || last_token == RPAREN
      || last_token == RBRACE
      || last_token == VARINT
      || last_token == VARFLOAT
      || last_token == NUM
      || last_token == BOOLTRUE
      || last_token == BOOLFALSE)
  {
    #ifdef TOKENDEV
    std::cout << "--> QUOTE" << std::endl;
    #endif
    return scan_throw(QUOTE);
  }
  else
  {
    pstBuffer.clear();
    str_opener_column = yylloc.first_column;
    #ifdef TOKENDEV
    std::cout << "--> Push State SIMPLESTRING" << std::endl;
    #endif
    yy_push_state(SIMPLESTRING);
  }
}

<INITIAL,MATRIX,CLASSDEC>{spaces}         scan_step();

<INITIAL,CLASSDEC>{newline}               {
  yylloc.last_line += 1;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL) {
    return scan_throw(EOL);
  }
}

<INITIAL,MATRIX,CLASSDEC>{blankline}      {
  yylloc.last_line += 1;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL)
  {
    return scan_throw(EOL);
  }
  scan_throw(EOL);
}

<INITIAL,MATRIX,CLASSDEC>{emptyline}      {
  yylloc.last_line += 2;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL)
  {
    return scan_throw(EOL);
  }
  scan_throw(EOL);
}

<CLASSDEC>
  {
    <<EOF>> {
      scan_reset();
    }
}

<INITIAL>
 {
    <<EOF>> {
      scan_reset();
      return YYEOF;
    }
}

<MATRIX>
 {
  {lparen} {
    ++paren_levels.top();
    return scan_throw(LPAREN);
  }

  {rparen} {
    // The matrix state owns a parenthesis level, but a closing parenthesis
    // may still be unmatched within that state. Preserve the level and let
    // Bison diagnose the unexpected token.
    if (paren_levels.empty() || paren_levels.top() == 0) {
      return scan_throw(RPAREN);
    }

    --paren_levels.top();
    return scan_throw(RPAREN);
  }

  {spaces}+{lparen} {
    unput(yytext[yyleng - 1]);
    --yylloc.last_column;
    if (last_token == ID
        || last_token == RPAREN
        || last_token == QUOTE
        || last_token == VARINT
        || last_token == VARFLOAT
        || last_token == NUM)
    {
      return scan_throw(COMMA);
    }
  }

  {spaces}*{colon}{spaces}*       return scan_throw(COLON);

  {newline} {
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    if(last_token != DOTS && last_token != EOL)
    {
      return scan_throw(EOL);
    }
    scan_throw(EOL);
  }

  {spaces}+{quote} {
    unput('\'');
    yylloc.last_column--;
    if (last_token == ID
        || last_token == RPAREN
        || last_token == QUOTE)
    {
      return scan_throw(COMMA);
    }
  }

  {rbrack}                {
    DEBUG("yy_pop_state()");
    yy_pop_state();
    paren_levels.pop();
    ParserSingleInstance::popControlStatus();
    return scan_throw(RBRACK);
  }

  {rbrace}                {
    yy_pop_state();
    paren_levels.pop();
    ParserSingleInstance::popControlStatus();
    return scan_throw(RBRACE);
  }

  {plus}                |
  {spaces}{plus}{spaces}                {
    // _+_ is always meaning a PLUS token
    // + alone is a plus if and only if it does not
    // fall into the {spaces}{plus} rule (in matrices space is coding)
    // as Flex is greedy the {plus} rule is only applied
    // if no {spaces}{plus} is found
    // Examples:
    // ========
    // [1 + 2 3] must be understood as [(1+2), 3]
    // [1 +2 3] must be understood as [1, 2, 3]
    // [1 +...
    // 2] must be understood as [(1+2)]
    return scan_throw(PLUS);
  }

  {minus}                |
  {spaces}{minus}{spaces}               {
    // _-_ is always meaning a MINUS token
    // - alone is a MINUS only if and only if it does not
    // fall into the {spaces}{minus} rule (in matrices space is coding)
    // as Flex is greedy the {minus} rule is only applied
    // if no {spaces}{minus} is found
    // Examples:
    // ========
    // [1 - 2 3] must be understood as [(1-2), 3]
    // [1 -2 3] must be understood as [1, -2, 3]
    // [1 -...
    // 2] must be understood as [(1-2)]
    return scan_throw(MINUS);
  }

  {spaces}{plus}                        {
    // This rule is made to take into account the coding spaces in matrices.
    // It is important to distinguish between a space coding a matrix column separator
    // and a simple non coding space around the PLUS operator
    // Examples
    // ========
    // [a + b] == [(a + b)]
    // but [a +b] == [a, +b] and plus here is unary and is not removed, as unary plus
    // is not necessary defined for all data types (https://gitlab.com/scilab/scilab/-/issues/15850)
    // A priori, the space *is* coding
    unput('+');
    yylloc.last_column--;
    if (last_token != LBRACK
       && last_token != EOL
       && last_token != SEMI
       && last_token != COMMA
       && last_token != DOTTIMES
       && last_token != DOTRDIVIDE
       && last_token != DOTLDIVIDE
       && last_token != DOTPOWER
       && last_token != MINUS
       && last_token != PLUS
       && last_token != TIMES
       && last_token != RDIVIDE
       && last_token != LDIVIDE
       && last_token != POWER
       && last_token != KRONTIMES
       && last_token != KRONRDIVIDE
       && last_token != KRONLDIVIDE
       && last_token != EQ
       && last_token != NE
       && last_token != LT
       && last_token != GT
       && last_token != LE
       && last_token != GE
      && paren_levels.top() == 0)
    {
      return scan_throw(COMMA);
    }
  }

  {spaces}{minus}                       {
    // See {spaces}{plus} rule for the rationale
    unput('-');
    yylloc.last_column--;
    if (last_token != LBRACK
       && last_token != EOL
       && last_token != SEMI
       && last_token != COMMA
       && last_token != DOTTIMES
       && last_token != DOTRDIVIDE
       && last_token != DOTLDIVIDE
       && last_token != DOTPOWER
       && last_token != MINUS
       && last_token != PLUS
       && last_token != TIMES
       && last_token != RDIVIDE
       && last_token != LDIVIDE
       && last_token != POWER
       && last_token != KRONTIMES
       && last_token != KRONRDIVIDE
       && last_token != KRONLDIVIDE
       && last_token != EQ
       && last_token != NE
       && last_token != LT
       && last_token != GT
       && last_token != LE
       && last_token != GE
       && paren_levels.top() == 0)
    {
       return scan_throw(COMMA);
    }
  }

  .                    {
    std::string str = "Unexpected token \'";
    str += yytext;
    str += "\' within a matrix.";
    BEGIN(INITIAL);
    yyerror(str);
    return scan_throw(FLEX_ERROR);
  }

  /* {next} rules
   * ============
   * Scilab can perform a line continuation with the ..
   * In matrices as space may be coding extra care must be taken when parsing {next}
   * Some states must be preserved to parse next line and to revert to a proper state
   * after the ... /NO_SPACE/ comments or ... /NO_SPACE* comments *NO_SPACE/
   */

  {spaces}*{plus}{next} {
    // This rule is made to take into account a +... without spaces after plus
    // if one simply ignores the next a situation like this could arise
    // Example
    // =======
    // A = [1 +...
    // 2]
    //
    // what is meant by the user [1 +2] ? or [1 + 2]
    // simply ignoring the ... would yield the 1st situation [1, 2]
    // We consider this is NOT proper and instead that the user meant a binary plus
    // split is two lines
    // The same rationale applies to minus.

    linebreak_stored_space = FALSE; // no spaces before ...
    linebreak_stored_token = PLUS; // keep last token to restore
    ParserSingleInstance::pushControlStatus(Parser::WithinDots);
    yy_push_state(LINEBREAK);
    return scan_throw(PLUS);
  }

  {spaces}*{minus}{next} {
    // see {spaces}*{minus}{next} for the rationale

    linebreak_stored_space = FALSE; // no spaces before ...
    linebreak_stored_token = MINUS; // keep last token to restore
    ParserSingleInstance::pushControlStatus(Parser::WithinDots);
    yy_push_state(LINEBREAK);
    return scan_throw(MINUS);
  }

  {next} {
    // Store the state of the previously scanned token for next rule
    // Only considerations of coding spaces is important for the parser

    linebreak_stored_space = FALSE; // no spaces before ...
    linebreak_stored_token = last_token; // keep last token to restore state
    ParserSingleInstance::pushControlStatus(Parser::WithinDots);
    yy_push_state(LINEBREAK);
  }

  {spaces}{next} {
    // Store the state of the previously scanned token for next rule
    // Only considerations of coding spaces is important for the parser

    linebreak_stored_space = TRUE; // no spaces before ...
    linebreak_stored_token = last_token; // keep last token to restore state
    ParserSingleInstance::pushControlStatus(Parser::WithinDots);
    yy_push_state(LINEBREAK);
  }

  <<EOF>>       {
    yy_pop_state();
    scan_reset();
  }
}

<LINEBREAK>
 {
  {newline}                {
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    last_token = linebreak_stored_token;
    if (linebreak_stored_space)
    {
      // This is important to restore coding spaces as if ... was not present
      unput(' ');
      linebreak_stored_space = FALSE;
    }
    yy_pop_state();
    ParserSingleInstance::popControlStatus();
  }

  {startblockcomment}            {
    yylval.comment = new std::wstring();
    comment_level = 1;
    ParserSingleInstance::pushControlStatus(Parser::WithinBlockComment);
    yy_push_state(REGIONCOMMENT);
  }

  {startcomment}            {
    scan_throw(DOTS);
    pstBuffer.clear();
    yy_push_state(LINECOMMENT);
  }

  {spaces}                {
    /* Do nothing... */
  }

  <<EOF>>    {
    yy_pop_state();
    scan_reset();
  }

  .                    {
    // The following case is not handled by the parser
    // a line of code ... /* some multiline
    // comments */ continued here;
    // without the special case telling we are after comments
    // will generate the error as follows:

    // Any characters after ... yields to an error
    std::string str = "Unexpected token \'";
    str += yytext;
    str += "\' after line break with .. or ...";
    yy_pop_state();
    ParserSingleInstance::popControlStatus();
    BEGIN(INITIAL);
    yyerror(str);
    return scan_throw(FLEX_ERROR);
  }
}


<LINECOMMENT>
 {
  {newline}    {
    //yylloc.last_line += 1;
    //yylloc.last_column = 1;
    //scan_step();
    yy_pop_state();
    // loop to manage \n and \r\n
    for (int i = yyleng - 1 ; i >= 0 ; --i)
    {
      //std::cerr << "Unputting i = {" << i << "}" << std::endl;
      //std::cerr << "Unputting {" << yytext[i] << "}" << std::endl;
      unput(yytext[i]);
      yylloc.last_column--;
    }
    // yylloc.first_column is the location of the {newline}
    // remove the size of the comment to have proper location
    // as for <<EOF>> '//' is not part of the comment location
    yylloc.first_column -= pstBuffer.length();

    /*
    ** To forgot comments after lines break
    */
    if (last_token != DOTS)
    {
      //std::cerr << "pstBuffer = {" << *pstBuffer << "}" << std::endl;
      //std::cerr << "pstBuffer->c_str() = {" << pstBuffer->c_str() << "}" << std::endl;
      wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
      //std::wcerr << L"pwstBuffer = W{" << pwstBuffer << L"}" << std::endl;
      if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
      {
        std::string str = "Can\'t convert \'";
        str += pstBuffer.c_str();
        str += "\' to UTF-8";
        pstBuffer.clear();
        BEGIN(INITIAL);
        yyerror(str);
        return scan_throw(FLEX_ERROR);
      }
      yylval.comment = new std::wstring(pwstBuffer);
      pstBuffer.clear();
      FREE (pwstBuffer);
      return scan_throw(COMMENT);
    }
    else
    {
      pstBuffer.clear();
    }
  }

  <<EOF>>    {
    yy_pop_state();
    scan_reset();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
    if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
    {
      std::string str = "Can\'t convert \'";
      str += pstBuffer.c_str();
      str += "\' to UTF-8";
      pstBuffer.clear();
      BEGIN(INITIAL);
      yyerror(str);
      return scan_throw(FLEX_ERROR);
    }
    yylval.comment = new std::wstring(pwstBuffer);
    pstBuffer.clear();
    FREE (pwstBuffer);
    return scan_throw(COMMENT);
  }

  {char_in_line_comment}         {
      // Put the char in a temporary CHAR buffer to go through UTF-8 trouble
      // only translate to WCHAR_T when popping state.
      pstBuffer += yytext;
  }
}


<REGIONCOMMENT>
 {
  {endblockcomment}                {
    --comment_level;
    if (comment_level == 0)
    {
      ParserSingleInstance::popControlStatus();
      yy_pop_state();
      //return scan_throw(BLOCKCOMMENT);
    }
  }

  {startblockcomment}                {
    ++comment_level;
    yy_push_state(REGIONCOMMENT);
  }

  {newline}                    {
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    *yylval.comment += L"\n";
  }

  {char_in_comment}                |
  .                                             {
      wchar_t *pwText = to_wide_string(yytext);
      *yylval.comment += std::wstring(pwText);
      FREE(pwText);
  }

 <<EOF>>                    {
      yy_pop_state();
      scan_reset();
//    std::string str = "unexpected end of file in a comment";
//    scan_error(str);
  }
}


<SIMPLESTRING>
 {
  {dquote}{dquote}               pstBuffer += "\"";
  {dquote}{quote}                pstBuffer += "'";
  {quote}{dquote}                pstBuffer += "\"";
  {quote}{quote}                 pstBuffer += "'";
  {quote}                        {
    yy_pop_state();
    //scan_step();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
    if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
    {
      std::string str = "Can\'t convert \'";
      str += pstBuffer.c_str();
      str += "\' to UTF-8";
      pstBuffer.clear();
      BEGIN(INITIAL);
      yyerror(str);
      return scan_throw(FLEX_ERROR);
    }
    yylval.str = new std::wstring(pwstBuffer);
    pstBuffer.clear();
    FREE(pwstBuffer);
    yylloc.first_column = str_opener_column;
    return scan_throw(STR);
  }

  {dquote}                  {
    pstBuffer.clear();
    BEGIN(INITIAL);
    yyerror("Heterogeneous string detected, starting with \' and ending with \".");
    return scan_throw(FLEX_ERROR);
  }

  {newline}                    {
    pstBuffer.clear();
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    BEGIN(INITIAL);
    ParserSingleInstance::popControlStatus();
    yyerror("Unexpected end of line in a string.");
    return scan_throw(FLEX_ERROR);
  }

  <<EOF>>                    {
    pstBuffer.clear();
    BEGIN(INITIAL);
    yyerror("Unexpected end of file in a string.");
    return scan_throw(FLEX_ERROR);
  }

  {in_string}                        |
  .                                                     {
    //scan_step();
    pstBuffer += yytext;
  }
}


<DOUBLESTRING>
 {
  {dquote}{dquote}               pstBuffer += "\"";
  {dquote}{quote}                pstBuffer += "'";
  {quote}{dquote}                pstBuffer += "\"";
  {quote}{quote}                 pstBuffer += "'";
  {dquote}                      {
    yy_pop_state();
    //scan_step();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
    if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
    {
      std::string str = "Can\'t convert \'";
      str += pstBuffer.c_str();
      str += "\' to UTF-8";
      pstBuffer.clear();
      BEGIN(INITIAL);
      yyerror(str);
      return scan_throw(FLEX_ERROR);
    }
    yylval.str = new std::wstring(pwstBuffer);
    pstBuffer.clear();
    FREE(pwstBuffer);
    yylloc.first_column = str_opener_column;
    return scan_throw(STR);
  }

  {quote}                  {
    pstBuffer.clear();
    BEGIN(INITIAL);
    yyerror("Heterogeneous string detected, starting with \" and ending with \'.");
    return scan_throw(FLEX_ERROR);
  }

  {newline} {
    pstBuffer.clear();
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    ParserSingleInstance::popControlStatus();
    BEGIN(INITIAL);
    yyerror("Unexpected end of line in a string.");
    return scan_throw(FLEX_ERROR);
  }

  <<EOF>>   {
    pstBuffer.clear();
    BEGIN(INITIAL);
    yyerror("Unexpected end of file in a string.");
    return scan_throw(FLEX_ERROR);
  }

  {in_string}         |
  .                   {
    //scan_step();
    pstBuffer += yytext;
  }
}


<SHELLMODE>
 {
    {spaces}                    {
        if (last_token == ID)
        {
          scan_throw(SPACES);
          //return ID;
        }
    }

    {semicolon}                 {
      BEGIN(INITIAL);
      scan_step();
      return scan_throw(SEMI);
    }

    {comma}                     {
      BEGIN(INITIAL);
      scan_step();
      return scan_throw(COMMA);
    }

    {newline}                   {
      BEGIN(INITIAL);
      yylloc.last_line += 1;
      yylloc.last_column = 1;
      scan_step();
      return scan_throw(EOL);
    }

    {assign} {
      if (last_token == STR || last_token == SPACES)
      {
        wchar_t *pwText = to_wide_string(yytext);
        yylval.str = new std::wstring(pwText);
        FREE(pwText);
        return scan_throw(STR);
      }
      else
      {
        BEGIN(INITIAL);
        return scan_throw(ASSIGN);
      }
    }

    {lparen} {
      if (last_token == STR || last_token == SPACES)
      {
        wchar_t *pwText = to_wide_string(yytext);
        yylval.str = new std::wstring(pwText);
        FREE(pwText);
        return scan_throw(STR);
      }
      else
      {
        BEGIN(INITIAL);
        return scan_throw(LPAREN);
      }
    }

    {lowerthan} {
        if (last_token == STR || last_token == SPACES)
        {
          wchar_t *pwText = to_wide_string(yytext);
          yylval.str = new std::wstring(pwText);
          FREE(pwText);
          return scan_throw(STR);
        }
        else
        {
          BEGIN(INITIAL);
          return scan_throw(LT);
        }
    }

    {greaterthan} {
        if (last_token == STR || last_token == SPACES)
        {
          wchar_t *pwText = to_wide_string(yytext);
          yylval.str = new std::wstring(pwText);
          FREE(pwText);
          return scan_throw(STR);
        }
        else
        {
          BEGIN(INITIAL);
          return scan_throw(GT);
        }
    }

    {boolnot} {
        if (last_token == STR || last_token == SPACES)
        {
          wchar_t *pwText = to_wide_string(yytext);
          yylval.str = new std::wstring(pwText);
          FREE(pwText);
          return scan_throw(STR);
        }
        else
        {
          BEGIN(INITIAL);
          return scan_throw(NOT);
        }
    }


    [^ \t\v\f\r\n,;'"]+               {
      wchar_t *pwText = to_wide_string(yytext);
      yylval.str = new std::wstring(pwText);
      FREE(pwText);
      return scan_throw(STR);
    }

    <<EOF>>                     {
      BEGIN(INITIAL);
      scan_reset();
    }

    <*>.                                {
    // Fallback rule
    // anything that has not be caught until here
    std::string str = "Unexpected token \'";
    str += yytext;
    str += "\'";
    BEGIN(INITIAL);
    yyerror(str);
    return scan_throw(FLEX_ERROR);
}
}

%%
void scan_reset() {
  while(YY_START != INITIAL) {
    yy_pop_state();
  }
  classdef_inner_level = 0;
  paren_levels = {};
  lambda_levels = {};
  comment_level = 0;
  last_token = 0;
  linebreak_stored_token = 0;
  linebreak_stored_space = FALSE;
  str_opener_column = 0;
  pstBuffer.clear();
}


int scan_throw(int token) {
  last_token = token;
#ifdef DEV
  std::cout << "--> [DEBUG] Lex=" << token_to_string(YY_START) << " console=" << ParserSingleInstance::getControlStatus() << " TOKEN : " << token_to_string(token);
  std::cout << " @(" << yylloc.first_line << "." << yylloc.first_column << " -> " << yylloc.last_line << "." << yylloc.last_column << ")";
  if (token == ID || token == STR)
  {
    std::cout << " [" << yytext << "]";
  }
  std::cout << std::endl;
#endif
  return token;
}

int get_last_token() {
  return last_token;
}

void scan_step() {
  yylloc.first_line = yylloc.last_line;
  yylloc.first_column = yylloc.last_column;
}

/*
** convert floating numbers to C standard
** 1.2d-3 -> 1.2e-3
** 1.2D-3 -> 1.2e-3
*/
void scan_exponent_convert(char *in)
{
  for (; *in != 'd' && *in != 'D'; ++in);
  *in = 'e';
}

/*
** convert complex floating numbers to C standard
** 1.2i -> 1.2
** 1.2D-3i -> 1.2e-3
*/
void scan_complex_convert(char *in)
{
  for (; *in != 'i' && *in != 'j'; ++in) 
  {
    if (*in == 'd' || *in == 'D') 
    {
      *in = 'e';
    }
  }
  *in = ' ';
}

#ifdef _MSC_VER
int isatty (int desc)
{
  return 0;
}
#endif

#ifdef DEV
std::string token_to_string(int token)
{
  std::string str;
  switch(token)
  {
    case INITIAL :               str = "INITIAL"; break;
    case MATRIX :                str = "MATRIX"; break;
    case BEGINID :               str = "BEGINID"; break;
    case SHELLMODE :             str = "SHELLMODE"; break;
    case CLASSDEC :              str = "CLASSDEC"; break;
    case ENUMERATION :           str = "ENUMERATION"; break;
    case PROPERTIES :            str = "PROPERTIES"; break;
    case METHODS :               str = "METHODS"; break;
    case AND :                   str = "AND"; break;
    case ASSIGN :                str = "ASSIGN"; break;
    case BOOLFALSE :             str = "BOOLFALSE"; break;
    case BOOLTRUE :              str = "BOOLTRUE"; break;
    case BREAK :                 str = "BREAK"; break;
    case CASE :                  str = "CASE"; break;
    case CATCH :                 str = "CATCH"; break;
    case CLASSDEF :              str = "CLASSDEF"; break;
    case COLON :                 str = "COLON"; break;
    case COMMA :                 str = "COMMA"; break;
    case COMMENT :               str = "COMMENT"; break;
    case CONTINUE :              str = "CONTINUE"; break;
    case CONTROLLDIVIDE :        str = "CONTROLLDIVIDE"; break;
    case CONTROLRDIVIDE :        str = "CONTROLRDIVIDE"; break;
    case CONTROLTIMES :          str = "CONTROLTIMES"; break;
    case DO :                    str = "DO"; break;
    case DOLLAR :                str = "DOLLAR"; break;
    case DOT :                   str = "DOT"; break;
    case DOTLDIVIDE :            str = "DOTLDIVIDE"; break;
    case DOTPOWER :              str = "DOTPOWER"; break;
    case DOTQUOTE :              str = "DOTQUOTE"; break;
    case DOTRDIVIDE :            str = "DOTRDIVIDE"; break;
    case DOTS :                  str = "DOTS"; break;
    case DOTTIMES :              str = "DOTTIMES"; break;
    case ELSE :                  str = "ELSE"; break;
    case ELSEIF :                str = "ELSEIF"; break;
    case END :                   str = "END"; break;
    case ENDFUNCTION :           str = "ENDFUNCTION"; break;
    case EOL :                   str = "EOL"; break;
    case EQ :                    str = "EQ"; break;
    case FLEX_ERROR :            str = "FLEX_ERROR"; break;
    case FOR :                   str = "FOR"; break;
    case FUNCTION :              str = "FUNCTION"; break;
    case GE :                    str = "GE"; break;
    case GT :                    str = "GT"; break;
    case ID :                    str = "ID"; break;
    case IF :                    str = "IF"; break;
    case KRONLDIVIDE :           str = "KRONLDIVIDE"; break;
    case KRONRDIVIDE :           str = "KRONRDIVIDE"; break;
    case KRONTIMES :             str = "KRONTIMES"; break;
    case LBRACE :                str = "LBRACE"; break;
    case LBRACK :                str = "LBRACK"; break;
    case LDIVIDE :               str = "LDIVIDE"; break;
    case LE :                    str = "LE"; break;
    case LPAREN :                str = "LPAREN"; break;
    case LT :                    str = "LT"; break;
    case MINUS :                 str = "MINUS"; break;
    case NE :                    str = "NE"; break;
    case NOT :                   str = "NOT"; break;
    case NUM :                   str = "NUM"; break;
    case OR :                    str = "OR"; break;
    case OROR :                  str = "OROR"; break;
    case OTHERWISE :             str = "OTHERWISE"; break;
    case PLUS :                  str = "PLUS"; break;
    case POWER :                 str = "POWER"; break;
    case QUOTE :                 str = "QUOTE"; break;
    case RBRACE :                str = "RBRACE"; break;
    case RBRACK :                str = "RBRACK"; break;
    case RDIVIDE :               str = "RDIVIDE"; break;
    case RETURN :                str = "RETURN"; break;
    case RPAREN :                str = "RPAREN"; break;
    case SELECT :                str = "SELECT"; break;
    case SEMI :                  str = "SEMI"; break;
    case SPACES :                str = "SPACES"; break;
    case STR :                   str = "STR"; break;
    case SWITCH :                str = "SWITCH"; break;
    case THEN :                  str = "THEN"; break;
    case TIMES :                 str = "TIMES"; break;
    case TRY :                   str = "TRY"; break;
    case VARFLOAT :              str = "VARFLOAT"; break;
    case VARINT :                str = "VARINT"; break;
    case WHILE :                 str = "WHILE"; break;
    default :                    str = "UNKNOWN"; break;
  }
  return str;

}
#endif
