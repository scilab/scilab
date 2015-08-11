/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OS_WFOPEN_H__
#define __OS_WFOPEN_H__

#include <wchar.h>
#include <stdio.h>

// Windows
#ifdef _MSC_VER
#define os_wfopen       _wfopen
#endif

// Linux
#ifdef __linux__
#define os_wfopen       linux_fopen
FILE* linux_fopen(const wchar_t* _pstFilename, const wchar_t* _pstMode);
#endif

// MacOS X
#ifdef __APPLE__
#define os_wfopen        linux_fopen
FILE* linux_fopen(const wchar_t* _pstFilename, const wchar_t* _pstMode);
#endif

#endif /* !__OS_WFOPEN_H__ */
