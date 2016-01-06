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

#include "gui_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME "gui"
extern "C"
{
#include "gw_gui.h"
#include "loadOnUseClassPath.h"
}

bool GuiModule::loadedDep = false;

int GuiModule::LoadDeps(const std::string& _functionName)
{
    if (loadedDep == false &&
            (_functionName == "uicontrol" ||
             _functionName == "uimenu" ||
             _functionName == "usecanvas" ||
             _functionName == "loadGui" ||
             _functionName == "figure"))
    {
        loadOnUseClassPath("graphics");
        loadedDep = true;
    }

    return 1;
}

int GuiModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("x_dialog", &sci_x_dialog, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("x_choose", &sci_x_choose, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("x_mdialog", &sci_x_mdialog, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xchoicesi", &sci_x_choice, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("delmenu", &sci_delmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("setmenu", &sci_setmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("unsetmenu", &sci_unsetmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("raise_window", &sci_raise_window, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getlookandfeel", &sci_getlookandfeel, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getinstalledlookandfeels", &sci_getinstalledlookandfeels, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("setlookandfeel", &sci_setlookandfeel, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ClipBoard", &sci_ClipBoard, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("toolbar", &sci_toolbar, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uigetdir", &sci_uigetdir, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uicontrol", &sci_uicontrol, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uimenu", &sci_uimenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uipopup", &sci_mpopup, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("x_choose_modeless", &sci_x_choose_modeless, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uicontextmenu", &sci_uicontextmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uiwait", &sci_uiwait, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("messagebox", &sci_messagebox, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("waitbar", &sci_waitbar, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("progressionbar", &sci_progressionbar, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("helpbrowser", &sci_helpbrowser, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uigetfont", &sci_uigetfont, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uigetcolor", &sci_uigetcolor, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getcallbackobject", &sci_getcallbackobject, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("printfigure", &sci_printfigure, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("exportUI", &sci_exportUI, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("printsetupbox", &sci_printsetupbox, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("toprint", &sci_toprint, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uigetfile", &sci_uigetfile, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("usecanvas", &sci_usecanvas, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uiDisplayTree", &sci_displaytree, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("uiputfile", &sci_uiputfile, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("about", &sci_about, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fire_closing_finished", &sci_fire_closing_finished, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("useeditor", &sci_useeditor, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("openged", &sci_openged, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipCreate", &sci_datatipcreate, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipRemove", &sci_datatipremove, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plotbrowser", &sci_plotbrowser, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipManagerMode", &sci_datatip_manager_mode, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipToggle", &sci_datatip_toggle, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipMove", &sci_datatipmove, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipSetStyle", &sci_datatip_set_style, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipSetDisplay", &sci_datatip_set_display, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipSetInterp", &sci_datatip_set_interp, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("datatipSetOrient", &sci_datatip_set_orient, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("figure", &sci_figure, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("saveGui", &sci_saveGui, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("loadGui", &sci_loadGui, &GuiModule::LoadDeps, MODULE_NAME));
    return 1;
}
