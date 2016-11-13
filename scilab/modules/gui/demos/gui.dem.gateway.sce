// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("gui.dem.gateway.sce");
    add_demo(gettext("GUI"), demopath + "gui.dem.gateway.sce");

    subdemolist = [_("Dialogs")   ,"dialogs/dialogs.dem.gateway.sce" ; ..
    _("Uicontrols 1") ,"uicontrol.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Uicontrols 2") ,"uicontrol_plot3d.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Uicontrols with LaTeX/MathML") ,  "uicontrol_LaTeX.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Show images with uicontrols ") ,  "uicontrol_image.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Listboxes and Popupmenus") ,  "uicontrol_listbox_popupmenu.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Uicontrols rendering") ,  "usedeprecatedskin.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Uicontrols layout") ,  "uicontrol_layout.dem.sce" ];

    subdemolist = [ subdemolist ;
    _("Uicontrols messagebox-like") ,  "uicontrol_messagebox.dem.sce" ];

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
