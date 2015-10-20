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

#include "base/exponentiation.hxx"

extern "C"
{

    void power_MdSi64(const int64_t x_r, const double * x, const int64_t expo, double ** o)
    {
	jit::exponentiation::powMSi64<double>(x_r, x, expo, o);
    }

    void power_McSi64(const int64_t x_r, const double * x_re, const double * x_im, const int64_t expo, double ** o_re, double ** o_im)
    {
	jit::exponentiation::powMcSi64(x_r, x_re, x_im, expo, o_re, o_im);
    }

    void power_Mi8Si64(const int64_t x_r, const int8_t * x, const int64_t expo, int8_t ** o)
    {
	jit::exponentiation::powMSi64<int8_t>(x_r, x, expo, o);
    }

    void power_Mi16Si64(const int64_t x_r, const int16_t * x, const int64_t expo, int16_t ** o)
    {
	jit::exponentiation::powMSi64<int16_t>(x_r, x, expo, o);
    }

    void power_Mi32Si64(const int64_t x_r, const int32_t * x, const int64_t expo, int32_t ** o)
    {
	jit::exponentiation::powMSi64<int32_t>(x_r, x, expo, o);
    }

    void power_Mi64Si64(const int64_t x_r, const int64_t * x, const int64_t expo, int64_t ** o)
    {
	jit::exponentiation::powMSi64<int64_t>(x_r, x, expo, o);
    }

    void power_Mui8Si64(const int64_t x_r, const uint8_t * x, const int64_t expo, uint8_t ** o)
    {
	jit::exponentiation::powMSi64<uint8_t>(x_r, x, expo, o);
    }

    void power_Mui16Si64(const int64_t x_r, const uint16_t * x, const int64_t expo, uint16_t ** o)
    {
	jit::exponentiation::powMSi64<uint16_t>(x_r, x, expo, o);
    }

    void power_Mui32Si64(const int64_t x_r, const uint32_t * x, const int64_t expo, uint32_t ** o)
    {
	jit::exponentiation::powMSi64<uint32_t>(x_r, x, expo, o);
    }

    void power_Mui64Si64(const int64_t x_r, const uint64_t * x, const int64_t expo, uint64_t ** o)
    {
	jit::exponentiation::powMSi64<uint64_t>(x_r, x, expo, o);
    }

}
