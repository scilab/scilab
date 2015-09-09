/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

using namespace types;

Function::ReturnValue sci_sparse(typed_list &in, int _piRetCount, typed_list &out)
{
    bool isValid = true;
    types::GenericType* pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "sparse", 1, 3);
        return Function::Error;
    }

    for (int i = 0 ; isValid && i < in.size() ; i++)
    {
        switch (in[i]->getType())
        {
            case InternalType::ScilabBool :
            case InternalType::ScilabSparseBool :
            {
                isValid = (i == (in.size() > 1) ? 1 : 0);
            }
            case InternalType::ScilabDouble :
            case InternalType::ScilabSparse :
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
            return Function::Error;
        }
    }
    // if one argument is given, it will be a matrix of constant or sparse type, which will be converted into a sparse matrix
    if (in.size() == 1)
    {
        switch (in[0]->getType())
        {
            case InternalType::ScilabSparse :
            {
                pRetVal = new types::Sparse(*in[0]->getAs<Sparse>());
                break;
            }
            case InternalType::ScilabDouble :
            {
                if (in[0]->getAs<Double>()->isEmpty())
                {
                    out.push_back(types::Double::Empty());
                    return types::Function::OK;
                }

                if (in[0]->getAs<Double>()->isIdentity())
                {
                    out.push_back(in[0]);
                    return types::Function::OK;
                }

                pRetVal = new types::Sparse(*in[0]->getAs<Double>());
                break;
            }
            case InternalType::ScilabBool :
            {
                pRetVal = new types::SparseBool(*in[0]->getAs<Bool>());
                break;
            }
            case InternalType::ScilabSparseBool :
            {
                pRetVal = new types::SparseBool(*in[0]->getAs<SparseBool>());
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
                return Function::Error;
            }
        }

        //Double* pDims( (in.size()==3) ? in[2]->getAs<Double>() : 0);
        Double* pDims = NULL;
        if (in.size() == 3)
        {
            pDims = in[2]->getAs<Double>();
            if (pDims->getRows() != 1 || pDims->getCols() != 2)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d x %d expected.\n"), "sparse", 3, 1, 2);
                return Function::Error;
            }

            if (pDims->get(0) * pDims->get(1) == 0)
            {
                out.push_back(types::Double::Empty());
                return types::Function::OK;
            }
        }

        Double* ij = in[0]->getAs<Double>();
        types::GenericType* pGT2 = in[1]->getAs<types::GenericType>();

        if (pGT2->getSize() != ij->getRows())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d expected.\n"), "sparse", 2, ij->getRows());
            return Function::Error;
        }

        bool alloc = false;
        if (pDims == nullptr)
        {
            int size = ij->getRows();
            double* i = ij->get();
            double* j = i + ij->getRows();
            pDims = new Double(1, 2, false);
            pDims->set(0, *std::max_element(i, i + size));
            pDims->set(1, *std::max_element(j, j + size));
            alloc = true;
        }

        if (in[1]->isDouble())
        {
            Double* dbl = pGT2->getAs<Double>();
            pRetVal = new Sparse(*dbl, *ij, *pDims);
        }
        else
        {
            Bool* b = pGT2->getAs<Bool>();
            pRetVal = new SparseBool(*b, *ij, *pDims);
        }

        if (alloc)
        {
            delete pDims;
        }
    }

    if (pRetVal == NULL)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);
    return Function::OK;
}
