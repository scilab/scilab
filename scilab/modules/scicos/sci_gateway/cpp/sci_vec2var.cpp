/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>
#include <cwchar>
#include <cstring>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

static const std::string funname = "vec2var";

/**
 * Calculate the length increment depending on the type of the Scilab type
 *
 * @param T Scilab type
 * @param v the instance on the Scilab type
 * @return the number of double used to store the data
 */
template<typename T>
size_t required_length(T* v)
{
    const size_t sizeof_double = sizeof(double);
    if (sizeof(typename T::type) >= sizeof_double)
    {
        return v->getSize() * sizeof(typename T::type) / sizeof_double;
    }
    else
    {
        // Increase the size to contain enough space, manage the size_t rounding issue
        return (v->getSize() * sizeof(typename T::type) + (sizeof_double - 1)) / sizeof_double;
    }
}

static void Error(const int index)
{
    Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), funname.c_str(), 1, index, 1);
}

template<typename T>
int encode(const double* const tab, const int tabSize, const int iDims, const int offset, T* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }

    res = new T(iDims, pDims);
    delete[] pDims;

    int numberOfDoubleNeeded = required_length(res);
    if (tabSize < numberOfDoubleNeeded + 2 + iDims)
    {
        // Error case: the input doesn't have enough elements
        Error(numberOfDoubleNeeded + 2 + iDims + offset);
        res->killMe();
        return -1;
    }

    //Use a buffer to prevent copying only parts of integers
    double* buffer = new double[numberOfDoubleNeeded];
    memcpy(buffer, tab + iDims, numberOfDoubleNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(typename T::type));
    delete[] buffer;

    return 2 + iDims + numberOfDoubleNeeded;
}

template<>
int encode(const double* const tab, const int tabSize, const int iDims, const int offset, types::Double* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    if (tabSize < iElements + 3 + iDims)
    {
        // Error case: the input doesn't have enough elements
        Error(iElements + 3 + iDims + offset);
        return -1;
    }
    if (iElements == 0)
    {
        res = types::Double::Empty();
        delete[] pDims;
        return 5;
    }
    bool isComplex = (tab[iDims] == 1);

    res = new types::Double(iDims, pDims, isComplex);
    delete[] pDims;

    res->set(tab + iDims + 1);
    if (isComplex == true)
    {
        res->setImg(tab + iDims + 1 + iElements);
    }

    return 2 + iDims + 1 + iElements;
}

template<>
int encode(const double* const tab, const int tabSize, const int iDims, const int offset, types::String* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    if (tabSize < iElements * 2 + 2 + iDims)
    {
        // Error case: the input doesn't have enough elements
        Error(iElements * 2 + 2 + iDims + offset);
        return -1;
    }

    res = new types::String(iDims, pDims);
    delete[] pDims;

    // Retrieving the first value pointer
    double* strData = const_cast<double*>(tab + iDims + iElements);

    res->set(0, (char*) & (*strData));
    strData += static_cast<size_t>(tab[iDims]);
    int stringOffset = static_cast<size_t>(tab[iDims]);
    for (int i = 1; i < iElements; i++)
    {
        res->set(i, (char*) & (*strData));

        // Increment the value pointer by the number of elements
        const size_t numberOfElem = static_cast<size_t>(tab[iDims + i]) - static_cast<size_t>(tab[iDims + i - 1]);
        strData += numberOfElem;
        stringOffset += static_cast<int>(numberOfElem);
    }

    return 2 + iDims + iElements + stringOffset;
}

static bool readElement(const double* const input, int iType, int iDims, int inputRows, int &offset, types::InternalType* &res)
{
    switch (iType)
    {
        case types::InternalType::ScilabDouble :
        {
            if (inputRows < 5)
            {
                Error(offset + 5);
                return false;
            }

            types::Double* pDouble = nullptr;
            int resSize = encode<types::Double>(input + 2, inputRows, iDims, offset, pDouble);
            if (resSize < 0)
            {
                return false;
            }
            res = pDouble;
            offset += resSize;
            break;
        }

        case types::InternalType::ScilabInt8   :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::Int8* pInt8 = nullptr;
            int resSize = encode<types::Int8>(input + 2, inputRows, iDims, offset, pInt8);
            if (resSize < 0)
            {
                return false;
            }
            res = pInt8;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabUInt8  :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::UInt8* pUInt8 = nullptr;
            int resSize = encode<types::UInt8>(input + 2, inputRows, iDims, offset, pUInt8);
            if (resSize < 0)
            {
                return false;
            }
            res = pUInt8;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabInt16  :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::Int16* pInt16 = nullptr;
            int resSize = encode<types::Int16>(input + 2, inputRows, iDims, offset, pInt16);
            if (resSize < 0)
            {
                return false;
            }
            res = pInt16;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabUInt16 :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::UInt16* pUInt16 = nullptr;
            int resSize = encode<types::UInt16>(input + 2, inputRows, iDims, offset, pUInt16);
            if (resSize < 0)
            {
                return false;
            }
            res = pUInt16;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabInt32  :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::Int32* pInt32 = nullptr;
            int resSize = encode<types::Int32>(input + 2, inputRows, iDims, offset, pInt32);
            if (resSize < 0)
            {
                return false;
            }
            res = pInt32;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabUInt32 :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::UInt32* pUInt32 = nullptr;
            int resSize = encode<types::UInt32>(input + 2, inputRows, iDims, offset, pUInt32);
            if (resSize < 0)
            {
                return false;
            }
            res = pUInt32;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabInt64  :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::Int64* pInt64 = nullptr;
            int resSize = encode<types::Int64>(input + 2, inputRows, iDims, offset, pInt64);
            if (resSize < 0)
            {
                return false;
            }
            res = pInt64;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabUInt64 :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::UInt64* pUInt64 = nullptr;
            int resSize = encode<types::UInt64>(input + 2, inputRows, iDims, offset, pUInt64);
            if (resSize < 0)
            {
                return false;
            }
            res = pUInt64;
            offset += resSize;
            break;
        }
        case types::InternalType::ScilabBool   :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::Bool* pBool = nullptr;
            int resSize = encode<types::Bool>(input + 2, inputRows, iDims, offset, pBool);
            if (resSize < 0)
            {
                return false;
            }
            res = pBool;
            offset += resSize;
            break;
        }

        case types::InternalType::ScilabString :
        {
            if (inputRows < 4)
            {
                Error(offset + 4);
                return false;
            }

            types::String* pString = nullptr;
            int resSize = encode<types::String>(input + 2, inputRows, iDims, offset, pString);
            if (resSize < 0)
            {
                return false;
            }
            res = pString;
            offset += resSize;
            break;
        }

        case types::InternalType::ScilabList   :
        case types::InternalType::ScilabTList  :
        case types::InternalType::ScilabMList  :
        {
            if (inputRows < 2)
            {
                Error(offset + 2);
                return false;
            }

            types::List* pList = nullptr;
            if (iType == types::InternalType::ScilabList)
            {
                pList = new types::List();
            }
            else if (iType == types::InternalType::ScilabTList)
            {
                pList = new types::TList();
            }
            else
            {
                pList = new types::MList();
            }

            offset += 2;
            for (int i = 0; i < iDims; ++i)
            {
                // Extract the list elements infos and recursively call readElement
                int elementType = *(input + offset);
                int elementDims = *(input + offset + 1);
                types::InternalType* element;
                if (!readElement(input + offset, elementType, elementDims, inputRows - offset, offset, element))
                {
                    return false;
                }
                pList->append(element);
            }
            res = pList;
            break;
        }

        default :
            Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), funname.c_str(), offset + 1, 1);
            return false;
    }

    return true;
}

types::Function::ReturnValue sci_vec2var(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }

    if (!in[0]->isDouble())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real row vector expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }
    types::Double* input = in[0]->getAs<types::Double>();

    if (input->getCols() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }
    if (input->getRows() < 2)
    {
        Error(2);
        return types::Function::Error;
    }

    int iType = input->get(0);
    int iDims = input->get(1);
    int inputRows = input->getRows();

    types::InternalType* res;
    int offset = 0;
    if (!readElement(input->get(), iType, iDims, inputRows, offset, res))
    {
        return types::Function::Error;
    }

    out.push_back(res);
    return types::Function::OK;
}
