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
    template<typename In, typename Out>
    void addAsOnes(In in, std::size_t n, Out out)
    {
        for(; n; --n, ++in, ++out)
        {
            *out += (*in ? 1. : 0);
        }
    }
}

Function::ReturnValue sci_bool2s(typed_list &in, int nbRes, typed_list &out)
{
    types::GenericType* res = NULL;

    if ( in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"bool2s", 1);
        return Function::Error;
    }

    if ( nbRes != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"bool2s", 1);
        return Function::Error;
    }


    switch(in[0]->getType())
    {
    case InternalType::RealDouble :
    case InternalType::RealBool :
    {
        Double* dRes(0);
        Double* d(in[0]->getAsDouble());
        // there should be a common interface between Bool and Double to avoid code dupplication
        if(d)
        {
            dRes= new Double(d->rows_get(), d->cols_get());
            dRes->zero_set();
            addAsOnes(d->real_get(),d->size_get(), dRes->real_get());
            if(d->isComplex())
            {
                addAsOnes(d->img_get(),d->size_get(), dRes->real_get());
            }
        }
        else
        {
            Bool* b(in[0]->getAsBool());
            dRes= new Double(b->rows_get(), b->cols_get());
            dRes->zero_set();
            addAsOnes(in[0]->getAsBool()->bool_get(), d->size_get(), dRes->real_get());
        }
        res= dRes;
        break;
    }
    case InternalType::RealSparse :
    {
        res= in[0]->getAsSparse()->newOnes();
        break;
    }
    case InternalType::RealSparseBool :
    {
        res= in[0]->getAsSparseBool()->newOnes();
        break;
    }
    default :
    {
        ScierrorW(999, _W("%s: Wrong type for input argument #%d.\n"), L"bool2s", 1);
        return Function::Error;
    }
    }
    if( !res )
    {
        return Function::Error;
    }

    out.push_back(res);

    return Function::OK;
}
