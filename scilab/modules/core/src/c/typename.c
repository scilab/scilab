/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "typename.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "cvstr.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
extern TYPE_struct C2F(typnams);
extern int C2F(addtypename)(int *, char*, int *,int); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
static BOOL isTypeNameAlreadyExist(char *name);
/*--------------------------------------------------------------------------*/
char **getAllTypesName(int *sizeArray)
{
	char **TypeNames = NULL;
	int numberOfTypes = getNumberOfTypes();
	*sizeArray = 0;

	if (numberOfTypes > 0)
	{
		TypeNames = (char**)MALLOC(sizeof(char*) * numberOfTypes);
		if (TypeNames)
		{
			int i = 0;
			int j = 0;

			for(i = 0; i < numberOfTypes; i++)
			{
				TypeNames[i] = (char*)MALLOC(sizeof(char)*(MAX_LENGTH_NAME_DEFINED_TYPE + 1));
				if (TypeNames[i] == NULL)
				{
					freeArrayOfString(TypeNames,i);
					*sizeArray = 0;
					return TypeNames;
				}
			}

			for(i = 0;i < MAX_SCILAB_DEFINED_TYPE; i++)
			{
				if ( C2F(typnams).ln[i] != 0) 
				{
					int job = 1;
					int lengthTypeName = C2F(typnams).ln[i];
					C2F(cvstr)(&lengthTypeName,&C2F(typnams).namrec[C2F(typnams).ptr[i]-1],TypeNames[j],&job,lengthTypeName);
					TypeNames[j][lengthTypeName] = '\0';
					j++;
				}
			}
			*sizeArray = j;
		}
	}
	return TypeNames;
}
/*--------------------------------------------------------------------------*/
int *getAllTypesNumber(int *sizeArray)
{
	int *TypeNumbers = NULL;
	int numberOfTypes = getNumberOfTypes();
	*sizeArray = 0;

	if (numberOfTypes > 0)
	{
		TypeNumbers = (int*)MALLOC(sizeof(int)*numberOfTypes);
		if (TypeNumbers)
		{
			int i = 0;
			int j = 0;
			for(i = 0;i < MAX_SCILAB_DEFINED_TYPE; i++)
			{
				if ( C2F(typnams).ln[i] != 0) 
				{
					TypeNumbers[j] = C2F(typnams).tp[i];
					j++;
				}
			}
            *sizeArray = j;
		}
	}
	return TypeNumbers;
}
/*--------------------------------------------------------------------------*/
int getNumberOfTypes(void)
{
	int NumberOfTypes = 0;
	int i = 0;
	for(i = 0;i < MAX_SCILAB_DEFINED_TYPE; i++)
	{
		if ( C2F(typnams).ln[i] != 0) NumberOfTypes++;
	}
	return NumberOfTypes;
}
/*--------------------------------------------------------------------------*/
int addNamedType(char *name,int val)
{
	int ierr = 0;
	if (isTypeNameAlreadyExist(name)) ierr = -1;
	else C2F(addtypename)(&val,name,&ierr,(int)strlen(name));
	return ierr;
}
/*--------------------------------------------------------------------------*/
BOOL isTypeNameAlreadyExist(char *name)
{
	int sizeArray = 0;
	char **TypeNames = getAllTypesName(&sizeArray);
	int i = 0;

	if (TypeNames)
	{
		for(i = 0;i < sizeArray; i++)
		{
			if (strcmp(name,TypeNames[i]) == 0)
			{
				freeArrayOfString(TypeNames,sizeArray);
				return TRUE;
			}
		}
		freeArrayOfString(TypeNames,sizeArray);
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
