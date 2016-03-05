/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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
