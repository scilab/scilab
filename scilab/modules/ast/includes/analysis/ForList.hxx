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

#ifndef __FORLIST_HXX__
#define __FORLIST_HXX__

#include <iostream>
#include <type_traits>

#include "TIType.hxx"
#include "core_math.h"

#ifdef _MSC_VER
#include "stdint.h"
#define TRUNC(x) ((x) > 0 ? floor(x) : ceil(x))
#else
#define TRUNC(x) (std::trunc(x))
#endif

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

    ForList() : constant(false), read_in_loop(false) { }

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
        return std::is_integral<U>::value ? TRUNC(min) : min;
    }

    template<typename U>
    inline U get_step() const
    {
        return std::is_integral<U>::value ? TRUNC(step) : step;
    }

    template<typename U>
    inline U get_max() const
    {
        return std::is_integral<U>::value ? TRUNC(max) : max;
    }

    inline TIType get_type() const
    {
        /*
            if (isempty())
            {
                return TIType(TIType::EMPTY);
            }

            if (is_int())
            {
                if (is_uint())
                {
                    if (std::is_same<T, int32_t>::value)
                    {
                        return TIType(TIType::UINT32);
                    }
                    else
                    {
                        return TIType(TIType::UINT64);
                    }
                }
                else
                {
                    if (std::is_same<T, int64_t>::value)
                    {
                        return TIType(TIType::INT32);
                    }
                    else
                    {
                        return TIType(TIType::INT64);
                    }
                }
            }
        */

        return TIType(TIType::DOUBLE);
    }

    friend std::wostream & operator<<(std::wostream & out, const ForList & fl)
    {
        out << L"ForList: { m: " << fl.min << L", s: " << fl.step << L", M: " << fl.max << L", int: " << (fl._int ? L"T" : L"F") << L", uint: " << (fl._unsigned ? L"T" : L"F");
        return out;
    }

private:

    inline bool isempty() const
    {
        return step == 0 || ISNAN(min) || ISNAN(step) || ISNAN(max) || !finite(min) || !finite(step) || !finite(max) || (min < max && step < 0) || (min > max && step > 0);
    }

    template<typename U>
    inline static bool is_int(const double x)
    {
        return x == TRUNC(x) && x <= std::numeric_limits<U>::max() && x >= std::numeric_limits<U>::min();
    }

    template<typename U>
    inline static U to_int(const double x)
    {
        return TRUNC(x);
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
