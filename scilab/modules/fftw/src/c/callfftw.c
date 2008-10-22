/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "callfftw.h"
/*--------------------------------------------------------------------------*/
#include "dynamiclibrary.h"
/*--------------------------------------------------------------------------*/
typedef void (*PROC_FFTW_EXECUTE_SPLIT_DFT) (const fftw_plan p, double *ri, double *ii, double *ro, double *io);
typedef fftw_plan (*PROC_FFTW_PLAN_GURU_SPLIT_DFT) (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags);
typedef void (*PROC_FFTW_DESTROY_PLAN) (fftw_plan p);

typedef char *(*PROC_FFTW_EXPORT_WISDOM_TO_STRING) (void);
typedef int (*PROC_FFTW_IMPORT_WISDOM_FROM_STRING) (const char *input_string);
typedef void (*PROC_FFTW_FORGET_WISDOM) (void);
/*--------------------------------------------------------------------------*/
static DynLibHandle hinstLib = NULL; 
static PROC_FFTW_EXECUTE_SPLIT_DFT MY_FFTW_EXECUTE_SPLIT_DFT=NULL;
static PROC_FFTW_PLAN_GURU_SPLIT_DFT MY_FFTW_PLAN_GURU_SPLIT_DFT=NULL;
static PROC_FFTW_DESTROY_PLAN MY_FFTW_DESTROY_PLAN=NULL;

static PROC_FFTW_EXPORT_WISDOM_TO_STRING MY_FFTW_EXPORT_WISDOM_TO_STRING=NULL;
static PROC_FFTW_IMPORT_WISDOM_FROM_STRING MY_FFTW_IMPORT_WISDOM_FROM_STRING=NULL;
static PROC_FFTW_FORGET_WISDOM MY_FFTW_FORGET_WISDOM=NULL;
/*--------------------------------------------------------------------------*/
BOOL IsLoadedFFTW(void)
{
	BOOL bOK=FALSE;
	if ( (MY_FFTW_EXECUTE_SPLIT_DFT) && (MY_FFTW_PLAN_GURU_SPLIT_DFT) && (MY_FFTW_DESTROY_PLAN) &&\
             (MY_FFTW_EXPORT_WISDOM_TO_STRING) && (MY_FFTW_IMPORT_WISDOM_FROM_STRING) &&\
             (MY_FFTW_FORGET_WISDOM) ) bOK=TRUE;
	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL LoadFFTWLibrary(char *libraryname)
{
	BOOL bOK=FALSE;

	if (libraryname == NULL) return bOK;

	if (hinstLib == NULL)
	{
		hinstLib = LoadDynLibrary(libraryname);
		MY_FFTW_EXECUTE_SPLIT_DFT=NULL;
		MY_FFTW_PLAN_GURU_SPLIT_DFT=NULL;
		MY_FFTW_DESTROY_PLAN=NULL;

        MY_FFTW_EXPORT_WISDOM_TO_STRING=NULL;
        MY_FFTW_IMPORT_WISDOM_FROM_STRING=NULL;
        MY_FFTW_FORGET_WISDOM=NULL;

		MY_FFTW_EXECUTE_SPLIT_DFT = (PROC_FFTW_EXECUTE_SPLIT_DFT) GetDynLibFuncPtr(hinstLib,"fftw_execute_split_dft");
		MY_FFTW_PLAN_GURU_SPLIT_DFT = (PROC_FFTW_PLAN_GURU_SPLIT_DFT) GetDynLibFuncPtr(hinstLib,"fftw_plan_guru_split_dft");
		MY_FFTW_DESTROY_PLAN = (PROC_FFTW_DESTROY_PLAN) GetDynLibFuncPtr(hinstLib,"fftw_destroy_plan");

        MY_FFTW_EXPORT_WISDOM_TO_STRING = (PROC_FFTW_EXPORT_WISDOM_TO_STRING) GetDynLibFuncPtr(hinstLib,"fftw_export_wisdom_to_string");
        MY_FFTW_IMPORT_WISDOM_FROM_STRING = (PROC_FFTW_IMPORT_WISDOM_FROM_STRING) GetDynLibFuncPtr(hinstLib, "fftw_import_wisdom_from_string");
        MY_FFTW_FORGET_WISDOM = (PROC_FFTW_FORGET_WISDOM) GetDynLibFuncPtr(hinstLib,"fftw_forget_wisdom");
	}

	if ( MY_FFTW_EXECUTE_SPLIT_DFT && MY_FFTW_PLAN_GURU_SPLIT_DFT && MY_FFTW_DESTROY_PLAN &&\
             MY_FFTW_EXPORT_WISDOM_TO_STRING && MY_FFTW_IMPORT_WISDOM_FROM_STRING &&\
             MY_FFTW_FORGET_WISDOM )
	{
		bOK=TRUE;
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL DisposeFFTWLibrary(void)
{
	BOOL fFreeResult;
	BOOL bOK=FALSE;

	if (hinstLib)
	{
		fFreeResult = FreeDynLibrary(hinstLib); 
		hinstLib=NULL;
	}
	
	if (MY_FFTW_EXECUTE_SPLIT_DFT) MY_FFTW_EXECUTE_SPLIT_DFT=NULL;
	if (MY_FFTW_PLAN_GURU_SPLIT_DFT) MY_FFTW_PLAN_GURU_SPLIT_DFT=NULL;
	if (MY_FFTW_DESTROY_PLAN) MY_FFTW_DESTROY_PLAN=NULL;

        if (MY_FFTW_EXPORT_WISDOM_TO_STRING) MY_FFTW_EXPORT_WISDOM_TO_STRING=NULL;
        if (MY_FFTW_IMPORT_WISDOM_FROM_STRING) MY_FFTW_IMPORT_WISDOM_FROM_STRING=NULL;
        if (MY_FFTW_FORGET_WISDOM) MY_FFTW_FORGET_WISDOM=NULL;

	if ( !MY_FFTW_EXECUTE_SPLIT_DFT && !MY_FFTW_PLAN_GURU_SPLIT_DFT && !MY_FFTW_DESTROY_PLAN &&\
             !MY_FFTW_EXPORT_WISDOM_TO_STRING && !MY_FFTW_IMPORT_WISDOM_FROM_STRING &&\
             !MY_FFTW_FORGET_WISDOM )
	{
		bOK=TRUE;
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft (const fftw_plan p, double *ri, double *ii, double *ro, double *io) 
{
	if (MY_FFTW_EXECUTE_SPLIT_DFT)
	{
		(MY_FFTW_EXECUTE_SPLIT_DFT)(p,ri,ii,ro,io);
	}
}
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags) 
{
	if (MY_FFTW_PLAN_GURU_SPLIT_DFT)
	{
		return (fftw_plan)(MY_FFTW_PLAN_GURU_SPLIT_DFT)(rank,dims,howmany_rank,howmany_dims,ri,ii,ro,io,flags);
	}
	else return NULL;
}
/*--------------------------------------------------------------------------*/
void call_fftw_destroy_plan (fftw_plan p) 
{
	if (MY_FFTW_DESTROY_PLAN)
	{
		(MY_FFTW_DESTROY_PLAN)(p);
	}
}
/*--------------------------------------------------------------------------*/
char *call_fftw_export_wisdom_to_string (void)
{
	if (MY_FFTW_EXPORT_WISDOM_TO_STRING)
	{
		return (char *)(MY_FFTW_EXPORT_WISDOM_TO_STRING)();
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
int call_fftw_import_wisdom_from_string (const char *input_string)
{
	if (MY_FFTW_IMPORT_WISDOM_FROM_STRING)
	{
		return (int)(MY_FFTW_IMPORT_WISDOM_FROM_STRING)(input_string);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
void call_fftw_forget_wisdom (void)
{
	if (MY_FFTW_FORGET_WISDOM)
	{
		(MY_FFTW_FORGET_WISDOM)();
	}
}
/*--------------------------------------------------------------------------*/
