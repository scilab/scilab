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

#ifndef __TYPES_POWER_HXX__
#define __TYPES_POWER_HXX__

#include "dynlib_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"

using namespace types;

EXTERN_OP int PowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut);
EXTERN_OP int PowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom** _pPolyOut);

#endif /* __TYPES_POWER_HXX__ */
