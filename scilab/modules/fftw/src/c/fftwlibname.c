/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <string.h>
#include "stdlib.h"
#include "fftwlibname.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static char *fftwlibname = NULL;
/*--------------------------------------------------------------------------*/
void setfftwlibname(char *libname)
{
    if (libname)
    {
        if (fftwlibname)
        {
            FREE(fftwlibname);
            fftwlibname = NULL;
        }
        fftwlibname = os_strdup(libname);
    }
}
/*--------------------------------------------------------------------------*/
char *getfftwlibname(void)
{
    char *name = NULL;

    if (fftwlibname)
    {
        name = os_strdup(fftwlibname);
    }

    return name;
}
/*--------------------------------------------------------------------------*/
void freefftwlibname(void)
{
    if (fftwlibname)
    {
        FREE(fftwlibname);
        fftwlibname = NULL;
    }
}
/*--------------------------------------------------------------------------*/
