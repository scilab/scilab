/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "cpp_gateway_prototype.hxx"
#include "dynlib_core_gw.h"

class CoreModule
{
private :
    CoreModule() {};
    ~CoreModule() {};
public :
    CORE_GW_IMPEXP static int Load();
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
CPP_GATEWAY_PROTOTYPE(sci_with_module);
CPP_GATEWAY_PROTOTYPE(sci_lasterror);
CPP_GATEWAY_PROTOTYPE(sci_getversion);
CPP_GATEWAY_PROTOTYPE(sci_warning);
CPP_GATEWAY_PROTOTYPE(sci_getmd5);
CPP_GATEWAY_PROTOTYPE(sci_error);
CPP_GATEWAY_PROTOTYPE(sci_pause);
CPP_GATEWAY_PROTOTYPE(sci_abort);
CPP_GATEWAY_PROTOTYPE(sci_who);
CPP_GATEWAY_PROTOTYPE(sci_stacksize);
CPP_GATEWAY_PROTOTYPE(sci_sciargs);
CPP_GATEWAY_PROTOTYPE(sci_format);
CPP_GATEWAY_PROTOTYPE(sci_ieee);
CPP_GATEWAY_PROTOTYPE(sci_funcprot);
CPP_GATEWAY_PROTOTYPE(sci_mode);
CPP_GATEWAY_PROTOTYPE(sci_typename);
CPP_GATEWAY_PROTOTYPE(sci_where);

#endif /* __CORE_GW_HXX__ */
