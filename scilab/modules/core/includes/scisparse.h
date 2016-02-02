/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michael BAUDIN
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

#ifndef __SCISPARSE_H__
#define __SCISPARSE_H__

/**
 * Structure used for sparse matrix
 */
typedef struct scisparse
{
    int m;
    int n;
    int it;
    int nel; /**< number of non nul elements */
    int *mnel;/**< mnel[i]: number of non nul elements of row i, size m */
    int *icol; /**< icol[j]: column of the j-th non nul element, size nel */
    double *R; /**< R[j]: real value of the j-th non nul element, size nel */
    double *I ; /**< I[j]: imag value of the j-th non nul element, size nel */
} SciSparse ;

#endif /* __SCISPARSE_H__ */
