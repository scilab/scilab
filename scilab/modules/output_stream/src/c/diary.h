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
#ifndef __DIARY_H__
#define __DIARY_H__

#include <wchar.h>
#include "BOOL.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
* write a string (wide char) in diaries
* @param[in] string to write
* @param[in] string is a input
*/
int diaryWrite(wchar_t *wstr, BOOL bInput);

/*
* same as diaryWrite but add carriage return
* @param[in] string to write
* @param[in] string is a input
*/
int diaryWriteln(wchar_t *wstr, BOOL bInput);

#ifdef  __cplusplus
}
#endif

#endif /* __DIARY_H__ */
/*--------------------------------------------------------------------------*/