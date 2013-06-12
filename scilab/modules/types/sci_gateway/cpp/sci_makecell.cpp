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

#include "types_gw.hxx"
#include "double.hxx"
#include "cell.hxx"
#include "function.hxx"
#include "funcmanager.hxx"

using namespace types;

int prodDims(Double* _pVar);

Function::ReturnValue sci_makecell(typed_list &in, int _piRetCount, typed_list &out)
{

    if (in.size() < 2)
    {
        return Function::Error;
    }

    //manage dims

    //check data type
    if (in[0]->isDouble() == false)
    {
        //FIXME: call overload function %_makecell to manage ohter input types
    }

    Double* pD = in[0]->getAs<Double>();

    //check vector format
    if (pD->getRows() != 1 && pD->getCols() != 1 || pD->getSize() == 1)
    {
        return Function::Error;
    }

    //check input parameters count
    if (prodDims(pD) != (in.size() - 1))
    {
        return Function::Error;
    }

    Cell *pC  = NULL;
    if (pD->getRows() > 2 || pD->getCols() > 2)
    {
        //arrayOf<Cell>
    }
    else
    {
        //2 dims
        pC  = new Cell(static_cast<int>(pD->getReal()[0]), static_cast<int>(pD->getReal()[1]));

        for (int i = 1 ; i < in.size() ; i++)
        {
            pC->set(i - 1, in[i]);
        }
    }

    out.push_back(pC);
    return Function::OK;
}

int prodDims(Double* _pVar)
{
    int iProd = 1;
    for (int i = 0 ; i < _pVar->getSize() ; i++)
    {
        iProd *= static_cast<int>(_pVar->getReal()[i]);
    }
    return iProd;
}
