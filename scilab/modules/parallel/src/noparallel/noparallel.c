/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    Scierror(999,_("%s: %s module is not installed.\n"), "Scilab", "parallel");
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

