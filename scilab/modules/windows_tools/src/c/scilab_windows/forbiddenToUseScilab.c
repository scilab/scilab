/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "forbiddenToUseScilab.h"
#include <Windows.h>
/*-----------------------------------------------------------------------------------*/ 
BOOL forbiddenToUseScilab(void)
{
	BOOL bOK=FALSE;
	HDC hdc=GetDC(NULL);
	int BitsByPixel = GetDeviceCaps(hdc, BITSPIXEL);

	ReleaseDC (NULL, hdc);

	if ( BitsByPixel < 8 )
	{
		MessageBox(NULL,"Warning", "Scilab supports only 256 colors or more.\n",MB_ICONSTOP);
		exit(1);
		return bOK;
	}

	bOK=TRUE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
