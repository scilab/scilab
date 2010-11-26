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

#ifndef __TYPES_ADD_H__
#define __TYPES_ADD_H__

#include "alltypes.hxx"
#include "operation.hxx"
#include "sparse.hxx"


using namespace types;

EXTERN_OP int AddDoubleToDouble(Double *_pDouble1, Double *_pDouble2, Double** _pDoubleOut);
EXTERN_OP int AddSparseToSparse(Sparse *_pSparse1, Sparse *_pSparse2, GenericType** _pSparseOut);
EXTERN_OP int AddSparseToDouble(Sparse *_pSparse,  Double* _pDouble, GenericType** _pDoubleOut);
EXTERN_OP int AddDoubleToSparse(Double* _pDouble, Sparse* _pSparse, GenericType** _pDoubleOut);
EXTERN_OP int AddDoubleToPoly(MatrixPoly *_pPoly, Double *_pDouble, MatrixPoly ** _pPolyOut);
EXTERN_OP int AddPolyToPoly(MatrixPoly* pPoly1, MatrixPoly* _pPoly2, MatrixPoly ** _pPolyOut);
EXTERN_OP int AddStringToString(String *_pString1, String *_pString2, String** _pStringOut);

#endif /* __TYPES_ADD_H__ */
