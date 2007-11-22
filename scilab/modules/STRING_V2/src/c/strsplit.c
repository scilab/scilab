/*------------------------------------------------------------------------*/
/* File: sci_convstr1.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/*------------------------------------------------------------------------*/
#include "strsplit.h"
/*------------------------------------------------------------------------*/
void strsplit(char **Input_StringMatrix,char **Output_StringMatrix,int *Row_Pointer,int *Row_Pointer_Two,int *Col_Pointer,int mn,int *Input_IntMatrix)
{
	int x = 0;
	/*To traverse every string of the input matrix*/
	for (x=0; x < strlen(Input_StringMatrix[0]) ; x++)
	{
		/* To put them in the correct order */
		if (x != Input_IntMatrix[*Row_Pointer_Two]) 
		{
			Output_StringMatrix[*Row_Pointer][(*Col_Pointer)++] = Input_StringMatrix[0][x];
		}
		else 
		{
			
			(*Row_Pointer)++;
			(*Row_Pointer_Two)++;
			*Col_Pointer=0;
			Output_StringMatrix[*Row_Pointer][(*Col_Pointer)++] = Input_StringMatrix[0][x];
		}
        Output_StringMatrix[*Row_Pointer][*Col_Pointer]=0;
	}
}
/*------------------------------------------------------------------------*/
