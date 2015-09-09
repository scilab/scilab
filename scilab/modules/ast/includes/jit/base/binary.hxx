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

#ifndef __BASE_BINARIES_HXX__
#define __BASE_BINARIES_HXX__

#include <complex>

#include "Cast.hxx"
#include "tools.hxx"

namespace jit
{
    namespace vect
    {

	template<typename F, typename T, typename U, typename V>
        inline void binary(WrapVecIn<T> && x, WrapVecIn<U> && y, const int64_t size, WrapOut<V> && o)
        {
	    WrapPtr<V> _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = F()(x[i], y[i]);
            }
        }

/*        template<typename T, typename U, typename V, V (F)(T, U)>
        inline void bin_MMM(const T * x, const int64_t x_r, const int64_t x_c, const U * y, const int64_t y_r, const int64_t y_c, V ** o)
        {
            const int64_t size = x_r * x_c;
            V * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = F(x[i], y[i]);
            }
        }

        // TODO: implement it correctly
        // i.e.: add error management when dims are incorrect
        template<typename T, typename U, typename V, V (F)(T, U)>
        inline void bin_UUU(const T * x, const int64_t x_r, const int64_t x_c, const U * y, const int64_t y_r, const int64_t y_c, V ** o)
        {
            bin_MMM<T, U, V, F>(x, x_r, x_c, y, y_r, y_c, o);
        }

        template<typename T, typename U, typename V, V (F)(T, U)>
        inline void bin_SMM(const T x, const U * y, const int64_t y_r, const int64_t y_c, V ** o)
        {
            const int64_t size = y_r * y_c;
            V * _o = getPtr(o, size);
	    for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = F(x, y[i]);
            }
        }

        template<typename T, typename U, typename V, V (F)(T, U)>
        inline void bin_MSM(const T * x, const int64_t x_r, const int64_t x_c, const U y, V ** o)
        {
            const int64_t size = x_r * x_c;
            V * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = F(x[i], y);
            }
        }

        template<std::complex<double> (CPXCPX)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_McMcMc(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, double ** o_re, double ** o_im)
        {
            const int64_t size = x_r * x_c;
            double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = CPXCPX(std::complex<double>(x_re[i], x_im[i]), std::complex<double>(y_re[i], y_im[i]));
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

        template<typename T, std::complex<double> (RECPX)(T, const std::complex<double> const std::complex<double>)>
        inline void bin_MMcMc(const T * x, const int64_t x_r, const int64_t x_c, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, double ** o_re, double ** o_im)
        {
	    const int64_t size = x_r * x_c;
            double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = RECPX(x[i], std::complex<double>(y_re[i], y_im[i]));
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

        template<typename T, std::complex<double> (RECPX)(T, const std::complex<double> const std::complex<double>)>
        inline void bin_SMcMc(const T x, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, double ** o_re, double ** o_im)
        {
	    const int64_t size = y_r * y_c;
            double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = RECPX(x, std::complex<double>(y_re[i], y_im[i]));
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

        template<typename T, std::complex<double> (CPXRE)(const std::complex<double> const std::complex<double>, T)>
        inline void bin_McMMc(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const T * y, const int64_t y_r, const int64_t y_c, double ** o_re, double ** o_im)
        {
	    const int64_t size = x_r * x_c;
            double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = CPXRE(std::complex<double>(x_re[i], x_im[i]), y[i]);
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }
	
        template<std::complex<double> (CPXCPX)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_ScMcMc(const double x_re, const double x_im, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, double ** o_re, double ** o_im)
        {
	    const int64_t size = y_r * y_c;
            double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
	    const std::complex<double> const std::complex<double> z1(x_re, x_im);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = CPXCPX(z1, std::complex<double>(y_re[i], y_im[i]));
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

        template<std::complex<double> (CPXCPX)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_McScMc(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const double y_re, const double y_im, double ** o_re, double ** o_im)
        {
            const int64_t size = x_r * x_c;
	    double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
	    const std::complex<double> const std::complex<double> z1(y_re, y_im);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = CPXCPX(std::complex<double>(x_re[i], x_im[i]), z1);
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }
	
        template<typename T, std::complex<double> (CPXRE)(const std::complex<double> const std::complex<double>, T)>
        inline void bin_McSMc(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const T y, double ** o_re, double ** o_im)
        {
            const int64_t size = x_r * x_c;
	    double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = CPXRE(std::complex<double>(x_re[i], x_im[i]), y);
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

        template<typename T, std::complex<double> (CPXRE)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_ScMMc(const double x_re, const double x_im, const T * y, const int64_t y_r, const int64_t y_c, double ** o_re, double ** o_im)
        {
            const int64_t size = y_r * y_c;
	    double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = CPXRE(std::complex<double>(x_re, x_im), y[i]);
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

	template<typename T, std::complex<double> (RECPX)(T, const std::complex<double> const std::complex<double>)>
        inline void bin_MScMc(const T * x, const int64_t x_r, const int64_t x_c, const double y_re, const double y_im, double ** o_re, double ** o_im)
        {
            const int64_t size = x_r * x_c;
	    double * _o_re = getPtr(o_re, size);
            double * _o_im = getPtr(o_im, size);
	    const std::complex<double> const std::complex<double> z1(y_re, y_im);
            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> const std::complex<double> z = RECPX(x[i], z1);
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }

	// FUCK
	template<typename T, T (CPXCPX)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_McMcM(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, T ** o)
        {
            const int64_t size = x_r * x_c;
            T * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = CPXCPX(std::complex<double>(x_re[i], x_im[i]), std::complex<double>(y_re[i], y_im[i]));
            }
        }

        template<typename T, typename U, U (RECPX)(T, const std::complex<double> const std::complex<double>)>
        inline void bin_MMcM(const T * x, const int64_t x_r, const int64_t x_c, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, U ** o)
        {
	    const int64_t size = x_r * x_c;
            U * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = RECPX(x[i], std::complex<double>(y_re[i], y_im[i]));
            }
        }

        template<typename T, typename U, U (RECPX)(T, const std::complex<double> const std::complex<double>)>
        inline void bin_SMcM(const T x, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, U ** o)
        {
	    const int64_t size = y_r * y_c;
            U * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = RECPX(x, std::complex<double>(y_re[i], y_im[i]));
            }
        }

        template<typename T, typename U, U (CPXRE)(const std::complex<double> const std::complex<double>, T)>
        inline void bin_McMM(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const T * y, const int64_t y_r, const int64_t y_c, U ** o)
        {
	    const int64_t size = x_r * x_c;
            U * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = CPXRE(std::complex<double>(x_re[i], x_im[i]), y[i]);
            }
        }
	
        template<typename T, T (CPXCPX)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_ScMcM(const double x_re, const double x_im, const double * y_re, const double * y_im, const int64_t y_r, const int64_t y_c, T ** o)
        {
	    const int64_t size = y_r * y_c;
            T * _o = getPtr(o, size);
            const std::complex<double> const std::complex<double> z1(x_re, x_im);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = CPXCPX(z1, std::complex<double>(y_re[i], y_im[i]));
            }
        }

        template<typename T, T (CPXCPX)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_McScM(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const double y_re, const double y_im, T ** o)
        {
            const int64_t size = x_r * x_c;
	    T * _o = getPtr(o, size);
	    const std::complex<double> const std::complex<double> z1(y_re, y_im);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = CPXCPX(std::complex<double>(x_re[i], x_im[i]), z1);
            }
        }
	
        template<typename T, typename U, U (CPXRE)(const std::complex<double> const std::complex<double>, T)>
        inline void bin_McSM(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, const T y, U ** o)
        {
            const int64_t size = x_r * x_c;
	    U * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = CPXRE(std::complex<double>(x_re[i], x_im[i]), y);
            }
        }

        template<typename T, typename U, U (CPXRE)(const std::complex<double> const std::complex<double>, const std::complex<double> const std::complex<double>)>
        inline void bin_ScMM(const double x_re, const double x_im, const T * y, const int64_t y_r, const int64_t y_c, U ** o)
        {
            const int64_t size = y_r * y_c;
	    U * _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = CPXRE(std::complex<double>(x_re, x_im), y[i]);
            }
        }

	template<typename T, typename U, U (RECPX)(T, const std::complex<double> const std::complex<double>)>
        inline void bin_MScM(const T * x, const int64_t x_r, const int64_t x_c, const double y_re, const double y_im, U ** o)
        {
            const int64_t size = x_r * x_c;
	    U * _o = getPtr(o, size);
            const std::complex<double> const std::complex<double> z1(y_re, y_im);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = RECPX(x[i], z1);
            }
        }
*/

    } // namespace BaseFunctions

} // namespace jit

#endif // __BASE_BINARIES_HXX__
