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

#ifndef __TYPES_DIV_H__
#define __TYPES_DIV_H__

#include "operation.hxx"
#include "double.hxx"
#include "matrixpoly.hxx"

using namespace types;

EXTERN_OP int DivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double **_pDoubleOut);
EXTERN_OP int DividePolyByDouble(MatrixPoly* _pPoly, Double* _pDouble, MatrixPoly** _pPolyOut);
EXTERN_OP int DivideDoubleByPoly(Double* _pDouble, MatrixPoly* _pPoly, MatrixPoly** _pPolyOut);


#endif /* __TYPES_DIV_H__ */
