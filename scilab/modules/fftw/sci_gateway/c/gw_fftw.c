/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "callfftw.h"
#include "Scierror.h"
#include "sciprint.h"
#include "MALLOC.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
typedef int (*fftw_interf) __PARAMS((char *fname,unsigned long fname_len));
typedef struct table_struct {
	fftw_interf f;    /** function **/
	char *name;      /** its name **/
} intFFTWTable;
/*-----------------------------------------------------------------------------------*/ 
static intFFTWTable Tab[]={
{ sci_loadfftwlibrary,"loadfftwlibrary"},
{ sci_disposefftwlibrary,"disposefftwlibrary"},
{ sci_fftwlibraryisloaded,"fftwlibraryisloaded"},
{ sci_fftw,"fftw"},
{ sci_fftw_flags,"fftw_flags"},
{ sci_get_fftw_wisdom,"get_fftw_wisdom"},
{ sci_set_fftw_wisdom,"set_fftw_wisdom"},
{ sci_fftw_forget_wisdom,"fftw_forget_wisdom"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_fftw)()
{  
	Rhs = Max(0, Rhs);

	if (Fin > 3) /* loadfftwlibrary, disposefftwlibrary, fftwlibraryisloaded not test if dll is loaded*/
	{
		if (!IsLoadedFFTW())
		{
			char *fftwlibNAME = getfftwlibname();

			if (fftwlibNAME)
			{
				Scierror(999,"FFTW Library %s not found.\r\n",fftwlibNAME);
			}
			else
			{
				Scierror(999,"FFTW Library not found.\r\n");
			}
			
			if (fftwlibNAME) {FREE(fftwlibNAME); fftwlibNAME=NULL;}
			
			return 0;
		}
	}

	#ifdef _MSC_VER
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
