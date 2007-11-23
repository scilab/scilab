/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "GetExceptionCode.h"
#include "ExceptionMessage.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
extern char *GetExceptionString(DWORD ExceptionCode);
/*--------------------------------------------------------------------------*/
void ExceptionMessage(DWORD ExceptionCode,char *functionname)
{
	char *ExceptionString=GetExceptionString(ExceptionCode);
	if (functionname)
	{
		sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nSave your data and restart Scilab.\n",ExceptionString,functionname);
	}
	else
	{
		sciprint("Warning !!!\nScilab has found a critical error (%s).\nSave your data and restart Scilab.\n",ExceptionString);
	}
	if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
}
/*--------------------------------------------------------------------------*/
