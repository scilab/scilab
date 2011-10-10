/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "InitializeConsole.h"
#include "scilabmode.h"
#include "ScilabLinesUpdate.h"
#include "scilines.h"
/*--------------------------------------------------------------------------*/
BOOL InitializeConsole(void)
{
    BOOL bRes = TRUE;
    scilinesdefault();
    if (getScilabMode() == SCILAB_STD)
    {
        bRes = ScilabLinesUpdate();
    }
    return bRes;
}
/*--------------------------------------------------------------------------*/
