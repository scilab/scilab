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
#include <stdio.h>
#include <time.h>

#ifndef _MSC_VER
	#include <sys/time.h>
#else 
	#include <windows.h>
	#include <winbase.h> /* header du compteur haute résolution */
#endif 

#include "timer.h"



/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
	static __int64 i64UserTick1;
	static LARGE_INTEGER   Tick1;
#else
	static clock_t t1;
#endif
/*--------------------------------------------------------------------------*/
static int init_clock = 1;
/*--------------------------------------------------------------------------*/
int C2F(timer)(double *etime)
{
#ifdef _MSC_VER 
  /* NT */
	/* Return CPU Time */
	FILETIME  ftCreation, ftExit, ftKernel,  ftUser;
  __int64 i64UserTick2;
	
	GetProcessTimes(GetCurrentProcess(), &ftCreation, &ftExit, &ftKernel, &ftUser);
	i64UserTick2=*((__int64 *) &ftUser);
	if (init_clock == 1) {init_clock = 0; i64UserTick1 = i64UserTick2;}
	*etime=(double) ((double)(i64UserTick2 - i64UserTick1)/(double)10000000U);
	i64UserTick1 = i64UserTick2;
#else
  clock_t t2;
  t2 = clock();
  if (init_clock == 1) {init_clock = 0; t1 = t2;}
  *etime=(double)((double)(t2 - t1)/(double)CLOCKS_PER_SEC);
  t1 = t2;
#endif
  return(0);
}
/*--------------------------------------------------------------------------*/
