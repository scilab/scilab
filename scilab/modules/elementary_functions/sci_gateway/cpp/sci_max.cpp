/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "max.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_max(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iCountElem   = in.size();
    int iDims        = 0;
    int* piDimsArray = NULL;
    int iOrientation = 0;
    int iLargerInput = 0; // used to know the larger type of inputs arguments (in interger case). max(int16, int32, int8) return an int32
    int iSigned      = 0; // if one of input arguments is signed the result is signed.

    types::Double* pDblIndex    = NULL;
    types::Double* pDblOut      = NULL;

    std::vector<types::InternalType*> inputs;
    std::vector<types::Double*> vectDouble;

    if (in.size() < 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "max", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "max", 1, 2);
        return types::Function::Error;
    }

    /***** get data *****/
    if (in.size() == 1 && in[0]->isList())
    {
        types::List* pList = in[0]->getAs<types::List>();
        iCountElem = pList->getSize();

        if (iCountElem == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Non empty list expected.\n"), "max", 1);
            return types::Function::Error;
        }

        for (int i = 0; i < iCountElem; i++)
        {
            inputs.push_back(pList->get(i)->getAs<types::InternalType>());
        }
    }
    else
    {
        inputs = in;
    }

    if (inputs[0]->isDouble() == false && inputs[0]->isInt() == false)
    {
        std::wstring wstFuncName = L"%"  + inputs[0]->getShortTypeStr() + L"_max";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    types::GenericType* pGT = NULL;
    // get the size of the first input non scalar
    for (int i = 0; i < inputs.size(); i++)
    {
        pGT = inputs[i]->getAs<types::GenericType>();
        if (pGT->isScalar() == false)
        {
            break;
        }
    }

    iDims       = pGT->getDims();
    piDimsArray = pGT->getDimsArray();

    if (pGT->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    if (in.size() == 2)
    {
        if (in[1]->isString())
        {
            std::wstring wcsOrientation = in[1]->getAs<types::String>()->get(0);
            if (wcsOrientation == L"r")
            {
                iOrientation = 1;
            }
            else if (wcsOrientation == L"c")
            {
                iOrientation = 2;
            }
            else if (wcsOrientation == L"m")
            {
                // old function was "mtlsel"
                for (int i = 0; i < iDims; i++)
                {
                    if (piDimsArray[i] > 1)
                    {
                        iOrientation = i + 1;
                        break;
                    }
                }
            }
            else if (wcsOrientation == L"*")
            {
                iOrientation = 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: r, c, m or * expected.\n"), "max", 2);
                return types::Function::Error;
            }
            iCountElem = 1;
        }
        //        else if(in[1]->isDouble())
        //        {
        //            types::Double* pDbl = in[1]->getAs<types::Double>();
        //            if(pDbl->isScalar() && in[0]->getAs<types::Double>()->isScalar() == false)
        //            {
        //                iOrientation = pDbl->get(0);
        //                iCountElem = 1;
        //            }
        //        }

        inputs = in;
    }

    if (iOrientation > iDims)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: value less than or equal to the number of dimension expected.\n"), "max", 2);
        return types::Function::Error;
    }

    for (int i = 0; i < iCountElem; i++)
    {
        if (inputs[i]->isDouble() || inputs[i]->isInt())
        {
            types::Double* pDbl = NULL;
            if (inputs[i]->isDouble())
            {
                pDbl = inputs[i]->getAs<types::Double>();
                if (pDbl->isComplex())
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "max", i + 1);
                    return types::Function::Error;
                }

                iLargerInput = 1000;
            }
            else if (inputs[i]->isUInt8())
            {
                types::UInt8* pIIn = inputs[i]->getAs<types::UInt8>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iLargerInput = max(iLargerInput, 8);
            }
            else if (inputs[i]->isInt8())
            {
                types::Int8* pIIn = inputs[i]->getAs<types::Int8>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iSigned = 1;
                iLargerInput = max(iLargerInput, 8);
            }
            else if (inputs[i]->isUInt16())
            {
                types::UInt16* pIIn = inputs[i]->getAs<types::UInt16>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iLargerInput = max(iLargerInput, 16);
            }
            else if (inputs[i]->isInt16())
            {
                types::Int16* pIIn = inputs[i]->getAs<types::Int16>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iSigned = 1;
                iLargerInput = max(iLargerInput, 16);
            }
            else if (inputs[i]->isUInt32())
            {
                types::UInt32* pIIn = inputs[i]->getAs<types::UInt32>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iLargerInput = max(iLargerInput, 32);
            }
            else if (inputs[i]->isInt32())
            {
                types::Int32* pIIn = inputs[i]->getAs<types::Int32>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iSigned = 1;
                iLargerInput = max(iLargerInput, 32);
            }
            else if (inputs[i]->isUInt64())
            {
                types::UInt64* pIIn = inputs[i]->getAs<types::UInt64>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iLargerInput = max(iLargerInput, 64);
            }
            else if (inputs[i]->isInt64())
            {
                types::Int64* pIIn = inputs[i]->getAs<types::Int64>();
                pDbl = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
                for (int i = 0; i < pIIn->getSize(); i++)
                {
                    pDbl->set(i, static_cast<double>(pIIn->get(i)));
                }

                iSigned = 1;
                iLargerInput = max(iLargerInput, 64);
            }

            if (pDbl->isScalar())
            {
                vectDouble.push_back(pDbl);
            }
            else if (iDims != pDbl->getDims())
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: All arguments must have the same size.\n"), "max", i + 1);
                return types::Function::Error;
            }
            else
            {
                int* iCurrentDimsArray = pDbl->getDimsArray();
                for (int iterDims = 0; iterDims < iDims; iterDims++)
                {
                    if (iCurrentDimsArray[iterDims] != piDimsArray[iterDims])
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: All arguments must have the same size.\n"), "max", i + 1);
                        return types::Function::Error;
                    }
                }

                vectDouble.push_back(pDbl);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "max", i + 1);
            return types::Function::Error;
        }
    }

    /***** perform operation and set result *****/
    int* iSizes = NULL;
    int iSize   = 2;

    if (iCountElem == 1)
    {
        if (iOrientation)
        {
            iSize = iDims;
            iSizes = new int[iDims];
            memcpy(iSizes, piDimsArray, iDims * sizeof(int));
            iSizes[iOrientation - 1] = 1;
        }
        else
        {
            iSizes = new int[2];
            iSizes[0] = 1;
            iSizes[1] = 1;
        }
    }
    else
    {
        iSize = iDims;
        iSizes = piDimsArray;
    }

    if (_iRetCount == 2)
    {
        if (iCountElem == 1 && iOrientation == 0)
        {
            int iCols = iDims;
            if (vectDouble[0]->getSize() == piDimsArray[iDims - 1]) // vector case
            {
                iCols = 1;
            }

            pDblIndex = new types::Double(1, iCols);
        }
        else
        {
            pDblIndex = new types::Double(iSize, iSizes);
        }
    }

    pDblOut = new types::Double(iSize, iSizes);
    max(vectDouble, iOrientation, pDblIndex, pDblOut);
    // if all inputs are integers return an integer
    switch (iLargerInput)
    {
        case 8 :
        {
            if (iSigned)
            {
                types::Int8* pIIn = new types::Int8(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<char>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            else
            {
                types::UInt8* pIIn = new types::UInt8(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<unsigned char>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            delete pDblOut;
            pDblOut = NULL;
            break;
        }
        case 16 :
        {
            if (iSigned)
            {
                types::Int16* pIIn = new types::Int16(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<short>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            else
            {
                types::UInt16* pIIn = new types::UInt16(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<unsigned short>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            delete pDblOut;
            pDblOut = NULL;
            break;
        }
        case 32 :
        {
            if (iSigned)
            {
                types::Int32* pIIn = new types::Int32(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<int>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            else
            {
                types::UInt32* pIIn = new types::UInt32(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<unsigned int>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            delete pDblOut;
            pDblOut = NULL;
            break;
        }
        case 64 :
        {
            if (iSigned)
            {
                types::Int64* pIIn = new types::Int64(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<long long>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            else
            {
                types::UInt64* pIIn = new types::UInt64(pDblOut->getDims(), pDblOut->getDimsArray());
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pIIn->set(i, static_cast<unsigned long long>(pDblOut->get(i)));
                }
                out.push_back(pIIn);
            }
            delete pDblOut;
            pDblOut = NULL;
            break;
        }
        default :
            out.push_back(pDblOut);
    }

    if (_iRetCount == 2)
    {
        out.push_back(pDblIndex);
    }

    if (iCountElem == 1)
    {
        delete iSizes;
        iSizes = NULL;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
