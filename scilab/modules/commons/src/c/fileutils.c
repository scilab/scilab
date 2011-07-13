/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include <Windows.h>
#include "charEncoding.h"
#else
#include <sys/types.h>
#include <dirent.h>
#include "PATH_MAX.h"
#endif

#include <string.h>
#include "fileutils.h"
#include "MALLOC.h"

#ifdef _MSC_VER
int isEmptyDirectory(char * dirName)
{
    wchar_t *wcpath = NULL;
    HANDLE hFile;
    WIN32_FIND_DATAW FileInformation;
    int ret = 1;

    wcpath = to_wide_string(dirName);

    hFile = FindFirstFileW(wcpath, &FileInformation);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        FREE(wcpath);
        return 0;
    }

    do
    {
        if (!wcscmp(FileInformation.cFileName, L".") || !wcscmp(FileInformation.cFileName, L".."))
        {
            continue;
        }

        ret = 0;
        break;
    } while (FindNextFileW(hFile, &FileInformation) == TRUE);

    FREE(wcpath);
    FindClose(hFile);

    return ret;
}
#else
int isEmptyDirectory(char * dirName)
{
    DIR *dir = NULL;
#ifdef __APPLE__
  struct dirent *ptr;
  struct dirent *result;
#else
    struct dirent64 *ptr;
    struct dirent64 *result;
#endif
    int ret = 1;

    dir = opendir(dirName);
    if (dir == NULL)
    {
        return 0;
    }

#ifdef __APPLE__
    ptr = MALLOC(sizeof(struct dirent) + (PATH_MAX + 1));
#else
    ptr = MALLOC(sizeof(struct dirent64) + (PATH_MAX + 1));
#endif
    if (ptr == NULL)
    {
        closedir(dir);
        return 0;
    }

#ifdef __APPLE__
    while ((readdir_r(dir, ptr, &result) == 0)  && (result != NULL))
#else
    while ((readdir64_r(dir, ptr, &result) == 0)  && (result != NULL))
#endif
    {
        if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, ".."))
        {
            continue;
        }

        ret = 0;
        break;
    }

    FREE(ptr);
    closedir(dir);

    return ret;
}
#endif
