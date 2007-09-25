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
		message_scilab("Warning !!!\nScilab has found a critical error (%s)\nwith "%s" function.\nSave your data and restart Scilab.",ExceptionString,functionname);
	}
	else
	{
		message_scilab("Warning !!!\nScilab has found a critical error (%s).\nSave your data and restart Scilab.",ExceptionString);
	}
	if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
}
/*-----------------------------------------------------------------------------------*/
