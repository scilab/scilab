/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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

#include <algorithm>

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_sparse(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    bool isValid = true;
    types::GenericType* pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "sparse", 1, 3);
        return types::Function::Error;
    }

    for (int i = 0 ; isValid && i < in.size() ; i++)
    {
        // Valid input arguments are of Bool and Double types (dense or sparse)
        switch (in[i]->getType())
        {
            case types::InternalType::ScilabBool :
            case types::InternalType::ScilabSparseBool :
            {
                isValid = (i == (in.size() > 1) ? 1 : 0);
            }
            case types::InternalType::ScilabDouble :
            case types::InternalType::ScilabSparse :
            {
                break;
            }
            default :
            {
                isValid = false;
            }
        }

        if (!isValid)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "sparse", i + 1);
            return types::Function::Error;
        }

        // Valid input arguments are matrices and not hypermatrices
        if ( in[i]->getAs<types::GenericType>()->getDims() > 2 )
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A m-by-n matrix expected.\n"), "sparse", i + 1);
            return types::Function::Error;
        }

    }
    // if one argument is given, it will be a matrix of constant or sparse type, which will be converted into a sparse matrix
    if (in.size() == 1)
    {
        switch (in[0]->getType())
        {
            case types::InternalType::ScilabSparse :
            {
                pRetVal = new types::Sparse(*in[0]->getAs<types::Sparse>());
                break;
            }
            case types::InternalType::ScilabDouble :
            {
                if (in[0]->getAs<types::Double>()->isEmpty())
                {
                    out.push_back(types::Double::Empty());
                    return types::Function::OK;
                }

                if (in[0]->getAs<types::Double>()->isIdentity())
                {
                    out.push_back(in[0]);
                    return types::Function::OK;
                }

                pRetVal = new types::Sparse(*in[0]->getAs<types::Double>());
                break;
            }
            case types::InternalType::ScilabBool :
            {
                pRetVal = new types::SparseBool(*in[0]->getAs<types::Bool>());
                break;
            }
            case types::InternalType::ScilabSparseBool :
            {
                pRetVal = new types::SparseBool(*in[0]->getAs<types::SparseBool>());
                break;
            }
            default :
            {
                pRetVal = NULL;
            }
        }
    }
    else if (in.size() == 2 || in.size() == 3)
    {
        // if two arguments are given the first is a 'n x 2' matrix of the non zero indices and the second is a 'n x 1' vector of the values
        for (int i = 0 ; i < in.size() ; i++)
        {
            if (in[i]->isDouble() == false && !(in[i]->isBool() && i == 1))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"), "sparse", i + 1);
                return types::Function::Error;
            }
        }

        //Double* pDims( (in.size()==3) ? in[2]->getAs<Double>() : 0);
        types::Double* pDims = NULL;
        if (in.size() == 3)
        {
            pDims = in[2]->getAs<types::Double>();
            if (pDims->getRows() != 1 || pDims->getCols() != 2)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d x %d expected.\n"), "sparse", 3, 1, 2);
                return types::Function::Error;
            }

            if (pDims->get(0) * pDims->get(1) == 0)
            {
                out.push_back(types::Double::Empty());
                return types::Function::OK;
            }
        }

        types::Double* ij = in[0]->getAs<types::Double>();
        types::GenericType* pGT2 = in[1]->getAs<types::GenericType>();

        if (pGT2->getSize() != ij->getRows())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d expected.\n"), "sparse", 2, ij->getRows());
            return types::Function::Error;
        }

        bool alloc = false;
        if (pDims == nullptr)
        {
            int size = ij->getRows();
            double* i = ij->get();
            double* j = i + ij->getRows();
            pDims = new types::Double(1, 2, false);
            pDims->set(0, *std::max_element(i, i + size));
            pDims->set(1, *std::max_element(j, j + size));
            alloc = true;
        }

        if (in[1]->isDouble())
        {
            types::Double* dbl = pGT2->getAs<types::Double>();
            pRetVal = new types::Sparse(*dbl, *ij, *pDims);
        }
        else
        {
            types::Bool* b = pGT2->getAs<types::Bool>();
            pRetVal = new types::SparseBool(*b, *ij, *pDims);
        }

        if (alloc)
        {
            delete pDims;
        }
    }

    if (pRetVal == NULL)
    {
        return types::Function::Error;
    }

    out.push_back(pRetVal);
    return types::Function::OK;
}
