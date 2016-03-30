/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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


extern "C" int C2F(bfinit)(int* neqns,  int* nsuper, int* xsuper,
                           int* snode, int* xlindx,
                           int* lindx, int* cachsz, int* tmpsiz, int* split);

using namespace types;

Function::ReturnValue sci_bfinit(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 7)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "bfinit", 7);
        return Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "bfinit", 2);
        return Function::Error;
    }


    //get argument #1
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 1);
        return Function::Error;
    }

    Double* pdbl1 = in[0]->getAs<Double>();
    pdbl1->convertToInteger();
    int* neqns = (int*)pdbl1->get();

    //get argument #2
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 2);
        return Function::Error;
    }

    Double* pdbl2 = in[1]->getAs<Double>();
    pdbl2->convertToInteger();
    int* nsuper = (int*)pdbl2->get();

    //get argument #3
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 3);
        return Function::Error;
    }

    Double* pdbl3 = in[2]->getAs<Double>();
    pdbl3->convertToInteger();
    int* xsuper = (int*)pdbl3->get();

    //get argument #4
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 4);
        return Function::Error;
    }

    Double* pdbl4 = in[3]->getAs<Double>();
    pdbl4->convertToInteger();
    int* snode = (int*)pdbl4->get();

    //get argument #5
    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 5);
        return Function::Error;
    }

    Double* pdbl5 = in[4]->getAs<Double>();
    pdbl5->convertToInteger();
    int* xlindx = (int*)pdbl5->get();

    //get argument #6
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 6);
        return Function::Error;
    }

    Double* pdbl6 = in[5]->getAs<Double>();
    pdbl6->convertToInteger();
    int* lindx = (int*)pdbl6->get();

    //get argument #7
    if (in[6]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "bfinit", 7);
        return Function::Error;
    }

    Double* pdbl7 = in[6]->getAs<Double>();
    pdbl7->convertToInteger();
    int* cachsz = (int*)pdbl7->get();


    Double* pdbl8 = new Double(1, 1);
    pdbl8->convertToInteger();
    int* tmpsiz = (int*)pdbl8->get();

    Double* pdbl9 = new Double(neqns[0], 1);
    pdbl9->convertToInteger();
    int* split = (int*)pdbl9->get();


    C2F(bfinit)(neqns, nsuper, xsuper,
                snode, xlindx,
                lindx, cachsz, tmpsiz, split);

    pdbl1->convertFromInteger();
    pdbl2->convertFromInteger();
    pdbl3->convertFromInteger();
    pdbl4->convertFromInteger();
    pdbl5->convertFromInteger();
    pdbl6->convertFromInteger();
    pdbl7->convertFromInteger();
    pdbl8->convertFromInteger();
    pdbl9->convertFromInteger();

    out.push_back(pdbl8);
    out.push_back(pdbl9);

    return Function::OK;
}

