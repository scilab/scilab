/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __COUNTER_HXX__
#define __COUNTER_HXX__

#include <chrono>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"

namespace coverage
{

class Counter
{

    uint64_t counter;
    uint64_t cumTime;
    std::chrono::steady_clock::time_point start;
    bool isRunning;
    types::Macro * macro;
    ast::Exp * e;

public:

    Counter(types::Macro * _macro, ast::Exp * _e) : counter(0), cumTime(0), macro(_macro), e(_e), isRunning(false) { }

    inline void inc()
    {
        ++counter;
    }

    inline uint64_t get() const
    {
        return counter;
    }

    inline types::Macro * getMacro()
    {
        return macro;
    }

    inline ast::Exp * getExp()
    {
        return e;
    }

    inline types::Macro * getMacro() const
    {
        return macro;
    }

    inline ast::Exp * getExp() const
    {
        return e;
    }

    inline void startChrono()
    {
        start = std::chrono::steady_clock::now();
        isRunning = true;
    }

    inline void stopChrono()
    {
        if (isRunning)
        {
            cumTime += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count();
            isRunning = false;
        }
    }

    inline uint64_t getNanoTime() const
    {
        return cumTime;
    }
};

class CallCounter
{
    uint64_t counter;

public:

    CallCounter() : counter(0) { }

    inline void inc()
    {
        ++counter;
    }

    inline uint64_t get() const
    {
        return counter;
    }

    inline uint64_t get()
    {
        return counter;
    }
};

} // namespace coverage

#endif // __COUNTER_HXX__
