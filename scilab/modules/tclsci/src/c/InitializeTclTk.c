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

#include "InitializeTclTk.h"
#include "InitTclTk.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
BOOL InitializeTclTk(void)
{
    if ( getScilabMode() != SCILAB_NWNI )
    {
        setTkStarted(TRUE);
        if ( OpenTCLsci() != 0 )
        {
            setTkStarted(FALSE);
        }
    }
    return isTkStarted();
}
/*--------------------------------------------------------------------------*/
