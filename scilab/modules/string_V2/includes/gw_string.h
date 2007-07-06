/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_STRING__
#define __GW_STRING__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*String_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	String_Interf f;    /** function **/
	char *name;      /** its name **/
} StringTable;

int C2F(sci_length) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_part) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_string) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_convstr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_emptystr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_str2code) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_code2str) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ssort) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strcat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strindex) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsubst) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ascii) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_grep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tokens) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsplit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_length1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_regexp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strindex1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsubst1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_grep1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_regexppri) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_emptystr1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strcat1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_part1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_convstr1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ascii1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_stripblanks1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsplit1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tokens1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_code2str1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_str2code1) _PARAMS((char *fname,unsigned long fname_len));
void strsplit1(char **Str,char **Str3,int *u,int *v,int *w,int mn,int *m4);
void tokens(char **Str,char **Str2,char **Str3,int *u,int *w,int mn,int mn2);
void code2str(char **Str3,int m1, int n1, int *m4);
void convstr(char **Str, char **Str3, char typ, int mn);
void stripblanks(char **Str,char **Str3,int mn,int n4);
void str2code(char **Str,int *values,int *nbValues);
void scistrcat(char **Str,char **Str2,int mn);

#endif /*  __GW_STRING__ */
/*-----------------------------------------------------------------------------------*/
