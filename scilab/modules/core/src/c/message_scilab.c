/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "message_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "../../../localization/includes/QueryStringMessage.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
/*-----------------------------------------------------------------------------------*/
int message_scilab(char *Tag,...)
{
	int ret=0;
	char *LocalizedString=QueryStringMessage(Tag);

	if (LocalizedString)
	{
		va_list ap;
		char s_buf[MAXCHARSSCIPRINT_FULL];
		int count=0;

		va_start(ap,Tag);
		#if defined(linux) || defined(_MSC_VER)
		{
			count= vsnprintf(s_buf,MAXCHARSSCIPRINT_FULL-1, LocalizedString, ap );
			if (count == -1)
			{
				s_buf[MAXCHARSSCIPRINT_FULL-1]='\0';
			}
		}
		#else
		(void )vsprintf(s_buf, LocalizedString, ap );
		#endif
		va_end(ap);
		sciprint(s_buf);
		sciprint("\n");
		ret=1;
	}
	else
	{
		Scierror(999,"ERROR : localized message not found : %s",Tag);
		return 0;
	}
	
	if (LocalizedString) {FREE(LocalizedString);LocalizedString=NULL;}
	return ret;
}
/*-----------------------------------------------------------------------------------*/
