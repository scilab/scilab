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
#ifndef __TYPES_MULTI_H__
#define __TYPES_MULTI_H__

#include "operation.hxx"
#include "alltypes.hxx"


using namespace types;

EXTERN_OP int MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut);
EXTERN_OP int MultiplyDoubleByPoly(Double* _pDouble, MatrixPoly* _pPoly, MatrixPoly** _pPolyOut);
EXTERN_OP int MultiplyPolyByDouble(MatrixPoly* _pPoly, Double* _pDouble, MatrixPoly **_pPolyOut);
EXTERN_OP int MultiplyPolyByPoly(MatrixPoly* _pPoly1, MatrixPoly* _pPoly2, MatrixPoly** _pPolyOut);

EXTERN_OP int DotMultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double**	 _pDoubleOut);

#endif /* __TYPES_MULTI_H__ */
