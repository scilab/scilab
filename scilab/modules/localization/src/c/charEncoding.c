/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - Yung-Jang Lee
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS , Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
int wcsicmp_others(const wchar_t* s1, const wchar_t* s2)
{
    while (towlower(*s1) == towlower(*s2))
    {
        if (*s1 == 0) return 0;
        s1++;
        s2++;
    }
    return towlower(*s1) - towlower(*s2);
}
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
char *wide_string_to_UTF8(const wchar_t *_wide)
{
	char *buf = NULL;
	DWORD size = 0;
	DWORD dwFlags = 0;

	if ((wchar_t *)NULL == _wide) return (char *)NULL;
	size = WideCharToMultiByte(CP_UTF8, dwFlags, _wide, -1, NULL, 0, NULL, 0);
	if (size == 0) return (char *)NULL;
	size += 1;
	buf = (char*)MALLOC(sizeof(char)*size);
	if (buf)
	{
		WideCharToMultiByte(CP_UTF8, dwFlags, _wide, -1, buf, size, NULL, 0);
		if (size <= 0)
		{
			FREE(buf);
			return (char *)NULL;
		}
	}
	return buf;
}
/*--------------------------------------------------------------------------*/
wchar_t *to_wide_string(const char *_UTFStr)
{
	int nwide = 0;
	wchar_t *_buf = NULL;

	/* About MultiByteToWideChar : 
	Starting with Windows Vista, 
	the function does not drop illegal code points 
	if the application does not set this flag. 

	Windows XP: To prevent the security problem of the non-shortest-form
	versions of UTF-8 characters, MultiByteToWideChar deletes these characters.
	*/

	DWORD dwFlags = 0;

	if(_UTFStr == NULL) return NULL;
	nwide = MultiByteToWideChar(CP_UTF8, dwFlags, _UTFStr, -1, NULL, 0);
	if(nwide == 0) return NULL;
	_buf = (wchar_t *)MALLOC(nwide * sizeof(wchar_t));
	if(_buf == NULL) return NULL;
	if(MultiByteToWideChar(CP_UTF8, dwFlags, _UTFStr, -1, _buf, nwide) == 0)
	{
		FREE(_buf);
		_buf = NULL;
	}
	return _buf;
}
/*--------------------------------------------------------------------------*/
int wcstat(char* filename, struct _stat *st)
{
	int stat_result = 0;
	wchar_t *wfilename = to_wide_string(filename);
	stat_result = _wstat(wfilename, st);
	FREE(wfilename);
	return stat_result;
}
/*--------------------------------------------------------------------------*/
#else //Linux check for MAC OS X
char *wide_string_to_UTF8(const wchar_t *_wide)
{
	size_t iCharLen = 0;
	wchar_t *pwstr = _wide;
	char* pchar = NULL;

	if (_wide == NULL)
	{
		return NULL;
	}

	/* The value of MB_CUR_MAX is the maximum number of bytes 
	   in a multibyte character for the current locale. */
	int iMaxLen = (int)wcslen(_wide) * MB_CUR_MAX ; 
	pchar = (char*) MALLOC(( iMaxLen + 1) * sizeof(char));
	if(pchar == NULL)
	{
		return NULL;
	}
	iCharLen = wcstombs (pchar, pwstr, iMaxLen);
	if ( iCharLen == (size_t)(-1) )
	{
		if (pchar) {FREE(pchar);pchar = NULL;}
		return NULL;
	}
	pchar[iCharLen] = '\0';
	return pchar;
}
/*--------------------------------------------------------------------------*/
wchar_t *to_wide_string(const char *_UTFStr)
{
	wchar_t *_buf = NULL;
	size_t pszLen = 0;
	char *psz = _UTFStr;
	mbstate_t ps;

	if (_UTFStr == NULL)
	{
		return NULL;
	}

	memset (&ps, 0x00, sizeof(ps));
	pszLen = mbsrtowcs(NULL, (const char**)&psz, 0, &ps);

	if ( pszLen == (size_t)(-1) )
	{
		return NULL;
	}

	_buf = (wchar_t*)MALLOC((pszLen + 1) * sizeof(wchar_t));
	if(_buf == NULL)
	{
		return NULL;
	}

	pszLen = mbsrtowcs(_buf, (const char**)&psz, (int)strlen(psz), &ps);

	if ( pszLen == (size_t)(-1) )
	{
		FREE(_buf);
		return NULL;
	}
	else
	{
		_buf[pszLen] = L'\0';
	}
	return _buf;
}
/*--------------------------------------------------------------------------*/
int wcstat(char* filename, struct stat *st)
{
	return stat(filename, st);
}
#endif
/*--------------------------------------------------------------------------*/
static int ReadUTF8Character(const char* str, int *nBytes)
{
	const unsigned char* s = (const unsigned char*)(str);
	unsigned char c = s[0];

	if (c < 0x80)
	{
		*nBytes = 1;
		return c;
	}
	else if (c < 0xC2)
	{
		return -1;
	}
	else if (c < 0xE0)
	{
		if (s[1] == 0 || (s[1] ^ 0x80) >= 0x40) 
		{
			return -1;
		}
		*nBytes = 2;
		return ((s[0] & 0x1F) << 6) + (s[1] ^ 0x80);
	}
	else if (c < 0xF0)
	{
		if (s[1] == 0 || s[2] == 0 || (s[1] ^ 0x80) >= 0x40
			|| (s[2] ^ 0x80) >= 0x40 || (c == 0xE0 && s[1] < 0xA0))
		{
			return -1;
		}
		*nBytes = 3;
		return ((s[0] & 0xF) << 12) + ((s[1] ^ 0x80) << 6) + (s[2] ^ 0x80);
	}
	else if (c < 0xF5)
	{
		if (s[1] == 0 || s[2] == 0 || s[3] == 0 || (s[1] ^ 0x80) >= 0x40
			|| (s[2] ^ 0x80) >= 0x40 || (s[3] ^ 0x80) >= 0x40
			|| (c == 0xF0 && s[1] < 0x90) || (c == 0xF4 && s[1] > 0x8F))
		{
			return -1;
		}
		*nBytes = 4;
		return ((s[0] & 0x07) << 18) + ((s[1] ^ 0x80) << 12) + ((s[2] ^ 0x80) << 6)
			+ (s[3] ^ 0x80);
	}
	else
	{
		return -1;
	}
}
/*--------------------------------------------------------------------------*/
BOOL IsValidUTF8(const char*  pStText)
{
	const char* s =  pStText;
	while (*s != 0)
	{
		int nBytes = 0;
		if (ReadUTF8Character(s, &nBytes) < 0)
		{
			return FALSE;
		}
		s += nBytes;
	}

	return TRUE;
}
/*--------------------------------------------------------------------------*/
