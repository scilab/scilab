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

#include "dynlib_operations.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"

/*
** Try to find a good algorithm to perform addition between those 2 datatypes.
** If none is find, this will return NULL. Overload can then be performed by caller.
** @param _pLeftOperand, the left addition operand
** @param _pRightOperand, the rightt addition operand
** @return addition result
**
*/
EXTERN_OP types::InternalType* GenericPlus(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int AddDoubleToDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double** _pDoubleOut);
int AddDoubleToPoly(types::Polynom *_pPoly, types::Double *_pDouble, types::Polynom ** _pPolyOut);
int AddPolyToPoly(types::Polynom* pPoly1, types::Polynom* _pPoly2, types::Polynom ** _pPolyOut);
int AddStringToString(types::String *_pString1, types::String *_pString2, types::String** _pStringOut);

#endif /* __TYPES_ADD_H__ */
