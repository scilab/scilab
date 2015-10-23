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

#ifndef __BASE_EXPONENTIATION_HXX__
#define __BASE_EXPONENTIATION_HXX__

#include <complex>

#include "Cast.hxx"
#include "base/multiplication.hxx"
#include "base/rdivision.hxx"
#include "base/tools.hxx"

namespace jit
{
namespace op
{

template<typename T, typename U, typename V>
struct Pow
{

    inline V operator()(T x, U y)
    {
        return (V)std::pow((V)x, (V)y);
    }
};

template<typename U>
struct Pow<const std::complex<double> &, U, std::complex<double>>
{

    inline std::complex<double> operator()(const std::complex<double> & x, U y)
    {
        return std::pow(x, (double)y);
    }
};

template<typename T>
struct Pow<T, const std::complex<double> &, std::complex<double>>
{

    inline std::complex<double> operator()(T x, const std::complex<double> & y)
    {
        return std::pow((double)x, y);
    }
};

template<>
struct Pow<const std::complex<double> &, const std::complex<double> &, std::complex<double>>
{

    inline std::complex<double> operator()(const std::complex<double> & x, const std::complex<double> & y)
    {
        return std::pow(x, y);
    }
};

} // namespace op

namespace exponentiation
{

template<typename T>
inline void SQ22(T * x)
{
    const T bc = x[1] * x[2];
    const T a_d = x[0] + x[3];
    x[0] = x[0] * x[0] + bc;
    x[1] *= a_d;
    x[2] *= a_d;
    x[3] = x[3] * x[3] + bc;
}

template<typename T>
inline void POWM22(T * x, uint64_t n)
{
    switch (n)
    {
        case 0:
            x[0] = x[3] = 1;
            x[1] = x[2] = 0;
            return;
        case 1:
            return;
        case 2:
        {
            SQ22(x);
            return;
        }
        default:
        {
            T y0, y1, y2, y3;
            if (n & 1)
            {
                y0 = x[0];
                y1 = x[1];
                y2 = x[2];
                y3 = x[3];
            }
            else
            {
                y0 = 1;
                y1 = 0;
                y2 = 0;
                y3 = 1;
            }

            while (n >>= 1)
            {
                SQ22(x);
                if (n & 1)
                {
                    T a, b;
                    a = y0 * x[0] + y2 * x[1];
                    y2 = y0 * x[2] + y2 * x[3];
                    b = y1 * x[0] + y3 * x[1];
                    y3 = y1 * x[2] + y3 * x[3];
                    y0 = a;
                    y1 = b;
                }

            }

            x[0] = y0;
            x[1] = y1;
            x[2] = y2;
            x[3] = y3;
        }
    }
}

inline void POWM(const double * x, const int64_t x_r, double * o, uint64_t n)
{
    /*
      This is not exactly the fast pow algorithm.
      I slightly modified it to avoid the dcopys.
      The idea is easy: we want the result in o so
      if N is even then o = t1*t1 else o=x*t1 and t1=o*o
      and we can replace n by n/2.
      If we were in the first case and n is even then t1=o*o
      else o=x*t1 and t1=o*o,...
      So to generate the dgemms we need to begin with the most signifiant bit.
      For example:
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, x, &x_r, &zero, o, &x_r); // x^2
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, o, &x_r, &zero, t1, &x_r); // x^3
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, t1, &x_r, t1, &x_r, &zero, o, &x_r); // x^6
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, o, &x_r, o, &x_r, &zero, t1, &x_r); // x^12
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, t1, &x_r, &zero, o, &x_r); // x^13

      The goal is to have one temp array, no memcpy and the final multiplication must put its result in o.
    */

    if (x_r == 2)
    {
        o[0] = x[0];
        o[1] = x[1];
        o[2] = x[2];
        o[3] = x[3];
        POWM22(o, n);
        return;
    }

    const int64_t size = x_r * x_r;
    int x_r_i = (int)x_r;

    switch (n)
    {
        case 0:
        {
            // Identity matrix
            std::memset(o, 0, size * sizeof(double));
            for (int64_t i = 0; i < x_r; ++i)
            {
                o[i * (x_r + 1)] = 1;
            }
            return;
        }
        case 1:
        {
            // x itself
            std::memcpy(o, x, size * sizeof(double));
            return;
        }
        case 2:
        {
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, x, o);
            return;
        }
        default:
            break;
    }

    //const unsigned int nbits = 8 * sizeof(uint64_t) - __builtin_clzll(n);
    //const bool parity = (nbits - __builtin_popcountll(n)) & 1;
    // since n >= 3, then nbits >= 2
    const unsigned int nbits = 8 * sizeof(uint64_t) - analysis::tools::clzll(n);
    const bool parity = (nbits - analysis::tools::popcount(n)) & 1;
    unsigned int mask = 1 << (nbits - 2);
    double * tmp = new double[size];

    // Trip cost is [log2(n)] (=nbits-1)
    // Except the most signifiant bit, for each 1 in binary representation of n we have 2 dgemms
    // and for each 0 we have 1 dgemm.
    // E.g. for n=13=0x1101, the cost is 2 * 2 + 1 dgemms.

    if (n & mask)
    {
        if (parity)
        {
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, x, o); // o = x^2
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, o, tmp); // tmp = x^3
            std::swap(o, tmp); // o <--> tmp => o = x^3 & tmp = x^2
        }
        else
        {
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, x, tmp); // tmp = x^2
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, tmp, o); // o = x^3
        }
    }
    else
    {
        if (parity)
        {
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, x, o); // o = x^2
        }
        else
        {
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, x, tmp); // tmp = x^2
            std::swap(o, tmp); // o <--> tmp => o = x^2 & tmp = ?
        }
    }

    while (mask >>= 1)
    {
        multiplication::DGEMM(o, x_r_i, x_r_i, x_r_i, o, tmp); // tmp = o^2
        if (n & mask)
        {
            multiplication::DGEMM(x, x_r_i, x_r_i, x_r_i, tmp, o); // o = x * tmp
        }
        else
        {
            std::swap(o, tmp); // o <--> tmp
        }
    }

    delete[] tmp;
}

inline void POWM(const cpx_t * x, const int64_t x_r, cpx_t * o, uint64_t n)
{
    /*
      This is not exactly the fast pow algorithm.
      I slightly modified it to avoid the dcopys.
      The idea is easy: we want the result in o so
      if N is even then o = t1*t1 else o=x*t1 and t1=o*o
      and we can replace n by n/2.
      If we were in the first case and n is even then t1=o*o
      else o=x*t1 and t1=o*o,...
      So to generate the dgemms we need to begin with the most signifiant bit.
      For example:
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, x, &x_r, &zero, o, &x_r); // x^2
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, o, &x_r, &zero, t1, &x_r); // x^3
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, t1, &x_r, t1, &x_r, &zero, o, &x_r); // x^6
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, o, &x_r, o, &x_r, &zero, t1, &x_r); // x^12
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, t1, &x_r, &zero, o, &x_r); // x^13

      The goal is to have one temp array, no memcpy and the final multiplication must put its result in o.
    */

    if (x_r == 2)
    {
        o[0] = x[0];
        o[1] = x[1];
        o[2] = x[2];
        o[3] = x[3];
        POWM22(o, n);
        return;
    }

    const int64_t size = x_r * x_r;
    int x_r_i = (int)x_r;

    switch (n)
    {
        case 0:
        {
            // Identity matrix
            std::memset(o, 0, size * sizeof(cpx_t));
            for (int64_t i = 0; i < x_r; ++i)
            {
                o[i * (x_r + 1)] = 1;
            }
            return;
        }
        case 1:
        {
            // x itself
            std::memcpy(o, x, size * sizeof(cpx_t));
            return;
        }
        case 2:
        {
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, x, o);
            return;
        }
        default:
            break;
    }

    //const unsigned int nbits = 8 * sizeof(uint64_t) - __builtin_clzll(n);
    //const bool parity = (nbits - __builtin_popcountll(n)) & 1;
    const unsigned int nbits = 8 * sizeof(uint64_t) - analysis::tools::clzll(n);
    const bool parity = (nbits - analysis::tools::popcount(n)) & 1;
    unsigned int mask = 1 << (nbits - 2);
    cpx_t * tmp = new cpx_t[size];

    // Trip cost is [log2(n)] (=nbits-1)
    // Except the most signifiant bit, for each 1 in binary representation of n we have 2 dgemms
    // and for each 0 we have 1 dgemm.
    // E.g. for n=13=0x1101, the cost is 2 * 2 + 1 dgemms.

    if (n & mask)
    {
        if (parity)
        {
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, x, o);
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, o, tmp);
            std::swap(o, tmp);
        }
        else
        {
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, x, tmp);
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, tmp, o);
        }
    }
    else
    {
        if (parity)
        {
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, x, o);
        }
        else
        {
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, x, tmp);
            std::swap(o, tmp);
        }
    }

    while (mask >>= 1)
    {
        multiplication::ZGEMM(o, x_r_i, x_r_i, x_r_i, o, tmp);
        if (n & mask)
        {
            multiplication::ZGEMM(x, x_r_i, x_r_i, x_r_i, tmp, o);
        }
        else
        {
            std::swap(o, tmp);
        }
    }

    delete[] tmp;
}

template<typename T>
inline void powMSi64(const int64_t x_r, const T * x, const int64_t expo, T ** o)
{
    const int64_t x_size = x_r * x_r;
    double * _x = new double[x_size];
    double * _o = new double[x_size];
    for (int64_t i = 0; i < x_size; ++i)
    {
        _x[i] = (double)x[i];
    }
    if (expo > 0)
    {
        POWM(_x, x_r, _o, (uint64_t)expo);
    }
    else
    {
        POWM(_x, x_r, _o, (uint64_t)(-expo));
        int x_r_i = (int)x_r;
        rdivision::DINV(_o, x_r_i);
    }
    delete[] _x;

    T * __o;
    if (*o)
    {
        __o = *o;
    }
    else
    {
        *o = new T[x_r * x_r];
        __o = *o;
    }
    for (int64_t i = 0; i < x_size; ++i)
    {
        __o[i] = (T)_o[i];
    }
    delete[] _o;
}

template<>
inline void powMSi64<double>(const int64_t x_r, const double * x, const int64_t expo, double ** o)
{
    // TODO: x^d => donne un complexe en general ca veut dire que meme ds le cas ou expo est correct
    // il faut prevoir de mettre o_im a null
    if (!*o)
    {
        *o = new double[x_r * x_r];
    }
    if (expo > 0)
    {
        POWM(x, x_r, *o, (uint64_t)expo);
    }
    else
    {
        POWM(x, x_r, *o, (uint64_t)(-expo));
        if (x_r == 2)
        {
            double * _o = *o;
            const double d = _o[0] * _o[3] - _o[1] * _o[2];
            const double x = _o[0] / d;
            _o[0] = _o[3] / d;
            _o[3] = x;
            _o[1] = -_o[1] / d;
            _o[2] = -_o[2] / d;
        }
        else
        {
            int x_r_i = (int)x_r;
            rdivision::DINV(*o, x_r_i);
        }
    }
}

inline void powMcSi64(const int64_t x_r, const double * x_re, const double * x_im, const int64_t expo, double ** o_re, double ** o_im)
{
    const int64_t size = x_r * x_r;
    if (!*o_re)
    {
        *o_re = new double[size];
    }

    if (x_im)
    {
        cpx_t * x = new cpx_t[size];
        cpx_t * o = new cpx_t[size];

        for (int64_t i = 0; i < size; ++i)
        {
            x[i] = cpx_t(x_re[i], x_im[i]);
        }

        if (!*o_im)
        {
            *o_im = new double[size];
        }
        double * _o_re = *o_re;
        double * _o_im = *o_im;

        if (expo > 0)
        {
            POWM(x, x_r, o, (uint64_t)expo);
            delete[] x;
        }
        else
        {
            POWM(x, x_r, o, (uint64_t)(-expo));
            delete[] x;
            if (x_r == 2)
            {
                const cpx_t d = o[0] * o[3] - o[1] * o[2];
                const cpx_t x = o[0] / d;
                o[0] = o[3] / d;
                o[3] = x;
                o[1] = -o[1] / d;
                o[2] = -o[2] / d;

            }
            else
            {
                int x_r_i = (int)x_r;
                rdivision::ZINV(o, x_r_i);
            }
        }
        for (int64_t i = 0; i < size; ++i)
        {
            _o_re[i] = o[i].real();
            _o_im[i] = o[i].imag();
        }
        delete[] o;
    }
    else
    {
        powMSi64<double>(x_r, x_re, expo, o_re);
    }
}

} // namespace exponentiation

} // namespace jit

#endif // __BASE_EXPONENTIATION_HXX__
