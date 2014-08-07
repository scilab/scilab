/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "internal.hxx"

namespace types
{
template<> InternalType::ScilabId InternalType::getScilabId<char>()
{
    return InternalType::IdScalarInt8;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned char>()
{
    return InternalType::IdScalarUInt8;
}
template<> InternalType::ScilabId InternalType::getScilabId<short>()
{
    return InternalType::IdScalarInt16;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned short>()
{
    return InternalType::IdScalarUInt16;
}
template<> InternalType::ScilabId InternalType::getScilabId<int>()
{
    return InternalType::IdScalarInt32;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned int>()
{
    return InternalType::IdScalarUInt32;
}
template<> InternalType::ScilabId InternalType::getScilabId<long long>()
{
    return InternalType::IdScalarInt64;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned long long>()
{
    return InternalType::IdScalarUInt64;
}
template<> InternalType::ScilabId InternalType::getScilabId<bool>()
{
    return InternalType::IdScalarBool;
}
template<> InternalType::ScilabId InternalType::getScilabId<double>()
{
    return InternalType::IdScalarDouble;
}
template<> InternalType::ScilabId InternalType::getScilabId<std::complex<double>>()
{
    return InternalType::IdScalarDoubleComplex;
}
template<> InternalType::ScilabId InternalType::getScilabId<char *>()
{
    return InternalType::IdInt8;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned char *>()
{
    return InternalType::IdUInt8;
}
template<> InternalType::ScilabId InternalType::getScilabId<short *>()
{
    return InternalType::IdInt16;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned short *>()
{
    return InternalType::IdUInt16;
}
template<> InternalType::ScilabId InternalType::getScilabId<int *>()
{
    return InternalType::IdInt32;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned int *>()
{
    return InternalType::IdUInt32;
}
template<> InternalType::ScilabId InternalType::getScilabId<long long *>()
{
    return InternalType::IdInt64;
}
template<> InternalType::ScilabId InternalType::getScilabId<unsigned long long *>()
{
    return InternalType::IdUInt64;
}
template<> InternalType::ScilabId InternalType::getScilabId<bool *>()
{
    return InternalType::IdBool;
}
template<> InternalType::ScilabId InternalType::getScilabId<double *>()
{
    return InternalType::IdDouble;
}
template<> InternalType::ScilabId InternalType::getScilabId<std::complex<double> *>()
{
    return InternalType::IdDoubleComplex;
}
}
