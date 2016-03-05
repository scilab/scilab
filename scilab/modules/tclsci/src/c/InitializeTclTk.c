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

#include "InitializeTclTk.h"
#include "InitTclTk.h"
#include "configvariable_interface.h"
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
