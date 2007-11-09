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
#include "scilabmode.h"
#include "stack-def.h"
#include "../../console/includes/ConsolePrintf.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*-----------------------------------------------------------------------------------*/ 
void sciprint_nd(char *fmt,...) 
{
	int i = 0, count = 0, lstr = 0;
	va_list args;
	char s_buf[MAXPRINTF];
	va_start(args,fmt);

#if defined(linux) || defined(_MSC_VER)
	count = vsnprintf (s_buf,MAXPRINTF-1, fmt, args);
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}
#else
	(void ) vsprintf(s_buf, fmt, args );
#endif
	va_end(args);
	lstr=(int)strlen(s_buf);

	if (getScilabMode() == SCILAB_STD)
	{
		ConsolePrintf(s_buf);
	}
	else
	{
		printf("%s",s_buf); 
	}
}
/*-----------------------------------------------------------------------------------*/ 

