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

#ifndef __GUI_GW_HXX__
#define __GUI_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_gui_gw.h"

class GuiModule
{
private :
    GuiModule() {};
    ~GuiModule() {};
public :
    GUI_GW_IMPEXP static int LoadDeps(const std::wstring& _functionName);
    GUI_GW_IMPEXP static int Load();
    GUI_GW_IMPEXP static int Unload()
    {
        return 1;
    }

    static bool loadedDep;
};

CPP_GATEWAY_PROTOTYPE(sci_displaytree);
CPP_GATEWAY_PROTOTYPE(sci_helpbrowser);

#endif /* !__GUI_GW_HXX__ */
