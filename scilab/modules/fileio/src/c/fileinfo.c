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
#endif
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "PATH_MAX.h"
#include "fileinfo.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "returnanan.h"
#include "os_string.h"
#include "expandPathVariable.h"

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static double* fileinfo_Windows(const char* _pstFilename, int* _piErr);
#else
static double* fileinfo_Others(const char* _pstFilename, int* _piErr);
#endif
/*--------------------------------------------------------------------------*/
double* filesinfo(char** _pstFilename, int _iSize, int* _piErr)
{
    int i = 0, j = 0;
    double *FILES_INFO_ARRAY = (double*)MALLOC(sizeof(double ) * _iSize * FILEINFO_ARRAY_SIZE);
    for (i = 0 ; i < _iSize ; i++)
    {
        char* pstExp = expandPathVariable(_pstFilename[i]);

        if (pstExp)
        {
            double *FILEINFO_ARRAY = NULL;
#ifdef _MSC_VER
            FILEINFO_ARRAY = fileinfo_Windows(pstExp, &_piErr[i]);
#else
            FILEINFO_ARRAY = fileinfo_Others(pstExp, &_piErr[i]);
#endif
            if (FILEINFO_ARRAY != NULL)
            {
                for (j = 0 ; j < FILEINFO_ARRAY_SIZE ; j++)
                {
                    FILES_INFO_ARRAY[i + j * _iSize] = FILEINFO_ARRAY[j];
                }
            }
            else
            {
                for (j = 0 ; j < FILEINFO_ARRAY_SIZE ; j++)
                {
                    FILES_INFO_ARRAY[i + j * _iSize] = C2F(returnanan)();
                }
            }
            FREE(FILEINFO_ARRAY);
            FREE(pstExp);
        }
    }
    return FILES_INFO_ARRAY;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static double *fileinfo_Windows(const char* _pstFilename, int *_piErr)
{
    struct _stat buf;
    char DriveTemp[PATH_MAX + FILENAME_MAX + 1];

    double *FILEINFO_ARRAY = NULL;
    int result = 0;

    *_piErr = 0;

    if (_pstFilename == NULL)
    {
        *_piErr = FILEINFO_DEFAULT_ERROR;
        return NULL;
    }

    os_sprintf(DriveTemp, PATH_MAX + FILENAME_MAX + 1, "%ls", _pstFilename);
    if ((DriveTemp[strlen(DriveTemp) - 1] == '/') || (DriveTemp[strlen(DriveTemp) - 1] == '\\'))
    {
        DriveTemp[strlen(DriveTemp) - 1] = '\0';
    }

    result = _stat(DriveTemp, &buf);

    if (result != 0)
    {
        if ((strlen(DriveTemp) == 2) || (strlen(DriveTemp) == 3))
        {
            UINT DriveType = GetDriveTypeA(DriveTemp);
            if ((DriveType == DRIVE_UNKNOWN) || (DriveType == DRIVE_NO_ROOT_DIR))
            {
                *_piErr = result;
                return NULL;
            }
            else
            {
                FILEINFO_ARRAY = (double*)MALLOC(sizeof(double) * FILEINFO_ARRAY_SIZE);
                if (FILEINFO_ARRAY)
                {
                    FILEINFO_ARRAY[FILEINFO_TOTAL_SIZE_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_MODE_INDICE] = 16895; /* default file mode for a directory on Windows 40777*/
                    FILEINFO_ARRAY[FILEINFO_UID_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_GID_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_DEV_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_MTIME_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_CTIME_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_ATIME_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_RDEV_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_BLKSIZE_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_BLOCKS_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_INO_INDICE] = 0;
                    FILEINFO_ARRAY[FILEINFO_NLINK_INDICE] = 0;
                    *_piErr = result;
                }
                else
                {
                    *_piErr = FILEINFO_DEFAULT_ERROR;
                    FREE(FILEINFO_ARRAY);
                    return NULL;
                }
            }
        }
        else
        {
            *_piErr = result;
            FREE(FILEINFO_ARRAY);
            return NULL;
        }
    }
    else
    {
        FILEINFO_ARRAY = (double*)MALLOC(sizeof(double) * FILEINFO_ARRAY_SIZE);
        if (FILEINFO_ARRAY)
        {
            FILEINFO_ARRAY[FILEINFO_TOTAL_SIZE_INDICE] = (double)buf.st_size;/* total size, in bytes */
            FILEINFO_ARRAY[FILEINFO_MODE_INDICE] = (double)buf.st_mode;/* protection */
            FILEINFO_ARRAY[FILEINFO_UID_INDICE] = (double)buf.st_uid;/* user ID of owner */
            FILEINFO_ARRAY[FILEINFO_GID_INDICE] = (double)buf.st_gid;/* group ID of owner */
            FILEINFO_ARRAY[FILEINFO_DEV_INDICE] = (double)buf.st_dev;/* device */
            FILEINFO_ARRAY[FILEINFO_MTIME_INDICE] = (double)buf.st_mtime;/* time of last modification */
            FILEINFO_ARRAY[FILEINFO_CTIME_INDICE] = (double)buf.st_ctime;/* time of last change */
            FILEINFO_ARRAY[FILEINFO_ATIME_INDICE] = (double)buf.st_atime;/* time of last access */
            FILEINFO_ARRAY[FILEINFO_RDEV_INDICE] = (double)buf.st_rdev;/* device type (if inode device) */
            FILEINFO_ARRAY[FILEINFO_BLKSIZE_INDICE] = 0;/* blocksize for filesystem I/O */
            FILEINFO_ARRAY[FILEINFO_BLOCKS_INDICE] = 0;/* number of blocks allocated */
            FILEINFO_ARRAY[FILEINFO_INO_INDICE] = (double)buf.st_ino;/* inode */
            FILEINFO_ARRAY[FILEINFO_NLINK_INDICE] = (double)buf.st_nlink;/* number of hard links */
            *_piErr = result;
        }
        else
        {
            *_piErr = FILEINFO_DEFAULT_ERROR;
            FREE(FILEINFO_ARRAY);
            return NULL;
        }
    }
    return FILEINFO_ARRAY;
}
/*--------------------------------------------------------------------------*/
#else
static double* fileinfo_Others(const char* _pstFilename, int* _piErr)
{
    struct stat buf;
    double *FILEINFO_ARRAY = NULL;
    int result = 0;

    *_piErr = 0;

    result = stat(_pstFilename, &buf );
    if (result == 0)
    {
        FILEINFO_ARRAY = (double*)MALLOC(sizeof(double) * FILEINFO_ARRAY_SIZE);
        if (FILEINFO_ARRAY)
        {
            FILEINFO_ARRAY[FILEINFO_TOTAL_SIZE_INDICE] = (double) buf.st_size;/* total size, in bytes */
            FILEINFO_ARRAY[FILEINFO_MODE_INDICE] = (double) buf.st_mode;/* protection */
            FILEINFO_ARRAY[FILEINFO_UID_INDICE] = (double) buf.st_uid;/* user ID of owner */
            FILEINFO_ARRAY[FILEINFO_GID_INDICE] = (double) buf.st_gid;/* group ID of owner */
            FILEINFO_ARRAY[FILEINFO_DEV_INDICE] = (double) buf.st_dev;/* device */
            FILEINFO_ARRAY[FILEINFO_MTIME_INDICE] = (double) buf.st_mtime;/* time of last modification */
            FILEINFO_ARRAY[FILEINFO_CTIME_INDICE] = (double) buf.st_ctime;/* time of last change */
            FILEINFO_ARRAY[FILEINFO_ATIME_INDICE] = (double) buf.st_atime;/* time of last access */
            FILEINFO_ARRAY[FILEINFO_RDEV_INDICE] = (double) buf.st_rdev;/* device type (if inode device) */
            FILEINFO_ARRAY[FILEINFO_BLKSIZE_INDICE] = (double) buf.st_blksize;/* blocksize for filesystem I/O */
            FILEINFO_ARRAY[FILEINFO_BLOCKS_INDICE] = (double) buf.st_blocks;/* number of blocks allocated */
            FILEINFO_ARRAY[FILEINFO_INO_INDICE] = (double) buf.st_ino;/* inode */
            FILEINFO_ARRAY[FILEINFO_NLINK_INDICE] = (double) buf.st_nlink;/* number of hard links */
        }
        else
        {
            *_piErr = FILEINFO_DEFAULT_ERROR;
            return NULL;
        }
    }
    *_piErr = result;
    return FILEINFO_ARRAY;
}
#endif
/*--------------------------------------------------------------------------*/
