/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#ifndef __TYPES_COMPARAISON_LT_LE_HXX__
#define __TYPES_COMPARAISON_LT_LE_HXX__

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
