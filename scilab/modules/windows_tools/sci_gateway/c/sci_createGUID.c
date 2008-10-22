/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "createGUID.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_createGUID(char *fname,unsigned long l)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_matrix)
	{
		int m1 = 0, n1 = 0, l1 = 0;
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		if ( (m1 == 1) && (n1 == 1) )
		{
			int numberOfGuids = (int)*stk(l1);
			
			if ( numberOfGuids > 0 )
			{
				char **GUIDs = (char **)MALLOC(numberOfGuids*(sizeof(char*)));

				if (GUIDs)
				{
					int mOutput = 0, nOutput = 0;
					int i = 0;
					for (i = 0; i < numberOfGuids ; i++)
					{
						GUIDs[i] = createGUID();
					}

					mOutput = numberOfGuids;
					nOutput = 1;
					CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&mOutput,&nOutput,GUIDs);

					freeArrayOfString(GUIDs,mOutput*nOutput);

					LhsVar(1) = Rhs+1 ;
					C2F(putlhsvar)();
				}
				else
				{
					Scierror(999,_("%s : Memory allocation error.\n"),fname);
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument: Non-negative integers expected.\n"),fname);
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
