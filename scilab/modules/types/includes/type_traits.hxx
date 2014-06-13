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

#include <Eigen/Sparse>

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

    template<typename T, typename U>
    inline static void neg(const int size, const T * const in, U * const out)
    {
        for (int i = 0; i < size; i++)
        {
            out[i] = !in[i]; //in[i] == 0 ? 1 : 0;
        }
    }

    inline static void neg(const int r, const int c, const Eigen::SparseMatrix<bool> * const in, Eigen::SparseMatrix<bool> * const out)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                out->coeffRef(i, j) = !in->coeff(i, j);
            }
        }

        out->finalize();
    }

    template<typename T, typename U>
    inline static void bin_neg(const int size, const T * const in, U * const out)
    {
        for (int i = 0; i < size; i++)
        {
            out[i] = ~in[i];
        }
    }
};

} // namespace types

#endif // __TYPE_TRAITS_HXX__
