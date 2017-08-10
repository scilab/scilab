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

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "eigen.h"
#include "issymmetric.h"
#include "vfinite.h"
}

bool isNoZeroImag(types::Double* _pDbl);
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_spec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double* pDataA          = NULL;
    double* pDataB          = NULL;
    bool symmetric          = FALSE;
    int iRet                = 0;

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "spec", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2 * in.size())
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "spec", 1, 2 * in.size());
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_spec";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* in0 = in[0]->getAs<types::Double>();

    if (in0->getCols() != in0->getRows())
    {
        Scierror(20, _("%s: Wrong type for input argument #%d: Square matrix expected.\n"), "spec", 1);
        return types::Function::Error;
    }

    if (in0->getRows() == -1 || in0->getCols() == -1) // manage eye case
    {
        Scierror(271, _("%s: Size varying argument a*eye(), (arg %d) not allowed here.\n"), "spec", 1);
        return types::Function::Error;
    }

    if (in0->getCols() == 0 || in0->getRows() == 0) // size null
    {
        out.push_back(types::Double::Empty());
        for (int i = 1; i < _iRetCount; i++)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    types::Double* pDblA = in0->clone()->getAs<types::Double>();

    if (in.size() == 1)
    {
        types::Double* pDblEigenValues  = NULL;
        types::Double* pDblEigenVectors = NULL;

        if (pDblA->isComplex())
        {
            pDataA = (double*)oGetDoubleComplexFromPointer(pDblA->getReal(), pDblA->getImg(), pDblA->getSize());
            if (!pDataA)
            {
                pDblA->killMe();
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "spec");
                return types::Function::Error;
            }
        }
        else
        {
            pDataA = pDblA->getReal();
        }

        int totalSize = pDblA->getSize();
        if ((pDblA->isComplex() ? C2F(vfiniteComplex)(&totalSize, (doublecomplex*)pDataA) : C2F(vfinite)(&totalSize, pDataA)) == false)
        {
            if (pDblA->isComplex())
            {
                vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
            }
            pDblA->killMe();
            Scierror(264, _("%s: Wrong value for input argument %d: Must not contain NaN or Inf.\n"), "spec", 1);
            return types::Function::Error;
        }

        symmetric = isSymmetric(pDblA->getReal(), pDblA->getImg(), pDblA->isComplex(), pDblA->getRows(), pDblA->getCols()) == 1;
        int eigenValuesCols = (_iRetCount == 1) ? 1 : pDblA->getCols();

        if (symmetric)
        {
            pDblEigenValues = new types::Double(pDblA->getCols(), eigenValuesCols);
            if (_iRetCount == 2)
            {
                pDblEigenVectors = new types::Double(pDblA->getCols(), pDblA->getCols(), pDblA->isComplex());
            }
        }
        else
        {
            pDblEigenValues  = new types::Double(pDblA->getCols(), eigenValuesCols, true);
            if (_iRetCount == 2)
            {
                pDblEigenVectors = new types::Double(pDblA->getCols(), pDblA->getCols(), true);
            }
        }

        if (pDblA->isComplex())
        {
            if (symmetric)
            {
                iRet = iEigen1ComplexSymmetricM((doublecomplex*)pDataA, pDblA->getCols(), (_iRetCount == 2), pDblEigenValues->getReal());

                if (iRet < 0)
                {
                    vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
                    pDblA->killMe();
                    Scierror(998, _("%s: On entry to ZGEEV parameter number  3 had an illegal value (lapack library problem).\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (iRet > 0)
                {
                    vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
                    pDblA->killMe();
                    Scierror(24, _("%s: Convergence problem, %d off-diagonal elements of an intermediate tridiagonal form did not converge to zero.\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (_iRetCount == 2)
                {
                    vGetPointerFromDoubleComplex((doublecomplex*)pDataA, pDblA->getSize() , pDblEigenVectors->getReal(), pDblEigenVectors->getImg());
                    vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
                    expandToDiagonalOfMatrix(pDblEigenValues->getReal(), pDblA->getCols());
                    out.push_back(pDblEigenVectors);
                }
                out.push_back(pDblEigenValues);
                pDblA->killMe();
            }
            else // not symmetric
            {
                doublecomplex* pEigenValues = (doublecomplex*)MALLOC(pDblA->getCols() * sizeof(doublecomplex));
                doublecomplex* pEigenVectors = pDblEigenVectors ? (doublecomplex*)MALLOC(sizeof(doublecomplex) * pDblA->getSize()) : NULL;
                iRet = iEigen1ComplexM((doublecomplex*)pDataA, pDblA->getCols(), pEigenValues, pEigenVectors);
                vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
                if (iRet < 0)
                {
                    pDblA->killMe();
                    Scierror(998, _("%s: On entry to ZHEEV parameter number  3 had an illegal value (lapack library problem).\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (iRet > 0)
                {
                    pDblA->killMe();
                    Scierror(24, _("%s: The QR algorithm failed to compute all the eigenvalues, and no eigenvectors have been computed. Elements and %d+1:N of W contain eigenvalues which have converged.\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (_iRetCount == 2)
                {
                    expandZToDiagonalOfCMatrix(pEigenValues, pDblA->getCols(), pDblEigenValues->getReal(), pDblEigenValues->getImg());
                    vGetPointerFromDoubleComplex(pEigenVectors, pDblA->getSize(), pDblEigenVectors->getReal(), pDblEigenVectors->getImg());

                    FREE(pEigenVectors);
                    out.push_back(pDblEigenVectors);
                }
                else
                {
                    vGetPointerFromDoubleComplex(pEigenValues, pDblA->getCols(), pDblEigenValues->getReal(), pDblEigenValues->getImg());
                }
                out.push_back(pDblEigenValues);
                FREE(pEigenValues);
                pDblA->killMe();
            }
        }
        else // real
        {
            if (symmetric)
            {
                iRet = iEigen1RealSymmetricM(pDataA, pDblA->getCols(), (_iRetCount == 2), pDblEigenValues->getReal());
                if (iRet < 0)
                {
                    pDblA->killMe();
                    Scierror(998, _("%s: On entry to ZGEEV parameter number  3 had an illegal value (lapack library problem).\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (iRet > 0)
                {
                    pDblA->killMe();
                    Scierror(24, _("%s: Convergence problem, %d off-diagonal elements of an intermediate tridiagonal form did not converge to zero.\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (_iRetCount == 2)
                {
                    expandToDiagonalOfMatrix(pDblEigenValues->getReal(), pDblA->getCols());
                    out.push_back(pDblA);
                }
                else
                {
                    pDblA->killMe();
                }

                out.push_back(pDblEigenValues);
            }
            else // not symmetric
            {
                iRet = iEigen1RealM(pDataA, pDblA->getCols(), pDblEigenValues->getReal(), pDblEigenValues->getImg(), pDblEigenVectors ? pDblEigenVectors->getReal() : NULL, pDblEigenVectors ? pDblEigenVectors->getImg() : NULL);

                if (iRet < 0)
                {
                    pDblA->killMe();
                    Scierror(998, _("%s: On entry to ZHEEV parameter number  3 had an illegal value (lapack library problem).\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (iRet > 0)
                {
                    pDblA->killMe();
                    Scierror(24, _("%s: The QR algorithm failed to compute all the eigenvalues, and no eigenvectors have been computed. Elements and %d+1:N of WR and WI contain eigenvalues which have converged.\n"), "spec", iRet);
                    return types::Function::Error;
                }

                if (_iRetCount == 2)
                {
                    expandToDiagonalOfMatrix(pDblEigenValues->getReal(), pDblA->getCols());
                    expandToDiagonalOfMatrix(pDblEigenValues->getImg(), pDblA->getCols());
                    out.push_back(pDblEigenVectors);
                }

                out.push_back(pDblEigenValues);
                pDblA->killMe();
            }
        }

        return types::Function::OK;
    }

    if (in.size() == 2)
    {
        types::Double* pDblL            = NULL;
        types::Double* pDblR            = NULL;
        types::Double* pDblBeta         = NULL;
        types::Double* pDblAlpha        = NULL;
        doublecomplex* pL               = NULL;
        doublecomplex* pR               = NULL;
        doublecomplex* pBeta            = NULL;
        doublecomplex* pAlpha           = NULL;
        bool bIsComplex                 = false;

        if (in[1]->isDouble() == false)
        {
            std::wstring wstFuncName = L"%" + in[1]->getShortTypeStr() + L"_spec";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        types::Double* in1 = in[1]->getAs<types::Double>();

        if (in1->getCols() != in1->getRows())
        {
            Scierror(20, _("%s: Wrong type for input argument #%d: Square matrix expected.\n"), "spec", 2);
            return types::Function::Error;
        }

        if (pDblA->getRows() != in1->getRows() && pDblA->getCols() != in1->getCols())
        {
            pDblA->killMe();
            Scierror(999, _("%s: Arguments %d and %d must have equal dimensions.\n"), "spec", 1, 2);
            return types::Function::Error;
        }

        //chekc if A and B are real complex or with imag part at 0
        if (isNoZeroImag(pDblA) == false && isNoZeroImag(in1) == false)
        {
            //view A and B as real matrix
            bIsComplex = false;
        }
        else
        {
            bIsComplex = pDblA->isComplex() || in1->isComplex();
        }

        types::Double* pDblB = in1->clone()->getAs<types::Double>();
        if (bIsComplex)
        {
            if (pDblA->isComplex() == false)
            {
                pDblA->setComplex(true);
            }

            if (pDblB->isComplex() == false)
            {
                pDblB->setComplex(true);
            }

            pDataA = (double*)oGetDoubleComplexFromPointer(pDblA->getReal(), pDblA->getImg(), pDblA->getSize());
            pDataB = (double*)oGetDoubleComplexFromPointer(pDblB->getReal(), pDblB->getImg(), pDblB->getSize());

            if (!pDataA && !pDataB)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "spec");
                return types::Function::Error;
            }

            if (!pDataA)
            {
                vFreeDoubleComplexFromPointer((doublecomplex*)pDataB);
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "spec");
                return types::Function::Error;
            }
            
            if (!pDataB)
            {
                vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "spec");
                return types::Function::Error;
            }
        }
        else
        {
            pDataA = pDblA->getReal();
            pDataB = pDblB->getReal();
        }

        int totalSize = pDblA->getSize();

        if ((pDblA->isComplex() ? C2F(vfiniteComplex)(&totalSize, (doublecomplex*)pDataA) : C2F(vfinite)(&totalSize, pDataA)) == false)
        {
            pDblA->killMe();
            pDblB->killMe();
            Scierror(264, _("%s: Wrong value for input argument %d: Must not contain NaN or Inf.\n"), "spec", 1);
            return types::Function::Error;
        }

        if ((pDblB->isComplex() ? C2F(vfiniteComplex)(&totalSize, (doublecomplex*)pDataB) : C2F(vfinite)(&totalSize, pDataB)) == false)
        {
            pDblA->killMe();
            pDblB->killMe();
            Scierror(264, _("%s: Wrong value for input argument %d: Must not contain NaN or Inf.\n"), "spec", 2);
            return types::Function::Error;
        }

        switch (_iRetCount)
        {
            case 4:
            {
                pDblL = new types::Double(pDblA->getRows(), pDblA->getCols(), true);
                if (bIsComplex)
                {
                    pL = (doublecomplex*)MALLOC(pDblA->getSize() * sizeof(doublecomplex));
                }
            }
            case 3:
            {
                pDblR = new types::Double(pDblA->getRows(), pDblA->getCols(), true);
                if (bIsComplex)
                {
                    pR = (doublecomplex*)MALLOC(pDblA->getSize() * sizeof(doublecomplex));
                }
            }
            case 2:
            {
                if (bIsComplex)
                {
                    pBeta = (doublecomplex*)MALLOC(pDblA->getCols() * sizeof(doublecomplex));
                }
                pDblBeta = new types::Double(pDblA->getCols(), 1, pBeta ? true : false);
            }
            default : // case 1:
            {
                if (bIsComplex)
                {
                    pAlpha = (doublecomplex*)MALLOC(pDblA->getCols() * sizeof(doublecomplex));
                }
                pDblAlpha = new types::Double(pDblA->getCols(), 1, true);
            }
        }

        if (bIsComplex)
        {
            iRet = iEigen2ComplexM((doublecomplex*)pDataA, (doublecomplex*)pDataB, pDblA->getCols(), pAlpha, pBeta, pR, pL);
        }
        else
        {
            iRet = iEigen2RealM(    pDataA, pDataB, pDblA->getCols(),
                                    pDblAlpha->getReal(), pDblAlpha->getImg(),
                                    pDblBeta ? pDblBeta->getReal()  : NULL,
                                    pDblR    ? pDblR->getReal()     : NULL,
                                    pDblR    ? pDblR->getImg()      : NULL,
                                    pDblL    ? pDblL->getReal()     : NULL,
                                    pDblL    ? pDblL->getImg()      : NULL);
        }

        if (iRet > 0)
        {
            sciprint(_("Warning :\n"));
            sciprint(_("Non convergence in the QZ algorithm.\n"));
            sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"), iRet);
        }

        if (iRet < 0)
        {
            pDblA->killMe();
            pDblB->killMe();
            Scierror(998, _("%s: On entry to ZHEEV parameter number  3 had an illegal value (lapack library problem).\n"), "spec", iRet);
            return types::Function::Error;
        }

        if (iRet > 0)
        {
            if (bIsComplex)
            {
                if (iRet <= pDblA->getCols())
                {
                    Scierror(24, _("%s: The QZ iteration failed in DGGEV.\n"), "spec");
                }
                else
                {
                    if (iRet == pDblA->getCols() + 1)
                    {
                        Scierror(999, _("%s: Other than QZ iteration failed in DHGEQZ.\n"), "spec");
                    }
                    if (iRet == pDblA->getCols() + 2)
                    {
                        Scierror(999, _("%s: Error return from DTGEVC.\n"), "spec");
                    }
                }
            }
            else
            {
                Scierror(24, _("%s: The QR algorithm failed to compute all the eigenvalues, and no eigenvectors have been computed. Elements and %d+1:N of W contain eigenvalues which have converged.\n"), "spec", iRet);
            }

            pDblA->killMe();
            pDblB->killMe();
            if (pDataA)
            {
                vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
            }

            if (pDataB)
            {
                vFreeDoubleComplexFromPointer((doublecomplex*)pDataB);
            }
            return types::Function::Error;
        }

        if (bIsComplex)
        {
            switch (_iRetCount)
            {
                case 4:
                    vGetPointerFromDoubleComplex(pL, pDblA->getSize(), pDblL->getReal(), pDblL->getImg());
                case 3:
                    vGetPointerFromDoubleComplex(pR, pDblA->getSize(), pDblR->getReal(), pDblR->getImg());
                case 2:
                    vGetPointerFromDoubleComplex(pBeta, pDblA->getCols(), pDblBeta->getReal(), pDblBeta->getImg());
                default : // case 1:
                    vGetPointerFromDoubleComplex(pAlpha, pDblA->getCols(), pDblAlpha->getReal(), pDblAlpha->getImg());
            }
        }

        switch (_iRetCount)
        {
            case 1:
            {
                out.push_back(pDblAlpha);
                break;
            }
            case 2:
            {
                out.push_back(pDblAlpha);
                out.push_back(pDblBeta);
                break;
            }
            case 3:
            {
                out.push_back(pDblAlpha);
                out.push_back(pDblBeta);
                out.push_back(pDblR);
                break;
            }
            case 4:
            {
                out.push_back(pDblAlpha);
                out.push_back(pDblBeta);
                out.push_back(pDblL);
                out.push_back(pDblR);
            }
        }

        if (pAlpha)
        {
            vFreeDoubleComplexFromPointer(pAlpha);
        }
        if (pBeta)
        {
            vFreeDoubleComplexFromPointer(pBeta);
        }
        if (pL)
        {
            vFreeDoubleComplexFromPointer(pL);
        }
        if (pR)
        {
            vFreeDoubleComplexFromPointer(pR);
        }
        if (bIsComplex && pDblB->isComplex())
        {
            vFreeDoubleComplexFromPointer((doublecomplex*)pDataB);
        }
        pDblB->killMe();

    } // if(in.size() == 2)

    if (pDblA->isComplex())
    {
        vFreeDoubleComplexFromPointer((doublecomplex*)pDataA);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
bool isNoZeroImag(types::Double* _pDbl)
{
    double* pdbl = _pDbl->getImg();
    if (pdbl)
    {
        for (int i = 0 ; i < _pDbl->getSize() ; i++)
        {
            if (pdbl[i])
            {
                return true;
            }
        }
    }
    return false;
}
/*--------------------------------------------------------------------------*/
