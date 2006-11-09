/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "fftw3.h"
/*-----------------------------------------------------------------------------------*/
#define FFTWLIBNAMEWINDOWS "libfftw3-3.dll"
#define FFTWLIBNAMELINUX "libfftw3.so"
/*-----------------------------------------------------------------------------------*/
BOOL IsLoadedFFTW(void);
/*-----------------------------------------------------------------------------------*/
BOOL LoadFFTWLibrary(void);
/*-----------------------------------------------------------------------------------*/
BOOL DisposeFFTWLibrary(void);
/*-----------------------------------------------------------------------------------*/
void  call_fftw_execute_split_dft (const fftw_plan p, double *ri, double *ii, double *ro, double *io); 
/*-----------------------------------------------------------------------------------*/
fftw_plan  call_fftw_plan_guru_split_dft (int rank, const fftw_iodim *dims, int howmany_rank, const fftw_iodim *howmany_dims, double *ri, double *ii, double *ro, double *io, unsigned flags) ;
/*-----------------------------------------------------------------------------------*/
void  call_fftw_destroy_plan (fftw_plan p); 
/*-----------------------------------------------------------------------------------*/

