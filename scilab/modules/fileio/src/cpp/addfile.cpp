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
#include "filemanager.hxx"
#include "file.hxx"

extern "C"
{
#include "addfile.h"
#include "filesmanagement.h"
#include "charEncoding.h"
#include "MALLOC.h"
}

/*--------------------------------------------------------------------------*/
void C2F(addfile)(int *fd, FILE *fa, int *swap2, int *type, int *mode, char *filename, int *ierr)
{
    wchar_t* wcsFilename = to_wide_string(filename);
    types::File* pFile = new types::File();

    pFile->setFilename(std::wstring(wcsFilename));

    if (*type == 2)
    {
        pFile->setFileDesc(fa);
        pFile->setFileModeAsInt(*mode);
    }
    else if (*type == -1)
    {
        pFile->setFileDesc(stdin);
    }
    else if (*type == -2)
    {
        pFile->setFileDesc(stdout);
    }
    else if (*type == -3)
    {
        pFile->setFileDesc(stderr);
    }
    else
    {
        pFile->setFileDesc((FILE*)0);
        pFile->setFileFortranMode(*mode);
    }

    pFile->setFileSwap(*swap2);
    pFile->setFileType(*type);

    FileManager::addFile(pFile);

    *ierr = 0;

    FREE(wcsFilename);
}
/*--------------------------------------------------------------------------*/

