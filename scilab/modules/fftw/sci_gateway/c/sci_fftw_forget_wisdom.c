/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_fftw_forget_wisdom.h"
#include "fftw_utilities.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
/*--------------------------------------------------------------------------*/
extern FFTW_Plan_struct Sci_Forward_Plan;
extern FFTW_Plan_struct Sci_Backward_Plan;
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
int sci_fftw_forget_wisdom(char *fname,unsigned long fname_len)
{
 CheckRhs(0,0);

 FreeFFTWPlan(&Sci_Backward_Plan);
 FreeFFTWPlan(&Sci_Forward_Plan);

 call_fftw_forget_wisdom();

 PutLhsVar();

 return(0);
}
/*--------------------------------------------------------------------------*/
