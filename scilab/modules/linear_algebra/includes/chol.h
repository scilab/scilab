/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "doublecomplex.h"
#include "dynlib_linear_algebra.h"

LINEAR_ALGEBRA_IMPEXP int iRealCholProduct(double *_pdblReal, int _iLeadDim);
LINEAR_ALGEBRA_IMPEXP int iComplexCholProduct(doublecomplex *_poIn, int _iLeadDim);
