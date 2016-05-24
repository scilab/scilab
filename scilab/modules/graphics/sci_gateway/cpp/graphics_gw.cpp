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

#pragma comment(lib,"../../../../bin/libintl.lib")

#include "graphics_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME L"graphics"
extern "C"
{
#include "gw_graphics.h"
#include "loadOnUseClassPath.h"
}

bool GraphicsModule::loadedDep = false;

int GraphicsModule::LoadDeps(const std::wstring& _functionName)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("graphics");
        loadedDep = true;
    }

    return 1;
}


int GraphicsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"champ", &sci_champ, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"champ1", &sci_champ1, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fec", &sci_fec, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"geom3d", &sci_geom3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"grayplot", &sci_grayplot, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"Matplot", &sci_matplot, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"Matplot1", &sci_matplot1, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"contour2di", &sci_contour2di, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"param3d", &sci_param3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"param3d1", &sci_param3d1, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xstringb", &sci_xstringb, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plot2d", &sci_plot2d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plot2d2", &sci_plot2d1_2, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plot2d3", &sci_plot2d1_3, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plot2d4", &sci_plot2d1_4, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plot3d", &sci_plot3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"plot3d1", &sci_plot3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"winsid", &sci_winsid, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xarc", &sci_xarc, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xarcs", &sci_xarcs, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xarrows", &sci_xarrows, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"drawaxis", &sci_drawaxis, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xchange", &sci_xchange, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xclick", &sci_xclick, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xdel", &sci_xdel, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xfarc", &sci_xarc, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xfarcs", &sci_xfarcs, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xfpoly", &sci_xfpoly, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xfpolys", &sci_xfpolys, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xfrect", &sci_xrect, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xget", &sci_xget, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xgetmouse", &sci_xgetmouse, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xgrid", &sci_xgrid, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xlfont", &sci_xlfont, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xpoly", &sci_xpoly, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xpolys", &sci_xpolys, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xrect", &sci_xrect, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xrects", &sci_xrects, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xsegs", &sci_xsegs, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"show_window", &sci_show_window, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xset", &sci_xset, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xstring", &sci_xstring, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xtitle", &sci_xtitle, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xgraduate", &sci_xgraduate, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"xname", &sci_xname, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"zoom_rect", &sci_zoom_rect, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"unzoom", &sci_unzoom, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"stringbox", &sci_stringbox, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"move", &sci_move, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"glue", &sci_glue, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"unglue", &sci_unglue, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"drawnow", &sci_drawnow, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"drawlater", &sci_drawlater, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"copy", &sci_copy, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"delete", &sci_delete, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"get", &sci_get, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"set", &sci_set, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"newaxes", &sci_newaxes, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"swap_handles", &sci_swap_handles, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rubberbox", &sci_rubberbox, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rotate_axes", &sci_rotate_axes, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"is_handle_valid", &sci_is_handle_valid, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"captions", &sci_Legend, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"light", &sci_light, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"addcolor", &sci_addcolor, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"name2rgb", &sci_name2rgb, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"color", &sci_color, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"contour2dm", &sci_contour2dm, &GraphicsModule::LoadDeps, MODULE_NAME));
    return 1;
}
