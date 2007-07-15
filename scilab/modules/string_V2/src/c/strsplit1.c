/*------------------------------------------------------------------------*/
/* File: sci_convstr1.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : converts the matrix of strings  str-matrix  into lower case (for*/  
/*          "l"  ;default value) or upper case (for  "u" ).               */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
#include "src_string.h"
/*------------------------------------------------------------------------*/
void strsplit1(char **Str,char **Str3,int *u,int *v,int *w,int mn,int *m4)
{
	int x;
	for (x=0;x<mn;x++)       /*To traverse every string of the input matrix*/
	{
		if (x!=m4[*v]) Str3[*u][(*w)++]=Str[0][x];   /* To put them in the correct order */
		else 
		{
			(*u)++;
			(*v)++;
			*w=0;
		}
        Str3[*u][*w]=0;
	}
}
/*------------------------------------------------------------------------*/