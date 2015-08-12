/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Alan LAYEC
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "fftw_utilities.h"
#include "callfftw.h"
#include "gw_fftw.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
/* Reset fftw wisdom
*
* Scilab Calling sequence :
*   -->fftw_forget_wisdom();
*
* Input : Nothing
*
* Output : Nothing
*
*/
/*--------------------------------------------------------------------------*/
int sci_fftw_forget_wisdom(char *fname, unsigned long fname_len)
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
