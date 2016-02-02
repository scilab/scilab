/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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
