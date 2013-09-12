/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __GETRANGE_H__
#define __GETRANGE_H__

#include "doublecomplex.h"
#include "complex_array.h"

#define SIZE_RANGE_SUPPORTED 4

#ifdef __cplusplus
extern "C" {
#endif

    // Extract the strings in pStrsValues which are in the range.
    char **getRangeAsString(const char **pStrsValues,
                            int nbRows, int nbCols,
                            const int *iRange,
                            int *returnedNbRows, int *returnedNbCols);


    // Extract the double complexes in pComplex which are in the range.
    complexArray *getRangeAsComplexArray(const complexArray *pComplex,
                                         int nbRows, int nbCols,
                                         const int *iRange,
                                         int *returnedNbRows, int *returnedNbCols);


    // Returns 1 if the range is valid, returns 0 if not.
    int isValidRange(const int *range, int sizeArray);

#ifdef __cplusplus
}
#endif


#endif /* __GETRANGE_H__ */
// =============================================================================
