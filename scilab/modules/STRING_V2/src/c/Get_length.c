/*------------------------------------------------------------------------*/
/* File: Get_length.c                                                     */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : Caculate the length of the output string                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "Get_length.h"
/*------------------------------------------------------------------------*/
int Get_length(char **Input_String_One,char **Input_String_Two,int Number_Input_One)
{
    int length = 0;
	int i = 0;

	for (i = 0; i < Number_Input_One; i++)    
	{  
		length = length + (int)strlen(Input_String_One[i]) + (int)strlen(Input_String_Two[0]);
	}
	length = length - (int)strlen(Input_String_Two[0]);
	return length;
}
/*------------------------------------------------------------------------*/
