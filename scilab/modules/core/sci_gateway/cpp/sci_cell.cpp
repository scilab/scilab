/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "double.hxx"
#include "cell.hxx"
#include "function.hxx"
#include "overload.hxx"

types::Function::ReturnValue sci_cell_gw(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Cell* pRetVal = NULL;
    if (in.size() == 0)
    {
        //cell or cell()
        out.push_back(new types::Cell());
        return types::Function::OK;
    }

    for (int i = 0 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false)
        {
            return Overload::call(L"%_cell", in, _iRetCount, out);
        }
    }

    if (in.size() == 1)
    {
        //cell([x])
        types::Double *pD = in[0]->getAs<types::Double>();
        if (pD->getSize() == 1)
        {
            //one value
            pRetVal = new types::Cell((int)pD->getReal()[0], (int)pD->getReal()[0]);
        }
        else if (pD->getSize() == 2)
        {
            pRetVal = new types::Cell((int)pD->getReal()[0], (int)pD->getReal()[1]);
        }
        else if (pD->getSize() == 0)
        {
            //[]
            pRetVal = new types::Cell(0, 0);
        }
        else
        {
            int* piDimsArray = new int[pD->getSize()];
            for (int i = 0; i < pD->getSize(); i++)
            {
                piDimsArray[i] = (int)pD->get(i);
            }
            pRetVal = new types::Cell(pD->getSize(), piDimsArray);
            delete[] piDimsArray;
        }
    }
    else if (in.size() == 2)
    {
        //cell(x,y)
        types::Double* pD1 = in[0]->getAs<types::Double>();
        types::Double* pD2 = in[1]->getAs<types::Double>();

        pRetVal = new types::Cell(pD1->getReal()[0], pD2->getReal()[0]);
    }
    else
    {
        int* piDimsArray = new int[in.size()];
        for (int i = 0; i < in.size(); i++)
        {
            piDimsArray[i] = (int)in[i]->getAs<types::Double>()->get(0);
        }
        pRetVal = new types::Cell(in.size(), piDimsArray);
        delete[] piDimsArray;
    }

    if (pRetVal == NULL)
    {
        return types::Function::Error;
    }

    out.push_back(pRetVal);
    return types::Function::OK;
}
