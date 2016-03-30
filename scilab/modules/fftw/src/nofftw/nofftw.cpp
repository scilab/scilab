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

#include "fftw_gw.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "BOOL.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_fftw.h"
#include "with_fftw.h"
}
/*--------------------------------------------------------------------------*/
int gw_fftw(void)
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "fftw");
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL withfftw(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int C2F(withfftw)(int *rep)
{
    *rep = 0;
    return 0;
}
/*--------------------------------------------------------------------------*/
int FFTWModule::Load()
{
    /* Do Nothing */
    return true;
}
