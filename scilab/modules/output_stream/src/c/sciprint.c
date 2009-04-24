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
#include <stdio.h>
#include <string.h>
#include "sciprint.h"
#include "../../fileio/includes/diary.h"
#include "stack-def.h" /* bsiz */
#include "scilabmode.h"
#include "../../console/includes/ConsolePrintf.h"
#ifdef _MSC_VER
#include "TermPrintf.h"
#endif
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*--------------------------------------------------------------------------*/ 
/* sciprint uses scivprint */
/* scivprint uses printf_scilab */
/*--------------------------------------------------------------------------*/ 
/*
* Print
* works as sciprint
* difference is that the argument list is a pointer to a list of arguments
* @param fmt Format of the format string
* @param [in] args
*/
static int scivprint(char *fmt,va_list args);
/*--------------------------------------------------------------------------*/ 
/**
* print a string 
* @param[in] buffer to disp
*/
static void printf_scilab(char *buffer);
/*--------------------------------------------------------------------------*/ 
void sciprint(char *fmt,...) 
{
	va_list ap;

	va_start(ap,fmt);
	scivprint(fmt,ap);
	va_end (ap);
}
/*--------------------------------------------------------------------------*/ 
static int scivprint(char *fmt,va_list args) 
{
	static char s_buf[MAXPRINTF];
	int count=0;

	va_list savedargs;
	va_copy(savedargs, args);
	
	count= vsnprintf(s_buf,MAXPRINTF-1, fmt, args );
	if (count == -1) s_buf[MAXPRINTF-1]='\0';

	printf_scilab(s_buf);
	
	va_end(savedargs);

	return count;
}
/*--------------------------------------------------------------------------*/ 
/* as sciprint but with an added first argument which is ignored (used in do_printf) */
int sciprint2 (int iv, char *fmt,...)
{
	int count = 0;
	va_list ap;

	va_start(ap,fmt);
	count = scivprint(fmt, ap);
	va_end (ap);

	return count;
}
/*--------------------------------------------------------------------------*/ 
static void printf_scilab(char *buffer)
{
	if (buffer)
	{
		if (getScilabMode() == SCILAB_STD)
		{
			ConsolePrintf(buffer);
		}
		else
		{
			#ifdef _MSC_VER
			TermPrintf_Windows(buffer);
			#else
			printf("%s",buffer);
			#endif
		}

		if ( getdiary() ) 
		{
			// diary output line
			int lstr = (int)strlen(buffer);
			diary(buffer,&lstr,FALSE);
		}
	}
}
/*--------------------------------------------------------------------------*/ 
