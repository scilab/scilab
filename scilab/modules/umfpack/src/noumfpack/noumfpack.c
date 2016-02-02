/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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
#include "gw_umfpack.h"
#include "with_umfpack.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int gw_umfpack(void)
{
    Scierror(999, _("Scilab UMFPACK module not installed.\n"));
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL withumfpack(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int C2F(withumfpack)(int *rep)
{
    *rep = 0;
    return 0;
}
/*--------------------------------------------------------------------------*/
