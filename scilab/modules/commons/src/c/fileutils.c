/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

#include <string.h>

#ifdef _MSC_VER
#include <Windows.h>
#include <stdio.h>
#include "charEncoding.h"
#else
#include <sys/types.h>
#include <dirent.h>
#endif

#include "sci_malloc.h"
#include "os_string.h"

#include "PATH_MAX.h"
#include "scicurdir.h"


#include "fileutils.h"

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
int isEmptyDirectory(char *dirName)
{
    wchar_t *wcpath = NULL;
    wchar_t wdirpath[PATH_MAX + FILENAME_MAX + 1];
    HANDLE hFile;
    WIN32_FIND_DATAW FileInformation;
    int ret = 1;

    wcpath = to_wide_string(dirName);
    os_swprintf(wdirpath, wcslen(wcpath) + 2 + 1, L"%s\\*", wcpath);
    FREE(wcpath);

    hFile = FindFirstFileW(wdirpath, &FileInformation);

    if (hFile == INVALID_HANDLE_VALUE)
    {
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
    }
    while (FindNextFileW(hFile, &FileInformation) == TRUE);

    FindClose(hFile);

    return ret;
}

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/
int isEmptyDirectory(char *dirName)
{
    DIR *dir = NULL;

#ifdef __USE_LARGEFILE64
    struct dirent64 *ptr;
    struct dirent64 *result;
#else
    struct dirent *ptr;
    struct dirent *result;
#endif
    int ret = 1;

    dir = opendir(dirName);
    if (dir == NULL)
    {
        return 0;
    }

#ifdef __USE_LARGEFILE64
    ptr = MALLOC(sizeof(struct dirent64) + (PATH_MAX + 1));
#else
    ptr = MALLOC(sizeof(struct dirent) + (PATH_MAX + 1));
#endif
    if (ptr == NULL)
    {
        closedir(dir);
        return 0;
    }

#ifdef __USE_LARGEFILE64
    while ((readdir64_r(dir, ptr, &result) == 0) && (result != NULL))
#else
    while ((readdir_r(dir, ptr, &result) == 0) && (result != NULL))
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
/*--------------------------------------------------------------------------*/
char *getCWD()
{
    int err = 0;
    char *str;

    str = scigetcwd(&err);

    if (err)
    {
        return NULL;
    }
    else
    {
        return str;
    }
}

/*--------------------------------------------------------------------------*/
