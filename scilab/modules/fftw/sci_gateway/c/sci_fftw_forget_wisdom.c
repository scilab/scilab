/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Alan LAYEC
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include "fftw_utilities.h"
#include "callfftw.h"
#include "gw_fftw.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
/* Reset fftw wisdom
*
* Scilab Syntax :
*   -->fftw_forget_wisdom();
*
* Input : Nothing
*
* Output : Nothing
*
*/
/*--------------------------------------------------------------------------*/
int sci_fftw_forget_wisdom(char *fname, void* pvApiCtx)
{
    CheckInputArgument(pvApiCtx, 0, 0);

    FreeFFTWPlan(getSci_Backward_Plan());
    FreeFFTWPlan(getSci_Forward_Plan());

    call_fftw_forget_wisdom();

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
