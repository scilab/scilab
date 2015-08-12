/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
/*--------------------------------------------------------------------------*/
int C2F(intisequalvar)(char * fname, int *job, long int fl); /* the gateway */
/*--------------------------------------------------------------------------*/
int sci_isequalbitwise(char *fname, unsigned long fname_len)
{
    int job = 0;
    C2F(intisequalvar)(fname, (job = 0, &job), (unsigned long)strlen(fname) );
    return 0;
}
/*--------------------------------------------------------------------------*/
