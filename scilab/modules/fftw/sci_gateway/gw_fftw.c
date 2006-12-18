#include "gw_fftw.h"
#include "callfftw.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
extern int  Scierror __PARAMS((int iv,char *fmt,...));
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int sci_fftw __PARAMS((char *fname,unsigned long fname_len));
extern int sci_fftw_flags __PARAMS((char *fname,unsigned long fname_len));
extern int sci_fftw_inplace __PARAMS((char *fname,unsigned long fname_len));
extern int sci_fftw_norm __PARAMS((char *fname,unsigned long fname_len));
extern int sci_pushfftw_plan __PARAMS((char *fname,unsigned long fname_len));
extern int sci_freefftw_plan __PARAMS((char *fname,unsigned long fname_len));
extern int sci_loadfftwlibrary __PARAMS((char *fname,unsigned long fname_len));
extern int sci_disposefftwlibrary __PARAMS((char *fname,unsigned long fname_len));
extern int sci_fftwlibraryisloaded __PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static intFFTWTable Tab[]={
{ sci_loadfftwlibrary,"loadfftwlibrary"},
{ sci_disposefftwlibrary,"disposefftwlibrary"},
{ sci_fftwlibraryisloaded,"fftwlibraryisloaded"},
{ sci_fftw,"fftw"},
{ sci_fftw_flags,"fftw_flags"},
{ sci_fftw_inplace,"fftw_inplace"},
{ sci_fftw_norm,"fftw_norm"},
{ sci_pushfftw_plan,"pushfftw_plan"},
{ sci_freefftw_plan,"freefftw_plan"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_fftw)()
{  
	Rhs = Max(0, Rhs);

	if (Fin > 3) /* loadfftwlibrary, disposefftwlibrary, fftwlibraryisloaded not test if dll is loaded*/
	{
		if (!IsLoadedFFTW())
		{
			#if _MSC_VER
			Scierror(999,"FFTW Library %s not found in SCI/bin directory.\r\n",FFTWLIBNAMEWINDOWS);
			#else
			Scierror(999,"FFTW Library %s not found in SCI/bin directory.\r\n",FFTWLIBNAMELINUX);
			#endif
			return 0;
		}
	}

	#if _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
