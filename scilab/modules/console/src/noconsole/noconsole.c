/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "machine.h"
#include "ConsolePrintf.h"
#include "ConsoleRead.h"
#include "ClearConsolePart.h"
#include "ClearConsole.h"
#include "SetConsolePrompt.h"
#include "InitializeConsole.h"
#include "TerminateConsole.h"
#include "PromptToHome.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "GetCharWithoutOutput.h"
/*-----------------------------------------------------------------------------------*/
/*
** Bruno : Useless...
int C2F(gw_console)()
{
Scierror(999,"console interface not installed.\n");
return 0;
}
*/
/*-----------------------------------------------------------------------------------*/
char *ConsoleRead(void)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/
int ConsolePrintf(char *line)
{
	int len = 0;
	return len;
}
/*-----------------------------------------------------------------------------------*/
BOOL ClearConsolePart(int nbLines)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ClearConsole(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
int GetCharWithoutOutput()
{
  return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL PromptToHome(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
BOOL SetConsolePrompt(char *Sci_prompt)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
BOOL InitializeConsole(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
BOOL TerminateConsole(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
