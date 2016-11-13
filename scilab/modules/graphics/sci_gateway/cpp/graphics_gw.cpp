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

#define MODULE_NAME "graphics"
extern "C"
{
#include "gw_graphics.h"
#include "loadOnUseClassPath.h"
}

bool GraphicsModule::loadedDep = false;

int GraphicsModule::LoadDeps(const std::string& _functionName)
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
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("champ", &sci_champ, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("champ1", &sci_champ1, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fec", &sci_fec, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("geom3d", &sci_geom3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("grayplot", &sci_grayplot, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("Matplot", &sci_matplot, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("Matplot1", &sci_matplot1, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("contour2di", &sci_contour2di, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("param3d", &sci_param3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("param3d1", &sci_param3d1, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xstringb", &sci_xstringb, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plot2d", &sci_plot2d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plot2d2", &sci_plot2d1_2, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plot2d3", &sci_plot2d1_3, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plot2d4", &sci_plot2d1_4, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plot3d", &sci_plot3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("plot3d1", &sci_plot3d, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("winsid", &sci_winsid, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xarc", &sci_xarc, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xarcs", &sci_xarcs, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xarrows", &sci_xarrows, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("drawaxis", &sci_drawaxis, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xchange", &sci_xchange, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xclick", &sci_xclick, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xdel", &sci_xdel, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xfarc", &sci_xarc, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xfarcs", &sci_xfarcs, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xfpoly", &sci_xfpoly, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xfpolys", &sci_xfpolys, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xfrect", &sci_xrect, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xget", &sci_xget, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xgetmouse", &sci_xgetmouse, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xgrid", &sci_xgrid, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xlfont", &sci_xlfont, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xpoly", &sci_xpoly, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xpolys", &sci_xpolys, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xrect", &sci_xrect, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xrects", &sci_xrects, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xsegs", &sci_xsegs, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("show_window", &sci_show_window, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xset", &sci_xset, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xstring", &sci_xstring, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xtitle", &sci_xtitle, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xgraduate", &sci_xgraduate, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("xname", &sci_xname, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("zoom_rect", &sci_zoom_rect, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("unzoom", &sci_unzoom, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("stringbox", &sci_stringbox, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("move", &sci_move, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("glue", &sci_glue, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("unglue", &sci_unglue, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("drawnow", &sci_drawnow, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("drawlater", &sci_drawlater, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("copy", &sci_copy, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("delete", &sci_delete, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("get", &sci_get, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("set", &sci_set, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("newaxes", &sci_newaxes, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("swap_handles", &sci_swap_handles, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("rubberbox", &sci_rubberbox, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("rotate_axes", &sci_rotate_axes, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("is_handle_valid", &sci_is_handle_valid, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("captions", &sci_Legend, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("light", &sci_light, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("addcolor", &sci_addcolor, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("name2rgb", &sci_name2rgb, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("color", &sci_color, &GraphicsModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("contour2dm", &sci_contour2dm, &GraphicsModule::LoadDeps, MODULE_NAME));
    return 1;
}
