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

using namespace types;

Function::ReturnValue sci_cell(typed_list &in, int _piRetCount, typed_list &out)
{
    Cell* pRetVal = NULL;
    if (in.size() == 0)
    {
        //cell or cell()
        pRetVal = new Cell();
    }

    for (int i = 0 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false)
        {
            //FIXME: call overload function %_cell to manage ohter input types
            return Function::Error;
        }
    }

    if (in.size() == 1)
    {
        //cell([x])
        Double *pD = in[0]->getAs<Double>();
        if (pD->getSize() == 1)
        {
            //one value
            pRetVal = new Cell(pD->getReal()[0], pD->getReal()[0]);
        }
        else if (pD->getSize() == 2)
        {
            pRetVal = new Cell(pD->getReal()[0], pD->getReal()[1]);
        }
        else if (pD->getSize() == 0)
        {
            //[]
            pRetVal = new Cell(0, 0);
        }
        else
        {
            // > 2
            //FIXME : arrayOf<Cell>
        }
    }
    else if (in.size() == 2)
    {
        //cell(x,y)
        Double* pD1 = in[0]->getAs<Double>();
        Double* pD2 = in[1]->getAs<Double>();

        pRetVal = new Cell(pD1->getReal()[0], pD2->getReal()[0]);
    }
    else
    {
        //cell(x,y, ... )
        //FIXME : arrayOf<Cell>
    }

    if (pRetVal == NULL)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);
    return Function::OK;
}
