/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#include "machine.h"
#include "gw_polynomials.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intpdiv)(int *id);
/*--------------------------------------------------------------------------*/
int sci_pppdiv(char *fname, unsigned long fname_len)
{
    static int id[6];
    C2F(intpdiv)(id);
    return 0;
}
/*--------------------------------------------------------------------------*/
