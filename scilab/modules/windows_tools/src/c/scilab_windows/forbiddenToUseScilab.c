/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#include "forbiddenToUseScilab.h"
#include <Windows.h>
/*--------------------------------------------------------------------------*/ 
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
	}

	bOK=TRUE;
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
