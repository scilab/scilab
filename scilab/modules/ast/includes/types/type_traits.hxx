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

#ifndef __TYPE_TRAITS_HXX__
#define __TYPE_TRAITS_HXX__

#include <cmath>
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

    //template<typename T>
    //inline static void neg(const int r, const int c, const T * const in, Eigen::SparseMatrix<bool, Eigen::RowMajor> * const out)
    //{
    //    for (int i = 0; i < r; i++)
    //    {
    //        for (int j = 0; j < c; j++)
    //        {
    //            out->coeffRef(i, j) = !in->coeff(i, j);
    //        }
    //    }

    //    out->prune(&keepForSparse<bool>);
    //    out->finalize();
    //}

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
