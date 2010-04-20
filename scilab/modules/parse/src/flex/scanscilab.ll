%{                                                            /* -*- C++ -*- */

#include "isatty.hxx"
#include "parse.hxx"
#include "parser.hxx"

static int comment_level = 0;
static int last_token = 0;
static int exit_status = PARSE_ERROR;
static std::string current_file;
static std::string program_name;

 static bool rejected = false;

#define YY_USER_ACTION           \
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

spaces			[ \t\v\f]+
integer			[0-9]+
number			[0-9]+[\.][0-9]*
little			\.[0-9]+

floating		({little}|{number}|{integer})[deDE][+-]?{integer}

hex			[0]x[0-9a-fA-F]+
oct			[0]o[0-7]+

id			[a-zA-Z_%#?][a-zA-Z_0-9#?]*
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
notequal		("~="|"<>")
lowerthan		"<"
greaterthan		">"
lowerequal		"<="
greaterequal		">="

krontimes		".*."
krondivide		"./."
kronrdivide		".\\."

controldivide		"/."

assign			"="

%%

"if"		{
	Parser::getInstance()->pushControlStatus(Parser::WithinIf);
	return scan_throw(IF);
}
"then"		return scan_throw(THEN);
"else" {
	Parser::getInstance()->popControlStatus();
	Parser::getInstance()->pushControlStatus(Parser::WithinElse);
	return scan_throw(ELSE);
}
"elseif" {
	Parser::getInstance()->popControlStatus();
	Parser::getInstance()->pushControlStatus(Parser::WithinElseIf);
	return scan_throw(ELSEIF);
}
"end"		{
  Parser::getInstance()->popControlStatus();
  return scan_throw(END);
}

"select"	{
  Parser::getInstance()->pushControlStatus(Parser::WithinSelect);
  return scan_throw(SELECT);
}

"switch"	{
  Parser::getInstance()->pushControlStatus(Parser::WithinSwitch);
  return scan_throw(SWITCH);
}

"otherwise" {
	Parser::getInstance()->popControlStatus();
	Parser::getInstance()->pushControlStatus(Parser::WithinOtherwise);
	return scan_throw(OTHERWISE);
}

"case"		{
  Parser::getInstance()->popControlStatus();
  Parser::getInstance()->pushControlStatus(Parser::WithinCase);
  return scan_throw(CASE);
}

"function" {
	Parser::getInstance()->pushControlStatus(Parser::WithinFunction);
	return scan_throw(FUNCTION);
}
"endfunction" {
	Parser::getInstance()->popControlStatus();
	return scan_throw(ENDFUNCTION);
}
"#function"	{
	Parser::getInstance()->pushControlStatus(Parser::WithinFunction);
	return scan_throw(HIDDENFUNCTION);
}
"hidden"	return scan_throw(HIDDEN);

"for" {
  Parser::getInstance()->pushControlStatus(Parser::WithinFor);
  return scan_throw(FOR);
}

"while"	{
	Parser::getInstance()->pushControlStatus(Parser::WithinWhile);
	return scan_throw(WHILE);
}

"do"		return scan_throw(DO);
"break"		return scan_throw(BREAK);


"try" {
	Parser::getInstance()->pushControlStatus(Parser::WithinTry);
	return scan_throw(TRY);
}

"catch" {
	Parser::getInstance()->pushControlStatus(Parser::WithinCatch);
	return scan_throw(CATCH);
}
"return"	return scan_throw(RETURN);


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
  return scan_throw(LBRACE);
}
<INITIAL,MATRIX>{rbrace}		{
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


<INITIAL,MATRIX>{controldivide}		{
  return scan_throw(CONTROLDIVIDE);
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
  Parser::getInstance()->pushControlStatus(Parser::WithinMatrix);
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
  yylval.str = new std::string(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] ID : " << yytext << std::endl;
#endif
//  scan_step();
  return scan_throw(ID);
}


<INITIAL,MATRIX>{startblockcomment}	{
  yylval.comment = new std::string();
  ++comment_level;
  yy_push_state(REGIONCOMMENT);
}


<INITIAL,MATRIX>{startcomment}		{
  yylval.comment = new std::string();
  yy_push_state(LINECOMMENT);
}


<INITIAL,MATRIX>{dquote}		{
  yylval.str = new std::string();
  yy_push_state(DOUBLESTRING);
}


<INITIAL,MATRIX>{quote}			{
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
    yylval.str = new std::string();
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
    Parser::getInstance()->popControlStatus();
    return scan_throw(RBRACK);
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
    yylval.str = new std::string(yytext);
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
    yylval.comment = new std::string();
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
	return scan_throw(COMMENT);
      }

  }

  <<EOF>>	{
    yy_pop_state();
    return scan_throw(COMMENT);
  }

  .		{
    *yylval.comment += yytext;
  }
}


<REGIONCOMMENT>
{
  {endblockcomment}				{
    --comment_level;
    if (comment_level == 0) {
      yy_pop_state();
      //return scan_throw(BLOCKCOMMENT);
    }
  }

  {startblockcomment}				{
    ++comment_level;
    yy_push_state(REGIONCOMMENT);
  }

  <<EOF>>					{
    std::string str = "unexpected end of file in a comment";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yyterminate();
  }

  {newline}					{
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    *yylval.comment += "\n//";
  }

  .						{
    *yylval.comment += yytext;
  }
}


<SIMPLESTRING>
{
  {dquote}{dquote}				{
    *yylval.str += "\"";
  }

  {dquote}{quote}				{
    *yylval.str += "'";
  }

  {quote}{dquote}				{
    *yylval.str += "\"";
  }

  {quote}{quote}				{
    *yylval.str += "'";
  }

  {quote}					{
    yy_pop_state();
    scan_step();
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
    *yylval.str += yytext;
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
    *yylval.str += yytext;
  }
}


<DOUBLESTRING>
{
  {dquote}{dquote}				{
    *yylval.str += "\"";
  }

  {dquote}{quote}				{
    *yylval.str += "'";
  }

  {quote}{dquote}				{
    *yylval.str += "\"";
  }

  {quote}{quote}				{
    *yylval.str += "'";
  }

  {dquote}					{
    yy_pop_state();
    scan_step();
    return scan_throw(STR);
  }

  {newline} {
    std::string str = "unexpected end of line in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    *yylval.str += yytext;
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
    *yylval.str += yytext;
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
  Parser::PrintError(msg);
  Parser::getInstance()->setExitStatus(Parser::Failed);
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
