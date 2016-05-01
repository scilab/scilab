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

#ifndef __CHRONO_HXX__
#define __CHRONO_HXX__

#include <chrono>
#include <iostream>

namespace analysis
{

class Chrono
{
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

public:

    void start_chrono()
    {
        start = std::chrono::steady_clock::now();
    }

    void stop_chrono()
    {
        end = std::chrono::steady_clock::now();
    }

    double get_duration() const
    {
        return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    }

    friend std::wostream & operator<<(std::wostream & out, const Chrono & chrono)
    {
        out << L"Elapsed time: " << chrono.get_duration() << L" s.";
        return out;
    }
};

} // namespace analysis

#endif // __CHRONO_HXX__
