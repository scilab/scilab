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

#ifndef __GW_FFTW__
#define __GW_FFTW__
/*--------------------------------------------------------------------------*/
#include "dynlib_fftw.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
FTTW_SCILAB_IMPEXP int gw_fftw(void);
FTTW_SCILAB_IMPEXP int sci_disposefftwlibrary(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_fftw(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_fftw_flags(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_fftw_forget_wisdom(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_fftwlibraryisloaded(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_get_fftw_wisdom(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_loadfftwlibrary(char *fname,unsigned long fname_len);
FTTW_SCILAB_IMPEXP int sci_set_fftw_wisdom(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_FFTW__ */
/*--------------------------------------------------------------------------*/
