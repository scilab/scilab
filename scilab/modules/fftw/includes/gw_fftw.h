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
#include "machine.h"
#include "api_common.h"
#include "sci_get_fftw_wisdom.h"
#include "sci_fftw_flags.h"
#include "sci_loadfftwlibrary.h"
#include "sci_set_fftw_wisdom.h"
#include "sci_fftw_forget_wisdom.h"
#include "sci_fftw.h"
#include "sci_fftwlibraryisloaded.h"
#include "sci_disposefftwlibrary.h"
/*--------------------------------------------------------------------------*/
int gw_fftw(void);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_FFTW__ */
/*--------------------------------------------------------------------------*/
