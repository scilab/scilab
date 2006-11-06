/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* ALAN LAYEC initial revision  : 30/09/06 fftw3 toolbox*/
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __FFTW_UTILITIES__
#define __FFTW_UTILITIES__

#include <string.h>
#include <stdio.h>
#include "fftw3.h"
#include "stack-c.h"


/* definiton of a static FFTW_Plan structure type */
typedef struct fftw_plan_st
{
	/* stored parmeters of fftw_plan_guru_split_dft function */
	fftw_plan *p;
	int *rank;
	fftw_iodim **dims;
	int *howmany_rank;
	fftw_iodim **howmany_dims;
	int *isn;
	unsigned *sflags;

	/* specific FFTW parameters */
	double *io[4];  /* to store input/ouput address when pushing planners */
	unsigned flags; /* to set the "flags" parameters when creating planners */
	int in;         /* to set if it is an in or out place transform */
	int norm;       /* to set if we compute normalized transform */

	int nb_CPlan;   /* Current index in FFTW_Plan_struct */
} FFTW_Plan_struct;

extern int C2F(dset)();

/* prototypes of utilities functions */
fftw_plan GetFFTWPlan(int rank, const fftw_iodim *dims,
					  int howmany_rank, const fftw_iodim *howmany_dims,
					  double *ri, double *ii, double *ro, double *io,
					  unsigned flags, int isn);

int UpdateFTTWPlan(fftw_plan p, int rank, const fftw_iodim *dims,
				   int howmany_rank, const fftw_iodim *howmany_dims,
				   int isn);

int PushFTTWPlan(void);
int FreeFTTWPlan(void);


#endif /* __FFTW_UTILITIES__ */
/*-----------------------------------------------------------------------------------*/
