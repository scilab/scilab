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

#ifndef __TOOLS_HXX__
#define __TOOLS_HXX__

#include <bitset>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#ifdef _MSC_VER
#include "stdint.h"
#include <intrin.h>
#endif

#include "core_math.h"
#include "symbol.hxx"

namespace analysis
{

namespace tools
{

#ifdef _MSC_VER
inline static double trunc(const double x)
{
    return x > 0 ? floor(x) : ceil(x);
}

inline static uint32_t clz(const uint32_t x)
{
    unsigned long r = 0;
    _BitScanForward(&r, x);
    return r;
}

inline static uint32_t clzll(const uint64_t x)
{
#ifdef _WIN64
    unsigned long r = 0;
    _BitScanForward64(&r, x);
    return r;
#else
    const uint32_t u32 = x >> 32;
    if (u32)
    {
        return clz(u32);
    }
    return 32 + clz(x & 0xFFFFFFFFUL);
#endif
}
#else
inline static double trunc(const double x)
{
#ifdef __APPLE__
    // Needed for compilation with GCC 4.8.2
    return x > 0 ? floor(x) : ceil(x);
#else
    return std::trunc(x);
#endif
}

inline static uint32_t clz(const uint32_t x)
{
    return x ? __builtin_clz(x) : 32;
}

inline static uint32_t clzll(const uint64_t x)
{
    return x ? __builtin_clzll(x) : 64;
}
#endif

inline static double NaN()
{
    return std::numeric_limits<double>::quiet_NaN();
}

inline static bool isNaN(const double x)
{
    return ISNAN(x) != 0;
}

inline static bool isFinite(const double x)
{
    return finite(x) != 0;
}

inline static bool isInfinite(const double x)
{
    return !isFinite(x);
}

enum IntType { NOTANINT, SIGNED, UNSIGNED };

inline static IntType getIntType(const double x)
{
    if (x == trunc(x))
    {
        if (x >= 0)
        {
            if (x <= (double)std::numeric_limits<uint64_t>::max())
            {
                return UNSIGNED;
            }
        }
        else if (x >= (double)std::numeric_limits<int64_t>::min())
        {
            return SIGNED;
        }
    }

    return NOTANINT;
}

template<typename T>
inline static bool asInteger(const double x, T & ival)
{
    if (x == trunc(x))
    {
        if (x >= 0)
        {
            if (x <= (double)std::numeric_limits<T>::max())
            {
                ival = (T)x;
                return true;
            }
        }
        else if (x >= (double)std::numeric_limits<T>::min())
        {
            ival = (T)x;
            return true;
        }
    }

    return false;
}

inline static bool isAnInt(const double x)
{
    return getIntType(x) != NOTANINT;
}

template<typename T>
inline static T cast(const double x)
{
    if (x < static_cast<double>(std::numeric_limits<T>::max()))
    {
        if (x > static_cast<double>(std::numeric_limits<T>::min()))
        {
            return static_cast<T>(x);
        }
        else
        {
            return std::numeric_limits<T>::min();
        }
    }
    else
    {
        return std::numeric_limits<T>::max();
    }
}

template<typename T>
inline static T powui(T x, uint64_t n)
{
    T p = x;
    T y = (n & 1) ? x : 1;

    while (n >>= 1)
    {
        p *= p;
        if (n & 1)
        {
            y *= p;
        }
    }

    return y;
}

inline std::wostream & operator<<(std::wostream & out, const IntType & it)
{
    switch (it)
    {
        case IntType::NOTANINT :
            out << L"NAI";
            break;
        case IntType::SIGNED :
            out << L'S';
            break;
        case IntType::UNSIGNED :
            out << L'U';
            break;
    }
    return out;
}

template<typename T>
inline static unsigned char popcount(const T x)
{
    return std::bitset<sizeof(T)>(x).count();
}

inline static unsigned char log2(const unsigned int x)
{
    return (unsigned char)((sizeof(unsigned int) << 3) - clz(x) - 1);
}

inline static unsigned char log2(const unsigned long long x)
{
    return (unsigned char)((sizeof(unsigned long long) << 3) - clzll(x) - 1);
}

template<typename T>
static void printSet(const T & set, std::wostream & out)
{
    if (set.empty())
    {
        out << L"{}";
    }
    else
    {
        out << L'{';
        for (typename T::const_iterator i = set.begin(); i != set.end(); ++i)
        {
            if (std::next(i) == set.end())
            {
                out << *i << L'}';
            }
            else
            {
                out << *i << L',';
            }
        }
    }
}

template<typename T>
static void printMap(const T & map, std::wostream & out, const bool newLine = false )
{
    if (map.empty())
    {
        out << L"{}";
    }
    else
    {
        out << L'{';
        for (typename T::const_iterator i = map.begin(); i != map.end(); ++i)
        {
            out << i->first << L" -> " << i->second;
            if (std::next(i) == map.end())
            {
                out << L'}';
            }
            else
            {
                out << L',';
                if (newLine)
                {
                    out << L'\n';
                }
            }
        }
    }
}

template<typename T>
static void printMapInfo(std::wostream & out, const T & map, const bool show_collisions = false)
{
    double mean = 0;
    double variance = 0;
    const unsigned int count = map.bucket_count();
    unsigned int empty_bucket_count = 0;
    unsigned int collision_count = 0;

    out << L"Map size: " << map.size() << std::endl;
    out << L"Number of buckets: " << count << std::endl;

    for (unsigned int i = 0; i < count; ++i)
    {
        if (const unsigned int s = map.bucket_size(i))
        {
            mean += (double)s;
            if (s > 1)
            {
                ++collision_count;
            }
        }
        else
        {
            ++empty_bucket_count;
        }
    }
    mean /= (double)count;

    for (unsigned int i = 0; i < count; ++i)
    {
        const double ms = mean - (double)map.bucket_size(i);
        variance += ms * ms;
    }
    variance /= (double)count;

    out << L"Number of elements by buckets: mean=" << mean << L", sigma=" << std::sqrt(variance) << std::endl;
    out << L"Number of empty buckets: " << empty_bucket_count << std::endl;
    out << L"Number of collisions: " << collision_count << std::endl;

    if (show_collisions)
    {
        std::multimap<unsigned int, typename T::key_type> collisions;
        for (const auto & p : map)
        {
            collisions.emplace(map.bucket(p.first), p.first);
        }

        for (const auto & p : collisions)
        {
            out << L"Bucket " << p.first << L": " << p.second << L", hash=" << (typename T::hasher()(p.second)) << std::endl;
        }
    }
}

inline static std::size_t hash_combine(const std::size_t seed)
{
    return seed;
}

template<typename... Args>
inline static std::size_t hash_combine(const std::size_t seed, Args... args)
{
    // it is the way Boost has implemented hash_combine:
    // http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
    return seed ^ (hash_combine(args...) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

struct HashSymbol
{
    inline std::size_t operator()(const symbol::Symbol & sym) const
    {
        return std::hash<std::wstring>()(sym.getName());
    }
};

struct EqSymbol
{
    inline bool operator()(const symbol::Symbol & L, const symbol::Symbol & R) const
    {
        return L == R;
    }
};

typedef std::set<symbol::Symbol> SymbolOrdSet;
typedef std::unordered_set<symbol::Symbol, HashSymbol> SymbolSet;
template<typename T>
using SymbolMap = std::unordered_map<symbol::Symbol, T, HashSymbol, EqSymbol>;

} // namespace tools

} // namespace analysis

#endif // __TOOLS_HXX__
