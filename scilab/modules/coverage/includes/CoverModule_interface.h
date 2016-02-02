/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
#include "dynlib_coverage.h"

COVERAGE_IMPEXP void CoverModule_invokeCallable(void* call);
COVERAGE_IMPEXP void CoverModule_invokeExp(void* exp);
COVERAGE_IMPEXP void CoverModule_invokeAndStartChrono(void* exp);
COVERAGE_IMPEXP void CoverModule_stopChrono(void* exp);


