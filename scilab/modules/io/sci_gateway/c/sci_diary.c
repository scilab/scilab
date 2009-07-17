/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include "gw_io.h"
#include "stack-c.h"
#include "localization.h"
#include "mopen.h"
#include "mclose.h"
#include "diary.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "cluni0.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_diary)(char *fname,unsigned long fname_len)
{

	int m1 = 0, n1 = 0, l1 = 0;

	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0) /* This feature was not documented */
	{
		closeAllDiaries();

		LhsVar(1) = 0;
		C2F(putlhsvar)();

		return 0;
	}

	if (GetType(1) == sci_strings)
	{
		char ** Inputstr = NULL;
		
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Inputstr);

		if ( (m1 == n1) && (n1 == 1) )
		{
			int fd_diary = getdiary();

			int fd_newdiary = 0;
			int swap = 1;
			int ierr = 0;
			double res = 0.;

			char filename[PATH_MAX + FILENAME_MAX];
			long int lout  = PATH_MAX + FILENAME_MAX;
			int out_n = 0;

			C2F(cluni0)(Inputstr[0] , filename, &out_n,(int)strlen(Inputstr[0]),lout);
			freeArrayOfString(Inputstr,m1 *n1);

			C2F(mopen)(&fd_newdiary,filename,"wt",&swap,&res,&ierr);

			/* close previous diary */
			if ( (fd_diary != 0) && (ierr == 0))
			{
				double res = 0.0;
				C2F(mclose)(&fd_diary, &res);
			}

			setDiaryId(fd_newdiary);

			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			freeArrayOfString(Inputstr,m1 *n1);
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname,1);
			return 0;
		}
	}
	else if (GetType(1) == sci_matrix)
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
		if ( (m1 == n1) && (n1 == 1) )
		{
			double dzero = *stk(l1);
			int zero = (int) dzero;

			if (dzero != (double)zero)
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
				return 0;
			}

			closeAllDiaries();

			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A int expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
