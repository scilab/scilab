/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Yung-Jang Lee
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS , Allan CORNET
 * Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef _MSC_VER
#include <iconv.h>
#include <errno.h>
#include <string.h>
#endif

#include <cstdlib>

#include "UTF8.hxx"

#ifdef _MSC_VER
#include <Windows.h>
#endif

namespace scilab
{

#ifdef _MSC_VER // Windows

std::string UTF8::toUTF8(const std::wstring & wstr)
{
    char * buf = nullptr;
    DWORD dwFlags = 0;
    DWORD size = 0;

    if (wstr.empty())
    {
        return std::string();
    }

    size = WideCharToMultiByte(CP_UTF8, dwFlags, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, 0);
    if (size == 0)
    {
        return std::string();
    }

    buf = new char[size];
    size = WideCharToMultiByte(CP_UTF8, dwFlags, wstr.c_str(), (int)wstr.size(), buf, size, nullptr, 0);
    if (size <= 0)
    {
        delete[] buf;
        return std::string();
    }

    std::string str(buf, buf + size);
    delete[] buf;

    return str;
}

std::wstring UTF8::toWide(const std::string & str)
{
    wchar_t *_buf = NULL;
    DWORD dwFlags = 0;
    UINT codePage = CP_ACP;

    if (str.empty())
    {
        return std::wstring();
    }

    if (IsValidUTF8(str.c_str()))
    {
        codePage = CP_UTF8;
    }

    int nwide = MultiByteToWideChar(codePage, dwFlags, str.c_str(), (int)str.size(), nullptr, 0);
    if (nwide == 0)
    {
        return std::wstring();
    }

    wchar_t * buf = new wchar_t[nwide];
    if (MultiByteToWideChar(codePage, dwFlags, str.c_str(), (int)str.size(), buf, nwide) == 0)
    {
        delete[] buf;
        return std::wstring();
    }

    std::wstring wstr(buf, buf + nwide);
    delete[] buf;

    return wstr;
}

#else
#ifdef __APPLE__ // Mac OS_X

std::string UTF8::toUTF8(const std::wstring & wstr)
{
    if (wstr.empty())
    {
        return std::string();
    }

    const int iMaxLen = (int)wstr.size() * MB_CUR_MAX ;

    char* pchar = new char[iMaxLen];
    size_t iCharLen = wcstombs(pchar, wstr.c_str(), iMaxLen);
    if (iCharLen == (size_t)(-1) )
    {
        delete[] pchar;
        return std::string();
    }

    std::string str(pchar, pchar + iMaxLen);
    delete[] pchar;

    return str;
}

std::wstring UTF8::toWide(const std::string & str)
{
    wchar_t *_buf = NULL;

    if (str.empty())
    {
        return std::wstring();
    }

    mbstate_t ps;
    memset (&ps, 0x00, sizeof(ps));

    const char * psz = str.c_str();
    size_t pszLen = mbsrtowcs(nullptr, &psz, 0, &ps);

    if (pszLen == (size_t)(-1))
    {
        return std::wstring();
    }

    wchar_t * buf = new wchar_t[pszLen];
    pszLen = mbsrtowcs(buf, &psz, str.size(), &ps);
    if (pszLen == (size_t)(-1))
    {
        delete[] buf;
        return std::wstring();
    }

    std::wstring wstr(buf, buf + pszLen);
    delete[] buf;

    return wstr;
}

#else // Linux

std::string UTF8::toUTF8(const std::wstring & wstr)
{
    if (wstr.empty())
    {
        return std::string();
    }

    char * pIn = (char *)wstr.c_str();
    size_t iLeftIn = wstr.size() * sizeof(wchar_t);
    size_t iLeftOut = iLeftIn + sizeof(wchar_t);
    char * pOut = new char[iLeftOut];
    char * pOutSave = pOut;
    iconv_t cd = iconv_open("UTF-8", "WCHAR_T");

    size_t iSize = iconv(cd, &pIn, &iLeftIn, &pOut, &iLeftOut);
    iconv_close(cd);
    if (iSize == (size_t)(-1))
    {
        delete[] pOutSave;
        return std::string();
    }

    std::string str(pOutSave, pOut);
    delete[] pOutSave;

    return str;
}

std::wstring UTF8::toWide(const std::string & str)
{
    if (str.empty())
    {
        return std::wstring();
    }

    char * pIn = (char *)str.c_str();
    size_t iLeftIn = str.size();
    size_t iLeftOut = (iLeftIn + 1) * sizeof(wchar_t);
    wchar_t * pOut = new wchar_t[iLeftOut];
    wchar_t * pOutSave = pOut;
    iconv_t cd = iconv_open("WCHAR_T", "UTF-8");

    size_t iSize = iconv(cd, &pIn, &iLeftIn, (char **)&pOut, &iLeftOut);
    iconv_close(cd);
    if (iSize == (size_t)(-1))
    {
        cd = iconv_open("WCHAR_T", "ISO_8859-1");
        pIn = (char *)str.c_str();
        iLeftIn = str.size();
        iLeftOut = (iLeftIn + 1) * sizeof(wchar_t);
        pOut = pOutSave;

        iSize = iconv(cd, &pIn, &iLeftIn, (char **)&pOut, &iLeftOut);
        iconv_close(cd);
        if (iSize == (size_t)(-1))
        {
            delete[] pOutSave;
            return std::wstring();
        }
    }

    std::wstring wstr(pOutSave, pOut);
    delete[] pOutSave;

    return wstr;
}

#endif
#endif

int UTF8::ReadUTF8Character(const char * str, int * nBytes)
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

bool UTF8::IsValidUTF8(const char*  pStText)
{
    const char* s = pStText;
    while (*s != 0)
    {
        int nBytes = 0;
        if (ReadUTF8Character(s, &nBytes) < 0)
        {
            return false;
        }
        s += nBytes;
    }

    return true;
}

} // namespace scilab
