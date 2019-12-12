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
            if (getPromptMode() != 2) /* Add extra newline for other modes besides mode 2 */
            {
                printf("\n");
            }
            bOK = TRUE;
#else
            if (getPromptMode() == 2) /* Check for output mode(2) and move the cursor up for extra 1 line */
            {
                printf("\033[%dA\033[J\033[A", nblines + 1);
            }
            else
            {
                printf("\033[%dA\033[J", nblines + 1);
            }
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
