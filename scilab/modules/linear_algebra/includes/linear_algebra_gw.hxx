/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
