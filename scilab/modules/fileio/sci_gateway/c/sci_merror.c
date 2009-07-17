/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include <stdlib.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "merror.h"
#include "Scierror.h"
#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "MALLOC.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
#define PREVIOUS_FILE_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_merror(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int one = 1, lr = 0;
	int fd = PREVIOUS_FILE_DESCRIPTOR;

	Nbvars = 0;
	CheckRhs(0,1);
	CheckLhs(1,2);

	if (Rhs == 0)
	{
		char *errmsg = NULL;

		int ierr = 0;

		C2F(merror)(&fd, &ierr);

		CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&lr);
		*stk(lr) = (double)ierr;
		LhsVar(1) = Rhs + 1;

		if (Lhs == 2)
		{
			if (ierr == 0)
			{
				errmsg = strdup("");
			}
			else
			{
				errmsg = strdup(strerror(ierr));
			}
			if (errmsg)
			{
				n1 = 1;
				CreateVarFromPtr(Rhs + 2,STRING_DATATYPE,(m1 = (int)strlen(errmsg), &m1),&n1,&errmsg);
				LhsVar(2) = Rhs + 2;
				FREE(errmsg);
				errmsg = NULL;
			}
		}

		C2F(putlhsvar)();
		return 0;
	}

	if (GetType(1) == sci_matrix)
	{
		if (Rhs == 1)
		{
			GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			if (m1*n1 == 1)
			{
				fd  = *istk(l1);

				if ( GetFileOpenedInScilab(fd) )
				{
					int ierr = 0;
					C2F(merror)(&fd, &ierr);

					CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&lr);
					*stk(lr) = (double)ierr;
					LhsVar(1) = Rhs + 1;

					if (Lhs == 2)
					{
						char *errmsg = NULL;

						if (ierr == 0)
						{
							errmsg = strdup("");
						}
						else
						{
							errmsg = strdup(strerror(ierr));
						}

						if (errmsg)
						{
							n1 = 1;
							CreateVarFromPtr(Rhs + 2,STRING_DATATYPE,(m1 = (int)strlen(errmsg), &m1),&n1,&errmsg);
							LhsVar(2) = Rhs + 2;

							FREE(errmsg);
							errmsg = NULL;
						}
					}
					C2F(putlhsvar)();
				}
				else
				{
					Scierror(999,_("%s: Cannot read file whose descriptor is %d: File is not active.\n"),fname,fd);
				}
			}
			else
			{
				Scierror(999, _("%s: Wrong size for input argument #%d: A integer expected.\n"), fname,1);
			}
		}
	}
	else
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
