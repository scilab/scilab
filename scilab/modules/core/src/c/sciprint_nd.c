/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Allan CORNET */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
#include "sciprint_nd.h"
#include <stdarg.h>
#include "scilabmode.h"
#include "../../console/includes/ShellPrintf.h"
/*-----------------------------------------------------------------------------------*/ 
#define MAXPRINTF 512
/*-----------------------------------------------------------------------------------*/ 
void sciprint_nd(char *fmt,...) 
{
	int i = 0, count = 0, lstr = 0;
	va_list args;
	char buf[MAXPRINTF];
	va_start(args,fmt);

#if defined(linux) || defined(_MSC_VER)
	count = vsnprintf (buf,MAXPRINTF-1, fmt, args);
	if (count == -1)
	{
		buf[MAXPRINTF-1]='\0';
	}
#else
	(void ) vsprintf(buf, fmt, args );
#endif
	va_end(args);
	lstr=strlen(buf);

	if (getScilabMode() == SCILAB_STD)
	{
		ShellPrintf(buf);
	}
	else
	{
		printf("%s",buf); 
	}
}
/*-----------------------------------------------------------------------------------*/ 

