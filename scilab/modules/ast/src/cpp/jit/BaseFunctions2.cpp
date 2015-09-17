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

#include <chrono>

#include "BaseFunctions.hxx"
#include "BaseFunctionsMacros2.h"
#include "base/elem_functions.hxx"

extern "C"
{
    int32_t toBoolean_Md(const double * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, double, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mi8(const int8_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, int8_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mi16(const int16_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, int16_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mi32(const int32_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, int32_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mi64(const int64_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, int64_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mui8(const uint8_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, uint8_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mui16(const uint16_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, uint16_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mui32(const uint32_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, uint32_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mui64(const uint64_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, uint64_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    int32_t toBoolean_Mb(const int32_t * x, const int64_t x_r, const int64_t x_c)
    {
        return jit::BaseFunctions::breakOnM<false, int32_t, jit::BaseFunctions::asBool>(x, x_r, x_c);
    }

    std::complex<double> cfloor(const std::complex<double> z)
    {
        return jit::elem::floor<const std::complex<double> &, std::complex<double>>()(z);
    }

    static std::chrono::steady_clock::time_point tictoc_start;
    void tic()
    {
        tictoc_start = std::chrono::steady_clock::now();
    }

    double toc()
    {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - tictoc_start).count() * 1e-9;
    }
}
