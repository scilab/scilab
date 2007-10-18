/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "error_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "sciprint.h"
#include "../../../localization/includes/localization.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
/*-----------------------------------------------------------------------------------*/
int error_scilab(int iv,char *Tag,...)
{
	int ret=0;

	va_list ap;
	char s_buf[MAXCHARSSCIPRINT_FULL];
	int count=0;

	va_start(ap,Tag);
#if defined(linux) || defined(_MSC_VER)
	{
		count= vsnprintf(s_buf,MAXCHARSSCIPRINT_FULL-1, Tag, ap );
		if (count == -1)
			{
				s_buf[MAXCHARSSCIPRINT_FULL-1]='\0';
			}
	}
#else
	(void )vsprintf(s_buf, Tag, ap );
#endif
	va_end(ap);
	ret=Scierror(iv,s_buf);
	sciprint("\n");

	return ret;
}
/*-----------------------------------------------------------------------------------*/
