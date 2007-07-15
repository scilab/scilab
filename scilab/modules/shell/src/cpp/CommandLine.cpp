/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include "CommandLine.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
CommandLine::CommandLine(void)
{

}
/*------------------------------------------------------------------------*/
CommandLine::CommandLine(char *line_)
{
	str.assign(line_);
}
/*------------------------------------------------------------------------*/
CommandLine::~CommandLine()
{
}
/*------------------------------------------------------------------------*/
char *CommandLine::get(void)
{
	char *line = NULL;
	if (str.length()>0)
	{
		line =  (char*) MALLOC ((str.length()+1)*(sizeof(char)));
		strcpy(line,str.c_str() );
	}
	return line ;
}
/*------------------------------------------------------------------------*/
BOOL CommandLine::set(char *line_)
{
	BOOL bOK = FALSE;
	if (line_)
	{
		str.assign(line_);
		bOK = TRUE;
	}
	else
	{
		bOK = FALSE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
