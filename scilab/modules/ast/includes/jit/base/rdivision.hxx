/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASE_RDIVISIONS_HXX__
#define __BASE_RDIVISIONS_HXX__

#include <complex>
#include <cstring>

#include "Cast.hxx"
#include "base/transposition.hxx"
#include "base/tools.hxx"

extern "C"
{
#include "elem_common.h"

    extern void C2F(dgetri)(int const* n, double* a, int const* ldA, int const* iPiv, double* work, int const* workSize, int* info);
    extern void C2F(zgetri)( int const* n, doublecomplex* a, int const* ldA, int const* iPiv, doublecomplex* work, int const* workSize, int* info);
    extern void C2F(dlaswp)(int const* n, double* a, int const* ldA, int const* k1, int const* k2, int const* iPiv, int const* incX);
    extern int C2F(dtrsm) (char* side, char* uplo, char* trans, char* diag, int* m, int* n, double* alpha, double* A, int* lda, double* B, int* ldb);
    extern int C2F(ilaenv)(int *, const char *, const char *, int *, int *, int *, int *);

}

namespace jit
{
    namespace op
    {

        template<typename T, typename U, typename V>
        struct Rdiv
        {

            inline V operator()(T x, U y)
                {
                    return (V)x / (V)y;
                }
        };

        template<typename U>
        struct Rdiv<const std::complex<double> &, U, std::complex<double>>
        {

            inline std::complex<double> operator()(const std::complex<double> & x, U y)
            {
                return x / (double)y;
            }
        };

        template<typename T>
        struct Rdiv<T, const std::complex<double> &, std::complex<double>>
        {

            inline std::complex<double> operator()(T x, const std::complex<double> & y)
            {
                return (double)x / y;
            }
        };

        template<>
        struct Rdiv<const std::complex<double> &, const std::complex<double> &, std::complex<double>>
        {

            inline std::complex<double> operator()(const std::complex<double> & x, const std::complex<double> & y)
            {
                return x / y;
            }
        };
    } // namespace op

    namespace rdivision
    {

	template<bool X_IDENTITY = false>
        inline void RDIV(const double * x, const int64_t x_r, const int64_t x_c, const int64_t y_r, const double * y, double * o)
        {
            // TODO: deal with warning messages...
            // x/y = (y'\x')'
            static char one = '1';
            static char N = 'N';
            static const double eps = C2F(dlamch)("e", 1L);

            const int64_t y_c = x_c;
            int64_t max, min;
            if (y_r < y_c)
            {
                min = y_r;
                max = y_c;
            }
            else
            {
                min = y_c;
                max = y_r;
            }
            const int64_t size = y_r * y_c;
            int info;
            double * xt = new double[max * x_r];
            double * yt = new double[size];

	    if (X_IDENTITY)
	    {
		std::memset(xt, 0, max * x_r * sizeof(double));
		for (int64_t i = 0; i < x_r; ++i)
		{
		    xt[i * (max + 1)] = *x;
		}
	    }
	    else
	    {
		transposition::oTransp(x, x_r, x_c, xt, max, x_r);
	    }
            transposition::oTransp(y, y_r, y_c, yt, y_c, y_r);

            bool lsq = y_r != y_c;
            double * syt = nullptr;
            int x_r_i = (int)x_r;
            int x_c_i = (int)x_c;
            int y_r_i = (int)y_r;
            int y_c_i = (int)y_c;

            if (!lsq)
            {
                int * pivot = new int[min];
                // y is modified by dgetrf so we need to copy to use eventually in lsq method
                syt = new double[size];
                std::memcpy(syt, yt, sizeof(double) * size);
                C2F(dgetrf)(&y_c_i, &y_r_i, yt, &y_c_i, pivot, &info);
                if (info > 0)
                {
                    // Singular matrix (a diagonal element of U is exactly 0)
                    lsq = true;
                }
                else
                {
                    double rcond = 0;
                    double * work = new double[4 * y_c];
                    int * iwork = new int[y_c];
                    double norm1 = C2F(dlange)(&one, &y_r_i, &y_c_i, const_cast<double *>(y), &y_r_i, nullptr);
                    C2F(dgecon)(&one, &y_c_i, yt, &y_c_i, &norm1, &rcond, work, iwork, &info);
                    if (rcond <= 10 * eps)
                    {
                        // Ill conditionned
                        lsq = true;
                    }
                    else
                    {
                        C2F(dgetrs)(&N/*o transpose*/, &y_c_i, &x_r_i, yt, &y_c_i, pivot, xt, &y_c_i, &info);
                    }

                    delete[] work;
                    delete[] iwork;
                }
                delete[] pivot;
            }

            if (lsq)
            {
                int * jpvt = new int[y_r]();
                int max_i = (int)max;
                double rcond = 10 * eps;
                int rank;
                int workMin = std::max(4 * y_c, std::max(min + 3 * y_r + 1, 2 * min + x_r));
                double * work = new double[workMin];
                info = 1;
                if (syt)
                {
                    delete[] yt;
                    yt = syt;
                    syt = nullptr;
                }
                C2F(dgelsy1)(&y_c_i, &y_r_i, &x_r_i, yt, &y_c_i, xt, &max_i, jpvt, &rcond, &rank, work, &workMin, &info);
                if (y_r != y_c && rank < min)
                {
                    // useless
                }

                delete[] jpvt;
                delete[] work;
            }

	    transposition::oTransp(xt, y_r, x_r, max, o, x_r);

            delete[] xt;
            delete[] yt;
            delete[] syt;
        }

	template<bool X_IDENTITY = false>
        inline void rdivCC(const int64_t x_r, const int64_t x_c, const int64_t y_r, const double * x_re, const double * x_im, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
        {
            // TODO: deal with warning messages...
            // x/y = (y'\x')'

            static char one = '1';
            static char N = 'N';
            static const double eps = C2F(dlamch)("e", 1L);

            const int64_t y_c = x_c;
            int64_t max, min;
            if (y_r < y_c)
            {
                min = y_r;
                max = y_c;
            }
            else
            {
                min = y_c;
                max = y_r;
            }
            const int64_t size = y_r * y_c;
            int info;
            double * xt = new double[2 * max * x_r];
            double * yt = new double[2 * size];
            double * _y = new double[2 * size];

            if (y_im)
            {
                for (int64_t i = 0; i < size; ++i)
                {
                    _y[2 * i] = y_re[i];
                    _y[2 * i + 1] = y_im[i];
                }
            }
            else
            {
                for (int64_t i = 0; i < size; ++i)
                {
                    _y[2 * i] = y_re[i];
                    _y[2 * i + 1] = 0;
                }
            }

	    if (X_IDENTITY)
	    {
		std::memset(xt, 0, 2 * max * x_r * sizeof(double));
		const double _x_re = *x_re;
		const double m_x_im = -*x_im;
		for (int64_t i = 0; i < x_r; ++i)
		{
		    xt[2 * i * (max + 1)] = _x_re;
		    xt[2 * i * (max + 1) + 1] = m_x_im;
		}
	    }
	    else
	    {
		transposition::oConj(x_re, x_im, x_r, x_c, xt, max, x_r);
	    }

            transposition::oConj(y_re, y_im, y_r, y_c, yt, y_c, y_r);

            bool lsq = y_r != y_c;
            double * syt = nullptr;
            int x_r_i = (int)x_r;
            int x_c_i = (int)x_c;
            int y_r_i = (int)y_r;
            int y_c_i = (int)y_c;

            if (!lsq)
            {
                int * pivot = new int[min];
                // y is modified by dgetrf so we need to copy to use eventually in lsq method
                syt = new double[2 * size];
                std::memcpy(syt, yt, 2 * sizeof(double) * size);
                C2F(zgetrf)(&y_c_i, &y_r_i, reinterpret_cast<doublecomplex *>(yt), &y_c_i, pivot, &info);
                if (info > 0)
                {
                    // Singular matrix (a diagonal element of U is exactly 0)
                    lsq = true;
                }
                else
                {
                    double rcond = 0;
                    double * work = new double[2 * 2 * y_c];
                    double * rwork = new double[2 * y_c];
                    double norm1 = C2F(zlange)(&one, &y_r_i, &y_c_i, _y, &y_r_i, nullptr);
                    C2F(zgecon)(&one, &y_c_i, reinterpret_cast<doublecomplex *>(yt), &y_c_i, &norm1, &rcond, reinterpret_cast<doublecomplex *>(work), rwork, &info);
                    if (rcond <= 10 * eps)
                    {
                        // Ill conditionned
                        lsq = true;
                    }
                    else
                    {
                        C2F(zgetrs)(&N/*o transpose*/, &y_c_i, &x_r_i, reinterpret_cast<doublecomplex *>(yt), &y_c_i, pivot, reinterpret_cast<doublecomplex *>(xt), &y_c_i, &info);
                    }

                    delete[] work;
                    delete[] rwork;
                }
                delete[] pivot;
            }

            if (lsq)
            {
                int * jpvt = new int[y_r]();
                int max_i = (int)max;
                double rcond = 10 * eps;
                int rank;
                int workMin = std::max(2 * y_c, min + std::max(2 * min, std::max(y_r + 1, min + x_r)));
                double * work = new double[2 * workMin];
                double * rwork = new double[2 * y_r];
                info = 1;
                if (syt)
                {
                    delete[] yt;
                    yt = syt;
                    syt = nullptr;
                }
                C2F(zgelsy1)(&y_c_i, &y_r_i, &x_r_i, reinterpret_cast<doublecomplex *>(yt), &y_c_i, reinterpret_cast<doublecomplex *>(xt), &max_i, jpvt, &rcond, &rank, reinterpret_cast<doublecomplex *>(work), &workMin, rwork, &info);
                if (y_r != y_c && rank < min)
                {
                    // useless
                }

                delete[] jpvt;
                delete[] work;
                delete[] rwork;
            }

            if (!*o_re)
            {
                *o_re = new double[x_r * y_r];
                *o_im = new double[x_r * y_r];
            }

	    //transposition::oTransp(xt, y_r, x_r, max, o, x_r);
            //transposition::oConj(xt, max, x_r, *o_re, *o_im, x_r, y_r);
	    transposition::oConj(xt, y_r, x_r, max, *o_re, *o_im, x_r);

            delete[] xt;
            delete[] yt;
            delete[] syt;
        }

        // double / int?
        template<typename T>
        inline void rdiv(const int64_t x_r, const int64_t x_c, const int64_t y_r, const double * x, const T * y, T ** o)
        {
            const int64_t x_size = x_r * x_c;
            const int64_t y_size = y_r * x_c;
            const int64_t o_size = x_r * y_r;
            double * _x = new double[x_size];
            double * _y = new double[y_size];
            double * _o = new double[o_size];
            for (int64_t i = 0; i < x_size; ++i)
            {
                _x[i] = (double)((T)x[i]);
            }
            for (int64_t i = 0; i < y_size; ++i)
            {
                _y[i] = (double)y[i];
            }

            RDIV(_x, x_r, x_c, y_r, _y, _o);

            delete[] _x;
            delete[] _y;

            T * __o;
            if (*o)
            {
                __o = *o;
            }
            else
            {
                __o = new T[o_size];
                *o = __o;
            }

            for (int64_t i = 0; i < o_size; ++i)
            {
                __o[i] = (T)_o[i];
            }

            delete[] _o;
        }

        // int? / double
        template<typename T>
        inline void rdiv(const int64_t x_r, const int64_t x_c, const int64_t y_r, const T * x, const double * y, T ** o)
        {
            const int64_t x_size = x_r * x_c;
            const int64_t y_size = y_r * x_c;
            const int64_t o_size = x_r * y_r;
            double * _x = new double[x_size];
            double * _y = new double[y_size];
            double * _o = new double[o_size];
            for (int64_t i = 0; i < y_size; ++i)
            {
                _y[i] = (double)((T)y[i]);
            }
            for (int64_t i = 0; i < y_size; ++i)
            {
                _x[i] = (double)x[i];
            }

            RDIV(_x, x_r, x_c, y_r, _y, _o);

            delete[] _x;
            delete[] _y;

            T * __o;
            if (*o)
            {
                __o = *o;
            }
            else
            {
                __o = new T[o_size];
                *o = __o;
            }

            for (int64_t i = 0; i < o_size; ++i)
            {
                __o[i] = (T)_o[i];
            }

            delete[] _o;
        }

        // int? / int?
        template<typename T, typename U, typename V>
        inline void rdiv(const int64_t x_r, const int64_t x_c, const int64_t y_r, const T * x, const U * y, V ** o)
        {
            const int64_t x_size = x_r * x_c;
            const int64_t y_size = y_r * x_c;
            const int64_t o_size = x_r * y_r;
            double * _x = new double[x_size];
            double * _y = new double[y_size];
            double * _o = new double[o_size];
            for (int64_t i = 0; i < x_size; ++i)
            {
                _x[i] = (double)((V)x[i]);
            }
            for (int64_t i = 0; i < y_size; ++i)
            {
                _y[i] = (double)((V)y[i]);
            }

            RDIV(_x, x_r, x_c, y_r, _y, _o);

            delete[] _x;
            delete[] _y;

            V * __o;
            if (*o)
            {
                __o = *o;
            }
            else
            {
                __o = new V[o_size];
                *o = __o;
            }

            for (int64_t i = 0; i < o_size; ++i)
            {
                __o[i] = (V)_o[i];
            }

            delete[] _o;
        }

        // double / double
        template<>
        inline void rdiv<double, double, double>(const int64_t x_r, const int64_t x_c, const int64_t y_r, const double * x, const double * y, double ** o)
        {
            if (!*o)
            {
                *o = new double[x_r * y_r];
            }

            RDIV(x, x_r, x_c, y_r, y, *o);
        }

	// double / double
        inline void rinvSdMd(const int64_t y_r, const int64_t y_c, const double x, const double * y, double ** o)
        {
            if (!*o)
            {
                *o = new double[y_c * y_r];
            }

            RDIV<true>(&x, y_c, y_c, y_r, y, *o);
        }

	// complex / double
        inline void rinvScMd(const int64_t y_r, const int64_t y_c, const double x_re, const double x_im, const double * y, double ** o_re, double ** o_im)
        {
	    const int64_t size = y_c * y_r;
            if (!*o_re)
            {
                *o_re = new double[size];
		*o_im = new double[size];
            }
	    const double one = 1;

            RDIV<true>(&one, y_c, y_c, y_r, y, *o_re);
	    double * _o_re = *o_re;
	    double * _o_im = *o_im;
	    for (int64_t i = 0; i < size; ++i)
	    {
		_o_im[i] = _o_re[i] * x_im;
		_o_re[i] *= x_re;
	    }
        }

	// complex / complex
        inline void rinvScMc(const int64_t y_r, const int64_t y_c, const double x_re, const double x_im, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
        {
            rdivCC<true>(y_c, y_c, y_r, &x_re, &x_im, y_re, y_im, o_re, o_im);
        }

	// double / complex
        inline void rinvSdMc(const int64_t y_r, const int64_t y_c, const double x, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
        {
	    const double x_im = 0;
            rdivCC<true>(y_c, y_c, y_r, &x, &x_im, y_re, y_im, o_re, o_im);
        }

        inline void rdivCD(const int64_t x_r, const int64_t x_c, const int64_t y_r, const double * x_re, const double * x_im, const double * y, double ** o_re, double ** o_im)
        {
            // TODO: voir avec Tonio: cet implem est plus rapide
            const int64_t o_size = x_r * x_c;
            if (!*o_re)
            {
                *o_re = new double[o_size];
            }

            RDIV(x_re, x_r, x_c, y_r, y, *o_re);
            if (x_im)
            {
                if (!*o_im)
                {
                    *o_im = new double[o_size];
                }
                RDIV(x_im, x_r, x_c, y_r, y, *o_im);
            }
            else if (*o_im)
            {
                std::memset(*o_im, 0, sizeof(double) * o_size);
            }
        }

        inline void rdivDC(const int64_t x_r, const int64_t x_c, const int64_t y_r, const double * x, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
        {
            if (!*o_re)
            {
                const int64_t o_size = x_r * x_c;
                *o_re = new double[o_size];
                *o_im = new double[o_size];
            }

            rdivCC(x_r, x_c, y_r, x, nullptr, y_re, y_im, o_re, o_im);
        }

        inline void DINV(double * x, int & x_r)
        {
            static char one = '1';
            static int iONE = 1;
            static int iMONE = -1;
	    static const double eps = std::sqrt(C2F(dlamch)("e", 1L));

            int info;
            int * pivot = new int[x_r];
            C2F(dgetrf)(&x_r, &x_r, x, &x_r, pivot, &info);
            if (info > 0)
            {
                // Singular matrix (a diagonal element of x is exactly 0)
                delete[] pivot;
            }
            else
            {
                double rcond = 0;
                int size = std::max(4, C2F(ilaenv)(&iONE, "dgetri", " ", &x_r, &iMONE, &iMONE, &iMONE)) * x_r;
                double * work = new double[size];
                int * iwork = new int[x_r];
		double norm1 = C2F(dlange)(&one, &x_r, &x_r, x, &x_r, work);
                C2F(dgecon)(&one, &x_r, x, &x_r, &norm1, &rcond, work, iwork, &info);
                if (rcond <= eps)
                {
                    // Ill conditionned
                }
                C2F(dgetri)(&x_r, x, &x_r, pivot, work, &size, &info);
                delete[] pivot;
                delete[] work;
                delete[] iwork;
            }
        }

	inline void ZINV(double * x, int & x_r)
        {
            static char one = '1';
            static int iONE = 1;
            static int iMONE = -1;
	    static const double eps = std::sqrt(C2F(dlamch)("e", 1L));

            int info;
            int * pivot = new int[x_r];
            C2F(zgetrf)(&x_r, &x_r, reinterpret_cast<doublecomplex *>(x), &x_r, pivot, &info);
            if (info > 0)
            {
                // Singular matrix (a diagonal element of x is exactly 0)
                delete[] pivot;
            }
            else
            {
                double rcond = 0;
                int size = std::max(4, C2F(ilaenv)(&iONE, "zgetri", " ", &x_r, &iMONE, &iMONE, &iMONE)) * x_r;
                double * work = new double[2 * size];
                double * pwork = new double[2 * size];
		double norm1 = C2F(zlange)(&one, &x_r, &x_r, x, &x_r, nullptr);
                C2F(zgecon)(&one, &x_r, reinterpret_cast<doublecomplex *>(x), &x_r, &norm1, &rcond, reinterpret_cast<doublecomplex *>(work), pwork, &info);
                if (rcond <= eps)
                {
                    // Ill conditionned
                }
                C2F(zgetri)(&x_r, reinterpret_cast<doublecomplex *>(x), &x_r, pivot, reinterpret_cast<doublecomplex *>(work), &size, &info);
                delete[] pivot;
                delete[] work;
                delete[] pwork;
            }
        }

	inline void ZINV(double * x_re, double * x_im, int & x_r)
        {
	    double * x = new double[2 * x_r * x_r];
	    for (int i = 0; i < x_r * x_r; ++i)
	    {
		x[2 * i] = x_re[i];
		x[2 * i + 1] = x_im[i];
	    }

	    ZINV(x, x_r);

	    for (int i = 0; i < x_r * x_r; ++i)
	    {
		x_re[i] = x[2 * i];
		x_im[i] = x[2 * i + 1];
	    }
	    delete[] x;
	}

	inline void ZINV(cpx_t * x, int & x_r)
        {
	    ZINV(reinterpret_cast<double *>(x), x_r);
	}

    } // namespace rdivision

} // namespace jit

#endif // __BASE_RDIVISIONS_HXX__
