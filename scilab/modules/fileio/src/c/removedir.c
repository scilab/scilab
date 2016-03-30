/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "localization.h"
#include "sciprint.h"
#include "removedir.h"
#include "isdir.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
#include "Sciwarning.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int DeleteDirectory(wchar_t *refcstrRootDirectory);
#else
static int DeleteDirectory(char *refcstrRootDirectory);
#endif
/*--------------------------------------------------------------------------*/
BOOL removedir(char *path)
{
    if (isdir(path))
    {
#ifdef _MSC_VER
        {
            wchar_t *pstPath = to_wide_string(path);
            if (pstPath)
            {
                removedirW(pstPath);
                FREE(pstPath);
                pstPath = NULL;
            }
        }
#else
        DeleteDirectory(path);
#endif
        if (!isdir(path))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL removedirW(wchar_t *pathW)
{
    if (isdirW(pathW))
    {
#ifdef _MSC_VER
        DeleteDirectory(pathW);
#else
        char *path = wide_string_to_UTF8(pathW);
        if (path)
        {
            DeleteDirectory(path);
            FREE(path);
            path = NULL;
        }
#endif
        if (!isdirW(pathW))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int DeleteDirectory(wchar_t *refcstrRootDirectory)
{
#define DEFAULT_PATTERN L"%ls/*.*"
BOOL bDeleteSubdirectories = TRUE;
BOOL bSubdirectory = FALSE;
HANDLE hFile;
WIN32_FIND_DATAW FileInformation;
DWORD dwError;
wchar_t	*strPattern		= NULL;
wchar_t	*strFilePath	= NULL;
int len = 0;

    if (refcstrRootDirectory == NULL)
    {
        return 1;
    }

    len = (int)(wcslen(refcstrRootDirectory) + (int)wcslen(DEFAULT_PATTERN) + 1);

    strPattern = (wchar_t*)MALLOC(sizeof(wchar_t) * len);
    if (strPattern)
    {
        os_swprintf(strPattern, len, DEFAULT_PATTERN, refcstrRootDirectory);
    }
    else
    {
        return 1;
    }

    hFile = FindFirstFileW(strPattern, &FileInformation);
    if (strPattern)
    {
        FREE(strPattern);
        strPattern = NULL;
    }

    if (hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ( (wcscmp(FileInformation.cFileName, L".") != 0) && (wcscmp(FileInformation.cFileName, L"..") != 0) )
            {
#define FORMAT_PATH_TO_REMOVE L"%ls\\%ls"
int len = (int) (wcslen(refcstrRootDirectory) + wcslen(FORMAT_PATH_TO_REMOVE) + wcslen((wchar_t*)(FileInformation.cFileName)) + 1);
strFilePath = (wchar_t*) MALLOC(sizeof(wchar_t) * len);
if (strFilePath)
{
os_swprintf(strFilePath, len, FORMAT_PATH_TO_REMOVE, refcstrRootDirectory, FileInformation.cFileName);
}

                if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (bDeleteSubdirectories)
                    {
                        int iRC = DeleteDirectory(strFilePath);
                        if (strFilePath)
                        {
                            FREE(strFilePath);
                            strFilePath = NULL;
                        }
                        if (strPattern)
                        {
                            FREE(strPattern);
                            strPattern = NULL;
                        }

                        if (iRC)
                        {
                            return iRC;
                        }
                    }
                    else
                    {
                        bSubdirectory = TRUE;
                    }
                }
                else
                {
                    if (SetFileAttributesW(strFilePath, FILE_ATTRIBUTE_NORMAL) == FALSE)
                    {
                        if (strFilePath)
                        {
                            FREE(strFilePath);
                            strFilePath = NULL;
                        }
                        if (strPattern)
                        {
                            FREE(strPattern);
                            strPattern = NULL;
                        }
                        return GetLastError();
                    }

                    if (DeleteFileW(strFilePath) == FALSE)
                    {
                        if (strFilePath)
                        {
                            FREE(strFilePath);
                            strFilePath = NULL;
                        }
                        if (strPattern)
                        {
                            FREE(strPattern);
                            strPattern = NULL;
                        }
                        return GetLastError();
                    }
                }
            }

            if (strFilePath)
            {
                FREE(strFilePath);
                strFilePath = NULL;
            }
        }
        while (FindNextFileW(hFile, &FileInformation) == TRUE);

        FindClose(hFile);
        if (strFilePath)
        {
            FREE(strFilePath);
            strFilePath = NULL;
        }
        if (strPattern)
        {
            FREE(strPattern);
            strPattern = NULL;
        }

        dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES)
        {
            return dwError;
        }
        else
        {
            if (!bSubdirectory)
            {
                if (SetFileAttributesW(refcstrRootDirectory, FILE_ATTRIBUTE_NORMAL) == FALSE)
                {
                    return GetLastError();
                }
                if (RemoveDirectoryW(refcstrRootDirectory) == FALSE)
                {
                    return GetLastError();
                }
            }
        }
    }

    if (strFilePath)
    {
        FREE(strFilePath);
        strFilePath = NULL;
    }
    if (strPattern)
    {
        FREE(strPattern);
        strPattern = NULL;
    }
    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static int DeleteDirectory(char *refcstrRootDirectory)
{
    DIR *dir;
    struct dirent *ent;
    dir = opendir(refcstrRootDirectory) ;

    if (dir == NULL)
    {
        Sciwarning(_("Warning: Error while opening %s: %s\n"), refcstrRootDirectory, strerror(errno));
        return -1;
    }

    while ((ent = readdir(dir)) != NULL)
    {
        char *filename = NULL;
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue ;
        }

        filename = MALLOC(sizeof(char) * (strlen(refcstrRootDirectory) + 1 + strlen(ent->d_name) + 1 + 1)) ;
        sprintf(filename, "%s/%s", refcstrRootDirectory, ent->d_name);
        if (isdir(filename))
        {
            /* Delete recursively */
            DeleteDirectory(filename);
            FREE(filename);
        }
        else
        {
            /* Not a directory... It must be a file (at least, I hope it is a file */
            if (remove(filename) != 0)
            {
                Sciwarning(_("Warning: Could not remove file %s: %s\n"), filename, strerror(errno));
            }

            FREE(filename);
        }
    }
    if (rmdir(refcstrRootDirectory) != 0)
    {
        Sciwarning(_("Warning: Could not remove directory %s: %s\n"), refcstrRootDirectory, strerror(errno));
    }

    if (dir)
    {
        closedir(dir);
    }

    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
