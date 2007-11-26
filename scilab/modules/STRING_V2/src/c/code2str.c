/*------------------------------------------------------------------------*/
/* File: code2str.c                                                      */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : str=code2str(c)
          Returns character string associated with Scilab integer codes.
          str is such that c(i) is the Scilab integer code of part(str,i))*/
/*------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "code2str.h"
#define Seperate_Num 62
/*--------------------------------------------------------------------------*/
/* The table of Scilab integer codes */
const char Table_One[] = 
{'0','1','2','3','4','5','6','7','8','9',
 'a','b','c','d','e','f','g','h','i','j',
 'k','l','m','n','o','p','q','r','s','t',
 'u','v','w','x','y','z','_','#','!','$',
 ' ','(',')',';',':','+','-','*','/','\\',
 '=','.',',','\'','[',']','%','|','&','<',
 '>','~','^'};
/*--------------------------------------------------------------------------*/
/* The table of Scilab integer codes */
const char Table_Two[] = 
{'0','1','2','3','4','5','6','7','8','9',
 'A','B','C','D','E','F','G','H','I','J',
 'K','L','M','N','O','P','Q','R','S','T',
 'U','V','W','X','Y','Z','0','0','?','0',
 '0','0','0','0','0','0','0','0','0','$',
 '0','0','0','"','{','}','0','0','0','`',
 '0','@','0'};
/*--------------------------------------------------------------------------*/
void code2str(char **Output_Matrix,int Number_of_Row, int Number_of_Col, int *Input_Matrix)
{
    int x = 0;

	/* To look up in the table */
	for (x=0;x < Number_of_Row*Number_of_Col;x++)                     
	{
		if (Input_Matrix[x] >= 0) Output_Matrix[0][x] = Table_One[Input_Matrix[x]];
		else Output_Matrix[0][x] = Table_Two[abs(Input_Matrix[x])];
	}
	Output_Matrix[0][Number_of_Row*Number_of_Col] = 0;
}
/*--------------------------------------------------------------------------*/
int str2code(int *Output_Matrix, char **Input_String)
{
	int i = 0;
	int nbOutput_Matrix = 0;

	for (i = 0;i < (int)strlen(Input_String[0]);i++)
    {
		int x = 0;
        int key=0;

        for (x=0;x <= Seperate_Num;x++)       /* To look up in the table */
		{
			if (Input_String[0][i] == Table_One[x])
			{
				key=1;
				Output_Matrix[nbOutput_Matrix++]=x;
			}    
		}

        if (key==0)
		{
			for (x=0;x<=Seperate_Num;x++)
			{
				if (Input_String[0][i] == Table_Two[x])
				{
					key=1;
					Output_Matrix[nbOutput_Matrix++]=-x;
				}       
			}
		}
    }
	return nbOutput_Matrix;
}
/*--------------------------------------------------------------------------*/
