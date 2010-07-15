/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OS_WCSICMP_H__
#define __OS_WCSICMP_H__

#ifdef _MSC_VER
#include <wchar.h>
#define os_wcsicmp _wcsicmp
#endif

#ifdef __linux__
#include <wchar.h>
#define os_wcsicmp wcscasecmp
#endif

#ifdef __APPLE__
#include <wchar.h>
#include <wctype.h>

inline int macOSwcscasecmp(const wchar_t *_pwcsS1, const wchar_t *_pwcsS2)
{
    int iResult = 0;
    int i = 0;

    wchar_t *pwcsLowerS1 = (wchar_t *) malloc(sizeof(wchar_t) * wcslen(_pwcsS1) + 1);
    wchar_t *pwcsLowerS2 = (wchar_t *) malloc(sizeof(wchar_t) * wcslen(_pwcsS2) + 1);;

    wcscpy(pwcsLowerS1, _pwcsS1);
    wcscpy(pwcsLowerS2, _pwcsS2);

    // Lower S1
    for(i = 0 ; i < wcslen(_pwcsS1) ; ++i)
    {
        pwcsLowerS1[i] = towlower(_pwcsS1[i]);
    }

    // Lower S2
    for(i = 0 ; i < wcslen(_pwcsS2) ; ++i)
    {
        pwcsLowerS2[i] = towlower(_pwcsS2[i]);
    }

    iResult = wcscmp(pwcsLowerS1, pwcsLowerS2);
    free(pwcsLowerS1);
    free(pwcsLowerS2);
    return iResult;
}

#define os_wcsicmp macOSwcscasecmp
#endif

#endif /* !__OS_WCSICMP_H__ */
