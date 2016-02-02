/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
#include "ConsolePrintf.h"
#include "ConsoleRead.h"
#include "ClearConsolePart.h"
#include "ClearConsole.h"
#include "SetConsolePrompt.h"
#include "InitializeConsole.h"
#include "PromptToHome.h"
#include "sci_malloc.h"
#include "ScilabLinesUpdate.h"
#include "GetCharWithoutOutput.h"
#include "ConsoleIsWaitingForInput.h"
#include "scilines.h"

/*--------------------------------------------------------------------------*/
char *ConsoleRead(void)
{
    return NULL;
}
/*--------------------------------------------------------------------------*/
int ConsolePrintf(const char *line)
{
    int len = 0;
    return len;
}
/*--------------------------------------------------------------------------*/
BOOL ClearConsolePart(int nbLines)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL ClearConsole(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int GetCharWithoutOutput()
{
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL PromptToHome(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL SetConsolePrompt(char *Sci_prompt)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL InitializeConsole(void)
{
    scilinesdefault();
    return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL ConsoleIsWaitingForInput(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL ScilabLinesUpdate(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
