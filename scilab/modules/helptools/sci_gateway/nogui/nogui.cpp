/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Simon LIPP
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "helptools_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "gw_helptools.h"
}
/*--------------------------------------------------------------------------*/
int gw_helptools()
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "helptools");
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_buildDoc(char *fname, unsigned long l)
{
    return 0;
}
/*--------------------------------------------------------------------------*/

int HelptoolsModule::Load()
{
    /* Do Nothing */
    return true;
}
