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
#ifndef __DIARY_H__
#define __DIARY_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_output_stream.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
* write a string (wide char) in diaries
* @param[in] string to write
* @param[in] string is a input
*/
OUTPUT_STREAM_IMPEXP int diaryWrite(const wchar_t *wstr, BOOL bInput);

/*
* same as diaryWrite but add carriage return
* @param[in] string to write
* @param[in] string is a input
*/
OUTPUT_STREAM_IMPEXP int diaryWriteln(const wchar_t *wstr, BOOL bInput);

#ifdef  __cplusplus
}
#endif

#endif /* __DIARY_H__ */
/*--------------------------------------------------------------------------*/