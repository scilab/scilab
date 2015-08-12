/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __STRDUP_WIN_H__
#define __STRDUP_WIN_H__

/* strdup is deprecated on Windows with VS 2005 and more */
/* required to fix warnings about strdup */

#ifdef _MSC_VER
#include <string.h>
#ifdef strdup
#undef strdup
#endif
#ifdef wstrdup
#undef wstrdup
#endif

#define strdup	strdup_windows
#define wstrdup wstrdup_windows

/**
* Duplicate strings
* @param[in] string source
* @return a pointer to the storage location for the copied string
*/
#ifdef __cplusplus
extern "C"
{
#endif
    char *strdup_windows(const char *strSource);
    wchar_t *wstrdup_windows(const wchar_t *strSource);
#ifdef __cplusplus
};
#endif

#endif

#endif /* __STRDUP_WIN_H__ */
/*--------------------------------------------------------------------------*/