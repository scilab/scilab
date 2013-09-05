/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h> /* strlen */
#include "filemanager.hxx"
#include "file.hxx"

extern "C"
{
#include "getfileinfo.h"
#include "charEncoding.h"
#include "MALLOC.h"
}
/*--------------------------------------------------------------------------*/
void C2F(getfileinfo)(int *fd, FILE *fa, int *swap2, int *type, int *mode, char *filename, int *lf, int *ierr)
{
    const wchar_t *filenamefromfd = NULL;
    if (*fd < 0)
    {
        *ierr = 1;
        return;
    }

    types::File* pFile = FileManager::getFile(*fd);

    if (*fd >= FileManager::getFileMaxID() || pFile == NULL || pFile->getFileType() == 0)
    {
        *ierr = 2;
        return;
    }

    *swap2 = pFile->getFileSwap();
    *type = pFile->getFileType();
    *mode = pFile->getFileModeAsInt();
    filenamefromfd = pFile->getFilename().c_str();
    if (filenamefromfd)
    {
        char* pstFileName = wide_string_to_UTF8(filenamefromfd);
        strcpy(filename, pstFileName);
        FREE(pstFileName);
    }
    else
    {
        strcpy(filename, "");
    }

    *lf = (int)strlen(filename);
    *ierr = 0;
}
/*--------------------------------------------------------------------------*/
