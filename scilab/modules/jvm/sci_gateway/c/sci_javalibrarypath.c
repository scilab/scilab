/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "Scierror.h"
#include "addToLibrarypath.h"
#include "getLibrarypath.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_javalibrarypath(char *fname,unsigned long fname_len)
{
	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		int nbRow=0;
		int nbCol=1;
		char **Strings=NULL;

		Strings=getLibrarypath(&nbRow);
		CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol,Strings );

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		freeArrayOfString(Strings,nbRow*nbCol);
	}
	else
	{
		if ( GetType(1) == sci_strings )
		{
			static int n1=0,m1=0;
			int i=0;
			BOOL bOK=FALSE;
			char **libraryPaths=NULL;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&libraryPaths);

			for (i = 0; i<m1*n1 ;i++)
			{
				bOK=addToLibrarypath(libraryPaths[i]);
				if (!bOK)
				{
					Scierror(999,_("%s: Could not add path to java.library.path: %s.\n"),fname, libraryPaths[i]);
					freeArrayOfString(libraryPaths,m1*n1);
					return 0;
				}
			}
			LhsVar(1) = 0;
			C2F(putlhsvar)();	
			freeArrayOfString(libraryPaths,m1*n1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname,1);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
