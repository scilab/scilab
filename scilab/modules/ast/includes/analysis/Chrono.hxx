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

#ifndef __CHRONO_HXX__
#define __CHRONO_HXX__

#include <string>
#include <chrono>
#include <iostream>

namespace analysis
{

class Chrono
{
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

    static std::chrono::steady_clock::time_point _tic;

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

    inline static void tic()
    {
        _tic = std::chrono::steady_clock::now();
    }

    inline static double toc(const std::wstring & mark = L"")
    {
        const double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - _tic).count() * 1e-9;
        if (!mark.empty())
        {
            std::wcerr << L"Elapsed time: " << duration << L" s (" << mark << L")" << std::endl;
        }
        return duration;
    }

    friend std::wostream & operator<<(std::wostream & out, const Chrono & chrono)
    {
        out << L"Elapsed time: " << chrono.get_duration() << L" s.";
        return out;
    }
};

} // namespace analysis

#endif // __CHRONO_HXX__
