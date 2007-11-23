/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include "sciprint.h"
#include "sciprint_nd.h"
#include "../../fileio/includes/diary.h"
#include "stack-def.h" /* bsiz */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*--------------------------------------------------------------------------*/ 
/* sciprint uses scivprint */
/* scivprint uses scivprint_nd */
/* sciprint_nd uses scivprint_nd */
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
	scivprint_nd(fmt,args);

	if (getdiary()) 
	{
		int count = 0;
		char s_buf[MAXPRINTF];
		integer lstr = 0;

		count= vsnprintf(s_buf,MAXPRINTF-1, fmt, args );

		if (count == -1) s_buf[MAXPRINTF-1]='\0';

		lstr = (integer)strlen(s_buf);
		diary_nnl(s_buf,&lstr);
	}
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

	sciprint(s_buf);
	return count;
}
/*--------------------------------------------------------------------------*/ 
