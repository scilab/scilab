/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Alan Layec
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __FFTW_UTILITIES__
#define __FFTW_UTILITIES__

#include <string.h>
#include <stdio.h>
#include "fftw3.h"
#include "stack-c.h"


/* definiton of a guru_dim structure type */
typedef struct guru_dim_st
{
  int rank;
  fftw_iodim *dims;
  int howmany_rank;
  fftw_iodim *howmany_dims;
} guru_dim_struct;

/* definiton of a FFTW_Plan structure type */
typedef struct fftw_plan_st
{
  /* stored parmeters of fftw_plan_guru_split_dft function */
  fftw_plan p;
  guru_dim_struct gdim;
  unsigned flags;
} FFTW_Plan_struct;

extern int C2F(dset)(int *n, double *dx, double *dy, int *incy);

/* prototypes of utilities functions */
fftw_plan GetFFTWPlan(guru_dim_struct *gdim,
                      double *ri, double *ii,
                      double *ro, double *io,
                      unsigned flags, int isn);

int FreeFFTWPlan(FFTW_Plan_struct *Sci_Plan);

int CheckGuruDims(guru_dim_struct *gdim1,
                  guru_dim_struct *gdim2);



#endif /* __FFTW_UTILITIES__ */
/*--------------------------------------------------------------------------*/
