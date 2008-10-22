/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#include <Windows.h>
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
char * ANSIToUTF8(char * pszCode)   
{   
	int     nLength = 0, nLength2 = 0;   
	BSTR    bstrCode;    
	char    *pszUTFCode = NULL;   

	nLength = MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlen(pszCode), NULL, NULL);    
	bstrCode = SysAllocStringLen(NULL, nLength);    
	MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlen(pszCode), bstrCode, nLength);   
	nLength2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL);    
	pszUTFCode = (char*)MALLOC(nLength2+1);    
	WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL, NULL);    
	return pszUTFCode;   
}  
/*--------------------------------------------------------------------------*/ 
char* UTF8ToANSI(char *pszCode)   
{   
    BSTR    bstrWide;   
	char*   pszAnsi = NULL;   
	int     nLength = 0;   

	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, NULL, NULL);   
	bstrWide = SysAllocStringLen(NULL, nLength);   
	MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, bstrWide, nLength);   
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);   
	pszAnsi = (char*) MALLOC(sizeof(char)*nLength+1);   
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);   
	SysFreeString(bstrWide);   
	return pszAnsi;   
} 
/*--------------------------------------------------------------------------*/ 
