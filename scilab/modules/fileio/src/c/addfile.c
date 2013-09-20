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
#include "addfile.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
void C2F(addfile)(int *fd, FILE *fa, int *swap2, int *type, int *mode, char *filename, int *ierr)
{
    if (SetFileNameOpenedInScilab(*fd, filename) == FALSE )
    {
        *ierr = 1;
        return;
    }
    if (*type == 2)
    {
        SetFileOpenedInScilab(*fd, fa);
    }
    else if (*type == -1)
    {
        SetFileOpenedInScilab(*fd, stdin);
    }
    else if (*type == -2)
    {
        SetFileOpenedInScilab(*fd, stdout);
    }
    else if (*type == -3)
    {
        SetFileOpenedInScilab(*fd, stderr);
    }
    else
    {
        SetFileOpenedInScilab(*fd, (FILE *) 0);
    }

    SetSwapStatus(*fd, *swap2);
    SetFileTypeOpenedInScilab(*fd, *type);
    SetFileModeOpenedInScilab(*fd, *mode);

    *ierr = 0;
}
/*--------------------------------------------------------------------------*/

