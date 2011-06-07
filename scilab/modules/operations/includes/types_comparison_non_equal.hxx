/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_COMPARISON_NON_EQUAL_HXX__
#define __TYPES_COMPARISON_NON_EQUAL_HXX__

#include "dynlib_operations.hxx"
#include "internal.hxx"

/*
** Try to find a good algorithm to perform non equal comparison between those 2 datatypes.
** If none is find, this will return NULL. Overload can then be performed by caller.
** @param _pLeftOperand, the left comparison operand
** @param _pRightOperand, the rightt comparison operand
** @return comparison result
**
*/
EXTERN_OP types::InternalType* GenericComparisonNonEqual(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

#endif /* !__TYPES_COMPARISON_NON_EQUAL_HXX__ */
