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
#define CONST
//[xadj,adjncy,anz]= sp2adj(
Function::ReturnValue sci_sp2adj(typed_list &in, int nbRes, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

   if ( in.size() != 1 )
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"spget", 1);
        return Function::Error;
    }
    if ( in[0]->getType() != InternalType::RealSparse)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: sparse matrix expected.\n"), L"spget", 1);
        return Function::Error;
    }
    if (nbRes > 3)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d to %d expected.\n"), L"spget", 1, 3);
        return Function::Error;
    }

    types::Sparse CONST& sp(*(in[0]->getAsSparse()));
    std::size_t const nonZeros(sp.nonZeros());

    double* data;
    types::Double* res;

    if(nbRes >= 1)
    {
       res= new Double(sp.cols_get()+1, 1, &data);
       data[0]= 1;
       for(std::size_t i(0); i != sp.cols_get(); ++i)
       {
           data[i+1]= data[i] + sp.nonZeros(i);
       }
       out.push_back(res);
    }
    if(nbRes >=2)
    {
       res= new Double(nonZeros, 1, &data);
       sp.outputCols(data);
       out.push_back(res);
    }
    if(nbRes == 3)
    {
        double* dataImag;
        res= sp.isComplex()
            ? new Double(nonZeros, 1, &data, &dataImag)
            : new Double(nonZeros, 1, &data );
        sp.outputValues(data, dataImag);
        out.push_back(res);
    }

    return Function::OK;
}
