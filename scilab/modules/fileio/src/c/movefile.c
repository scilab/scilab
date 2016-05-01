/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "movefile.h"
#include "deleteafile.h"
#include "removedir.h"
#include "copyfile.h"
#include "createdirectory.h"
#include "isdir.h"
/*--------------------------------------------------------------------------*/
int MoveFileFunction(wchar_t *DestinationFilename, wchar_t *SourceFilename)
{
    int ierr = 0;
    ierr = CopyFileFunction(DestinationFilename, SourceFilename);
    if (ierr == 0)
    {
        if (deleteafileW(SourceFilename))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int MoveDirectoryFunction(wchar_t *DestinationDirectory, wchar_t *SourceDirectory)
{
    int ierr = 0;

    if ( !isdirW(DestinationDirectory)  )
    {
        createdirectoryW(DestinationDirectory);
    }

    ierr = CopyDirectoryFunction(DestinationDirectory, SourceDirectory);
    if (ierr == 0)
    {
        if (removedirW(SourceDirectory))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 0;

}
/*--------------------------------------------------------------------------*/
