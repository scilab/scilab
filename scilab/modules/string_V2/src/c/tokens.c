/*------------------------------------------------------------------------*/
/* File: sci_tokens1.c                                                    */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : returns the tokens of a character string.  
          The main function
		                                                                  */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
#include "src_string.h"
/*------------------------------------------------------------------------*/
void tokens(char **Str,char **Str2,char **Str3,int *u,int *w,int mn,int mn2)
{
	int x,i,bo;
	unsigned y;
	for (x=0;x<mn;x++)
	{
		for (y=0;y<strlen(Str[x]);y++)             /*To traverse every string in the input string matrix */
		{
				for (i=0; i<mn2; i++)  
					if (Str[x][y]==Str2[i][0]) bo=1;
				if    (bo==0) Str3[*u][(*w)++]=Str[0][y];    /*bo is a singal, to put the input string matrix into the output matrix in a correct token way*/
				else 
				{
					Str3[*u][*w]=0;               /* To start copying from the next row, and from the very beginning*/
					(*u)++;
					*w=0;
				}
				bo=0;
		}
	}
    Str3[*u][*w]=0;
}
/*------------------------------------------------------------------------*/