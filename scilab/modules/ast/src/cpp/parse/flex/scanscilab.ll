%{
/* -*- C++ -*- */
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2012 - Scilab Enterprises - Bruno JOFRET
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

static std::stack<int> paren_levels;

static int comment_level = 0;
static int last_token = 0;
static int linebreak_stored_token = 0;
static bool linebreak_stored_space = FALSE;
static int exit_status = PARSE_ERROR;
static int str_opener_column = 0;
static std::string current_file;
static std::string program_name;

static std::string pstBuffer;

extern void yyerror(std::string);

#define YY_USER_ACTION                          \
    yylloc.first_column = yylloc.last_column; yylloc.last_column += yyleng;
//yylloc.last_column += yyleng;

/* -*- Verbose Special Debug -*- */
//#define DEV
//#define TOKENDEV

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

spaces			[ \t\v\f]+
integer			[0-9]+
number			[0-9]+[\.][0-9]*
little			\.[0-9]+
bom             \xEF\xBB\xBF

floating_D		({little}|{number}|{integer})[dD][+-]?{integer}
floating_E		({little}|{number}|{integer})[eE][+-]?{integer}

hex             [0]x[0-9a-fA-F]+
oct             [0]o[0-7]+


utf2            ([\xC2-\xDF][\x80-\xBF])
utf31           ([\xE0][\xA0-\xBF][\x80-\xBF])
utf32           ([\xE1-\xEC][\x80-\xBF][\x80-\xBF])
utf33           ([\xED][\x80-\x9F][\x80-\xBF])
utf34           ([\xEE-\xEF][\x80-\xBF][\x80-\xBF])
utf41           ([\xF0][\x90-\xBF][\x80-\xBF][\x80-\xBF])
utf42           ([\xF1-\xF3][\x80-\xBF][\x80-\xBF][\x80-\xBF])
utf43           ([\xF4][\x80-\x8F][\x80-\xBF][\x80-\xBF])

utf3            ({utf31}|{utf32}|{utf33}|{utf34})
utf4            ({utf41}|{utf42}|{utf43})

utf             ({utf2}|{utf3}|{utf4})
id              ((([a-zA-Z_%#?]|{utf})([a-zA-Z_0-9#?$]|{utf})*)|([$]([a-zA-Z_0-9#?$]|{utf})+))


newline			("\r"|"\n"|"\r\n")
blankline		{spaces}+{newline}
emptyline       {newline}({spaces}|[,;])+{newline}
next			\.\.+
char_in_line_comment    [^\r\n]*
char_in_comment         [^\r\n\/*]*

boolnot			("@"|"~")
booltrue		("%t"|"%T")
boolfalse		("%f"|"%F")
booland			("&")
boolandand		("&&")
boolor			("|")
booloror		("||")

lbrack			"["
rbrack			"]"

lparen			"("
rparen			")"

lbrace			"{"
rbrace			"}"

dollar			"$"

semicolon		";"
comma			","
colon			":"

startcomment		"//"
startblockcomment	"/*"
endblockcomment		"*/"

dquote			"\""
quote			"'"
in_string               [^\"\'\r\n\.]*

dot             "."
dotquote		".'"
dottimes		".*"
dotrdivide		"./"
dotldivide		".\\"
dotpower		(".^"|".**")

plus			"+"
minus			"-"
rdivide			"/"
ldivide			"\\"
times			"*"
power			("^"|"**")

equal			"="{spaces}*"="
notequal		("~"{spaces}*"="|"@"{spaces}*"="|"<"{spaces}*">")
lowerthan		"<"
greaterthan		">"
lowerequal		"<"{spaces}*"="
greaterequal		">"{spaces}*"="

krontimes		".*."
kronrdivide		"./."
kronldivide		".\\."

controltimes    ("*."[^0-9])
controlrdivide	("/."[^0-9])
controlldivide  ("\\."[^0-9])

assign			"="

%%
<INITIAL>{bom}/.* {
// BOM found ==> ignored
}

<INITIAL,BEGINID>{booltrue}     {
    BEGIN(INITIAL);
    return scan_throw(BOOLTRUE);
}
<INITIAL,BEGINID>{boolfalse}    {
    BEGIN(INITIAL);
    return scan_throw(BOOLFALSE);
}

<INITIAL,BEGINID>"if"            {
	if (last_token != DOT)
    {
        ParserSingleInstance::pushControlStatus(Parser::WithinIf);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(IF);
}

<INITIAL,BEGINID>"then"          {
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(THEN);
}

<INITIAL,BEGINID>"else"          {
	if (last_token != DOT)
    {
        // Pop to step out IF
        ParserSingleInstance::popControlStatus();
        ParserSingleInstance::pushControlStatus(Parser::WithinElse);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
	return scan_throw(ELSE);
}

<INITIAL,BEGINID>"elseif" {
	if (last_token != DOT)
    {
        ParserSingleInstance::popControlStatus();
        ParserSingleInstance::pushControlStatus(Parser::WithinElseIf);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
	return scan_throw(ELSEIF);
}

<INITIAL,BEGINID>"end"		{
	if (last_token != DOT)
    {
        ParserSingleInstance::popControlStatus();
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(END);
}

<INITIAL,BEGINID>"select"	{
	if (last_token != DOT)
    {
        ParserSingleInstance::pushControlStatus(Parser::WithinSelect);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(SELECT);
}

<INITIAL,BEGINID>"switch"	{
	if (last_token != DOT)
    {
        ParserSingleInstance::pushControlStatus(Parser::WithinSwitch);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(SWITCH);
}

<INITIAL,BEGINID>"otherwise" {
	if (last_token != DOT)
    {
        ParserSingleInstance::popControlStatus();
        ParserSingleInstance::pushControlStatus(Parser::WithinOtherwise);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
	return scan_throw(OTHERWISE);
}

<INITIAL,BEGINID>"case"		{
	if (last_token != DOT)
    {
        ParserSingleInstance::popControlStatus();
        ParserSingleInstance::pushControlStatus(Parser::WithinCase);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(CASE);
}

<INITIAL,BEGINID>"function" {
	if (last_token != DOT)
    {
        ParserSingleInstance::pushControlStatus(Parser::WithinFunction);
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
    return scan_throw(FUNCTION);
}

<INITIAL,BEGINID>"endfunction" {
	if (last_token != DOT)
    {
        ParserSingleInstance::popControlStatus();
    }
    DEBUG("BEGIN(INITIAL)");
    BEGIN(INITIAL);
	return scan_throw(ENDFUNCTION);
}

<INITIAL,BEGINID>"for" {
	if (last_token != DOT)
    {
        ParserSingleInstance::pushControlStatus(Parser::WithinFor);
    }
    BEGIN(INITIAL);
    return scan_throw(FOR);
}

<INITIAL,BEGINID>"while"	{
	if (last_token != DOT)
    {
        ParserSingleInstance::pushControlStatus(Parser::WithinWhile);
    }
	BEGIN(INITIAL);
	return scan_throw(WHILE);
}

<INITIAL,BEGINID>"do"		{
	BEGIN(INITIAL);
    return scan_throw(DO);
}

<INITIAL,BEGINID>"break"		{
    	BEGIN(INITIAL);
        return scan_throw(BREAK);
}

<INITIAL,BEGINID>"continue"		{
    	BEGIN(INITIAL);
        return scan_throw(CONTINUE);
}

<INITIAL,BEGINID>"try" {
	ParserSingleInstance::pushControlStatus(Parser::WithinTry);
	BEGIN(INITIAL);
	return scan_throw(TRY);
}

<INITIAL,BEGINID>"catch" {
    // Pop to step out TRY
	ParserSingleInstance::popControlStatus();
	ParserSingleInstance::pushControlStatus(Parser::WithinCatch);
	BEGIN(INITIAL);
	return scan_throw(CATCH);
}

<INITIAL,BEGINID>"return"	{
    BEGIN(INITIAL);
    return scan_throw(RETURN);
}

<INITIAL,BEGINID>"resume"	{
    BEGIN(INITIAL);
    return scan_throw(RETURN);
}

^{spaces}*/({id}){spaces}([^ \t\v\f(=<>~@,;]|([~@]{spaces}*[^=]?)) {
        BEGIN(BEGINID);
}

<BEGINID>
{
    {id}                        {
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
	types::InternalType * pIT = symbol::Context::getInstance()->get(symbol::Symbol(*yylval.str));
        if (pIT && pIT->isCallable())
        {
            BEGIN(SHELLMODE);
        }
        else
        {
            BEGIN(INITIAL);
        }
	return scan_throw(ID);
    }

}

<INITIAL,MATRIX>{boolnot}		{
  return scan_throw(NOT);
}
<INITIAL,MATRIX>{dollar}		{
  return scan_throw(DOLLAR);
}
<INITIAL,MATRIX>{booltrue}		{
  return scan_throw(BOOLTRUE);
}
<INITIAL,MATRIX>{boolfalse}		{
  return scan_throw(BOOLFALSE);
}
<INITIAL,MATRIX>{booland}		{
  return scan_throw(AND);
}
<INITIAL,MATRIX>{boolandand}	{
  return scan_throw(ANDAND);
}
<INITIAL,MATRIX>{boolor}		{
  return scan_throw(OR);
}
<INITIAL,MATRIX>{booloror}		{
  return scan_throw(OROR);
}


<INITIAL>{lparen}		        {
  return scan_throw(LPAREN);
}
<INITIAL>{rparen}		        {
  return scan_throw(RPAREN);
}


<INITIAL,MATRIX>{semicolon}		{
	scan_step();
  return scan_throw(SEMI);
}

<INITIAL,MATRIX>{comma}			{
	scan_step();
  return scan_throw(COMMA);
}

<INITIAL,MATRIX>{colon}			{
  return scan_throw(COLON);
}


<INITIAL,MATRIX>{lbrace}		{
  yy_push_state(MATRIX);
  paren_levels.push(0);
  ParserSingleInstance::pushControlStatus(Parser::WithinCell);
  return scan_throw(LBRACE);
}

{rbrace}                        {
  return scan_throw(RBRACE);
}


<INITIAL,MATRIX>{dotquote}		{
  return scan_throw(DOTQUOTE);
}
<INITIAL,MATRIX>{dottimes}		{
  return scan_throw(DOTTIMES);
}
<INITIAL,MATRIX>{dotrdivide}		{
  return scan_throw(DOTRDIVIDE);
}
<INITIAL,MATRIX>{dotldivide}		{
  return scan_throw(DOTLDIVIDE);
}
<INITIAL,MATRIX>{dotpower}		{
  return scan_throw(DOTPOWER);
}


{minus}					{
  return scan_throw(MINUS);
}
{plus}					{
  return scan_throw(PLUS);
}
<INITIAL,MATRIX>{times}			{
  return scan_throw(TIMES);
}
<INITIAL,MATRIX>{rdivide}		{
  return scan_throw(RDIVIDE);
}
<INITIAL,MATRIX>{ldivide}		{
  return scan_throw(LDIVIDE);
}
<INITIAL,MATRIX>{power}			{
  return scan_throw(POWER);
}

<INITIAL,MATRIX>{krontimes}		{
  return scan_throw(KRONTIMES);
}
<INITIAL,MATRIX>{kronrdivide}		{
  return scan_throw(KRONRDIVIDE);
}
<INITIAL,MATRIX>{kronldivide}		{
  return scan_throw(KRONLDIVIDE);
}


<INITIAL,MATRIX>{controltimes}		{
    unput(yytext[yyleng - 1]);
    return scan_throw(CONTROLTIMES);
}
<INITIAL,MATRIX>{controlrdivide}		{
    unput(yytext[yyleng - 1]);
    return scan_throw(CONTROLRDIVIDE);
}
<INITIAL,MATRIX>{controlldivide}		{
    unput(yytext[yyleng - 1]);
    return scan_throw(CONTROLLDIVIDE);
}


<INITIAL,MATRIX>{equal}			{
  return scan_throw(EQ);
}
<INITIAL,MATRIX>{notequal}		{
  return scan_throw(NE);
}
<INITIAL,MATRIX>{lowerthan}		{
  return scan_throw(LT);
}
<INITIAL,MATRIX>{greaterthan}		{
  return scan_throw(GT);
}
<INITIAL,MATRIX>{lowerequal}		{
  return scan_throw(LE);
}
<INITIAL,MATRIX>{greaterequal}		{
  return scan_throw(GE);
}


<INITIAL,MATRIX>{assign}		{
  return scan_throw(ASSIGN);
 }


<INITIAL,MATRIX>{lbrack}		{
  DEBUG("yy_push_state(MATRIX)");
  yy_push_state(MATRIX);
  paren_levels.push(0);
  ParserSingleInstance::pushControlStatus(Parser::WithinMatrix);
  return scan_throw(LBRACK);
}

<INITIAL>{rbrack}				{
  return scan_throw(RBRACK);
}


<INITIAL,MATRIX>{dot}			{
  return scan_throw(DOT);
}

<INITIAL>{next}                 {
    ParserSingleInstance::pushControlStatus(Parser::WithinDots);
    yy_push_state(LINEBREAK);
}

<INITIAL,MATRIX>{integer}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] INTEGER : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(VARINT);
}


<INITIAL,MATRIX>{floating_D}		{
  scan_exponent_convert(yytext);
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
  //scan_step();
  return scan_throw(VARFLOAT);
}

<INITIAL,MATRIX>{floating_E}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
  //scan_step();
  return scan_throw(VARFLOAT);
}

<INITIAL,MATRIX>[0-9]+[\.]/[\*^\\\/]		{

  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] NUMBER WITH DOT AS LAST CHARACTER : " << yytext << std::endl;
#endif
//  scan_step();
  unput('.');
  yylloc.last_column--;
  return scan_throw(NUM);
}

<INITIAL,MATRIX>{number}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] NUMBER : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(NUM);
}


<INITIAL,MATRIX>{little}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] LITTLE : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(NUM);
}


<INITIAL,MATRIX>{id}			{
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


<INITIAL,MATRIX,SHELLMODE,BEGINID>{startblockcomment}	{
  yylval.comment = new std::wstring();
  comment_level = 1;
  ParserSingleInstance::pushControlStatus(Parser::WithinBlockComment);
  yy_push_state(REGIONCOMMENT);
}


<INITIAL,MATRIX,SHELLMODE,BEGINID>{startcomment}		{
  pstBuffer.clear();
  yy_push_state(LINECOMMENT);
}


<INITIAL,MATRIX,SHELLMODE>{dquote}		{
  pstBuffer.clear();
  str_opener_column = yylloc.first_column;
  yy_push_state(DOUBLESTRING);
}


<INITIAL,MATRIX,SHELLMODE>{quote}			{
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
      return scan_throw(QUOTE);
  }
  else
  {
      pstBuffer.clear();
      str_opener_column = yylloc.first_column;
      yy_push_state(SIMPLESTRING);
  }
}


<INITIAL,MATRIX>{spaces}		{
        scan_step();
}


<INITIAL>{newline}		{
  yylloc.last_line += 1;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL) {
      return scan_throw(EOL);
  }

}


<INITIAL,MATRIX>{blankline}		{
  yylloc.last_line += 1;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL)
  {
      return scan_throw(EOL);
  }
  scan_throw(EOL);
}

<INITIAL,MATRIX>{emptyline}		{
  yylloc.last_line += 2;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL)
  {
      return scan_throw(EOL);
  }
  scan_throw(EOL);
}
.					{
    std::string str = "Unexpected token \'";
    str += yytext;
    str += "\'";
    BEGIN(INITIAL);
    yyerror(str);
    return scan_throw(FLEX_ERROR);
}


<MATRIX>
{
  {lparen} {
    ++paren_levels.top();
    return scan_throw(LPAREN);
  }

  {rparen} {
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

  {spaces}*{colon}{spaces}* {
      return scan_throw(COLON);
  }

  {newline} {
      yylloc.last_line += 1;
      yylloc.last_column = 1;
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

  {rbrack}				{
    DEBUG("yy_pop_state()");
    yy_pop_state();
    paren_levels.pop();
    ParserSingleInstance::popControlStatus();
    return scan_throw(RBRACK);
  }

  {rbrace}				{
    yy_pop_state();
    paren_levels.pop();
    ParserSingleInstance::popControlStatus();
    return scan_throw(RBRACE);
  }

  {plus}				|
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

  {minus}				|
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
    // but [a +b] == [a, b] and plus here is unary
    // the space is non coding:
    // * after any other binary operator __op__
    //   Example : [a __op__ +b]
    // * after brackets or parentheses delimiters
    //   Example : [(1*2*a) +3]
    // * at the beginning of a line
    //   Example : [3 ...
    //              _+2]

    // no need to unput the '+'
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
   else
   {
       unput('+');
       yylloc.last_column--;
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

  .					{
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
   * after the ... // comments or ... \/* comments *\/
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
      paren_levels.pop();
  }
}

<LINEBREAK>
{
  {newline}				{
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

  {startblockcomment}			{
    yylval.comment = new std::wstring();
    comment_level = 1;
    ParserSingleInstance::pushControlStatus(Parser::WithinBlockComment);
    yy_push_state(REGIONCOMMENT);
  }

  {startcomment}			{
    scan_throw(DOTS);
    pstBuffer.clear();
    yy_push_state(LINECOMMENT);
  }

  {spaces}				{
      /* Do nothing... */
  }

  <<EOF>>	{
      yy_pop_state();
  }
  .					{
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
  {newline}	{
    //yylloc.last_line += 1;
    //yylloc.last_column = 1;
    //scan_step();
    yy_pop_state();
    for (int i = yyleng - 1 ; i >= 0 ; --i)
    {
        //std::cerr << "Unputting i = {" << i << "}" << std::endl;
        //std::cerr << "Unputting {" << yytext[i] << "}" << std::endl;
        unput(yytext[i]);
        yylloc.last_column--;
    }
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
	    pstBuffer.clear();
	    std::string str = "Can\'t convert \'";
	    str += pstBuffer.c_str();
	    str += "\' to UTF-8";
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

  <<EOF>>	{
    yy_pop_state();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
    if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
    {
	pstBuffer.clear();
	std::string str = "Can\'t convert \'";
	str += pstBuffer.c_str();
	str += "\' to UTF-8";
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
  {endblockcomment}				{
    --comment_level;
    if (comment_level == 0) {
      ParserSingleInstance::popControlStatus();
      yy_pop_state();
      //return scan_throw(BLOCKCOMMENT);
    }
  }

  {startblockcomment}				{
    ++comment_level;
    yy_push_state(REGIONCOMMENT);
  }

  {newline}					{
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    *yylval.comment += L"\n//";
  }

  {char_in_comment}				|
  .                                             {
      wchar_t *pwText = to_wide_string(yytext);
      *yylval.comment += std::wstring(pwText);
      FREE(pwText);
  }

 <<EOF>>					{
      yy_pop_state();
//    std::string str = "unexpected end of file in a comment";
//    scan_error(str);
  }
}


<SIMPLESTRING>
{
  {dquote}{dquote}				{
    pstBuffer += "\"";
  }

  {dquote}{quote}				{
    pstBuffer += "'";
  }

  {quote}{dquote}				{
    pstBuffer += "\"";
  }

  {quote}{quote}				{
    pstBuffer += "'";
  }

  {quote}					{
    yy_pop_state();
    //scan_step();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
    if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
    {
	pstBuffer.clear();
	std::string str = "Can\'t convert \'";
        str += pstBuffer.c_str();
        str += "\' to UTF-8";
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

  {newline}					{
    pstBuffer.clear();
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    BEGIN(INITIAL);
    ParserSingleInstance::popControlStatus();
    yyerror("Unexpected end of line in a string.");
    return scan_throw(FLEX_ERROR);
  }

  <<EOF>>					{
    pstBuffer.clear();
    BEGIN(INITIAL);
    yyerror("Unexpected end of file in a string.");
    return scan_throw(FLEX_ERROR);
  }

  {in_string}						|
  .                                                     {
    //scan_step();
    pstBuffer += yytext;
  }
}


<DOUBLESTRING>
{
  {dquote}{dquote}				{
    pstBuffer += "\"";
  }

  {dquote}{quote}				{
    pstBuffer += "'";
  }

  {quote}{dquote}               {
    pstBuffer += "\"";
  }

  {quote}{quote}				{
    pstBuffer += "'";
  }

  {dquote}                      {
    yy_pop_state();
    //scan_step();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer.c_str());
    if (pstBuffer.c_str() != NULL && pwstBuffer == NULL)
    {
	pstBuffer.clear();
        std::string str = "Can\'t convert \'";
        str += pstBuffer.c_str();
        str += "\' to UTF-8";
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
    }

}

%%

int scan_throw(int token) {
  last_token = token;
#ifdef DEV
  std::cout << "--> [DEBUG] TOKEN : " << token << " - " << token_to_string(token) << std::endl;
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
        case AND :                   str = "AND";
                                     break;
        case ASSIGN :                str = "ASSIGN";
                                     break;
        case BOOLFALSE :             str = "BOOLFALSE";
                                     break;
        case BOOLTRUE :              str = "BOOLTRUE";
                                     break;
        case BREAK :                 str = "BREAK";
                                     break;
        case CASE :                  str = "CASE";
                                     break;
        case CATCH :                 str = "CATCH";
                                     break;
        case COLON :                 str = "COLON";
                                     break;
        case COMMA :                 str = "COMMA";
                                     break;
        case COMMENT :               str = "COMMENT";
                                     break;
        case CONTINUE :              str = "CONTINUE";
                                     break;
        case CONTROLLDIVIDE :        str = "CONTROLLDIVIDE";
                                     break;
        case CONTROLRDIVIDE :        str = "CONTROLRDIVIDE";
                                     break;
        case CONTROLTIMES :          str = "CONTROLTIMES";
                                     break;
        case DO :                    str = "DO";
                                     break;
        case DOLLAR :                str = "DOLLAR";
                                     break;
        case DOT :                   str = "DOT";
                                     break;
        case DOTLDIVIDE :            str = "DOTLDIVIDE";
                                     break;
        case DOTPOWER :              str = "DOTPOWER";
                                     break;
        case DOTQUOTE :              str = "DOTQUOTE";
                                     break;
        case DOTRDIVIDE :            str = "DOTRDIVIDE";
                                     break;
        case DOTS :                  str = "DOTS";
                                     break;
        case DOTTIMES :              str = "DOTTIMES";
                                     break;
        case ELSE :                  str = "ELSE";
                                     break;
        case ELSEIF :                str = "ELSEIF";
                                     break;
        case END :                   str = "END";
                                     break;
        case ENDFUNCTION :           str = "ENDFUNCTION";
                                     break;
        case EOL :                   str = "EOL";
                                     break;
        case EQ :                    str = "EQ";
                                     break;
        case FLEX_ERROR :            str = "FLEX_ERROR";
                                     break;
        case FOR :                   str = "FOR";
                                     break;
        case FUNCTION :              str = "FUNCTION";
                                     break;
        case GE :                    str = "GE";
                                     break;
        case GT :                    str = "GT";
                                     break;
        case ID :                    str = "ID";
                                     break;
        case IF :                    str = "IF";
                                     break;
        case KRONLDIVIDE :           str = "KRONLDIVIDE";
                                     break;
        case KRONRDIVIDE :           str = "KRONRDIVIDE";
                                     break;
        case KRONTIMES :             str = "KRONTIMES";
                                     break;
        case LBRACE :                str = "LBRACE";
                                     break;
        case LBRACK :                str = "LBRACK";
                                     break;
        case LDIVIDE :               str = "LDIVIDE";
                                     break;
        case LE :                    str = "LE";
                                     break;
        case LPAREN :                str = "LPAREN";
                                     break;
        case LT :                    str = "LT";
                                     break;
        case MINUS :                 str = "MINUS";
                                     break;
        case NE :                    str = "NE";
                                     break;
        case NOT :                   str = "NOT";
                                     break;
        case NUM :                   str = "NUM";
                                     break;
        case OR :                    str = "OR";
                                     break;
        case OROR :                  str = "OROR";
                                     break;
        case OTHERWISE :             str = "OTHERWISE";
                                     break;
        case PLUS :                  str = "PLUS";
                                     break;
        case POWER :                 str = "POWER";
                                     break;
        case QUOTE :                 str = "QUOTE";
                                     break;
        case RBRACE :                str = "RBRACE";
                                     break;
        case RBRACK :                str = "RBRACK";
                                     break;
        case RDIVIDE :               str = "RDIVIDE";
                                     break;
        case RETURN :                str = "RETURN";
                                     break;
        case RPAREN :                str = "RPAREN";
                                     break;
        case SELECT :                str = "SELECT";
                                     break;
        case SEMI :                  str = "SEMI";
                                     break;
        case SPACES :                str = "SPACES";
                                     break;
        case STR :                   str = "STR";
                                     break;
        case SWITCH :                str = "SWITCH";
                                     break;
        case THEN :                  str = "THEN";
                                     break;
        case TIMES :                 str = "TIMES";
                                     break;
        case TRY :                   str = "TRY";
                                     break;
        case VARFLOAT :              str = "VARFLOAT";
                                     break;
        case VARINT :                str = "VARINT";
                                     break;
        case WHILE :                 str = "WHILE";
                                     break;
        default :                    str = "UNKNOWN";
                                     break;
    }
    return str;

}
#endif
