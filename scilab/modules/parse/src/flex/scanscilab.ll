%{                                                            /* -*- C++ -*- */

#include "isatty.hxx"
#include "parse.hxx"
#include "parser_private.hxx"

#include "context.hxx"

extern "C"
{
#include "charEncoding.h"
#include "MALLOC.h"
}

static int comment_level = 0;
static int last_token = 0;
static int exit_status = PARSE_ERROR;
static std::string current_file;
static std::string program_name;

static std::string *pstBuffer;

 static bool rejected = false;

#define YY_USER_ACTION                          \
 yylloc.last_column += yyleng;
/* -*- Verbose Special Debug -*- */
//#define DEV
//#define TOKENDEV
%}

%option stack
%option noyywrap

%x SIMPLESTRING
%x DOUBLESTRING
%x REGIONCOMMENT
%x LINECOMMENT
%x LINEBREAK

%x MATRIX
%x MATRIXMINUSID

%x SHELLMODE
%x BEGINID

spaces			[ \t\v\f]+
integer			[0-9]+
number			[0-9]+[\.][0-9]*
little			\.[0-9]+

floating		({little}|{number}|{integer})[deDE][+-]?{integer}

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
id              (([a-zA-Z_%#?]|{utf})([a-zA-Z_0-9#?]|{utf})*)


newline			("\n"|"\r\n"|"\r")
blankline		^[ \t\v\f]+{newline}
next			(".."|"...")

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

dot			"."
dotquote		".'"
dottimes		".*"
dotdivide		"./"
dotrdivide		".\\"
dotpower		(".^"|".**")

plus			"+"
minus			"-"
divide			"/"
rdivide			"\\"
times			"*"
power			("^"|"**")

equal			"=="
notequal		("~="|"@="|"<>")
lowerthan		"<"
greaterthan		">"
lowerequal		"<="
greaterequal		">="

krontimes		".*."
krondivide		"./."
kronrdivide		".\\."

controltimes        ("*."[^0-9])
controldivide		("/."[^0-9])
controlrdivide      ("\\."[^0-9])

assign			"="

%%

<INITIAL,BEGINID>"if"            {
	ParserSingleInstance::pushControlStatus(Parser::WithinIf);
	BEGIN(INITIAL);
    return scan_throw(IF);
}

<INITIAL,BEGINID>"then"          {
    	BEGIN(INITIAL);
        return scan_throw(THEN);
}

<INITIAL,BEGINID>"else"          {
    // Pop to step out IF
	ParserSingleInstance::popControlStatus();
	ParserSingleInstance::pushControlStatus(Parser::WithinElse);
	BEGIN(INITIAL);
	return scan_throw(ELSE);
}

<INITIAL,BEGINID>"elseif" {
	ParserSingleInstance::popControlStatus();
	ParserSingleInstance::pushControlStatus(Parser::WithinElseIf);
	BEGIN(INITIAL);
	return scan_throw(ELSEIF);
}

<INITIAL,BEGINID>"end"		{
  ParserSingleInstance::popControlStatus();
	BEGIN(INITIAL);
  return scan_throw(END);
}

<INITIAL,BEGINID>"select"	{
  ParserSingleInstance::pushControlStatus(Parser::WithinSelect);
	BEGIN(INITIAL);
  return scan_throw(SELECT);
}

<INITIAL,BEGINID>"switch"	{
  ParserSingleInstance::pushControlStatus(Parser::WithinSwitch);
	BEGIN(INITIAL);
  return scan_throw(SWITCH);
}

<INITIAL,BEGINID>"otherwise" {
	ParserSingleInstance::popControlStatus();
	ParserSingleInstance::pushControlStatus(Parser::WithinOtherwise);
	BEGIN(INITIAL);
	return scan_throw(OTHERWISE);
}

<INITIAL,BEGINID>"case"		{
  ParserSingleInstance::popControlStatus();
  ParserSingleInstance::pushControlStatus(Parser::WithinCase);
	BEGIN(INITIAL);
  return scan_throw(CASE);
}

<INITIAL,BEGINID>"function" {
	ParserSingleInstance::pushControlStatus(Parser::WithinFunction);
	BEGIN(INITIAL);
	return scan_throw(FUNCTION);
}

<INITIAL,BEGINID>"endfunction" {
	ParserSingleInstance::popControlStatus();
	BEGIN(INITIAL);
	return scan_throw(ENDFUNCTION);
}

<INITIAL,BEGINID>"#function"	{
	ParserSingleInstance::pushControlStatus(Parser::WithinFunction);
	BEGIN(INITIAL);
	return scan_throw(HIDDENFUNCTION);
}

<INITIAL,BEGINID>"hidden"	{
 	BEGIN(INITIAL);
   return scan_throw(HIDDEN);
}

<INITIAL,BEGINID>"for" {
  ParserSingleInstance::pushControlStatus(Parser::WithinFor);
	BEGIN(INITIAL);
  return scan_throw(FOR);
}

<INITIAL,BEGINID>"while"	{
	ParserSingleInstance::pushControlStatus(Parser::WithinWhile);
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

^{spaces}*/({id}){spaces}[^(=] {
        BEGIN(BEGINID);
}

<BEGINID>
{
    {id}                        {
        yylval.str = new std::wstring(to_wide_string(yytext));
        if (symbol::Context::getInstance()->get(*yylval.str) != NULL
            && symbol::Context::getInstance()->get(*yylval.str)->isCallable())
        {
            scan_throw(ID);
            BEGIN(SHELLMODE);
        }
        else
        {
            BEGIN(INITIAL);
            return scan_throw(ID);
        }
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


<INITIAL,MATRIX>{lparen}		{
  return scan_throw(LPAREN);
}
<INITIAL,MATRIX>{rparen}		{
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
<INITIAL,MATRIX>{dotdivide}		{
  return scan_throw(DOTDIVIDE);
}
<INITIAL,MATRIX>{dotrdivide}		{
  return scan_throw(DOTRDIVIDE);
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
<INITIAL,MATRIX>{divide}		{
  return scan_throw(DIVIDE);
}
<INITIAL,MATRIX>{rdivide}		{
  return scan_throw(RDIVIDE);
}
<INITIAL,MATRIX>{power}			{
  return scan_throw(POWER);
}

<INITIAL,MATRIX>{krontimes}		{
  return scan_throw(KRONTIMES);
}
<INITIAL,MATRIX>{krondivide}		{
  return scan_throw(KRONDIVIDE);
}
<INITIAL,MATRIX>{kronrdivide}		{
  return scan_throw(KRONRDIVIDE);
}


<INITIAL,MATRIX>{controltimes}		{
    unput(yytext[yyleng - 1]);
    return scan_throw(CONTROLTIMES);
}
<INITIAL,MATRIX>{controldivide}		{
    unput(yytext[yyleng - 1]);
    return scan_throw(CONTROLDIVIDE);
}
<INITIAL,MATRIX>{controlrdivide}		{
    unput(yytext[yyleng - 1]);
    return scan_throw(CONTROLRDIVIDE);
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
  yy_push_state(MATRIX);
  ParserSingleInstance::pushControlStatus(Parser::WithinMatrix);
  return scan_throw(LBRACK);
}

{rbrack}				{
  return scan_throw(RBRACK);
}


<INITIAL,MATRIX>{dot}			{
  return scan_throw(DOT);
}

<INITIAL,MATRIX>{next}			{
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


<INITIAL,MATRIX>{floating}		{
  scan_exponent_convert(yytext);
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
  scan_step();
  return scan_throw(VARFLOAT);
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
  scan_step();
  return scan_throw(NUM);
}


<INITIAL,MATRIX>{id}			{
    yylval.str = new std::wstring(to_wide_string(yytext));
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] ID : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(ID);
}


<INITIAL,MATRIX>{startblockcomment}	{
  yylval.comment = new std::wstring();
  comment_level = 1;
  ParserSingleInstance::pushControlStatus(Parser::WithinBlockComment);
  yy_push_state(REGIONCOMMENT);
}


<INITIAL,MATRIX>{startcomment}		{
  pstBuffer = new std::string();
  yy_push_state(LINECOMMENT);
}


<INITIAL,MATRIX,SHELLMODE>{dquote}		{
  pstBuffer = new std::string();
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
      || last_token == RBRACE)
    {
      return scan_throw(QUOTE);
    }
  else {
    pstBuffer = new std::string();
    yy_push_state(SIMPLESTRING);
  }
}


<INITIAL,MATRIX>{spaces}		{
  scan_step();
  scan_throw(SPACES);
}


<INITIAL,MATRIX>{newline}		{
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
  scan_throw(EOL);
}


.					{
    std::string str = "unexpected token '";
    str += yytext;
    str += "'";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yyterminate();
}


<MATRIX>
{
  {rbrack}				{
    yy_pop_state();
    ParserSingleInstance::popControlStatus();
    return scan_throw(RBRACK);
  }

  {rbrace}				{
    yy_pop_state();
    ParserSingleInstance::popControlStatus();
    return scan_throw(RBRACE);
  }

  {plus}				{
    return scan_throw(PLUS);
  }

  {minus}				{
    return scan_throw(MINUS);
  }

  {spaces}({plus}|{minus}){integer}			{
   int i;
    for (i = yyleng - 1 ; i >= 0 ; --i)
      {
	unput(yytext[i]);
      }
    yy_push_state(MATRIXMINUSID);
    if (last_token != LBRACK
	&& last_token != EOL
	&& last_token != SEMI)
      {
	return scan_throw(COMMA);
      }
  }

  {spaces}({plus}|{minus}){number}	{
   int i;
    for (i = yyleng - 1 ; i >= 0 ; --i)
      {
	unput(yytext[i]);
      }
    yy_push_state(MATRIXMINUSID);
    if (last_token != LBRACK
	&& last_token != EOL
	&& last_token != SEMI)
      {
	return scan_throw(COMMA);
      }
  }

  {spaces}({plus}|{minus}){floating}	{
   int i;
    for (i = yyleng - 1 ; i >= 0 ; --i)
      {
	unput(yytext[i]);
      }
    yy_push_state(MATRIXMINUSID);
    if (last_token != LBRACK
	&& last_token != EOL
	&& last_token != SEMI)
      {
	return scan_throw(COMMA);
      }
  }

  {spaces}({plus}|{minus}){little}	{
   int i;
    for (i = yyleng - 1 ; i >= 0 ; --i)
      {
	unput(yytext[i]);
      }
    yy_push_state(MATRIXMINUSID);
    if (last_token != LBRACK
	&& last_token != EOL
	&& last_token != SEMI)
      {
	return scan_throw(COMMA);
      }
  }

  {spaces}({minus}|{plus}){id}		{
    int i;
    for (i = yyleng - 1 ; i >= 0 ; --i)
      {
	unput(yytext[i]);
      }
    yy_push_state(MATRIXMINUSID);
    if (last_token != LBRACK
	&& last_token != EOL
	&& last_token != SEMI)
      {
	return scan_throw(COMMA);
      }
  }
  .					{
    std::string str = "unexpected token '";
    str += yytext;
    str += "' within a matrix.";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yyterminate();
  }
}

<MATRIXMINUSID>
{
  {minus}				{
    return scan_throw(MINUS);
  }

  {plus}				{
     /* Do Nothing. */
  }

  {integer}				{
    yy_pop_state();
    yylval.number = atof(yytext);
#ifdef TOKENDEV
    std::cout << "--> [DEBUG] INTEGER : " << yytext << std::endl;
#endif
    scan_step();
    return scan_throw(VARINT);
  }

  {number}				{
    yy_pop_state();
    yylval.number = atof(yytext);
#ifdef TOKENDEV
    std::cout << "--> [DEBUG] NUMBER : " << yytext << std::endl;
#endif
    scan_step();
    return scan_throw(NUM);
  }

  {little}				{
    yy_pop_state();
    yylval.number = atof(yytext);
#ifdef TOKENDEV
    std::cout << "--> [DEBUG] LITTLE : " << yytext << std::endl;
#endif
    scan_step();
    return scan_throw(NUM);
  }

  {floating}				{
    yy_pop_state();
    scan_exponent_convert(yytext);
    yylval.number = atof(yytext);
#ifdef TOKENDEV
    std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
    scan_step();
    return scan_throw(VARFLOAT);
  }

  {id}					{
    yy_pop_state();
    yylval.str = new std::wstring(to_wide_string(yytext));
#ifdef TOKENDEV
    std::cout << "--> [DEBUG] ID : " << yytext << std::endl;
#endif
    scan_step();
    return scan_throw(ID);
  }

  {spaces}				{
    /* Do Nothing. */
  }

  .					{
    std::string str = "unexpected token '";
    str += yytext;
    str += "' within a matrix.";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yyterminate();
  }
}

<LINEBREAK>
{
  {newline}				{
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    yy_pop_state();
  }

  {startblockcomment}			{
    ++comment_level;
    yy_push_state(REGIONCOMMENT);
  }

  {startcomment}			{
    scan_throw(DOTS);
    pstBuffer = new std::string();
    yy_push_state(LINECOMMENT);
  }

  {spaces}				{
    /* Do nothing... */
  }

  .					{
    std::string str = "unexpected token '";
    str += yytext;
    str += "' after line break with .. or ...";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yyterminate();
  }
}


<LINECOMMENT>
{
  {newline}	{
    //yylloc.last_line += 1;
    //yylloc.last_column = 1;
    //scan_step();
    yy_pop_state();
    unput('\n');
    /*
    ** To forgot comments after lines break
    */
    if (last_token != DOTS)
    {
        wchar_t *pwstBuffer = to_wide_string(pstBuffer->c_str());
        yylval.comment = new std::wstring(pwstBuffer);
        delete pstBuffer;
        FREE (pwstBuffer);
        return scan_throw(COMMENT);
    }
  }

  <<EOF>>	{
    yy_pop_state();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer->c_str());
    yylval.comment = new std::wstring(pwstBuffer);
    delete pstBuffer;
    FREE (pwstBuffer);
    return scan_throw(COMMENT);
  }

  .         {
     // Put the char in a temporary CHAR buffer to go through UTF-8 trouble
     // only translate to WCHAR_T when popping state.
     *pstBuffer += yytext;
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

  .						{
      *yylval.comment += std::wstring(to_wide_string(yytext));
  }

 <<EOF>>					{
    std::string str = "unexpected end of file in a comment";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yy_pop_state();
    yyterminate();
  }
}


<SIMPLESTRING>
{
  {dquote}{dquote}				{
    *pstBuffer += "\"";
  }

  {dquote}{quote}				{
    *pstBuffer += "'";
  }

  {quote}{dquote}				{
    *pstBuffer += "\"";
  }

  {quote}{quote}				{
    *pstBuffer += "'";
  }

  {quote}					{
    yy_pop_state();
    scan_step();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer->c_str());
    yylval.str = new std::wstring(pwstBuffer);
    delete pstBuffer;
    FREE(pwstBuffer);
    return scan_throw(STR);
  }

  {next}					{
    //yylloc.last_line += 1;
    scan_step();
  }

  {newline}					{
    std::string str = "unexpected end of line in a string.";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    yy_pop_state();
    yyterminate();
  }

  <<EOF>>					{
    std::string str = "unexpected end of file in a string.";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yy_pop_state();
    yyterminate();
  }

  .						{
    scan_step();
    *pstBuffer += yytext;
  }
}


<DOUBLESTRING>
{
  {dquote}{dquote}				{
    *pstBuffer += "\"";
  }

  {dquote}{quote}				{
    *pstBuffer += "'";
  }

  {quote}{dquote}               {
    *pstBuffer += "\"";
  }

  {quote}{quote}				{
    *pstBuffer += "'";
  }

  {dquote}                      {
    yy_pop_state();
    scan_step();
    wchar_t *pwstBuffer = to_wide_string(pstBuffer->c_str());
    yylval.str = new std::wstring(pwstBuffer);
    delete pstBuffer;
    FREE(pwstBuffer);
    return scan_throw(STR);
  }

  {newline} {
    std::string str = "unexpected end of line in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    yyterminate();
  }

  {next} {
    //yylloc.last_line += 1;
    scan_step();
  }

  <<EOF>>   {
    std::string str = "unexpected end of file in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yy_pop_state();
    yyterminate();
  }

  .         {
    scan_step();
    *pstBuffer += yytext;
  }
}


<SHELLMODE>
{
    {spaces}                    {
        if (last_token == ID)
        {
            scan_throw(SPACES);
            return ID;
        }
    }

    {semicolon}                 {
        BEGIN(INITIAL);
        return scan_throw(SEMI);
    }

    {comma}                     {
        BEGIN(INITIAL);
        return scan_throw(COMMA);
    }

    {newline}                   {
        BEGIN(INITIAL);
        return scan_throw(EOL);
    }

    [^ \t\v\f\r\n,;'"]+               {
        yylval.str = new std::wstring(to_wide_string(yytext));
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
  std::cout << "--> [DEBUG] TOKEN : " << token << std::endl;
#endif
  return token;
}

void scan_step() {
  yylloc.first_line = yylloc.last_line;
  yylloc.first_column = yylloc.last_column;
}

void scan_error(std::string msg)
{
  wchar_t* pstMsg = to_wide_string(msg.c_str());
  std::wcerr << pstMsg << std::endl;
  ParserSingleInstance::PrintError(pstMsg);
  ParserSingleInstance::setExitStatus(Parser::Failed);
  FREE(pstMsg);
}

/*
** convert floating numbers to C standard
** 1.2d-3 -> 1.2e-3
** 1.2D-3 -> 1.2e-3
*/
void scan_exponent_convert(char *in)
{
  char *pString;
  while((pString=strpbrk(in,"d"))!=NULL)
    {
      *pString='e';
    }
  while((pString=strpbrk(in,"D"))!=NULL)
    {
      *pString='e';
    }
}

#ifdef _MSC_VER
int isatty (int desc)
{
  return 0;
}
#endif
