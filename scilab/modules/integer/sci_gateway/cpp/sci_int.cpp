/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */
/*--------------------------------------------------------------------------*/
#include <limits>

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
    static T minval = std::numeric_limits<T>::min();
    static T maxval = std::numeric_limits<T>::max();

    for (int i = 0 ; i < _iSize ; i++)
    {
        if (std::isnan((double)_pIn[i]))
        {
            _pOut[i] = 0;
        }
        else if (std::isinf((double)_pIn[i]))
        {
            if ((double)_pIn[i] > 0)
            {
                _pOut[i] = maxval;
            }
            else
            {
                _pOut[i] = minval;
            }
        }
        else
        {
            _pOut[i] = (T)_pIn[i];
        }
    }
}

template <class T>
void convertInt(types::InternalType* _pIn, T* _pOut)
{
    switch (_pIn->getType())
    {
        case types::InternalType::ScilabBool :
        {
            types::Bool* pBool = _pIn->getAs<types::Bool>();
            convert_int(pBool->get(), pBool->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabDouble :
        {
            types::Double* pD = _pIn->getAs<types::Double>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabInt8 :
        {
            types::Int8* pD = _pIn->getAs<types::Int8>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabUInt8 :
        {
            types::UInt8* pD = _pIn->getAs<types::UInt8>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabInt16 :
        {
            types::Int16* pD = _pIn->getAs<types::Int16>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabUInt16 :
        {
            types::UInt16* pD = _pIn->getAs<types::UInt16>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabInt32 :
        {
            types::Int32* pD = _pIn->getAs<types::Int32>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabUInt32 :
        {
            types::UInt32* pD = _pIn->getAs<types::UInt32>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabInt64 :
        {
            types::Int64* pD = _pIn->getAs<types::Int64>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        case types::InternalType::ScilabUInt64 :
        {
            types::UInt64* pD = _pIn->getAs<types::UInt64>();
            convert_int(pD->get(), pD->getSize(), _pOut->get());
            break;
        }
        default:
            return;
    }
}

template< class T>
types::Callable::ReturnValue commonInt(types::typed_list &in, int _iRetCount, types::typed_list &out, std::string _stName)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), _stName.c_str(), 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false && in[0]->isInt() == false && in[0]->isBool() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s, %s or %s expected.\n"), _stName.c_str(), 1, "integer", "boolean", "double");
        return types::Function::Error;
    }

    types::GenericType* pGT = in[0]->getAs<types::GenericType>();
    if (pGT->getDims() == 2 && pGT->getRows() == 0 && pGT->getCols() == 0)
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
    return commonInt<types::Int8>(in, _iRetCount, out, "int8");
}

types::Callable::ReturnValue sci_uinteger8(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt8>(in, _iRetCount, out, "uint8");
}

types::Callable::ReturnValue sci_integer16(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int16>(in, _iRetCount, out, "int16");
}

types::Callable::ReturnValue sci_uinteger16(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt16>(in, _iRetCount, out, "uint16");
}

types::Callable::ReturnValue sci_integer32(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int32>(in, _iRetCount, out, "int32");
}

types::Callable::ReturnValue sci_uinteger32(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt32>(in, _iRetCount, out, "uint32");
}

types::Callable::ReturnValue sci_integer64(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::Int64>(in, _iRetCount, out, "int64");
}

types::Callable::ReturnValue sci_uinteger64(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return commonInt<types::UInt64>(in, _iRetCount, out, "uint64");
}
/*--------------------------------------------------------------------------*/
