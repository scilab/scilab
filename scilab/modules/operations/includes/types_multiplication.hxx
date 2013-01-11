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

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

int MultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
int MultiplyDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom** _pPolyOut);
int MultiplyPolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom **_pPolyOut);
int MultiplyPolyByPoly(types::Polynom* _pPoly1, types::Polynom* _pPoly2, types::Polynom** _pPolyOut);

int DotMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);

int MultiplySparseBySparse(types::Sparse* _pSparse1, types::Sparse* _pSparse2, types::Sparse** _pSparseOut);
int MultiplyDoubleBySparse(types::Double* _pDouble, types::Sparse *_pSparse, types::GenericType** _pDoubleOut);
int MultiplySparseByDouble(types::Sparse *_pSparse, types::Double*_pDouble, types::GenericType** _pDoubleOut);

int DotMultiplySparseBySparse(types::Sparse* _pSparse1, types::Sparse* _pSparse2, types::Sparse** _pSparseOut);
int DotMultiplyDoubleBySparse(types::Double* _pDouble1, types::Sparse* _pSparse2, types::GenericType**  _pSparseOut);
int DotMultiplySparseByDouble(types::Sparse* _pSparse1, types::Double* _pDouble2, types::GenericType** _pSparseOut);

int DotMultiplyDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom**  _pPolyOut);
int DotMultiplyPolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom**  _pPolyOut);
int DotMultiplyPolyByPoly(types::Polynom* _pPoly1, types::Polynom* _pPoly2, types::Polynom**  _pPolyOut);
#endif /* __TYPES_MULTIPLICATION_HXX__ */
