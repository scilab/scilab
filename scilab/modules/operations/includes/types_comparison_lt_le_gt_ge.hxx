/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#ifndef __TYPES_COMPARAISON_LT_LE_HXX__
#define __TYPES_COMPARAISON_LT_LE_HXX__

#include "dynlib_operations.hxx"
#include "generic_operations.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "bool.hxx"

int DoubleLessDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Bool** _pOut);
int DoubleLessEqualDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Bool** _pOut);

int DoubleLessSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::SparseBool** _pOut);
int DoubleLessEqualSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::SparseBool** _pOut);
int SparseLessDouble(types::Sparse* _pSparse, types::Double* _pDouble, types::SparseBool** _pOut);
int SparseLessEqualDouble(types::Sparse* _pSparse, types::Double* _pDouble, types::SparseBool** _pOut);

int SparseLessSparse(types::Sparse* _pSparse1, types::Sparse* _pSparse2, types::SparseBool** _pOut);
int SparseLessEqualSparse(types::Sparse* _pSparse1, types::Sparse* _pSparse2, types::SparseBool** _pOut);

int IntLessInt(types::InternalType* _pI1, types::InternalType*  _pI2, types::GenericType** _pOut);
int IntLessEqualInt(types::InternalType* _pI1, types::InternalType*  _pI2, types::GenericType** _pOut);


#endif /* !__TYPES_COMPARAISON_LT_LE_HXX__ */
