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

#include <stdlib.h>
#include "InitializeGUI.h"
#include "configvariable_interface.h"
#include "setMainWindowTitle.h"
#include "sci_malloc.h"
#include "buildMainWindowTitle.h"
#ifdef _MSC_VER
#include "WindowShow.h"
#endif

/*--------------------------------------------------------------------------*/
BOOL InitializeGUI(void)
{
    if (getScilabMode() == SCILAB_STD)
    {

        // TODO why this line since already done above
        // createSwingView();
#ifdef _MSC_VER
        WindowShow();
#endif
    }
    else
    {
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------------------------------------------*/
