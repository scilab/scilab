/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#ifndef _MSC_VER
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "copyfile.h"
#include "BOOL.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "isdir.h"
#include "FileExist.h"
#include "createdirectory.h"
#include "PATH_MAX.h"
#include "fullpath.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int CopyFileFunction_others(wchar_t *DestinationFilename, wchar_t *SourceFilename);
static int CopyDirectoryFunction_others(wchar_t *DestinationDirectory, wchar_t *SourceDirectory);
static int RecursiveCopyDirectory(char *DestinationDir, char *SourceDir);
/*--------------------------------------------------------------------------*/
int CopyFileFunction(wchar_t *DestinationFilename, wchar_t *SourceFilename)
{
    if (os_wcsicmp(DestinationFilename, SourceFilename) == 0)
    {
        return EPERM;
    }
    return CopyFileFunction_others(DestinationFilename, SourceFilename);
}
/*--------------------------------------------------------------------------*/
int CopyDirectoryFunction(wchar_t *DestinationDirectory, wchar_t *SourceDirectory)
{
    /* remove last file separator if it does not exists */
    if ( (SourceDirectory[wcslen(SourceDirectory) - 1] == L'\\') ||
            (SourceDirectory[wcslen(SourceDirectory) - 1] == L'/') )
    {
        SourceDirectory[wcslen(SourceDirectory) - 1] = L'\0';
    }

    /* remove last file separator if it does not exists */
    if ( (DestinationDirectory[wcslen(DestinationDirectory) - 1] == L'\\') ||
            (DestinationDirectory[wcslen(DestinationDirectory) - 1] == L'/') )
    {
        DestinationDirectory[wcslen(DestinationDirectory) - 1] = L'\0';
    }

    return CopyDirectoryFunction_others(DestinationDirectory, SourceDirectory);

}
/*--------------------------------------------------------------------------*/
/* Copy file with all attributes  */
static int CopyFileFunction_others(wchar_t *DestinationFilename, wchar_t *SourceFilename)
{
    char *pStrDest = wide_string_to_UTF8(DestinationFilename);
    char *pStrSrc = wide_string_to_UTF8(SourceFilename);

    char strDestFullPath[PATH_MAX * 2 + 1];
    char strSrcFullPath[PATH_MAX * 2 + 1];

    int sfd = -1;
    int dfd = -1;
    struct stat st;
    char buf[1024 * 32];
    char *ptr = NULL;
    int nread = 0, nwritten = 0;
    int status = 0;

    get_full_path(strDestFullPath, pStrDest, PATH_MAX * 2);
    get_full_path(strSrcFullPath, pStrSrc, PATH_MAX * 2);

    if (strcmp(strDestFullPath, strSrcFullPath) == 0)
    {
        status = EPERM;
        goto err;
    }

    if ((sfd = open (pStrSrc, O_RDONLY, 0)) < 0)
    {
        status = errno;
        goto err;
    }

    if (fstat (sfd, &st) < 0)
    {
        status = errno;
        goto err;
    }

    if ((dfd = open (pStrDest, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode & 0777)) < 0)
    {
        status = errno;
        goto err;
    }

    while ((nread = read (sfd, buf, sizeof (buf))) > 0)
    {
        ptr = buf;
        while (nread > 0)
        {
            nwritten = write (dfd, ptr, nread);
            if (nwritten <= 0)
            {
                status = errno;
                goto err;
            }
            nread -= nwritten;
            ptr += nwritten;
        }
    }

    if (nread < 0)
    {
        status = errno;
        goto err;
    }

    if (close (dfd) < 0)
    {
        status = errno;
        goto err;
    }

    fchmod (dfd, st.st_mode & 0777);
    close (sfd);
    FREE(pStrDest);
    pStrDest = NULL;
    FREE(pStrSrc);
    pStrSrc = NULL;
    return 0;

err:
    if (sfd >= 0)
    {
        close (sfd);
    }
    if (dfd >= 0)
    {
        close (dfd);
        unlink (pStrDest);
    }

    FREE(pStrDest);
    pStrDest = NULL;
    FREE(pStrSrc);
    pStrSrc = NULL;
    return status;
}
/*--------------------------------------------------------------------------*/
static int CopyDirectoryFunction_others(wchar_t *DestinationDirectory, wchar_t *SourceDirectory)
{
    char *pStrDest = wide_string_to_UTF8(DestinationDirectory);
    char *pStrSrc = wide_string_to_UTF8(SourceDirectory);

    int ierr = 0;

    /* we check directory source exists */
    if (!isdir(pStrSrc))
    {
        if (pStrDest)
        {
            FREE(pStrDest);
            pStrDest = NULL;
        }
        FREE(pStrSrc);
        return ENOTDIR;
    }

    /* we check destination directory exists */
    if (!isdir(pStrDest))
    {
        if (FileExistW(DestinationDirectory))
        {
            FREE(pStrDest);
            FREE(pStrSrc);
            return ENOTDIR;
        }
        else
        {
            if (!createdirectoryW(DestinationDirectory))
            {
                FREE(pStrDest);
                FREE(pStrSrc);
                return ENOTDIR;
            }
        }
    }

    ierr = RecursiveCopyDirectory(pStrDest, pStrSrc);

    FREE(pStrDest);
    FREE(pStrSrc);

    if (ierr)
    {
        return errno;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static int RecursiveCopyDirectory(char *DestinationDir, char *SourceDir)
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir(SourceDir) ;

    if (dir == NULL)
    {
        return errno;
    }

    while ((ent = readdir(dir)) != NULL)
    {
        char *filenameSRC = NULL;
        char *filenameDST = NULL;

        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue ;
        }

        filenameSRC = (char*)MALLOC(sizeof(char) * (strlen(SourceDir) + 1 + strlen(ent->d_name) + 1 + 1)) ;
        filenameDST = (char*)MALLOC(sizeof(char) * (strlen(DestinationDir) + 1 + strlen(ent->d_name) + 1 + 1)) ;

        sprintf(filenameSRC, "%s/%s", SourceDir, ent->d_name);

        if (DestinationDir[strlen(DestinationDir) - 1] == '/')
        {
            sprintf(filenameDST, "%s%s", DestinationDir, ent->d_name);
        }
        else
        {
            sprintf(filenameDST, "%s/%s", DestinationDir, ent->d_name);
        }

        if (isdir(filenameSRC))
        {
            int ierr = 0;
            BOOL bOK = createdirectory(filenameDST);
            if (!bOK)
            {
                FREE(filenameDST);
                FREE(filenameSRC);
                closedir(dir);
                return EACCES;
            }

            ierr = RecursiveCopyDirectory(filenameDST, filenameSRC);
            if (ierr)
            {
                FREE(filenameDST);
                FREE(filenameSRC);
                closedir(dir);
                return ierr;
            }
        }
        else
        {
            wchar_t* wcfileDest = to_wide_string(filenameDST);
            wchar_t* wcfileSrc = to_wide_string(filenameSRC);

            int ierr = CopyFileFunction_others(wcfileDest,  wcfileSrc);
            FREE(wcfileDest);
            FREE(wcfileSrc);

            if (ierr)
            {
                FREE(filenameDST);
                FREE(filenameSRC);
                closedir(dir);
                return ierr;
            }
        }

        FREE(filenameDST);
        FREE(filenameSRC);
    }

    closedir(dir);
    return 0;
}
/*--------------------------------------------------------------------------*/
#endif /* #ifndef _MSC_VER */
/*--------------------------------------------------------------------------*/
