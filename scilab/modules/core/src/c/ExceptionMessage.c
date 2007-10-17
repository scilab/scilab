/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "GetExceptionCode.h"
#include "ExceptionMessage.h"
#include "message_scilab.h"
/*-----------------------------------------------------------------------------------*/
extern char *GetExceptionString(DWORD ExceptionCode);
/*-----------------------------------------------------------------------------------*/
void ExceptionMessage(DWORD ExceptionCode,char *functionname)
{
	char *ExceptionString=GetExceptionString(ExceptionCode);
	if (functionname)
	{
		message_scilab(_("Warning !!! exception 2"),ExceptionString,functionname);
	}
	else
	{
		message_scilab(_("Warning !!! exception 1"),ExceptionString);
	}
	if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
}
/*-----------------------------------------------------------------------------------*/
