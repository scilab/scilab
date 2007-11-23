/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <ctype.h>
#include <string.h>
#include "stristr.h"
/*--------------------------------------------------------------------------*/
char *stristr(const char *psz,const char *tofind)
{
	const char *ptr = psz;
	const char *ptr2;

	while(1)
	{
		ptr = strchr(psz,toupper(*tofind));
		ptr2 = strchr(psz,tolower(*tofind));
		if (!ptr)
		{
			ptr = ptr2; /* was ptr2 = ptr.  Big bug fixed 10/22/99 */
		}
		if (!ptr)
		{
			break;
		}
		if (ptr2 && (ptr2 < ptr))
		{
			ptr = ptr2;
		}
		if (!_strnicmp(ptr,tofind,strlen(tofind)))
		{
			return (char *) ptr;
		}
		psz = ptr+1;
	}
	return 0;
} 
/*--------------------------------------------------------------------------*/
