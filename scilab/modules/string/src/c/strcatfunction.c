/*------------------------------------------------------------------------*/
/* File: strcatfunction.c                                                 */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : conconcatenate: character strings                                  */
/*        The main function                                               */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "strcatfunction.h"
/*----------------------------------------------------------------------------*/
BOOL strcatfunction(char **Input_String_One,char **Input_String_Two,int Number_Input_One,char **Output_StringMatrix)
{
	int x = 0;

	if (Number_Input_One > 1)
	{
		/* Travers for every argument*/
		for (x = 1; x < (Number_Input_One-1) ;++x)
		{
			if (strlen(Input_String_Two[0]) == 0) return FALSE;

			/* Put the each of the two argument together */
			strcat(Output_StringMatrix[0],Input_String_One[x]);
			strcat(Output_StringMatrix[0] ,Input_String_Two[0]);
		}
		strcat(Output_StringMatrix[0] ,Input_String_One[Number_Input_One-1]);
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
