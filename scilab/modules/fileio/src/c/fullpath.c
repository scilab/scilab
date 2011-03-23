/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <sys/param.h>
#include <limits.h>
#endif
#include "fullpath.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "machine.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
static unsigned int isDirSeparator(const char c);
static int normalizePath(char *path);
/*--------------------------------------------------------------------------*/
char * get_full_path(char * _FullPath, const char * _Path, size_t _SizeInBytes)
{
#if defined(_MSC_VER)
    char *returnedFullPath = NULL;	

    wchar_t *wPath = to_wide_string((char*)_Path);
    wchar_t *wFullPath = (wchar_t *) MALLOC(sizeof(wchar_t)*_SizeInBytes);

    _wfullpath(wFullPath,wPath, _SizeInBytes );
    returnedFullPath = wide_string_to_UTF8(wFullPath);
    if (returnedFullPath)
    {
        strcpy(_FullPath,returnedFullPath);
        FREE(returnedFullPath);
        returnedFullPath = NULL;
    }

    if (wPath) {FREE(wPath);wPath=NULL;}
    if (wFullPath) {FREE(wFullPath);wFullPath=NULL;}

    return _FullPath;
#else
    int lenPath = (int)strlen(_Path);
    char *rp = realpath(_Path, _FullPath);
    int lenFullPath = 0;
    int haveFileSep = ( (lenPath > 1) && isDirSeparator(_Path[lenPath - 1]) );
    int addFileSep = 0;

    if (!rp)
    {
        strcpy(_FullPath, _Path);
        normalizePath(_FullPath);
    }
    lenFullPath = (int)strlen(_FullPath);
    addFileSep = ( (lenFullPath > 1) && (!isDirSeparator(_FullPath[lenFullPath - 1])) && haveFileSep );
    if (addFileSep)
    {
        char *bufTmp = (char *) MALLOC(sizeof(char)* (lenFullPath + strlen(DIR_SEPARATOR) + 1));
        if (bufTmp)
        {
            sprintf(bufTmp, "%s%s", _FullPath, DIR_SEPARATOR);
            strcpy(_FullPath, bufTmp);
            FREE(bufTmp);
            bufTmp = NULL;
        }
    }
    return _FullPath;
#endif
}
/*--------------------------------------------------------------------------*/
wchar_t * get_full_pathW(wchar_t * _wcFullPath, const wchar_t * _wcPath, size_t _SizeInBytes)
{
    wchar_t *wcResult = NULL;
#if defined(_MSC_VER)
    if (_wcPath)
    {
        wcResult = (wchar_t *) MALLOC(sizeof(wchar_t)*_SizeInBytes);
        if (wcResult)
        {
            _wfullpath(wcResult, _wcPath, _SizeInBytes );
            wcscpy(_wcFullPath, wcResult);
        }
    }
#else
    if (_wcPath)
    {
        char *_Path = wide_string_to_UTF8(_wcPath);
        if (_Path)
        {
            char *_FullPath = (char *) MALLOC(sizeof(char)*(_SizeInBytes));
            if (_FullPath)
            {
                char *rp = realpath(_Path, _FullPath);
                wcResult = to_wide_string(_FullPath);
                if (wcResult)
                {
                    wcscpy(_wcFullPath, wcResult);
                }
                FREE(_FullPath);
                _FullPath = NULL;
            }
            FREE(_Path);
            _Path = NULL;
        }
    }
#endif
    return wcResult;
}
/*--------------------------------------------------------------------------*/
static unsigned int isDirSeparator(const char c)
{
    return (c == '/' || c == '\\');
}
/*--------------------------------------------------------------------------*/
static int normalizePath(char *path)
{
    char *dirs[PATH_MAX];
    int depth = 0;
    char *dstptr = path;
    char *srcptr = path;

    dirs[0] = path;
    depth++;

    while(1)
    {
        if ((srcptr[0] == '.') && isDirSeparator(srcptr[1]))
        {
            /* ./ */
            srcptr += 2;
        }
        else if (srcptr[0] == '.' && srcptr[1] == '.' && isDirSeparator(srcptr[2]))
        {
            /* ../ */
            if(depth == 1)
            {
                /* ../ */
                dstptr[0] = '.';
                dstptr[1] = '.';
                dstptr[2] = DIR_SEPARATOR[0];
                dstptr += 3;
                srcptr += 3;
                dirs[0] = dstptr;
            }
            else
            {
                /* a/b/../c */
                depth--;
                dstptr = dirs[depth-1];
                srcptr += 3;
            }
        }
        else if(srcptr[0] == '.'  && srcptr[1] == '.' && srcptr[2] == 0)
        {
            /* .. */
            if(depth == 1)
            {
                dstptr[0] = '.';
                dstptr[1] = '.';
                dstptr += 2;
                srcptr += 2;
                dirs[0] = dstptr;
            }
            else
            {
                depth--;
                dstptr = dirs[depth-1];
                srcptr += 2;
            }
        }
        else
        {
            while(!isDirSeparator(srcptr[0]) && srcptr[0])
            {
                *dstptr++ = *srcptr++;
            }

            if(srcptr[0] == 0)
            {
                if(dstptr != path && isDirSeparator(dstptr[-1]))
                {
                    dstptr[-1] = 0;
                }
                dstptr[0] = 0;
                return 0;
            }
            else if(isDirSeparator(srcptr[0]))
            {
                *dstptr++ = *srcptr++;
                dirs[depth] = dstptr;
                depth++;
                /* // */
                while(isDirSeparator(srcptr[0]) && srcptr[0])
                {
                    srcptr++;
                }
            }
            else
            {
                /* error */
                return -1;
            }
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
