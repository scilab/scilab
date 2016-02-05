/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#ifndef __PATHCONVERT_H__
#define __PATHCONVERT_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_fileio.h"

typedef enum
{
    WINDOWS_STYLE = 0,
    UNIX_STYLE = 1,
    AUTO_STYLE = 2
} PathConvertType;

FILEIO_IMPEXP wchar_t *pathconvertW(wchar_t* wcpath, BOOL flagtrail, BOOL flagexpand, PathConvertType PType);

FILEIO_IMPEXP char *pathconvert(char* path, BOOL flagtrail, BOOL flagexpand, PathConvertType PType);

#endif /* PATHCONVERT_H__ */
/*--------------------------------------------------------------------------*/
