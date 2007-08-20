/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "machine.h" 
#include "ShellPrintf.h"
#include "ShellRead.h"
#include "ClearConsolePart.h"
#include "ClearConsole.h"
#include "PromptToHome.h"
#include "MALLOC.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_console)()
{
	Scierror(999,"console interface not installed.\r\n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
char *ShellRead(void)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
int ShellPrintf(char *line)
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