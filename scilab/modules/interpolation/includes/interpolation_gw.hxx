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

#ifndef __INTERPOLATION_GW_HXX__
#define __INTERPOLATION_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_interpolation.h"
}

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_splin,             INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_lsq_splin,         INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_interp,            INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_linear_interpn,    INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_splin2d,           INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_interp2d,          INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_cshep2d,           INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_eval_cshep2d,      INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_splin3d,           INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_interp3d,          INTERPOLATION_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_bsplin3val,        INTERPOLATION_IMPEXP);

#endif /* !__INTERPOLATION_GW_HXX__ */
