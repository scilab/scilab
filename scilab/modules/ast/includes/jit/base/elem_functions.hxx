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

#ifndef __BASE_ELEM_FUNCTIONS_HXX__
#define __BASE_ELEM_FUNCTIONS_HXX__

#include <cmath>
#include <complex>

#include "Cast.hxx"

#include "core_math.h"
#include "faddeeva.h"

extern "C"
{
#include "basic_functions.h"
}

namespace jit
{
namespace elem
{

template<typename T, typename U>
struct cast
{
    inline U operator()(T x)
    {
        return (U)x;
    }
};

template<typename T, typename U = T>
struct abs
{
    inline U operator()(T x)
    {
        if (std::is_unsigned<T>::value)
        {
            return x;
        }
        return x >= 0 ? x : -x;
    }
};

template<>
struct abs<double, double>
{
    inline double operator()(double x)
    {
        return std::fabs(x);
    }
};

template<>
struct abs<const std::complex<double> &, double>
{
    inline double operator()(const std::complex<double> & z)
    {
        return std::hypot(z.real(), z.imag());
    }
};

template<typename T, typename U = T>
struct ceil
{
    inline U operator()(T x)
    {
        return x;
    }
};

template<>
struct ceil<double, double>
{
    inline double operator()(double x)
    {
        return std::ceil(x);
    }
};

template<>
struct ceil<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::complex<double>(std::ceil(z.real()), std::ceil(z.imag()));
    }
};

template<typename T, typename U = T>
struct floor
{
    inline U operator()(T x)
    {
        return x;
    }
};

template<>
struct floor<double, double>
{
    inline double operator()(double x)
    {
        return std::floor(x);
    }
};

template<>
struct floor<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::complex<double>(std::floor(z.real()), std::floor(z.imag()));
    }
};

template<typename T, typename U = T>
struct trunc
{
    inline U operator()(T x)
    {
        return x;
    }
};

template<>
struct trunc<double, double>
{
    inline double operator()(double x)
    {
        return std::trunc(x);
    }
};

template<>
struct trunc<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::complex<double>(std::trunc(z.real()), std::trunc(z.imag()));
    }
};

template<typename T, typename U = T>
struct round
{
    inline U operator()(T x)
    {
        return x;
    }
};

template<>
struct round<double, double>
{
    inline double operator()(double x)
    {
        return std::round(x);
    }
};

template<>
struct round<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::complex<double>(std::round(z.real()), std::round(z.imag()));
    }
};

/*template<typename T>
inline T min(T x, T y)
{
return x < y ? x : y;
}

template<typename T>
inline T max(T x, T y)
{
return x > y ? x : y;
}

inline double rand()
{
static int * _iVal = 0;
return durands(_iVal);
}
*/


template<typename T, typename U = T>
struct sign
{
    inline U operator()(T x)
    {
        if (std::is_unsigned<T>::value)
        {
            return 1;
        }
        else
        {
            return x < 0 ? -1 : 1;
        }
    }
};

template<>
struct sign<double, double>
{
    inline double operator()(double x)
    {
        if (x == 0 || std::isnan(x))
        {
            return x;
        }
        return x < 0 ? -1 : 1;
    }
};

template<>
struct sign<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return z / std::hypot(z.real(), z.imag());
    }
};

template<typename T, typename U = T>
struct log2;

template<>
struct log2<double, double>
{
    inline double operator()(double x)
    {
        return std::log(x) / M_LN2;
    }
};

template<>
struct log2<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::log(z) / M_LN2;
    }
};

template<typename T, typename U = T>
struct log;

template<>
struct log<double, double>
{
    inline double operator()(double x)
    {
        return std::log(x);
    }
};

template<>
struct log<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::log(z);
    }
};

template<typename T, typename U = T>
struct log10;

template<>
struct log10<double, double>
{
    inline double operator()(double x)
    {
        return std::log10(x);
    }
};

template<>
struct log10<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::log10(z);
    }
};

template<typename T, typename U = T>
struct sqrt;

template<>
struct sqrt<double, double>
{
    inline double operator()(double x)
    {
        return std::sqrt(x);
    }
};

template<>
struct sqrt<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::sqrt(z);
    }
};

template<typename T, typename U = T>
struct sqrt_neg;

template<>
struct sqrt_neg<double, std::complex<double>>
{
    inline std::complex<double> operator()(double x)
    {
        return std::complex<double>(0, std::sqrt(-x));
    }
};

template<typename T, typename U = T>
struct log_neg;

template<>
struct log_neg<double, std::complex<double>>
{
    inline std::complex<double> operator()(double x)
    {
        return std::complex<double>(std::log(-x), M_PI);
    }
};

template<typename T, typename U = T>
struct log10_neg;

template<>
struct log10_neg<double, std::complex<double>>
{
    inline std::complex<double> operator()(double x)
    {
        return std::complex<double>(std::log10(-x), M_PI);
    }
};

template<typename T, typename U = T>
struct log2_neg;

template<>
struct log2_neg<double, std::complex<double>>
{
    inline std::complex<double> operator()(double x)
    {
        return std::complex<double>(std::log(-x) / M_LN2, M_PI / M_LN2);
    }
};

//template<typename T, typename U = T>
//struct erf;
//
//template<>
//struct erf<double, double>
//{
//    inline double operator()(double x)
//    {
//        return std::erf(x);
//    }
//};
//
//template<>
//struct erf<const std::complex<double> &, std::complex<double>>
//{
//    inline std::complex<double> operator()(const std::complex<double> & z)
//    {
//        return Faddeeva::erf(z, 0);
//    }
//};
//
//template<typename T, typename U = T>
//struct erfc;
//
//template<>
//struct erfc<double, double>
//{
//    inline double operator()(double x)
//    {
//        return std::erfc(x);
//    }
//};
//
//template<>
//struct erfc<const std::complex<double> &, std::complex<double>>
//{
//    inline std::complex<double> operator()(const std::complex<double> & z)
//    {
//        return Faddeeva::erfc(z, 0);
//    }
//};
//
//template<typename T, typename U = T>
//struct erfi;
//
//template<>
//struct erfi<double, double>
//{
//    inline double operator()(double x)
//    {
//        return Faddeeva::erfi(x);
//    }
//};
//
//template<>
//struct erfi<const std::complex<double> &, std::complex<double>>
//{
//    inline std::complex<double> operator()(const std::complex<double> & z)
//    {
//        return Faddeeva::erfi(z, 0);
//    }
//};
//
//template<typename T, typename U = T>
//struct erfcx;
//
//template<>
//struct erfcx<double, double>
//{
//    inline double operator()(double x)
//    {
//        return Faddeeva::erfcx(x);
//    }
//};
//
//template<>
//struct erfcx<const std::complex<double> &, std::complex<double>>
//{
//    inline std::complex<double> operator()(const std::complex<double> & z)
//    {
//        return Faddeeva::erfcx(z, 0);
//    }
//};
//
//template<typename T, typename U = T>
//struct dawson;
//
//template<>
//struct dawson<double, double>
//{
//    inline double operator()(double x)
//    {
//        return Faddeeva::Dawson(x);
//    }
//};
//
//template<>
//struct dawson<const std::complex<double> &, std::complex<double>>
//{
//    inline std::complex<double> operator()(const std::complex<double> & z)
//    {
//        return Faddeeva::Dawson(z, 0);
//    }
//};

template<typename T, typename U = T>
struct sin;

template<>
struct sin<double, double>
{
    inline double operator()(double x)
    {
        return std::sin(x);
    }
};

template<>
struct sin<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::sin(z);
    }
};

template<typename T, typename U = T>
struct cos;

template<>
struct cos<double, double>
{
    inline double operator()(double x)
    {
        return std::cos(x);
    }
};

template<>
struct cos<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::cos(z);
    }
};

template<typename T, typename U = T>
struct tan;

template<>
struct tan<double, double>
{
    inline double operator()(double x)
    {
        return std::tan(x);
    }
};

template<>
struct tan<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::tan(z);
    }
};

template<typename T, typename U = T>
struct tanh;

template<>
struct tanh<double, double>
{
    inline double operator()(double x)
    {
        return std::tanh(x);
    }
};

template<>
struct tanh<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::tanh(z);
    }
};

template<typename T, typename U = T>
struct sinh;

template<>
struct sinh<double, double>
{
    inline double operator()(double x)
    {
        return std::sinh(x);
    }
};

template<>
struct sinh<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::sinh(z);
    }
};

template<typename T, typename U = T>
struct cosh;

template<>
struct cosh<double, double>
{
    inline double operator()(double x)
    {
        return std::cosh(x);
    }
};

template<>
struct cosh<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::cosh(z);
    }
};

template<typename T, typename U = T>
struct atan;

template<>
struct atan<double, double>
{
    inline double operator()(double x)
    {
        return std::atan(x);
    }
};

template<>
struct atan<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::atan(z);
    }
};

template<typename T, typename U = T>
struct exp;

template<>
struct exp<double, double>
{
    inline double operator()(double x)
    {
        return std::exp(x);
    }
};

template<>
struct exp<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return std::exp(z);
    }
};

template<typename T, typename U = T>
struct tgamma;

template<>
struct tgamma<double, double>
{
    inline double operator()(double x)
    {
        return std::tgamma(x);
    }
};

template<typename T, typename U = T>
struct lgamma;

template<>
struct lgamma<double, double>
{
    inline double operator()(double x)
    {
        return std::lgamma(x);
    }
};

template<typename T, typename U = T>
struct real;

template<>
struct real<double, double>
{
    inline double operator()(double x)
    {
        return x;
    }
};

template<>
struct real<const std::complex<double> &, std::complex<double>>
{
    inline std::complex<double> operator()(const std::complex<double> & z)
    {
        return z.real();
    }
};

template<typename T, typename U = T>
struct imag;

template<>
struct imag<double, double>
{
    inline double operator()(double x)
    {
        return 0;
    }
};

template<>
struct imag<const std::complex<double> &, double>
{
    inline double operator()(const std::complex<double> & z)
    {
        return z.imag();
    }
};

template<typename T>
struct is_strict_negative;

template<>
struct is_strict_negative<double>
{
    inline bool operator()(double x)
    {
        return x < 0;
    }
};

} // namespace BaseFunctions

} // namespace jit

#endif // __BASE_ELEM_FUNCTIONS_HXX__
