/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdarg.h>
#ifdef _MSC_VER
#include "message_scilab.h"
#endif
#include "MALLOC.h"
#include "sciprint.h"
#include "../../../localization/includes/QueryStringMessage.h"
/*-----------------------------------------------------------------------------------*/
int message_scilab(char *Tag,va_list argptr)
{
	int ret=0;
	char *LocalizedString=QueryStringMessage(Tag);

	if (LocalizedString)
	{
		sciprint(LocalizedString, argptr);
		ret=1;
	}
	else
	{
		sciprint("ERROR : localized message not found : %s",Tag);
	}
	sciprint("\n");
	if (LocalizedString) {FREE(LocalizedString);LocalizedString=NULL;}
	return ret;
}
/*-----------------------------------------------------------------------------------*/
