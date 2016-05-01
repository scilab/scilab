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

extern "C" int C2F(symfct)(int* neqns, int* adjen, int* xadj, int* adjncy,
                           int* perm, int* invp, int* colcnt, int* nsuper, int* xsuper,
                           int* snode, int* nofsub, int* xlindx,
                           int* lindx, int* xlnz, int* iwsiz, int* iwork, int* iflag);

using namespace types;

Function::ReturnValue sci_symfcti(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 13)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "symfcti", 13);
        return Function::Error;
    }

    if (_iRetCount != 4)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "symfcti", 4);
        return Function::Error;
    }


    //get argument #1
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 1);
        return Function::Error;
    }

    Double* pdbl1 = in[0]->getAs<Double>();
    pdbl1->convertToInteger();
    int* neqns = (int*)pdbl1->get();

    //get argument #2
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 2);
        return Function::Error;
    }

    Double* pdbl2 = in[1]->getAs<Double>();
    pdbl2->convertToInteger();
    int* adjen = (int*)pdbl2->get();

    //get argument #3
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 3);
        return Function::Error;
    }

    Double* pdbl3 = in[2]->getAs<Double>();
    pdbl3->convertToInteger();
    int* xadj = (int*)pdbl3->get();

    //get argument #4
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 4);
        return Function::Error;
    }

    Double* pdbl4 = in[3]->getAs<Double>();
    pdbl4->convertToInteger();
    int* adjncy = (int*)pdbl4->get();

    //get argument #5
    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 5);
        return Function::Error;
    }

    Double* pdbl5 = in[4]->getAs<Double>();
    pdbl5->convertToInteger();
    int* perm = (int*)pdbl5->get();

    //get argument #6
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 6);
        return Function::Error;
    }

    Double* pdbl6 = in[5]->getAs<Double>();
    pdbl6->convertToInteger();
    int* invp = (int*)pdbl6->get();

    //get argument #7
    if (in[6]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 7);
        return Function::Error;
    }

    Double* pdbl7 = in[6]->getAs<Double>();
    pdbl7->convertToInteger();
    int* colcnt = (int*)pdbl7->get();

    //get argument #8
    if (in[7]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 8);
        return Function::Error;
    }

    Double* pdbl8 = in[7]->getAs<Double>();
    pdbl8->convertToInteger();
    int* nsuper = (int*)pdbl8->get();

    //get argument #9
    if (in[8]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 9);
        return Function::Error;
    }

    Double* pdbl9 = in[8]->getAs<Double>();
    pdbl9->convertToInteger();
    int* xsuper = (int*)pdbl9->get();

    //get argument #10
    if (in[9]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 10);
        return Function::Error;
    }

    Double* pdbl10 = in[9]->getAs<Double>();
    pdbl10->convertToInteger();
    int* snode = (int*)pdbl10->get();

    //get argument #11
    if (in[10]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 11);
        return Function::Error;
    }

    Double* pdbl11 = in[10]->getAs<Double>();
    pdbl11->convertToInteger();
    int* nofsub = (int*)pdbl11->get();

    //get argument #12
    if (in[11]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 12);
        return Function::Error;
    }

    Double* pdbl12 = in[11]->getAs<Double>();
    pdbl12->convertToInteger();
    int* iwsiz = (int*)pdbl12->get();

    //get argument #13
    if (in[12]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "symfcti", 13);
        return Function::Error;
    }

    Double* pdbl13 = in[12]->getAs<Double>();
    pdbl13->convertToInteger();
    int* iwork = (int*)pdbl13->get();


    Double* pdbl14 = new Double(nsuper[0] + 1, 1);
    pdbl14->convertToInteger();
    int* xlindx = (int*)pdbl14->get();

    Double* pdbl15 = new Double(nofsub[0], 1);
    pdbl15->convertToInteger();
    int* lindx = (int*)pdbl15->get();

    Double* pdbl16 = new Double(neqns[0] + 1, 1);
    pdbl16->convertToInteger();
    int* xlnz = (int*)pdbl16->get();

    Double* pdbl17 = new Double(1, 1);
    pdbl17->convertToInteger();
    int* iflag = (int*)pdbl17->get();


    C2F(symfct)(neqns, adjen, xadj, adjncy,
                perm, invp, colcnt, nsuper, xsuper,
                snode, nofsub, xlindx, lindx,
                xlnz, iwsiz, iwork, iflag);

    if (iflag[0])
    {
        delete pdbl14;
        delete pdbl15;
        delete pdbl16;
        delete pdbl17;
        Scierror(999, _("%s: insufficient working storage"), "symfcti");
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
    pdbl16->convertFromInteger();
    pdbl17->convertFromInteger();

    out.push_back(pdbl14);
    out.push_back(pdbl15);
    out.push_back(pdbl16);
    out.push_back(pdbl17);

    return Function::OK;
}

