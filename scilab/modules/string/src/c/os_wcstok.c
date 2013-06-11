/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "os_wcstok.h"


wchar_t *os_wcstok(wchar_t *_pwstData, const wchar_t *_pwstDelim, wchar_t** _pswtState)
{
#ifndef _MSC_VER
    return wcstok(_pwstData, _pwstDelim, _pswtState);
#else
    return wcstok(_pwstData, _pwstDelim);
#endif
}
