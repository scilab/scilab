
%{
#ifndef FLEX_SCANNER
#undef input
#undef unput
#endif

#include    <X11/Xlib.h>
#include    <X11/Xresource.h>
#include    <X11/IntrinsicP.h>
#include    <X11/StringDefs.h>

#include    "LayoutP.h"
#include    "laygram.h"
static char *yysourcebase, *yysource;

#ifndef FLEX_SCANNER
#define input() (*yysource++)
#define unput(c)    (--yysource)
#else
#include <string.h>
static void my_yyinput(char *buf, int *result, int max_size);
#define YY_INPUT(buf, res, max) my_yyinput(buf, &(res), max)
#endif

#ifdef __STDC__
static int count ();
#endif
%}
%%
vertical		return VERTICAL;
horizontal		return HORIZONTAL;
"{"			return OC;
"}"			return CC;
"("			return OP;
")"			return CP;
"<"			return OA;
">"			return CA;
infinity		{ yylval.ival = 1; return INFINITY; }
inff*			{ yylval.ival = count(yytext, 'f'); return INFINITY; }
[0-9][0-9]*		{ yylval.ival = atoi(yytext); return NUMBER; }
"="			{ return EQUAL; }
"$"			{ return DOLLAR; }
"+"			{ yylval.oval = Plus; return PLUS; }
"-"			{ yylval.oval = Minus; return MINUS; }
"*"			{ yylval.oval = Times; return TIMES; }
"/"			{ yylval.oval = Divide; return DIVIDE; }
"%"			{ yylval.oval = Percent; return PERCENT; }
%[ \t\n]*of		{ yylval.oval = Percent; return PERCENTOF; }
width			return WIDTH;
height			return HEIGHT;
\\[a-zA-Z_][a-zA-Z0-9_]* { 
#ifdef FLEX_SCANNER
			    yytext[yyleng] = '\0';
#else
			    yytext[yyleng-1] = '\0';
#endif
			    yylval.qval = XrmStringToQuark (yytext+1);
 			    return NAME;
			}

[a-zA-Z_][a-zA-Z0-9_]*	{ 
#ifdef FLEX_SCANNER
			    yytext[yyleng] = '\0';
#else
			    yytext[yyleng-1] = '\0';
#endif
			    yylval.qval = XrmStringToQuark (yytext);
 			    return NAME;
			}
" "			;
"\t"			;
"\n"			;
.			fprintf (stderr, "ignoring %c\n", *yytext);
%%

static int
count (s, c)
    char    *s;
    char    c;
{
    int	i = 0;
    while (*s)
	if (*s++ == c)
	    i++;
    return i;
}

yysetsource(s)
    char    *s;
{
    yysourcebase = yysource = s;
}

yyerror(s)
    char    *s;
{
    char    *t;
    
    fprintf (stderr, "%s\n", s);
    t = yysource - 50;
    if (t < yysourcebase)
	t = yysourcebase;
    while (*t && t < yysource + 50) {
	if (t == yysource)
	    putc ('@', stderr);
	putc (*t++, stderr);
    }
    if (t == yysource)
	putc ('@', stderr);
    if (!*t)
	fprintf (stderr, "<EOF>");
    fprintf (stderr, "\n");
}

#ifdef FLEX_SCANNER
static void
my_yyinput(buf, result, max_size)
    char *buf; int *result; int max_size;
{
    int size = max_size < strlen(yysource) ? max_size : strlen(yysource);

    strncpy(buf, yysource, size);
    yysource += size;
    *result = size;
}
#endif
