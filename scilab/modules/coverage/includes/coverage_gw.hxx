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

#ifndef __COVERAGE_GW_HXX__
#define __COVERAGE_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#include "dynlib_coverage.h"

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covStart, COVERAGE_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covWrite, COVERAGE_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covStop, COVERAGE_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covMerge, COVERAGE_IMPEXP);

#endif /* __COVERAGE_GW_HXX__ */

