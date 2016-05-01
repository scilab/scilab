/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "svd.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/
/*
  s=svd(X): [R: min(rows, cols) x 1]
  [U,S,V]=svd(X) [U,S,V]:  [ [C|R: rows x rows], [R: rows x cols ],  [R|C: cols x cols ] ]
  [U,S,V]=svd(X,0) (obsolete) [U,S,V]=svd(X,"e"): [ [C|R:rows x min(rows,cols)], [R: min(rows,cols) x min(rows,cols)], [C|R:cols x min(rows,cols)] ]
  [U,S,V,rk]=svd(X [,tol]) : cf. supra, rk[R 1 x 1]

  /!\ Contrary to specifications (@ http://www.scilab.org/product/man/svd.html )
  , previous version was accepting Lhs==2. Worse : tests were using this undocumented behavior.
  implementation and tests have been fixed according to the specification.

*/
extern "C"
{
    extern int C2F(vfinite)(int *n, double *v);
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_svd(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl     = NULL;
    types::Double* ptrsU    = NULL;
    types::Double* ptrsV    = NULL;
    types::Double* ptrS     = NULL;
    types::Double* pRk      = NULL;
    types::Double* pSV      = NULL;
    double* pData           = NULL;
    int economy             = 0;
    int totalsize           = 0;
    int iRet                = 0;
    double tol              = 0.;

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "svd", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): At least %d expected.\n"), "svd", 4);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_svd";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
    pDbl = in[0]->clone()->getAs<types::Double>();

    if (in.size() == 2)
    {
        if (in[1]->isString() == true)
        {
            if (_iRetCount == 4)
            {
                Scierror(78, _("%s: Wrong number of output argument(s): %d or %d expected.\n"), "svd", 1, 3);
                return types::Function::Error;
            }
            types::String* pStr = in[1]->getAs<types::String>();
            if ((wcslen(pStr->get(0)) == 1) && (pStr->get(0)[0] == L'e'))
            {
                economy = 1;
            }
        }
        if (in[1]->isDouble() == true)
        {
            /* no further testing for "old Economy size:  [U,S,V]=svd(A,0) " */
            if (_iRetCount == 3)
            {
                economy = 1;
            }
        }
    }

    if (pDbl->isEmpty()) /* empty matrix */
    {
        for (int i = 0; i < _iRetCount - 1; i++)
        {
            out.push_back(types::Double::Empty());
        }

        if (_iRetCount == 4)
        {
            types::Double* pDZero = new types::Double(1, 1);
            pDZero->set(0, 0);
            out.push_back(pDZero);
        }
        else
        {
            out.push_back(types::Double::Empty());
        }

        delete pDbl;
        return types::Function::OK;
    }

    if ((pDbl->getRows() == -1) || (pDbl->getCols() == -1)) // manage eye case
    {
        Scierror(271, _("%s: Size varying argument a*eye(), (arg %d) not allowed here.\n"), "svd", 1);
        delete pDbl;
        return types::Function::Error;
    }

    if (pDbl->isComplex())
    {
        pData = (double *)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
    }
    else
    {
        pData = pDbl->get();
    }

    totalsize = pDbl->getSize() * (pDbl->isComplex() ? 2 : 1);
    if (C2F(vfinite)(&totalsize, pData) == false)
    {
        Scierror(264, _("%s: Wrong value for argument %d: Must not contain NaN or Inf.\n"), "svd", 1);
        delete pDbl;
        return types::Function::Error;
    }

    switch (_iRetCount)
    {
        case 0:
        case 1:
        {
            pSV = new types::Double(std::min(pDbl->getRows(), pDbl->getCols()), 1, false);
            iRet = iSvdM(pData, pDbl->getRows(), pDbl->getCols(), pDbl->isComplex(), economy, tol, pSV->get(), NULL, NULL, NULL, NULL);
        }
        break;
        case 4:
        {
            if ((in.size() == 2) && (in[1]->isDouble()))
            {
                tol = in[1]->getAs<types::Double>()->get(0);
            }
            pRk = new types::Double(1, 1, false);
        }
        case 2:
        case 3:
        {
            int economyRows = economy ? std::min(pDbl->getRows(), pDbl->getCols()) : pDbl->getRows();
            int economyCols = economy ? std::min(pDbl->getRows(), pDbl->getCols()) : pDbl->getCols();

            ptrsU = new types::Double(pDbl->getRows(), economyRows, pDbl->isComplex());
            ptrS  = new types::Double(economyRows, economyCols, false);
            ptrsV = new types::Double(pDbl->getCols(), economyCols, pDbl->isComplex());

            if (pDbl->isComplex())
            {
                double* U = (double*)MALLOC(pDbl->getRows() * economyRows * sizeof(doublecomplex));
                double* V = (double*)MALLOC(pDbl->getCols() * economyCols * sizeof(doublecomplex));

                iRet = iSvdM(pData, pDbl->getRows(), pDbl->getCols(), true /*isComplex*/, economy, tol, NULL, U, ptrS->get(), V, (pRk ? pRk->get() : NULL));

                vGetPointerFromDoubleComplex((doublecomplex*)U, ptrsU->getSize(), ptrsU->getReal(), ptrsU->getImg());
                vFreeDoubleComplexFromPointer((doublecomplex*)U);
                vGetPointerFromDoubleComplex((doublecomplex*)V, ptrsV->getSize(), ptrsV->getReal(), ptrsV->getImg());
                vFreeDoubleComplexFromPointer((doublecomplex*)V);
            }
            else
            {
                iRet = iSvdM(pData, pDbl->getRows(), pDbl->getCols(), false /*isComplex*/, economy, tol, NULL, ptrsU->get(), ptrS->get(), ptrsV->get(), (pRk ? pRk->get() : NULL));
            }
        }
        break;
            // default: // makes at the beginning of this gateway
    }

    if (iRet != 0)
    {
        if (iRet == -1)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), "svd");
        }
        else
        {
            Scierror(24, _("%s: Convergence problem...\n"), "svd");
        }

        delete pDbl;
        return types::Function::Error;
    }

    if (pDbl->isComplex())
    {
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }

    switch (_iRetCount)
    {
        case 4:
        {
            out.push_back(ptrsU);
            out.push_back(ptrS);
            out.push_back(ptrsV);
            out.push_back(pRk);
            break;
        }
        case 3:
        {
            out.push_back(ptrsU);
            out.push_back(ptrS);
            out.push_back(ptrsV);
            break;
        }
        case 2:
        {
            out.push_back(ptrsU);
            out.push_back(ptrS);
            delete ptrsV;
            break;
        }
        case 1:
            out.push_back(pSV);
            // default: // makes at the beginning of this gateway
    }

    delete pDbl;
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

