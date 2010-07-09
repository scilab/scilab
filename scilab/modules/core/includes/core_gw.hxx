/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CORE_GW_HXX__
#define __CORE_GW_HXX__

#include "function.hxx"
#include "internal.hxx"

#include "dynlib_core_gw.h"

class CoreModule
{
private :
    CoreModule(){};
    ~CoreModule(){};
public :
    CORE_GW_IMPEXP static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_getmodules);
CPP_GATEWAY_PROTOTYPE(sci_clear);
CPP_GATEWAY_PROTOTYPE(sci_banner);
CPP_GATEWAY_PROTOTYPE(sci_quit);
CPP_GATEWAY_PROTOTYPE(sci_exit);
CPP_GATEWAY_PROTOTYPE(sci_exists);
CPP_GATEWAY_PROTOTYPE(sci_isdef);
CPP_GATEWAY_PROTOTYPE(sci_argn);
CPP_GATEWAY_PROTOTYPE(sci_global);
CPP_GATEWAY_PROTOTYPE(sci_clearglobal);
CPP_GATEWAY_PROTOTYPE(sci_isglobal);
CPP_GATEWAY_PROTOTYPE(sci_getscilabmode);
CPP_GATEWAY_PROTOTYPE(sci_getos);

#endif /* __CORE_GW_HXX__ */
