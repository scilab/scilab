/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h> /* strlen */
#include "getfileinfo.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
void C2F(getfileinfo)(int *fd, FILE *fa, int *swap2, int *type, int *mode, char *filename, int *lf, int *ierr)
{
    char *filenamefromfd = NULL;
    if (*fd < 0 || *fd >= GetMaximumFileOpenedInScilab() )
    {
        *ierr = 1;
        return;
    }
    if ( GetFileTypeOpenedInScilab(*fd) == 0 )
    {
        *ierr = 2;
        return;
    }

    *swap2 = GetSwapStatus(*fd);
    *type = GetFileTypeOpenedInScilab(*fd);
    *mode = GetFileModeOpenedInScilab(*fd);
    filenamefromfd = GetFileNameOpenedInScilab(*fd);
    if (filenamefromfd)
    {
        strcpy(filename, GetFileNameOpenedInScilab(*fd));
    }
    else
    {
        strcpy(filename, "");
    }

    *lf = (int)strlen(filename);
    *ierr = 0;
}
/*--------------------------------------------------------------------------*/
