/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) ENPC
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include <string.h>
#include "cluni0.h"
#include "expandPathVariable.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout)
{
    char *expandedVar = NULL;
    in_name[lin] = 0;

    expandedVar = expandPathVariable(in_name);
    if (expandedVar)
    {
        strcpy(out_name, expandedVar);
        FREE(expandedVar);
        expandedVar = NULL;
        *out_n = (int) strlen(out_name);
    }
    else
    {
        strcpy(out_name, in_name);
        *out_n = (int) strlen(out_name);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
