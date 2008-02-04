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
#include "getfastcode.h"
/*--------------------------------------------------------------------------*/
void code2str(char **Output_Matrix, int *Input_Matrix,int Dim_Input_Matrix)
{
    int x = 0;
	/* To look up in the table */
	for ( x = 0;x < Dim_Input_Matrix; x++)                     
	{
		Output_Matrix[0][x] = convertScilabCodeToAsciiCode(Input_Matrix[x]);
	}
}
/*--------------------------------------------------------------------------*/
void str2code(int *Output_Matrix, char **Input_String)
{
	int i = 0;
	int nbOutput_Matrix = (int)strlen(Input_String[0]);

	for (i = 0;i < nbOutput_Matrix ;i++)
    {
		Output_Matrix[i] = convertAsciiCodeToScilabCode(Input_String[0][i]);
    }
}
/*--------------------------------------------------------------------------*/
