/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    if(in.size() != 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"delip", 2);
        return types::Function::Error;
    }

    //check first input parameter : x
    if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isComplex() == true)
    {
        ScierrorW(999, _W("%ls: Wrong type for argument %d: Real matrix expected.\n"), L"delip", 1);
        return types::Function::Error;
    }

    types::Double* pIn1 = in[0]->getAs<types::Double>();
    double* pdblIn1 = pIn1->get();
    int iSize = pIn1->getSize();

    for(int i = 0 ; i < iSize ; i++)
    {
        if(pdblIn1[i] < 0)
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Must be in the interval [%ls, %ls].\n"), L"delip", 1, L"0", L"+INF");
            return types::Function::Error;
        }
        else if(pdblIn1[i] > 1)
        {
            bBecomeComplex = true;
        }
    }

    //check second input parameter : ck
    if(in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex() == true)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Real scalar expected.\n"), L"delip", 2);
        return types::Function::Error;
    }

    double pdblCK = in[1]->getAs<types::Double>()->get(0);
    if(pdblCK < -1 || pdblCK > 1)
    {
      ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Must be in the interval [%ls, %ls].\n"), L"delip", 2, L"-1", L"1");
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
