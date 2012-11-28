/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __POLYNOMIALS_GW_HXX__
#define __POLYNOMIALS_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_polynomials_gw.h"

class PolynomialsModule
{
private :
    PolynomialsModule() {};
    ~PolynomialsModule() {};
public :
    EXTERN_POLYNOMIALS_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_poly);
CPP_GATEWAY_PROTOTYPE(sci_varn);
CPP_GATEWAY_PROTOTYPE(sci_roots);
CPP_GATEWAY_PROTOTYPE(sci_coeff);
CPP_GATEWAY_PROTOTYPE(sci_degree);
CPP_GATEWAY_PROTOTYPE(sci_pppdiv);
CPP_GATEWAY_PROTOTYPE(sci_simpMode);
CPP_GATEWAY_PROTOTYPE(sci_simp);
CPP_GATEWAY_PROTOTYPE(sci_sfact);
CPP_GATEWAY_PROTOTYPE(sci_bezout);

#endif /* __POLYNOMIALS_GW_HXX__ */
