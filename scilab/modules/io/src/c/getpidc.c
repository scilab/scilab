/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include <Windows.h>
#else
	#include <stdlib.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif
#include "getpidc.h"
/*--------------------------------------------------------------------------*/
int C2F(getpidc)(int *id1)
{
	#if _MSC_VER
		*id1=GetCurrentProcessId();
	#else
		*id1=getpid();
	#endif
	
	return(0) ;
}
/*--------------------------------------------------------------------------*/
