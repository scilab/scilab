/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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

#include <algorithm>

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"
#include "overload.hxx"
#include "int.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

template<typename T>
size_t nonZeros(T *pIT)
{
    size_t iRes = 0;
    int iSize = pIT->getSize();

    if (pIT->isComplex())
    {
        for (int i = 0; i < iSize; ++i)
        {
            if (pIT->get(i) == 0 && pIT->getImg(i) == 0)
            {
                ++iRes;
            }
        }
    }
    else
    {
        iRes = std::count(pIT->get(), pIT->get() + iSize, 0);
    }

    return iSize - iRes;
}


types::Function::ReturnValue sci_nnz(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "nnz", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "nnz", 1);
        return types::Function::Error;
    }

    double dblVal = 0;

    switch (in[0]->getType())
    {
    case types::InternalType::ScilabSparse:
        dblVal = static_cast<double>(in[0]->getAs<types::Sparse>()->nonZeros());
        break;
    case types::InternalType::ScilabSparseBool:
        dblVal = static_cast<double>(in[0]->getAs<types::SparseBool>()->nbTrue());
        break;
    case types::InternalType::ScilabDouble:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::Double>()));
        break;
    case types::InternalType::ScilabBool:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::Bool>()));
        break;
    case types::InternalType::ScilabInt8:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::Int8>()));
        break;
    case types::InternalType::ScilabUInt8:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::UInt8>()));
        break;
    case types::InternalType::ScilabInt16:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::Int16>()));
        break;
    case types::InternalType::ScilabUInt16:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::UInt16>()));
        break;
    case types::InternalType::ScilabInt32:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::Int32>()));
        break;
    case types::InternalType::ScilabUInt32:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::UInt32>()));
        break;
    case types::InternalType::ScilabInt64:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::Int64>()));
        break;
    case types::InternalType::ScilabUInt64:
        dblVal = static_cast<double>(nonZeros(in[0]->getAs<types::UInt64>()));
        break;
    default:
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_nnz";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    out.push_back(new types::Double(dblVal));
    return types::Function::OK;
}

