/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef GW_SCICOS_HXX_
#define GW_SCICOS_HXX_

#include "cpp_gateway_prototype.hxx"
#include "dynlib_scicos.h"

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_buildouttb, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ctree2, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ctree3, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_ctree4, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_coserror, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_curblock, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_data2sig, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_duplicate, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_end_scicosim, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_haltscicos, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_phase_simulation, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_pointer_xproperty, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicos_debug, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicos_debug_count, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicos_log, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicos_new, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicos_setfield, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicos_time, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_scicosim, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_set_blockerror, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_set_xproperty, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_sig2data, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_validvar, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_var2vec, SCICOS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_vec2var, SCICOS_IMPEXP);

#endif /* GW_SCICOS_HXX_ */
