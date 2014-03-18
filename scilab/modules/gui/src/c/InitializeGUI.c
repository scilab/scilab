/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdlib.h>
#include "InitializeGUI.h"
#include "scilabmode.h"
#include "setMainWindowTitle.h"
#include "MALLOC.h"
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
