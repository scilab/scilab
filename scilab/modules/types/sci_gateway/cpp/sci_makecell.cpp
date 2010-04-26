/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "alltypes.hxx"
#include "funcmanager.hxx"

using namespace types;

int prodDims(Double* _pVar);

Function::ReturnValue sci_makecell(typed_list &in, int _piRetCount, typed_list &out)
{

    if(in.size() < 2)
    {
        return Function::Error;
    }

    //manage dims

    //check data type
    if(in[0]->getType() != InternalType::RealDouble)
    {
        //FIXME: call overload function %_makecell to manage ohter input types
    }

    Double* pD = in[0]->getAsDouble();

    //check vector format
    if(pD->rows_get() != 1 && pD->cols_get() != 1 || pD->size_get() == 1)
    {
        return Function::Error;
    }

    //check input parameters count
    if(prodDims(pD) != (in.size() - 1))
    {
        return Function::Error;
    }

    Cell *pC  = NULL;
    if(pD->rows_get() > 2 || pD->cols_get() > 2)
    {//arrayOf<Cell>
    }
    else
    {//2 dims
        pC  = new Cell(static_cast<int>(pD->real_get()[0]), static_cast<int>(pD->real_get()[1]));

        for(int i = 1 ; i < in.size() ; i++)
        {
            pC->set(i-1, in[i]);
        }
    }

    out.push_back(pC);
    return Function::OK;
}

int prodDims(Double* _pVar)
{
    int iProd = 1;
    for(int i = 0 ; i < _pVar->size_get() ; i++)
    {
        iProd *= static_cast<int>(_pVar->real_get()[i]);
    }
    return iProd;
}