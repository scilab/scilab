/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

#ifndef __GW_FFTW__
#define __GW_FFTW__
/*--------------------------------------------------------------------------*/
#include "dynlib_fftw.h"
/*--------------------------------------------------------------------------*/
FTTW_SCILAB_IMPEXP int gw_fftw(void);
FTTW_SCILAB_IMPEXP int sci_disposefftwlibrary(char *fname, void* pvApiCtx);
FTTW_SCILAB_IMPEXP int sci_fftw_flags(char *fname, void* pvApiCtx);
FTTW_SCILAB_IMPEXP int sci_fftw_forget_wisdom(char *fname, void* pvApiCtx);
FTTW_SCILAB_IMPEXP int sci_fftwlibraryisloaded(char *fname, void* pvApiCtx);
FTTW_SCILAB_IMPEXP int sci_get_fftw_wisdom(char *fname, void* pvApiCtx);
FTTW_SCILAB_IMPEXP int sci_loadfftwlibrary(char *fname, void* pvApiCtx);
FTTW_SCILAB_IMPEXP int sci_set_fftw_wisdom(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_FFTW__ */
/*--------------------------------------------------------------------------*/
