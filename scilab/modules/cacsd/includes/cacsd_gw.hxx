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
