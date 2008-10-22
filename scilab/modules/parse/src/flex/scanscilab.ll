%{                                                            /* -*- C++ -*- */

#include "isatty.hxx"
#include "parse.hxx"
#include "parser.hxx"

static int string_level = 0;
static int comment_level = 0;
static int last_token = 0;
static int exit_status = PARSE_ERROR;
static std::string current_file;
static std::string program_name;

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

spaces		[ \t\v\f]+
integer		[0-9]+
number		[0-9]+[\.][0-9]*
little		\.[0-9]+

floating	({little}|{number}|{integer})[deDE][+-]?{integer}

hex		[0]x[0-9a-fA-F]+
oct		[0]o[0-7]+

id		[a-zA-Z_%#?][a-zA-Z_0-9#?]*
newline		("\n"|"\r\n")
blankline	^[ \t\v\f]+{newline}
next		(".."|"...")

%%


"@"		return scan_throw(NOT);
"~"		return scan_throw(NOT);
"$"		return scan_throw(DOLLAR);

"%t"		return scan_throw(BOOLTRUE);
"%f"		return scan_throw(BOOLFALSE);
"%T"		return scan_throw(BOOLTRUE);
"%F"		return scan_throw(BOOLFALSE);


"("		return scan_throw(LPAREN);
")"		return scan_throw(RPAREN);
";"		return scan_throw(SEMI);
","		return scan_throw(COMMA);
":"		return scan_throw(COLON);
"["		return scan_throw(LBRACK);
"]"		return scan_throw(RBRACK);
"{"		return scan_throw(LBRACE);
"}"		return scan_throw(RBRACE);
"."		return scan_throw(DOT);
".'"		return scan_throw(DOTQUOTE);
"-"		return scan_throw(MINUS);
".-"		return scan_throw(DOTMINUS);
"+"		return scan_throw(PLUS);
".+"		return scan_throw(DOTPLUS);
"*"		return scan_throw(TIMES);
".*"		return scan_throw(DOTTIMES);
".*."		return scan_throw(KRONTIMES);
"/"		return scan_throw(DIVIDE);
"./"		return scan_throw(DOTDIVIDE);
"/."		return scan_throw(CONTROLDIVIDE);
"./."		return scan_throw(KRONDIVIDE);
"\\"		return scan_throw(RDIVIDE);
".\\"		return scan_throw(DOTRDIVIDE);
".\\."		return scan_throw(KRONRDIVIDE);
"**"		return scan_throw(POWER);
"^"		return scan_throw(POWER);
".^"		return scan_throw(DOTPOWER);

"=="		return scan_throw(EQ);
"<"		return scan_throw(LT);
">"		return scan_throw(GT);
"<>"		return scan_throw(NE);
"~="		return scan_throw(NE);
"<="		return scan_throw(LE);
">="		return scan_throw(GE);
"&"		return scan_throw(AND);
"|"		return scan_throw(OR);
"="		return scan_throw(ASSIGN);

"if"		return scan_throw(IF);
"then"		return scan_throw(THEN);
"else"		return scan_throw(ELSE);
"elseif"	return scan_throw(ELSEIF);
"end"		return scan_throw(END);

"select"	return scan_throw(SELECT);
"case"		return scan_throw(CASE);

"function"	return scan_throw(FUNCTION);
"endfunction"	return scan_throw(ENDFUNCTION);

"for"		return scan_throw(FOR);

"while"		return scan_throw(WHILE);
"do"		return scan_throw(DO);
"break"		return scan_throw(BREAK);

"try"		return scan_throw(TRY);
"catch"		return scan_throw(CATCH);
"return"	return scan_throw(RETURN);


{next}			{
  BEGIN LINEBREAK;
}

{integer}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] INTEGER : " << yytext << std::endl;
#endif
  scan_step();
  return scan_throw(VARINT);
}

{floating} {
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] FLOATING : " << yytext << std::endl;
#endif
  scan_step();
  return scan_throw(VARFLOAT);
}

{number}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] NUMBER : " << yytext << std::endl;
#endif
  scan_step();
  return scan_throw(NUM);
}

{little}		{
  yylval.number = atof(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] LITTLE : " << yytext << std::endl;
#endif
  scan_step();
  return scan_throw(NUM);
}

{id}			{
  yylval.str = new std::string(yytext);
#ifdef TOKENDEV
  std::cout << "--> [DEBUG] ID : " << yytext << std::endl;
#endif
  scan_step();
  return scan_throw(ID);
}

"/*"			{
  yylval.comment = new std::string();
  ++comment_level;
  BEGIN REGIONCOMMENT;
}

"//"			{
  yylval.comment = new std::string();
  yy_push_state(LINECOMMENT);
}

"\""          {
  yylval.str = new std::string();
  ++string_level;
  yy_push_state(DOUBLESTRING);
}

"'"	      {
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
    ++string_level;
    yy_push_state(SIMPLESTRING);
  }
}

{spaces}      {
  scan_step();
  scan_throw(SPACES);
}

{newline}     {
  yylloc.last_line += 1;
  yylloc.last_column = 1;
  scan_step();
  if (last_token != EOL) {
    return scan_throw(EOL);
  }
}

{blankline}   {
  yylloc.last_line += 1;
  yylloc.last_column = 1;
  scan_step();
  scan_throw(EOL);
}

.             {
  std::string str = " : unexpected token '";
  str += yytext;
  str += "'";
  exit_status = SCAN_ERROR;
  scan_error(str);
}


<LINEBREAK>
{
  {newline}	{
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    BEGIN INITIAL;
  }

  "/*"			{
    ++comment_level;
    BEGIN REGIONCOMMENT;
  }

  "//"			{
    scan_throw(DOTS);
    yylval.comment = new std::string();
    BEGIN LINECOMMENT;
  }

  {spaces}		{
    /* Do nothing... */
  }

  . {
    std::string str = " : unexpected token '";
    str += yytext;
    str += "' after line break with .. or ...";
    exit_status = SCAN_ERROR;
    scan_error(str);
  }

}


<LINECOMMENT>
{
  {newline}	{
    //yylloc.last_line += 1;
    //yylloc.last_column = 1;
    //scan_step();
    BEGIN INITIAL;
    /*
    ** To forgot comments after lines break
    */
    if (last_token != DOTS)
      {
	unput('\n');
	return scan_throw(COMMENT);
      }
  }

  <<EOF>>	{
    BEGIN INITIAL;
    return scan_throw(COMMENT);
  }

  .		{
    *yylval.comment += yytext;
  }
}



<REGIONCOMMENT>
{
  "*/"		{
    --comment_level;
    if (comment_level == 0) {
      BEGIN INITIAL;
      //return scan_throw(BLOCKCOMMENT);
    }
  }

  "/*"		{
    ++comment_level;
    BEGIN REGIONCOMMENT;
  }

  <<EOF>>	{
    std::string str = " : unexpected end of file in a comment";
    exit_status = SCAN_ERROR;
    scan_error(str);
  }

  {newline}	{
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    scan_step();
    *yylval.comment += "\n//";
  }

  .		{
    *yylval.comment += yytext;
  }
}


<SIMPLESTRING>
{
  "\"\"" {
    *yylval.str += "\"";
  }

  "\"'" {
    *yylval.str += "'";
  }

  "'\"" {
    *yylval.str += "\"";
  }

  "''" {
    *yylval.str += "'";
  }

  "\""      {
    ++string_level;
    yy_push_state(DOUBLESTRING);
  }

  "'"      {
    yy_pop_state();
    scan_step();
    --string_level;
    if (string_level == 0) {
      return scan_throw(STR);
    }
  }

  {next} {
    //yylloc.last_line += 1;
    scan_step();
  }

  {newline} {
    std::string str = " : unexpected end of line in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    *yylval.str += yytext;
  }

  <<EOF>>   {
    std::string str = " : unexpected end of file in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
  }

  .         {
    *yylval.str += yytext;
  }
}



<DOUBLESTRING>
{
  "\"\"" {
    *yylval.str += "\"";
  }

  "\"'" {
    *yylval.str += "'";
  }

  "'\"" {
    *yylval.str += "\"";
  }

  "''" {
    *yylval.str += "'";
  }

  "'"      {
    ++string_level;
    yy_push_state(SIMPLESTRING);
  }

  "\""      {
    yy_pop_state();
    scan_step();
    --string_level;
    if (string_level == 0) {
      return scan_throw(STR);
    }
  }

  {newline} {
    std::string str = " : unexpected end of line in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
    yylloc.last_line += 1;
    yylloc.last_column = 1;
    *yylval.str += yytext;
  }

  {next} {
    //yylloc.last_line += 1;
    scan_step();
  }

  <<EOF>>   {
    std::string str = " : unexpected end of file in a string";
    exit_status = SCAN_ERROR;
    scan_error(str);
  }

  .         {
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
  exit(exit_status);
}

void scan_open (const std::string &name)
{
  if (name == "-")
    {
      yyin = stdin;
      current_file = "standard input";
    }
  else
    {
      yyin = fopen (name.c_str (), "r");
      current_file = name;
      if (!yyin)
	{
	  std::cerr
	    << program_name
	    << ": cannot open `" << name << "': "
	    << strerror (errno) << std::endl;
	  exit (1);
	}
    }
}

void scan_close (void)
{
  fclose (yyin);
}

#ifdef _MSC_VER
int isatty (int desc)
{
  return 0;
}
#endif
