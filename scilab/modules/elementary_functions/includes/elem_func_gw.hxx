/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __ELEM_FUNC_GW_HXX__
#define __ELEM_FUNC_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "double.hxx"
#include "complex"

extern "C"
{
#include "dynlib_elementary_functions_gw.h"
}

class ElemFuncModule
{
private :
    ElemFuncModule() {};
    ~ElemFuncModule() {};
public :
    EXTERN_EF_GW static int Load();
    EXTERN_EF_GW static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_isequal);
CPP_GATEWAY_PROTOTYPE(sci_size);
CPP_GATEWAY_PROTOTYPE(sci_eye);
CPP_GATEWAY_PROTOTYPE(sci_zeros);
CPP_GATEWAY_PROTOTYPE(sci_ones);
CPP_GATEWAY_PROTOTYPE(sci_rand);
CPP_GATEWAY_PROTOTYPE(sci_prod);
CPP_GATEWAY_PROTOTYPE(sci_sum);
CPP_GATEWAY_PROTOTYPE(sci_matrix);
CPP_GATEWAY_PROTOTYPE(sci_abs);
CPP_GATEWAY_PROTOTYPE(sci_acos);
CPP_GATEWAY_PROTOTYPE(sci_asin);
CPP_GATEWAY_PROTOTYPE(sci_atan);
CPP_GATEWAY_PROTOTYPE(sci_ceil);
CPP_GATEWAY_PROTOTYPE(sci_clean);
CPP_GATEWAY_PROTOTYPE(sci_conj);
CPP_GATEWAY_PROTOTYPE(sci_cos);
CPP_GATEWAY_PROTOTYPE(sci_sin);
CPP_GATEWAY_PROTOTYPE(sci_tan);
CPP_GATEWAY_PROTOTYPE(sci_cumprod);
CPP_GATEWAY_PROTOTYPE(sci_cumsum);
CPP_GATEWAY_PROTOTYPE(sci_diag);
CPP_GATEWAY_PROTOTYPE(sci_dsearch);
CPP_GATEWAY_PROTOTYPE(sci_exp);
CPP_GATEWAY_PROTOTYPE(sci_expm);
CPP_GATEWAY_PROTOTYPE(sci_floor);
CPP_GATEWAY_PROTOTYPE(sci_frexp);
CPP_GATEWAY_PROTOTYPE(sci_gsort);
CPP_GATEWAY_PROTOTYPE(sci_imag);
CPP_GATEWAY_PROTOTYPE(sci_imult);
CPP_GATEWAY_PROTOTYPE(sci_int);
CPP_GATEWAY_PROTOTYPE(sci_isreal);
CPP_GATEWAY_PROTOTYPE(sci_real);
CPP_GATEWAY_PROTOTYPE(sci_kron);
CPP_GATEWAY_PROTOTYPE(sci_log);
CPP_GATEWAY_PROTOTYPE(sci_log1p);
CPP_GATEWAY_PROTOTYPE(sci_max); // Old name sci_maxi
CPP_GATEWAY_PROTOTYPE(sci_min); // Old name sci_mini
CPP_GATEWAY_PROTOTYPE(sci_rat);
CPP_GATEWAY_PROTOTYPE(sci_triu);
CPP_GATEWAY_PROTOTYPE(sci_tril);
CPP_GATEWAY_PROTOTYPE(sci_round);
CPP_GATEWAY_PROTOTYPE(sci_sign);
CPP_GATEWAY_PROTOTYPE(sci_cosh);
CPP_GATEWAY_PROTOTYPE(sci_sinh);
CPP_GATEWAY_PROTOTYPE(sci_tanh);
CPP_GATEWAY_PROTOTYPE(sci_acosh);
CPP_GATEWAY_PROTOTYPE(sci_asinh);
CPP_GATEWAY_PROTOTYPE(sci_atanh);
CPP_GATEWAY_PROTOTYPE(sci_sqrt);

bool getDimsFromArguments(types::typed_list& in, const std::string& _pstName, int* _iDims, int** _piDims, bool* _alloc);

template <class T>
types::Double* getAsDouble(T* _val)
{
    types::Double* dbl = new types::Double(_val->getDims(), _val->getDimsArray());
    double* pOut = dbl->get();
    typename T::type* pIn = _val->get();
    int size = dbl->getSize();
    for (int i = 0; i < size; i++)
    {
        pOut[i] = static_cast<double>(pIn[i]);
    }

    return dbl;
}

template <class T>
T* toInt(types::Double* _dbl)
{
    T* pI = new T(_dbl->getDims(), _dbl->getDimsArray());
    typename T::type* p = pI->get();
    double* pdbl = _dbl->get();
    int size = _dbl->getSize();
    for (int i = 0; i < size; i++)
    {
        p[i] = static_cast<typename T::type>(pdbl[i]);
    }

    return pI;
}

typedef double(*func_real)(double);
typedef std::complex<double>(*func_complex)(const std::complex<double>&);

types::Double* trigo(types::Double* in, func_real func_r, func_complex func_c, bool forceComplex = false);

#endif /* __ELEM_FUNC_GW_HXX__ */
