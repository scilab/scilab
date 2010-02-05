/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "PATH_MAX.h"
#include "fileinfo.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "returnanan.h"
#include "expandPathVariable.h"
/*--------------------------------------------------------------------------*/
#define FILEINFO_ARRAY_SIZE 13

#define FILEINFO_TOTAL_SIZE_INDICE 0
#define FILEINFO_MODE_INDICE       1
#define FILEINFO_UID_INDICE        2
#define FILEINFO_GID_INDICE        3
#define FILEINFO_DEV_INDICE        4
#define FILEINFO_MTIME_INDICE      5
#define FILEINFO_CTIME_INDICE      6
#define FILEINFO_ATIME_INDICE      7
#define FILEINFO_RDEV_INDICE       8
#define FILEINFO_BLKSIZE_INDICE    9
#define FILEINFO_BLOCKS_INDICE    10
#define FILEINFO_INO_INDICE       11
#define FILEINFO_NLINK_INDICE     12

#define FILEINFO_DEFAULT_ERROR    -1
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static double *fileinfo_Windows(char *filepathname,int *ierr);
#else
static double *fileinfo_Others(char *filepathname,int *ierr);
#endif
/*--------------------------------------------------------------------------*/
double * fileinfo(char *filename, int *ierr)
{
	char *expandedpath = NULL;
	double *FILEINFO_ARRAY = NULL;
	int out_n = 0;

	if (filename == NULL)
	{
		*ierr = FILEINFO_DEFAULT_ERROR;
		return NULL;
	}

	expandedpath = expandPathVariable(filename);
	if (expandedpath)
	{
#ifdef _MSC_VER
		FILEINFO_ARRAY = fileinfo_Windows(expandedpath,ierr);
#else
		FILEINFO_ARRAY = fileinfo_Others(expandedpath,ierr);
#endif
		FREE(expandedpath);
		expandedpath = NULL;
	}

	return FILEINFO_ARRAY;
}
/*--------------------------------------------------------------------------*/
double * filesinfo(char **filenames, int dim_filenames, int *ierrs)
{
	double *FILES_INFO_ARRAY = NULL;

	if (dim_filenames > 0)
	{
		int i = 0;
		int j = 0;
		FILES_INFO_ARRAY = (double*)MALLOC(sizeof(double ) * (dim_filenames * FILEINFO_ARRAY_SIZE));
		for (i = 0; i < dim_filenames; i++)
		{
			int k = 0;
			int ierr = 0;
			double * FILEINFO_ARRAY = fileinfo(filenames[i],&ierr);
			if (FILEINFO_ARRAY == NULL)
			{
				FILEINFO_ARRAY = (double*)MALLOC(sizeof(double)*FILEINFO_ARRAY_SIZE);

				FILEINFO_ARRAY[FILEINFO_TOTAL_SIZE_INDICE] = C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_MODE_INDICE] = C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_UID_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_GID_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_DEV_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_MTIME_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_CTIME_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_ATIME_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_RDEV_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_BLKSIZE_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_BLOCKS_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_INO_INDICE] =  C2F(returnanan)();
				FILEINFO_ARRAY[FILEINFO_NLINK_INDICE] =  C2F(returnanan)();
			}

			for (k = 0;k < FILEINFO_ARRAY_SIZE; k++)
			{
				FILES_INFO_ARRAY[j+k] = FILEINFO_ARRAY[k];
			}
			j = j + FILEINFO_ARRAY_SIZE;
			FREE(FILEINFO_ARRAY);
			FILEINFO_ARRAY = NULL;
			ierrs[i] = ierr;
		}
	}
	return FILES_INFO_ARRAY;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static double *fileinfo_Windows(char *filepathname,int *ierr)
{
	struct _stat buf;
	wchar_t* wcpath = to_wide_string(filepathname);
	wchar_t DriveTemp[PATH_MAX + FILENAME_MAX + 1];

	double *FILEINFO_ARRAY = NULL;
	int result = 0;

	*ierr = 0;

	if (wcpath == NULL)
	{
		*ierr = FILEINFO_DEFAULT_ERROR;
		return NULL;
	}

	swprintf(DriveTemp, wcslen(wcpath)+1, L"%s", wcpath);
	if ( (DriveTemp[wcslen(DriveTemp)-1] == L'/') || (DriveTemp[wcslen(DriveTemp)-1] == L'\\') )
	{
		DriveTemp[wcslen(DriveTemp)-1] = L'\0';
	}

	result = _wstat(DriveTemp, &buf );

	FREE(wcpath); wcpath = NULL;

	if( result != 0 ) 
	{
		if ( (wcslen(DriveTemp) == 2) ||(wcslen(DriveTemp) == 3) )
		{
			UINT DriveType = GetDriveTypeW(DriveTemp);
			if ( (DriveType == DRIVE_UNKNOWN) || (DriveType == DRIVE_NO_ROOT_DIR) )
			{
				*ierr = result;
				return NULL;
			}
			else
			{
				FILEINFO_ARRAY = (double*)MALLOC(sizeof(double)*FILEINFO_ARRAY_SIZE);
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
					*ierr = result;
				}
				else
				{
					*ierr = FILEINFO_DEFAULT_ERROR;
					return NULL;
				}
			}
		}
		else
		{
			*ierr = result;
			return NULL;
		}
	}
	else
	{
		FILEINFO_ARRAY = (double*)MALLOC(sizeof(double)*FILEINFO_ARRAY_SIZE);
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
			FILEINFO_ARRAY[FILEINFO_BLKSIZE_INDICE] = 0;/* blocksize for filesystem I/O */
			FILEINFO_ARRAY[FILEINFO_BLOCKS_INDICE] = 0;/* number of blocks allocated */
			FILEINFO_ARRAY[FILEINFO_INO_INDICE] = (double) buf.st_ino;/* inode */
			FILEINFO_ARRAY[FILEINFO_NLINK_INDICE] = (double) buf.st_nlink;/* number of hard links */
			*ierr = result;
		}
		else
		{
			*ierr = FILEINFO_DEFAULT_ERROR;
			return NULL;
		}
	}
	return FILEINFO_ARRAY;
}
/*--------------------------------------------------------------------------*/
#else
static double *fileinfo_Others(char *filepathname,int *ierr)
{
	struct stat buf;
	double *FILEINFO_ARRAY = NULL;
	int result = 0;

	*ierr = 0;

	result = stat(filepathname, &buf );
	if (result == 0)
	{
		FILEINFO_ARRAY = (double*)MALLOC(sizeof(double)*FILEINFO_ARRAY_SIZE);
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
			*ierr = FILEINFO_DEFAULT_ERROR;
			return NULL;
		}
	}
	*ierr = result;
	return FILEINFO_ARRAY;
}
#endif
/*--------------------------------------------------------------------------*/
