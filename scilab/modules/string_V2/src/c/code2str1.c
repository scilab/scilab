/*------------------------------------------------------------------------*/
/* File: code2str1.c                                                      */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : str=code2str(c)
          Returns character string associated with Scilab integer codes.
          str is such that c(i) is the Scilab integer code of part(str,i))*/
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
#include "src_string.h"
/*-----------------------------------------------------------------------------------*/
void code2str(char **Str3,int m1, int n1, int *m4)
{
    int x;
	char m5[]={'0','1','2','3','4','5','6','7','8','9',
           'a','b','c','d','e','f','g','h','i','j',
           'k','l','m','n','o','p','q','r','s','t',
           'u','v','w','x','y','z','_','#','!','$',
           ' ','(',')',';',':','+','-','*','/','\\',
           '=','.',',','\'','[',']','%','|','&','<','>','~',
		   '^'};
	char m6[]={'0','1','2','3','4','5','6','7','8','9',
           'A','B','C','D','E','F','G','H','I','J',
           'K','L','M','N','O','P','Q','R','S','T',
           'U','V','W','X','Y','Z','0','0','?','0',
           '0','0','0','0','0','0','0','0','0','$',
           '0','0','0','"','{','}','0','0','0','`','0','@',
		   '0'};
	for (x=0;x<m1*n1;x++)                     /* To look up in the table */
		   if (m4[x]>=0) 
	        Str3[0][x]=m5[m4[x]];
		   else Str3[0][x]=m6[abs(m4[x])];
	   Str3[0][m1*n1]=0;
}