/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FILESMANAGEMENT_H__
#define __FILESMANAGEMENT_H__

#include <stdio.h>
#include "machine.h"


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
int GetMaximumFileOpenedInScilab(void);

/**
* Get the file associated to int Id
* @param Id
* @return FILE ptr
*/
FILE *GetFileOpenedInScilab(int Id);

/**
* Set the file associated to int Id
* @param FILE ptr
* @param Id
*/
void SetFileOpenedInScilab(int Id,FILE *fptr);

/**
* Get the current Id (current file)
* @return Id
*/
int GetCurrentFileId(void);

/**
* Set the current Id (current file)
* @param Id
*/
void SetCurrentFileId(int Id);

/**
* Get the swap status of file Id
* @param Id
* @return swap status
*/
int GetSwapStatus(integer Id);

/**
* Set the swap status of file Id
* @param Id
* @param new swap
*/
void SetSwapStatus(int Id,int newswap);

/**
* Get the mode of file Id
* @param Id
* @return mode
*/
int GetFileModeOpenedInScilab(int Id);

/**
* Set the mode of file Id
* @param Id
* @param new mode
*/
void SetFileModeOpenedInScilab(int Id,int mode);

/**
* Get the type of file Id
* @param Id
* @return Type (Fortran,C)
*/
int GetFileTypeOpenedInScilab(int Id);

/**
* Set the mode of file Id
* @param Id
* @param new Type (Fortran,C)
*/
void SetFileTypeOpenedInScilab(int Id,int Type);

/**
* Get the name of file Id
* @param Id
* @return name
*/
char* GetFileNameOpenedInScilab(int Id);

/**
* Set the name of file Id
* @param Id
* @param new name
* @return TRUE or FALSE
*/
BOOL SetFileNameOpenedInScilab(int Id,char *name);

/**
* Free filename of file Id
* @param Id
* @return TRUE or FALSE
*/
BOOL FreeFileNameOpenedInScilab(int Id);

/**
* Initialize Scilab's file list
* @return TRUE or FALSE
*/
BOOL InitializeScilabFilesList(void);

/**
* Terminate Scilab's file list
* @return TRUE or FALSE
*/
BOOL TerminateScilabFilesList(void);

/**
* Extend Scilab's file list
* @return TRUE or FALSE
*/
BOOL ExtendScilabFilesList(int NewSize);

/**
* Search if filename is already opened in Scilab
* @return TRUE or FALSE
*/
BOOL IsAlreadyOpenedInScilab(char *filename);
#endif /* __FILESMANAGEMENT_H__ */
/*--------------------------------------------------------------------------*/

