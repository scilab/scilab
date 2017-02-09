/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2017 - Scilab Enterprises - Antoine ELIAS
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
#include <string>
#include "string_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "overload.hxx"
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

static const std::string fname("emptystr");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_emptystr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* ret = nullptr;
    int iRhs = static_cast<int>(in.size());

    // check output parameters
    if (_iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    switch (iRhs)
    {
        case 1:
        {
            if (in[0]->isGenericType())
            {
                types::GenericType* pGT = in[0]->getAs<types::GenericType>();
                if (pGT->getSize() == 0)
                {
                    out.push_back(types::Double::Empty());
                    return types::Function::OK;
                }

                ret = new types::String(pGT->getDims(), pGT->getDimsArray());
            }
            else if (in[0]->isList())
            {
                types::List* pL = in[0]->getAs<types::List>();
                ret = new types::String(pL->getSize(), 1);
            }
            else
            {
                return Overload::generateNameAndCall(L"emptystr", in, _iRetCount, out);
            }

            break;
        }
        case 2:
        {
            if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"), fname.data(), 1);
                return types::Function::Error;
            }

            if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"), fname.data(), 2);
                return types::Function::Error;
            }

            int row = static_cast<int>(in[0]->getAs<types::Double>()->get()[0]);
            int col = static_cast<int>(in[1]->getAs<types::Double>()->get()[0]);

            if (row == 0 || col == 0)
            {
                out.push_back(types::Double::Empty());
                return types::Function::OK;
            }

            ret = new types::String(row, col);
            break;
        }
        default:
        {
            ret = new types::String(1, 1);
        }
    }

    int size = ret->getSize();
    wchar_t** strs = ret->get();
    for (int i = 0; i < size; ++i)
    {
        strs[i] = os_wcsdup(L"");
    }

    out.push_back(ret);
    return types::Function::OK;

}
