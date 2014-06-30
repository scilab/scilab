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

class DifferentialEquationsModule
{
private :
    DifferentialEquationsModule() {};
    ~DifferentialEquationsModule() {};

public :
    DIFFERENTIAL_EQUATIONS_GW_IMPEXP static int Load();
    DIFFERENTIAL_EQUATIONS_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

// equation 1
CPP_GATEWAY_PROTOTYPE(sci_ode);

// equation 2
CPP_GATEWAY_PROTOTYPE(sci_impl);    // Old function name was sciimpl
CPP_GATEWAY_PROTOTYPE(sci_dassl);   // Old function name dassli
CPP_GATEWAY_PROTOTYPE(sci_dasrt);   // Old function name was dasrti
CPP_GATEWAY_PROTOTYPE(sci_daskr);

// equation 3
CPP_GATEWAY_PROTOTYPE(sci_odedc);   // Old function name was sci_odc

// equation 4
CPP_GATEWAY_PROTOTYPE(sci_intg);
CPP_GATEWAY_PROTOTYPE(sci_int2d);
CPP_GATEWAY_PROTOTYPE(sci_int3d);
CPP_GATEWAY_PROTOTYPE(sci_feval);

// equation 6
CPP_GATEWAY_PROTOTYPE(sci_bvode);   // Old function name was scibvode

#endif /* !__DIFFERENTIAL_EQUATIONS_GW_HXX__ */
