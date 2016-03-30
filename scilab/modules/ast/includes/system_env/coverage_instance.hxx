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

#ifndef __COVERAGE_INSTANCE_HXX__
#define __COVERAGE_INSTANCE_HXX__

extern "C"
{
#include "dynlib_ast.h"
#include "CoverModule_interface.h"
}

namespace types
{
class Callable;
}

namespace ast
{
class Exp;

class EXTERN_AST CoverageInstance
{
    //coverage
private:
    static void* coverage;
public:
    static inline void* getCoverage()
    {
        return coverage;
    }

    static inline void setCoverage(void* cov)
    {
        coverage = cov;
    }

    static inline void invoke(const ast::Exp& exp)
    {
        if (coverage)
        {
            CoverModule_invokeExp((void*)&exp);
        }
    }

    static inline void invoke(types::Callable* call)
    {
        if (coverage)
        {
            CoverModule_invokeCallable(call);
        }
    }

    static inline void invokeAndStartChrono(void* exp)
    {
        if (coverage)
        {
            CoverModule_invokeAndStartChrono(exp);
        }
    }

    static inline void stopChrono(void* exp)
    {
        if (coverage)
        {
            CoverModule_stopChrono(exp);
        }
    }
};
}
#endif /* !__COVERAGE_INSTANCE_HXX__ */