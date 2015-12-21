/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_FINITE__
#define __TYPES_FINITE__

#include "double.hxx"

extern "C"
{
#include "matrix_finite.h"
}

bool isDoubleFinite(types::Double *_pDouble);

#endif /* __TYPES_FINITE__ */
