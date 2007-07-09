/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include "CommandLine.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
CommandLine::CommandLine(void)
{
	line = NULL;
}
/*------------------------------------------------------------------------*/
CommandLine::CommandLine(char *line_)
{
	set(line_);
}
/*------------------------------------------------------------------------*/
CommandLine::~CommandLine()
{
}
/*------------------------------------------------------------------------*/
char *CommandLine::get(void)
{
	char *str = NULL;
	if (line)
	{
		str =  (char*) MALLOC ((strlen(line)+1)*(sizeof(char)));
		strcpy(str,line);
	}
	return str;
}
/*------------------------------------------------------------------------*/
BOOL CommandLine::set(char *line_)
{
	BOOL bOK = FALSE;
	if (line_)
	{
		line =  (char*) MALLOC ((strlen(line_)+1)*(sizeof(char)));
		if (line)
		{
			strcpy(line,line_);
			bOK = TRUE;

		}
	}
	else
	{
		line = NULL;
		bOK = FALSE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
void CommandLine::free(void)
{
	if (line)
	{
		FREE(line);
		line = NULL;
	}
}
/*------------------------------------------------------------------------*/
