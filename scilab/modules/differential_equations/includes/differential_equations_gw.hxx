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

#ifndef __DIFFERENTIAL_EQUATIONS_GW_HXX__
#define __DIFFERENTIAL_EQUATIONS_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_differential_equations_gw.h"
}

// equation 1
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ode, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);

// equation 2
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_impl, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);    // Old function name was sciimpl
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_dassl, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);   // Old function name dassli
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_dasrt, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);   // Old function name was dasrti
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_daskr, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);

// equation 3
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_odedc, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);   // Old function name was sci_odc

// equation 4
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_intg, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_int2d, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_int3d, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_feval, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);

// equation 6
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_bvode, DIFFERENTIAL_EQUATIONS_GW_IMPEXP);   // Old function name was scibvode

#endif /* !__DIFFERENTIAL_EQUATIONS_GW_HXX__ */
