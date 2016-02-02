/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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

#include <cmath>

#include "elem_func_gw.hxx"
#include "api_scilab.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "polynom.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "abs.h"
}

template <class T>
T* absInt(T* _pIn)
{
    T* pIntOut = new T(_pIn->getDims(), _pIn->getDimsArray());
    int size = _pIn->getSize();

    typename T::type* pI = _pIn->get();
    typename T::type* pO = pIntOut->get();
    for (int i = 0; i < size; i++)
    {
        pO[i] = std::abs(pI[i]);
    }

    return pIntOut;
}
/*
clear a; nb = 2500; a = rand(nb, nb) - 0.5; tic(); abs(a); toc
clear a; nb = 2500; a = rand(nb, nb) - 0.5; a = a + a *%i; tic(); abs(a); toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_abs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "abs", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "abs", 1);
        return types::Function::Error;
    }

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            api_scilab::Double* pDblIn = api_scilab::getAsDouble(in[0]);
            api_scilab::Double* pDblOut = new api_scilab::Double(pDblIn->getDims(), pDblIn->getDimsArray());

            double* pdblInR = pDblIn->get();
            double* pdblInI = pDblIn->getImg();
            double* pdblOut = pDblOut->get();
            int size = pDblIn->getSize();
            if (pDblIn->isComplex())
            {
                for (int i = 0; i < size; i++)
                {
                    if (ISNAN(pdblInR[i]))
                    {
                        pdblOut[i] = pdblInR[i];
                    }
                    else if (ISNAN(pdblInI[i]))
                    {
                        pdblOut[i] = pdblInI[i];
                    }
                    else
                    {
                        pdblOut[i] = dabsz(pdblInR[i], pdblInI[i]);
                    }
                }
            }
            else
            {
                for (int i = 0; i < size; i++)
                {
                    if (ISNAN(pdblInR[i]))
                    {
                        pdblOut[i] = pdblInR[i];
                    }
                    else
                    {
                        pdblOut[i] = std::fabs(pdblInR[i]);
                    }
                }
            }

            out.push_back(api_scilab::getReturnVariable(pDblOut));
            delete pDblOut;
            delete pDblIn;
            break;
        }
        case types::InternalType::ScilabPolynom:
        {
            types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
            types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());
            double* data = NULL;

            if (pPolyIn->isComplex())
            {
                for (int i = 0; i < pPolyIn->getSize(); i++)
                {
                    int rank = pPolyIn->get(i)->getRank();
                    types::SinglePoly* pSP = new types::SinglePoly(&data, rank);

                    for (int j = 0; j < rank + 1; j++)
                    {
                        data[j] = dabsz(pPolyIn->get(i)->get()[j], pPolyIn->get(i)->getImg()[j]);
                    }

                    pPolyOut->set(i, pSP);
                    delete pSP;
                    pSP = NULL;
                }
            }
            else
            {
                for (int i = 0; i < pPolyIn->getSize(); i++)
                {
                    int rank = pPolyIn->get(i)->getRank();
                    types::SinglePoly* pSP = new types::SinglePoly(&data, rank);

                    for (int j = 0; j < rank + 1; j++)
                    {
                        data[j] = dabss(pPolyIn->get(i)->get()[j]);
                    }

                    pPolyOut->set(i, pSP);
                    delete pSP;
                    pSP = NULL;
                }
            }

            out.push_back(pPolyOut);
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            out.push_back(absInt(in[0]->getAs<types::Int8>()));
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            out.push_back(absInt(in[0]->getAs<types::Int16>()));
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            out.push_back(absInt(in[0]->getAs<types::Int32>()));
            break;
        }
        case types::InternalType::ScilabInt64:
        {
            out.push_back(absInt(in[0]->getAs<types::Int64>()));
            break;
        }
        case types::InternalType::ScilabUInt8:
        case types::InternalType::ScilabUInt16:
        case types::InternalType::ScilabUInt32:
        case types::InternalType::ScilabUInt64:
        {
            out.push_back(in[0]);
            break;
        }
        default:
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_abs";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
