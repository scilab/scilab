/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "callfftw.h"
/*-----------------------------------------------------------------------------------*/
#include <dlfcn.h>
/*-----------------------------------------------------------------------------------*/
typedef void (*PROC_FFTW_EXECUTE_SPLIT_DFT) (const fftw_plan p, double *ri, double *ii, double *ro, double *io);
typedef fftw_plan (*PROC_FFTW_PLAN_GURU_SPLIT_DFT) (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags);
typedef void (*PROC_FFTW_DESTROY_PLAN) (fftw_plan p) ;
/*-----------------------------------------------------------------------------------*/
static void *hinstLib = NULL;
static PROC_FFTW_EXECUTE_SPLIT_DFT MY_FFTW_EXECUTE_SPLIT_DFT=NULL;
static PROC_FFTW_PLAN_GURU_SPLIT_DFT MY_FFTW_PLAN_GURU_SPLIT_DFT=NULL;
static PROC_FFTW_DESTROY_PLAN MY_FFTW_DESTROY_PLAN=NULL;
/*-----------------------------------------------------------------------------------*/
BOOL IsLoadedFFTW(void)
{
	BOOL bOK=FALSE;
	if ( (MY_FFTW_EXECUTE_SPLIT_DFT) && (MY_FFTW_PLAN_GURU_SPLIT_DFT) && (MY_FFTW_DESTROY_PLAN) ) bOK=TRUE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL LoadFFTWLibrary(void)
{
	BOOL bOK=FALSE;

	if (hinstLib == NULL)
	{
		hinstLib = dlopen(FFTWLIBNAMELINUX, RTLD_NOW | RTLD_GLOBAL); 
		
		MY_FFTW_EXECUTE_SPLIT_DFT=NULL;
		MY_FFTW_PLAN_GURU_SPLIT_DFT=NULL;
		MY_FFTW_DESTROY_PLAN=NULL;

		MY_FFTW_EXECUTE_SPLIT_DFT = (PROC_FFTW_EXECUTE_SPLIT_DFT) dlsym(hinstLib,"fftw_execute_split_dft");
		MY_FFTW_PLAN_GURU_SPLIT_DFT = (PROC_FFTW_PLAN_GURU_SPLIT_DFT) dlsym(hinstLib,"fftw_plan_guru_split_dft");
		MY_FFTW_DESTROY_PLAN = (PROC_FFTW_DESTROY_PLAN) dlsym(hinstLib,"fftw_destroy_plan");
	}

	if ( MY_FFTW_EXECUTE_SPLIT_DFT && MY_FFTW_PLAN_GURU_SPLIT_DFT && MY_FFTW_DESTROY_PLAN )
	{
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL DisposeFFTWLibrary(void)
{
	BOOL fFreeResult;
	BOOL bOK=FALSE;

	if (hinstLib)
	{
			if (!dlclose(hinstLib)) hinstLib=NULL;
	
	}
	
	if (MY_FFTW_EXECUTE_SPLIT_DFT) MY_FFTW_EXECUTE_SPLIT_DFT=NULL;
	if (MY_FFTW_PLAN_GURU_SPLIT_DFT) MY_FFTW_PLAN_GURU_SPLIT_DFT=NULL;
	if (MY_FFTW_DESTROY_PLAN) MY_FFTW_DESTROY_PLAN=NULL;

	if ( !MY_FFTW_EXECUTE_SPLIT_DFT && !MY_FFTW_PLAN_GURU_SPLIT_DFT && !MY_FFTW_DESTROY_PLAN )
	{
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
void call_fftw_execute_split_dft (const fftw_plan p, double *ri, double *ii, double *ro, double *io) 
{
	if (MY_FFTW_EXECUTE_SPLIT_DFT)
	{
		(MY_FFTW_EXECUTE_SPLIT_DFT)(p,ri,ii,ro,io);
	}
}
/*-----------------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags) 
{
	if (MY_FFTW_PLAN_GURU_SPLIT_DFT)
	{
		return (fftw_plan)(MY_FFTW_PLAN_GURU_SPLIT_DFT)(rank,dims,howmany_rank,howmany_dims,ri,ii,ro,io,flags);
	}
	else return NULL;
}
/*-----------------------------------------------------------------------------------*/
void call_fftw_destroy_plan (fftw_plan p) 
{
	if (MY_FFTW_DESTROY_PLAN)
	{
		(MY_FFTW_DESTROY_PLAN)(p);
	}
}
/*-----------------------------------------------------------------------------------*/
