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

#ifndef __BASE_FUNCTIONS_HXX__
#define __BASE_FUNCTIONS_HXX__

#include <algorithm>
#include <complex>
#include <cmath>
#include <type_traits>
#include <vector>

#include "Cast.hxx"
/*#include "base/binary.hxx"
#include "base/addition.hxx"
#include "base/subtraction.hxx"
#include "base/product.hxx"
#include "base/rdivision.hxx"
#include "base/ldivision.hxx"
#include "base/exponentiation.hxx"
#include "base/not_equal.hxx"
#include "base/equal.hxx"
#include "base/greater_than.hxx"
#include "base/greater_or_eq.hxx"
#include "base/lower_than.hxx"
#include "base/lower_or_eq.hxx"
#include "base/and.hxx"
#include "base/or.hxx"
*/

#include "base/elem_functions.hxx"
//#include "base/addition.hxx"
#include "base/tools.hxx"
#include "base/vectorization.hxx"
#include "base/functors.hxx"
#include "base/binary_functors.hxx"
//#include "base/binary.hxx"

#include "core_math.h"
#include "faddeeva.h"

extern "C"
{
    //#include "basic_functions.h"
#include "elem_common.h"

    extern void C2F(dgetri)(int const* n, double* a, int const* ldA, int const* iPiv, double* work, int const* workSize, int* info);
    extern void C2F(dlaswp)(int const* n, double* a, int const* ldA, int const* k1, int const* k2, int const* iPiv, int const* incX);
    extern int C2F(dtrsm) (char* side, char* uplo, char* trans, char* diag, int* m, int* n, double* alpha, double* A, int* lda, double* B, int* ldb);
    extern int C2F(ilaenv)(int *, const char *, const char *, int *, int *, int *, int *);
}

namespace jit
{
namespace BaseFunctions
{
// TODO : LLVMize all these functions to inline them !
// TODO : several vectorizations could be merged: sin(a)+b (a,b matrices with same dims)
//        could be transformed into for i=1:N; res(i)=sin(a(i))+b(i);end => 1 loop rather than 2 !

/*template<typename T, typename U, U (F)(T), typename V = U>
inline void vectorize(const T * x, const int64_t x_r, const int64_t x_c, V ** o)
{
    const int64_t size = x_r * x_c;
V * _o = getPtr(o, size);
    for (int64_t i = 0; i < size; ++i)
    {
        _o[i] = (V)F(x[i]);
    }
}

template<std::complex<double> (CPX)(const std::complex<double> &)>
inline void vectorize(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, double ** o_re, double ** o_im)
{
    const int64_t size = x_r * x_c;
double * _o_re = getPtr(o_re, size);
    double * _o_im = getPtr(o_im, size);
    for (int64_t i = 0; i < size; ++i)
    {
        const std::complex<double> z = CPX(std::complex<double>(x_re[i], x_im[i]));
        _o_re[i] = z.real();
        _o_im[i] = z.imag();
    }
}

template<typename T, T (CPX)(const std::complex<double> &)>
inline void vectorize(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, T ** o)
{
const int64_t size = x_r * x_c;
    T * _o = getPtr(o, size);
    for (int64_t i = 0; i < size; ++i)
    {
        _o[i] = CPX(std::complex<double>(x_re[i], x_im[i]));
    }
}

template<typename T, std::complex<double> (F)(T)>
inline void vectorize(const T * x, const int64_t x_r, const int64_t x_c, double ** o_re, double ** o_im)
{
const int64_t size = x_r * x_c;
double * _o_re = getPtr(o_re, size);
    double * _o_im = getPtr(o_im, size);
    for (int64_t i = 0; i < size; ++i)
    {
        const std::complex<double> z = F(x[i]);
        _o_re[i] = z.real();
        _o_im[i] = z.imag();
    }
}

template<typename T, double (RE)(T), std::complex<double> (CPX)(T), bool USECPX(T)>
inline void vectorize(const T * x, const int64_t x_r, const int64_t x_c, double ** o_re, double ** o_im)
{
const int64_t size = x_r * x_c;
double * _o_re = getPtr(o_re, size);
    double * _o_im = getPtr(o_im, size);

    for (int64_t i = 0; i < size; ++i)
    {
        if (USECPX(x[i]))
        {
            if (!_o_im)
            {
                _o_im = new double[size]();
            }
            const std::complex<double> z = CPX(x[i]);
            _o_re[i] = z.real();
            _o_im[i] = z.imag();
        }
        else
        {
            _o_re[i] = RE(x[i]);
        }
    }

    *o_im = _o_im;
}

// use it with frexp
template<typename T, typename U, typename V, V (F)(T, U *), typename W = V>
inline void vectorize(const T * x, const int64_t x_r, const int64_t x_c, V * o1, W * o2)
{
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        U t;
        o1[i] = (V)F(x[i], &t);
        o2[i] = (W)t;
    }
}

// use it with rand()
template<typename T, T (F)(), typename U = T>
inline void vectorize(const int64_t x_r, const int64_t x_c, U ** o)
{
const int64_t size = x_r * x_c;
    U * _o = getPtr(o, size);
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        o[i] = (T)F();
    }
}*/

// use it with cumsum and cumprod
template<typename T, T (F)(T, T)>
inline void cumulateIn(const T * x, const int64_t x_r, const int64_t x_c, T * o)
{
    // TODO: check x_r > 0 && x_c > 0
    o[0] = x[0];
    for (int64_t i = 1; i < x_r * x_c; ++i)
    {
        o[i] = F(o[i - 1], x[i]);
    }
}

// use it with max(..., 'r')
template<typename T, T (F)(T, T)>
inline void cumulateInR(const T * x, const int64_t x_r, const int64_t x_c, T * o)
{
    for (int i = 0; i < x_c; ++i)
    {
        const T t = x[i * x_r];
        for (int j = 1; i < x_r; ++j)
        {
            t = F(t, x[j + i * x_r]);
        }
        o[i] = t;
    }
}

// use it with max(..., 'c')
template<typename T, T (F)(T, T)>
inline void cumulateInC(const T * x, const int64_t x_r, const int64_t x_c, T * o)
{
    for (int64_t i = 0; i < x_r; ++i)
    {
        const T t = x[i];
        for (int64_t j = 1; i < x_c; ++j)
        {
            t = F(t, x[i + j * x_r]);
        }
        o[i] = t;
    }
}

template<typename T, T (F)(T, T)>
inline T cumulate(const T * x, const int64_t x_r, const int64_t x_c)
{
    T y = x[0];
    for (int64_t i = 1; i < x_r * x_c; ++i)
    {
        y = F(y, x[i]);
    }

    return y;
}

template<bool B, typename T, bool (F)(T)>
inline int breakOnM(const T * x, const int64_t x_r, const int64_t x_c)
{
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        if (F(x[i]) == B)
        {
            return B;
        }
    }

    return !B;
}

template<bool B, typename T, typename U, bool (F)(T, U)>
inline int breakOnMM(const T * x, const int64_t x_r, const int64_t x_c, const U * y, const int64_t y_r, const int64_t y_c)
{
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        if (F(x[i], y[i]) == B)
        {
            return B;
        }
    }

    return !B;
}

template<bool B, typename T, typename U, bool (F)(T, U)>
inline int breakOnSM(const T x, const U * y, const int64_t y_r, const int64_t y_c)
{
    for (int64_t i = 0; i < y_r * y_c; ++i)
    {
        if (F(x, y[i]) == B)
        {
            return B;
        }
    }

    return !B;
}

template<bool B, typename T, typename U, bool (F)(T, U)>
inline int breakOnMS(const T * x, const int64_t x_r, const int64_t x_c, const U y)
{
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        if (F(x[i], y) == B)
        {
            return B;
        }
    }

    return !B;
}

template<typename T, typename V, V (F)(T)>
inline void unaryM(const T * x, const int64_t x_r, const int64_t x_c, V ** o)
{
    V * _o;
    const int64_t size = x_r * x_c;
    if (*o)
    {
        _o = *o;
    }
    else
    {
        _o = new V[size];
        *o = _o;
    }
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        _o[i] = F(x[i]);
    }
}

template<typename T, typename V, V (F)(T)>
inline void unaryU(const T * x, const int64_t x_r, const int64_t x_c, V ** o)
{
    unaryM<T, V, F>(x, x_r, x_c, o);
}

template<typename T, typename U, typename V = typename ScilabType<T, U>::type>
inline void timesMM(const T * x, const int64_t x_r, const int64_t x_c, const U * y, const int64_t y_r, const int64_t y_c, V ** o)
{
    V * _o;
    const int64_t size = x_r * y_c;
    if (*o)
    {
        _o = *o;
    }
    else
    {
        _o = new V[size];
        *o = _o;
    }
    memset(_o, 0, size * sizeof(V));
    for (int j = 0; j < y_c; ++j)
    {
        for (int k = 0; k < x_c; ++k)
        {
            for (int i = 0; i < x_r; ++i)
            {
                _o[i + x_r * j] += (V)(x[i + x_r * k] * y[k + x_c * j]);
            }
        }
    }
}

// Useful to simplify something like find(A >= B)
// It would avoid to calculate A>=B and to search the true values after.
template<typename T, typename U, typename V, V (F)(T, U)>
inline int findBinMM(const T * x, const int64_t x_r, const int64_t x_c, const U * y, double ** o)
{
    std::vector<double> v;
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        if (F(x[i], y[i]))
        {
            v.push_back((double)i);
        }
    }

    const size_t s = v.size();
    *o = new double[s];
    memcpy(*o, &(v[0]), s * sizeof(double));

    return s;
}

template<typename T, typename U, typename V, V (F)(T, U)>
inline int findBinSM(const T x, const int64_t x_r, const int64_t x_c, const U * y, double ** o)
{
    std::vector<double> v;
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        if (F(x, y[i]))
        {
            v.push_back((double)i);
        }
    }

    const size_t s = v.size();
    *o = new double[s];
    memcpy(*o, &(v[0]), s * sizeof(double));

    return s;
}

template<typename T, typename U, typename V, V (F)(T, U)>
inline int findBinMS(const T * x, const int64_t x_r, const int64_t x_c, U y, double ** o)
{
    std::vector<double> v;
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        if (F(x[i], y))
        {
            v.push_back((double)i);
        }
    }

    const size_t s = v.size();
    *o = new double[s];
    memcpy(*o, &(v[0]), s * sizeof(double));

    return s;
}

inline void DGEMM(double * x, int & x_r, int & x_c, int & y_c, double * y, double * o)
{
    static char n = 'n';
    static double zero = 0;
    static double one = 1;

    C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, x, &x_r, y, &x_c, &zero, o, &x_r);
}

template<>
inline void timesMM<double, double, double>(const double * x, const int64_t x_r, const int64_t x_c, const double * y, const int64_t y_r, const int64_t y_c, double ** o)
{
    double * _o;
    const int64_t size = x_r * y_c;
    if (*o)
    {
        _o = *o;
    }
    else
    {
        _o = new double[size];
        *o = _o;
    }

    int _x_r = x_r;
    int _x_c = x_c;
    int _y_c = y_c;
    DGEMM(const_cast<double *>(x), _x_r, _x_c, _y_c, const_cast<double *>(y), _o);
}

inline void DSQM(double * x, double * y, int x_r, double * o)
{
    DGEMM(x, x_r, x_r, x_r, y, o);
}

inline void inv(double * X, int x_r)
{
    static char one = '1';
    static int iONE = 1;
    static int iMONE = -1;

    double norm1 = C2F(dlange)(&one, &x_r, &x_r, X, &x_r, nullptr);
    int info;
    int * pivot = new int[x_r];
    C2F(dgetrf)(&x_r, &x_r, X, &x_r, pivot, &info);
    if (info > 0)
    {
        // Singular matrix (a diagonal element of U is exactly 0)
        std::cout << "Singular matrix" << std::endl;
        delete[] pivot;
    }
    else
    {
        double rcond = 0;
        int size = std::max(4, C2F(ilaenv)(&iONE, "dgetri", " ", &x_r, &iMONE, &iMONE, &iMONE)) * x_r;
        double * work = new double[size];
        int * iwork = new int[x_r];
        C2F(dgecon)(&one, &x_r, X, &x_r, &norm1, &rcond, work, iwork, &info);
        if (rcond <= std::sqrt(C2F(dlamch)("e", 1L)))
        {
            // Ill conditionned
            std::cout << "Ill conditionned" << std::endl;
        }
        C2F(dgetri)(&x_r, X, &x_r, pivot, work, &size, &info);
        delete[] pivot;
        delete[] work;
        delete[] iwork;
    }
}

inline void inv(double * X, int & x_r, double * O)
{
    memcpy(O, X, x_r * x_r * sizeof(double));
    inv(O, x_r);
}

template<typename T>
inline void sq22(T * X)
{
    const T bc = X[1] * X[2];
    const T a_d = X[0] + X[3];
    X[0] = X[0] * X[0] + bc;
    X[1] *= a_d;
    X[2] *= a_d;
    X[3] = X[3] * X[3] + bc;
}

template<typename T>
inline void pow2ui(T * X, uint64_t N)
{
    switch (N)
    {
        case 0:
            X[0] = X[3] = 1;
            X[1] = X[2] = 0;
            return;
        case 1:
            return;
        case 2:
        {
            sq22(X);
            return;
        }
        default:
        {
            T y0, y1, y2, y3;
            if (N & 1)
            {
                y0 = X[0];
                y1 = X[1];
                y2 = X[2];
                y3 = X[3];
            }
            else
            {
                y0 = 1;
                y1 = 0;
                y2 = 0;
                y3 = 1;
            }

            while (N >>= 1)
            {
                sq22(X);
                if (N & 1)
                {
                    T a, b, c, d;
                    a = y0 * X[0] + y2 * X[1];
                    b = y0 * X[2] + y2 * X[3];
                    c = y1 * X[0] + y3 * X[1];
                    d = y1 * X[2] + y3 * X[3];
                    y0 = a;
                    y1 = c;
                    y2 = b;
                    y3 = d;
                }

            }

            X[0] = y0;
            X[1] = y1;
            X[2] = y2;
            X[3] = y3;
        }
    }
}

template<typename T>
inline void pow2ui(T * X, uint64_t N, T * O)
{
    O[0] = X[0];
    O[1] = X[1];
    O[2] = X[2];
    O[3] = X[3];
    pow2ui(O, N);
}

inline void powM(double * X, const int x_r, double * O, uint64_t N, double * T)
{
    /*
      This is not exactly the fast pow algorithm.
      I slightly modified it to avoid the dcopys.
      The idea is easy: we want the result in o so
      if N is even then o = t1*t1 else o=x*t1 and t1=o*o
      and we can replace N by N/2.
      If we were in the first case and N is even then t1=o*o
      else o=x*t1 and t1=o*o,...
      So to generate the dgemms we need to begin with the most signifiant bit.
      For example:
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, x, &x_r, &zero, o, &x_r); // x^2
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, o, &x_r, &zero, t1, &x_r); // x^3
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, t1, &x_r, t1, &x_r, &zero, o, &x_r); // x^6
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, o, &x_r, o, &x_r, &zero, t1, &x_r); // x^12
      C2F(dgemm)(&n, &n, &x_r, &x_r, &x_r, &one, x, &x_r, t1, &x_r, &zero, o, &x_r); // x^13

      The goal is to have one temp array, no memcpy and the final multiplication must put its result in O.
    */

    if (x_r == 2)
    {
        pow2ui(X, N, O);
        return;
    }

    const int size = x_r * x_r;

    if (N == 0)
    {
        // Identity matrix
        memset(O, 0, size * sizeof(double));
        for (int i = 0; i < x_r; ++i)
        {
            O[i * (x_r + 1)] = 1;
        }
        return;
    }
    else if (N == 1)
    {
        // X itself
        memcpy(O, X, size * sizeof(double));
        return;
    }
    else if (N == 2)
    {
        DSQM(X, X, x_r, O);
        return;
    }

    //const unsigned int nbits = 8 * sizeof(uint64_t) - __builtin_clzll(N);
    //const bool parity = (nbits - __builtin_popcountll(N)) & 1;
    const unsigned int nbits = 8 * sizeof(uint64_t) - analysis::tools::clzll(N);
    const bool parity = (nbits - analysis::tools::popcount(N)) & 1;
    unsigned int mask = 1 << (nbits - 2);
    const bool alloc = !T;
    if (alloc)
    {
        T = new double[size];
    }

    // Trip cost is [log2(N)] (=nbits-1)
    // Except the most signifiant bit, for each 1 in binary representation of N we have 2 dgemms
    // and for each 0 we have 1 dgemm.
    // E.g. for N=13=0x1101, the cost is 2 * 2 + 1 dgemms.

    if (N & mask)
    {
        if (parity)
        {
            DSQM(X, X, x_r, O);
            DSQM(X, O, x_r, T);
            std::swap(O, T);
        }
        else
        {
            DSQM(X, X, x_r, T);
            DSQM(X, T, x_r, O);
        }
    }
    else
    {
        if (parity)
        {
            DSQM(X, X, x_r, O);
        }
        else
        {
            DSQM(X, X, x_r, T);
            std::swap(O, T);
        }
    }

    while (mask >>= 1)
    {
        DSQM(O, O, x_r, T);
        if (N & mask)
        {
            DSQM(X, T, x_r, O);
        }
        else
        {
            std::swap(O, T);
        }
    }

    if (alloc)
    {
        delete[] T;
    }
}

inline void powMatHelper(double * X, const int x_r, double * O, const int64_t N, double * T)
{
    // TODO: Actually the X^d where d is not an integer is made in the macro %s_p_s.sci
    // So we should probably rewrite it in C++...
    if (N >= 0)
    {
        powM(X, x_r, O, (uint64_t)N, T);
    }
    else
    {
        powM(X, x_r, O, (uint64_t)(-N), T);
        inv(O, x_r);
    }
}

inline void powMat(double * X, const int64_t x_r, const int64_t N, double ** O)
{
    // TODO: Actually the X^d where d is not an integer is made in the macro %s_p_s.sci
    // So we should probably rewrite it in C++...
    double * _O;
    if (*O)
    {
        _O = *O;
    }
    else
    {
        _O = new double[x_r * x_r];
        *O = _O;
    }
    powMatHelper(X, (int)x_r, _O, N, nullptr);
}

inline void powM(double * X, const int x_r, int64_t N)
{
    if (N >= 0)
    {
        powM(X, x_r, N);
    }
    else
    {
        powM(X, x_r, -N);
        inv(X, x_r);
    }
}

inline void powM(double * X, const int x_r, uint64_t N)
{
    if (x_r == 2)
    {
        pow2ui(X, N);
        return;
    }

    const int size = x_r * x_r;

    if (N == 0)
    {
        memset(X, 0, size * sizeof(double));
        for (int i = 0; i < x_r; ++i)
        {
            X[i * (x_r + 1)] = 1;
        }
        return;
    }
    else if (N == 1)
    {
        return;
    }
    else if (N == 2)
    {
        double * T1 = new double[size];
        DSQM(X, X, x_r, T1);
        memcpy(X, T1, size * sizeof(double));
        return;
    }

    double * T1 = new double[size];
    const uint64_t l2N = analysis::tools::clzll(N);
    if ((l2N & 1) && (N == (1 << l2N)))
    {
        // N is a power of 4
        while (N >>= 2)
        {
            DSQM(X, X, x_r, T1);
            DSQM(T1, T1, x_r, X);
        }
        delete[] T1;
        return;
    }

    double * T2 = new double[size];
    powM(X, x_r, T1, N >> 1, T2);
    if (N & 1)
    {
        DSQM(X, T1, x_r, T2);
        DSQM(T1, T2, x_r, X);
    }
    else
    {
        DSQM(T1, T1, x_r, X);
    }

    delete[] T1;
    delete[] T2;
}

template<typename T>
inline T powui(T x, uint64_t n)
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

template<typename T>
inline T powi(T x, int64_t n)
{
    if (n >= 0)
    {
        return powui(x, n);
    }
    else
    {
        return 1 / powui(x, (uint64_t)(-n));
    }
}

inline void powui(double & r, double & i, uint64_t n)
{
    if (i == 0)
    {
        r = powui(r, n);
        return;
    }

    if (r == 0)
    {
        const double x = powui(i, n);
        switch (n % 4)
        {
            case 0:
                r = x;
                i = 0;
                return;
            case 1:
                r = 0;
                i = x;
                return;
            case 2:
                r = -x;
                i = 0;
                return;
            case 3:
                r = 0;
                i = -x;
                return;
        }
    }

    double p_r = r;
    double p_i = i;
    double y_r, y_i;

    if (n & 1)
    {
        y_r = r;
        y_i = i;
    }
    else
    {
        y_r = 1;
        y_i = 0;
    }

    while (n >>= 1)
    {
        const double s = p_r;
        p_r = p_r * p_r - p_i * p_i;
        p_i *= 2 * s;
        if (n & 1)
        {
            const double s = y_r;
            y_r = y_r * p_r - y_i * p_i;
            y_i = y_i * p_r + s * p_i;
        }
    }

    r = y_r;
    i = y_i;
}

inline void powi(double & r, double & i, int64_t n)
{
    if (n >= 0)
    {
        powui(r, i, n);
    }
    else
    {
        powui(r, i, -n);
        const double d = r * r + i * i;
        r /= d;
        i /= -d;
    }
}

template<typename T, typename U, typename V = typename ScilabType<T, U>::type>
inline V modulo(T x, U y)
{
    return x % y;
}

inline double modulo(double x, double y)
{
    return std::fmod(x, y);
}

template<typename T, typename U = T>
inline U opposite(T x)
{
    return -x;
}

template<typename T, typename U = T>
inline U pow2(T x)
{
    return x * x;
}

template<typename T, typename U = T>
inline U powm1(T x)
{
    return 1 / x;
}

template<typename T>
inline int asbool(T x)
{
    return x != 0;
}

template<typename T>
inline int boolneg(T x)
{
    return !x;
}

template<typename T>
inline bool asBool(T x)
{
    return x != 0;
}

template<typename T>
inline bool Boolneg(T x)
{
    return !x;
}

template<typename T, typename U, typename V = typename ScilabType<T, U>::type>
inline V binand(T x, U y)
{
    return x & y;
}

template<typename T, typename U, typename V = typename ScilabType<T, U>::type>
inline V binor(T x, U y)
{
    return x | y;
}

template<typename T, typename U, typename V = typename ScilabType<T, U>::type>
inline V binxor(T x, U y)
{
    return x ^ y;
}

template<typename T, typename U = T>
inline U binneg(T x)
{
    return ~x;
}

template<typename T, typename U, typename V = bool>
inline V booland(T x, U y)
{
    return x && y;
}

template<typename T, typename U, typename V = bool>
inline V boolor(T x, U y)
{
    return x || y;
}

template<typename T, typename U, typename V = bool>
inline V boolxor(T x, U y)
{
    return !x != !y;
}

template<typename T, typename U = bool>
inline U boolneg(T x)
{
    return !x;
}

template<typename T>
inline bool isStrictNegative(T x)
{
    return x < 0;
}

template<typename T>
inline bool isStrictPositive(T x)
{
    return x > 0;
}

template<typename T>
inline bool isAbsGreaterThan1(T x)
{
    return std::fabs(x) > 1;
}

template<typename T>
inline bool getFalse(T x)
{
    return false;
}

template<typename T>
inline bool getTrue(T x)
{
    return true;
}


inline void makeImplicitList(const double start, const double step, const double end, double *& out, int & r, int & c)
{
    double val;
    int type = analysis::ForList64::checkList(start, end, step, val);

    switch (type)
    {
        case 0:
            out = nullptr;
            r = c = 0;
            break;
        case 1:
            out = new double[1];
            out[0] = val;
            r = c = 1;
            break;
        case 2:
        {
            uint64_t N = analysis::ForList64::size(start, end, step);
            out = new double[N];
            r = 1;
            c = N;
            double _s = start;
            for (uint64_t i = 0; i < N; ++i, _s += step)
            {
                out[i] = _s;
            }
            break;
        }
        default:
            break;
    }
}

template<typename T>
inline void __obliviousTranspose(const unsigned int rb, const unsigned int re, const unsigned int cb, const unsigned int ce, const T * const X, const int64_t x_stride, T * const O, const int64_t o_stride)
{
    // Algorithm found here: http://cacs.usc.edu/education/cs653/Frigo-CacheOblivious-FOCS99.pdf

    const unsigned int dr = re - rb;
    const unsigned int dc = ce - cb;

    if (dr <= 16 && dc <= 16)
    {
        for (unsigned int i = rb; i < re; ++i)
        {
            for (unsigned int j = cb; j < ce; ++j)
            {
                O[j + i * o_stride] = X[i + j * x_stride];
            }
        }
    }
    else if (dr >= dc)
    {
        __obliviousTranspose(rb, rb + dr / 2, cb, ce, X, x_stride, O, o_stride);
        __obliviousTranspose(rb + dr / 2, re, cb, ce, X, x_stride, O, o_stride);
    }
    else
    {
        __obliviousTranspose(rb, re, cb, cb + dc / 2, X, x_stride, O, o_stride);
        __obliviousTranspose(rb, re, cb + dc / 2, ce, X, x_stride, O, o_stride);
    }
}

template<typename T>
inline void oTransp(const T * const X, const int64_t x_r, const int64_t x_c, T * const O, const int64_t o_r, const int64_t o_c)
{
    __obliviousTranspose(0, x_r, 0, x_c, X, x_r, O, o_r);
}

inline void transp(const double * X, const int64_t x_r, const int64_t x_c, double * O)
{
    for (int i = 0; i < x_r; ++i)
    {
        for (int j = 0; j < x_c; ++j)
        {
            O[j + x_c * i] = X[i + x_r * j];
        }
    }
}

inline void transp(double * X, const int64_t x_r)
{
    for (int i = 1; i < x_r; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            std::swap(X[i + x_r * j], X[j + x_r * i]);
        }
    }
}

inline void transp(double * X, const int64_t x_r, const int64_t x_c)
{
    if (x_r == x_c)
    {
        // Square matrix so transp can be done in place no need to have a temp
        transp(X, x_r);
    }
    else
    {
        const int s = x_r * x_c;
        double * O = new double[s];
        oTransp(X, x_r, x_c, O, x_c, x_r);
        memcpy(X, O, s * sizeof(double));
        delete[] O;
    }
}

/*inline void rdiv(double * X, const int64_t x_r, const int64_t x_c, double * Y, const int64_t y_r, const int64_t y_c, double ** O)
{
    // TODO: deal with warning messages...
    // We know that x_c == y_c
    // X/Y = (Y'\X')'

    static char one = '1';
    static char N = 'N';
    static const double eps = C2F(dlamch)("e", 1L);

    const int64_t max = std::max(y_r, y_c);
    const int64_t min = std::min(y_r, y_c);
    const int64_t size = y_r * y_c;
    int info;
    double * Xt = new double[max * x_r];
    double * Yt = new double[size];

    transposition::oTransp(X, x_r, x_c, Xt, max, x_r);
    transposition::oTransp(Y, y_r, y_c, Yt, y_c, y_r);

    bool lsq = y_r != y_c;
    double * sYt = nullptr;
    int x_r_i = (int)x_r;
    int x_c_i = (int)x_c;
    int y_r_i = (int)y_r;
    int y_c_i = (int)y_c;

    if (!lsq)
    {
        double norm1 = C2F(dlange)(&one, &y_r_i, &y_c_i, Y, &y_r_i, nullptr);
        int * pivot = new int[min];
        // Y is modified by dgetrf so we need to copy to use eventually in lsq method
        sYt = new double[size];
        memcpy(sYt, Yt, sizeof(double) * size);
        C2F(dgetrf)(&y_c_i, &y_r_i, Yt, &y_c_i, pivot, &info);
        if (info > 0)
        {
            // Singular matrix (a diagonal element of U is exactly 0)
            std::wcerr << "Singular matrix" << std::endl;
            lsq = true;
            delete[] pivot;
        }
        else
        {
            double rcond = 0;
            double * work = new double[4 * y_c];
            int * iwork = new int[y_c];
            C2F(dgecon)(&one, &y_c_i, Yt, &y_c_i, &norm1, &rcond, work, iwork, &info);
            if (rcond <= 10 * eps)
            {
                // Ill conditionned
                std::wcerr << "Ill conditionned" << std::endl;
                lsq = true;
            }
            else
            {
                C2F(dgetrs)(&N, &y_c_i, &x_r_i, Yt, &y_c_i, pivot, Xt, &y_c_i, &info);
            }

            delete[] pivot;
            delete[] work;
            delete[] iwork;
        }
    }

    if (lsq)
    {
        int * jpvt = new int[y_r]();
        int max_i = (int)max;
        const double rcond = 10 * eps;
        int rank;
        int workMin = std::max(4 * y_c, std::max(min + 3 * y_r + 1, 2 * min + x_r));
        double * work = new double[workMin];
        info = 1;
        if (sYt)
        {
            delete[] Yt;
            Yt = sYt;
        }
        C2F(dgelsy1)(&y_c_i, &y_r_i, &x_r_i, Yt, &y_c_i, Xt, &max_i, jpvt, &rcond, &rank, work, &workMin, &info);
        if (y_r != y_c && rank < min)
        {
            // useless
        }

        delete[] jpvt;
        delete[] work;
    }

    if (!*O)
    {
	*O = new double[x_r * y_r];
    }
    transposition::oTransp(Xt, max, x_r, *O, x_r, y_r);
    
    delete[] Xt;
    delete[] Yt;
}
*/

/*        template<>
        inline wchar_t * sum<wchar_t *, wchar_t *, wchar_t *>(wchar_t * x, wchar_t * y)
        {
            const size_t lenx = wcslen(x) * sizeof(wchar_t);
            const size_t leny = (wcslen(y) + 1) * sizeof(wchar_t);
            void * const s = MALLOC(lenx + leny);
            memcpy(s, x, lenx);
            memcpy((char *)s + lenx, y, leny);

            return (wchar_t *)s;
        }
*/

/*template<>
  inline void binSM<wchar_t *, wchar_t *, wchar_t *, sum<wchar_t *, wchar_t *, wchar_t *>>(wchar_t * x, wchar_t * const * y, const int64_t y_r, const int64_t y_c, wchar_t ** o)
  {
  const size_t lenx = wcslen(x) * sizeof(wchar_t);
  for (int64_t i = 0; i < y_r * y_c; ++i)
  {
  const size_t leny = (wcslen(y[i]) + 1) * sizeof(wchar_t);
  void * const s = MALLOC(lenx + leny);
  memcpy(s, x, lenx);
  memcpy((char *)s + lenx, y[i], leny);
  o[i] = (wchar_t *)s;
  }
  }

  template<>
  inline void binMS<wchar_t *, wchar_t *, wchar_t *, sum<wchar_t *, wchar_t *, wchar_t *>>(wchar_t * const * x, const int64_t x_r, const int64_t x_c, wchar_t * y, wchar_t ** o)
  {
  const size_t leny = (wcslen(y) + 1) * sizeof(wchar_t);
  for (int64_t i = 0; i < x_r * x_c; ++i)
  {
  const size_t lenx = wcslen(x[i]) * sizeof(wchar_t);
  void * const s = MALLOC(lenx + leny);
  memcpy(s, x[i], lenx);
  memcpy((char *)s + lenx, y, leny);
  o[i] = (wchar_t *)s;
  }
  }
*/

template<unsigned int N>
inline wchar_t * sum(wchar_t ** x)
{
    static_assert(N >= 3, "More than 3 strings expected in the sum");
    size_t lens[N];
    lens[0] = wcslen(x[0]) * sizeof(wchar_t);
    for (unsigned int i = 1; i < N - 1; ++i)
    {
        lens[i] = lens[i - 1] + wcslen(x[i]) * sizeof(wchar_t);
    }
    lens[N - 1] = lens[N - 2] + (wcslen(x[N - 1]) + 1) * sizeof(wchar_t);

    void * const s = MALLOC(lens[N - 1]);
    memcpy(s, x[0], lens[0]);
    for (unsigned int i = 1; i < N; ++i)
    {
        memcpy((char *)s + lens[i - 1], x[i], lens[i] - lens[i - 1]);
    }

    return (wchar_t *)s;
}

} // namespace BaseFunctions
} // namespace jit

#endif // __BASE_FUNCTIONS_HXX__
