/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "fftw_utilities.h"
#include "MALLOC.h"
#include "callfftw.h"
/*--------------------------------------------------------------------------*/ 
/* definition of structures to store parameters
 * of FFTW planners - set here default value -
 */
FFTW_Plan_struct Sci_Forward_Plan =
{
  NULL,            /* fftw_plan p              */
  {0,NULL,0,NULL}, /* guru_dim_struct gdim     */
  FFTW_ESTIMATE    /* unsigned flags           */
};

FFTW_Plan_struct Sci_Backward_Plan =
{
  NULL,             /* fftw_plan p              */
  {0,NULL,0,NULL},  /* guru_dim_struct gdim     */
  FFTW_ESTIMATE     /* unsigned flags           */
};

unsigned cur_fftw_flags=FFTW_ESTIMATE;
/*--------------------------------------------------------------------------*/
/* Free a FFTW_Plan_struct
 *
 * Input : FFTW_Plan_struct *Sci_Plan
 *
 * Output : int, return 1.
 *
 */
int FreeFFTWPlan(FFTW_Plan_struct *Sci_Plan)
{
  if(Sci_Plan->p != NULL) {
    call_fftw_destroy_plan(Sci_Plan->p);
    Sci_Plan->p = NULL;
  }
  if(Sci_Plan->gdim.rank != 0) {
    Sci_Plan->gdim.rank = 0;
    FREE(Sci_Plan->gdim.dims);
    Sci_Plan->gdim.dims = NULL;
  }
  if(Sci_Plan->gdim.howmany_rank != 0) {
    Sci_Plan->gdim.howmany_rank = 0;
    FREE(Sci_Plan->gdim.howmany_dims);
    Sci_Plan->gdim.howmany_dims = NULL;
  }
  return(1);
}
/*--------------------------------------------------------------------------*/
/* Return a valid plan ptr.
 * This function search in the Sci_xx_Plan structures if
 * the given input parameters follows an already stored
 * set of parameters.
 * If found then return an already stored plan ptr.
 * If not found, then returns 
 * a new set of parameters (and a new plan)
 * with fftw_plan_guru_split_dft
 * and store it in Sci_xx_Plan structures
 *
 * Input : guru_dim_struct *gdim
 *         double *ri, double *ii
 *         double *ro, double *io
 *         unsigned flags, int isn
 *
 * Output : fftw_plan
 *
 *
 */
fftw_plan GetFFTWPlan(guru_dim_struct *gdim,
                      double *ri, double *ii,
                      double *ro, double *io,
                      unsigned flags, int isn)
{
  FFTW_Plan_struct *Sci_Plan;
  int i;

  if (isn==-1) Sci_Plan = &Sci_Backward_Plan;
  else Sci_Plan = &Sci_Forward_Plan;

  if ( (!(CheckGuruDims(&(Sci_Plan->gdim), gdim))) ||
       (Sci_Plan->flags != cur_fftw_flags) ) {

    FreeFFTWPlan(Sci_Plan);

    if (gdim->rank != 0) {
      Sci_Plan->gdim.rank = gdim->rank;
      if ((Sci_Plan->gdim.dims = \
              (fftw_iodim *)MALLOC(sizeof(fftw_iodim)*gdim->rank))==NULL) {
       return(NULL);
      }
      for (i=0;i<gdim->rank;i++) {
        Sci_Plan->gdim.dims[i].n  = gdim->dims[i].n;
        Sci_Plan->gdim.dims[i].is = gdim->dims[i].is;
        Sci_Plan->gdim.dims[i].os = gdim->dims[i].os;
      }
    }

    if (gdim->howmany_rank != 0) {
      Sci_Plan->gdim.howmany_rank = gdim->howmany_rank;
      if ((Sci_Plan->gdim.howmany_dims = \
             (fftw_iodim *)MALLOC(sizeof(fftw_iodim)*gdim->howmany_rank))==NULL) {
       FREE(Sci_Plan->gdim.dims);
       return(NULL);
      }
      for (i=0;i<gdim->howmany_rank;i++) {
        Sci_Plan->gdim.howmany_dims[i].n  = gdim->howmany_dims[i].n;
        Sci_Plan->gdim.howmany_dims[i].is = gdim->howmany_dims[i].is;
        Sci_Plan->gdim.howmany_dims[i].os = gdim->howmany_dims[i].os;
      }
    }

    Sci_Plan->flags = cur_fftw_flags;

    Sci_Plan->p = call_fftw_plan_guru_split_dft(Sci_Plan->gdim.rank,
                                                Sci_Plan->gdim.dims,
                                                Sci_Plan->gdim.howmany_rank,
                                                Sci_Plan->gdim.howmany_dims,
                                                ri, ii, ro, io,
                                                Sci_Plan->flags);
  }
  return(Sci_Plan->p);
}
/*--------------------------------------------------------------------------*/
/* Check if two guru_dim structures are equal
 *
 * Input : guru_dim_struct *gdim1
 *         guru_dim_struct *gdim2
 *
 * Output : int, return 0 if False, else 1.
 *
 */
int CheckGuruDims(guru_dim_struct *gdim1, guru_dim_struct *gdim2)
{
  int i;

  if( (gdim1->rank==gdim2->rank) &&
      (gdim1->howmany_rank==gdim2->howmany_rank)) {
    for(i=0;i<gdim1->rank;i++) {
      if (gdim1->dims[i].n  != gdim2->dims[i].n)  return(0);
      if (gdim1->dims[i].is != gdim2->dims[i].is) return(0);
      if (gdim1->dims[i].os != gdim2->dims[i].os) return(0);
    }
    for(i=0;i<gdim1->howmany_rank;i++) {
      if (gdim1->howmany_dims[i].n  != gdim2->howmany_dims[i].n)  return(0);
      if (gdim1->howmany_dims[i].is != gdim2->howmany_dims[i].is) return(0);
      if (gdim1->howmany_dims[i].os != gdim2->howmany_dims[i].os) return(0);
    }
    return(1);
  }
  else return(0);
}
/*--------------------------------------------------------------------------*/
