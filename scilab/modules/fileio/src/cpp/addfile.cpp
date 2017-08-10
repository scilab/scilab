/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include "filemanager.hxx"
#include "file.hxx"

extern "C"
{
#include "addfile.h"
#include "charEncoding.h"
#include "sci_malloc.h"
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

