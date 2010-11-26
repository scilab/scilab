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

#include "alltypes.hxx"
#include "sparse.hxx"
#include "funcmanager.hxx"
extern "C" {
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
        GenericType* pRes(0);
        if ( (indices.rows_get() != vals.size_get()) || (indices.cols_get() != 2))
        {
            return 0;
        }
        // indices must be >=1
        if(*std::min_element(indices.real_get(), indices.real_get() + indices.size_get()) < 1)
        {
            return 0;
        }

        if( !pDim )
        {
            pRes = new typename types::SparseTraits<DenseType>::type(vals, indices);
        }
        // if three arguments are given the first two are the same for the case of two arugments and the third is a '1 x 2' vector for the size of the matrix
        else
        {
            if ((pDim->rows_get() != 1) || (pDim->cols_get() != 2) )
            {
                return 0;
            }
            double *  endOfRow(indices.real_get() + indices.rows_get());
            if(*std::max_element(indices.real_get(), endOfRow) > pDim->real_get(0,0))
            {
                return 0;
            }
            if(*std::max_element(endOfRow, endOfRow+ indices.rows_get()) > pDim->real_get(0,1))
            {
                return 0;
            }

            pRes = new typename types::SparseTraits<DenseType>::type(vals, indices, *pDim);
        }
        return pRes;
    }
}


Function::ReturnValue sci_sparse(typed_list &in, int _piRetCount, typed_list &out)
{
    types::GenericType* pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if ( (in.size() < 1) || (in.size() > 3) )
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"adj2sp", 1, 3);
        return Function::Error;
    }
    bool isValid(true);
    for(int i = 0 ; isValid && (i < in.size()) ; i++)
    {
        switch(in[i]->getType())
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
            isValid= false;
        }
        }
        if(!isValid)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix expected.\n"),L"sparse", i+1);
            return Function::Error;
        }
    }
    // if one argument is given, it will be a matrix of constant or sparse type, which will be converted into a sparse matrix
    if(in.size() == 1)
    {
        switch(in[0]->getType())
        {
        case InternalType::RealSparse :
        {
            pRetVal= new types::Sparse(*in[0]->getAsSparse());
            break;
        }
        case InternalType::RealDouble :
        {
            pRetVal= new types::Sparse(*in[0]->getAsDouble());
            break;
        }
        case InternalType::RealBool :
        {
            pRetVal= new types::SparseBool(*in[0]->getAsBool());
            break;
        }
        case InternalType::RealSparseBool :
        {
            pRetVal= new types::SparseBool(*in[0]->getAsSparseBool());
            break;
        }
        default :
        {
            pRetVal= 0;
        }
        }
        // if two arguments are given the first is a 'n x 2' matrix of the non zero indices and the second is a 'n x 1' vector of the values
    }else if((in.size() == 2) || (in.size() == 3))
    {
        for(int i = 0 ; i < in.size() ; i++)
        {
            if(in[i]->getType() != InternalType::RealDouble
               && !((in[i]->getType() == InternalType::RealBool) && (i==1)) )
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),L"sparse", i+1);
                return Function::Error;
            }
        }

        Double* pDims( (in.size()==3) ? in[2]->getAsDouble() : 0);

        pRetVal= in[1]->isDouble()
            ? buildSparse(*in[0]->getAsDouble(), *in[1]->getAsDouble(), pDims)
            : buildSparse(*in[0]->getAsDouble(), *in[1]->getAsBool(), pDims);
    }
    if(!pRetVal)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);

    return Function::OK;
}
