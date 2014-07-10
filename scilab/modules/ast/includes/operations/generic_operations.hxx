/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GENERIC_OPERATIONS_HXX__
#define __GENERIC_OPERATIONS_HXX__

#include "types.hxx"
/*
** Try to find a good algorithm to perform operation between those 2 datatypes.
** If none is find, this will return NULL. Overload can then be performed by caller.
** @param _pLeftOperand, the left operand
** @param _pRightOperand, the right operand
** @return the operation result
**
*/

// +
types::InternalType* GenericPlus(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// -
types::InternalType* GenericUnaryMinus(types::InternalType *_pRightOperand);

// -
types::InternalType* GenericMinus(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// /
types::InternalType *GenericRDivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// "\"
types::InternalType *GenericLDivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// ./
types::InternalType *GenericDotRDivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// ".\"
types::InternalType *GenericDotLDivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);


// .*
types::InternalType *GenericDotTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// *
types::InternalType *GenericTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// ^
types::InternalType* GenericPower(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// .^
types::InternalType* GenericDotPower(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// ==
types::InternalType* GenericComparisonEqual(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// <
types::InternalType *GenericLess(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// <=
types::InternalType *GenericLessEqual(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// >
types::InternalType *GenericGreater(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// >=
types::InternalType *GenericGreaterEqual(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// <>
types::InternalType* GenericComparisonNonEqual(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// .*.
types::InternalType* GenericKrontimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// ./.
types::InternalType* GenericKronrdivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// .\.
types::InternalType* GenericKronldivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// &&
types::InternalType* GenericShortcutAnd(types::InternalType *_pLeftOperand);

// &
types::InternalType* GenericLogicalAnd(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

// ||
types::InternalType* GenericShortcutOr(types::InternalType *_pLeftOperand);

// |
types::InternalType* GenericLogicalOr(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

#endif /* !__GENERIC_OPERATIONS_HXX__ */


