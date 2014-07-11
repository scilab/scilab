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
#include "gw_data_structures1.h"
#include <api_scilab.h>
/*--------------------------------------------------------------------------*/
extern int C2F(rattyp)(); /* fortran */
extern int C2F(mtlist)(); /* fortran */
/*--------------------------------------------------------------------------*/
int sci_rlist(char *fname, unsigned long fname_len)
{

    CheckInputArgument(pvApiCtx, 0, 3);

    if (nbInputArgument(pvApiCtx) == 2)
    {
        int lw = 0;
        C2F(overload)(&lw, fname, fname_len);
        return 0;
    }
    else
    {
        C2F(mtlist)(C2F(rattyp));
        return 0;
    }


}
/*--------------------------------------------------------------------------*/
