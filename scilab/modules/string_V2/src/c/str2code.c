/*------------------------------------------------------------------------*/
/* File: sci_str2code1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : This function return scilab integer codes associated with a 
          character string    The algoritm of str2code                    */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h" 
/*-------------------------------------------------------------------------------------*/
void str2code(char **Str,int *values,int *nbValues)
{
   char m5[]={'0','1','2','3','4','5','6','7','8','9',   /* The table of the code */
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
	int key,x;
	for (x=0;x<=62;x++)       /* To look up in the table */
		if (Str[0][0] == m5[x]) 
 		{
			key=1;
            values[(*nbValues)++]=x;
		}	
		if (key==0) 
			for (x=0;x<=62;x++)
				if (Str[0][0] == m6[x]) 
 				{
					key=1;
					values[(*nbValues)++]=-x;
				}	 
}