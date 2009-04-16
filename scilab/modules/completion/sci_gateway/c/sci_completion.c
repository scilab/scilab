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
#include "gw_completion.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "completion.h"
#include "Scierror.h"
#include "toolsdictionary.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static int returnEmptyMatrix(int pos);
static int putResultOnStack(int pos,char **result,int sizeresult);
/*--------------------------------------------------------------------------*/
int C2F(sci_completion)(char *fname,unsigned long fname_len)
{
	int l1 = 0, m1 = 0, n1 = 0;
	char *partOfWord = NULL;

	CheckRhs(1,2);
	CheckLhs(1,6);

	if (GetType(1) == sci_strings)
	{
		char ** Inputs1 = NULL;
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Inputs1);
		if ( (m1 == n1) && (n1 == 1) )
		{
			partOfWord = Inputs1[0];
		}
		else
		{
			freeArrayOfString(Inputs1,m1 * n1);
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname,1);
		return 0;
	}

	if (Lhs == 1)
	{
		if (Rhs == 1)
		{
			char **Results = NULL;
			int sizeResults = 0;

			Results = completion(partOfWord, &sizeResults);
			putResultOnStack(1,Results,sizeResults);
			freePointerDictionary(Results,sizeResults);
			C2F(putlhsvar)();
		}
		else /* Rhs == 2 */
		{
			if (GetType(2) == sci_strings)
			{
				char **Inputs2 = NULL;
				char *param2 = NULL;
				char **Results = NULL;
				int sizeResults = 0;

				GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Inputs2);
				if ( (m1 == n1) && (n1 == 1) )
				{
					param2 = Inputs2[0];
				}
				else
				{
					freeArrayOfString(Inputs2,m1 * n1);
					Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
					return 0;
				}

				if ( strcmp(param2,"functions") == 0 )
				{
					Results = completionOnFunctions(partOfWord, &sizeResults);
				}
				else if ( strcmp(param2,"commands") == 0 )
				{
					Results = completionOnCommandWords(partOfWord, &sizeResults);
				}
				else if ( strcmp(param2,"variables") == 0 )
				{
					Results = completionOnVariablesWithoutMacros(partOfWord, &sizeResults);
				}
				else if( strcmp(param2,"macros") == 0 )
				{
					Results = completionOnMacros(partOfWord, &sizeResults);
				}
				else if( strcmp(param2,"graphic_properties") == 0 )
				{
					Results = completionOnHandleGraphicsProperties(partOfWord, &sizeResults);
				}
				else if( strcmp(param2,"files") == 0 )
				{
					Results = completionOnFiles(partOfWord, &sizeResults);
				}
				else
				{
					freeArrayOfString(Inputs2,m1*n1);
					Scierror(999,_("%s: Wrong value for input argument: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n"),fname,"functions","commands","variables","macros","graphic_properties","files");
					return 0;
				}
				putResultOnStack(1,Results,sizeResults);
				freePointerDictionary(Results,sizeResults);
				freeArrayOfString(Inputs2,m1*n1);
				C2F(putlhsvar)();

			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,2);
				return 0;
			}
		}
	}
	else
	{
		char **FunctionsList = NULL;
		int sizeFunctionsList = 0;

		if (Rhs == 2)
		{
			Scierror(999,_("%s: Wrong number of output argument(s).\n"),fname);
			return 0;
		}

		FunctionsList = completionOnFunctions(partOfWord, &sizeFunctionsList);
		putResultOnStack(1,FunctionsList,sizeFunctionsList);
		freePointerDictionary(FunctionsList,sizeFunctionsList);

		if (Lhs > 1)
		{
			char **CommandsList = NULL;
			int sizeCommandsList = 0;

			CommandsList = completionOnCommandWords(partOfWord, &sizeCommandsList);
			putResultOnStack(2,CommandsList,sizeCommandsList);

			freePointerDictionary(CommandsList,sizeCommandsList);
		}

		if (Lhs > 2)
		{
			char **VariablesList = NULL;
			int sizeVariablesList = 0;

			VariablesList = completionOnVariablesWithoutMacros(partOfWord, &sizeVariablesList);
			putResultOnStack(3,VariablesList,sizeVariablesList);

			freePointerDictionary(VariablesList,sizeVariablesList);
		}

		if (Lhs > 3)
		{
			char **MacrosList = NULL;
			int sizeMacrosList = 0;

			MacrosList = completionOnMacros(partOfWord, &sizeMacrosList);
			putResultOnStack(4,MacrosList,sizeMacrosList);

			freePointerDictionary(MacrosList,sizeMacrosList);
		}

		if (Lhs > 4)
		{
			char **HandleGraphicsPropertiesList = NULL;
			int sizeHandleGraphicsPropertiesList = 0;

			HandleGraphicsPropertiesList = completionOnHandleGraphicsProperties(partOfWord, &sizeHandleGraphicsPropertiesList);
			putResultOnStack(5,HandleGraphicsPropertiesList,sizeHandleGraphicsPropertiesList);

			freePointerDictionary(HandleGraphicsPropertiesList,sizeHandleGraphicsPropertiesList);
		}

		if (Lhs > 5)
		{
			char **PathsList = NULL;
			int sizePathsList = 0;

			PathsList = completionOnFiles(partOfWord, &sizePathsList);
			putResultOnStack(6,PathsList,sizePathsList);

			freePointerDictionary(PathsList,sizePathsList);
		}

		C2F(putlhsvar)();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int returnEmptyMatrix(int pos)
{
	int l1 = 0, m1 = 0, n1 = 0;
	CreateVarFromPtr(Rhs+ pos,MATRIX_OF_DOUBLE_DATATYPE,&n1,&m1,&l1);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int putResultOnStack(int pos,char **result,int sizeresult)
{
	if (result)
	{
		int m1 = 0, n1 = 0;
		m1 = sizeresult;
		n1 = 1;
		CreateVarFromPtr(Rhs+pos,MATRIX_OF_STRING_DATATYPE, &m1, &n1, result);
	}
	else
	{
		returnEmptyMatrix(pos);
	}
	LhsVar(pos) = Rhs+pos;
	return 0;
}
/*--------------------------------------------------------------------------*/
