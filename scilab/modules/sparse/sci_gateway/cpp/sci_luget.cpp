/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

#include "sparse_gw.hxx"
#include "function.hxx"
#include "types.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "pointer.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
#include "lu.h"

}
types::Function::ReturnValue sci_luget(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double abstol   = 0;
    double reltol   = 0.001;
    int nrank       = 0;
    int ierr        = 0;
    int n1          = 0;
    int nl          = 0;
    int nu          = 0;
    int tp          = 0;
    int nonZerosP   = 0;
    int nonZerosL   = 0;
    int nonZerosU   = 0;
    int nonZerosQ   = 0;

    bool cplx           = false;
    const void* pData   = NULL;

    double* dblP    = NULL;
    double* dblL    = NULL;
    double* dblU    = NULL;
    double* dblQ    = NULL;
    int* itemsRowP  = NULL;
    int* itemsRowL  = NULL;
    int* itemsRowU  = NULL;
    int* itemsRow   = NULL;
    int* itemsRowQ  = NULL;
    int* fmatindex  = NULL;

    //check input parameters
    if (in.size() != 1 )
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d  expected.\n"), "luget", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 4)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "luget", 4);
        return types::Function::Error;
    }

    if (in[0]->isPointer() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d:  Handle to sparse lu factors expected.\n"), "luget", 1);
        return types::Function::Error;
    }

    types::Pointer *pPointerIn = in[0]->getAs<types::Pointer>();
    n1 = pPointerIn->getCols();
    cplx = pPointerIn->isComplex();
    pData = pPointerIn->get();
    fmatindex = (int*)pData;

    C2F(lusiz1)(fmatindex, &nl, &nu, &ierr);
    if (ierr > 0)
    {
        Scierror(999, _("Wrong value for argument #%d: the lu handle is no more valid.\n"), 1);
        return types::Function::Error;
    }

    dblP = new double[n1];
    dblL = new double[nl];
    dblU = new double[nu];
    dblQ = new double[n1];

    types::Sparse *p = new types::Sparse(n1, n1, cplx);
    types::Sparse *l = new types::Sparse(n1, n1, cplx);
    types::Sparse *u = new types::Sparse(n1, n1, cplx);
    types::Sparse *q = new types::Sparse(n1, n1, cplx);

    itemsRowP = new int[n1 + n1];
    itemsRowL = new int[nl + n1];
    itemsRowU = new int[nu + n1];
    itemsRowQ = new int[n1 + n1];

    C2F(luget1)(fmatindex, itemsRowP, dblP, itemsRowL, dblL, itemsRowU, dblU, itemsRowQ, dblQ, &ierr);

    tp = n1;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < itemsRowP[i]; j++)
        {
            p->set(i, itemsRowP[j + tp] - 1, dblP[j + tp - n1], true);
        }
        tp += itemsRowP[i];
    }

    tp = n1;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < itemsRowL[i]; j++)
        {

            l->set(i, itemsRowL[j + tp] - 1, dblL[j + tp - n1], true);
        }
        tp += itemsRowL[i];
    }

    tp = n1;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < itemsRowU[i]; j++)
        {
            u->set(i, itemsRowU[j + tp] - 1, dblU[j + tp - n1], true);
        }
        tp += itemsRowU[i];
    }

    tp = n1;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < itemsRowQ[i]; j++)
        {
            q->set(i, itemsRowQ[j + tp] - 1, dblQ[j + tp - n1], true);
        }
        tp += itemsRowQ[i];
    }

    out.push_back(p);
    out.push_back(l);
    out.push_back(u);
    out.push_back(q);

    delete[] dblP;
    delete[] dblL;
    delete[] dblU;
    delete[] dblQ;
    delete[] itemsRowP;
    delete[] itemsRowL;
    delete[] itemsRowU;
    delete[] itemsRowQ;

    return types::Function::OK;
}
