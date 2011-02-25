/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_DIVIDE_HXX__
#define __TYPES_DIVIDE_HXX__

#include "operation.hxx"
#include "double.hxx"
#include "polynom.hxx"

EXTERN_OP types::InternalType *GenericRDivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

EXTERN_OP int DivideDoubleByDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double **_pDoubleOut);
EXTERN_OP int DividePolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom** _pPolyOut);
EXTERN_OP int DivideDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom** _pPolyOut);


#endif /* !__TYPES_DIVIDE_HXX__ */
