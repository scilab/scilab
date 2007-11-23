/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "callfftw.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
{ sci_loadfftwlibrary,"loadfftwlibrary"},
{ sci_disposefftwlibrary,"disposefftwlibrary"},
{ sci_fftwlibraryisloaded,"fftwlibraryisloaded"},
{ sci_fftw,"fftw"},
{ sci_fftw_flags,"fftw_flags"},
{ sci_get_fftw_wisdom,"get_fftw_wisdom"},
{ sci_set_fftw_wisdom,"set_fftw_wisdom"},
{ sci_fftw_forget_wisdom,"fftw_forget_wisdom"}
};
/*--------------------------------------------------------------------------*/ 
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
				Scierror(999,_("FFTW Library %s not found.\n"),fftwlibNAME);
			}
			else
			{
				Scierror(999,_("FFTW Library not found.\n"));
			}
			
			if (fftwlibNAME) {FREE(fftwlibNAME); fftwlibNAME=NULL;}
			
			return 0;
		}
	}

	callFunctionFromGateway(Tab);

	return 0;
}
/*--------------------------------------------------------------------------*/ 
