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
/*----------------------------------------------------------------------------*/
#ifndef __STRSPLITFUNCTION_H__
#define __STRSPLITFUNCTION_H__

#include <wchar.h>
#include "dynlib_string.h"

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
STRING_IMPEXP wchar_t **strsplit(wchar_t * wcstringToSplit, double *indices, int sizeIndices, strsplit_error *ierr);

#endif /* __STRSPLITFUNCTION_H__ */
/*----------------------------------------------------------------------------*/
