/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "BOOL.h"
#include "gw_helptools.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;

/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{NULL, ""} // buildDocv2
};

/*--------------------------------------------------------------------------*/
int gw_helptools(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
