
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include "gw_cacsd3.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
extern int C2F(sciarl2)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_arl2(char *fname, unsigned long fname_len)
{
    C2F(sciarl2)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
