/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
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
#ifdef _MSC_VER
#include <conio.h>
#endif
#include <stdio.h>
#include "more.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "sciprint.h"
#include "GetCharWithoutOutput.h"
#include "ConsolePrintf.h"
/*--------------------------------------------------------------------------*/
#define MSG_MORE _("[Continue display? n (no) to stop, any other key to continue]")
/*--------------------------------------------------------------------------*/
int linesmore(void)
{
    int retval = 0;
    if (getScilabMode() != SCILAB_STD)
    {
        int ch = 0;
        /* Scilab has not his own window */
        sciprint(MSG_MORE);

#if _MSC_VER
        ch = _getch();
        if ( (ch != ' ') && (ch != 13) && (ch != 'y') )
        {
            retval = 1;
        }
#else
        ch = getchar();
        if ( (ch != ' ') && (ch != '\n') && (ch != 'y') )
        {
            retval = 1;
        }
#endif

        sciprint("\n");
    }
    else
    {
        /* scilab has his own window */
        int ch = 0;
        ConsolePrintf(MSG_MORE);
        ch = GetCharWithoutOutput();
        if ( ch == 110 )
        {
            retval = 1;
        }
    }
    return retval;
}
/*--------------------------------------------------------------------------*/
