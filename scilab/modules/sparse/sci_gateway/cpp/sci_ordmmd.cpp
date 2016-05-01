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

#include <iostream>
#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

extern "C" int C2F(ordmmd)(int* neqns, int* xadj, int* adjncy, int* invp, int* perm,
                           int* iwsiz, int* iwork, int* nofsub, int* iflag);

types::Function::ReturnValue sci_ordmmd(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 3)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "ordmmd", 3);
        return types::Function::Error;
    }

    if (_iRetCount != 3)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "ordmmd", 3);
        return types::Function::Error;
    }


    //get argument #1
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "ordmmd", 1);
        return types::Function::Error;
    }

    types::Double* pdbl1 = in[0]->getAs<types::Double>();
    pdbl1->convertToInteger();
    int* pXADJ = (int*)pdbl1->get();

    //get argument #2
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "ordmmd", 2);
        return types::Function::Error;
    }

    types::Double* pdbl2 = in[1]->getAs<types::Double>();
    pdbl2->convertToInteger();
    int* pADJNCY = (int*)pdbl2->get();
    //get argument #3
    types::Double* pdbl3 = in[2]->getAs<types::Double>();
    if (in[2]->isDouble() == false || pdbl3->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), "ordmmd", 3);
        return types::Function::Error;
    }

    int NEQNS = (int)pdbl3->get(0);
    if (NEQNS != (pdbl1->getSize() - 1))
    {

        Scierror(999, _(" The provided \"n\" does not correspond to the matrix defined by xadj and iadj\n"));
        return types::Function::Error;
    }

    types::Double* pdbl4 = new types::Double(NEQNS, 1);
    pdbl4->convertToInteger();

    types::Double* pdbl5 = new types::Double(NEQNS, 1);
    pdbl5->convertToInteger();

    types::Double* pdbl6 = new types::Double(1, 1);
    pdbl6->convertToInteger();

    types::Double* pdbl7 = new types::Double(4 * NEQNS, 1);
    pdbl7->convertToInteger();
    int iSize = 4 * NEQNS;
    int iFlag = 0;
    C2F(ordmmd)(&NEQNS, (int*)pdbl1->get(), (int*)pdbl2->get(), (int*)pdbl5->get(),
                (int*)pdbl4->get(), &iSize, (int*)pdbl7->get(), (int*)pdbl6->get(), &iFlag);

    if (iFlag)
    {
        Scierror(999, _("%s: insufficient working storage"), "ordmmd");
        return types::Function::Error;
    }

    pdbl1->convertFromInteger();
    pdbl2->convertFromInteger();
    pdbl4->convertFromInteger();
    pdbl5->convertFromInteger();
    pdbl6->convertFromInteger();
    pdbl7->convertFromInteger();
    delete pdbl7;

    out.push_back(pdbl4);
    out.push_back(pdbl5);
    out.push_back(pdbl6);

    return types::Function::OK;
}

