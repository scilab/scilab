/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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

#ifndef __CACSD_GW_HXX__
#define __CACSD_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_cacsd.h"
#include "gw_cacsd.h"
}

class CacsdModule
{
private:
    CacsdModule() {};
    ~CacsdModule() {};

public:
    CACSD_IMPEXP static int Load();
    CACSD_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_arl2_ius);
CPP_GATEWAY_PROTOTYPE(sci_ldiv);
CPP_GATEWAY_PROTOTYPE(sci_tzer);
CPP_GATEWAY_PROTOTYPE(sci_residu);
CPP_GATEWAY_PROTOTYPE(sci_ppol);
CPP_GATEWAY_PROTOTYPE(sci_freq);
CPP_GATEWAY_PROTOTYPE(sci_ltitr);
CPP_GATEWAY_PROTOTYPE(sci_rtitr);
CPP_GATEWAY_PROTOTYPE(sci_ereduc);
CPP_GATEWAY_PROTOTYPE(sci_fstair);

#endif /* __CACSD_GW_HXX__ */
