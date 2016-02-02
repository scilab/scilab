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
#ifndef __DIARY_MANAGER_HXX__
#define __DIARY_MANAGER_HXX__

#include <wchar.h>
#include "DiaryModesEnum.hxx"

extern "C"
{
#include "dynlib_output_stream.h"
}
/**
* get filename of a ID
* @param[in] _Id
* @return filename
*/
OUTPUT_STREAM_IMPEXP wchar_t *getDiaryFilename(int _Id);

/**
* get all filenames in manager
* @param[out] size of returned array
* @return filenames
*/
OUTPUT_STREAM_IMPEXP wchar_t **getDiaryFilenames(int *array_size);

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
OUTPUT_STREAM_IMPEXP double *getDiaryIDsAsDouble(int *array_size);

/**
* close all diaries
* @return 0
*/
OUTPUT_STREAM_IMPEXP int diaryCloseAll(void);

/**
* close a diary by ID
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryClose(int _iId);

/**
* close a diary by filename
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryClose(wchar_t *filename);

/**
* Pause all diaries
* @return 0
*/
OUTPUT_STREAM_IMPEXP int diaryPauseAll(void);

/**
* Pause a diary by ID
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryPause(int _iId);

/**
* Pause a diary by filename
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryPause(wchar_t *filename);

/**
* Resume All diaries
* @return 0
*/
OUTPUT_STREAM_IMPEXP int diaryResumeAll(void);

/**
* Resume a diary by ID
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryResume(int _iId);

/**
* Resume a diary by filename
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryResume(wchar_t *filename);

/**
* check if a ID exists
* @param[in] ID
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryExists(int _iId);

/**
* checks if a filename exists
* @param[in] filename
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryExists(wchar_t *filename);

/**
* create a new diary
* @param[in] filename
* @return ID or -1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryNew(wchar_t *filename, bool autorename);

/**
* append to a diary
* @param[in] filename
* @return ID or -1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diaryAppend(wchar_t *filename);

/*
* Set filter mode (input, output)
* @param[in] ID
* @param[in] mode
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diarySetFilterMode(int _iId, diary_filter mode);

/*
* Set prefix time mode
* @param[in] ID
* @param[in] mode
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diarySetPrefixMode(int ID_diary, diary_prefix_time_format iPrefixMode);

/*
* get prefix mode
* @param[in] ID
* @return prefix mode
*/
OUTPUT_STREAM_IMPEXP int diaryGetPrefixMode(int ID_diary);

/*
* set prefix IO mode filter
* @param[in] ID
* @param[in] mode
* @return 0 (OK) 1 (NOK)
*/
OUTPUT_STREAM_IMPEXP int diarySetPrefixIoModeFilter(int ID_diary, diary_prefix_time_filter mode);

/*
* get prefix IO mode filter
* @param[in] ID
* @return mode
*/
OUTPUT_STREAM_IMPEXP diary_prefix_time_filter diaryGetPrefixIoModeFilter(int ID_diary);

#endif /* __DIARY_MANAGER_HXX__ */
/*--------------------------------------------------------------------------*/
