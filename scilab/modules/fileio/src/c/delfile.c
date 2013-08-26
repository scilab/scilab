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
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "delfile.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
void C2F(delfile)(int *fd)
{
    if (*fd >= 0 && *fd < GetMaximumFileOpenedInScilab() )
    {
        SetFileOpenedInScilab(*fd, (FILE*) NULL);
        SetSwapStatus(*fd, 0);
        SetFileTypeOpenedInScilab(*fd, 0);
        SetFileModeOpenedInScilab(*fd, 0);
        FreeFileNameOpenedInScilab(*fd);
    }
}
/*--------------------------------------------------------------------------*/
