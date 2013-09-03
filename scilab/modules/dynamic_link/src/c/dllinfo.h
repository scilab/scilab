/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __DLLINFO_H__
#define __DLLINFO_H__

#include "BOOL.h"
#include "charEncoding.h"

/**
* check if it is a valid dll
* @param[in] dll filename
* @return TRUE or FALSE
*/
BOOL isDll(const char *dllfilename);

/**
*  check if it is a valid x64 dll
* @param[in] dll filename
* @return TRUE or FALSE
*/
BOOL isX64Dll(const char *dllfilename);

/**
*  check if it is a valid x64 dll
* @param[in] dll filename
* @return TRUE or FALSE
*/
BOOL isX86Dll(const char *dllfilename);

/* wide char version */
BOOL isDllW(const wchar_t *dllfilename);
BOOL isX64DllW(const wchar_t *dllfilename);
BOOL isX86DllW(const wchar_t *dllfilename);

#endif
