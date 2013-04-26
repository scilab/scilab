/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_COMPARISON_EQUAL_HXX__
#define __TYPES_COMPARISON_EQUAL_HXX__

#include "dynlib_operations.hxx"
#include "generic_operations.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "sparse.hxx"

int EqualToDoubleAndDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::GenericType** _pOut);
int EqualToSparseAndSparse(types::Sparse* _pSparse1, types::Sparse* _pSarse2, types::GenericType** _pOut);
int EqualToDoubleAndSparse(types::Double* _pDouble, types::Sparse* _pSarse, types::GenericType** _pOut);
int EqualToSparseAndDouble(types::Sparse* _pSparse, types::Double* _pDouble, types::GenericType** _pOut);
int EqualToSparseBoolAndSparseBool(types::SparseBool* _pSB1, types::SparseBool* _pSB2, types::GenericType** _pOut);
int EqualToSparseBoolAndBool(types::SparseBool* _pSB1, types::Bool* _pB2, types::GenericType** _pOut);
int EqualToBoolAndSparseBool(types::Bool* _pB1, types::SparseBool* _pSB2, types::GenericType** _pOut);

int EqualToIntAndInt(types::InternalType* _pI1, types::InternalType*  _pI2, types::GenericType** _pOut);

#endif /* !__TYPES_COMPARISON_EQUAL_HXX__ */
