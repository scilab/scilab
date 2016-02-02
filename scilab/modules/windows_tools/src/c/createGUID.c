/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include <unknwn.h>
#include "createGUID.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#define _OLEAUT32_
/*--------------------------------------------------------------------------*/
wchar_t *createGUID(void)
{
    GUID guid;
    wchar_t* pwstrGUID = NULL;
    wchar_t* ret = NULL;

    CoCreateGuid (&guid);
    StringFromCLSID(&guid, &pwstrGUID);

    //remove first '{' and last '}'
    pwstrGUID[wcslen(pwstrGUID) - 1] = L'\0';
    ret = os_wcsdup(pwstrGUID + 1);
    CoTaskMemFree(pwstrGUID);
    return ret;
}
/*--------------------------------------------------------------------------*/
