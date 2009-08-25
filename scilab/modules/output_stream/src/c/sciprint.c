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
#include "diary.h"
#include "stack-def.h" /* bsiz */
#include "scilabmode.h"
#include "../../console/includes/ConsolePrintf.h"
#ifdef _MSC_VER
#include "TermPrintf.h"
#endif
#include "MALLOC.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*--------------------------------------------------------------------------*/ 
/* sciprint uses scivprint */
/* scivprint uses printf_scilab */
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
int scivprint(char *fmt,va_list args) 
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
static void printf_scilab(char *buffer)
{
	if (buffer)
	{
		wchar_t *wcBuffer = NULL;
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

		wcBuffer = to_wide_string(buffer);
		if (wcBuffer)
		{
			diaryWrite(wcBuffer, FALSE);
			FREE(wcBuffer);
			wcBuffer = NULL;
		}
	}
}
/*--------------------------------------------------------------------------*/ 
