/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO -  Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "string.h"
#include "os_wcsdup.h"

#ifdef __APPLE__
wchar_t *macOSwcsdup(const wchar_t *_pwcsSource)
{
    wchar_t * pwcsDest = (wchar_t *) malloc(sizeof(wchar_t) * (wcslen(_pwcsSource) + 1));
    wcscpy(pwcsDest, _pwcsSource);

    return pwcsDest;
}
#endif

