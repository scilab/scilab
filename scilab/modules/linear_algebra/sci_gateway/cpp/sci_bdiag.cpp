/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "core_math.h" /* for Abs  macro */
}
/*--------------------------------------------------------------------------*/
extern "C"
{
extern int C2F(vfinite)(int const*n, double const*v);

extern void C2F(bdiag)(int const * lda, int const* n, double* a, double const * epsshr, double const* rMax, double* er, double* ei, int *bs
		       , double* x, double* xi, double* scale, int const* job, int * fail);
extern void C2F(wbdiag)(int const* lda, int const* n, double* ar, double* ai, double const*rMax, double* er, double* ei, int* bs
			, double* xr , double* xi, double* yr, double* yi, double* scale, int const* job, int* fail);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_bdiag(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblMatrix   = NULL;
    types::Double* pDblScalar   = NULL;
    double rMax                 = 0;

    if((in.size() != 1) && (in.size() != 2))
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"bdiag", 1, 2);
        return types::Function::Error;
    }
    if(_iRetCount > 3)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"bdiag", 1, 3);
        return types::Function::Error;
    }

    if(in[0]->isDouble() == false)
    {
        ScierrorW(201, _W("%ls: Wrong type for argument %d: Real or complex matrix expected.\n"), L"bdiag", 1);
        return types::Function::Error;
    }

    pDblMatrix = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>(); // input data will be modified
    if(pDblMatrix->getRows() != pDblMatrix->getCols())
    {
		ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"bdiag", 1);
        return types::Function::Error;
    }

    if(in.size() == 2)
    {
        if(in[1]->isDouble() == false && in[1]->getAs<types::Double>()->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for argument %d: A scalar expected.\n"), L"bdiag", 2);
            return types::Function::Error;
        }

        pDblScalar = in[1]->getAs<types::Double>();
    }

    if(pDblMatrix->getCols() == 0)
    {
        int value = Min(_iRetCount, 3);
        for(int i = 0; i < value; i++)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    int const totalSize = pDblMatrix->getSize();

    if( C2F(vfinite)(&totalSize, pDblMatrix->getReal()) == false &&
        (pDblMatrix->isComplex() == false ||
         C2F(vfinite)(&totalSize, pDblMatrix->getImg())))
    {
		ScierrorW(264, _W("%ls: Wrong value for argument %d: Must not contain NaN or Inf.\n"), L"bdiag", 1);
        return types::Function::Error;
    }

    if(pDblScalar != NULL)
    {
        rMax = pDblScalar->get(0);
    }
    else
    {
        for(int j = 0; j < pDblMatrix->getCols(); j++)
        {
            double t = 0.0;
            for(int i = 0; i < pDblMatrix->getCols(); i++)
            {
                t += Abs(pDblMatrix->get(i+j*pDblMatrix->getCols()));
            }
            rMax = Max(t, rMax);
        }
    }

    types::Double* lx = pDblMatrix->clone()->getAs<types::Double>();
    int iDim        = pDblMatrix->getCols(); // square matrix
    int fail        = 0;
    int const job   = 0;

    /* allocating the two memory buffers in one place as the original code did */
    double* le = (double*) MALLOC( 2*iDim * sizeof(double) );
    int*    lib= (int*) MALLOC(iDim * sizeof(int));
    double* lw = (double*)MALLOC(iDim * sizeof(double));

    if((le && lib && lw) == false)
    {
		ScierrorW(999, _W("%ls: Allocation failed.\n"), L"bdiag");
        return types::Function::Error;
    }

    if(pDblMatrix->isComplex())
    {
        double dummy;
        C2F(wbdiag)(&iDim, &iDim, pDblMatrix->getReal(), pDblMatrix->getImg(), &rMax, le, le + iDim, lib, lx->getReal() , lx->getImg(),  &dummy, &dummy, lw, &job, &fail);
    }
    else
    {
        double const epsshr= 0.;
        C2F(bdiag)(&iDim, &iDim, pDblMatrix->getReal(), &epsshr, &rMax, le, le + iDim, lib, lx->getReal(), NULL /* not used when job = 0 */, lw, &job, &fail);
    }

    if(fail)
    {
        FREE(le);
        FREE(lib);
        FREE(lw);
		ScierrorW(24, _W("%ls: Non convergence in QR steps.\n"), L"bdiag");
        return types::Function::Error;
    }
    else
    {
        out.push_back(pDblMatrix);
        if(_iRetCount > 1)
        {
            out.push_back(lx);
        }
        if(_iRetCount == 3)
        {
            int nbloc   = 0;
            int k       = 0;

            for(k = 0; k < iDim; k++)
            {
                if(lib[k] >= 0)
                {
                    nbloc++;
                }
            }

            types::Double* lbs = new types::Double(nbloc, 1);

            for (int i = k = 0; k < iDim; k++)
            {
                if(lib[k] >= 0)
                {
                    lbs->set(i,(double) lib[k]);
                    i++;
                }
            }
            out.push_back(lbs);
        }
        FREE(le);
        FREE(lib);
        FREE(lw);
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

