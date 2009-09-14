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
#include "Scierror.h"
#include "MALLOC.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[] = 
{
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
int gw_fftw(void)
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

	callFunctionFromGateway(Tab,SIZE_CURRENT_GENERIC_TABLE(Tab));

	return 0;
}
/*--------------------------------------------------------------------------*/ 
