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
#include "Scierror.h"
#include "MALLOC.h"
#include "charEncoding.h"
#include "yaspio.hxx"
#include "lasterror.h"

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#define vsnprintf _vsnprintf
    #define vsnwprintf _vsnwprintf
#endif
/*--------------------------------------------------------------------------*/
/* Scilab Error at C level */
/*--------------------------------------------------------------------------*/ 
int  Scierror(int iv,const char *fmt,...)
{
	int retval = 0;
	int lstr = 0;
	char s_buf[bsiz];
	va_list ap;

	va_start(ap,fmt);

#if _MSC_VER
	retval = vsnprintf(s_buf,bsiz-1, fmt, ap );
#else
	retval = sprintf(s_buf,fmt, ap );
#endif
	if (retval < 0) s_buf[bsiz-1]='\0';

	lstr = (int) strlen(s_buf);
	va_end(ap);

    YaspWrite(s_buf);
    YaspWrite("\n");
	return retval;
}

int ScierrorW(int iv, const wchar_t *fmt,...)
{
	int retval = 0;
	int lstr = 0;
	wchar_t s_buf[bsiz];
	va_list ap;

	va_start(ap,fmt);

#if _MSC_VER
	retval = vsnwprintf(s_buf,bsiz-1, fmt, ap );
#else
	retval = vswprintf(s_buf, bsiz-1, fmt, ap );
#endif
	if (retval < 0) s_buf[bsiz-1]='\0';

	lstr = (int) wcslen(s_buf);
	va_end(ap);


    setLastError(iv, s_buf, 0, NULL);
    YaspWriteW(s_buf);
    YaspWriteW(L"\n");

	return retval;
}
/*--------------------------------------------------------------------------*/
