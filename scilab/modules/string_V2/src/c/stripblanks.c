/*------------------------------------------------------------------------*/
/* File: stripblanks1.c                                                   */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : The algorithm of stripblank
                                                                          */                                    
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
#include "src_string.h"
static char* substr(const char*str, unsigned start, unsigned end)
{
	unsigned n = end - start;
	char *stbuf;
    stbuf=(char*)MALLOC(sizeof(char*)*(1));
	strncpy(stbuf, str + start, n);   /*Put a part of str  into stbuf*/
	stbuf[n] = 0;
	return stbuf;
}
void stripblanks(char **Str,char **Str3,int mn,int n4)
{
    int x,l1,l2;
	char *_replacedstr;
	for (x=0;x<mn;x++)   /*Search for all the matrix*/
	{
		if (n4==0) 
		{                 /*strips leading and trailing blanks*/
			l2=strlen(Str[x])-1;
			while (l2>0 &&  Str[x][l2]==' ')  --l2; /* To find the last character which is not a blank*/
			l1=0;
			while (l1<l2 &&  Str[x][l1]==' ') ++l1; /* To find the first character which is not a blank*/
			if (l1<=l2 && Str[x][l1]!=' ') 
			{
				_replacedstr=substr(Str[x], l1, l2+1 ); /*Get the substring without a blank*/ 
				strcpy(Str3[x],_replacedstr);          /*To add the substring into the output matrix*/
			}
		}
			else  
			{                    /*strips leading and trailing tabs*/
				l2=strlen(Str[x])-1;
				while (l2>0 &&  Str[x][l2]==9)  --l2;   /* To find the last character which is not a tab  The ascii code of a tab is 9*/
				l1=0;
				while (l1<l2 &&  Str[x][l1]==9) ++l1;   /* To find the first character which is not a tab*/
				if (l1<=l2 && Str[x][l1]!=9 && Str[x][l1]!=' ') 
				{
					_replacedstr=substr(Str[x], l1, l2+1 ); /*Get the substring without tabs*/ 
					strcpy(Str3[x],_replacedstr);     /*To add the substring into the output matrix*/
				}
			}
	}
}