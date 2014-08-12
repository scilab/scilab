/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

void fillSubtractFunction();
void fillOppositeFunction();

//define arrays on operation functions
typedef types::InternalType*(*opposite_function)(types::InternalType*);


#define DECLARE_OPPOSITE_PROTO(x) template<class T, class O> inline types::InternalType* x(T *_pL)
DECLARE_OPPOSITE_PROTO(opposite_S);
DECLARE_OPPOSITE_PROTO(opposite_M);


typedef types::InternalType*(*subtract_function)(types::InternalType*, types::InternalType*);

int SubstractDoubleToDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double **_pDoubleOut);
int SubstractPolyToDouble(types::Double *_pDouble, types::Polynom *_pPoly, types::Polynom **_pPolyOut);
int SubstractDoubleToPoly(types::Polynom *_pPoly, types::Double *_pDouble, types::Polynom **_pPolyOut);
int SubstractPolyToPoly(types::Polynom *_pPoly1, types::Polynom *_pPoly2, types::Polynom **_pPolyOut);

//Sparse
int SubstractSparseToSparse(types::Sparse* _pSparse1, types::Sparse* _pSparse2, types::GenericType **_pSparseOut);
int SubstractSparseToDouble(types::Sparse* _pSparse, types::Double* _pDouble, types::GenericType **_pDoubleOut);
int SubstractDoubleToSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::GenericType **_pDoubleOut);

#endif /* __TYPES_SUB_H__ */
