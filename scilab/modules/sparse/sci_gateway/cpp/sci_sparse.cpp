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

namespace
{
template<typename DenseType>
GenericType* buildSparse(Double CONST& indices, DenseType CONST& vals, Double CONST* pDim)
{
    GenericType* pRes = NULL;
    InternalType* pVals = (InternalType*)&vals;
    if (indices.isEmpty() && vals.isDouble() && pVals->getAs<Double>()->isEmpty())
    {
        //sparse([],[], ...)
        if (pDim == NULL)
        {
            //sparse([], []) -> KO
            return NULL;
        }

        //check pDims is a row vector(1,2)
        if (pDim->getRows() != 1 || pDim->getCols() != 2)
        {
            return NULL;
        }

        //can create an empty sparse matrix
        return new Sparse(pDim->get(0), pDim->get(1));
    }

    if (indices.getRows() != vals.getSize() || (indices.getCols() != 2 && indices.getCols() != 0))
    {
        return NULL;
    }

    // indices must be >=1
    if (*std::min_element(indices.getReal(), indices.getReal() + indices.getSize()) < 1)
    {
        return NULL;
    }

    if (pDim == NULL)
    {
        pRes = new typename types::SparseTraits<DenseType>::type(vals, indices);
    }
    else
    {
        // if three arguments are given the first two are the same for the case of two arugments and the third is a '1 x 2' vector for the size of the matrix
        if (pDim->getRows() != 1 || pDim->getCols() != 2)
        {
            return NULL;
        }

        double* endOfRow(indices.getReal() + indices.getRows());
        if (*std::max_element(indices.getReal(), endOfRow) > pDim->getReal(0, 0))
        {
            return NULL;
        }

        if (*std::max_element(endOfRow, endOfRow + indices.getRows()) > pDim->getReal(0, 1))
        {
            return NULL;
        }

        pRes = new typename types::SparseTraits<DenseType>::type(vals, indices, *pDim);
    }
    return pRes;
}
}


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
            case InternalType::RealBool :
            case InternalType::RealSparseBool :
            {
                isValid = (i == (in.size() > 1) ? 1 : 0);
            }
            case InternalType::RealDouble :
            case InternalType::RealSparse :
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
            case InternalType::RealSparse :
            {
                pRetVal = new types::Sparse(*in[0]->getAs<Sparse>());
                break;
            }
            case InternalType::RealDouble :
            {
                if (in[0]->getAs<Double>()->isEmpty())
                {
                    out.push_back(types::Double::Empty());
                    return types::Function::OK;
                }

                pRetVal = new types::Sparse(*in[0]->getAs<Double>());
                break;
            }
            case InternalType::RealBool :
            {
                pRetVal = new types::SparseBool(*in[0]->getAs<Bool>());
                break;
            }
            case InternalType::RealSparseBool :
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

        types::GenericType* pGT1 = in[0]->getAs<types::GenericType>();
        types::GenericType* pGT2 = in[1]->getAs<types::GenericType>();

        if (pGT2->getSize() != pGT1->getRows())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d expected.\n"), "sparse", 2, pGT1->getRows());
            return Function::Error;
        }

        if (in[1]->isDouble())
        {
            pRetVal = buildSparse(*in[0]->getAs<Double>(), *in[1]->getAs<Double>(), pDims);
        }
        else
        {
            pRetVal = buildSparse(*in[0]->getAs<Double>(), *in[1]->getAs<Bool>(), pDims);
        }
    }

    if (pRetVal == NULL)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);
    return Function::OK;
}
