/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/*--------------------------------------------------------------------------*/ 
#include "sciprint_nd.h"
#include "scilabmode.h"
#include "stack-def.h"
#include "../../console/includes/ConsolePrintf.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*--------------------------------------------------------------------------*/ 
void sciprint_nd(char *fmt,...) 
{
	va_list ap;

	va_start(ap,fmt);
	scivprint_nd(fmt,ap) ;
	va_end(ap);
}
/*--------------------------------------------------------------------------*/ 
void scivprint_nd(char *fmt,va_list args) 
{
	if (getScilabMode() == SCILAB_STD)
	{
		static char s_buf[MAXPRINTF];

		#if defined(linux) || defined(_MSC_VER)
		{
			int count=0;

			count= vsnprintf(s_buf,MAXPRINTF-1, fmt, args );

			if (count == -1) s_buf[MAXPRINTF-1]='\0';
		}
		#else
		(void )vsprintf(s_buf, fmt, args );
		#endif

		ConsolePrintf(s_buf);
	}
	else
	{
		vprintf(fmt,args);
	}
}
/*--------------------------------------------------------------------------*/ 
