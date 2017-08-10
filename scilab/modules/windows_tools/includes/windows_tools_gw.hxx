/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
