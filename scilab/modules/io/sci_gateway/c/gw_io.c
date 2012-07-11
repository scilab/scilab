/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_io.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsave)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{

{NULL, ""}, //setenv
{NULL,""}, //read
{NULL, ""}, //getenv
{NULL,""}, //getio
{NULL,""},
{NULL,""},
{NULL,""}, //write
{NULL,""}, //rat
{NULL, ""}, //file
{NULL ,""}, //host
{NULL, ""}, //unix
{NULL,""}, //readb
{NULL,""}, //writb
{NULL,""}, //getpid
{NULL,""}, //read4b
{NULL,""}, //write4b
{NULL,""}, //save
{NULL, ""}, //load
{NULL, ""} //%_load
};
/*--------------------------------------------------------------------------*/
int gw_io(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
