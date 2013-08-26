/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*----------------------------------------------------------------------------*/
#ifndef __STRSPLITFUNCTION_H__
#define __STRSPLITFUNCTION_H__

#include <wchar.h>

typedef enum
{
    STRSPLIT_NO_ERROR = 0,
    STRSPLIT_INCORRECT_VALUE_ERROR = 1,
    STRSPLIT_INCORRECT_ORDER_ERROR = 2,
    STRSPLIT_MEMORY_ALLOCATION_ERROR = 3
} strsplit_error;

/**
* split a string into substrings
* @param[in] string to split
* @param[in] indices where it is splitted
* @param[in] size of array indices
* @param[out] error code
* @return substrings
*/
wchar_t **strsplit(wchar_t * wcstringToSplit, double *indices, int sizeIndices, strsplit_error *ierr);

#endif /* __STRSPLITFUNCTION_H__ */
/*----------------------------------------------------------------------------*/
