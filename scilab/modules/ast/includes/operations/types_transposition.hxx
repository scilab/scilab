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
inline void Transposition::transpose<char*, char*>(const int r, const int c, char* const* const in, char** const out)
{
    for (int i = 0, k = 0; i < c; i++, k += r)
    {
        for (int j = 0, l = 0; j < r; j++, l += c)
        {
            out[i + l] = os_strdup(in[j + k]);
        }
    }
}



#endif // __TYPES_TRANSPOSITION_HXX__
