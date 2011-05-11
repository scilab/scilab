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
#ifndef __TYPES_MULTIPLICATION_HXX__
#define __TYPES_MULTIPLICATION_HXX__

#include "dynlib_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"


EXTERN_OP types::InternalType *GenericDotTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);
EXTERN_OP types::InternalType *GenericTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

int MultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
int MultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
int MultiplyDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom** _pPolyOut);
int MultiplyPolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom **_pPolyOut);
int MultiplyPolyByPoly(types::Polynom* _pPoly1, types::Polynom* _pPoly2, types::Polynom** _pPolyOut);

int DotMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double**	 _pDoubleOut);

#endif /* __TYPES_MULTIPLICATION_HXX__ */
