/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
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
#include "callfftw.h"
/*--------------------------------------------------------------------------*/
#include "dynamiclibrary.h"
#include "getshortpathname.h"
#include "sci_malloc.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
typedef void (*PROC_FFTW_EXECUTE_SPLIT_DFT)     (const fftw_plan p, double *ri, double *ii, double *ro, double *io);
typedef void (*PROC_FFTW_EXECUTE_SPLIT_DFT_C2R) (const fftw_plan p, double *ri, double *ii, double *ro);
typedef void (*PROC_FFTW_EXECUTE_SPLIT_DFT_R2C) (const fftw_plan p, double *ri,             double *ro, double *io);
typedef void (*PROC_FFTW_EXECUTE_SPLIT_DFT_R2R) (const fftw_plan p, double *ri,             double *ro);
typedef fftw_plan (*PROC_FFTW_PLAN_GURU_SPLIT_DFT) (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags);
typedef fftw_plan (*PROC_FFTW_PLAN_GURU_SPLIT_DFT_C2R) (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, unsigned flags);
typedef fftw_plan (*PROC_FFTW_PLAN_GURU_SPLIT_DFT_R2C) (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ro, double *io, unsigned flags);
typedef fftw_plan (*PROC_FFTW_PLAN_GURU_SPLIT_DFT_R2R) (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ro, fftw_r2r_kind *kind, unsigned flags);
typedef void (*PROC_FFTW_DESTROY_PLAN) (fftw_plan p);
typedef char *(*PROC_FFTW_EXPORT_WISDOM_TO_STRING) (void);
typedef int (*PROC_FFTW_IMPORT_WISDOM_FROM_STRING) (const char *input_string);
typedef void (*PROC_FFTW_FORGET_WISDOM) (void);
/*--------------------------------------------------------------------------*/
static DynLibHandle hinstLib = NULL;
static PROC_FFTW_EXECUTE_SPLIT_DFT       MY_FFTW_EXECUTE_SPLIT_DFT       = NULL;
static PROC_FFTW_EXECUTE_SPLIT_DFT_C2R   MY_FFTW_EXECUTE_SPLIT_DFT_C2R   = NULL;
static PROC_FFTW_EXECUTE_SPLIT_DFT_R2C   MY_FFTW_EXECUTE_SPLIT_DFT_R2C   = NULL;
static PROC_FFTW_EXECUTE_SPLIT_DFT_R2R   MY_FFTW_EXECUTE_SPLIT_DFT_R2R   = NULL;
static PROC_FFTW_PLAN_GURU_SPLIT_DFT     MY_FFTW_PLAN_GURU_SPLIT_DFT     = NULL;
static PROC_FFTW_PLAN_GURU_SPLIT_DFT_C2R MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R = NULL;
static PROC_FFTW_PLAN_GURU_SPLIT_DFT_R2C MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C = NULL;
static PROC_FFTW_PLAN_GURU_SPLIT_DFT_R2R MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R = NULL;
static PROC_FFTW_DESTROY_PLAN            MY_FFTW_DESTROY_PLAN            = NULL;
static PROC_FFTW_EXPORT_WISDOM_TO_STRING MY_FFTW_EXPORT_WISDOM_TO_STRING = NULL;
static PROC_FFTW_IMPORT_WISDOM_FROM_STRING MY_FFTW_IMPORT_WISDOM_FROM_STRING = NULL;
static PROC_FFTW_FORGET_WISDOM MY_FFTW_FORGET_WISDOM = NULL;
/*--------------------------------------------------------------------------*/
BOOL IsLoadedFFTW(void)
{
    if ( (MY_FFTW_EXECUTE_SPLIT_DFT) && (MY_FFTW_PLAN_GURU_SPLIT_DFT) && (MY_FFTW_DESTROY_PLAN) &&
            (MY_FFTW_EXPORT_WISDOM_TO_STRING) && (MY_FFTW_IMPORT_WISDOM_FROM_STRING) &&
            (MY_FFTW_FORGET_WISDOM) )
    {
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL LoadFFTWLibrary(const char* libraryname)
{

    if (libraryname == NULL)
    {
        return FALSE;
    }
    if (hinstLib == NULL)
    {
#ifdef _MSC_VER
        {
            wchar_t * wclibraryname = to_wide_string(libraryname);
            if (wclibraryname)
            {
                hinstLib = LoadDynLibraryW(wclibraryname);
                FREE(wclibraryname);
                wclibraryname = NULL;
            }
        }
#else
        hinstLib = LoadDynLibrary(libraryname);
#endif
        MY_FFTW_EXECUTE_SPLIT_DFT        = NULL;
        MY_FFTW_EXECUTE_SPLIT_DFT_C2R    = NULL;
        MY_FFTW_EXECUTE_SPLIT_DFT_R2C    = NULL;
        MY_FFTW_EXECUTE_SPLIT_DFT_R2R    = NULL;

        MY_FFTW_PLAN_GURU_SPLIT_DFT      = NULL;
        MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R  = NULL;
        MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C  = NULL;
        MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R  = NULL;

        MY_FFTW_DESTROY_PLAN             = NULL;

        MY_FFTW_EXPORT_WISDOM_TO_STRING  = NULL;
        MY_FFTW_IMPORT_WISDOM_FROM_STRING = NULL;
        MY_FFTW_FORGET_WISDOM            = NULL;

        MY_FFTW_EXECUTE_SPLIT_DFT     = (PROC_FFTW_EXECUTE_SPLIT_DFT)     GetDynLibFuncPtr(hinstLib, "fftw_execute_split_dft");
        MY_FFTW_EXECUTE_SPLIT_DFT_C2R = (PROC_FFTW_EXECUTE_SPLIT_DFT_C2R) GetDynLibFuncPtr(hinstLib, "fftw_execute_split_dft_c2r");
        MY_FFTW_EXECUTE_SPLIT_DFT_R2C = (PROC_FFTW_EXECUTE_SPLIT_DFT_R2C) GetDynLibFuncPtr(hinstLib, "fftw_execute_split_dft_r2c");
        MY_FFTW_EXECUTE_SPLIT_DFT_R2R = (PROC_FFTW_EXECUTE_SPLIT_DFT_R2R) GetDynLibFuncPtr(hinstLib, "fftw_execute_r2r");

        MY_FFTW_PLAN_GURU_SPLIT_DFT     = (PROC_FFTW_PLAN_GURU_SPLIT_DFT)     GetDynLibFuncPtr(hinstLib, "fftw_plan_guru_split_dft");
        MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R = (PROC_FFTW_PLAN_GURU_SPLIT_DFT_C2R) GetDynLibFuncPtr(hinstLib, "fftw_plan_guru_split_dft_c2r");
        MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C = (PROC_FFTW_PLAN_GURU_SPLIT_DFT_R2C) GetDynLibFuncPtr(hinstLib, "fftw_plan_guru_split_dft_r2c");
        MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R = (PROC_FFTW_PLAN_GURU_SPLIT_DFT_R2R) GetDynLibFuncPtr(hinstLib, "fftw_plan_guru_r2r");


        MY_FFTW_DESTROY_PLAN = (PROC_FFTW_DESTROY_PLAN) GetDynLibFuncPtr(hinstLib, "fftw_destroy_plan");

        MY_FFTW_EXPORT_WISDOM_TO_STRING   = (PROC_FFTW_EXPORT_WISDOM_TO_STRING)   GetDynLibFuncPtr(hinstLib, "fftw_export_wisdom_to_string");
        MY_FFTW_IMPORT_WISDOM_FROM_STRING = (PROC_FFTW_IMPORT_WISDOM_FROM_STRING) GetDynLibFuncPtr(hinstLib, "fftw_import_wisdom_from_string");
        MY_FFTW_FORGET_WISDOM             = (PROC_FFTW_FORGET_WISDOM) GetDynLibFuncPtr(hinstLib, "fftw_forget_wisdom");
    }

    return IsLoadedFFTW();
}
/*--------------------------------------------------------------------------*/
BOOL DisposeFFTWLibrary(void)
{
    BOOL fFreeResult;

    if (hinstLib)
    {
        fFreeResult = FreeDynLibrary(hinstLib);
        hinstLib = NULL;
    }

    if (MY_FFTW_EXECUTE_SPLIT_DFT)
    {
        MY_FFTW_EXECUTE_SPLIT_DFT = NULL;
    }
    if (MY_FFTW_EXECUTE_SPLIT_DFT_C2R)
    {
        MY_FFTW_EXECUTE_SPLIT_DFT_C2R = NULL;
    }
    if (MY_FFTW_EXECUTE_SPLIT_DFT_R2C)
    {
        MY_FFTW_EXECUTE_SPLIT_DFT_R2C = NULL;
    }
    if (MY_FFTW_EXECUTE_SPLIT_DFT_R2R)
    {
        MY_FFTW_EXECUTE_SPLIT_DFT_R2R = NULL;
    }

    if (MY_FFTW_PLAN_GURU_SPLIT_DFT)
    {
        MY_FFTW_PLAN_GURU_SPLIT_DFT = NULL;
    }
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R)
    {
        MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R = NULL;
    }
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C)
    {
        MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C = NULL;
    }
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R)
    {
        MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R = NULL;
    }

    if (MY_FFTW_DESTROY_PLAN)
    {
        MY_FFTW_DESTROY_PLAN = NULL;
    }

    if (MY_FFTW_EXPORT_WISDOM_TO_STRING)
    {
        MY_FFTW_EXPORT_WISDOM_TO_STRING     = NULL;
    }
    if (MY_FFTW_IMPORT_WISDOM_FROM_STRING)
    {
        MY_FFTW_IMPORT_WISDOM_FROM_STRING = NULL;
    }
    if (MY_FFTW_FORGET_WISDOM)
    {
        MY_FFTW_FORGET_WISDOM                         = NULL;
    }

    if ( !MY_FFTW_EXECUTE_SPLIT_DFT       && !MY_FFTW_EXECUTE_SPLIT_DFT_C2R     &&
            !MY_FFTW_EXECUTE_SPLIT_DFT_R2C   && !MY_FFTW_EXECUTE_SPLIT_DFT_R2R     &&
            !MY_FFTW_PLAN_GURU_SPLIT_DFT     && !MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R   &&
            !MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C && !MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R   &&
            !MY_FFTW_DESTROY_PLAN            &&
            !MY_FFTW_EXPORT_WISDOM_TO_STRING && !MY_FFTW_IMPORT_WISDOM_FROM_STRING &&
            !MY_FFTW_FORGET_WISDOM )
    {
        return TRUE;
    }

    return FALSE;
}
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft (const fftw_plan p, double *ri, double *ii, double *ro, double *io)
{
    if (MY_FFTW_EXECUTE_SPLIT_DFT)
    {
        (MY_FFTW_EXECUTE_SPLIT_DFT)(p, ri, ii, ro, io);
    }
}
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft_c2r (const fftw_plan p, double *ri, double *ii, double *ro)
{
    if (MY_FFTW_EXECUTE_SPLIT_DFT_C2R)
    {
        (MY_FFTW_EXECUTE_SPLIT_DFT_C2R)(p, ri, ii, ro);
    }
}
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft_r2c (const fftw_plan p, double *ri, double *ro, double *io)
{
    if (MY_FFTW_EXECUTE_SPLIT_DFT_R2C)
    {
        (MY_FFTW_EXECUTE_SPLIT_DFT_R2C)(p, ri, ro, io);
    }
}
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft_r2r (const fftw_plan p, double *ri, double *ro)
{
    if (MY_FFTW_EXECUTE_SPLIT_DFT_R2R)
    {
        (MY_FFTW_EXECUTE_SPLIT_DFT_R2R)(p, ri, ro);
    }
}
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags)
{
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT)
    {
        return (fftw_plan)(MY_FFTW_PLAN_GURU_SPLIT_DFT)(rank, dims, howmany_rank, howmany_dims, ri, ii, ro, io, flags);
    }
    else
    {
        return NULL;
    }
}
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft_c2r (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro,  unsigned flags)
{
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R)
    {
        return (fftw_plan)(MY_FFTW_PLAN_GURU_SPLIT_DFT_C2R)(rank, dims, howmany_rank, howmany_dims, ri, ii, ro, flags);
    }
    else
    {
        return NULL;
    }
}
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft_r2c (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ro, double *io, unsigned flags)
{
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C)
    {
        return (fftw_plan)(MY_FFTW_PLAN_GURU_SPLIT_DFT_R2C)(rank, dims, howmany_rank, howmany_dims, ri, ro, io, flags);
    }
    else
    {
        return NULL;
    }
}
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft_r2r (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ro, fftw_r2r_kind *kind, unsigned flags)
{
    if (MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R)
    {
        return (fftw_plan)(MY_FFTW_PLAN_GURU_SPLIT_DFT_R2R)(rank, dims, howmany_rank, howmany_dims, ri, ro, kind, flags);
    }
    else
    {
        return NULL;
    }
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
