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

#include "os_wfopen.h"
#include "sci_malloc.h"
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
