/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __TYPES_TRANSPOSITION_HXX__
#define __TYPES_TRANSPOSITION_HXX__

extern "C"
{
#include "os_string.h"
}

struct Transposition
{
    template<typename T, typename U>
    inline static void transpose(const int r, const int c, const T * const in, U * const out)
    {
        for (int i = 0, k = 0; i < c; i++, k += r)
        {
            for (int j = 0, l = 0; j < r; j++, l += c)
            {
                out[i + l] = in[j + k];
            }
        }
    }

    template<typename T, typename U>
    inline static void transpose_clone(const int r, const int c, const T * const in, U * const out)
    {
        for (int i = 0, k = 0; i < c; i++, k += r)
        {
            for (int j = 0, l = 0; j < r; j++, l += c)
            {
                out[i + l] = in[j + k]->clone();
            }
        }
    }

    template<typename T, typename U>
    inline static void transpose(const int r, const int c, const T * const in_R, U * const out_R, const T * const in_I, U * const out_I)
    {
        for (int i = 0, k = 0; i < c; i++, k += r)
        {
            for (int j = 0, l = 0; j < r; j++, l += c)
            {
                out_R[i + l] = in_R[j + k];
                out_I[i + l] = in_I[j + k];
            }
        }
    }

    template<typename T, typename U>
    inline static void adjoint(const int r, const int c, const T * const in_R, U * const out_R, const T * const in_I, U * const out_I)
    {
        for (int i = 0, k = 0; i < c; i++, k += r)
        {
            for (int j = 0, l = 0; j < r; j++, l += c)
            {
                out_R[i + l] = in_R[j + k];
                out_I[i + l] = -in_I[j + k];
            }
        }
    }

    template<typename T, typename U>
    inline static void adjoint_clone(const int r, const int c, const T * const in, U * const out)
    {
        for (int i = 0, k = 0; i < c; i++, k += r)
        {
            for (int j = 0, l = 0; j < r; j++, l += c)
            {
                out[i + l] = in[j + k]->conjugate();
            }
        }
    }

    template<typename T, typename U>
    inline static void adjoint(const int r, const int c, const T * const in, U * const out)
    {
        return transpose(r, c, in, out);
    }

    template<typename T, typename U>
    inline static void conjugate(const int size, const T * const in_R, U * const out_R, const T * const in_I, U * const out_I)
    {
        for (int i = 0; i < size; i++)
        {
            out_R[i] = in_R[i];
            out_I[i] = -in_I[i];
        }
    }
};

template<>
inline void Transposition::transpose<wchar_t *, wchar_t *>(const int r, const int c, wchar_t * const * const in, wchar_t ** const out)
{
    for (int i = 0, k = 0; i < c; i++, k += r)
    {
        for (int j = 0, l = 0; j < r; j++, l += c)
        {
            out[i + l] = os_wcsdup(in[j + k]);
        }
    }
}



#endif // __TYPES_TRANSPOSITION_HXX__
