/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "BOOL.h"
#include "api_scilab.h"
#include "sci_gateway.h"
/*-----------------------------------------------------------------------------------*/
static int callExternalFunction(char *fname, GatefuncS F, BOOL withPutLhsVar);
/*-----------------------------------------------------------------------------------*/
/** generic scilab interface **/
int sci_gateway(char *fname, GatefuncS F)
{
    return callExternalFunction(fname, F, TRUE);
}
/*-----------------------------------------------------------------------------------*/
int sci_gateway_without_putlhsvar(char *fname, GatefuncS F)
{
    return callExternalFunction(fname, F, FALSE);
}
/*-----------------------------------------------------------------------------------*/
static int callExternalFunction(char *fname, GatefuncS F, BOOL withPutLhsVar)
{
    (*F)(fname, (int)strlen(fname));
    if (withPutLhsVar)
    {
        ReturnArguments(NULL);
    }
    return 0;
}
/*-----------------------------------------------------------------------------------*/
