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