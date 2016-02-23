/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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
