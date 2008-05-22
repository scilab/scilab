
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
#include "sci_sleep.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_sleep)(char *fname,unsigned long fname_len)
{
	integer m1,n1,l1,sec=0;

	CheckLhs(0,1);
	CheckRhs(1,1);
	if (Rhs == 1)
	{
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		CheckScalar(1,m1,n1);
		sec = (integer) *stk(l1);
		if (sec <= 0)
		{
			Scierror(999,_("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),fname,1);
			return 0;
		}

	#ifdef _MSC_VER
		{
			int ms = (sec); /** time is specified in milliseconds in scilab**/
			if (ms > 0) Sleep(ms); /* Number of milliseconds to sleep. */
		}
	#else
		{
			unsigned useconds;
			useconds = (unsigned) sec;
			if (useconds != 0)
				#ifdef HAVE_USLEEP
					{ usleep(useconds*1000); }
				#else
					#ifdef HAVE_SLEEP
					{  sleep(useconds*1000); }
					#endif
				#endif
		}
	#endif
	}
	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
