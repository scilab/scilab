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

#include "base/transposition.hxx"

extern "C"
{
    
    void transp_Md(const double * x, const int64_t x_r, const int64_t x_c, double ** o)
    {
	jit::transposition::transp<double>(x, x_r, x_c, o);
    }

    void transp_Mc(const double * x_re, const double * x_im, const int64_t x_r, const int64_t x_c, double ** o_re, double ** o_im)
    {
	jit::transposition::transp(x_re, x_im, x_r, x_c, o_re, o_im);
    }

    void transp_Mi8(const int8_t * x, const int64_t x_r, const int64_t x_c, int8_t ** o)
    {
	jit::transposition::transp<int8_t>(x, x_r, x_c, o);
    }

    void transp_Mi16(const int16_t * x, const int64_t x_r, const int64_t x_c, int16_t ** o)
    {
	jit::transposition::transp<int16_t>(x, x_r, x_c, o);
    }

    void transp_Mi32(const int32_t * x, const int64_t x_r, const int64_t x_c, int32_t ** o)
    {
	jit::transposition::transp<int32_t>(x, x_r, x_c, o);
    }

    void transp_Mi64(const int64_t * x, const int64_t x_r, const int64_t x_c, int64_t ** o)
    {
	jit::transposition::transp<int64_t>(x, x_r, x_c, o);
    }

    void transp_Mui8(const uint8_t * x, const int64_t x_r, const int64_t x_c, uint8_t ** o)
    {
	jit::transposition::transp<uint8_t>(x, x_r, x_c, o);
    }

    void transp_Mui16(const uint16_t * x, const int64_t x_r, const int64_t x_c, uint16_t ** o)
    {
	jit::transposition::transp<uint16_t>(x, x_r, x_c, o);
    }

    void transp_Mui32(const uint32_t * x, const int64_t x_r, const int64_t x_c, uint32_t ** o)
    {
	jit::transposition::transp<uint32_t>(x, x_r, x_c, o);
    }

    void transp_Mui64(const uint64_t * x, const int64_t x_r, const int64_t x_c, uint64_t ** o)
    {
	jit::transposition::transp<uint64_t>(x, x_r, x_c, o);
    }
}
 
