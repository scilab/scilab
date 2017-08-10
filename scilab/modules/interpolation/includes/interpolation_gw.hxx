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
