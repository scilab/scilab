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
#ifndef __TYPES_KRONECKER_HXX__
#define __TYPES_KRONECKER_HXX__

#include "dynlib_operations.hxx"
#include "double.hxx"

// DOUBLE .*. DOUBLE
EXTERN_OP types::InternalType *GenericKrontimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

EXTERN_OP int KroneckerMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);

// DOUBLE ./. DOUBLE
EXTERN_OP types::InternalType *GenericKronrdivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int KroneckerRDivideDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);

// DOUBLE .\. DOUBLE
EXTERN_OP types::InternalType *GenericKronldivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int KroneckerLDivideDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
#endif /* __TYPES_KRONECKER_HXX__ */
