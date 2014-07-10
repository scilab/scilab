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

#ifndef __TYPE_TRAITS_HXX__
#define __TYPE_TRAITS_HXX__

#include <cmath>
#include <Eigen/Sparse>
#include "types_transposition.hxx"

namespace types
{

struct type_traits
{

    template<typename T>
    inline static bool isTrue(const int size, const T * const data)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i] == 0)
            {
                return false;
            }
        }

        return true;
    }

    inline static bool isTrue(const double start, const double step, const double end)
    {
        if (start == 0 || end == 0 || step == 0)
        {
            return false;
        }

        if ((start < 0 && end > 0) || (start > 0 && end < 0))
        {
            double q = start / step;
            if (std::floor(q) == q)
            {
                return false;
            }
        }

        return true;
    }

    template<typename T, typename U>
    inline static void neg(const int size, const T * const in, U * const out)
    {
        for (int i = 0; i < size; i++)
        {
            out[i] = !in[i]; //in[i] == 0 ? 1 : 0;
        }
    }

    template<typename T>
    inline static void neg(const T start, const T step, const T end, int * const out)
    {
        int j = 0;
        for (T i = start; i < end; i += step, j++)
        {
            out[j] = i == 0;
        }
    }

    template<typename T>
    inline static void neg(const int r, const int c, const T * const in, Eigen::SparseMatrix<bool> * const out)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                out->coeffRef(i, j) = !in->coeff(i, j);
            }
        }
    }

    template<typename T, typename U>
    inline static void bin_neg(const int size, const T * const in, U * const out)
    {
        for (int i = 0; i < size; i++)
        {
            out[i] = ~in[i];
        }
    }

    template<typename T>
    inline static bool transpose(T & in, InternalType *& out)
    {
        if (in.isScalar())
        {
            out = in.clone();
            return true;
        }

        if (in.getDims() == 2)
        {
            T * pReturn = new T(in.getCols(), in.getRows());
            out = pReturn;
            Transposition::transpose(in.getRows(), in.getCols(), in.get(), pReturn->get());

            return true;
        }

        return false;
    }
};

} // namespace types

#endif // __TYPE_TRAITS_HXX__
