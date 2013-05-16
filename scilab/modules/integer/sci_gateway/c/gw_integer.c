
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_integer.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include "BOOL.h"
extern BOOL BuildWithVS8ExpressF2C(void);
#endif
/*--------------------------------------------------------------------------*/
static int C2F(scivoid)(char *fname, unsigned long fname_len)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int gw_integer(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
