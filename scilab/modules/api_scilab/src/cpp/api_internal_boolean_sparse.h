/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#ifndef __INTERNAL_BOOLEAN_SPARSE_API__
#define __INTERNAL_BOOLEAN_SPARSE_API__

//internal functions
//internal boolean sparse functions
SciErr fillBooleanSparseMatrix(void* _pvCtx, int *_piAddress, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);

#endif /* __INTERNAL_BOOLEAN_SPARSE_API__ */
