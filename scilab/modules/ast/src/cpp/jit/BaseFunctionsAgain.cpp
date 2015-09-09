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

#include <complex>
#include <cmath>

#include "BaseFunctionsAgain.hxx"

extern "C"
{
    void sin_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::sin>(in_re, in_im, out_re, out_im);
    }

    void cos_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::cos>(in_re, in_im, out_re, out_im);
    }

    void tan_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::tan>(in_re, in_im, out_re, out_im);
    }

    void sinh_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::sinh>(in_re, in_im, out_re, out_im);
    }

    void cosh_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::cosh>(in_re, in_im, out_re, out_im);
    }

    void tanh_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::tanh>(in_re, in_im, out_re, out_im);
    }

    void asin_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::asin>(in_re, in_im, out_re, out_im);
    }

    void acos_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::acos>(in_re, in_im, out_re, out_im);
    }

    void atan_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::atan>(in_re, in_im, out_re, out_im);
    }

    void asinh_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::asinh>(in_re, in_im, out_re, out_im);
    }

    void acosh_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::acosh>(in_re, in_im, out_re, out_im);
    }

    void atanh_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::atanh>(in_re, in_im, out_re, out_im);
    }

    void exp_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::exp>(in_re, in_im, out_re, out_im);
    }

    void log_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::log>(in_re, in_im, out_re, out_im);
    }

    void log10_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::log10>(in_re, in_im, out_re, out_im);
    }

    void sqrt_Sc(const double in_re, const double in_im, double & out_re, double & out_im)
    {
        jit::BaseFunctions::complexify<std::sqrt>(in_re, in_im, out_re, out_im);
    }



    void sin_Mc(const double * __restrict__ in_re, const double * __restrict__ in_im, const int64_t x_r, const int64_t x_c, double ** __restrict__ out_re, double ** __restrict__ out_im)
    {
        jit::BaseFunctions::cvectorize<std::sin>(in_re, in_im, out_re, out_im);
    }



    void pow_ScSd(const double in_re, const double in_im, const double in_pow, double & out_re, double & out_im)
    {
        const std::complex<double> z = std::pow(std::complex<double>(in_re, in_im), in_pow);
        out_re = z.real();
        out_im = z.imag();
    }

    void pow_SdSc(const double in, const double in_re_pow, const double in_im_pow, double & out_re, double & out_im)
    {
        const std::complex<double> z = std::pow(in, std::complex<double>(in_re_pow, in_im_pow));
        out_re = z.real();
        out_im = z.imag();
    }

    void pow_ScSc(const double in_re, const double in_im, const double in_re_pow, const double in_im_pow, double & out_re, double & out_im)
    {
        const std::complex<double> z = std::pow(std::complex<double>(in_re, in_im), std::complex<double>(in_re_pow, in_im_pow));
        out_re = z.real();
        out_im = z.imag();
    }
}
