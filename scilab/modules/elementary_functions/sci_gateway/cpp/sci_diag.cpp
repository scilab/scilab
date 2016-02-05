/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - DIGITEO - Cedric DELAMARRE
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "diag.hxx"
#include "int.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*
clear a; nb = 2500; a = rand(nb, nb); tic(); diag(a); toc
clear a; nb = 2500; a = rand(nb, nb); a = a + a *%i; tic(); diag(a); toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_diag(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iStartPos = 0;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "diag", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "diag", 1);
        return types::Function::Error;
    }

    if (in[0]->isGenericType() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_diag";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (in[0]->getAs<types::GenericType>()->getDims() > 2)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_diag";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "diag", 2);
            return types::Function::Error;
        }

        types::Double* pDbl = in[1]->getAs<types::Double>();

        if (pDbl->isScalar() == false || pDbl->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "diag", 2);
            return types::Function::Error;
        }

        iStartPos = static_cast<int>(pDbl->get(0));
    }

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble :
            out.push_back(diag<types::Double, double>(in[0]->getAs<types::Double>(), iStartPos));
            break;
        case types::InternalType::ScilabPolynom :
            out.push_back(diag(in[0]->getAs<types::Polynom>(), iStartPos));
            break;
        case types::InternalType::ScilabString :
            out.push_back(diag(in[0]->getAs<types::String>(), iStartPos));
            break;
        case types::InternalType::ScilabBool :
            out.push_back(diag<types::Bool, int>(in[0]->getAs<types::Bool>(), iStartPos));
            break;
        case types::InternalType::ScilabInt8 :
            out.push_back(diag<types::Int8, char>(in[0]->getAs<types::Int8>(), iStartPos));
            break;
        case types::InternalType::ScilabInt16 :
            out.push_back(diag<types::Int16, short>(in[0]->getAs<types::Int16>(), iStartPos));
            break;
        case types::InternalType::ScilabInt32 :
            out.push_back(diag<types::Int32, int>(in[0]->getAs<types::Int32>(), iStartPos));
            break;
        case types::InternalType::ScilabInt64 :
            out.push_back(diag<types::Int64, long long>(in[0]->getAs<types::Int64>(), iStartPos));
            break;
        case types::InternalType::ScilabUInt8 :
            out.push_back(diag<types::UInt8, unsigned char>(in[0]->getAs<types::UInt8>(), iStartPos));
            break;
        case types::InternalType::ScilabUInt16 :
            out.push_back(diag<types::UInt16, unsigned short>(in[0]->getAs<types::UInt16>(), iStartPos));
            break;
        case types::InternalType::ScilabUInt32 :
            out.push_back(diag<types::UInt32, unsigned int>(in[0]->getAs<types::UInt32>(), iStartPos));
            break;
        case types::InternalType::ScilabUInt64 :
            out.push_back(diag<types::UInt64, unsigned long long>(in[0]->getAs<types::UInt64>(), iStartPos));
            break;
        default :
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_diag";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
