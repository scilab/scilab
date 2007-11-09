/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include "sciprint.h"
#include "sciprint_nd.h"
#include "../../console/includes/ConsolePrintf.h"
#include "../../fileio/includes/diary.h"
#include "scilabmode.h"
#include "stack-def.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*-----------------------------------------------------------------------------------*/ 
void sciprint(char *fmt,...) 
{
	va_list ap;
	int count = 0;
	char s_buf[MAXPRINTF];

	va_start (ap, fmt);
	count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
	va_end (ap);
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}

	sciprint_nd(s_buf);

	if (getdiary()) 
	{
		integer lstr = (integer)strlen(s_buf);
		diary_nnl(s_buf,&lstr);
	}
}
/*-----------------------------------------------------------------------------------*/ 
/* as sciprint but with an added first argument which is ignored (used in do_printf) */
int sciprint2 (int iv, char *fmt,...)
{
	va_list ap;
	int count = 0;
	char s_buf[MAXPRINTF];

	va_start (ap, fmt);
	count= vsnprintf(s_buf,MAXPRINTF-1, fmt, ap );
	va_end (ap);
	if (count == -1)
	{
		s_buf[MAXPRINTF-1]='\0';
	}

	sciprint(s_buf);
	return count;
}
/*-----------------------------------------------------------------------------------*/ 
