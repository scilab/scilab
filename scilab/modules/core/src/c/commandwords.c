/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "commandwords.h"
#include "MALLOC.h"
#include "BOOL.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

/*--------------------------------------------------------------------------*/ 
/* if you update command.f , please update this file too ... */
/*--------------------------------------------------------------------------*/ 
static const int nbrCommands=29;
/*--------------------------------------------------------------------------*/ 
static char *CommandWords[]={
 "if", "else",
 "for", "while",
 "end", "select",
 "case", "quit",
 "return", "help",
 "what", "who",
 "pause", "clear",
 "resume", "then",
 "do", "apropos",
 "abort", "break",
 "elseif", "pwd",
 "function", "endfunction",
 "clc", "continue",
 "try", "catch",
 "exit"
};
/*--------------------------------------------------------------------------*/ 
static void SortStrings(char **Strs,int SizeOfStrs);
/*--------------------------------------------------------------------------*/ 
char **getcommandkeywords(int *sizearray)
{
	char **keywords = NULL;

	keywords = (char**)MALLOC(sizeof(char*)*nbrCommands);

	if (keywords)
	{
		int i = 0;
		for (i = 0;i < nbrCommands; i++)
		{
			keywords[i] = strdup(CommandWords[i]);
		}
		*sizearray =  nbrCommands;

		SortStrings(keywords,*sizearray);
	}
	else
	{
		*sizearray = 0;
	}

	return keywords;
}
/*--------------------------------------------------------------------------*/ 
static void SortStrings(char **Strs,int SizeOfStrs)
{
	int fin,i;
	for(fin=SizeOfStrs-1;fin>0;fin--)
	{
		int Sorted=FALSE;
		for(i=0;i<fin;i++)
		{
			if(strcmp(Strs[i],Strs[i+1])>0)
			{
				char *tmp;
				memcpy(&tmp,Strs+i,sizeof(char *));
				memcpy(Strs+i,Strs+(i+1),sizeof(char *));
				memcpy(Strs+(i+1),&tmp,sizeof(char *));
				Sorted=TRUE;
			}
		}
		if(!Sorted)break;
	}
}
/*--------------------------------------------------------------------------*/
