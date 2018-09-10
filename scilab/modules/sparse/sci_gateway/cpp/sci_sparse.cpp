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
#include "double.hxx"

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
    else // 2 or 3 arguments
    {
        types::Double* pDij = in[0]->getAs<types::Double>(); // arg 2 necessarily *Double
        types::GenericType* pGT2 = in[1]->getAs<types::GenericType>();
        int size = pGT2->getSize();

        if ( ((size !=0) || (pDij->getSize() != 0)) && ((pDij->getCols() != 2) || (pDij->getRows() != size)))
        {
            if (size > 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d x %d expected.\n"), "sparse", 1, size, 2);
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: An empty matrix expected.\n"), "sparse", 1);
            }
            return types::Function::Error;
        }

        //Double* pDdims( (in.size()==3) ? in[2]->getAs<Double>() : 0);
        types::Double* pDdims = NULL;
        if (in.size() == 3)
        {
            pDdims = in[2]->getAs<types::Double>();
            if (pDdims->getSize() != 2)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %d x %d expected.\n"), "sparse", 3, 1, 2);
                return types::Function::Error;
            }

            if (pDdims->get(0) != (double) ( (unsigned int) pDdims->get(0) ) || pDdims->get(1) != (double) ( (unsigned int) pDdims->get(1) ))
            {
                Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), "sparse", 3);
                return types::Function::Error;
            }
        }

        bool alloc = false;
        double* i = pDij->get();
        double* j = i + size;

        if ( (size > 0) && ((*std::min_element(i, i + size) <= 0) || (*std::min_element(j, j + size) <= 0)) )
        {
            Scierror(999, _("%s: Invalid index.\n"), "sparse");
            return types::Function::Error;
        }

        if (pDdims == nullptr)
        {
            pDdims = new types::Double(1, 2, false);
            pDdims->setZeros();
            if (size > 0)
            {
                pDdims->set(0, *std::max_element(i, i + size));
                pDdims->set(1, *std::max_element(j, j + size));
            }
            alloc = true;
        }
        else if ( (size > 0) && ((pDdims->get(0) < *std::max_element(i, i + size)) || (pDdims->get(1) < *std::max_element(j, j + size))) )
        {
            Scierror(999, _("%s: Invalid index.\n"),"sparse");
            return types::Function::Error;
        }

        if ((pDdims->get(0) == 0) || (pDdims->get(1) == 0))
        {
            pRetVal = new types::Sparse(0,0,false);
        }
        else if (in[1]->isDouble())
        {
            types::Double* pDbl = pGT2->getAs<types::Double>();
            pRetVal = new types::Sparse(*pDbl, *pDij, *pDdims);
        }
        else
        {
            types::Bool* pBb = pGT2->getAs<types::Bool>();
            pRetVal = new types::SparseBool(*pBb, *pDij, *pDdims);
        }

        if (alloc)
        {
            delete pDdims;
        }
    }

    if (pRetVal == NULL)
    {
        return types::Function::Error;
    }

    out.push_back(pRetVal);
    return types::Function::OK;
}
