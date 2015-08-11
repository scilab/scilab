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

extern "C"{
#include "getfiletype.h"
}
/*--------------------------------------------------------------------------*/
void C2F(getfiletype)(int *fd, int *type, int *ierr)
{
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

    *type = pFile->getFileType();
    *ierr = 0;
}
/*--------------------------------------------------------------------------*/
