/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#include <stdio.h>
#include "Scierror.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "scilabWrite.hxx"
#include "lasterror.h"

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#define vsnwprintf _vsnwprintf
#endif
/*--------------------------------------------------------------------------*/
/* Scilab Error at C level */
/*--------------------------------------------------------------------------*/
int  Scierror(int iv, const char *fmt, ...)
{
    int retval = 0;
    char* pstError = NULL;
    char s_buf[bsiz];
    va_list ap;

    va_start(ap, fmt);

#if defined (vsnprintf) || defined (linux)
    retval = vsnprintf(s_buf, bsiz - 1, fmt, ap );
#else
    retval = vsnprintf(s_buf, bsiz - 1, fmt, ap);
#endif
    if (retval < 0)
    {
        s_buf[bsiz - 1] = '\0';
    }

    va_end(ap);

    setLastError(iv, s_buf, 0, NULL);
    return retval;
}
/*--------------------------------------------------------------------------*/
