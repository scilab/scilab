/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006/2007 - INRIA - Alan Layec
* Copyright (C) 2008 - INRIA - Allan CORNET
* Copyright (C) 2012 - INRIA - Serge STEER
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
#ifndef __FFTW_UTILITIES__
#define __FFTW_UTILITIES__

#include <string.h>
#include <stdio.h>
#include "fftw3.h"


/* definiton of a guru_dim structure type */
typedef struct guru_dim_st
{
    int rank;
    fftw_iodim *dims;
    int howmany_rank;
    fftw_iodim *howmany_dims;
} guru_dim_struct;

enum Plan_Type
{
    C2C_PLAN = 0,
    R2C_PLAN = 1,
    C2R_PLAN = 2,
    R2R_PLAN = 3
};

/* definition of a FFTW_Plan structure type */
typedef struct fftw_plan_st
{
    /* stored parameters of fftw_plan_guru_split_dft function */
    enum Plan_Type plan_type;
    fftw_plan p;
    guru_dim_struct gdim;
    unsigned flags;
    fftw_r2r_kind *kind;
} FFTW_Plan_struct;



/* prototypes of utilities functions */
fftw_plan GetFFTWPlan(enum Plan_Type type, guru_dim_struct *gdim,
                      double *ri, double *ii,
                      double *ro, double *io,
                      unsigned flags, int isn, fftw_r2r_kind *kind, int *errflag);

int FreeFFTWPlan(FFTW_Plan_struct *Sci_Plan);

int CheckGuruDims(guru_dim_struct *gdim1, guru_dim_struct *gdim2);
int CheckKindArray(fftw_r2r_kind *kind1, fftw_r2r_kind *kind2, int rank);

void ExecuteFFTWPlan(enum Plan_Type type, const fftw_plan p, double *ri, double *ii, double *ro, double *io);

int is_real(double *Ar, double *Ai, int ndims, int *dims);
int check_array_symmetry(double *Ar, double *Ai, guru_dim_struct gdim);
int complete_array(double *Ar, double *Ai, guru_dim_struct gdim);
int dct_scale_array(double *Ar, double *Ai, guru_dim_struct gdim, int isn);
int dst_scale_array(double *Ar, double *Ai, guru_dim_struct gdim, int isn);

unsigned int getCurrentFftwFlags(void);
void setCurrentFftwFlags(unsigned int newFftwFlags);

FFTW_Plan_struct *getSci_Backward_Plan(void);
FFTW_Plan_struct *getSci_Forward_Plan(void);

#endif /* __FFTW_UTILITIES__ */
/*--------------------------------------------------------------------------*/
