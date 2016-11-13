/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
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
#include "deleteafile.h"
#ifndef _MSC_VER
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <Windows.h>
#endif
#include "charEncoding.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
BOOL deleteafile(const char *filename)
{
#ifndef _MSC_VER
    {
        FILE *f = fopen(filename, "r") ;
        if (! f)
        {
            return FALSE;
        }

        fclose(f) ;

        if (chmod(filename, S_IWRITE))
        {
            return FALSE;
        }

        if (remove(filename) == 0)
        {
            return TRUE;
        }

        return FALSE;
    }
#else
    {
        BOOL bOK = FALSE;
        if (filename)
        {
            bOK = DeleteFileA(filename);
        }

        return bOK;
    }
#endif
}
/*--------------------------------------------------------------------------*/
