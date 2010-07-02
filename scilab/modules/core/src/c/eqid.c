/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "eqid.h"
#include "stack-def.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int C2F(eqid)(int *x, int *y)
{
	/**
	* Intel C++ Compiler provides optimized versions of memcpy, memset, memcmp, 
	* and memmove inside the run-time library.
	* http://software.intel.com/en-us/articles/memcpy-performance/
	* http://origin-software.intel.com/en-us/forums/showthread.php?t=63277&o=d&s=lr
	* http://software.intel.com/sites/products/documentation/hpc/compilerpro/en-us/cpp/lin/main_cls_lin.pdf
	*/
	#ifndef __INTEL_COMPILER
	int i = 0;
	for (i = 0; i < nsiz; i++)
	{
		if (x[i] != y[i])
		{
			return FALSE;
		}
	}
	return TRUE;
	#else
	return (memcmp(x, y, sizeof(int) * nsiz) ? FALSE : TRUE);
	#endif
} 
/*--------------------------------------------------------------------------*/

