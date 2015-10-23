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

#include "base/multiplication.hxx"

#include "dynlib_ast.h"

extern "C"
{

    EXTERN_AST void times_MdMd(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const double * y, double ** o)
    {
        jit::multiplication::times<double, double, double>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMc(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
    {
        jit::multiplication::timesDC(x_r, x_c, y_c, x, y_re, y_im, o_re, o_im);
    }

    EXTERN_AST void times_McMd(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x_re, const double * x_im, const double * y, double ** o_re, double ** o_im)
    {
        jit::multiplication::timesCD(x_r, x_c, y_c, x_re, x_im, y, o_re, o_im);
    }

    EXTERN_AST void times_McMc(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x_re, const double * x_im, const double * y_re, const double * y_im, double ** o_re, double ** o_im)
    {
        jit::multiplication::timesCC(x_r, x_c, y_c, x_re, x_im, y_re, y_im, o_re, o_im);
    }

    EXTERN_AST void times_MdMi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const int8_t * y, int8_t ** o)
    {
        jit::multiplication::times<int8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const int16_t * y, int16_t ** o)
    {
        jit::multiplication::times<int16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const int32_t * y, int32_t ** o)
    {
        jit::multiplication::times<int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const int64_t * y, int64_t ** o)
    {
        jit::multiplication::times<int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const uint8_t * y, uint8_t ** o)
    {
        jit::multiplication::times<uint8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const uint16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_MdMui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const double * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const double * y, int8_t ** o)
    {
        jit::multiplication::times<int8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const double * y, int16_t ** o)
    {
        jit::multiplication::times<int16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const double * y, int32_t ** o)
    {
        jit::multiplication::times<int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const double * y, int64_t ** o)
    {
        jit::multiplication::times<int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const double * y, uint8_t ** o)
    {
        jit::multiplication::times<uint8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const double * y, uint16_t ** o)
    {
        jit::multiplication::times<uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const double * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Md(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const double * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const int8_t * y, int8_t ** o)
    {
        jit::multiplication::times<int8_t, int8_t, int8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const int16_t * y, int16_t ** o)
    {
        jit::multiplication::times<int8_t, int16_t, int16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const int32_t * y, int32_t ** o)
    {
        jit::multiplication::times<int8_t, int32_t, int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const int64_t * y, int64_t ** o)
    {
        jit::multiplication::times<int8_t, int64_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const uint8_t * y, uint8_t ** o)
    {
        jit::multiplication::times<int8_t, uint8_t, uint8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const uint16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<int8_t, uint16_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<int8_t, uint32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi8Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int8_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int8_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const int8_t * y, int16_t ** o)
    {
        jit::multiplication::times<int16_t, int8_t, int16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const int16_t * y, int16_t ** o)
    {
        jit::multiplication::times<int16_t, int16_t, int16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const int32_t * y, int32_t ** o)
    {
        jit::multiplication::times<int16_t, int32_t, int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const int64_t * y, int64_t ** o)
    {
        jit::multiplication::times<int16_t, int64_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const uint8_t * y, uint16_t ** o)
    {
        jit::multiplication::times<int16_t, uint8_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const uint16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<int16_t, uint16_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<int16_t, uint32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi16Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int16_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int16_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const int8_t * y, int32_t ** o)
    {
        jit::multiplication::times<int32_t, int8_t, int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const int16_t * y, int32_t ** o)
    {
        jit::multiplication::times<int32_t, int16_t, int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const int32_t * y, int32_t ** o)
    {
        jit::multiplication::times<int32_t, int32_t, int32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const int64_t * y, int64_t ** o)
    {
        jit::multiplication::times<int32_t, int64_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const uint8_t * y, uint32_t ** o)
    {
        jit::multiplication::times<int32_t, uint8_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const uint16_t * y, uint32_t ** o)
    {
        jit::multiplication::times<int32_t, uint16_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<int32_t, uint32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi32Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int32_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int32_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const int8_t * y, int64_t ** o)
    {
        jit::multiplication::times<int64_t, int8_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const int16_t * y, int64_t ** o)
    {
        jit::multiplication::times<int64_t, int16_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const int32_t * y, int64_t ** o)
    {
        jit::multiplication::times<int64_t, int32_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const int64_t * y, int64_t ** o)
    {
        jit::multiplication::times<int64_t, int64_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const uint8_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int64_t, uint8_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const uint16_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int64_t, uint16_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const uint32_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int64_t, uint32_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mi64Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const int64_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<int64_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const int8_t * y, uint8_t ** o)
    {
        jit::multiplication::times<uint8_t, int8_t, uint8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const int16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint8_t, int16_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const int32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint8_t, int32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const int64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint8_t, int64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const uint8_t * y, uint8_t ** o)
    {
        jit::multiplication::times<uint8_t, uint8_t, uint8_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const uint16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint8_t, uint16_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint8_t, uint32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui8Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint8_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint8_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const int8_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint16_t, int8_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const int16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint16_t, int16_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const int32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint16_t, int32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const int64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint16_t, int64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const uint8_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint16_t, uint8_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const uint16_t * y, uint16_t ** o)
    {
        jit::multiplication::times<uint16_t, uint16_t, uint16_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint16_t, uint32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui16Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint16_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint16_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const int8_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t, int8_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const int16_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t, int16_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const int32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t, int32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const int64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint32_t, int64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const uint8_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t, uint8_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const uint16_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t, uint16_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const uint32_t * y, uint32_t ** o)
    {
        jit::multiplication::times<uint32_t, uint32_t, uint32_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui32Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint32_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint32_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mi8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const int8_t * y, int64_t ** o)
    {
        jit::multiplication::times<uint64_t, int8_t, int64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mi16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const int16_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, int16_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mi32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const int32_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, int32_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mi64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const int64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, int64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mui8(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const uint8_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, uint8_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mui16(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const uint16_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, uint16_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mui32(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const uint32_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, uint32_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

    EXTERN_AST void times_Mui64Mui64(const int64_t x_r, const int64_t x_c, const int64_t y_c, const uint64_t * x, const uint64_t * y, uint64_t ** o)
    {
        jit::multiplication::times<uint64_t, uint64_t, uint64_t>(x_r, x_c, y_c, x, y, o);
    }

}
