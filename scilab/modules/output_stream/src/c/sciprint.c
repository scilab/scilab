
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
#include "sciprint_nd.h"
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
/* scivprint uses scivprint_nd */
/* sciprint_nd uses scivprint_nd */
/* all use printf_scilab at the end */
/*--------------------------------------------------------------------------*/ 
void sciprint(char *fmt,...) 
{
	va_list ap;

	va_start(ap,fmt);
	scivprint(fmt,ap);
	va_end (ap);
}
/*--------------------------------------------------------------------------*/ 
void scivprint(char *fmt,va_list args) 
{
	va_list savedargs;
	va_copy(savedargs, args);
	
	scivprint_nd(fmt,args);

	if (getdiary()) 
	{
		int count = 0;
		char s_buf[MAXPRINTF];
		integer lstr = 0;

		count= vsnprintf(s_buf,MAXPRINTF-1, fmt, savedargs );

		if (count == -1) s_buf[MAXPRINTF-1]='\0';

		lstr = (integer)strlen(s_buf);
		diary_nnl(s_buf,&lstr);
	}
	
	va_end(savedargs);
}
/*--------------------------------------------------------------------------*/ 
/* as sciprint but with an added first argument which is ignored (used in do_printf) */
int sciprint2 (int iv, char *fmt,...)
{
	va_list ap;
	int count = 0;
	char s_buf[MAXPRINTF];

	va_start(ap,fmt);
	count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
	va_end (ap);

	if (count == -1) s_buf[MAXPRINTF-1]='\0';

	printf_scilab(s_buf,TRUE);

	return count;
}
/*--------------------------------------------------------------------------*/ 
void printf_scilab(char *buffer,BOOL withDiary)
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

		if ( (withDiary) && getdiary() ) 
		{
			int lstr = (int)strlen(buffer);
			diary_nnl(buffer,&lstr);
		}
	}
}
/*--------------------------------------------------------------------------*/ 
