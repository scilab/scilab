/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __DIARY_H__
#define __DIARY_H__

#include "BOOL.h"

/**
* Get File ID descriptor of the diary
* @return File ID
*/
int getdiary(void);

/**
* Diary (writes in a file)
* @param str : string to write
* @param addCR : to add a '\n'
*/
void diary(char *str, BOOL addCR);

/**
* Close diaries
*/
BOOL closeAllDiaries(void);

/**
* Set diary ID
*/
void setDiaryId(int fd);


#endif /* __DIARY_H__ */

