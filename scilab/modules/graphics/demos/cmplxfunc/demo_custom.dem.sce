// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

prot = funcprot();
funcprot(0);

function demo_custom()

    // Parameters
    // =========================================================================

    frame_w      = 300;     // Frame width
    frame_h      = 765;     // Frame height

    plot_w       = 500;     // Plot width
    plot_h       = frame_h; // Plot height

    margin_x     = 15;      // Horizontal margin between each elements
    margin_y     = 15;      // Vertical margin between each elements

    defaultfont  = "arial"; // Default Font

    // Figure creation
    // =========================================================================

    axes_w       = 3*margin_x + frame_w + plot_w; // axes width
    axes_h       = 2*margin_y + frame_h;          // axes height (100 => toolbar height)

    fig_handle = figure( ...
    "infobar_visible", "off", ...
    "menubar", "none", ...
    "toolbar", "none", ...
    "default_axes", "on", ...
    "layout", "gridbag", ...
    "visible", "off", ...
    "background", -2, ...
    "figure_position", [0 0], ...
    "axes_size", [axes_w axes_h], ...
    "figure_name", _("Customize your complex function"));

    // The plot will be in the right 2/3 of the figure
    a = gca();
    a.axes_bounds = [1/3 1 2/3 1];
    a.tight_limits = "on";

    c = createConstraints("gridbag", [2 1 2 1], [1 1], "vertical", "right");
    // Create an empty panel on the right to occupy the 3/4 of the screen (figure plot)
    empty_frame = uicontrol(fig_handle, "style", "frame","constraints", c);

    c = createConstraints("gridbag", [1 1 1 5], [0.25 1], "both", "left", [0 0], [150 0]);
    u = uicontrol(fig_handle, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "border", ...
    "constraints", c);

    // Add top and bottom empty panels to pad the upcoming control panel
    c = createConstraints("border", "top", [0 20]);
    top = uicontrol(u, "style", "frame", "backgroundcolor", [1 1 1], "constraints", c);
    c = createConstraints("border", "bottom", [0 20]);
    bottom = uicontrol(u, "style", "frame", "backgroundcolor", [1 1 1], "constraints", c);

    // Border for the control parameters frame
    b_f_controlParams = createBorderFont("", 18);
    b_l_controlParams = createBorder("line", "navy", 2);
    b_controlParams   = createBorder("titled", b_l_controlParams, _("Control panel"), "center", "top", b_f_controlParams, "navy");

    control_frame = uicontrol(u, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "border", b_controlParams, ...
    "layout", "gridbag", ...
    "tag", "control_frame");

    demo_viewCode("demo_custom.dem.sce");

    fig_handle.color_map = jetcolormap(128);

    // Frames creation [Control Panel]
    // =========================================================================

    // Explanatory text
    // =========================================================================

    my_exptext_string = "";

    my_exptext_string = my_exptext_string + "<html>";

    my_exptext_string = my_exptext_string + "<center>";
    my_exptext_string = my_exptext_string + "<img src=""file:///"+SCI+"/modules/graphics/demos/cmplxfunc/warning.png"+""">";
    my_exptext_string = my_exptext_string + "BE CAREFUL, NOTHING IS PROTECTED";
    my_exptext_string = my_exptext_string + "</center>";

    my_exptext_string = my_exptext_string + "<hr><br />";

    my_exptext_string = my_exptext_string + " To draw your own complex  function :";

    my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px; width:260;''>";

    my_exptext_string = my_exptext_string + "1. define the  function by a correct ";
    my_exptext_string = my_exptext_string + "string where the complex var must ";
    my_exptext_string = my_exptext_string + "be z. Also, as the function will ";
    my_exptext_string = my_exptext_string + "be evaluated on a matrix, don''t forget ";
    my_exptext_string = my_exptext_string + "the . to operate elementwise. Examples: ";

    my_exptext_string = my_exptext_string + "</div>";


    my_exptext_string = my_exptext_string + "<pre>    z.^2       (z+1).*(z-1)     (1)./(z+2)</pre>";
    my_exptext_string = my_exptext_string + "<pre>    sqrt(z)    (z+%i).*(z-%i)   1+2*z+z.^2</pre>";

    my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px; width:260;''>";
    my_exptext_string = my_exptext_string + "2. define the type of the domain: Square or Disk,";
    my_exptext_string = my_exptext_string + "</div>";


    my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px; width:260;''>";
    my_exptext_string = my_exptext_string + "3. define the ""radius"" R of the domain,"+ "<br />";
    my_exptext_string = my_exptext_string + "</div>";


    my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px; width:260;''>";
    my_exptext_string = my_exptext_string + "4. should your function have some kind of ";
    my_exptext_string = my_exptext_string + "discontinuity on Ox or Oy => check ";
    my_exptext_string = my_exptext_string + "Ox or Oy. Otherwise check No.";
    my_exptext_string = my_exptext_string + "</div>";

    my_exptext_string = my_exptext_string + "</html>";


    // Explanatory text : frame

    b_f_expTxt = createBorderFont("", 12);
    b_l_expTxt = createBorder("line", "navy", 1);
    b_expTxt   = createBorder("titled", b_l_expTxt, "", "center", "top", b_f_expTxt, "navy");

    // Border for the explanatory text frame
    c = createConstraints("gridbag", [1 1 1 1], [1 1], "both", "upper");
    my_exptext_frame = uicontrol( ...
    "parent"              , control_frame,...
    "style"               , "frame",...
    "layout"              , "gridbag", ...
    "border"              , b_expTxt, ...
    "units"               , "pixels",...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_exptext_frame" ...
    );

    // Explanatory text : actual text

    c = createConstraints("gridbag", [1 1 1 1], [1 1], "both", "upper");
    my_exptext_box = uicontrol( ...
    "parent"              , my_exptext_frame,...
    "style"               , "text",...
    "units"               , "pixels",...
    "string"              , my_exptext_string,...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 9,...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_exptext_box" ...
    );


    // Function definition
    // =========================================================================

    // Function definition : frame

    // Border for the function definition frame
    b_f_funDef = createBorderFont("", 12);
    b_l_funDef = createBorder("line", "navy", 1);
    b_funDef   = createBorder("titled", b_l_funDef, _("Function definition"), "center", "top", b_f_funDef, "navy");

    c = createConstraints("gridbag", [1 2 1 1], [1 1], "both", "upper");
    my_fundef_frame = uicontrol( ...
    "parent"              , control_frame,...
    "style"               , "frame",...
    "layout"              , "gridbag", ...
    "border"              , b_funDef, ...
    "units"               , "pixels",...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 13,...
    "fontweight"          , "bold", ...
    "background"          , [1 1 1], ...
    "constraints"         , c, ...
    "tag"                 , "my_fundef_frame" ...
    );

    // Function definition : Text "f(z) = "

    c = createConstraints("gridbag", [1 1 1 1], [1/12 1], "both", "upper");
    my_fundef_editbox = uicontrol( ...
    "parent"             , my_fundef_frame,...
    "style"              , "text",...
    "string"              , "f(z) = ", ...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "background"         , [1 1 1], ...
    "constraint"         , c, ...
    "tag"                , "my_fundef_editbox");

    // Function definition : Edit box

    c = createConstraints("gridbag", [2 1 1 1], [12 1], "both", "upper");
    my_fundef_editbox = uicontrol( ...
    "parent"             , my_fundef_frame,...
    "style"              , "edit",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "string"             , "(1)./((z+2).*(2-z))", ...
    "callback"           , "update_fundef()",...
    "constraint"         , c, ...
    "tag"                , "my_fundef_editbox");

    // Function definition : message bar

    c = createConstraints("gridbag", [1 2 2 1], [1 1], "both", "upper");
    my_exptext_bar = uicontrol( ...
    "parent"              , my_fundef_frame,...
    "style"               , "text",...
    "string"              , " ", ...
    "units"               , "pixels",...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 11,...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_funDef_bar" ...
    );


    // Domain type
    // =========================================================================

    // Domain type : frame

    // Border for the domain type frame
    b_f_domain = createBorderFont("", 12);
    b_l_domain = createBorder("line", "navy", 1);
    b_domain   = createBorder("titled", b_l_domain, _("Domain type"), "center", "top", b_f_domain, "navy");

    c = createConstraints("gridbag", [1 3 1 1], [1 1], "both", "upper");
    my_dt_frame = uicontrol( ...
    "parent"              , control_frame,...
    "style"               , "frame",...
    "layout"              , "gridbag", ...
    "border"              , b_domain, ...
    "units"               , "pixels",...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 13,...
    "fontweight"          , "bold", ...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_dt_frame" ...
    );

    // Domain type : square

    c = createConstraints("gridbag", [1 1 1 1], [1 1], "both", "upper");
    square_radio = uicontrol( ...
    "parent"             , my_dt_frame,...
    "style"              , "radiobutton",...
    "string"             , "Square",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "value"              , 0, ...
    "background"         , [1 1 1], ...
    "callback"           , "update_domain_type();",...
    "constraints"        , c, ...
    "tag"                , "square_radio");

    // Domain type : Disk

    c = createConstraints("gridbag", [2 1 1 1], [1 1], "both", "upper");
    disk_radio = uicontrol( ...
    "parent"             , my_dt_frame,...
    "style"              , "radiobutton",...
    "string"             , "Disk",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "value"              , 1, ...
    "background"          , [1 1 1], ...
    "callback"           , "update_domain_type();",...
    "constraints"        , c, ...
    "tag"                , "disk_radio");


    // Radius of the domain
    // =========================================================================

    // Radius of the domain : frame

    // Border for the radius frame
    b_f_radius = createBorderFont("", 12);
    b_l_radius = createBorder("line", "navy", 1);
    b_radius   = createBorder("titled", b_l_radius, _("Radius of the domain"), "center", "top", b_f_radius, "navy");

    c = createConstraints("gridbag", [1 4 1 1], [1 1], "both", "upper");
    my_radius_frame = uicontrol( ...
    "parent"              , control_frame,...
    "style"               , "frame",...
    "layout"              , "gridbag", ...
    "border"              , b_radius, ...
    "units"               , "pixels",...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 13,...
    "fontweight"          , "bold", ...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_radius_frame" ...
    );

    // Radius of the domain : Edit box

    c = createConstraints("gridbag", [1 1 1 1], [1 1], "both", "upper");
    my_radius_editbox = uicontrol( ...
    "parent"             , my_radius_frame,...
    "style"              , "edit",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "string"             , "1.9", ...
    "callback"           , "update_radius()",...
    "constraint"         , c, ...
    "tag"                , "my_radius_editbox");

    // Radius of the domain : message bar

    c = createConstraints("gridbag", [1 2 1 1], [1 1], "both", "upper");
    my_exptext_bar = uicontrol( ...
    "parent"              , my_radius_frame,...
    "style"               , "text",...
    "string"              , " ", ...
    "units"               , "pixels",...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 11,...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_radius_bar" ...
    );


    // Cut on axes
    // =========================================================================

    // Cut on axes : frame

    // Border for the cut on axes frame
    b_f_cut = createBorderFont("", 12);
    b_l_cut = createBorder("line", "navy", 1);
    b_cut   = createBorder("titled", b_l_cut, _("Cut on axes"), "center", "top", b_f_cut, "navy");

    c = createConstraints("gridbag", [1 5 1 1], [1 1], "both", "upper");
    my_coa_frame = uicontrol( ...
    "parent"              , control_frame,...
    "style"               , "frame",...
    "layout"              , "gridbag", ...
    "border"              , b_cut, ...
    "units"               , "pixels",...
    "fontname"            , defaultfont,...
    "fontunits"           , "points",...
    "fontsize"            , 13,...
    "fontweight"          , "bold", ...
    "background"          , [1 1 1], ...
    "constraint"          , c, ...
    "tag"                 , "my_coa_frame" ...
    );

    // Cut on axes : Ox

    c = createConstraints("gridbag", [1 1 1 1], [1 1], "both", "upper");
    ox_radio = uicontrol( ...
    "parent"             , my_coa_frame,...
    "style"              , "radiobutton",...
    "string"             , "Ox",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "value"              , 0, ...
    "background"         , [1 1 1], ...
    "callback"           , "update_cao();",...
    "constraint"         , c, ...
    "tag"                , "ox_radio");

    // Cut on axes : Oy

    c = createConstraints("gridbag", [2 1 1 1], [1 1], "both", "upper");
    oy_radio = uicontrol( ...
    "parent"             , my_coa_frame,...
    "style"              , "radiobutton",...
    "string"             , "Oy",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "value"              , 0, ...
    "background"         , [1 1 1], ...
    "callback"           , "update_cao();",...
    "constraint"          , c, ...
    "tag"                , "oy_radio");

    // Cut on axes : No

    c = createConstraints("gridbag", [3 1 1 1], [1 1], "both", "upper");
    no_radio = uicontrol( ...
    "parent"             , my_coa_frame,...
    "style"              , "radiobutton",...
    "string"             , "No",...
    "fontname"           , defaultfont,...
    "fontunits"          , "points",...
    "fontsize"           , 11,...
    "value"              , 1, ...
    "background"         , [1 1 1], ...
    "callback"           , "update_cao();",...
    "constraint"          , c, ...
    "tag"                , "no_radio");

    // Default Values
    // =========================================================================

    global my_fundef_val;
    global my_dt_val;
    global my_radius;
    global my_typeCut;
    global my_e;


    my_fundef_val = "(1)./((z+2).*(2-z))";
    my_dt_val     = "Disk";
    my_radius     = 1.9;
    my_typeCut    = "No";

    my_e          = 0.001;
    theta         = -110;
    alpha         = 75;

    deff("Z=f(z)","Z="+my_fundef_val);

    PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
    fig_handle.visible = "on";

endfunction

function update_fundef()

    global my_fundef_val;
    global my_dt_val;
    global my_radius;
    global my_typeCut;
    global my_e;

    my_fundef_val_tmp = get(gcbo,"string");

    if strchr(my_fundef_val_tmp, "z") == "" then
        updateStatusBar("my_funDef_bar", _("Function should contain ""z"""), [1 0 0]);
        return
    end

    try // Try f(z) on a simple matrix
        ieeeMode = ieee();
        ieee(2);
        execstr(strsubst(my_fundef_val_tmp, "z", "[1 2 3; 1 2 3]"));
        ieee(ieeeMode);
    catch
        updateStatusBar("my_funDef_bar", _("Wrong definition of f(z)"), [1 0 0]);
        return
    end

    my_fundef_val = my_fundef_val_tmp;

    // Delete the old plots

    demo_fig = gcf();
    demo_fig.immediate_drawing = "off";

    a = gca(); delete(a);
    a = gca(); delete(a);
    a = gca(); delete(a);

    theta = -110;
    alpha = 75;

    deff("Z=f(z)","Z="+my_fundef_val);

    if my_dt_val == "Square" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
    elseif my_dt_val == "Disk" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
    end

    demo_fig.immediate_drawing = "on";

endfunction

function update_domain_type()

    global my_fundef_val;
    global my_dt_val;
    global my_radius;
    global my_typeCut;
    global my_e;

    // Update the radio elements
    set(findobj("tag", "square_radio") , "value", 0);
    set(findobj("tag", "disk_radio")   , "value", 0);
    set(gcbo, "value", 1);

    // Delete the old plots

    demo_fig = gcf();
    demo_fig.immediate_drawing = "off";

    a = gca(); delete(a);
    a = gca(); delete(a);
    a = gca(); delete(a);

    theta = -110;
    alpha = 75;

    deff("Z=f(z)","Z="+my_fundef_val);

    if get(gcbo, "tag") == "square_radio" then
        my_dt_val = "Square";
    elseif get(gcbo, "tag") == "disk_radio" then
        my_dt_val = "Disk";
    end

    if my_dt_val == "Square" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
    elseif my_dt_val == "Disk" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
    end

    demo_fig.immediate_drawing = "on";

endfunction

function update_radius()

    global my_fundef_val;
    global my_dt_val;
    global my_radius;
    global my_typeCut;
    global my_e;

    try
        my_radius_tmp = evstr(get(gcbo,"string"));
    catch
        updateStatusBar("my_radius_bar", _("Radius should be real and finite"), [1 0 0]);
        return
    end

    if ~isscalar(my_radius_tmp) | and(type(my_radius_tmp) <> [1 8]) | ~isreal(my_radius_tmp) | isinf(my_radius_tmp) then
        updateStatusBar(_("Radius should be real and finite"), [1 0 0]);
        return
    else
        my_radius = my_radius_tmp;
    end

    // Delete the old plots

    demo_fig = gcf();
    demo_fig.immediate_drawing = "off";

    a = gca(); delete(a);
    a = gca(); delete(a);
    a = gca(); delete(a);

    theta = -110;
    alpha = 75;

    deff("Z=f(z)","Z="+my_fundef_val);

    if my_dt_val == "Square" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
    elseif my_dt_val == "Disk" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
    end

    demo_fig.immediate_drawing = "on";

endfunction

function update_cao()

    global my_fundef_val;
    global my_dt_val;
    global my_radius;
    global my_typeCut;
    global my_e;

    // Update the radio elements
    set(findobj("tag", "ox_radio") , "value", 0);
    set(findobj("tag", "oy_radio") , "value", 0);
    set(findobj("tag", "no_radio") , "value", 0);
    set(gcbo, "value", 1);

    // Delete the old plots

    demo_fig = gcf();
    demo_fig.immediate_drawing = "off";

    a = gca(); delete(a);
    a = gca(); delete(a);
    a = gca(); delete(a);

    theta = -110;
    alpha = 75;

    deff("Z=f(z)","Z="+my_fundef_val);

    if get(gcbo, "tag")     == "ox_radio" then
        my_typeCut = "Ox";
    elseif get(gcbo, "tag") == "oy_radio" then
        my_typeCut = "Oy";
    elseif get(gcbo, "tag") == "no_radio" then
        my_typeCut = "No";
    end

    if my_dt_val == "Square" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
    elseif my_dt_val == "Disk" then
        PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
    end

    demo_fig.immediate_drawing = "on";

endfunction

function []=PlotCmplxFunc(R,e,TypeDomain,TypeCut,n,StrFunc,theta,alpha,DomReal)

    //  A function to draw on a square or a disk a complex function
    //  with branch(es) cut(s) on Ox or Oy
    //
    //  TypeDomain : "Square" or "Disk"
    //     TypeCut : "Ox" or "Oy"
    //           R : length of half a side of the square or radius of the disk
    //           e : thin layer to avoid the branch(es) cut(s)
    //           n : a scalar (for Square) or a 2-vector = [ntheta, nr]
    //               (for Disk) for discretization
    //     StrFunc : the string which names the complex function (this is
    //               because primitive don't pass as function argument)
    // theta,alpha : usual parameters for plot3d
    //     DomReal : interval for which the real restriction is drawn

    // computes the facets

    [xr,yr,zr,xi,yi,zi] = CmplxFacets(R,e,TypeDomain,TypeCut,n,StrFunc);

    // draw
    // ============================================

    // Title
    // ============================================

    my_title_axes             = newaxes();
    my_title_axes.axes_bounds = [1/3,0,2/3,1];

    // make axes transparent
    my_title_axes.filled = "off";

    Rs = string(R);

    if TypeDomain == "Square" then
        end_title = " function on [-"+Rs+","+Rs+"]x[-"+Rs+","+Rs+"]"
    else
        end_title = " function on D(0,R="+Rs+")"
    end

    if StrFunc == "f" then
        the_title = "Your custom (named f) complex" + end_title;
    else
        the_title = "The complex " + StrFunc + end_title;
    end

    xtitle(the_title);

    my_title_axes.title.text       = the_title;
    my_title_axes.title.font_size  = 3;
    my_title_axes.margins     = [ 0.15 0.08 0.08 0.08 ]

    // plot Im(z)
    // ============================================

    subplot(211)
    plot3d(xi,yi,zi,theta,alpha,"Re(z)@Im(z)@",[2 6 4]);

    my_IM_axes = gca();
    my_IM_axes.axes_bounds = [0.16, 0.05, 1, 0.5];
    my_IM_plot             = my_IM_axes.children;
    my_IM_plot.color_flag  = 1;

    xtitle("Im("+StrFunc+"(z))");
    my_IM_axes.margins      = [0.2, 0.2, 0.2, 0.2];
    my_IM_axes.cube_scaling = "on";

    // plot Re(z) + the real restriction
    // ============================================

    subplot(212)
    plot3d(xr,yr,zr,theta,alpha,"Re(z)@Im(z)@",[2 6 4]);

    my_RE_axes = gca();
    my_RE_axes.axes_bounds = [0.16, 0.5, 1, 0.5];
    my_RE_plot             = my_RE_axes.children;
    my_RE_plot.color_flag  = 1;

    xtitle("Re("+StrFunc+"(z))");
    my_RE_axes.margins      = [0.2, 0.2, 0.2, 0.2];
    my_RE_axes.cube_scaling = "on";

    // real function in yellow
    // ============================================

    if DomReal(2) > DomReal(1) then
        xstring(0.1,-0.15," In yellow : the real "+StrFunc+" function")
    end

    if DomReal(2) > DomReal(1) then
        xx = linspace(DomReal(1),DomReal(2),40)';
        yy = zeros(xx);
        zz = evstr(StrFunc+"(xx)");
        param3d1(xx,yy,list(zz,32),theta,alpha,flag=[0,0]);
        yellow_line = get("hdl");
        yellow_line.thickness = 3;
    end

endfunction

function [xr,yr,zr,xi,yi,zi] = CmplxFacets(R,e,TypeDomain,TypeCut,n,StrFunc)

    //  A function to compute the facets for drawing a complex function
    //  on a square or a disk with branch(es) cut(s) on Ox or Oy
    //
    //  TypeDomain : "Square" or "Disk"
    //     TypeCut : "Ox" or "Oy"
    //           R : length of half a side of the square or radius of the disk
    //           e : thin layer to avoid the branch(es) cut(s)
    //           n : a scalar (for Square) or a 2-vector = [ntheta, nr]
    //               (for Disk) for discretization
    //     StrFunc : the string which names the complex function (this is
    //               because primitive don't pass as function argument)

    if TypeDomain == "Square" then
        if TypeCut == "Ox" then
            x1 = linspace(-R, R, n);
            y1 = linspace( e, R, int(n/2));
        else  // for TypeCut = "Oy" ...
            x1 = linspace( e, R, int(n/2));
            y1 = linspace(-R, R, n);
        end
        X1 = ones(y1')*x1 ; Y1 = y1'*ones(x1);

    else // for TypeDomain = "Disk"
        r = linspace(0,R, n(2));
        if TypeCut == "Ox" then
            theta = linspace(0,%pi,n(1))';
            X1 = cos(theta)*r;
            Y1 = e + sin(theta)*r;
        else // for TypeCut = "Oy"
            theta = linspace(-%pi/2,%pi/2,n(1))';
            X1 = e + cos(theta)*r;
            Y1 = sin(theta)*r;
        end
    end

    X2 = -X1 ; Y2 = -Y1;
    Z1 = evstr(StrFunc+"(X1 + %i*Y1)");
    Z2 = evstr(StrFunc+"(X2 + %i*Y2)");
    [xr1,yr1,zr1] = nf3d(X1,Y1,real(Z1));
    [xr2,yr2,zr2] = nf3d(X2,Y2,real(Z2));
    xr = [xr1 xr2]; yr = [yr1 yr2]; zr = [zr1 zr2];
    [xi1,yi1,zi1] = nf3d(X1,Y1,imag(Z1));
    [xi2,yi2,zi2] = nf3d(X2,Y2,imag(Z2));
    xi = [xi1 xi2]; yi = [yi1 yi2]; zi = [zi1 zi2];

endfunction

// =============================================================================
// updateStatusBar
// + Update the string in the text frame
// =============================================================================
function updateStatusBar(tag,msg, msg_color)
    time_active = 2; //Time the message is active in s

    if argn(2) == 0 then
        set(tag, "String", " ");
        return
    end

    h = gcf();
    set(tag, "Foregroundcolor", msg_color, "String", msg);

    delta_time = 0;
    timer()
    while delta_time < time_active
        delta_time = delta_time + timer();
    end

    if is_handle_valid(h)
        updateStatusBar();
    end
endfunction

funcprot(prot);

demo_custom();

clear demo_custom();
