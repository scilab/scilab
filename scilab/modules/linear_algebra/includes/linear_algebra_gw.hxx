/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#ifndef __LINEAR_ALGEBRA_GW_HXX__
#define __LINEAR_ALGEBRA_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_linear_algebra_gw.h"
}

class LinearAlgebraModule
{
private :
    LinearAlgebraModule() {};
    ~LinearAlgebraModule() {};
public :
    LINEAR_ALGEBRA_GW_IMPEXP static int Load();
    LINEAR_ALGEBRA_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_chol);
CPP_GATEWAY_PROTOTYPE(sci_bdiag);
CPP_GATEWAY_PROTOTYPE(sci_inv);
CPP_GATEWAY_PROTOTYPE(sci_svd);
CPP_GATEWAY_PROTOTYPE(sci_rcond);
CPP_GATEWAY_PROTOTYPE(sci_qr);
CPP_GATEWAY_PROTOTYPE(sci_lu);
CPP_GATEWAY_PROTOTYPE(sci_lsq);
CPP_GATEWAY_PROTOTYPE(sci_hess);
CPP_GATEWAY_PROTOTYPE(sci_spec);
CPP_GATEWAY_PROTOTYPE(sci_det);
CPP_GATEWAY_PROTOTYPE(sci_balanc);
CPP_GATEWAY_PROTOTYPE(sci_schur);

#endif /* !__LINEAR_ALGEBRA_GW_HXX__ */
