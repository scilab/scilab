// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatipsGUI(curve_handle)
    global datatipGUIwin
    if datatipGUIwin<>[]&or(datatipGUIwin==winsid()) then //the gui window is already opened
        global  datatipGUICurve
        datatipGUICurve= curve_handle
        datatipSetGUI()
        return
    end


    global datatipGUIHandles datatipGUICurve datatipGUIwin
    margin_x     = 5;      // Horizontal margin between each elements
    margin_y     = 5;      // Vertical margin between each elements
    button_w     = 150;
    button_h     = 25;
    label_h      = 20;
    label_w      = 160;
    slabel_w     = 80;
    editor_h     = 120;
    axes_w       = 3*margin_x+label_w+2*(slabel_w+4*margin_x+label_h);
    editor_w     = axes_w-2*margin_x;
    axes_h       = 4*margin_y+6*(label_h+margin_y)+ button_h;
    defaultfont  = "arial"; // Default Font
    datatipGUICurve= curve_handle
    ud=datatipGetStruct(datatipGUICurve)
    if typeof(ud)<>"datatips" then
        //no datatips structure yet, initialize it
        ud=datatipGetStruct(curve)
    end
    style=ud.style

    datatipGUIwin=max(winsid())+1

    fig_id=datatipGUIwin
    fig = scf(fig_id)
    fig.event_handler="datatipGUIEventHandler"
    fig.event_handler_enable="on"

    // Remove Scilab graphics menus & toolbar
    //  drawlater (bug)
    delmenu(fig.figure_id, gettext("&File"));
    delmenu(fig.figure_id, gettext("&Tools"));
    delmenu(fig.figure_id, gettext("&Edit"));
    delmenu(fig.figure_id, gettext("&?"));
    toolbar(fig.figure_id, "off");
    fig.axes_size       = [axes_w axes_h];


    fig.background      = addcolor([0.8 0.8 0.8]);
    fig.figure_name     = _("Datatips style");
    ax=fig.children;
    ax.background= fig.background ;
    gui=uicontrol( ..
    "parent"              , fig,...
    "style"               , "frame", ..
    "units"               , "pixels", ..
    "position"            , [0 0 axes_w axes_h], ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");

    yo=axes_h-margin_y-label_h;
    xo=margin_x;
    H=[]
    //Datatips positionning
    x=xo;
    y=yo;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Marker style:"), ..
    "units"               , "pixels", ..
    "position"            , [x y label_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "fontweight"          , "bold", ..
    "horizontalalignment" , "left", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+label_w+margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Square"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "HorizontalAlignment" , "right", ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+slabel_w+margin_x
    r1=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 1, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""sty"",%t)", ..
    "visible"             , "on");
    x=x+label_h+4*margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Arrow"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "HorizontalAlignment" , "right", ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");

    x=x+slabel_w+margin_x;
    r2=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 0, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""sty"",%f)", ..
    "visible"             , "on");
    H=[H;r1 r2]
    //Datatips Label
    x=xo;
    y=y-margin_y-label_h;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Labeled?"), ..
    "units"               , "pixels", ..
    "position"            , [x y label_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "fontweight"          , "bold", ..
    "horizontalalignment" , "left", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+label_w+margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Yes"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+slabel_w+margin_x
    r1=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 1, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""lab"",%t)", ..
    "visible"             , "on");
    x=x+label_h+4*margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("No"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");

    x=x+slabel_w+margin_x;
    r2=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 0, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""lab"",%f)", ..
    "visible"             , "on");
    H=[H;r1 r2]

    //Datatips box
    x=xo;
    y=y-margin_y-label_h;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Boxed?"), ..
    "units"               , "pixels", ..
    "position"            , [x y label_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "fontweight"          , "bold", ..
    "horizontalalignment" , "left", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+label_w+margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Yes"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+slabel_w+margin_x;
    r1=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 1, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""box"",%t)", ..
    "visible"             , "on");
    x=x+label_h+4*margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("No"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");

    x=x+slabel_w+margin_x;
    r2=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 0, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""box"",%f)", ..
    "visible"             , "on");
    H=[H;r1 r2]
    //marker position
    x=xo;
    y=y-margin_y-label_h;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Positionning?"), ..
    "units"               , "pixels", ..
    "position"            , [x y label_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "fontweight"          , "bold", ..
    "horizontalalignment" , "left", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+label_w+margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Knots"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+slabel_w+margin_x;
    r1=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 1, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""pos"",%t)", ..
    "visible"             , "on");
    x=x+label_h+4*margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Interpolated"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");

    x=x+slabel_w+margin_x;
    r2=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 0, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""pos"",%f)", ..
    "visible"             , "on");
    H=[H;r1 r2]
    //Multiple datatips
    x=xo;
    y=y-margin_y-label_h;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Allow multiple datatips?"), ..
    "units"               , "pixels", ..
    "position"            , [x y label_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "fontweight"          , "bold", ..
    "horizontalalignment" , "left", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+label_w+margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("Yes"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");
    x=x+slabel_w+margin_x;
    r1=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 1, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""mul"",%t)", ..
    "visible"             , "on");
    x=x+label_h+4*margin_x;
    uicontrol( ...
    "parent"              , gui, ..
    "style"               , "text", ..
    "string"              , _("No"), ..
    "units"               , "pixels", ..
    "position"            , [x y slabel_w label_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "horizontalalignment" , "right", ..
    "background"          , [1 1 1]*0.8, ..
    "visible"             , "on");

    x=x+slabel_w+margin_x;
    r2=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "radiobutton", ..
    "value"               , 0, ..
    "units"               , "pixels", ..
    "position"            , [x y label_h label_h], ..
    "background"          , [1 1 1]*0.8, ..
    "callback"            , "datatipRadioCallback(""mul"",%f)", ..
    "visible"             , "on");
    H=[H;r1 r2]
    //label edition function
    x=xo;
    y=y-2*margin_y-button_h;

    E=  uicontrol( ...
    "parent"              , gui, ..
    "style"               , "pushbutton", ..
    "string"              , _("Edit label generator"), ..
    "units"               , "pixels", ..
    "position"            , [x y button_w button_h], ..
    "fontname"            , defaultfont, ..
    "fontunits"           , "points", ..
    "fontsize"            , 12, ..
    "callback"            , "global  datatipGUICurve;datatipSetDisplay(datatipGUICurve)", ..
    "visible"             , "on");


    datatipGUIHandles=H
    datatipSetGUI()
endfunction
