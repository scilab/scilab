/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "isDrive.h"
/*--------------------------------------------------------------------------*/ 
BOOL isDrive(const char *strname)
{
	BOOL bOK=FALSE;
	#ifdef _MSC_VER
	if (strname)
	{
		if ( ((strlen(strname) == 2) || (strlen(strname) == 3)) && (strname[1]== ':') )
		{
			if (strlen(strname) == 3)
			{
				if ( (strname[2]!= '\\') && (strname[2]!= '/') )
				{
					return FALSE;
				}
			}

			if ( ( strname[0] >= 'A' && strname[0] <= 'Z' ) || ( strname[0] >= 'a' && strname[0] <= 'z' ) )
			{
				bOK =TRUE;
			}
		}
	}
	#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
