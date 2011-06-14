/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    DYNAMIC_LINK_GW_IMPEXP static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_getdynlibext);
CPP_GATEWAY_PROTOTYPE(sci_link);
CPP_GATEWAY_PROTOTYPE(sci_ilib_verbose);
CPP_GATEWAY_PROTOTYPE(sci_c_link);
CPP_GATEWAY_PROTOTYPE(sci_ulink);

#endif /* !__DYNAMIC_LINK_GW_HXX__ */
