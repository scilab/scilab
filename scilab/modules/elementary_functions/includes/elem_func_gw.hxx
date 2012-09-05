/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __ELEM_FUNC_GW_HXX__
#define __ELEM_FUNC_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_elementary_functions_gw.h"

class ElemFuncModule
{
private :
    ElemFuncModule() {};
    ~ElemFuncModule() {};
public :
    EXTERN_EF_GW static int Load();
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
CPP_GATEWAY_PROTOTYPE(sci_kron);
CPP_GATEWAY_PROTOTYPE(sci_log);
CPP_GATEWAY_PROTOTYPE(sci_log1p);
CPP_GATEWAY_PROTOTYPE(sci_max); // Old name sci_maxi
CPP_GATEWAY_PROTOTYPE(sci_min); // Old name sci_mini
CPP_GATEWAY_PROTOTYPE(sci_rat);

#endif /* __ELEM_FUNC_GW_HXX__ */
