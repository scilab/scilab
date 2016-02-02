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
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <stdio.h>
#include "version.h"
#include "configvariable_interface.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void disp_scilab_version(void)
{
    if ( (getScilabMode() == SCILAB_NWNI) || (getScilabMode() == SCILAB_NW) || (getScilabMode() == SCILAB_API) )
    {
        printf(_("Scilab version \"%d.%d.%d.%d\"\n"), SCI_VERSION_MAJOR, SCI_VERSION_MINOR, SCI_VERSION_MAINTENANCE, SCI_VERSION_TIMESTAMP);
        printf("%s\n\n", SCI_VERSION_STRING);
    }
    else
    {
#ifdef _MSC_VER
        {
            char msg[1024];
            wsprintf(msg, gettext("Scilab version \"%d.%d.%d.%d\"\n%s\n"), SCI_VERSION_MAJOR, SCI_VERSION_MINOR, SCI_VERSION_MAINTENANCE, SCI_VERSION_TIMESTAMP, SCI_VERSION_STRING);
            MessageBox(NULL, msg, gettext("Scilab Version Info."), MB_ICONINFORMATION);
        }
#else
        printf(_("Scilab version \"%d.%d.%d.%d\"\n"), SCI_VERSION_MAJOR, SCI_VERSION_MINOR, SCI_VERSION_MAINTENANCE, SCI_VERSION_TIMESTAMP);
        printf("%s\n\n", SCI_VERSION_STRING);
#endif
    }
}
/*--------------------------------------------------------------------------*/
