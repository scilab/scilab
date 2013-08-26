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
#include <stdio.h> /* FILE */
#include "mputstr.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(mputstr) (int *fd, char *str, double *res, int *ierr)
{
    FILE *fa = NULL;
    fa = GetFileOpenedInScilab(*fd);
    if (!fa)
    {
        char *filename = GetFileNameOpenedInScilab(*fd);
        if (filename)
        {
            sciprint(_("%s: Error while opening, reading or writing '%s'.\n"), "mputstr", filename);
        }
        else
        {
            sciprint(_("%s: Error while opening, reading or writing.\n"), "mputstr");
        }
        *ierr = 1;
    }
    else
    {
        fprintf(fa, "%s", str);
        (*res) = (*ierr) = ferror(fa);
    }
}
/*--------------------------------------------------------------------------*/
