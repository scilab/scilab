/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __COVERAGE_GW_HXX__
#define __COVERAGE_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#include "dynlib_coverage.h"

class CoverageModule
{
private:
    CoverageModule() {};
    ~CoverageModule() {};

public:
    COVERAGE_IMPEXP static int Load();
    COVERAGE_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covStart, COVERAGE_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covWrite, COVERAGE_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covStop, COVERAGE_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_covMerge, COVERAGE_IMPEXP);

#endif /* __COVERAGE_GW_HXX__ */

