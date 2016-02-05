/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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
#ifndef __TYPES_KRONECKER_HXX__
#define __TYPES_KRONECKER_HXX__

#include "double.hxx"
#include "dynlib_ast.h"

// DOUBLE .*. DOUBLE
types::InternalType *GenericKrontimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

EXTERN_AST int KroneckerMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);

// DOUBLE ./. DOUBLE
types::InternalType *GenericKronrdivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int KroneckerRDivideDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);

// DOUBLE .\. DOUBLE
types::InternalType *GenericKronldivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int KroneckerLDivideDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
#endif /* __TYPES_KRONECKER_HXX__ */
