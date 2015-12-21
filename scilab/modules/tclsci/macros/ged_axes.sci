function ged_axes(h)
    global ged_handle;ged_handle=h;

    LoadTicks2TCL(h);

    TCL_SetVar("Xaxes_reverseToggle",h.axes_reverse(1))
    TCL_SetVar("Yaxes_reverseToggle",h.axes_reverse(2))
    TCL_SetVar("Zaxes_reverseToggle",h.axes_reverse(3))

    // forgotten axes bounds info.
    TCL_SetVar("axes_boundsL",string(h.axes_bounds(1,1)))
    TCL_SetVar("axes_boundsU",string(h.axes_bounds(1,2)))
    TCL_SetVar("axes_boundsW",string(h.axes_bounds(1,3)))
    TCL_SetVar("axes_boundsH",string(h.axes_bounds(1,4)))

    // forgotten visibility info.
    TCL_SetVar("xlabel_visibility",string(h.x_label.visible))
    TCL_SetVar("ylabel_visibility",string(h.y_label.visible))
    TCL_SetVar("zlabel_visibility",string(h.z_label.visible))
    TCL_SetVar("titlelabel_visibility",string(h.title.visible))

    TCL_SetVar("Lmargins",string(h.margins(1)));
    TCL_SetVar("Rmargins",string(h.margins(2)));
    TCL_SetVar("Tmargins",string(h.margins(3)));
    TCL_SetVar("Bmargins",string(h.margins(4)));
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);
    TCL_SetVar("curautoclear",h.auto_clear);
    TCL_SetVar("curautoscale",h.auto_scale);
    //TCL_SetVar("curfillmode",h.fill_mode);
    TCL_SetVar("curalpharotation",string(h.rotation_angles(1)))
    TCL_SetVar("curthetarotation",string(h.rotation_angles(2)))
    ged_fontarray = ["Monospaced" "Symbol" "Serif",..
    "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
    "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
    "SansSerif Bold Italic"];
    TCL_SetVar("Xlabelpos",h.x_location)
    TCL_SetVar("Ylabelpos",h.y_location)
    TCL_SetVar("Xlabelfontstyle",ged_fontarray(h.x_label.font_style+1))
    TCL_SetVar("Ylabelfontstyle",ged_fontarray(h.y_label.font_style+1))
    TCL_SetVar("Zlabelfontstyle",ged_fontarray(h.z_label.font_style+1))
    TCL_SetVar("TITLEfontstyle",ged_fontarray(h.title.font_style+1))
    TCL_SetVar("fontstyle",ged_fontarray(h.font_style+1))

    // label texts
    if size(h.x_label.text,"*") == 1 then
        txt = """" + h.x_label.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.x_label.text)),"x")+" string array]" ;
    end
    TCL_SetVar("xlabel", txt );

    if size(h.y_label.text,"*") == 1 then
        txt = """" + h.y_label.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.y_label.text)),"x")+" string array]" ;
    end
    TCL_SetVar("ylabel", txt );

    if size(h.z_label.text,"*") == 1 then
        txt = """" + h.z_label.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.z_label.text)),"x")+" string array]" ;
    end
    TCL_SetVar("zlabel", txt );

    if size(h.title.text,"*") == 1 then
        txt = """" + h.title.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.title.text)),"x")+" string array]" ;
    end
    TCL_SetVar("tlabel", txt );

    TCL_SetVar("xlabel_fontforeground",string(h.x_label.font_foreground))
    TCL_SetVar("ylabel_fontforeground",string(h.y_label.font_foreground))
    TCL_SetVar("zlabel_fontforeground",string(h.z_label.font_foreground))
    TCL_SetVar("titlelabel_fontforg",string(h.title.font_foreground))
    TCL_SetVar("xlabel_foreground",string(h.x_label.foreground))
    TCL_SetVar("ylabel_foreground",string(h.y_label.foreground))
    TCL_SetVar("zlabel_foreground",string(h.z_label.foreground))
    TCL_SetVar("titlelabel_foreground",string(h.title.foreground))
    TCL_SetVar("xlabel_background",string(h.x_label.background))
    TCL_SetVar("ylabel_background",string(h.y_label.background))
    TCL_SetVar("zlabel_background",string(h.z_label.background))
    TCL_SetVar("titlelabel_background",string(h.title.background))
    TCL_SetVar("x_position",sci2exp(h.x_label.position,0))
    TCL_SetVar("y_position",sci2exp(h.y_label.position,0))
    TCL_SetVar("z_position",sci2exp(h.z_label.position,0))
    TCL_SetVar("title_position",sci2exp(h.title.position,0))
    TCL_SetVar("xauto_position",h.x_label.auto_position)
    TCL_SetVar("yauto_position",h.y_label.auto_position)
    TCL_SetVar("zauto_position",h.z_label.auto_position)
    TCL_SetVar("titleauto_position",h.title.auto_position)
    TCL_SetVar("xauto_rotation",h.x_label.auto_rotation)
    TCL_SetVar("yauto_rotation",h.y_label.auto_rotation)
    TCL_SetVar("zauto_rotation",h.z_label.auto_rotation)
    TCL_SetVar("titleauto_rotation",h.title.auto_rotation)
    TCL_SetVar("Xfillmode",h.x_label.fill_mode)
    TCL_SetVar("Yfillmode",h.y_label.fill_mode)
    TCL_SetVar("Zfillmode",h.z_label.fill_mode)
    TCL_SetVar("Titlefillmode",h.title.fill_mode)
    TCL_SetVar("xlabel_fontsize",string(h.x_label.font_size))
    TCL_SetVar("ylabel_fontsize",string(h.y_label.font_size))
    TCL_SetVar("zlabel_fontsize",string(h.z_label.font_size))
    TCL_SetVar("titlelabel_fontsize",string(h.title.font_size))
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("fcolor",string(h.foreground))
    TCL_SetVar("bcolor",string(h.background))
    TCL_SetVar("curthick",string(h.thickness))
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curfontsize",string(h.font_size))
    TCL_SetVar("curfontcolor",string(h.font_color))
    TCL_SetVar("limToggle",h.tight_limits(1))
    TCL_SetVar("isoToggle",h.isoview)
    TCL_SetVar("cubToggle",h.cube_scaling)
    TCL_SetVar("viewToggle",h.view)
    TCL_SetVar("curBoxState",h.box)
    TCL_SetVar("xToggle",part(h.log_flags,1))
    TCL_SetVar("yToggle",part(h.log_flags,2))
    TCL_SetVar("zToggle",part(h.log_flags,3))
    TCL_SetVar("xGrid",string(h.grid(1)))
    TCL_SetVar("yGrid",string(h.grid(2)))

    TCL_SetVar("hiddenAxisColor",h.hidden_axis_color)
    TCL_SetVar("curfontangle_x",string(h.x_label.font_angle))
    TCL_SetVar("curfontangle_y",string(h.y_label.font_angle))
    TCL_SetVar("curfontangle_z",string(h.z_label.font_angle))
    TCL_SetVar("curfontangle_title",string(h.title.font_angle))


    select h.view
    case "2d"
        drawlater();
        h.view="3d"
        TCL_SetVar("old_curalpharotation",string(h.rotation_angles(1)))
        TCL_SetVar("old_curthetarotation",string(h.rotation_angles(2)))
        TCL_SetVar("zGrid",string(h.grid(3)))
        TCL_SetVar("zGrid_initial",string(h.grid(3))) //to avoid useless redraw (see Axes.tcl)
        TCL_SetVar("dbxmin",string(h.data_bounds(1,1)))
        TCL_SetVar("dbymin",string(h.data_bounds(1,2)))
        TCL_SetVar("dbzmin",string(h.data_bounds(1,3)))
        TCL_SetVar("dbxmax",string(h.data_bounds(2,1)))
        TCL_SetVar("dbymax",string(h.data_bounds(2,2)))
        TCL_SetVar("dbzmax",string(h.data_bounds(2,3)))
        h.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("zGrid",string(h.grid(3)))
        TCL_SetVar("zGrid_initial",string(h.grid(3))) //to avoid useless redraw (see Axes.tcl)
        TCL_SetVar("dbxmin",string(h.data_bounds(1,1)))
        TCL_SetVar("dbymin",string(h.data_bounds(1,2)))
        TCL_SetVar("dbzmin",string(h.data_bounds(1,3)))
        TCL_SetVar("dbxmax",string(h.data_bounds(2,1)))
        TCL_SetVar("dbymax",string(h.data_bounds(2,2)))
        TCL_SetVar("dbzmax",string(h.data_bounds(2,3)))
    end

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Axes.tcl")
endfunction
