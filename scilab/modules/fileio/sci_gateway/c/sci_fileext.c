/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "FindFileExtension.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_fileext(char *fname,unsigned long fname_len)
{
	Rhs = Max(Rhs,0);

	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		int n1 = 0, m1 = 0, l1 = 0;
		int i = 0;

		char **Input_filenames  = NULL;
		char **Output_extensions = NULL;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_filenames);

		Output_extensions = (char**)MALLOC(sizeof(char*)*(m1*n1));
		if (Output_extensions)
		{
			
			for (i = 0; i < m1*n1; i++)
			{
				if (Input_filenames[i])
				{
					/* Bug 3089 */
					Output_extensions[i] = FindFileExtension(Input_filenames[i]);
				}
				else
				{
					Output_extensions[i] = NULL;
				}
			}
			CreateVarFromPtr( Rhs+1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, Output_extensions );
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
			freeArrayOfString(Input_filenames, m1*n1);
			freeArrayOfString(Output_extensions,m1*n1);
		}
		else
		{
			freeArrayOfString(Input_filenames, m1*n1);
			Scierror(999,_("%s: No more memory.\n"),fname);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/

