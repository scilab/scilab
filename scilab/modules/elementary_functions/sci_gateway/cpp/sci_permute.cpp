/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2018- Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <numeric>
#include "double.hxx"
#include "int.hxx"
#include "polynom.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "function.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h" 
}

void computeOffsets(int iDims, const int* piDimsArray, const std::vector<int>& dimsVect, int* piOffset, int* piMaxOffset)
{
    for (int i = 0; i < iDims; ++i)
    {
        int iOffset = i > 0 ? iOffset * piDimsArray[dimsVect[i - 1] - 1] : 1;
        int j = dimsVect[i] - 1;
        piOffset[j] = iOffset;
        piMaxOffset[j] = iOffset * piDimsArray[j];
    }
}

template <typename T>
T *doNativePermute(T *pIn, const std::vector<int>& dimsVect)
{
    int iDims = pIn->getDims();
    int* piDimsArray = pIn->getDimsArray();
    int* piIndex = new int[iDims]();
    int* piOffset = new int[iDims];
    int* piMaxOffset = new int[iDims];

    computeOffsets(iDims, piDimsArray, dimsVect, piOffset, piMaxOffset);

    T* pOut = pIn->clone();
    typename T::type* pout = pOut->get();

    if (pIn->isComplex())
    {
        typename T::type* poutImg = pOut->getImg();
        for (typename T::type *pin = pIn->get(), *pinImg = pIn->getImg(); pin < pIn->get() + pIn->getSize(); pin++, pinImg++)
        {
            *pout = *pin;
            *poutImg = *pinImg;
            for (int j = 0; j < iDims; j++)
            {
                ++piIndex[j];
                pout += piOffset[j];
                poutImg += piOffset[j];
                if (piIndex[j] < piDimsArray[j])
                {
                    break;
                }

                pout -= piMaxOffset[j];
                poutImg -= piMaxOffset[j];
                piIndex[j] = 0;
            }
        }
    }
    else
    {
        for (typename T::type *pin = pIn->get(); pin < pIn->get() + pIn->getSize(); pin++)
        {
            *pout = *pin;
            for (int j = 0; j < iDims; j++)
            {
                ++piIndex[j];
                pout += piOffset[j];
                if (piIndex[j] < piDimsArray[j])
                {
                    break;
                }

                pout -= piMaxOffset[j];
                piIndex[j] = 0;
            }
        }
    }

    delete[] piIndex;
    delete[] piOffset;
    delete[] piMaxOffset;

    return pOut;
}

template <typename T>
T* doPermute(T* pIn, const std::vector<int>& dimsVect)
{
    int iDims = pIn->getDims();
    int* piDimsArray = pIn->getDimsArray();
    int* piOffset = new int[iDims];
    int* piMaxOffset = new int[iDims];
    int* piIndex = new int[iDims]();

    computeOffsets(iDims, piDimsArray, dimsVect, piOffset, piMaxOffset);

    T* pOut = pIn->clone();

    for (int iSource = 0, iDest = 0; iSource < pIn->getSize(); iSource++)
    {
        pOut->set(iDest, pIn->get(iSource));
        for (int j = 0; j < iDims; j++)
        {
            ++piIndex[j];
            iDest += piOffset[j];
            if (piIndex[j] < piDimsArray[j])
            {
                break;
            }

            iDest -= piMaxOffset[j];
            piIndex[j] = 0;
        }
    }

    delete[] piIndex;
    delete[] piOffset;
    delete[] piMaxOffset;

    return pOut;
}

types::Function::ReturnValue sci_permute(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "permute", 2);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "permute", 1);
        return types::Function::Error;
    }

    if (in[0]->isArrayOf() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_permute";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::GenericType* pIn = in[0]->getAs<types::GenericType>();
    types::GenericType* pDims = in[1]->getAs<types::GenericType>();

    int iDims = pIn->getDims();
    int* piDimsArray = pIn->getDimsArray();
    int iNewDims = pDims->getSize();
    int* piNewDimsArray = NULL;
    std::vector<int> dimsVect;

    if ((iNewDims >= iDims) & pDims->isDouble() & !pDims->getAs<types::Double>()->isComplex())
    {
        // Check if 2nd argument is a permutation of [1..iNewDims]
        types::Double* pDbl = pDims->getAs<types::Double>();
        std::vector<double> sortedNewDimsVect(pDbl->get(), pDbl->get() + iNewDims);
        std::sort(sortedNewDimsVect.begin(), sortedNewDimsVect.end());
        std::vector<double> rangeVect(iNewDims);
        std::iota(rangeVect.begin(), rangeVect.end(), 1.0);

        if (sortedNewDimsVect == rangeVect)
        {
            piNewDimsArray = new int[iNewDims];
            for (int i = 0; i < iNewDims; i++)
            {
                int j = (int)pDbl->get(i);
                piNewDimsArray[i] = 1;
                if (j <= iDims)
                {
                    piNewDimsArray[i] = piDimsArray[j - 1];
                    dimsVect.push_back(j);
                }
            }
        }
    }

    if (dimsVect.empty())
    {
        delete[] piNewDimsArray;
        Scierror(78, _("%s: Wrong value for input argument #%d: Must be a valid permutation of [1..n>%d] integers.\n"), "permute", 2, iDims - 1);
        return types::Function::Error;
    }

    types::GenericType *pOut;

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            pOut = doNativePermute(in[0]->getAs<types::Double>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabUInt64:
        {
            pOut = doNativePermute(in[0]->getAs<types::UInt64>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabInt64:
        {
            pOut = doNativePermute(in[0]->getAs<types::Int64>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabUInt32:
        {
            pOut = doNativePermute(in[0]->getAs<types::UInt32>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            pOut = doNativePermute(in[0]->getAs<types::Int32>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabUInt16:
        {
            pOut = doNativePermute(in[0]->getAs<types::UInt16>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            pOut = doNativePermute(in[0]->getAs<types::Int16>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabUInt8:
        {
            pOut = doNativePermute(in[0]->getAs<types::UInt8>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            pOut = doNativePermute(in[0]->getAs<types::Int8>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabBool:
        {
            pOut = doNativePermute(in[0]->getAs<types::Bool>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabString:
        {
            pOut = doPermute(in[0]->getAs<types::String>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabPolynom:
        {
            pOut = doPermute(in[0]->getAs<types::Polynom>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabStruct:
        {
            pOut = doPermute(in[0]->getAs<types::Struct>(), dimsVect);
            break;
        }
        case types::InternalType::ScilabCell:
        {
            pOut = doPermute(in[0]->getAs<types::Cell>(), dimsVect);
            break;
        }
        default:
        {
            delete[] piNewDimsArray;
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_permute";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    pOut->reshape(piNewDimsArray, iNewDims);

    delete[] piNewDimsArray;

    out.push_back(pOut);

    return types::Function::OK;
}
