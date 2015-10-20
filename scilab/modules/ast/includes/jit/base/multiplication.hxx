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

#ifndef __BASE_MULTIPLICATION_HXX__
#define __BASE_MULTIPLICATION_HXX__

#include <cstdint>
#include <cstring>

#include "base/tools.hxx"

extern "C"
{
#include "elem_common.h"

    extern int C2F(zgemm) (char *_pstTransA, char *_pstTransB, int *_piN, int *_piM, int *_piK, doublecomplex *_pdblAlpha, doublecomplex *_pdblA, int *_piLdA, doublecomplex *_pdblB, int *_piLdB, doublecomplex *_pdblBeta, doublecomplex *_pdblC, int *_piLdC);
}

namespace jit
{

    namespace multiplication
    {

        inline void DGEMM(const double * x, int & x_r, int & x_c, int & y_c, const double * y, double * o)
        {
            static char n = 'n';
            static double zero = 0;
            static double one = 1;

            C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, const_cast<double *>(x), &x_r, const_cast<double *>(y), &x_c, &zero, o, &x_r);
        }

	inline void ZGEMM(const doublecomplex * x, int & x_r, int & x_c, int & y_c, const doublecomplex * y, doublecomplex * o)
        {
            static char n = 'n';
            static doublecomplex zero = {0., 0.};
            static doublecomplex one = {1., 0.};

            C2F(zgemm)(&n, &n, &x_r, &y_c, &x_c, &one, const_cast<doublecomplex *>(x), &x_r, const_cast<doublecomplex *>(y), &x_c, &zero, o, &x_r);
        }

	inline void ZGEMM(const cpx_t * x, int & x_r, int & x_c, int & y_c, const cpx_t * y, cpx_t * o)
        {
	    ZGEMM(reinterpret_cast<const doublecomplex *>(x), x_r, x_c, y_c, reinterpret_cast<const doublecomplex *>(y), reinterpret_cast<doublecomplex *>(o));
        }

	// complex * complex
        inline void ZGEMM(const double * x_re, const double * x_im, int & x_r, int & x_c, int & y_c, const double * y_re, const double * y_im, double * o_re, double ** o_im)
        {
            static char n = 'n';
            static double zero = 0;
            static double one = 1;
            static double m_one = -1;

	    const int o_size = x_r * y_c;
            double * _x_re = const_cast<double *>(x_re);
            double * _x_im = const_cast<double *>(x_im);
            double * _y_re = const_cast<double *>(y_re);
            double * _y_im = const_cast<double *>(y_im);

            // o_re = x_re * y_re
            C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, _x_re, &x_r, _y_re, &x_c, &zero, o_re, &x_r);

            if (x_im)
            {
		if (!*o_im)
		{
		    *o_im = new double[o_size];
		}
                if (y_im)
                {
                    // o_re = -x_im * y_im + o_re
                    C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &m_one, _x_im, &x_r, _y_im, &x_c, &one, o_re, &x_r);

                    // o_im = x_re * y_im
                    C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, _x_re, &x_r, _y_im, &x_c, &zero, *o_im, &x_r);
                    // o_im = x_im * y_re + o_im
                    C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, _x_im, &x_r, _y_re, &x_c, &one, *o_im, &x_r);
                }
                else
                {
                    // o_im = x_im * y_re
                    C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, _x_im, &x_r, _y_re, &x_c, &zero, *o_im, &x_r);
                }
            }
            else if (y_im)
            {
		if (!*o_im)
		{
		    *o_im = new double[o_size];
		}
                // o_im = x_re * y_im
                C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, _x_re, &x_r, _y_im, &x_c, &zero, *o_im, &x_r);
            }
	    else if (*o_im)
	    {
		std::memset(*o_im, 0, sizeof(double) * o_size);
	    }
        }

        // double * int?
        template<typename T>
        inline void times(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const T * y, T ** o)
        {
            const int64_t x_size = x_r * x_c;
            const int64_t y_size = x_c * y_c;
            const int64_t o_size = x_r * y_c;
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

            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            DGEMM(_x, _x_r, _x_c, _y_c, _y, _o);

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

        // int? * double
        template<typename T>
        inline void times(const int64_t x_r, const int64_t x_c, const int64_t y_c, const T * x, const double * y, T ** o)
        {
            const int64_t x_size = x_r * x_c;
            const int64_t y_size = x_c * y_c;
            const int64_t o_size = x_r * y_c;
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

            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            DGEMM(_x, _x_r, _x_c, _y_c, _y, _o);

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

        // int? * int?
        template<typename T, typename U, typename V>
        inline void times(const int64_t x_r, const int64_t x_c, const int64_t y_c, const T * x, const U * y, V ** o)
        {
            const int64_t x_size = x_r * x_c;
            const int64_t y_size = x_c * y_c;
            const int64_t o_size = x_r * y_c;
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

            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            DGEMM(_x, _x_r, _x_c, _y_c, _y, _o);

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

        // double * double
	template<>
        inline void times<double, double, double>(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const double * y, double ** o)
        {
            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            if (!*o)
            {
                *o = new double[x_r * y_c];
            }

            DGEMM(x, _x_r, _x_c, _y_c, y, *o);
        }

        // complex * complex
        inline void timesCC(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x_re, const double * x_im, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
        {
            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            if (!*o_re)
            {
                *o_re = new double[x_r * y_c];
            }

            ZGEMM(x_re, x_im, _x_r, _x_c, _y_c, y_re, y_im, *o_re, o_im);
        }

        // complex * double
        inline void timesCD(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x_re, const double * x_im, const double * y, double ** o_re, double ** o_im)
        {
	    const int64_t o_size = x_r * y_c;
            if (!*o_re)
            {
                *o_re = new double[o_size];
            }

            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            // o_re = x_re * y
            DGEMM(x_re, _x_r, _x_c, _y_c, y, *o_re);

            if (x_im)
            {
		if (!*o_im)
		{
		    *o_im = new double[o_size];
		}
                // o_im = x_im * y
                DGEMM(x_im, _x_r, _x_c, _y_c, y, *o_im);
            }
	    else if (*o_im)
	    {
		std::memset(*o_im, 0, sizeof(double) * o_size);
	    }
        }

        // double * complex
        inline void timesDC(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
        {
	    const int64_t o_size = x_r * y_c;
	    if (!*o_re)
            {
                *o_re = new double[o_size];
            }

            int _x_r = x_r;
            int _x_c = x_c;
            int _y_c = y_c;

            // o_re = x * y_re
            DGEMM(x, _x_r, _x_c, _y_c, y_re, *o_re);

            if (y_im)
            {
		if (!*o_im)
		{
		    *o_im = new double[o_size];
		}
                // o_im = x * y_im
                DGEMM(x, _x_r, _x_c, _y_c, y_im, *o_im);
            }
	    else if (*o_im)
	    {
		std::memset(*o_im, 0, sizeof(double) * o_size);
	    }
        }

    } // namespace multiplication

} // namespace jit

#endif // __BASE_MULTIPLICATION_HXX__
