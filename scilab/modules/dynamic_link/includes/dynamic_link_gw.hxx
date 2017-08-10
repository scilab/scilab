/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __DYNAMIC_LINK_GW_HXX__
#define __DYNAMIC_LINK_GW_HXX__

#include "dynlib_dynamic_link_gw.h"

#include "cpp_gateway_prototype.hxx"

class DynamicLinkModule
{
private:
    DynamicLinkModule() {};
    ~DynamicLinkModule() {};

public:
    DYNAMIC_LINK_GW_IMPEXP static int Load();
    DYNAMIC_LINK_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_getdynlibext);
CPP_GATEWAY_PROTOTYPE(sci_link);
CPP_GATEWAY_PROTOTYPE(sci_ilib_verbose);
CPP_GATEWAY_PROTOTYPE(sci_c_link);
CPP_GATEWAY_PROTOTYPE(sci_ulink);
CPP_GATEWAY_PROTOTYPE(sci_addinter);

#endif /* !__DYNAMIC_LINK_GW_HXX__ */
