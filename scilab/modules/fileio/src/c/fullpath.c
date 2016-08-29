/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
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
#include "sci_malloc.h"
#include "machine.h"
#include "PATH_MAX.h"
#include "strsubst.h"
#include "scicurdir.h"
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static unsigned int isDirSeparator(const char c);
static int normalizePath(char *path);
#endif
/*--------------------------------------------------------------------------*/
char *get_full_path(char *_FullPath, const char *_Path, size_t _SizeInBytes)
{
#if defined(_MSC_VER)
    char *returnedFullPath = NULL;

    wchar_t *wPath = to_wide_string((char *)_Path);
    wchar_t *wFullPath = (wchar_t *) MALLOC(sizeof(wchar_t) * _SizeInBytes);

    _wfullpath(wFullPath, wPath, _SizeInBytes);
    returnedFullPath = wide_string_to_UTF8(wFullPath);
    if (returnedFullPath)
    {
        strcpy(_FullPath, returnedFullPath);
        FREE(returnedFullPath);
        returnedFullPath = NULL;
    }

    if (wPath)
    {
        FREE(wPath);
        wPath = NULL;
    }
    if (wFullPath)
    {
        FREE(wFullPath);
        wFullPath = NULL;
    }

    return _FullPath;
#else
    char *rp = NULL;
    int lenPath = (int)strlen(_Path);
    int lenFullPath = 0;
    int haveFileSep = ((lenPath > 1) && isDirSeparator(_Path[lenPath - 1]));
    int addFileSep = 0;

    rp = realpath(_Path, _FullPath);
    lenFullPath = (int)strlen(_FullPath);

    if (rp == NULL)
    {
        char * tofind;
        char * toadd;
        char * _Path_tmp;
        char * _Path_start;
        char * _FullPath_start;
        char* pstWorkingPath = NULL;

        //if argument is a relative path, add currentdir at start
        if (_Path[0] != '/')
        {
            int ierr = 0;
            char* pstCurrentPath = scigetcwd(&ierr);
            //alloc buffer + 2, 1 for '/' and 1 for null termination
            pstWorkingPath = (char*)CALLOC(sizeof(char), (lenPath + strlen(pstCurrentPath) + 2));
            sprintf(pstWorkingPath, "%s/%s", pstCurrentPath, _Path);
            lenPath = strlen(pstWorkingPath);
            FREE(pstCurrentPath);
        }
        else
        {
            pstWorkingPath = strdup(_Path);
        }

        _Path_tmp = (char *)MALLOC(sizeof(char) * (lenPath + 1));
        _Path_start = (char *)MALLOC(sizeof(char) * (lenPath + 1));
        _FullPath_start = (char *)MALLOC(sizeof(char) * (lenFullPath + 1));

        //First case(1): fullpath(TMPDIR+"/a/b/c"), second case(2): fullpath("a/b/c") or third case(3): fullpath("../a/b")
        strcpy(_Path_start, pstWorkingPath); // _Path_start=TMPDIR+"/a/b/c" (1) or _Path_start="a/b/c" (2) or _Path_start="../a/b/c" (3)
        strcpy(_FullPath_start, _FullPath); // _Fullpath_Start=TMPDIR+"/a" (1) or _FullPath_start=SCI+"/a" (2) or _FullPath_start=../SCI+"/a" (3)
        strtok(_Path_start, "/"); // _Path_start=/tmp  (1) or _Path_start="a" (2) or _Path_start="a/b/c" (3)
        strtok(_FullPath_start, "/"); // _FullPath_start=/tmp (1) or _FullPath_start=/home (2) and (3)

#if defined(__APPLE__)
        if (strcmp(_FullPath_start, "/private") == 0) // For case: fullpath(TMPDIR+"/a/b/c") (1)
        {
            normalizePath(_FullPath);
        }
#else
        if (strcmp(_Path_start, _FullPath_start) == 0) // For case: fullpath(TMPDIR+"/a/b/c") (1)
        {
            strcpy(_FullPath, pstWorkingPath);
            normalizePath(_FullPath);
        }
#endif
        else if (strcmp(_Path, _FullPath) != 0) // For case: fullpath("a/b/c") (2) or fullpath("../a/b/c") (3)
        {
            strcpy(_Path_tmp, pstWorkingPath); //_Path_tmp="a/b/c" (2) or _Path_tmp="../a/b/c" (3)
            strtok(_Path_tmp, "./"); // _Path_tmp becomes a (2) or ../a (3)
            toadd = strsub(pstWorkingPath, _Path_tmp, ""); // to add = "/b/c"
            strcat(_FullPath, toadd); //_FullPath=_Fullpath+toadd
            FREE(toadd);
        }

        FREE(pstWorkingPath);
        FREE(_FullPath_start);
        FREE(_Path_start);
        FREE(_Path_tmp);
    }

    lenFullPath = (int)strlen(_FullPath);
    addFileSep = ((lenFullPath > 1) && (!isDirSeparator(_FullPath[lenFullPath - 1])) && haveFileSep);
    if (addFileSep)
    {
        char *bufTmp = (char *)MALLOC(sizeof(char) * (lenFullPath + strlen(DIR_SEPARATOR) + 1));
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
wchar_t *get_full_pathW(wchar_t * _wcFullPath, const wchar_t * _wcPath, size_t _SizeInBytes)
{
#if defined(_MSC_VER)
    if (_wcPath)
    {
        _wfullpath(_wcFullPath, _wcPath, _SizeInBytes);
        return _wcFullPath;
    }
    return NULL;
#else
    if (_wcPath)
    {
        char *_Path = wide_string_to_UTF8(_wcPath);
        if (_Path)
        {
            char *_FullPath = (char *)MALLOC(sizeof(char) * (_SizeInBytes));

            if (_FullPath)
            {
                wchar_t *wcResult = NULL;
                char *rp = NULL;

                rp = realpath(_Path, _FullPath);
                if (!rp)
                {
                    strcpy(_FullPath, _Path);
                    normalizePath(_FullPath);
                }
                wcResult = to_wide_string(_FullPath);
                FREE(_FullPath);
                if (wcResult)
                {
                    wcscpy(_wcFullPath, wcResult);
                    FREE(wcResult);
                }
            }
            FREE(_Path);
        }
    }
    return _wcFullPath;
#endif
}

/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
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

    while (1)
    {
        if ((srcptr[0] == '.') && isDirSeparator(srcptr[1]))
        {
            /* ./ */
            srcptr += 2;
        }
        else if (srcptr[0] == '.' && srcptr[1] == '.' && isDirSeparator(srcptr[2]))
        {
            /* ../ */
            if (depth == 1)
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
                dstptr = dirs[depth - 1];
                srcptr += 3;
            }
        }
        else if (srcptr[0] == '.' && srcptr[1] == '.' && srcptr[2] == 0)
        {
            /* .. */
            if (depth == 1)
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
                dstptr = dirs[depth - 1];
                srcptr += 2;
            }
        }
        else
        {
            while (!isDirSeparator(srcptr[0]) && srcptr[0])
            {
                *dstptr++ = *srcptr++;
            }

            if (srcptr[0] == 0)
            {
                if (dstptr != path && isDirSeparator(dstptr[-1]))
                {
                    dstptr[-1] = 0;
                }
                dstptr[0] = 0;
                return 0;
            }
            else if (isDirSeparator(srcptr[0]))
            {
                *dstptr++ = *srcptr++;
                dirs[depth] = dstptr;
                depth++;
                /* // */
                while (isDirSeparator(srcptr[0]) && srcptr[0])
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
}
#endif
/*--------------------------------------------------------------------------*/
