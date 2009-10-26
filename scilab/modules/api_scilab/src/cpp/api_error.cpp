/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be 
 * still available and supported in Scilab 6.
 */

#include "MALLOC.h"
#include <stdio.h> 
#include "api_common.h"
#include "sciprint.h"
#include "Scierror.h"
#include "stack-c.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "localization.h"
extern "C"
{
	#include "stackinfo.h"
	#include "mode_exec.h"
}

int addStackSizeError(StrErr* _pstrErr, char* _pstCaller, int _iNeeded)
{
	char pstMsg1[bsiz];
	char pstMsg2[bsiz];
	char pstMsg3[bsiz];
	char pstMsg4[bsiz];
	char pstMsg5[bsiz];

	int Memory_used_for_variables = 0;
	int Total_Memory_available = 0;

	C2F(getstackinfo)(&Total_Memory_available,&Memory_used_for_variables);

#ifdef _MSC_VER
	sprintf_s(pstMsg1, bsiz, "%s\n%s", _pstCaller, _("stack size exceeded!\n"));
	sprintf_s(pstMsg2, bsiz, _("Use stacksize function to increase it.\n"));
	sprintf_s(pstMsg3, bsiz, _("Memory used for variables: %d\n"),Memory_used_for_variables);
	sprintf_s(pstMsg4, bsiz, _("Intermediate memory needed: %d\n"),_iNeeded);
	sprintf_s(pstMsg5, bsiz, _("Total memory available: %d\n"),Total_Memory_available);
#else
	sprintf(pstMsg1, _("stack size exceeded!\n"));
	sprintf(pstMsg2, _("Use stacksize function to increase it.\n"));
	sprintf(pstMsg3, _("Memory used for variables: %d\n"),Memory_used_for_variables);
	sprintf(pstMsg4, _("Intermediate memory needed: %d\n"),_iNeeded);
	sprintf(pstMsg5, _("Total memory available: %d\n"),Total_Memory_available);
#endif

	strcat(pstMsg1, pstMsg2);
	strcat(pstMsg1, pstMsg3);
	strcat(pstMsg1, pstMsg4);
	strcat(pstMsg1, pstMsg5);

	return addErrorMessage(_pstrErr, 17, pstMsg1);
}

int addErrorMessage(StrErr* _pstrErr, int _iErr, const char* _pstMsg, ...)
{
	int iRet = 0;
	char pstMsg[bsiz];
	va_list ap;

	va_start(ap,_pstMsg);
#if defined (vsnprintf) || defined (linux)
	iRet = vsnprintf(pstMsg, bsiz-1, _pstMsg, ap );
#else
	iRet = vsprintf(pstMsg, _pstMsg, ap );
#endif

	if(_pstrErr->iMsgCount == MESSAGE_STACK_SIZE - 1)
	{//rotation ...
		FREE(_pstrErr->pstMsg[0]);
		for(int i = MESSAGE_STACK_SIZE - 1 ; i > 0 ; i--)
		{
			_pstrErr->pstMsg[i - 1] = _pstrErr->pstMsg[i];
		}
	}
	_pstrErr->pstMsg[_pstrErr->iMsgCount++] = strdup(pstMsg);
	_pstrErr->iErr = _iErr;
	return iRet;
}

int printError(StrErr* _pstrErr, int _iLastMsg)
{
	int iMode = getExecMode();

	if(_pstrErr->iErr == 0)
	{
		return 0;
	}

	SciStoreError(_pstrErr->iErr);

	if(iMode == SILENT_EXEC_MODE)
	{
		return 0;
	}

	if(_iLastMsg)
	{
		sciprint(_("API Error:\n"));
		sciprint(_("\tin %s\n"), _pstrErr->pstMsg[0]);
	}
	else
	{
		sciprint(_("API Error:\n"));

		//		for(int i = 0 ;  i < _pstrErr->iMsgCount ;i++)
		for(int i = _pstrErr->iMsgCount - 1 ;  i >= 0 ; i--)
		{
//			if(i == 0)
			sciprint(_("\tin %s\n"), _pstrErr->pstMsg[i]);
		}
	}
	return 0;
}

char* getErrorMessage(StrErr _strErr)
{
	if(_strErr.iErr == 0)
	{
		return NULL;
	}

	return _strErr.pstMsg[0];
}

