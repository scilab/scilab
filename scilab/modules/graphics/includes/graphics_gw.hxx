/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __GRAPHICS_GW_HXX__
#define __GRAPHICS_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_graphics_gw.h"

class GraphicsModule
{
private :
    GraphicsModule() {};
    ~GraphicsModule() {};


public :
    GRAPHICS_GW_IMPEXP static int LoadDeps(const std::wstring& _functionName);
    GRAPHICS_GW_IMPEXP static int Load();
    GRAPHICS_GW_IMPEXP static int Unload()
    {
        return 1;
    }

    static bool loadedDep;
};


CPP_GATEWAY_PROTOTYPE_EXPORT(sci_get, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_xget, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_xset, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_winsid, GRAPHICS_GW_IMPEXP);
CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_matplot, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_matplot1, GRAPHICS_GW_IMPEXP);
#endif /* !__GUI_GW_HXX__ */
