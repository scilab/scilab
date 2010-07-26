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

#include "os_wfopen.h"
#include "MALLOC.h"
#include "charEncoding.h"

#ifndef _MSC_VER
FILE* linux_fopen(const wchar_t* _pstFilename, const wchar_t* _pstMode)
{
    char* pstFilename   = wide_string_to_UTF8(_pstFilename);
    char* pstMode       = wide_string_to_UTF8(_pstMode);

    FILE* pFile         = fopen(pstFilename, pstMode);
    FREE(pstFilename);
    FREE(pstMode);
    return pFile;
}

#endif
