/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MIN_H__
#define __MIN_H__

#include <vector>
#include "double.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

#ifdef _MSC_VER
#undef min //to remove ~define from windows header
#endif

ELEMENTARY_FUNCTIONS_IMPEXP void min(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut);

#endif /* __MIN_H__ */
