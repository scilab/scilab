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
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "tohome.h"
#include "scilabmode.h"
#include "PromptToHome.h"
#ifndef _MSC_VER
#include "others/clrscr_nw.h"
#endif
/*--------------------------------------------------------------------------*/
BOOL tohome(void)
{
    BOOL bOK = FALSE;

    if ( getScilabMode() != SCILAB_STD )
    {
#ifdef _MSC_VER
        /* not on Windows */
        bOK = FALSE;
#else
        clrscr_nw();
        bOK = TRUE;
#endif
    }
    else
    {
        /* Java Console */
        bOK = PromptToHome();
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
