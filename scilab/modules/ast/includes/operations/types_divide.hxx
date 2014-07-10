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

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"

int RDivideDoubleByDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double **_pDoubleOut);
int RDividePolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom** _pPolyOut);
int RDivideDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom** _pPolyOut);

int DotRDivideDoubleByDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double **_pDoubleOut);
int DotRDividePolyByDouble(types::Polynom *_pPoly1, types::Double *_pDouble2, types::Polynom **_pPolyOut);

#endif /* !__TYPES_DIVIDE_HXX__ */
