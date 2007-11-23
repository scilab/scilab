/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Alan Layec :  - initial revision  : 30/09/06 fftw3 toolbox*/
/*               - revision : 18/04/07 */
/* Allan CORNET scilab module */
/*--------------------------------------------------------------------------*/ 
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

extern int C2F(dset)();

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
