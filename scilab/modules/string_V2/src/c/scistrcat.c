/*------------------------------------------------------------------------*/
/* File: scistrcat.c                                                    */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : catenate character strings                                      */
/*        The main function                                               */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h" 
#include "src_string.h"
/*-------------------------------------------------------------------------------------*/
void scistrcat(char **Str,char **Str2,int mn)
{
	int x;
	if (mn>1) 
	  {
  		for (x=1;x<mn-1;++x)  /* Travers for every argument*/
		{
			if (strlen(Str2[0])==0) 
			{
					Scierror(999, "2th argument must not be an empty string");
			}
			strcat(Str[0],Str[x]);  /* Put the each of the two argument together */
			strcat(Str[0] ,Str2[0]);
		}
		strcat(Str[0] ,Str[mn-1]);
	  }
}