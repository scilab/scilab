/*----------------------------------------------------------------------------*/
/* Copyright INRIA 2007 */
/* @Author : Allan CORNET */
/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "stringsstrrchr.h"
#include "MALLOC.h"
/*----------------------------------------------------------------------------*/
char ** strings_strrchr(char **InputStrings,int Dim_InputStrings,char** InputChar,int Dim_InputChar,BOOL do_strrchr)
{
	char **Output_Strings = NULL;

	if ( (InputStrings) && (InputChar) )
	{
		Output_Strings = (char**)MALLOC(sizeof(char*)*Dim_InputStrings);
		if (Output_Strings)
		{
			int i = 0;
			for(i = 0;i < Dim_InputStrings; i++)
			{
				int c = 0;
				char *ptrStr = NULL;

				if (Dim_InputChar == 1) c = InputChar[0][0];
				else  c = InputChar[i][0];

				if (do_strrchr) ptrStr = strrchr(InputStrings[i],c);
				else ptrStr = strchr(InputStrings[i],c);

				if (ptrStr)
				{
					Output_Strings[i] = (char*)MALLOC(sizeof(char)*(strlen(ptrStr)+1));
					if (Output_Strings[i]) strcpy(Output_Strings[i],ptrStr);
				}
				else
				{
					Output_Strings[i] = (char*)MALLOC(sizeof(char)*(strlen("")+1));
					if (Output_Strings[i]) strcpy(Output_Strings[i],"");
				}
			}
		}
	}
	return Output_Strings;
}
/*--------------------------------------------------------------------------*/
