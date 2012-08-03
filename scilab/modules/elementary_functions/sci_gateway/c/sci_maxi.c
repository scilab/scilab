/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include "api_scilab.h"
#include "gw_elementary_functions.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intmaxi) (char *fname, int *id, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_maxi(char *fname, unsigned long fname_len)
{
    Rhs = Max(Rhs, 0);
    if (Rhs == 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s).\n"), fname);
    }
    else
    {
        static int id[6];

        C2F(intmaxi) (fname, id, fname_len);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
