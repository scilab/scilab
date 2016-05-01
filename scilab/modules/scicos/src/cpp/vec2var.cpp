/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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

#include <vector>
#include <string>
#include <cwchar>
#include <cstring>

#include "vec2var.hxx"

#include "types.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
//#include "struct.hxx"

extern "C"
{
#include "api_scilab.h"

#include "Scierror.h"
#include "localization.h"
}

static const std::string vec2varName = "vec2var";

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

template<typename T>
int decode(const double* const tab, const int tabSize, const int iDims, const int offset, T* &res)
{
    if (iDims < 1)
    {
        Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: Integer matrix cannot be empty.\n"), vec2varName.c_str(), offset + 3, 1);
        return -1;
    }

    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = static_cast<int>(tab[i]);
        iElements *= pDims[i];
    }

    res = new T(iDims, pDims);
    delete[] pDims;

    const int numberOfDoubleNeeded = static_cast<int>(required_length(res));
    if (tabSize < numberOfDoubleNeeded + 2 + iDims)
    {
        // Error case: the input doesn't have enough elements
        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, numberOfDoubleNeeded + 2 + iDims + offset, 1);
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
int decode(const double* const tab, const int tabSize, const int iDims, const int offset, types::Double* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = static_cast<int>(tab[i]);
        iElements *= pDims[i];
    }
    if (tabSize < iElements + 3 + iDims)
    {
        // Error case: the input doesn't have enough elements
        delete[] pDims;
        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, iElements + 3 + iDims + offset, 1);
        return -1;
    }
    if (iElements == 0)
    {
        res = types::Double::Empty();
        delete[] pDims;
        return 5;
    }
    const bool isComplex = (tab[iDims] == 1);

    res = new types::Double(iDims, pDims, isComplex);
    delete[] pDims;

    res->set(tab + iDims + 1);
    if (isComplex == true)
    {
        res->setImg(tab + iDims + 1 + iElements);
        iElements *= 2;
    }

    return 2 + iDims + 1 + iElements;
}

template<>
int decode(const double* const tab, const int tabSize, const int iDims, const int offset, types::String* &res)
{
    if (iDims < 1)
    {
        Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: String matrix cannot be empty.\n"), vec2varName.c_str(), offset + 2, 1);
        return -1;
    }

    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = static_cast<int>(tab[i]);
        iElements *= pDims[i];
    }
    if (tabSize < iElements * 2 + 2 + iDims)
    {
        // Error case: the input doesn't have enough elements
        delete[] pDims;
        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, iElements * 2 + 2 + iDims + offset, 1);
        return -1;
    }

    res = new types::String(iDims, pDims);
    delete[] pDims;

    // Retrieving the first value pointer
    double* strData = const_cast<double*>(tab + iDims + iElements);

    res->set(0, (char*) & (*strData));
    strData += static_cast<size_t>(tab[iDims]);
    int stringOffset = static_cast<int>(tab[iDims]);
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

static bool readElement(const double* const input, const int iType, const int iDims, const int inputRows, int &offset, types::InternalType* &res)
{
    switch (iType)
    {
        case sci_matrix  :
        {
            if (inputRows < 5)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                return false;
            }

            types::Double* pDouble = nullptr;
            const int resSize = decode<types::Double>(input + 2, inputRows, iDims, offset, pDouble);
            if (resSize < 0)
            {
                return false;
            }
            res = pDouble;
            offset += resSize;
            break;
        }

        case sci_ints    :
        {
            switch (static_cast<int>(input[1]))
            {
                case sci_int8   :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::Int8* pInt8 = nullptr;
                    const int resSize = decode<types::Int8>(input + 3, inputRows, iDims, offset, pInt8);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pInt8;
                    offset += 1 + resSize;
                    break;
                }
                case sci_uint8  :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::UInt8* pUInt8 = nullptr;
                    const int resSize = decode<types::UInt8>(input + 3, inputRows, iDims, offset, pUInt8);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pUInt8;
                    offset += 1 + resSize;
                    break;
                }
                case sci_int16  :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::Int16* pInt16 = nullptr;
                    const int resSize = decode<types::Int16>(input + 3, inputRows, iDims, offset, pInt16);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pInt16;
                    offset += 1 + resSize;
                    break;
                }
                case sci_uint16 :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::UInt16* pUInt16 = nullptr;
                    const int resSize = decode<types::UInt16>(input + 3, inputRows, iDims, offset, pUInt16);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pUInt16;
                    offset += 1 + resSize;
                    break;
                }
                case sci_int32  :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::Int32* pInt32 = nullptr;
                    const int resSize = decode<types::Int32>(input + 3, inputRows, iDims, offset, pInt32);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pInt32;
                    offset += 1 + resSize;
                    break;
                }
                case sci_uint32 :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::UInt32* pUInt32 = nullptr;
                    const int resSize = decode<types::UInt32>(input + 3, inputRows, iDims, offset, pUInt32);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pUInt32;
                    offset += 1 + resSize;
                    break;
                }
                case sci_int64 :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::Int64* pInt64 = nullptr;
                    const int resSize = decode<types::Int64>(input + 3, inputRows, iDims, offset, pInt64);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pInt64;
                    offset += 1 + resSize;
                    break;
                }
                case sci_uint64 :
                {
                    if (inputRows < 5)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 5, 1);
                        return false;
                    }

                    types::UInt64* pUInt64 = nullptr;
                    const int resSize = decode<types::UInt64>(input + 3, inputRows, iDims, offset, pUInt64);
                    if (resSize < 0)
                    {
                        return false;
                    }
                    res = pUInt64;
                    offset += 1 + resSize;
                    break;
                }
            }
            break;
        }
        case sci_boolean :
        {
            if (inputRows < 4)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 4, 1);
                return false;
            }

            types::Bool* pBool = nullptr;
            const int resSize = decode<types::Bool>(input + 2, inputRows, iDims, offset, pBool);
            if (resSize < 0)
            {
                return false;
            }
            res = pBool;
            offset += resSize;
            break;
        }

        case sci_strings :
        {
            if (inputRows < 4)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 4, 1);
                return false;
            }

            types::String* pString = nullptr;
            const int resSize = decode<types::String>(input + 2, inputRows, iDims, offset, pString);
            if (resSize < 0)
            {
                return false;
            }
            res = pString;
            offset += resSize;
            break;
        }

        case sci_list    :
        case sci_tlist   :
        case sci_mlist   :
        {
            if (inputRows < 2)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 2, 1);
                return false;
            }

            types::List* pList = nullptr;
            if (iType == sci_list)
            {
                pList = new types::List();
            }
            else if (iType == sci_tlist)
            {
                pList = new types::TList();
            }
            else
            {
                pList = new types::MList();
            }

            int offsetSave = 0;
            if (offset == 0)
            {
                offset += 2;
            }
            else
            {
                // If reading a sublist, start off with a new offset
                offsetSave = offset;
                offset = 2;
            }

            for (int i = 0; i < iDims; ++i)
            {
                if (inputRows < 2 + offset)
                {
                    delete pList;
                    Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 2, 1);
                    return false;
                }
                // Extract the list elements infos and recursively call readElement
                int elementType = static_cast<int>(*(input + offset));
                int elementDims;
                if (elementType == sci_ints)
                {
                    // Integers store their precision in *(input + offset + 1)
                    elementDims = static_cast<int>(*(input + offset + 2));
                }
                else
                {
                    elementDims = static_cast<int>(*(input + offset + 1));
                }
                types::InternalType* element;
                if (!readElement(input + offset, elementType, elementDims, inputRows - offset, offset, element))
                {
                    delete pList;
                    return false;
                }

                pList->append(element);
                if (element->isList())
                {
                    element->killMe();
                }
            }
            offset += offsetSave;
            res = pList;
            break;
        }

        // Structs are not used yet
        //case types::InternalType::ScilabStruct :
        //{
        //    if (inputRows < 2)
        //    {
        //        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 2, 1);
        //        return false;
        //    }

        //    if (iDims <= 0)
        //    {
        //        res = new types::Struct();
        //        offset += 2;
        //        break;
        //    }

        //    int offsetSave = 0;
        //    if (offset == 0)
        //    {
        //        offset += 2;
        //    }
        //    else
        //    {
        //        // If reading a sublist, start off with a new offset
        //        offsetSave = offset;
        //        offset = 2;
        //    }
        //    // Read the header...
        //    int elementType = static_cast<int>(*(input + offset));
        //    if (elementType != types::InternalType::ScilabString)
        //    {
        //        Scierror(999, _("%s: Wrong value for input argument #%d: %d (String) expected.\n"), vec2varName.c_str(), 1, 11);
        //        return false;
        //    }
        //    int elementDims = static_cast<int>(*(input + offset + 1));
        //    types::InternalType* element;
        //    if (!readElement(input + offset, elementType, elementDims, inputRows - offset, offset, element))
        //    {
        //        return false;
        //    }

        //    types::Struct* pStruct = new types::Struct(1, 1);
        //    types::String* header = element->getAs<types::String>();
        //    // ... and copy it in 'pStruct'
        //    for (int i = 0; i < header->getSize(); ++i)
        //    {
        //        pStruct->get(0)->addField(header->get(i));
        //    }

        //    for (int i = 1; i < iDims + 1; ++i)
        //    {
        //        if (inputRows < 2 + offset)
        //        {
        //            delete pStruct;
        //            Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, offset + 2, 1);
        //            return false;
        //        }
        //        // Extract the fields content infos and recursively call readElement
        //        elementType = static_cast<int>(*(input + offset));
        //        elementDims = static_cast<int>(*(input + offset + 1));
        //        if (!readElement(input + offset, elementType, elementDims, inputRows - offset, offset, element))
        //        {
        //            delete pStruct;
        //            return false;
        //        }
        //        pStruct->get(0)->set(header->get(i - 1), element);
        //    }

        //    header->killMe();
        //    offset += offsetSave;
        //    res = pStruct;
        //    break;
        //}

        default :
            Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), vec2varName.c_str(), offset + 1, 1);
            return false;
    }

    return true;
}

bool vec2var(const std::vector<double> in, types::InternalType* &out)
{
    const int iType = static_cast<int>(in[0]);
    int iDims;
    if (iType == sci_ints)
    {
        // Integers store their precision in in[1]
        iDims = static_cast<int>(in[2]);
    }
    else
    {
        iDims = static_cast<int>(in[1]);
    }

    int offset = 0;
    if (!readElement(&in[0], iType, iDims, static_cast<int>(in.size()), offset, out))
    {
        return false;
    }

    return true;
}
