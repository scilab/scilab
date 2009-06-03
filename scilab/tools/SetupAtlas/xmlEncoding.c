/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include <windows.h>
#include "xmlEncoding.h"
/*--------------------------------------------------------------------------*/ 
wchar_t *toWideString(char *_Str)
{
	int nwide = 0;
	wchar_t *_buf = NULL;

	if(_Str == NULL) return NULL;
	nwide = MultiByteToWideChar(CP_UTF8, 0, _Str, -1, NULL, 0);
	if(nwide == 0) return NULL;
	_buf = (wchar_t *)calloc(nwide, sizeof(wchar_t));
	if(_buf == NULL) return NULL;
	if(MultiByteToWideChar(CP_UTF8, 0, _Str, -1, _buf, nwide) == 0)
	{
		free(_buf);
		_buf = NULL;
	}
	return _buf;
}
/*--------------------------------------------------------------------------*/ 
char *toUTF(wchar_t *_wcStr)
{
	DWORD size = 0;
	char *buf = NULL;

	if ((wchar_t *)NULL == _wcStr) return (char *)NULL;
	size = WideCharToMultiByte(CP_UTF8, 0, _wcStr, -1, NULL, 0, NULL, 0);
	if (size == 0) return (char *)NULL;
	size += 1;
	buf = (char*)calloc(size, sizeof(char));
	if (buf)
	{
		WideCharToMultiByte(CP_UTF8, 0, _wcStr, -1, buf, size, NULL, 0);
		if (size <= 0)
		{
			free(buf);
			return NULL;
		}
	}
	return buf;
}
/*--------------------------------------------------------------------------*/ 