/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OPTIMIZATION_GW_HXX__
#define __OPTIMIZATION_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_optimization.h"
}

CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_optim, OPTIMIZATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_fsolve, OPTIMIZATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_lsqrsolve, OPTIMIZATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_semidef, OPTIMIZATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_readmps, OPTIMIZATION_IMPEXP);

#endif /* !__OPTIMIZATION_GW_HXX__ */
