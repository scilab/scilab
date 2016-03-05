/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

/*--------------------------------------------------------------------------*/
#include "forbiddenToUseScilab.h"
#include <Windows.h>
/*--------------------------------------------------------------------------*/
BOOL forbiddenToUseScilab(void)
{
    BOOL bOK = FALSE;
    HDC hdc = GetDC(NULL);
    int BitsByPixel = GetDeviceCaps(hdc, BITSPIXEL);

    ReleaseDC (NULL, hdc);

    if ( BitsByPixel < 8 )
    {
        MessageBox(NULL, "Warning", "Scilab supports only 256 colors or more.\n", MB_ICONSTOP);
        exit(1);
    }

    bOK = TRUE;
    return bOK;
}
/*--------------------------------------------------------------------------*/
