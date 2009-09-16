/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
* Copyright (C) 2008 - DIGITEO - Vincent COUVERT
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_uigetfile.cpp                                                */
/* desc : interface for uigetfile routine                                 */
/*------------------------------------------------------------------------*/
#include "CallJuigetfile.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_gui.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "BOOL.h"
}
using namespace org_scilab_modules_gui_filechooser;
/*--------------------------------------------------------------------------*/

int sci_uigetfile(char *fname, unsigned long fname_len) 
{
	int nbRow = 0, nbCol = 0;
	int nbRow2 = 0, nbCol2 = 0;
	int nbRow3 = 0, nbCol3 = 0;
	int nbRow4 = 0, nbCol4 = 0;

	int nbRowOutSelection = 1, nbColOutSelection = 0;
	int nbRowOutFilterIndex = 1, nbColOutFilterIndex = 1;
	int nbRowOutPath = 1, nbColOutPath = 1;

	int voidSelectionAdr = 0;

	char *optName = NULL;

	char **mask = NULL; 
	char ** description = NULL;
	char **titleBox = NULL, *selectionPathName = NULL;
	char ** initialDirectory = NULL;
	int multipleSelection = 0;
	int multipleSelectionAdr  = NULL;

	char **selection = NULL;
	char **selectionFileNames = NULL;
	int selectionSize = 0;  		
	int filterIndex = 0; 

	char *menuCallback = NULL;

	CheckRhs(0,4);
	CheckLhs(1,3);

	if ((optName = (char*)MALLOC(sizeof(char*)*(strlen("title")+1))) == NULL)
	{
		Scierror(999, _("%s: No more memory.\n"), fname);
		return 0;
	}

	//inputs checking
	/* call uigetfile with 1 arg */
	if (Rhs >= 1) 
	{
		if (VarType(1) != sci_strings) 
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A string matrix expected.\n"),fname, 1);
			FREE(optName);
			return 0;
		}

		GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &mask);

		if (nbCol == 1)
		{
			// only masks of files are provided
			description = NULL;
		}
		else if (nbCol == 2)
		{
			// mask contains both the masks for files and the description of each mask
			// in the sequence [m1, m2,..., mn, d1, d2,...,dn].
			// So description is at the middle of the array.
			description = (char**)MALLOC( sizeof(char*) * nbRow);
			for (int i = 0;i < nbRow; i++)
			{
				description[i] = strdup(mask[nbRow+i]);
			}
		}
		else
		{
			Scierror(999, _("%s: Wrong size for input argument #%d: A string matrix expected.\n"),fname, 1);
			FREE(optName);
			return 0;
		}
	}

	/* call uigetfile with 2 arg */
	if (Rhs >= 2)
	{
		int out_n = 0;
		char *path = NULL;

		if (VarType(2) != sci_strings) 
		{ 
			Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 2);
			FREE(optName);
			return 0;
		}

		GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow2, &nbCol2, &initialDirectory);

		if (nbCol2 != 1 || nbRow2 != 1) 
		{
			Scierror(999, _("%s: Wrong size for input argument #%d: A string  expected.\n"),fname, 2);
			FREE(optName);
			return 0;
		}

		path = expandPathVariable(initialDirectory[0]);
		FREE(initialDirectory[0]);
		initialDirectory[0] = path;
	}

	/* call uigetfile with 3 arg */
	if (Rhs >= 3)
	{
		if (VarType(3) != sci_strings) 
		{ 
			Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 3);
			FREE(optName);
			return 0;
		}

		GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &nbRow3, &nbCol3, &titleBox);

		if (nbCol3 != 1 || nbRow3 != 1) 
		{
			Scierror(999, _("%s: Wrong size for input argument #%d: A string  expected.\n"),fname, 3);
			FREE(optName);
			return 0;
		}
	}

	/* Call Java */
	switch (Rhs)
	{
		case 0:
			CallJuigetfileWithoutInput();
		break;

		case 1:
			CallJuigetfileOnlyWithMask(mask, description, nbRow);
		break;

		case 2:
			CallJuigetfileWithMaskAndInitialdirectory(mask, description, nbRow, initialDirectory[0]);
		break;

		case 3:
			CallJuigetfileWithoutMultipleSelection(mask, description, nbRow, initialDirectory[0], titleBox[0]);
		break;

		case 4:
		{
			if (VarType(4) != sci_boolean) 
			{ 
				Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 4);
				FREE(optName);
				return 0;
			}				

			GetRhsVar(4, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow4, &nbCol4, &multipleSelectionAdr);

			if (nbCol4 != 1 || nbRow4 != 1) 
			{
				Scierror(999, _("%s: Wrong size for input argument #%d: A boolean matrix expected.\n"),fname, 4);
				FREE(optName);
				return 0;
			}			
			multipleSelection = istk(multipleSelectionAdr)[0];

			CallJuigetfile(mask, description, nbRow, initialDirectory[0], titleBox[0], BOOLtobool(multipleSelection));
		}
		break;

		default:
			// never here
		break;
	}

	// free pointer
	freeArrayOfString(description, nbRow);
	freeArrayOfString(mask,nbRow * nbCol);
	freeArrayOfString(initialDirectory,nbRow2 * nbCol2);
	freeArrayOfString(titleBox,nbRow3 * nbCol3);

	// Get return values
	selection = getJuigetfileSelection();
	selectionPathName = getJuigetfileSelectionPathName();
	selectionFileNames = getJuigetfileSelectionFileNames();
	selectionSize = getJuigetfileSelectionSize();
	multipleSelection = getJuigetfileMultipleSelection();
	filterIndex = getJuigetfileFilterIndex();
	menuCallback = getJuigetfileMenuCallback();

	// nbColOutSelection
	nbColOutSelection = selectionSize;

	//if cancel is selected on the filechooser
	if (strcmp(selection[0], "") == 0)
	{
		nbRowOutSelection = 1; 
		nbColOutSelection = 1;

		// "" is returned as filename
		CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRowOutSelection, &nbColOutSelection, selection);
		LhsVar(1) = Rhs + 1 ;

		if (Lhs > 1)
		{
			// "" is returned as pathname
			CreateVarFromPtr(Rhs+2, MATRIX_OF_STRING_DATATYPE, &nbRowOutSelection, &nbColOutSelection, selection);
			LhsVar(2) = Rhs + 2 ;
		}

		if (Lhs > 2)
		{
			// 0 is returned as pathname
			double *tmp = (double*)MALLOC(sizeof(double));
			if (tmp == NULL)
			{
				Scierror(999, _("%s: No more memory.\n"), fname);
				return 0;
			}
			tmp[0] = 0;
			CreateVarFromPtr(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &nbRowOutSelection, &nbColOutSelection, &tmp);
			FREE(tmp); tmp = NULL;
			LhsVar(3) = Rhs + 3 ;
		}
		C2F(putlhsvar)();
		return 0;
	}

	// Only one output then it contains path+filenames
	if (Lhs == 1) 
	{
          CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRowOutSelection, &nbColOutSelection, selection);
          LhsVar(1) = Rhs + 1 ;
          C2F(putlhsvar)();
          return 0;
	}

	// More than one output
	CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRowOutSelection, &nbColOutSelection, selectionFileNames);
	
	nbColOutPath = (int)strlen(selectionPathName);
	CreateVarFromPtr(Rhs+2, STRING_DATATYPE, &nbColOutPath,&nbRowOutPath, &selectionPathName);

	LhsVar(1) = Rhs + 1 ;
	LhsVar(2) = Rhs + 2 ;
	if (Lhs > 2)
	{
		double *tmp = (double*)MALLOC(sizeof(double));
		if (tmp == NULL)
		{
			Scierror(999, _("%s: No more memory.\n"), fname);
			return 0;
		}
		tmp[0] = filterIndex;
		CreateVarFromPtr(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &nbRowOutFilterIndex, &nbColOutFilterIndex, &tmp);
		FREE(tmp); tmp = NULL;
		LhsVar(3) = Rhs + 3 ;
	}
        
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
