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

#include <stdio.h> 
#include "api_common.h"
#include "sciprint.h"
#include "Scierror.h"
#include "stack-c.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "localization.h"

int addErrorMessage(StrErr* _pstrErr, int _iErr, char* _pstMsg, ...)
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

	_pstrErr->pstMsg[_pstrErr->iMsgCount++] = strdup(pstMsg);
	_pstrErr->iErr = _iErr;
	return iRet;
}

int printError(StrErr _strErr, int _iLastMsg)
{
	if(_strErr.iErr == 0)
	{
		return 0;
	}

	if(_iLastMsg)
	{
		Scierror(_strErr.iErr, _strErr.pstMsg[0]);
	}
	else
	{
		for(int i = _strErr.iMsgCount - 1;  i >= 0 ;i--)
		{
			//if(i == _strErr.iMsgCount - 1)
			//{
			//	Scierror(_strErr.iErr, "%s", _strErr.pstMsg[i]);
			//}
			//else
			{
				sciprint(_("API error: %s"), _strErr.pstMsg[i]);
			}
		}
		sciprint("\n");
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
