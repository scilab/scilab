// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("gui.dem.gateway.sce");
    add_demo(gettext("GUI"), demopath + "gui.dem.gateway.sce");

    subdemolist = [_("Dialogs")                       , demopath + "dialog.dem.sce" ; ..
    _("Uicontrols 1")                  , demopath + "uicontrol.dem.sce" ; ..
    _("Uicontrols 2")                  , demopath + "uicontrol_plot3d.dem.sce" ; ..
    _("Uicontrols with LaTeX/MathML")  , demopath + "uicontrol_LaTeX.dem.sce" ; ..
    _("Show images with uicontrols ")  , demopath + "uicontrol_image.dem.sce" ];
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
