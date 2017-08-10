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

#ifndef __CALLFFTW_H__
#define __CALLFFTW_H__
/*--------------------------------------------------------------------------*/
#include "BOOL.h"
#include "fftw3.h"
/*--------------------------------------------------------------------------*/
BOOL IsLoadedFFTW(void);
/*--------------------------------------------------------------------------*/
BOOL LoadFFTWLibrary(const char *libraryname);
/*--------------------------------------------------------------------------*/
BOOL DisposeFFTWLibrary(void);
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft (const fftw_plan p, double *ri, double *ii, double *ro, double *io);
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft_c2r (const fftw_plan p, double *ri, double *ii, double *ro);
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft_r2c (const fftw_plan p, double *ri, double *ro, double *io);
/*--------------------------------------------------------------------------*/
void call_fftw_execute_split_dft_r2r (const fftw_plan p, double *ri, double *ro);
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags) ;
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft_c2r (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro,  unsigned flags);
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft_r2c (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ro, double *io, unsigned flags);
/*--------------------------------------------------------------------------*/
fftw_plan call_fftw_plan_guru_split_dft_r2r (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ro, fftw_r2r_kind *kind, unsigned flags);
/*--------------------------------------------------------------------------*/
void call_fftw_destroy_plan (fftw_plan p);
/*--------------------------------------------------------------------------*/
char *call_fftw_export_wisdom_to_string (void);
/*--------------------------------------------------------------------------*/
int call_fftw_import_wisdom_from_string (const char *input_string);
/*--------------------------------------------------------------------------*/
void call_fftw_forget_wisdom (void);
/*--------------------------------------------------------------------------*/
int withMKL(void);
/*--------------------------------------------------------------------------*/

#endif /* __CALLFFTW_H__ */
/*--------------------------------------------------------------------------*/
