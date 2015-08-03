/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - cedric delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CUMPROD_H__
#define __CUMPROD_H__

#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP int cumprod(types::Double* pIn,  int iOrientation, types::Double* pOut);
ELEMENTARY_FUNCTIONS_IMPEXP int cumprod(types::Polynom* pIn,  int iOrientation, types::Polynom* pOut);

#endif /* __CUMPROD_H__ */
