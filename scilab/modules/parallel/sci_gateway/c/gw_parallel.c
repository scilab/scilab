/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include <string.h>
#include "api_scilab.h"
#include "gw_parallel.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"

/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {NULL, ""},                 // "parallel_run"
    {NULL, ""}                  // "parallel_concurrency"
};

/*--------------------------------------------------------------------------*/
int gw_parallel(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
