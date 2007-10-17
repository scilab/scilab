/** 
 * INRIA 2007
 * @author Allan CORNET 
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> (update with gettext)
 */

#include <stdlib.h>
#include "message_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "../../../localization/includes/localization.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
/*-----------------------------------------------------------------------------------*/
int message_scilab(char *Tag,...)
{
	int ret=0;

	va_list ap;
	char s_buf[MAXCHARSSCIPRINT_FULL];
	int count=0;

	va_start(ap,Tag);
#if defined(linux) || defined(_MSC_VER)
	{
		count= vsnprintf(s_buf,MAXCHARSSCIPRINT_FULL-1, _(Tag), ap );
		if (count == -1)
			{
				s_buf[MAXCHARSSCIPRINT_FULL-1]='\0';
			}
	}
#else
	(void )vsprintf(s_buf, _(Tag), ap );
#endif
	va_end(ap);
	sciprint(s_buf);
	sciprint("\n");
	ret=1;
	
	return ret;
}
/*-----------------------------------------------------------------------------------*/
