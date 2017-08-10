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

extern "C" int C2F(sfinit)(int* neqns, int* nnza, int* xadj, int* adjncy,
                           int* perm, int* invp, int* colcnt, int* nnzl, int* nsub,
                           int* nsuper, int* snode, int* xsuper,
                           int* iwsiz, int* iwork, int* iflag);

using namespace types;

Function::ReturnValue sci_sfinit(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 8)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "sfinit", 8);
        return Function::Error;
    }

    if (_iRetCount != 9)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "sfinit", 9);
        return Function::Error;
    }


    //get argument #1
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 1);
        return Function::Error;
    }

    Double* pdbl1 = in[0]->getAs<Double>();
    pdbl1->convertToInteger();
    int* neqns = (int*)pdbl1->get();

    //get argument #2
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 2);
        return Function::Error;
    }

    Double* pdbl2 = in[1]->getAs<Double>();
    pdbl2->convertToInteger();
    int* nnza = (int*)pdbl2->get();

    //get argument #3
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 3);
        return Function::Error;
    }

    Double* pdbl3 = in[2]->getAs<Double>();
    pdbl3->convertToInteger();
    int* xadj = (int*)pdbl3->get();

    //get argument #4
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 4);
        return Function::Error;
    }

    Double* pdbl4 = in[3]->getAs<Double>();
    pdbl4->convertToInteger();
    int* adjncy = (int*)pdbl4->get();

    //get argument #5
    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 5);
        return Function::Error;
    }

    Double* pdbl5 = in[4]->getAs<Double>();
    pdbl5->convertToInteger();
    int* perm = (int*)pdbl5->get();

    //get argument #6
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 6);
        return Function::Error;
    }

    Double* pdbl6 = in[5]->getAs<Double>();
    pdbl6->convertToInteger();
    int* invp = (int*)pdbl6->get();

    //get argument #7
    if (in[6]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 7);
        return Function::Error;
    }

    Double* pdbl7 = in[6]->getAs<Double>();
    pdbl7->convertToInteger();
    int* iwsiz = (int*)pdbl7->get();

    //get argument #8
    if (in[7]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 8);
        return Function::Error;
    }

    Double* pdbl8 = in[7]->getAs<Double>();
    pdbl8->convertToInteger();
    int* iwork = (int*)pdbl8->get();

    Double* pdbl9 = new Double(neqns[0], 1);
    pdbl9->convertToInteger();
    int* colcnt = (int*)pdbl9->get();

    Double* pdbl10 = new Double(1, 1);
    pdbl10->convertToInteger();
    int* nnzl = (int*)pdbl10->get();

    Double* pdbl11 = new Double(1, 1);
    pdbl11->convertToInteger();
    int* nsub = (int*)pdbl11->get();

    Double* pdbl12 = new Double(1, 1);
    pdbl12->convertToInteger();
    int* nsuper = (int*)pdbl12->get();

    Double* pdbl13 = new Double(neqns[0], 1);
    pdbl13->convertToInteger();
    int* snode = (int*)pdbl13->get();

    Double* pdbl14 = new Double(neqns[0] + 1, 1);
    pdbl14->convertToInteger();
    int* xsuper = (int*)pdbl14->get();

    Double* pdbl15 = new Double(1, 1);
    pdbl15->convertToInteger();
    int* iflag = (int*)pdbl15->get();

    C2F(sfinit)(neqns, nnza, xadj, adjncy, perm, invp, colcnt, nnzl, nsub,
                nsuper, snode, xsuper, iwsiz, iwork, iflag);

    if (iflag[0])
    {
        delete pdbl9;
        delete pdbl10;
        delete pdbl11;
        delete pdbl12;
        delete pdbl13;
        delete pdbl14;
        delete pdbl15;
        Scierror(999, _("%s: insufficient working storage"), "sfinit");
        return Function::Error;
    }

    pdbl1->convertFromInteger();
    pdbl2->convertFromInteger();
    pdbl3->convertFromInteger();
    pdbl4->convertFromInteger();
    pdbl5->convertFromInteger();
    pdbl6->convertFromInteger();
    pdbl7->convertFromInteger();
    pdbl8->convertFromInteger();
    pdbl9->convertFromInteger();
    pdbl10->convertFromInteger();
    pdbl11->convertFromInteger();
    pdbl12->convertFromInteger();
    pdbl13->convertFromInteger();
    pdbl14->convertFromInteger();
    pdbl15->convertFromInteger();

    out.push_back(pdbl5);
    out.push_back(pdbl6);
    out.push_back(pdbl9);
    out.push_back(pdbl10);
    out.push_back(pdbl11);
    out.push_back(pdbl12);
    out.push_back(pdbl13);
    out.push_back(pdbl14);
    out.push_back(pdbl15);

    return Function::OK;
}

