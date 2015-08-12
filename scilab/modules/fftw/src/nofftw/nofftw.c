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
#include "machine.h" /* C2F */
#include "BOOL.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_fftw.h"
#include "with_fftw.h"
/*--------------------------------------------------------------------------*/
int gw_fftw(void)
{
    Scierror(999, _("Scilab FFTW module not installed.\n"));
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL withfftw(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int C2F(withfftw)(int *rep)
{
    *rep = 0;
    return 0;
}
/*--------------------------------------------------------------------------*/
