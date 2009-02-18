/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michael BAUDIN
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCISPARSE_H__
#define __SCISPARSE_H__

/**
 * Structure used for sparse matrix
 */
typedef struct scisparse {
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
