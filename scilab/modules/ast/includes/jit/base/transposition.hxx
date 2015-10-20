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

#ifndef __BASE_TRANSPOSITION_HXX__
#define __BASE_TRANSPOSITION_HXX__

#include <cstdint>
#include <cstring>
#include <utility>

extern "C"
{
#include "elem_common.h"
}

namespace jit
{

    namespace transposition
    {

        template<typename T>
        inline void __obliviousTranspose(const unsigned int rb, const unsigned int re, const unsigned int cb, const unsigned int ce, const T * const x, const int64_t x_stride, T * const o, const int64_t o_stride)
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
                        o[j + i * o_stride] = x[i + j * x_stride];
                    }
                }
            }
            else if (dr >= dc)
            {
                __obliviousTranspose(rb, rb + dr / 2, cb, ce, x, x_stride, o, o_stride);
                __obliviousTranspose(rb + dr / 2, re, cb, ce, x, x_stride, o, o_stride);
            }
            else
            {
                __obliviousTranspose(rb, re, cb, cb + dc / 2, x, x_stride, o, o_stride);
                __obliviousTranspose(rb, re, cb + dc / 2, ce, x, x_stride, o, o_stride);
            }
        }

        template<typename T>
        inline void oTransp(const T * const x, const int64_t x_r, const int64_t x_c, T * const o, const int64_t o_r, const int64_t o_c)
        {
            __obliviousTranspose(0, x_r, 0, x_c, x, x_r, o, o_r);
        }

	template<typename T>
	inline void oTransp(const T * const x, const int64_t x_r, const int64_t x_c, const int64_t x_stride, T * const o, const int64_t o_stride)
        {
            __obliviousTranspose(0, x_r, 0, x_c, x, x_stride, o, o_stride);
        }

        inline void __obliviousConj(const unsigned int rb, const unsigned int re, const unsigned int cb, const unsigned int ce, const double * const x_re, const double * const x_im, const int64_t x_stride, double * const o_re, double * const o_im, const int64_t o_stride)
        {
            const unsigned int dr = re - rb;
            const unsigned int dc = ce - cb;

            if (dr <= 16 && dc <= 16)
            {
		if (x_im)
		{
		    for (unsigned int i = rb; i < re; ++i)
		    {
			for (unsigned int j = cb; j < ce; ++j)
			{
			    o_re[j + i * o_stride] = x_re[i + j * x_stride];
			    o_im[j + i * o_stride] = -x_im[i + j * x_stride];
			}
		    }
		}
		else
		{
		    for (unsigned int i = rb; i < re; ++i)
		    {
			for (unsigned int j = cb; j < ce; ++j)
			{
			    o_re[j + i * o_stride] = x_re[i + j * x_stride];
			}
		    }
		}
            }
            else if (dr >= dc)
            {
                __obliviousConj(rb, rb + dr / 2, cb, ce, x_re, x_im, x_stride, o_re, o_im, o_stride);
                __obliviousConj(rb + dr / 2, re, cb, ce, x_re, x_im, x_stride, o_re, o_im, o_stride);
            }
            else
            {
                __obliviousConj(rb, re, cb, cb + dc / 2, x_re, x_im, x_stride, o_re, o_im, o_stride);
                __obliviousConj(rb, re, cb + dc / 2, ce, x_re, x_im, x_stride, o_re, o_im, o_stride);
            }
        }

        inline void oConj(const double * const x_re, const double * const x_im, const int64_t x_r, const int64_t x_c, double * const o_re, double * const o_im, const int64_t o_r, const int64_t o_c)
        {
            __obliviousConj(0, x_r, 0, x_c, x_re, x_im, x_r, o_re, o_im, o_r);
	    if (!x_im)
	    {
		std::memset(o_im, 0, sizeof(double) * o_r * o_c);
	    }
        }

	inline void __obliviousConj(const unsigned int rb, const unsigned int re, const unsigned int cb, const unsigned int ce, const double * const x_re, const double * const x_im, const int64_t x_stride, double * const o, const int64_t o_stride)
        {
            const unsigned int dr = re - rb;
            const unsigned int dc = ce - cb;

            if (dr <= 16 && dc <= 16)
            {
		if (x_im)
		{
		    for (unsigned int i = rb; i < re; ++i)
		    {
			for (unsigned int j = cb; j < ce; ++j)
			{
			    o[2 * (j + i * o_stride)] = x_re[i + j * x_stride];
			    o[2 * (j + i * o_stride) + 1] = -x_im[i + j * x_stride];
			}
		    }
		}
		else
		{
		    for (unsigned int i = rb; i < re; ++i)
		    {
			for (unsigned int j = cb; j < ce; ++j)
			{
			    o[2 * (j + i * o_stride)] = x_re[i + j * x_stride];
			    o[2 * (j + i * o_stride) + 1] = 0;
			}
		    }
		}
            }
            else if (dr >= dc)
            {
                __obliviousConj(rb, rb + dr / 2, cb, ce, x_re, x_im, x_stride, o, o_stride);
                __obliviousConj(rb + dr / 2, re, cb, ce, x_re, x_im, x_stride, o, o_stride);
            }
            else
            {
                __obliviousConj(rb, re, cb, cb + dc / 2, x_re, x_im, x_stride, o, o_stride);
                __obliviousConj(rb, re, cb + dc / 2, ce, x_re, x_im, x_stride, o, o_stride);
            }
        }

        inline void oConj(const double * const x_re, const double * const x_im, const int64_t x_r, const int64_t x_c, double * const o, const int64_t o_r, const int64_t o_c)
        {
            __obliviousConj(0, x_r, 0, x_c, x_re, x_im, x_r, o, o_r);
        }

	inline void __obliviousConj(const unsigned int rb, const unsigned int re, const unsigned int cb, const unsigned int ce, const double * const x, const int64_t x_stride, double * const o_re, double * const o_im, const int64_t o_stride)
        {
            const unsigned int dr = re - rb;
            const unsigned int dc = ce - cb;

            if (dr <= 16 && dc <= 16)
            {
                for (unsigned int i = rb; i < re; ++i)
                {
                    for (unsigned int j = cb; j < ce; ++j)
                    {
                        o_re[j + i * o_stride] = x[2 * (i + j * x_stride)];
			o_im[j + i * o_stride] = -x[2 * (i + j * x_stride) + 1];
                    }
                }
            }
            else if (dr >= dc)
            {
                __obliviousConj(rb, rb + dr / 2, cb, ce, x, x_stride, o_re, o_im, o_stride);
                __obliviousConj(rb + dr / 2, re, cb, ce, x, x_stride, o_re, o_im, o_stride);
            }
            else
            {
                __obliviousConj(rb, re, cb, cb + dc / 2, x, x_stride, o_re, o_im, o_stride);
                __obliviousConj(rb, re, cb + dc / 2, ce, x, x_stride, o_re, o_im, o_stride);
            }
        }

        inline void oConj(const double * const x, const int64_t x_r, const int64_t x_c, double * const o_re, double * const o_im, const int64_t o_r, const int64_t o_c)
        {
            __obliviousConj(0, x_r, 0, x_c, x, x_r, o_re, o_im, o_r);
        }

	inline void oConj(const double * const x, const int64_t x_r, const int64_t x_c, const int64_t x_stride, double * const o_re, double * const o_im, const int64_t o_stride)
        {
	    __obliviousConj(0, x_r, 0, x_c, x, x_stride, o_re, o_im, o_stride);
        }
	
	template<typename T>
        inline void transpose(const T * X, const int64_t x_r, const int64_t x_c, T * O)
        {
            for (int i = 0; i < x_r; ++i)
            {
                for (int j = 0; j < x_c; ++j)
                {
                    O[j + x_c * i] = X[i + x_r * j];
                }
            }
        }

	template<typename T>
        inline void transpose(T * X, const int64_t x_r)
        {
            for (int i = 1; i < x_r; ++i)
            {
                for (int j = 0; j < i; ++j)
                {
                    std::swap(X[i + x_r * j], X[j + x_r * i]);
                }
            }
        }

	template<typename T>
        inline void transpose(T * X, const int64_t x_r, const int64_t x_c)
        {
            if (x_r == x_c)
            {
                // Square matrix so transp can be done in place no need to have a temp
                transpose(X, x_r);
            }
            else
            {
                const int s = x_r * x_c;
                T * O = new T[s];
                oTransp(X, x_r, x_c, O, x_c, x_r);
		std::memcpy(X, O, s * sizeof(double));
                delete[] O;
            }
        }

	template<typename T>
        inline void transp(const T * x, const int64_t x_r, const int64_t x_c, T ** o)
        {
	    if (!*o)
	    {
		*o = new T[x_r * x_c];
	    }
	    oTransp(x, x_r, x_c, *o, x_c, x_r);
        }

	inline void transp(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, double ** o_re, double ** o_im)
        {
	    const int64_t size = x_r * x_c;
	    if (!*o_re)
	    {
		*o_re = new double[size];
	    }
	    if (x_im)
	    {
		if (!o_im)
		{
		    *o_im = new double[size];
		}
		__obliviousConj(0, x_r, 0, x_c, x_re, x_im, x_r, *o_re, *o_im, x_c);
	    }
	    else
	    {
		oTransp(x_re, x_r, x_c, *o_re, x_c, x_r);
		if (o_im)
		{
		    std::memset(o_im, 0, size * sizeof(double));
		}
	    }
	}

    } // namespace transposition

} // namespace jit

#endif // __BASE_TRANSPOSITION_HXX__
