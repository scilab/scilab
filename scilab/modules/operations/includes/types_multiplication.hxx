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

#include "operation.hxx"
#include "double.hxx"
#include "matrixpoly.hxx"



EXTERN_OP types::InternalType *GenericTimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand);

EXTERN_OP int MultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
EXTERN_OP int MultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut);
EXTERN_OP int MultiplyDoubleByPoly(types::Double* _pDouble, types::MatrixPoly* _pPoly, types::MatrixPoly** _pPolyOut);
EXTERN_OP int MultiplyPolyByDouble(types::MatrixPoly* _pPoly, types::Double* _pDouble, types::MatrixPoly **_pPolyOut);
EXTERN_OP int MultiplyPolyByPoly(types::MatrixPoly* _pPoly1, types::MatrixPoly* _pPoly2, types::MatrixPoly** _pPolyOut);

EXTERN_OP int DotMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double**	 _pDoubleOut);

#endif /* __TYPES_MULTIPLICATION_HXX__ */
