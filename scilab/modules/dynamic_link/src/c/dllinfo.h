/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Allan CORNET
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
#ifndef __DLLINFO_H__
#define __DLLINFO_H__

#include "BOOL.h"
#include "charEncoding.h"

/**
* check if it is a valid dll
* @param[in] dll filename
* @return TRUE or FALSE
*/
BOOL isDll(const char* _pstDLLName);

/**
*  check if it is a valid x64 dll
* @param[in] dll filename
* @return TRUE or FALSE
*/
BOOL isX64Dll(const char* _pstDLLName);

/**
*  check if it is a valid x64 dll
* @param[in] dll filename
* @return TRUE or FALSE
*/
BOOL isX86Dll(const char* _pstDLLName);

/* wide char version */
BOOL isDllW(const wchar_t* _pwstDLLName);
BOOL isX64DllW(const wchar_t* _pwstDLLName);
BOOL isX86DllW(const wchar_t* _pwstDLLName);

#endif
