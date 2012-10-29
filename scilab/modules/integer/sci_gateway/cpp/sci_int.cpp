/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */
/*--------------------------------------------------------------------------*/

#include "int.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "function.hxx"
#include "integer_gw.hxx"

extern "C"
{
#include "Scierror.h"
}

template <class T, class U>
void convert_int(U* _pIn, int _iSize, T* _pOut)
{
    for (int i = 0 ; i < _iSize ; i++)
    {
        _pOut[i] = (T)_pIn[i];
    }
}

template <class T>
void convertInt(types::InternalType* _pIn, T* _pOut)
{
    switch (_pIn->getType())
    {
        case types::InternalType::RealBool :
        {
            types::Bool* pBool = _pIn->getAs<types::Bool>();
            convert_int(pBool->get(), pBool->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealDouble :
        {
            types::Double* pD = _pIn->getAs<types::Double>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealInt8 :
        {
            types::Int8* pD = _pIn->getAs<types::Int8>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealUInt8 :
        {
            types::UInt8* pD = _pIn->getAs<types::UInt8>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealInt16 :
        {
            types::Int16* pD = _pIn->getAs<types::Int16>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealUInt16 :
        {
            types::UInt16* pD = _pIn->getAs<types::UInt16>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealInt32 :
        {
            types::Int32* pD = _pIn->getAs<types::Int32>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealUInt32 :
        {
            types::UInt32* pD = _pIn->getAs<types::UInt32>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealInt64 :
        {
            types::Int64* pD = _pIn->getAs<types::Int64>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::RealUInt64 :
        {
            types::UInt64* pD = _pIn->getAs<types::UInt64>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
    }
}

template< class T>
types::Callable::ReturnValue commonInt(types::typed_list &in, int _iRetCount, types::typed_list &out, std::wstring _wstName)
{
    if (in.size() != 1)
    {
        ScierrorW(77, _W("%s: Wrong number of input argument(s): %d expected.\n"), _wstName.c_str(), 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false && in[0]->isInt() == false && in[0]->isBool() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: %ls, %ls or %ls expected.\n"), _wstName.c_str(), 1, L"integer", L"boolean", L"double");
        return types::Function::Error;
    }

    types::GenericType* pGT = in[0]->getAs<types::GenericType>();
    if (pGT->getDims() == 2 && pGT->getRows() == 0 && pGT->getCols() == 2)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    T* pOut = new T(pGT->getDims(), pGT->getDimsArray());

    convertInt(in[0], pOut);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
types::Callable::ReturnValue sci_integer8(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int8>(in, _iRetCount, out, L"int8");
}

types::Callable::ReturnValue sci_uinteger8(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt8>(in, _iRetCount, out, L"uint8");
}

types::Callable::ReturnValue sci_integer16(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int16>(in, _iRetCount, out, L"int16");
}

types::Callable::ReturnValue sci_uinteger16(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt16>(in, _iRetCount, out, L"uint16");
}

types::Callable::ReturnValue sci_integer32(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int32>(in, _iRetCount, out, L"int32");
}

types::Callable::ReturnValue sci_uinteger32(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt32>(in, _iRetCount, out, L"uint32");
}

types::Callable::ReturnValue sci_integer64(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int64>(in, _iRetCount, out, L"int64");
}

types::Callable::ReturnValue sci_uinteger64(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt64>(in, _iRetCount, out, L"uint64");
}
/*--------------------------------------------------------------------------*/
