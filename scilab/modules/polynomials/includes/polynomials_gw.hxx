/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
    EXTERN_POLYNOMIALS_GW static int Unload()
    {
        return 1;
    }
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
