/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
