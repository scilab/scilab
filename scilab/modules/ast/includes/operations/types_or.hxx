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

#ifndef __TYPES_OR_H__
#define __TYPES_OR_H__

#include "generic_operations.hxx"
#include "types.hxx"
#include "bool.hxx"

// ||
int IntOrInt(types::InternalType* _pL, types::Bool** _pOut);
int BoolOrBool(types::Bool* _pI1, types::Bool** _pOut);
int SparseBoolOrSparseBool(types::InternalType* _pL, types::Bool** _pOut);

// |
int IntLogicalOrInt(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut);
int BoolLogicalOrBool(types::Bool* _pL, types::Bool*  _pR, types::Bool** _pOut);
int SparseBoolLogicalOrSparseBool(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut);

#endif /* __TYPES_OR_H__ */
