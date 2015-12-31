/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <windows.h>
#include <shellapi.h>
#include <string.h>
#include "copyfile.h"
#include "BOOL.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "isdir.h"
#include "createdirectory.h"
#include "PATH_MAX.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int CopyFileFunction_windows(char* DestinationFilename, char* SourceFilename);
static int CopyDirectoryFunction_windows(char* DestinationDirectory, char* SourceDirectory);
/*--------------------------------------------------------------------------*/
int CopyFileFunction(char* DestinationFilename, char* SourceFilename)
{
    if (stricmp(DestinationFilename, SourceFilename) == 0)
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 1;
    }
    return CopyFileFunction_windows(DestinationFilename, SourceFilename);
}
/*--------------------------------------------------------------------------*/
int CopyDirectoryFunction(char* DestinationDirectory, char* SourceDirectory)
{
    /* remove last file separator if it does not exists */
    int srclen = (int)strlen(SourceDirectory);
    if ( (SourceDirectory[srclen - 1] == '\\') ||
        (SourceDirectory[srclen - 1] == '/'))
    {
        SourceDirectory[srclen - 1] = '\0';
    }

    /* remove last file separator if it does not exists */
    int dstlen = (int)strlen(DestinationDirectory);
    if ((DestinationDirectory[dstlen - 1] == '\\') ||
        (DestinationDirectory[dstlen - 1] == '/'))
    {
        DestinationDirectory[dstlen - 1] = '\0';
    }

    if (stricmp(DestinationDirectory, SourceDirectory) == 0)
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 1;
    }


    return CopyDirectoryFunction_windows(DestinationDirectory, SourceDirectory);
}
/*--------------------------------------------------------------------------*/
static int CopyFileFunction_windows(char* DestinationFilename, char* SourceFilename)
{
    BOOL bFailIfExists = FALSE;
    if (!CopyFileA(SourceFilename, DestinationFilename, bFailIfExists))
    {
        return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int CopyDirectoryFunction_windows(char* DestinationDirectory, char* SourceDirectory)
{
    WIN32_FIND_DATA dir_find_data;
    char src_buffer[MAX_PATH * 2 + 1];
    char dest_buffer[MAX_PATH * 2 + 1];
    HANDLE find_handle = FindFirstFileA(DestinationDirectory, &dir_find_data);
    BOOL ans;

    if (find_handle && (find_handle != INVALID_HANDLE_VALUE))
    {
        /* destdir exists, now make sure it is a directory */
        if (dir_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            ans = 1;
        }
        else
        {
            ans = 0;
        }

        FindClose(find_handle);
    }
    else
    {
        /* Create the destdir */
        ans = createdirectory(DestinationDirectory);
    }

    FindClose(find_handle);

    if (!ans)
    {
        return 1;
    }

    /* Get ready to do some copying */
    strcpy(src_buffer, SourceDirectory);
    strcat(src_buffer, "\\*");

    strcpy(dest_buffer, DestinationDirectory);
    strcat(dest_buffer, "\\");

    find_handle = FindFirstFileA(src_buffer, &dir_find_data);
    if (find_handle && find_handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (strcmp(dir_find_data.cFileName, ".") == 0)
            {
                continue;
            }
            if (strcmp(dir_find_data.cFileName, "..") == 0)
            {
                continue;
            }

            strcpy(src_buffer + strlen(SourceDirectory) + 1, dir_find_data.cFileName);
            strcpy(dest_buffer + strlen(DestinationDirectory) + 1, dir_find_data.cFileName);

            if (dir_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ans = (CopyDirectoryFunction_windows( dest_buffer, src_buffer) == 0);
            }
            else
            {
                ans = CopyFileA(src_buffer, dest_buffer, FALSE);
            }
            if (!ans)
            {
                break;
            }
        }
        while (FindNextFileA(find_handle, &dir_find_data));

        FindClose(find_handle);
    }
    return (ans ? 0 : 1);
}
/*--------------------------------------------------------------------------*/
#endif /* #ifdef _MSC_VER */
/*--------------------------------------------------------------------------*/
