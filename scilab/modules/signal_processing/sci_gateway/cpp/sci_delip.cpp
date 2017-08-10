/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
/*--------------------------------------------------------------------------*/

#include "signal_gw.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"

    extern void C2F(delip)(int *length,
                           double *output_real, double *output_imaginary,
                           double *x, double *ck);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_delip(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bBecomeComplex = false;
    //check input parameters
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "delip", 2);
        return types::Function::Error;
    }

    //check first input parameter : x
    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "delip", 1);
        return types::Function::Error;
    }

    types::Double* pIn1 = in[0]->getAs<types::Double>();
    double* pdblIn1 = pIn1->get();
    int iSize = pIn1->getSize();

    for (int i = 0 ; i < iSize ; i++)
    {
        if (pdblIn1[i] < 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), "delip", 1, "0", "+INF");
            return types::Function::Error;
        }
        else if (pdblIn1[i] > 1)
        {
            bBecomeComplex = true;
        }
    }

    //check second input parameter : ck
    if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "delip", 2);
        return types::Function::Error;
    }

    double pdblCK = in[1]->getAs<types::Double>()->get(0);
    if (pdblCK < -1 || pdblCK > 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), "delip", 2, "-1", "1");
        return types::Function::OK;
    }

    //alloc output parameter
    types::Double* pOut = new types::Double(pIn1->getRows(), pIn1->getCols(), true);

    //call math function
    C2F(delip)(&iSize, pOut->getReal(), pOut->getImg(), pIn1->get(), &pdblCK);

    //update complexity of output parameter
    pOut->setComplex(bBecomeComplex);

    out.push_back(pOut);
    return types::Function::OK;
}
