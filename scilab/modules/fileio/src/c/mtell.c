/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "mtell.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(mtell) (int *fd, double *offset, int *err)
{
    FILE *fa = GetFileOpenedInScilab(*fd);
    if ( fa == (FILE *) 0 )
    {
        char *filename = GetFileNameOpenedInScilab(*fd);
        if (filename)
        {
            sciprint(_("%s: Error while opening, reading or writing '%s'.\n"), "mtell", filename);
        }
        else
        {
            sciprint(_("%s: Error while opening, reading or writing.\n"), "mtell");
        }

        *err = 1;
        return;
    }
    *err = 0;
#ifdef _MSC_VER
#if _WIN64
    *offset = (double) _ftelli64(fa) ;
#else
    *offset = (double) ftell(fa) ;
#endif
#else
    *offset = (double) ftell(fa) ;
#endif
}
/*--------------------------------------------------------------------------*/
