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
