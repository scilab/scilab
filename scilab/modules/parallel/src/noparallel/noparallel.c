/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "noparallel.h"
#include "concurrency.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int gw_parallel(void)
{
    Scierror(999, _("%s: %s module is not installed.\n"), "Scilab", "parallel");
    return 0;
}
/*--------------------------------------------------------------------------*/
int parallelConcurrency(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int parallelForbidden(char const* fname)
{
    /* never called then do nothing */
    return 0;
}
/*--------------------------------------------------------------------------*/

