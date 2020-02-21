/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2019 - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
/*--------------------------------------------------------------------------*/

#include <algorithm>
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "int.hxx"
#include "string.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
}

template <typename T> types::String* dectobase(T *_pIn, int iParam[]);

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_dec2base(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iParam[2] = { 0, 0 };
    types::String* pOut = NULL;

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dec2base", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(77, _("%s: Wrong number of output argument(s): %d expected.\n"), "dec2base", 1);
        return types::Function::Error;
    }

    for (int i = 1; i < in.size(); i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A positive integer expected.\n"), "dec2base", i + 1);
            return types::Function::Error;
        }
        else
        {
            types::Double *pDbl = in[i]->getAs<types::Double>();
            if (pDbl->isComplex() == true || pDbl->isScalar() == false ||
                pDbl->get(0) != trunc(pDbl->get(0)) || pDbl->get(0) < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive integer expected.\n"), "dec2base", i + 1);
                return types::Function::Error;
            }

            iParam[i - 1] = (int)pDbl->get(0);
        }
    }

    if (iParam[0] < 2 || iParam[0] > 36)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d."), "dec2base", 2, 2, 36);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();

        if (pDblIn->isEmpty())
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        if (pDblIn->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "dec2base", 1);
            return types::Function::Error;
        }

        double max_value = pow(2, 53);
        double* dbl_in = pDblIn->get();
        for (int i = 0; i < pDblIn->getSize(); i++)
        {
            if (trunc(dbl_in[i]) != dbl_in[i])
            {
                Scierror(999, _("%s: Wrong value(s) for input argument #%d: A matrix of positive integer values expected.\n"), "dec2base", 1);
                return types::Function::Error;
            }

            if (dbl_in[i] > max_value)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be between 0 and 2^53.\n"), "dec2base", 1);
                return types::Function::Error;
            }
        }

        pOut = dectobase(pDblIn, iParam);
    }
    else if (in[0]->isInt())
    {
        switch (in[0]->getType())
        {
        case types::InternalType::ScilabInt8:
            pOut = dectobase(in[0]->getAs<types::Int8>(), iParam);
            break;
        case types::InternalType::ScilabUInt8:
            pOut = dectobase(in[0]->getAs<types::UInt8>(), iParam);
            break;
        case types::InternalType::ScilabInt16:
            pOut = dectobase(in[0]->getAs<types::Int16>(), iParam);
            break;
        case types::InternalType::ScilabUInt16:
            pOut = dectobase(in[0]->getAs<types::UInt16>(), iParam);
            break;
        case types::InternalType::ScilabInt32:
            pOut = dectobase(in[0]->getAs<types::Int32>(), iParam);
            break;
        case types::InternalType::ScilabUInt32:
            pOut = dectobase(in[0]->getAs<types::UInt32>(), iParam);
            break;
        case types::InternalType::ScilabInt64:
            pOut = dectobase(in[0]->getAs<types::Int64>(), iParam);
            break;
        case types::InternalType::ScilabUInt64:
            pOut = dectobase(in[0]->getAs<types::UInt64>(), iParam);
            break;
        default: // should not happen
            break;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real or integer matrix expected.\n"), "dec2base", 2);
    }

    if (pOut == NULL)
    {
        return types::Function::Error;
    }

    out.push_back(pOut);
    return types::Function::OK;
}

template <typename T> types::String* dectobase(T *_pIn, int iParam[])
{
    int iBaseUsed = iParam[0];
    int iNbDigits = iParam[1];
    unsigned long long int n;
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    types::String* _pOut = new types::String(_pIn->getDims(), _pIn->getDimsArray());

    if (iBaseUsed == 2)
    {
        // for binary conversion default iNbDigits is the largest needed number of bits among matrix terms
        unsigned long long iMax;
        iMax = (unsigned long long) *(std::max_element(_pIn->get(), _pIn->get() + _pIn->getSize()));
        int iDepth = 0;
        // iDepth is equal to trunc(log2(iMax))+1 i.e. number of needed bits
        while (iMax)
        {
            iMax >>= 1;
            iDepth++;
        }

        iNbDigits = iNbDigits > iDepth ? iNbDigits : iDepth;
    }

    for (int i = 0; i < _pIn->getSize(); i++)
    {
        if (_pIn->get(i) < 0)
        {
            Scierror(999, _("%s: Wrong value(s) for input argument #%d: A matrix of positive integer values expected.\n"), "dec2base", 1);
            _pOut->killMe();
            return NULL;
        }

        std::string s;
        s.reserve(65);
        n = (unsigned long long int) _pIn->get(i);

        // digits are obtained in reverse order
        do
        {
            s.append(1, digits[n % iBaseUsed]); // store and get next digit
        } while ((n /= iBaseUsed) > 0); // "delete" it

        s.append(std::max(iNbDigits - (int)s.length(), 0), '0');
        reverse(s.begin(), s.end());
        _pOut->set(i, s.data());
    }
    return _pOut;
}

/*--------------------------------------------------------------------------*/
