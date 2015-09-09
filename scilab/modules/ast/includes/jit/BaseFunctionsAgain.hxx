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

#ifndef __BASE_FUNCTIONS_AGAIN_HXX__
#define __BASE_FUNCTIONS_AGAIN_HXX__

namespace jit
{
    namespace BaseFunctions
    {
        template<typename std::complex<double> (F)(const std::complex<double> &)>
        inline static void complexify(const double in_re, const double in_im, double & out_re, double & out_im)
        {
            const std::complex<double> z = F(std::complex<double>(in_re, in_im));
            out_re = z.real();
            out_im = z.imag();
        }

        template<typename std::complex<double> (F)(const std::complex<double> &)>
        inline static void cvectorize(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, double ** o_re, double ** o_im)
        {
            double * __restrict__ _o_re;
            double * __restrict__ _o_im;
            const int64_t size = x_r * x_c;
            if (*o_re)
            {
                _o_re = *o_re;
                _o_im = *o_im;
            }
            else
            {
                _o_re = new double[size];
                _o_im = new double[size];
                *o_re = _o_re;
            }

            for (int64_t i = 0; i < size; ++i)
            {
                const std::complex<double> z = F(std::complex<double>(x_re[i], x_im[i]));
                _o_re[i] = z.real();
                _o_im[i] = z.imag();
            }
        }
    }
}

#endif // __BASE_FUNCTIONS_AGAIN_HXX__
