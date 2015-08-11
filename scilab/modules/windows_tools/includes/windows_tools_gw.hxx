/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __WINDOWS_TOOLS_GW_HXX__
#define __WINDOWS_TOOLS_GW_HXX__


#include "dynlib_windows_tools.h"
#include "cpp_gateway_prototype.hxx"

#include "context.hxx"


class WindowsToolsModule
{
private :
    WindowsToolsModule() {};
    ~WindowsToolsModule() {};

public :
    WINDOWS_TOOLS_IMPEXP static int Load();
    WINDOWS_TOOLS_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_consolebox);
CPP_GATEWAY_PROTOTYPE(sci_createGUID);
CPP_GATEWAY_PROTOTYPE(sci_win64);
CPP_GATEWAY_PROTOTYPE(sci_winqueryreg);
CPP_GATEWAY_PROTOTYPE(sci_winopen);
CPP_GATEWAY_PROTOTYPE(sci_findfileassociation);
CPP_GATEWAY_PROTOTYPE(sci_mcisendstring);
CPP_GATEWAY_PROTOTYPE(sci_istssession);
CPP_GATEWAY_PROTOTYPE(sci_getsystemmetrics);
#endif /* !__WINDOWS_TOOLS_GW_HXX__ */
