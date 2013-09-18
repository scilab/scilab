/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    GRAPHICS_GW_IMPEXP static void LoadDeps(void);
    GRAPHICS_GW_IMPEXP static int Load();

    static bool loadedDep;
};


CPP_GATEWAY_PROTOTYPE_EXPORT(sci_get, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_xget, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_xset, GRAPHICS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_winsid, GRAPHICS_GW_IMPEXP);
#endif /* !__GUI_GW_HXX__ */
