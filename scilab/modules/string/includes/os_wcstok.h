/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OS_WCSTOK_H__
#define __OS_WCSTOK_H__

#include <wchar.h>
#include "dynlib_string.h"


STRING_IMPEXP wchar_t* os_wcstok(wchar_t *_pwstData, const wchar_t *_pwstDelim, wchar_t** _pswtState);
STRING_IMPEXP char* os_strtok(char *_pstData, const char *_pstDelim, char** _pstState);

#endif /* !__OS_WCSDUP_H__ */
