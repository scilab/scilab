/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xlfontt.c                                                    */
/* desc : interface for xlfont routine                                    */
/*------------------------------------------------------------------------*/

#include "stack-c.h"
#include "RendererFontManager.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "FileExist.h"
/*--------------------------------------------------------------------------*/
static int xlfont_no_rhs(char * fname);
static int xlfont_one_rhs(char * fname);
static int xlfont_n_rhs(char * fname);
/*--------------------------------------------------------------------------*/
int sci_xlfont( char * fname, unsigned long fname_len )
{
	CheckRhs(0,4);
	CheckLhs(0,1);

	switch (Rhs)
	{
		case 0:
			return xlfont_no_rhs(fname);
			break;
		case 1:
			return xlfont_one_rhs(fname);
			break;
		default:
			return xlfont_n_rhs(fname);
			break;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int xlfont_no_rhs(char * fname)
{
	int m1 = 0, n1 = 0;
	int nbElements = 0;
	char **fontsname = getInstalledFontsName(&nbElements);

	m1 = 1;
	n1 = nbElements;
	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, fontsname );

	freeArrayOfString(fontsname,nbElements);

	LhsVar(1) = Rhs+1 ;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int xlfont_one_rhs(char * fname)
{
	if (VarType(1) == sci_strings)
	{
		int m1 = 0, n1 = 0, l1 = 0;
		GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
		if ( strcmp(cstk(l1),"AVAILABLE_FONTS")==0)
		{
			int nbElements = 0;
			char **fontsname = getAvailableFontsName(&nbElements);

			m1 = nbElements;
			n1 = 1;
			CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, fontsname );

			freeArrayOfString(fontsname,nbElements);

			LhsVar(1) = Rhs+1 ;
			PutLhsVar();
			return 0;
		}
		else if ( strcmp(cstk(l1),"reset")==0)
		{
			resetFontManager();
			LhsVar(1) = 0 ;
			PutLhsVar();
			return 0;
		}
		else
		{
			if (isAvailableFontsName(cstk(l1)))
			{
				int fontID = addFont(cstk(l1));

				m1 = 1; n1 = 1; l1 = 0;
				CreateVar( Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1 );
				*istk(l1) = fontID ;

				LhsVar(1) = Rhs+1 ;
				PutLhsVar();
				return 0;
			}
			else if (FileExist(cstk(l1)))
			{
				int fontID = addFontFromFilename(cstk(l1));

				m1 = 1; n1 = 1; l1 = 0;
				CreateVar( Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1 );
				*istk(l1) = fontID ;

				LhsVar(1) = Rhs+1 ;
				PutLhsVar();
				return 0;
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: A valid fontname expected.\n"),fname,1);
			}
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int xlfont_n_rhs(char * fname)
{
	BOOL isBold = FALSE;
	BOOL isItalic = FALSE;

	if ( Rhs == 3 )
	{
		int m3 = 0, n3 = 0, l3 = 0;

		if ( VarType(3) != sci_boolean )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"),fname,3); 
			return 0;
		}

		GetRhsVar(3,MATRIX_OF_BOOLEAN_DATATYPE,&m3,&n3,&l3);
		isBold = (BOOL) *istk(l3);
	}

	if ( Rhs == 4 )
	{
		int m4 = 0, n4 = 0, l4 = 0;

		if ( VarType(4) != sci_boolean )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"),fname,3); 
			return 0;
		}
		GetRhsVar(4,MATRIX_OF_BOOLEAN_DATATYPE,&m4,&n4,&l4);
		isItalic = (BOOL) *istk(l4);
	}

	if ( (VarType(1) == sci_strings) && (VarType(2) == sci_matrix) )
	{
		int m1 = 0, n1 = 0, l1 = 0;
		int m2 = 0, n2 = 0, l2 = 0;
		GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
		GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
		if ( (m2 == 1) && (n2 == 1) )
		{
			int fontIndex = (int)(*stk(l2));
			char *fontname = cstk(l1);
			if (fontIndex < 0)
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: Non-negative int expected.\n"),fname,2);
				return 0;
			}

			if ( (Rhs == 2) && FileExist(fontname) )
			{
				int Id = changeFontFromFilename(fontIndex,fontname);
				m1 = 1; n1 = 1; l1 = 0;
				CreateVar( Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1 );
				*istk(l1) = Id ;

				LhsVar(1) = Rhs+1 ;
				PutLhsVar();
			}
			else if ( isAvailableFontsName(fontname) )
			{
				int Id = changeFontWithProperty(fontIndex,fontname,isBold,isItalic);
				m1 = 1; n1 = 1; l1 = 0;
				CreateVar( Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1 );
				*istk(l1) = Id ;

				LhsVar(1) = Rhs+1 ;
				PutLhsVar();
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: A valid fontname expected.\n"),fname,1);
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
		}
	}
	else
	{
		if ( VarType(1) != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1); 
			return 0;
		}

		if ( VarType(2) == sci_matrix )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Non-negative int expected.\n"),fname,2); 
			return 0;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
