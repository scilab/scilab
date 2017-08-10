/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
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


