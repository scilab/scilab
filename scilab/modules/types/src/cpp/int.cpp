/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "int.hxx"

namespace types
{
// Specializations
template<>
std::wstring Int<char>::getTypeStr()
{
    return L"int8";
}

template<>
std::wstring Int<short>::getTypeStr()
{
    return L"int16";
}

template<>
std::wstring Int<int>::getTypeStr()
{
    return L"int32";
}

template<>
std::wstring Int<long long>::getTypeStr()
{
    return L"int64";
}

template<>
std::wstring Int<unsigned char>::getTypeStr()
{
    return L"uint8";
}

template<>
std::wstring Int<unsigned short>::getTypeStr()
{
    return L"uint16";
}

template<>
std::wstring Int<unsigned int>::getTypeStr()
{
    return L"uint32";
}

template<>
std::wstring Int<unsigned long long>::getTypeStr()
{
    return L"uint64";
}

template<>
void Int<char>::whoAmI()
{
    std::cout << "types::Int8";
}

template<>
void Int<short>::whoAmI()
{
    std::cout << "types::Int16";
}

template<>
void Int<int>::whoAmI()
{
    std::cout << "types::Int32";
}

template<>
void Int<long long>::whoAmI()
{
    std::cout << "types::Int64";
}

template<>
void Int<unsigned char>::whoAmI()
{
    std::cout << "types::UInt8";
}

template<>
void Int<unsigned short>::whoAmI()
{
    std::cout << "types::UInt16";
}

template<>
void Int<unsigned int>::whoAmI()
{
    std::cout << "types::UInt32";
}

template<>
void Int<unsigned long long>::whoAmI()
{
    std::cout << "types::UInt64";
}


#ifdef _MSC_VER
template TYPES_IMPEXP class ArrayOf<char>;
template TYPES_IMPEXP class ArrayOf<unsigned char>;
template TYPES_IMPEXP class ArrayOf<short>;
template TYPES_IMPEXP class ArrayOf<unsigned short>;
template TYPES_IMPEXP class ArrayOf<unsigned int>;
template TYPES_IMPEXP class ArrayOf<unsigned long long>;
#endif
}
