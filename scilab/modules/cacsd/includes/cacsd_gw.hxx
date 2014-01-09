/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CACSD_GW_HXX__
#define __CACSD_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_cacsd.h"
}

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_arl2_ius, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ldiv, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_tzer, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_residu, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ppol, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_freq, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ltitr, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_rtitr, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ereduc, CACSD_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_fstair, CACSD_IMPEXP);

#endif /* __CACSD_GW_HXX__ */
