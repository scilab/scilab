/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_coeff(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblV    = NULL;
    types::Double* pDblOut  = NULL;
    types::Double* pDblIn   = NULL;
    types::Polynom* pPolyIn = NULL;

    double* pdblV = NULL;
    int iVSize    = 1;
    int iOne      = 1;
    double dZero  = 1;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "coeff", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "coeff", 1);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "coeff", 2);
            return types::Function::Error;
        }

        pDblV = in[1]->getAs<types::Double>();

        if (pDblV->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "coeff", 2);
            return types::Function::Error;
        }

        if (pDblV->isEmpty())
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        if (pDblV->getRows() != 1 && pDblV->getCols() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real vector expected.\n"), "coeff", 2);
            return types::Function::Error;
        }

        pdblV = pDblV->get();

        for (int i = 0; i < pDblV->getSize(); i++)
        {
            if (pdblV[i] < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Positive vector expected.\n"), "coeff", 2);
                return types::Function::Error;
            }
        }

        iVSize = pDblV->getSize();
    }

    if (in[0]->isDouble())
    {
        if (in.size() == 1)
        {
            out.push_back(in[0]);
            return types::Function::OK;
        }

        pDblIn = in[0]->getAs<types::Double>();
        pDblOut = new types::Double(pDblIn->getRows(), pDblIn->getCols() * iVSize, pDblIn->isComplex());
        int iSize = pDblIn->getSize();
        for (int i = 0; i < iVSize; i++)
        {
            if (pdblV[i] == 0)
            {
                C2F(dcopy)(&iSize, pDblIn->get(), &iOne, pDblOut->get() + i * iSize, &iOne);
                if (pDblIn->isComplex())
                {
                    C2F(dcopy)(&iSize, pDblIn->getImg(), &iOne, pDblOut->getImg() + i * iSize, &iOne);
                }
            }
            else
            {
                memset(pDblOut->get() + i * iSize, 0x00, pDblIn->getSize() * sizeof(double));
                if (pDblIn->isComplex())
                {
                    memset(pDblOut->getImg() + i * iSize, 0x00, pDblIn->getSize() * sizeof(double));
                }
            }
        }
    }
    else if (in[0]->isPoly())
    {
        pPolyIn = in[0]->getAs<types::Polynom>();
        int iPolySize = pPolyIn->getSize();

        if (in.size() == 1)
        {
            pDblOut = new types::Double(pPolyIn->getRows(), pPolyIn->getCols() * (pPolyIn->getMaxRank() + 1), pPolyIn->isComplex());
            double* pdblOut = pDblOut->get();
            memset(pdblOut, 0x00, pDblOut->getSize() * sizeof(double));
            if (pPolyIn->isComplex())
            {
                double* pdblOutI = pDblOut->getImg();
                memset(pdblOutI, 0x00, pDblOut->getSize() * sizeof(double));
                for (int i = 0; i < iPolySize; i++)
                {
                    double* pdblcoeffR = pPolyIn->get(i)->get();
                    double* pdblcoeffI = pPolyIn->get(i)->getImg();
                    for (int j = 0; j < pPolyIn->get(i)->getSize(); j++)
                    {
                        pdblOut[j * iPolySize + i] = pdblcoeffR[j];
                        pdblOutI[j * iPolySize + i] = pdblcoeffI[j];
                    }
                }
            }
            else
            {
                for (int i = 0; i < iPolySize; i++)
                {
                    double* pdblcoeff = pPolyIn->get(i)->get();
                    for (int j = 0; j < pPolyIn->get(i)->getSize(); j++)
                    {
                        pdblOut[j * iPolySize + i] = pdblcoeff[j];
                    }
                }
            }
        }
        else
        {
            pDblOut = new types::Double(pPolyIn->getRows(), pPolyIn->getCols() * iVSize, pPolyIn->isComplex());
            double* pdblOut = pDblOut->get();
            memset(pdblOut, 0x00, pDblOut->getSize() * sizeof(double));
            if (pPolyIn->isComplex())
            {
                double* pdblOutI = pDblOut->getImg();
                memset(pdblOutI, 0x00, pDblOut->getSize() * sizeof(double));
                for (int i = 0; i < iPolySize; i++)
                {
                    double* pdblcoeffR = pPolyIn->get(i)->get();
                    double* pdblcoeffI = pPolyIn->get(i)->getImg();
                    for (int j = 0; j < iVSize; j++)
                    {
                        int iCoeffPos = (int)pdblV[j];
                        if (iCoeffPos > pPolyIn->get(i)->getRank())
                        {
                            pdblOut[j * iPolySize + i] = 0;
                            pdblOutI[j * iPolySize + i] = 0;
                        }
                        else
                        {
                            pdblOut[j * iPolySize + i] = pdblcoeffR[iCoeffPos];
                            pdblOutI[j * iPolySize + i] = pdblcoeffI[iCoeffPos];
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < iPolySize; i++)
                {
                    double* pdblcoeff = pPolyIn->get(i)->get();
                    for (int j = 0; j < iVSize; j++)
                    {
                        int iCoeffPos = (int)pdblV[j];
                        if (iCoeffPos > pPolyIn->get(i)->getRank())
                        {
                            pdblOut[j * iPolySize + i] = 0;
                        }
                        else
                        {
                            pdblOut[j * iPolySize + i] = pdblcoeff[iCoeffPos];
                        }
                    }
                }
            }
        }
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_coeff";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

