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
		message_scilab("core_message_125",ExceptionString,functionname);
	}
	else
	{
		message_scilab("core_message_126",ExceptionString);
	}
	if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
}
/*-----------------------------------------------------------------------------------*/
