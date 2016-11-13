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
static BOOL find_spec(const char *filename , const char *filespec);
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
char** findfiles(const char *path, const char *filespec, int *sizeListReturned, BOOL warning)
{
    char** ListFiles = NULL;
    char* strPattern = NULL;
    HANDLE hFile;
    WIN32_FIND_DATAA FileInformation;
    int nbElements = 0;
    int len = 0;

    len = (int)(strlen(path) + strlen(filespec) + 8);
    strPattern = (char*)MALLOC(sizeof(char) * len);
    os_sprintf(strPattern, len, "%s/%s", path, filespec);

    hFile = FindFirstFile(strPattern, &FileInformation);
    if (strPattern)
    {
        FREE(strPattern);
        strPattern = NULL;
    }

    if (hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (strcmp(FileInformation.cFileName, ".") &&  strcmp(FileInformation.cFileName, ".."))
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
                ListFiles[nbElements - 1] = os_strdup(FileInformation.cFileName);
            }
        }
        while (FindNextFileA(hFile, &FileInformation) == TRUE);
    }
    else
    {
        if (warning)
        {
            Sciwarning(_("Warning: Could not open directory %s: %s\n"), path, strerror(errno));
        }
    }
    FindClose(hFile);
    *sizeListReturned = nbElements;
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
/**
 * Check if the file matches the mask
 * '*' for all chars
 * '?' for only one
 * @TODO check if it can be optimized
 * @param filename the filename
 * @param filespec the mask
 */
static BOOL find_spec(const char *filename , const char *filespec)
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
