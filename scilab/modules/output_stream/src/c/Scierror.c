
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "Scierror.h"
#include "stack-def.h" /* bsiz */
#include "error_internal.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#define vsnprintf _vsnprintf
#endif
/*--------------------------------------------------------------------------*/ 
/* Scilab Error at C level */
/*--------------------------------------------------------------------------*/ 
int  Scierror(int iv,char *fmt,...) 
{
	int retval = 0;
	integer lstr = 0;
	char s_buf[bsiz];
	va_list ap;
	
	va_start(ap,fmt);

#if defined (vsnprintf) || defined (linux)
	retval = vsnprintf(s_buf,bsiz-1, fmt, ap );
#else
	retval = vsprintf(s_buf,fmt, ap );
#endif
	if (retval == -1) s_buf[bsiz-1]='\0';

	lstr = (integer)strlen(s_buf);
	va_end(ap);

	error_internal(&iv,s_buf,ERROR_FROM_C);

	return retval;
}
/*--------------------------------------------------------------------------*/
