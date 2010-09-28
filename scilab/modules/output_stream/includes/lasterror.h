/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __LASTERROR_H__
#define __LASTERROR_H__

#include "dynlib_output_stream.h"

/**
* set last recorded error
* @param[in] new recorded error number
* @param[in] new recorded error message
* @param[in] size of new recorded error message
* @param[in] new line position in function
* @param[in] new function name
* @return 0
*/
OUTPUT_STREAM_IMPEXP int setLastError(int iErr, 
                                      char** strErrorMessage,
                                      int iNbLines,
                                      int iLinePos,
                                      char *strFunctionName);

/**
* clear last error
* @return 0
*/
OUTPUT_STREAM_IMPEXP int clearLastError(void);

/**
* set current last error text buffer
* @param[in] new buffer text
* @param[in] size of new text buffer
* @return 0
*/
OUTPUT_STREAM_IMPEXP int setLastErrorMessage(char** strErrorMessage, int iNbLines);

/**
* get current last error buffer
* @param[out] returns size of returned buffer 
* @return buffer last error (no need to free)
*/
OUTPUT_STREAM_IMPEXP const char** getLastErrorMessage(int *iNbLines);

/**
* Append a line to current last error buffer
* @param[in] strErrorMessage line to append
* @return 0 if OK , 1 if NOT OK
*/
OUTPUT_STREAM_IMPEXP int appendStringToLastErrorMessage(char *strErrorMessage);

/**
* Returns Last Error Value
* @return last error value (int)
*/
OUTPUT_STREAM_IMPEXP int getLastErrorValue(void);

/**
* set Last Error Value
* @param[in] int: new value of last error
* @return int: previous value of last error
*/
OUTPUT_STREAM_IMPEXP int setLastErrorValue(int iVal);

/**
* get last recorded function line position
* @return line position
*/
OUTPUT_STREAM_IMPEXP int getLastErrorLinePosition(void);

/**
* set last recorded function line position
* @param[in] new line position
* @return previous line position
*/
OUTPUT_STREAM_IMPEXP int setLastErrorLinePosition(int iLinePosition);

/**
* get last recorded function name
* @return function name (no need to free)
*/
OUTPUT_STREAM_IMPEXP const char *getLastErrorFunctionName(void);

/**
* set last recorded function name
* @param[in] function name
* @return 0
*/
OUTPUT_STREAM_IMPEXP int setLastErrorFunctionName(char *strFunctionName);

#endif /* __LASTERROR_H__ */
