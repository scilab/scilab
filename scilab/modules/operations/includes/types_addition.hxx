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
#include "generic_operations.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

//Double
int AddDoubleToDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double** _pDoubleOut);

//Poly
int AddDoubleToPoly(types::Polynom *_pPoly, types::Double *_pDouble, types::Polynom ** _pPolyOut);
int AddPolyToPoly(types::Polynom* pPoly1, types::Polynom* _pPoly2, types::Polynom ** _pPolyOut);

//String
int AddStringToString(types::String *_pString1, types::String *_pString2, types::String** _pStringOut);

//Sparse
int AddSparseToSparse(types::Sparse *_pSparse1, types::Sparse *_pSparse2, types::Sparse** _pSparseOut);
int AddSparseToDouble(types::Sparse *_pSparse,  types::Double* _pDouble, types::GenericType** _pDoubleOut);
int AddDoubleToSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::GenericType** _pDoubleOut);

#endif /* __TYPES_ADD_H__ */
