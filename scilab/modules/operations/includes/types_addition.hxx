/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_ADD_H__
#define __TYPES_ADD_H__

#include "operation.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"

using namespace types;

/*
** Try to find a good algorithm to perform addition between those 2 datatypes.
** If none is find, this will return NULL. Overload can then be performed by caller.
** @param _pLeftOperand, the left addition operand
** @param _pRightOperand, the rightt addition operand
** @return addition result
**
*/
EXTERN_OP InternalType* GenericPlus(InternalType *_pLeftOperand, InternalType *_pRightOperand);

EXTERN_OP int AddDoubleToDouble(Double *_pDouble1, Double *_pDouble2, Double** _pDoubleOut);
EXTERN_OP int AddDoubleToPoly(Polynom *_pPoly, Double *_pDouble, Polynom ** _pPolyOut);
EXTERN_OP int AddPolyToPoly(Polynom* pPoly1, Polynom* _pPoly2, Polynom ** _pPolyOut);
EXTERN_OP int AddStringToString(String *_pString1, String *_pString2, String** _pStringOut);

#endif /* __TYPES_ADD_H__ */
