/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "dynlib_coverage.h"

COVERAGE_IMPEXP void CoverModule_invokeCallable(void* call);
COVERAGE_IMPEXP void CoverModule_invokeExp(void* exp);
COVERAGE_IMPEXP void CoverModule_invokeAndStartChrono(void* exp);
COVERAGE_IMPEXP void CoverModule_stopChrono(void* exp);


