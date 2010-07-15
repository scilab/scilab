/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OS_WCSDUP_H__
#define __OS_WCSDUP_H__

#include <wchar.h>

// Windows
#ifdef _MSC_VER
#define os_wcsdup       _wcsdup
#endif

// Linux
#ifdef __linux__
#define os_wcsdup       wcsdup
#endif

// MacOS X
#ifdef __APPLE__

inline wchar_t *macOSwcsdup(const wchar_t *_pwcsSource)
{
    wchar_t * pwcsDest = (wchar_t *) malloc(sizeof(wchar_t) * (wcslen(_pwcsSource) + 1));
    wcscpy(pwcsDest, _pwcsSource);

    return pwcsDest;
}

#define os_wcsdup       macOSwcsdup
#endif

#endif /* !__OS_WCSDUP_H__ */
