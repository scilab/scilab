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
#ifdef _MSC_VER
#include <Windows.h>
#endif
#ifdef HAVE_USLEEP
#include <unistd.h>
#endif
#include "gw_time.h"
#include "stack-c.h"
#include "api_common.h"
#include "api_double.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_sleep(char *fname,unsigned long fname_len)
{
  int m1 = 0, n1 = 0, sec=0;
  int * p1_in_address = NULL;
  int res = 0;
  double * pDblReal = NULL;

  CheckLhs(0,1);
  CheckRhs(1,1);

  if (Rhs == 1)
    {
      getVarAddressFromPosition(1, &p1_in_address);
      res = getMatrixOfDouble(p1_in_address, &m1, &n1, &pDblReal);

      CheckScalar(1,m1,n1);

      sec = (int)  *pDblReal;
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
