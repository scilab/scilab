/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __LOCALENAMETOLCID_WINDOWS_H__
#define __LOCALENAMETOLCID_WINDOWS_H__

#ifdef _MSC_VER

#include <Windows.h>

/* loaded from system LocaleNameToLCID function */
/* Some Windows does not have this function */
/* Windows must support multilanguage */
/* by default LCID returns 0*/

LCID dllLocaleNameToLCID(LPCWSTR lpName, DWORD dwFlags);

#endif

#endif /* __LOCALENAMETOLCID_WINDOWS_H__ */
/*--------------------------------------------------------------------------*/
