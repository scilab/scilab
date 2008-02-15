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
#include <stdlib.h>
#include "getvariablesname.h"
#include "stackinfo.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
static void SortStrings(char **Strings,int SizeStrings);
static void RemoveDuplicateStrings(char **Strings,int *SizeStrings);
/*--------------------------------------------------------------------------*/ 
char **getVariablesName(int *sizearray, BOOL sorted)
{
	char **variables = NULL;

	char **localvariables = NULL;
	int sizelocalvariables = 0;

	char **globalvariables = NULL;
	int sizeglobalvariables = 0;

	localvariables = getLocalVariablesName(&sizelocalvariables,sorted);
	globalvariables = getGlobalVariablesName(&sizeglobalvariables,sorted);

	if (localvariables && globalvariables)
	{
		int i = 0;
		variables = (char **)MALLOC(sizeof(char*)*(sizelocalvariables+sizeglobalvariables+1));
		for (i = 0; i < sizelocalvariables ; i++) variables[i] = localvariables[i];
		for (i = sizelocalvariables; i < sizelocalvariables + sizeglobalvariables; i++) 
		{
			variables[i] = globalvariables[i - sizelocalvariables];
		}
		FREE(localvariables); localvariables = NULL;
		FREE(globalvariables); globalvariables = NULL;
	}

	if (variables)
	{
		*sizearray = sizelocalvariables+sizeglobalvariables;
		if (sorted) SortStrings(variables,*sizearray);
		RemoveDuplicateStrings(variables,sizearray);
	}
	else
	{
		*sizearray = 0;
	}
	return variables;
}
/*----------------------------------------------------------------------------------*/ 
char **getLocalVariablesName(int *sizearray,BOOL sorted)
{
	char **variablesLocal = NULL;
	int Ltotal = 0;
	int Lused = 0;
	int j = 0;

	C2F(getvariablesinfo)(&Ltotal,&Lused);

	if (Lused) 
	{
		variablesLocal = (char **)MALLOC(sizeof(char*)*(Lused+1));
		if (variablesLocal)
		{
			for (j=1;j<Lused+1;++j) variablesLocal[j-1] = getLocalNamefromId(j);
			*sizearray = Lused;
			if (sorted) SortStrings(variablesLocal,*sizearray);
		}
		else
		{
			*sizearray = 0;
		}
	}
	else
	{
		*sizearray = 0;
	}

	return variablesLocal;
}
/*--------------------------------------------------------------------------*/ 
char **getGlobalVariablesName(int *sizearray, BOOL sorted)
{
	char **variablesGlobal = NULL;
	int Gtotal = 0;
	int Gused = 0;
	int j = 0;

	C2F(getgvariablesinfo)(&Gtotal,&Gused);

	if (Gused) 
	{
		variablesGlobal = (char **)MALLOC(sizeof(char*)*(Gused+1));
		if (variablesGlobal)
		{
			for (j=0;j<Gused;++j) variablesGlobal[j] = getGlobalNamefromId(j);
			*sizearray = Gused;
			if (sorted) SortStrings(variablesGlobal,*sizearray);
		}
		else
		{
			*sizearray = 0;
		}
	}
	else
	{
		*sizearray = 0;
	}

	return variablesGlobal;
}
/*--------------------------------------------------------------------------*/ 
static void SortStrings(char **Strings,int SizeStrings)
{
	int fin,i;
	for(fin=SizeStrings-1;fin>0;fin--)
	{
		int Sorted=FALSE;
		for(i=0;i<fin;i++)
		{
			if(strcmp(Strings[i],Strings[i+1])>0)
			{
				char *StringTmp;

				StringTmp = Strings[i];

				Strings[i] = Strings[i+1];
				Strings[i+1] = StringTmp;

				Sorted=TRUE;
			}
		}
		if(!Sorted)break;
	}
}
/*--------------------------------------------------------------------------*/ 
static void RemoveDuplicateStrings(char **Strings,int *SizeStrings)
{
	int fin,i;
	int newsize = *SizeStrings;
	for(fin=*SizeStrings-1;fin>0;fin--)
	{
		int Sorted=FALSE;
		for(i=0;i<fin;i++)
		{
			if (Strings[i])
			{
				if(strcmp(Strings[i],Strings[i+1]) == 0)
				{
					FREE(Strings[i+1]);
					Strings[i+1] = NULL;
					Sorted=TRUE;
					newsize--;
				}
			}
			else
			{
				Strings[i] = Strings[i+1];
				Strings[i+1] = NULL;
				Sorted=TRUE;
			}
		}
		if(!Sorted)break;
	}

	*SizeStrings = newsize;
}
/*--------------------------------------------------------------------------*/ 
