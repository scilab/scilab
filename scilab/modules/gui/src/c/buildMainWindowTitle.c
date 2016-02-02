/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#include <string.h>
#include "buildMainWindowTitle.h"
#include "os_string.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/buildMainWindowTitle_Windows.h"
#endif
#include "version.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
char *buildMainWindowTitle(void)
{
    char *retTitle = NULL;

#ifndef _MSC_VER
    retTitle = os_strdup(SCI_VERSION_STRING);
#else
    retTitle = buildMainWindowTitle_Windows();
#endif
    return retTitle;
}
/*--------------------------------------------------------------------------*/
