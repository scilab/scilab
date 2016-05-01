/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
