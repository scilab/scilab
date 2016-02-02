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

#include "gui_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME L"gui"
extern "C"
{
#include "gw_gui.h"
#include "loadOnUseClassPath.h"
}

bool GuiModule::loadedDep = false;

int GuiModule::LoadDeps(const std::wstring& _functionName)
{
    if (loadedDep == false &&
            (_functionName == L"uicontrol" ||
             _functionName == L"uimenu" ||
             _functionName == L"usecanvas" ||
             _functionName == L"loadGui" ||
             _functionName == L"figure"))
    {
        loadOnUseClassPath("graphics");
        loadedDep = true;
    }

    return 1;
}

int GuiModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"x_dialog", &sci_x_dialog, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"x_choose", &sci_x_choose, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"x_mdialog", &sci_x_mdialog, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xchoicesi", &sci_x_choice, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"delmenu", &sci_delmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"setmenu", &sci_setmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"unsetmenu", &sci_unsetmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"raise_window", &sci_raise_window, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getlookandfeel", &sci_getlookandfeel, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getinstalledlookandfeels", &sci_getinstalledlookandfeels, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"setlookandfeel", &sci_setlookandfeel, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ClipBoard", &sci_ClipBoard, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"toolbar", &sci_toolbar, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uigetdir", &sci_uigetdir, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uicontrol", &sci_uicontrol, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uimenu", &sci_uimenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uipopup", &sci_mpopup, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"x_choose_modeless", &sci_x_choose_modeless, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uicontextmenu", &sci_uicontextmenu, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uiwait", &sci_uiwait, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"messagebox", &sci_messagebox, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"waitbar", &sci_waitbar, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"progressionbar", &sci_progressionbar, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"helpbrowser", &sci_helpbrowser, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uigetfont", &sci_uigetfont, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uigetcolor", &sci_uigetcolor, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getcallbackobject", &sci_getcallbackobject, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"printfigure", &sci_printfigure, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"exportUI", &sci_exportUI, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"printsetupbox", &sci_printsetupbox, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"toprint", &sci_toprint, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uigetfile", &sci_uigetfile, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"usecanvas", &sci_usecanvas, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uiDisplayTree", &sci_displaytree, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uiputfile", &sci_uiputfile, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"about", &sci_about, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fire_closing_finished", &sci_fire_closing_finished, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"useeditor", &sci_useeditor, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"openged", &sci_openged, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipCreate", &sci_datatipcreate, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipRemove", &sci_datatipremove, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plotbrowser", &sci_plotbrowser, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipManagerMode", &sci_datatip_manager_mode, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipToggle", &sci_datatip_toggle, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipMove", &sci_datatipmove, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipSetStyle", &sci_datatip_set_style, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipSetDisplay", &sci_datatip_set_display, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipSetInterp", &sci_datatip_set_interp, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"datatipSetOrient", &sci_datatip_set_orient, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"figure", &sci_figure, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"saveGui", &sci_saveGui, &GuiModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"loadGui", &sci_loadGui, &GuiModule::LoadDeps, MODULE_NAME));
    return 1;
}
