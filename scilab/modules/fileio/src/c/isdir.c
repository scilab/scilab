/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif
#include <string.h>
#include "isDrive.h"
#include "isdir.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
BOOL isdir(const char * path)
{
    BOOL bOK = FALSE;
#ifndef _MSC_VER
    struct stat buf;
    if (path == NULL)
    {
        return FALSE;
    }
    if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
    {
        bOK = TRUE;
    }
#else
    if (isDrive(path))
    {
        return TRUE;
    }
    else
    {
        DWORD attr = 0;
        char* tmpPath = os_strdup(path);

        int len = (int)strlen(tmpPath);
        if ((tmpPath[len - 1] == '\\') || (tmpPath[len - 1] == '/'))
        {
            tmpPath[len - 1] = '\0';
        }

        attr = GetFileAttributesA(tmpPath);
        FREE(tmpPath);
        if (attr == INVALID_FILE_ATTRIBUTES)
        {
            return FALSE;
        }
        return ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0) ? TRUE : FALSE;
    }
#endif
    return bOK;
}
/*--------------------------------------------------------------------------*/
