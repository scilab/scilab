/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif

#include "elem_func_gw.hxx"
#include "context.hxx"
#include "overload.hxx"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "sparse.hxx"
#include "int.hxx"

#define MODULE_NAME L"elementary_functions"
extern "C"
{
#include "gw_elementary_functions.h"
}

int ElemFuncModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"abs", &sci_abs, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"acos", &sci_acos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"asin", &sci_asin, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"atan", &sci_atan, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ceil", &sci_ceil, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clean", &sci_clean, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"conj", &sci_conj, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"cos", &sci_cos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"cumprod", &sci_cumprod, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"cumsum", &sci_cumsum, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"diag", &sci_diag, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dsearch", &sci_dsearch, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"exp", &sci_exp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"expm", &sci_expm, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"eye", &sci_eye, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"floor", &sci_floor, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"frexp", &sci_frexp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"gsort", &sci_gsort, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"imag", &sci_imag, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"imult", &sci_imult, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"int", &sci_int, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isreal", &sci_isreal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isequal", &sci_isequal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"kron", &sci_kron, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"log", &sci_log, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"log1p", &sci_log1p, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"matrix", &sci_matrix, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"max", &sci_max, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"min", &sci_min, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"nearfloat", &sci_nearfloat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"number_properties", &sci_number_properties, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ones", &sci_ones, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"prod", &sci_prod, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rand", &sci_rand, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"real", &sci_real, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"round", &sci_round, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sign", &sci_sign, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sin", &sci_sin, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"size", &sci_size, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sqrt", &sci_sqrt, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sum", &sci_sum, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tan", &sci_tan, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"testmatrix", &sci_testmatrix, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tril", &sci_tril, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"triu", &sci_triu, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"zeros", &sci_zeros, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rat", &sci_rat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"base2dec", &sci_base2dec, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dec2base", &sci_dec2base, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"log10", &sci_log10, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"cosh", &sci_cosh, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sinh", &sci_sinh, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tanh", &sci_tanh, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"acosh", &sci_acosh, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"asinh", &sci_asinh, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"atanh", &sci_atanh, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isvector", &sci_isvector, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"issquare", &sci_issquare, MODULE_NAME));
    return 1;
}


bool getDimsFromArguments(types::typed_list& in, const std::string& _pstName, int* _iDims, int** _piDims, bool* _alloc)
{
    types::Double* pOut = 0;

    *_alloc = false;
    *_iDims = 0;
    *_piDims = NULL;

    if (in.size() == 0)
    {
        *_iDims = 2;
        *_piDims = new int[*_iDims];
        (*_piDims)[0] = 1;
        (*_piDims)[1] = 1;
        *_alloc = true;
        return true;
    }
    else if (in.size() == 1)
    {
        *_iDims = 1;
        // :
        if (in[0]->isColon())
        {
            *_iDims = -1;
            return false;
        }

        if (in[0]->isArrayOf() == false)
        {
            if (in[0]->isSparse())
            {
                types::Sparse* sp = in[0]->getAs<types::Sparse>();
                *_iDims = sp->getDims();
                *_piDims = sp->getDimsArray();
                return true;
            }
            else if (in[0]->isSparseBool())
            {
                types::SparseBool* sp = in[0]->getAs<types::SparseBool>();
                *_iDims = sp->getDims();
                *_piDims = sp->getDimsArray();
                return true;
            }
            return false;
        }

        types::GenericType* pIn = in[0]->getAs<types::GenericType>();
        *_iDims = pIn->getDims();
        *_piDims = pIn->getDimsArray();

        return true;
    }
    else
    {
        *_iDims = static_cast<int>(in.size());
        *_piDims = new int[*_iDims];
        *_alloc = true;
        for (int i = 0; i < *_iDims; i++)
        {
            if (in[i]->isArrayOf() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _pstName.c_str(), i + 1);
                delete[] * _piDims;
                return false;
            }

            types::GenericType* pGTIn = in[i]->getAs<types::GenericType>();
            if (pGTIn->isScalar() == false || pGTIn->isComplex())
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), _pstName.c_str(), i + 1);
                delete[] * _piDims;
                return false;
            }

            switch (in[i]->getType())
            {
                case types::InternalType::ScilabDouble:
                {
                    double dValue = in[i]->getAs<types::Double>()->get(0);
                    if (dValue >= INT_MAX)
                    {
                        Scierror(999, _("%s: variable size exceeded : less than %d expected.\n"), _pstName.c_str(), INT_MAX);
                        delete[] * _piDims;
                        return false;
                    }
                    (*_piDims)[i] = static_cast<int>(dValue);
                }
                break;
                case types::InternalType::ScilabInt8:
                    (*_piDims)[i] = static_cast<int>(in[i]->getAs<types::Int8>()->get()[0]);
                    break;
                case types::InternalType::ScilabUInt8:
                    (*_piDims)[i] = static_cast<int>(in[i]->getAs<types::UInt8>()->get()[0]);
                    break;
                case types::InternalType::ScilabInt16:
                    (*_piDims)[i] = static_cast<int>(in[i]->getAs<types::Int16>()->get()[0]);
                    break;
                case types::InternalType::ScilabUInt16:
                    (*_piDims)[i] = static_cast<int>(in[i]->getAs<types::UInt16>()->get()[0]);
                    break;
                case types::InternalType::ScilabInt32:
                    (*_piDims)[i] = in[i]->getAs<types::Int32>()->get()[0];
                    break;
                case types::InternalType::ScilabUInt32:
                    (*_piDims)[i] = static_cast<int>(in[i]->getAs<types::UInt32>()->get()[0]);
                    break;
                case types::InternalType::ScilabInt64:
                {
                    long long llValue = in[i]->getAs<types::Int64>()->get(0);
                    if (llValue >= INT_MAX)
                    {
                        Scierror(999, _("%s: variable size exceeded : less than %d expected.\n"), _pstName.c_str(), INT_MAX);
                        delete[] * _piDims;
                        return false;
                    }
                    (*_piDims)[i] = static_cast<int>(llValue);
                    break;
                }
                case types::InternalType::ScilabUInt64:
                {
                    unsigned long long ullValue = in[i]->getAs<types::UInt64>()->get(0);
                    if (ullValue >= INT_MAX)
                    {
                        Scierror(999, _("%s: variable size exceeded : less than %d expected.\n"), _pstName.c_str(), INT_MAX);
                        delete[] * _piDims;
                        return false;
                    }
                    (*_piDims)[i] = static_cast<int>(ullValue);
                    break;
                }
                default:
                    Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), _pstName.c_str(), i + 1);
                    delete[] * _piDims;
                    return false;
            }
        }

        return true;
    }

    return false;
}

types::Double* trigo(types::Double* in, func_real func_r, func_complex func_c, bool forceComplex)
{
    bool isComplex = in->isComplex() || forceComplex;
    types::Double* out = new types::Double(in->getDims(), in->getDimsArray(), isComplex);

    int size = in->getSize();
    double* pInR = in->get();
    double* pOutR = out->get();

    if (isComplex)
    {
        double* pInI = in->getImg();
        double* pOutI = out->getImg();
        std::complex<double> d;
        for (int i = 0; i < size; ++i)
        {
            d.real(pInR[i]);
            d.imag(pInI[i]);
            std::complex<double> res = func_c(d);
            pOutR[i] = res.real();
            pOutI[i] = res.imag();
        }
    }
    else
    {
        for (int i = 0; i < size; ++i)
        {
            pOutR[i] = func_r(pInR[i]);
        }
    }

    return out;
}
