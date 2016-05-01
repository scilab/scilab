/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <string.h>
#include "findfiles.h"
#include "sci_malloc.h"
#include "localization.h"
#include "sciprint.h"
#include "os_string.h"
#include "charEncoding.h"
#include "Sciwarning.h"
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static BOOL find_spec( char *filename , char *filespec);
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
wchar_t** findfilesW(const wchar_t *path, const wchar_t *filespec, int *sizeListReturned, BOOL warning)
{
    wchar_t **ListFiles = NULL;
    wchar_t *wcstrPattern = NULL;
    HANDLE hFile;
    WIN32_FIND_DATAW FileInformation;
    int nbElements = 0;
    int len = 0;

    len = (int)( wcslen(path) + wcslen(filespec) + 8);
    wcstrPattern = (wchar_t*)MALLOC(sizeof(wchar_t) * len);
    os_swprintf(wcstrPattern, len, L"%ls/%ls", path, filespec);

    hFile = FindFirstFileW(wcstrPattern, &FileInformation);
    if (wcstrPattern)
    {
        FREE(wcstrPattern);
        wcstrPattern = NULL;
    }

    if (hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (wcscmp(FileInformation.cFileName, L".") &&  wcscmp(FileInformation.cFileName, L".."))
            {
                nbElements++;
                if (ListFiles)
                {
                    ListFiles = (wchar_t**)REALLOC(ListFiles, sizeof(char*) * (nbElements));
                }
                else
                {
                    ListFiles = (wchar_t**)MALLOC(sizeof(wchar_t*) * (nbElements));
                }
                ListFiles[nbElements - 1] = os_wcsdup(FileInformation.cFileName);
            }
        }
        while (FindNextFileW(hFile, &FileInformation) == TRUE);
    }
    else
    {
        if (warning)
        {
            char* pstPath = wide_string_to_UTF8(path);
            char* pstError = wide_string_to_UTF8(_wcserror(errno));
            Sciwarning(_("Warning: Could not open directory %s: %s\n"), pstPath, pstError);
            FREE(pstPath);
            FREE(pstError);
        }
    }
    FindClose(hFile);
    *sizeListReturned = nbElements;
    return ListFiles;
}

char** findfiles(const char *path, const char *filespec, int *sizeListReturned, BOOL warning)
{
    int i = 0;
    wchar_t** wListFiles = NULL;
    char** ListFiles = NULL;
    wchar_t* pstPath = to_wide_string(path);
    wchar_t* pstFileSpec = to_wide_string(filespec);
    wListFiles = findfilesW(pstPath, pstFileSpec, sizeListReturned, warning);

    if (*sizeListReturned)
    {
        ListFiles = (char**)MALLOC(sizeof(char*) * *sizeListReturned);

        for (i = 0 ; i < *sizeListReturned ; i++)
        {
            ListFiles[i] = wide_string_to_UTF8(wListFiles[i]);
            FREE(wListFiles[i]);
        }
        FREE(wListFiles);
    }

    FREE(pstPath);
    FREE(pstFileSpec);
    return ListFiles;
}
#else
/*--------------------------------------------------------------------------*/
char **findfiles(const char *path, const char *filespec, int *sizeListReturned, BOOL warning)
{
    char **ListFiles = NULL;
    int nbElements = 0;
    DIR *folder = NULL;
    struct dirent *read = NULL;

    *sizeListReturned = 0;

    folder = opendir(path);
    if (folder)
    {
        while ((read = readdir(folder)))
        {
            if ( strcmp(read->d_name, ".") && strcmp(read->d_name, "..") )
            {
                if ( find_spec(read->d_name , filespec) )
                {
                    nbElements++;
                    if (ListFiles)
                    {
                        ListFiles = (char**)REALLOC(ListFiles, sizeof(char*) * (nbElements));
                    }
                    else
                    {
                        ListFiles = (char**)MALLOC(sizeof(char*) * (nbElements));
                    }
                    ListFiles[nbElements - 1] = os_strdup(read->d_name);
                }
            }
        }
        closedir(folder);
    }
    else
    {
        if (warning)
        {
            Sciwarning(_("Warning: Could not open directory %s: %s\n"), path, strerror(errno));
        }
    }

    *sizeListReturned = nbElements;
    return ListFiles;
}
/*--------------------------------------------------------------------------*/
wchar_t** findfilesW(const wchar_t* path, const wchar_t* filespec, int* sizeListReturned, BOOL warning)
{
    int i = 0;
    char* pstPath = wide_string_to_UTF8(path);
    char* pstFileSpec = wide_string_to_UTF8(filespec);

    wchar_t** wListFiles = NULL;
    char** ListFiles = findfiles(pstPath, pstFileSpec, sizeListReturned, warning);

    if (*sizeListReturned != 0)
    {
        wListFiles = (wchar_t**)MALLOC(sizeof(wchar_t*) * (*sizeListReturned));

        for (i = 0 ; i < (*sizeListReturned) ; i++)
        {
            wListFiles[i] = to_wide_string(ListFiles[i]);
            FREE(ListFiles[i]);
        }

        FREE(ListFiles);
    }

    FREE(pstPath);
    FREE(pstFileSpec);
    return wListFiles;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
/**
 * Check if the file matches the mask
 * '*' for all chars
 * '?' for only one
 * @TODO check if it can be optimized
 * @param filename the filename
 * @param filespec the mask
 */
static BOOL find_spec( char *filename , char *filespec)
{
    char *any = NULL;

    while ( ( *filespec ) || ( *filename ) )
    {
        switch ( *filespec )
        {
            case '*':
                any = filespec;
                filespec++;
                while ( ( *filename != *filespec ) && ( *filename ) )
                {
                    filename++;
                }
                break;

            case '?':
                filespec++;
                if ( *filename )
                {
                    filename++;
                }
                break;

            default:
                if ( *filespec != *filename )
                {
                    if ( ( any ) && ( *filename ) )
                    {
                        filespec = any;
                    }
                    else
                    {
                        return FALSE;
                    }
                }
                else
                {
                    filespec++;
                    filename++;
                }
                break;
        }
    }

    return TRUE;
}
#endif
/*--------------------------------------------------------------------------*/
