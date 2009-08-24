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
#ifndef __DIARY_MANAGER_HXX__
#define __DIARY_MANAGER_HXX__

#include <wchar.h>
#include "DiaryModesEnum.hxx"

/**
* get filename of a ID
* @param[in] _Id
* @return filename
*/
wchar_t *getDiaryFilename(int _Id);

/**
* get all filenames in manager
* @param[out] size of returned array
* @return filenames
*/
wchar_t **getDiaryFilenames(int *array_size);

/**
* get all IDs in manager
* @param[out] size of returned array
* @return IDs
*/
int *getDiaryIDs(int *array_size);

/**
* get all IDs in manager
* @param[out] size of returned array
* @return Ids (as double)
*/
double *getDiaryIDsAsDouble(int *array_size);

/**
* close all diaries
* @return 0
*/
int diaryCloseAll(void);

/**
* close a diary by ID
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
int diaryClose(int _iId);

/**
* close a diary by filename
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
int diaryClose(wchar_t *filename);

/**
* Pause all diaries
* @return 0
*/
int diaryPauseAll(void);

/**
* Pause a diary by ID
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
int diaryPause(int _iId);

/**
* Pause a diary by filename
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
int diaryPause(wchar_t *filename);

/**
* Resume All diaries
* @return 0
*/
int diaryResumeAll(void);

/**
* Resume a diary by ID
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
int diaryResume(int _iId);

/**
* Resume a diary by filename
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
int diaryResume(wchar_t *filename);

/**
* check if a ID exists
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
int diaryExists(int _iId);

/**
* checks if a filename exists
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
int diaryExists(wchar_t *filename);

/**
* create a new diary
* @param[in] filename
* @return ID or -1 (NOK)
*/
int diaryNew(wchar_t *filename, bool autorename);

/**
* append to a diary
* @param[in] filename
* @return ID or -1 (NOK)
*/
int diaryAppend(wchar_t *filename);

/*
* Set filter mode (input, output)
* @param[in] ID
* @param[in] mode
* @return 0 (OK) 1 (NOK)
*/
int diarySetFilterMode(int _iId, diary_filter mode);

/*
* Set prefix time mode 
* @param[in] ID
* @param[in] mode
* @return 0 (OK) 1 (NOK)
*/
int diarySetPrefixMode(int ID_diary,diary_prefix_time_format iPrefixMode);

/*
* get prefix mode
* @param[in] ID
* @return prefix mode
*/
int diaryGetPrefixMode(int ID_diary);

/*
* set prefix IO mode filter
* @param[in] ID
* @param[in] mode
* @return 0 (OK) 1 (NOK)
*/
int diarySetPrefixIoModeFilter(int ID_diary,diary_prefix_time_filter mode);

/*
* get prefix IO mode filter
* @param[in] ID
* @return mode 
*/
diary_prefix_time_filter diaryGetPrefixIoModeFilter(int ID_diary);

#endif /* __DIARY_MANAGER_HXX__ */
/*--------------------------------------------------------------------------*/
