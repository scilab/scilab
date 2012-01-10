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
#ifndef __TYPES_MULTIPLICATION_HXX__
#define __TYPES_MULTIPLICATION_HXX__

#include "dynlib_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

using namespace types;

EXTERN_OP types::InternalType *GenericDotTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);
EXTERN_OP types::InternalType *GenericTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int MultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
int MultiplyDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom** _pPolyOut);
int MultiplyPolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom **_pPolyOut);
int MultiplyPolyByPoly(types::Polynom* _pPoly1, types::Polynom* _pPoly2, types::Polynom** _pPolyOut);

int DotMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);

int MultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pSparseOut);
int MultiplyDoubleBySparse(Double* _pDouble, Sparse *_pSparse, GenericType** _pDoubleOut);
int MultiplySparseByDouble(Sparse *_pSparse, Double*_pDouble, GenericType** _pDoubleOut);

int DotMultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pSparseOut);
int DotMultiplyDoubleBySparse(Double* _pDouble1, Sparse* _pSparse2, GenericType**  _pSparseOut);
int DotMultiplySparseByDouble(Sparse* _pSparse1, Double* _pDouble2, GenericType** _pSparseOut);
#endif /* __TYPES_MULTIPLICATION_HXX__ */
