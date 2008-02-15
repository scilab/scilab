/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "machine.h"
#include "ConsolePrintf.h"
#include "ConsoleRead.h"
#include "ClearConsolePart.h"
#include "ClearConsole.h"
#include "SetConsolePrompt.h"
#include "InitializeConsole.h"
#include "PromptToHome.h"
#include "MALLOC.h"
#include "GetCharWithoutOutput.h"
/*--------------------------------------------------------------------------*/
char *ConsoleRead(void)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int ConsolePrintf(char *line)
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
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL ConsoleIsWaitingForInput(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
