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
extern "C"
{
#include "delfile.h"
}
/*--------------------------------------------------------------------------*/
void C2F(delfile)(int *fd)
{
    if (*fd >= 0 && *fd < FileManager::getFileMaxID())
    {
        FileManager::deleteFile(*fd);
    }
}
/*--------------------------------------------------------------------------*/
