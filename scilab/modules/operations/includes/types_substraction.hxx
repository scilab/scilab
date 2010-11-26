/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_SUB_H__
#define __TYPES_SUB_H__

#include "operation.hxx"
#include "alltypes.hxx"
#include "sparse.hxx"

using namespace types;

EXTERN_OP int SubstractDoubleToDouble(Double* _pDouble1, Double* _pDouble2, Double **_pDoubleOut);
EXTERN_OP int SubstractSparseToSparse(Sparse* _pSparse1, Sparse* _pSparse2, GenericType **_pSparseOut);
EXTERN_OP int SubstractSparseToDouble(Sparse* _pSparse, Double* _pDouble, GenericType **_pDoubleOut);
EXTERN_OP int SubstractDoubleToSparse(Double* _pDouble, Sparse* _pSparse, GenericType **_pDoubleOut);
EXTERN_OP int SubstractPolyToDouble(Double *_pDouble, MatrixPoly *_pPoly, MatrixPoly **_pPolyOut);
EXTERN_OP int SubstractDoubleToPoly(MatrixPoly *_pPoly, Double *_pDouble, MatrixPoly **_pPolyOut);
EXTERN_OP int SubstractPolyToPoly(MatrixPoly *_pPoly1, MatrixPoly *_pPoly2, MatrixPoly **_pPolyOut);

#endif /* __TYPES_SUB_H__ */
