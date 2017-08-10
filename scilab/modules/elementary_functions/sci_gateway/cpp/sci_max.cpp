/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#include <algorithm>

#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "overload.hxx"
#include "max.hxx"
#include "min.hxx"
#include "int.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();max(a);toc
clear a;nb = 2500;a = rand(nb, nb);b = rand(nb, nb);tic();max(a,b);toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_MinMax(types::typed_list &in, int _iRetCount, types::typed_list &out, const char* fname)
{
    int iCountElem   = static_cast<int>(in.size());
    int iDims        = 0;
    int* piDimsArray = NULL;
    int iOrientation = 0;
    int iLargerInput = 0; // used to know the larger type of inputs arguments (in interger case). max(int16, int32, int8) return an int32
    int iSigned      = 0; // if one of input arguments is signed the result is signed.

    types::Double* pDblIndex    = NULL;
    types::Double* pDblOut      = NULL;

    std::vector<types::InternalType*> inputs;
    std::vector<types::Double*> vectDouble;

    void (*pFuncMinMax)(std::vector<types::Double*> vectIn, int iOrientation, types::Double * pDblIndex, types::Double * pOut);

    if (in.size() < 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), fname, 1, 2);
        return types::Function::Error;
    }

    if (strcmp(fname, "max") == 0)
    {
        pFuncMinMax = &(max);
    }
    else
    {
        pFuncMinMax = &(min);
    }

    /***** get data *****/
    if (in.size() == 1 && in[0]->isList())
    {
        types::List* pList = in[0]->getAs<types::List>();
        iCountElem = pList->getSize();

        if (iCountElem == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Non empty list expected.\n"), fname, 1);
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
        wchar_t* wcsMinMax = to_wide_string(fname);
        std::wstring wstFuncName = L"%" + inputs[0]->getShortTypeStr() + L"_" + wcsMinMax;
        FREE(wcsMinMax);
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::GenericType* pGT = NULL;
    int iNbEmptyMatrix = 0;
    int iPosFirstEmpty = 0;
    bool bFindFirstMatrix = false;

    // get the size of the first input non scalar
    for (int i = 0; i < inputs.size(); i++)
    {
        pGT = inputs[i]->getAs<types::GenericType>();
        if (pGT->getSize() == 0)
        {
            if (iNbEmptyMatrix == 0)
            {
                iPosFirstEmpty = i + 1;
            }
            iNbEmptyMatrix++;
        }
        else if (bFindFirstMatrix == false && pGT->isScalar() == false)
        {
            bFindFirstMatrix = true;
            iDims       = pGT->getDims();
            piDimsArray = pGT->getDimsArray();
        }
    }

    // all inputs are scalar
    if (iDims == 0)
    {
        iDims       = pGT->getDims();
        piDimsArray = pGT->getDimsArray();
    }

    if (inputs.size() == 2 && inputs[1]->isString())
    {
        std::wstring wcsOrientation = inputs[1]->getAs<types::String>()->get(0);
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
            Scierror(999, _("%s: Wrong value for input argument #%d: r, c, m or * expected.\n"), fname, 2);
            return types::Function::Error;
        }
        iCountElem = 1;
    }

    if (iNbEmptyMatrix == iCountElem)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }
    else if (iPosFirstEmpty)
    {
        Scierror(45, _("%s: null matrix (argument # %d).\n"), fname, iPosFirstEmpty);
        return types::Function::Error;
    }

    if (iOrientation > iDims)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: value less than or equal to the number of dimension expected.\n"), fname, 2);
        return types::Function::Error;
    }

    std::vector<bool> cloned(iCountElem, true);
    for (int i = 0; i < iCountElem; i++)
    {
        types::Double* pDbl = NULL;
        switch (inputs[i]->getType())
        {
            case types::InternalType::ScilabDouble:
            {
                pDbl = inputs[i]->getAs<types::Double>();
                if (pDbl->isComplex())
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, i + 1);
                    return types::Function::Error;
                }

                iLargerInput = 1000;
                cloned[i] = false;
                break;
            }
            case types::InternalType::ScilabInt8:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::Int8>());
                iLargerInput = std::max(iLargerInput, 8);
                iSigned = 1;
                break;
            }
            case types::InternalType::ScilabInt16:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::Int16>());
                iLargerInput = std::max(iLargerInput, 16);
                iSigned = 1;
                break;
            }
            case types::InternalType::ScilabInt32:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::Int32>());
                iLargerInput = std::max(iLargerInput, 32);
                iSigned = 1;
                break;
            }
            case types::InternalType::ScilabInt64:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::Int64>());
                iLargerInput = std::max(iLargerInput, 64);
                iSigned = 1;
                break;
            }
            case types::InternalType::ScilabUInt8:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::UInt8>());
                iLargerInput = std::max(iLargerInput, 8);
                break;
            }
            case types::InternalType::ScilabUInt16:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::UInt16>());
                iLargerInput = std::max(iLargerInput, 16);
                break;
            }
            case types::InternalType::ScilabUInt32:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::UInt32>());
                iLargerInput = std::max(iLargerInput, 32);
                break;
            }
            case types::InternalType::ScilabUInt64:
            {
                pDbl = getAsDouble(inputs[i]->getAs<types::UInt64>());
                iLargerInput = std::max(iLargerInput, 64);
                break;
            }
            default:
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), fname, i + 1);
                return types::Function::Error;
            }
        }

        if (pDbl->isScalar())
        {
            vectDouble.push_back(pDbl);
        }
        else if (iDims != pDbl->getDims())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: All arguments must have the same size.\n"), fname, i + 1);
            return types::Function::Error;
        }
        else
        {
            int* iCurrentDimsArray = pDbl->getDimsArray();
            for (int iterDims = 0; iterDims < iDims; iterDims++)
            {
                if (iCurrentDimsArray[iterDims] != piDimsArray[iterDims])
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: All arguments must have the same size.\n"), fname, i + 1);
                    return types::Function::Error;
                }
            }

            vectDouble.push_back(pDbl);
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

            // vector case
            if (vectDouble[0]->getSize() == piDimsArray[iDims - 1] ||
                    vectDouble[0]->getSize() == piDimsArray[0])
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
    pFuncMinMax(vectDouble, iOrientation, pDblIndex, pDblOut);
    // if all inputs are integers return an integer
    switch (iLargerInput)
    {
        case 8 :
        {
            if (iSigned)
            {
                out.push_back(toInt<types::Int8>(pDblOut));
            }
            else
            {
                out.push_back(toInt<types::UInt8>(pDblOut));
            }
            break;
        }
        case 16 :
        {
            if (iSigned)
            {
                out.push_back(toInt<types::Int16>(pDblOut));
            }
            else
            {
                out.push_back(toInt<types::UInt16>(pDblOut));
            }
            break;
        }
        case 32 :
        {
            if (iSigned)
            {
                out.push_back(toInt<types::Int32>(pDblOut));
            }
            else
            {
                out.push_back(toInt<types::UInt32>(pDblOut));
            }
            break;
        }
        case 64 :
        {
            if (iSigned)
            {
                out.push_back(toInt<types::Int64>(pDblOut));
            }
            else
            {
                out.push_back(toInt<types::UInt64>(pDblOut));
            }
            break;
        }
        default :
            out.push_back(pDblOut);
    }

    for (int i = 0; i < iCountElem; ++i)
    {
        if (cloned[i])
        {
            vectDouble[i]->killMe();
        }
    }
    if (iLargerInput != 1000)
    {
        //do not delete for double output
        delete pDblOut;
        pDblOut = NULL;
    }

    if (_iRetCount == 2)
    {
        out.push_back(pDblIndex);
    }

    if (iCountElem == 1)
    {
        delete[] iSizes;
        iSizes = NULL;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_min(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return sci_MinMax(in, _iRetCount, out, "min");
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_max(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return sci_MinMax(in, _iRetCount, out, "max");
}
/*--------------------------------------------------------------------------*/
