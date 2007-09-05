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
#include "../../console/includes/ConsolePrintf.h"
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
	lstr=(int)strlen(buf);

	if (getScilabMode() == SCILAB_STD)
	{
		ConsolePrintf(buf);
	}
	else
	{
		printf("%s",buf); 
	}
}
/*-----------------------------------------------------------------------------------*/ 

