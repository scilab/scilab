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

/**
*
*/
wchar_t *getDiaryFilename(int _Id);

/**
*
*/
wchar_t **getDiaryFilenames(int *array_size);

/**
*
*/
int *getDiaryIDs(int *array_size);

/**
*
*/
double *getDiaryIDsAsDouble(int *array_size);

/**
*
*/
int diaryCloseAll(void);

/**
*
*/
int diaryClose(int _iId);

/**
*
*/
int diaryClose(wchar_t *filename);

/**
*
*/
int diaryPauseAll(void);

/**
*
*/
int diaryPause(int _iId);

/**
*
*/
int diaryPause(wchar_t *filename);

/**
*
*/
int diaryResumeAll(void);

/**
*
*/
int diaryResume(int _iId);

/**
*
*/
int diaryResume(wchar_t *filename);

/**
*
*/
int diaryExists(int _iId);

/**
*
*/
int diaryExists(wchar_t *filename);

/**
*
*/
int diaryNew(wchar_t *filename);

/**
*
*/
int diaryAppend(wchar_t *filename);

/*
*
*/
int diarySetFilterMode(int _iId, int mode);

/*
*
*/
int diarySetPrefixMode(int ID_diary,int iPrefixMode);

/*
*
*/
int diaryGetPrefixMode(int ID_diary);

/*
*
*/
int diarySetPrefixIoModeFilter(int ID_diary,int mode);

/*
*
*/
int diaryGetPrefixIoModeFilter(int ID_diary);

#endif /* __DIARY_MANAGER_HXX__ */
/*--------------------------------------------------------------------------*/
