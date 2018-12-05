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
#include "alltypes.hxx"
#include "allvar.hxx"

#include "MacroLoc.hxx"

namespace coverage
{

class Counter
{

    uint64_t counter;
    uint64_t cumTime;
    std::chrono::steady_clock::time_point start;
    bool isRunning;
    types::Macro* macro;
    ast::Exp* e;

  public:
    Counter(types::Macro* _macro, ast::Exp* _e) : counter(0), cumTime(0), macro(_macro), e(_e), isRunning(false) {}

    inline void inc()
    {
        ++counter;
    }

    inline uint64_t get() const
    {
        return counter;
    }

    inline types::Macro* getMacro()
    {
        return macro;
    }

    inline ast::Exp* getExp()
    {
        return e;
    }

    inline types::Macro* getMacro() const
    {
        return macro;
    }

    inline ast::Exp* getExp() const
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
    CallCounter() : counter(0) {}

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

struct CounterPredicate
{
    struct by_file_and_location
    {
        bool operator()(const Location& o1, const Location& o2) const
        {
            return o1.first_line < o2.first_line || (o1.first_line == o2.first_line && o2.last_line < o1.last_line);
        };

        bool operator()(const Counter& o1, const Counter& o2) const
        {
            bool file = o1.getMacro()->getFileName() < o2.getMacro()->getFileName();
            if (!file && o1.getMacro()->getFileName() == o2.getMacro()->getFileName())
            {
                const Location& l1 = o1.getExp()->getLocation();
                const Location& l2 = o2.getExp()->getLocation();
                return this->operator()(l1, l2);
            }
            return file;
        };

        bool operator()(types::Macro* o1, const Counter& o2) const
        {
            bool file = o1->getFileName() < o2.getMacro()->getFileName();
            if (!file && o1->getFileName() == o2.getMacro()->getFileName())
            {
                const Location& l1 = o1->getBody()->getLocation();
                const Location& l2 = o2.getExp()->getLocation();
                return this->operator()(l1, l2);
            }

            return file;
        };

        bool operator()(const Counter& o1, types::Macro* o2) const
        {
            bool file = o1.getMacro()->getFileName() < o2->getFileName();
            if (!file && o1.getMacro()->getFileName() == o2->getFileName())
            {
                const Location& l1 = o1.getExp()->getLocation();
                const Location& l2 = o2->getBody()->getLocation();
                return this->operator()(l1, l2);
            }

            return file;
        };

        bool operator()(const Counter& o1, const MacroLoc& o2) const
        {
            if (o1.getMacro()->getName() == o2.name)
            {
                return this->operator()(o1.getExp()->getLocation(), o2.loc);
            }
            return o1.getMacro()->getName() < o2.name;
        };

        bool operator()(const MacroLoc& o1, const Counter& o2) const
        {
            if (o1.name == o2.getMacro()->getName())
            {
                return this->operator()(o1.loc, o2.getExp()->getLocation());
            }
            return o1.name < o2.getMacro()->getName();
        };
    };
};

} // namespace coverage

#endif // __COUNTER_HXX__
