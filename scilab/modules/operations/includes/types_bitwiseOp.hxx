/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_BITWISEOP_H__
#define __TYPES_BITWISEOP_H__

#include "dynlib_operations.hxx"
#include "types.hxx"

EXTERN_OP int bitwiseAndToIntAndInt(types::InternalType* _pI1, types::InternalType*  _pI2, types::InternalType** _pOut);
template <class K>
int bitwiseAndToIntAndInt(K* _pI1, K* _pI2, types::InternalType** _pOut);

EXTERN_OP int bitwiseOrToIntAndInt(types::InternalType* _pI1, types::InternalType*  _pI2, types::InternalType** _pOut);
template <class K>
int bitwiseOrToIntAndInt(K* _pI1, K* _pI2, types::InternalType** _pOut);

#endif /* __TYPES_BITWISEOP_H__ */