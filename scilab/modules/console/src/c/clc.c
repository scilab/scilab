/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "clc.h"
#include "configvariable_interface.h"
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
