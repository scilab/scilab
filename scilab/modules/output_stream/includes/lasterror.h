/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
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

/* This header must be used ONLY in Scilab */
/* see call_scilab.h for others use cases */

/**
* set last recorded error
* @param[in] new recorded error number
* @param[in] new recorded error message
* @param[in] size of new recorded error message
* @param[in] new line position in function
* @param[in] new function name
* @return 0
*/
OUTPUT_STREAM_IMPEXP int setInternalLastError(int iErr, 
                                      char** strErrorMessage,
                                      int iNbLines,
                                      int iLinePos,
                                      char *strFunctionName);

/**
* clear last error
* @return 0
*/
OUTPUT_STREAM_IMPEXP int clearInternalLastError(void);

/**
* set current last error text buffer
* @param[in] new buffer text
* @param[in] size of new text buffer
* @return 0
*/
OUTPUT_STREAM_IMPEXP int setInternalLastErrorMessage(char** strErrorMessage, int iNbLines);

/**
* get current last error buffer
* @param[out] returns size of returned buffer 
* @return buffer last error (no need to free)
*/
OUTPUT_STREAM_IMPEXP const char** getInternalLastErrorMessage(int *iNbLines);

/**
* Append a line to current last error buffer
* @param[in] strErrorMessage line to append
* @return 0 if OK , 1 if NOT OK
*/
OUTPUT_STREAM_IMPEXP int appendStringToInternalLastErrorMessage(char *strErrorMessage);

/**
* Returns Last Error Value
* @return last error value (int)
*/
OUTPUT_STREAM_IMPEXP int getInternalLastErrorValue(void);

/**
* set Last Error Value
* @param[in] int: new value of last error
* @return int: previous value of last error
*/
OUTPUT_STREAM_IMPEXP int setInternalLastErrorValue(int iVal);

/**
* get last recorded function line position
* @return line position
*/
OUTPUT_STREAM_IMPEXP int getInternalLastErrorLinePosition(void);

/**
* set last recorded function line position
* @param[in] new line position
* @return previous line position
*/
OUTPUT_STREAM_IMPEXP int setInternalLastErrorLinePosition(int iLinePosition);

/**
* get last recorded function name
* @return function name (no need to free)
*/
OUTPUT_STREAM_IMPEXP const char *getInternalLastErrorFunctionName(void);

/**
* set last recorded function name
* @param[in] function name
* @return 0
*/
OUTPUT_STREAM_IMPEXP int setInternalLastErrorFunctionName(char *strFunctionName);

#endif /* __LASTERROR_H__ */
