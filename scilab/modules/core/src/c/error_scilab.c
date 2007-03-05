/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdarg.h>
#ifdef _MSC_VER
#include "error_scilab.h"
#endif
#include "MALLOC.h"
#include "Scierror.h"
#include "sciprint.h"
#include "../../../localization/includes/QueryStringError.h"
/*-----------------------------------------------------------------------------------*/
int error_scilab(int iv,char *Tag,va_list argptr)
{
	int ret=0;
	char *LocalizedString=QueryStringError(Tag);

	if (LocalizedString)
	{
		ret=Scierror(iv,LocalizedString, argptr);
	}
	else
	{
		Scierror(iv,"ERROR : localized message not found : %s",Tag);
	}
	sciprint("\n");
	if (LocalizedString) {FREE(LocalizedString);LocalizedString=NULL;}
	return ret;
}
/*-----------------------------------------------------------------------------------*/
