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

#ifndef __FORLIST_HXX__
#define __FORLIST_HXX__

#include <iostream>
#include <type_traits>

#include "tools.hxx"
#include "TIType.hxx"
#include "core_math.h"

namespace analysis
{
template<typename T>
class ForList
{
    bool constant;
    bool read_in_loop;
    double min;
    double step;
    double max;
    bool _int;
    bool _unsigned;

    static_assert(std::is_same<typename std::make_signed<T>::type, int32_t>::value || std::is_same<typename std::make_signed<T>::type, int64_t>::value, "ForList template parameter must be int32_t or int64_t");

public:

    ForList() : constant(false), read_in_loop(false), min(0), step(0), max(0), _int(false), _unsigned(false) { }

    ForList(const double m, const double s, const double M) : constant(true), read_in_loop(false), min(m), step(s), max(M)
    {
        if (!isempty())
        {
            if (min >= 0 && step > 0 && is_int<typename std::make_unsigned<T>::type>(min) && is_int<typename std::make_unsigned<T>::type>(step) && !overflow<typename std::make_unsigned<T>::type>(min, step, max))
            {
                _int = true;
                _unsigned = true;
            }
            else if (is_int<typename std::make_signed<T>::type>(min) && is_int<typename std::make_signed<T>::type>(step) && !overflow<typename std::make_signed<T>::type>(min, step, max))
            {
                _int = true;
                _unsigned = false;
            }
            else
            {
                _int = false;
                _unsigned = false;
            }
        }
        else
        {
            _int = false;
            _unsigned = false;
        }
    }

    inline bool is_read_in_loop() const
    {
        return read_in_loop;
    }

    inline void set_read_in_loop(const bool read)
    {
        read_in_loop = read;
    }

    inline bool is_constant() const
    {
        return constant;
    }

    inline bool is_int() const
    {
        return _int;
    }

    inline bool is_uint() const
    {
        return _unsigned;
    }

    template<typename U>
    inline U get_min() const
    {
        return std::is_integral<U>::value ? tools::trunc(min) : min;
    }

    template<typename U>
    inline U get_step() const
    {
        return std::is_integral<U>::value ? tools::trunc(step) : step;
    }

    template<typename U>
    inline U get_max() const
    {
        return std::is_integral<U>::value ? tools::trunc(max) : max;
    }

    friend std::wostream & operator<<(std::wostream & out, const ForList & fl)
    {
        out << L"ForList: { m: " << fl.min << L", s: " << fl.step << L", M: " << fl.max << L", int: " << (fl._int ? L"T" : L"F") << L", uint: " << (fl._unsigned ? L"T" : L"F");
        return out;
    }

    inline static bool isempty(const double min, const double max, const double step)
    {
        return step == 0 || tools::isNaN(min) || tools::isNaN(step) || tools::isNaN(max) || !tools::isFinite(min) || !tools::isFinite(step) || !tools::isFinite(max) || (min < max && step < 0) || (min > max && step > 0);
    }

    inline static uint64_t size(const double min, const double max, const double step)
    {
        return (uint64_t)std::floor((max - min) / step) + 1;
    }

    inline static int checkList(const double min, const double max, const double step, double & out)
    {
        if (step == 0)
        {
            if (tools::isNaN(min) || tools::isNaN(max))
            {
                out = tools::NaN();
                return 1; // one value
            }
            else
            {
                return 0; // empty
            }
        }
        else
        {
            if (tools::isNaN(min) || tools::isNaN(step) || tools::isNaN(max))
            {
                out = tools::NaN();
                return 1; // one value
            }
            else
            {
                if (min < max)
                {
                    if (step < 0)
                    {
                        return 0; //empty
                    }
                }
                else if (step > 0)
                {
                    return 0; // empty
                }

                if (!tools::isFinite(min) || !tools::isFinite(max))
                {
                    out = tools::NaN();
                    return 1; // one value
                }
                else
                {
                    return 2; //one or more...
                }
            }
        }
    }

private:

    inline bool isempty() const
    {
        return isempty(min, max, step);
    }

    template<typename U>
    inline static bool is_int(const double x)
    {
        const U y = (U)tools::trunc(x);
        return x == y && y <= std::numeric_limits<U>::max() && y >= std::numeric_limits<U>::min();
    }

    template<typename U>
    inline static U to_int(const double x)
    {
        return tools::trunc(x);
    }

    template<typename U>
    inline static bool overflow(const double m, const double s, const double M)
    {
        if (s > 0)
        {
            // m + k.s <= MAX<U> < m + (k + 1).s
            const double k = std::floor(((double)std::numeric_limits<U>::max() - m) / s);

            // so if m + k * s < M then m + (k + 1).s > MAX<U> => overflow
            return m + k * s < M;
        }
        else
        {
            // m + k.s >= MIN<U> > m + (k + 1).s
            const double k = std::floor(((double)std::numeric_limits<U>::min() - m) / s);

            // so if m + k * s > M then m + (k + 1).s < MIN<U> => overflow
            return m + k * s > M;
        }
    }
};

typedef ForList<int64_t> ForList64;
typedef ForList<int32_t> ForList32;

} // namespace analysis

#endif // __FORLIST_HXX__
