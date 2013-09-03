/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __FILESMANAGEMENT_H__
#define __FILESMANAGEMENT_H__

#include <stdio.h>
#include "dynlib_fileio.h"
#include "BOOL.h"

/* ID returned if File is not defined */
#define FILE_ID_NOT_DEFINED -100

/**
* Default max of files opened in scilab
*/
#ifdef FOPEN_MAX
#define DEFAULT_MAX_FILES FOPEN_MAX
#else
#define DEFAULT_MAX_FILES 20
#endif
/**
* Get max of files opened in scilab
* @return max of files opened in scilab
*/
FILEIO_IMPEXP int GetMaximumFileOpenedInScilab(void);

/**
* Get the file associated to int Id
* @param Id
* @return FILE ptr
*/
FILEIO_IMPEXP FILE *GetFileOpenedInScilab(int Id);

/**
* Set the file associated to int Id
* @param FILE ptr
* @param Id
*/
FILEIO_IMPEXP void SetFileOpenedInScilab(int Id, FILE *fptr);

/**
* Get the current Id (current file)
* @return Id
*/
FILEIO_IMPEXP int GetCurrentFileId(void);

/**
* Get the previous Id (previous file)
* @return Id
*/
FILEIO_IMPEXP int GetPreviousFileId(void);

/**
* Set the current Id (current file)
* @param Id
*/
FILEIO_IMPEXP void SetCurrentFileId(int Id);

/**
* Get the swap status of file Id
* @param Id
* @return swap status
*/
FILEIO_IMPEXP int GetSwapStatus(int Id);

/**
* Set the swap status of file Id
* @param Id
* @param new swap
*/
FILEIO_IMPEXP void SetSwapStatus(int Id, int newswap);

/**
* Get the mode of file Id
* @param Id
* @return mode
*/
FILEIO_IMPEXP int GetFileModeOpenedInScilab(int Id);

/**
* Set the mode of file Id
* @param Id
* @param new mode
*/
FILEIO_IMPEXP void SetFileModeOpenedInScilab(int Id, int mode);

/**
* Get the type of file Id
* @param Id
* @return Type (Fortran,C)
*/
FILEIO_IMPEXP int GetFileTypeOpenedInScilab(int Id);


/**
* Get the type of file Id
* @param Id
* @return Type 'C' or 'F' (Fortran,C) (to free)
*/
FILEIO_IMPEXP char *GetFileTypeOpenedInScilabAsString(int Id);

/**
* Set the mode of file Id
* @param Id
* @param new Type (Fortran,C)
*/
FILEIO_IMPEXP void SetFileTypeOpenedInScilab(int Id, int Type);

/**
* Get the name of file Id
* @param Id
* @return name
*/
FILEIO_IMPEXP char* GetFileNameOpenedInScilab(int Id);

/**
* Set the name of file Id
* @param Id
* @param new name
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL SetFileNameOpenedInScilab(int Id, char *name);

/**
* Free filename of file Id
* @param Id
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL FreeFileNameOpenedInScilab(int Id);

/**
* Initialize Scilab's file list
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL InitializeScilabFilesList(void);

/**
* Terminate Scilab's file list
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL TerminateScilabFilesList(void);

/**
* Extend Scilab's file list
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL ExtendScilabFilesList(int NewSize);

/**
* Search if filename is already opened in Scilab
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL IsAlreadyOpenedInScilab(char *filename);

/**
* Get ID of a filename opened in scilab
* @param[in] filename
* @return file ID or FILE_ID_NOT_DEFINED
*/
FILEIO_IMPEXP int GetIdFromFilename(char *filename);

/**
* Get Ids opened in Scilab
* @param[out] size of returned array
* @return Array of Id
*/
FILEIO_IMPEXP double *GetFilesIdUsed(int *sizeArrayReturned);

/**
* Get Ids opened in Scilab
* @param[out] size of returned array
* @return Array of Id
*/
FILEIO_IMPEXP double *GetSwapsUsed(int *sizeArrayReturned);

/**
* Get Modes used in Scilab
* @param[out] size of returned array
* @return Modes associated to Id
*/
FILEIO_IMPEXP double *GetModesUsed(int *sizeArrayReturned);

/**
* Get Types used in Scilab
* @param[out] size of returned array
* @return Types associated to Id
*/
FILEIO_IMPEXP int *GetTypesUsed(int *sizeArrayReturned);

/**
* Get Types used in Scilab
* @param[out] size of returned array
* @return Types associated to Id as Strings (user reading)
*/
FILEIO_IMPEXP char **GetTypesUsedAsString(int *sizeArrayReturned);

/**
* Get filenames used in Scilab
* @param[out] size of returned array
* @return filenames
*/
FILEIO_IMPEXP char **GetFilenamesUsed(int *sizeArrayReturned);

/**
* Get number of Ids Used in Scilab
* @return number of Ids
*/
FILEIO_IMPEXP int GetNumberOfIdsUsed(void);

#endif /* __FILESMANAGEMENT_H__ */
/*--------------------------------------------------------------------------*/

