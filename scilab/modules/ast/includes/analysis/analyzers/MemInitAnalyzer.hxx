/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MEM_INIT_ANALYZER_HXX__
#define __MEM_INIT_ANALYZER_HXX__

#include "CallAnalyzer.hxx"

namespace analysis
{
// Analyzer for ones, zeros, rand which allocate and initialized memory
class MemInitAnalyzer : public CallAnalyzer
{

public:

    bool analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e);
};

class ZerosAnalyzer : public MemInitAnalyzer
{

public:

    FnName getFnName() const
    {
        return ZEROS;
    }
};

class OnesAnalyzer : public MemInitAnalyzer
{

public:

    FnName getFnName() const
    {
        return ONES;
    }
};

class RandAnalyzer : public MemInitAnalyzer
{

public:

    FnName getFnName() const
    {
        return RAND;
    }
};

class EyeAnalyzer : public MemInitAnalyzer
{

public:

    FnName getFnName() const
    {
        return EYE;
    }
};

} // namespace analysis

#endif // __MEM_INIT_ANALYZER_HXX__
