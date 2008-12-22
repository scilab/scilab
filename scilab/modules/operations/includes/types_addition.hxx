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

#ifndef __TYPES_ADD_H__
#define __TYPES_ADD_H__

#include "alltypes.hxx" 

using namespace types;

Double* AddDoubleToDouble(Double *_pDouble1, Double *_pDouble2);
MatrixPoly* AddDoubleToPoly(MatrixPoly *_pPoly, Double *_pDouble);
MatrixPoly* AddPolyToPoly(MatrixPoly* pPoly1, MatrixPoly* _pPoly2);
String* AddStringToString(String *_pString1, String *_pString2);

#endif /* __TYPES_ADD_H__ */
