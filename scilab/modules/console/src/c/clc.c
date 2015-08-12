/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include <stdio.h>
#include "clc.h"
#include "scilabmode.h"
#include "ClearConsole.h"
#include "ClearConsolePart.h"
#ifdef _MSC_VER
#include "windows/clrscr_nw.h"
#include "windows/clrscrPart_nw.h"
#else
#include "others/clrscr_nw.h"
#endif
/*--------------------------------------------------------------------------*/
BOOL clc(int nblines)
{
    BOOL bOK = FALSE;
    if ( getScilabMode() != SCILAB_STD )
    {
        /* console C */
        if (nblines != -1)
        {
#ifdef  _MSC_VER
            clrscrPart_nw(nblines);
            bOK = TRUE;
#else
            printf("\033[%dA\033[J", nblines + 2);
            bOK = TRUE;
#endif
        }
        else
        {
            clrscr_nw();
            bOK = TRUE;
        }
    }
    else
    {
        /* Java Console*/
        if ( nblines == -1) /* Clear the whole console window */
        {
            bOK = ClearConsole();
        }
        else if (nblines >= 0) /* Clear a part of the console window */
        {
            bOK = ClearConsolePart(nblines);
        }
        else
        {
            /* error */
            bOK = FALSE;
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
