function ged_polyline(h)
    global ged_handle; ged_handle=h

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

    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curcolor",string(h.foreground))
    TCL_SetVar("curback",string(h.background))
    TCL_SetVar("curthick",string(h.thickness))
    TCL_SetVar("curarrowsizefactor",string(h.arrow_size_factor))
    TCL_SetVar("curvis",h.visible)

    ged_polylinestylearray=["interpolated" "staircase" "barplot" "arrowed" "filled" "bar"];
    TCL_SetVar("curpolylinestyle",ged_polylinestylearray(max(h.polyline_style,1)))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))

    TCL_SetVar("curlinemode",h.line_mode)
    TCL_SetVar("curclosedmode",h.closed)
    TCL_SetVar("curfillmode",h.fill_mode)
    TCL_SetVar("curinterpcolormode",h.interp_color_mode)
    TCL_SetVar("curinterpcolorvector",sci2exp(h.interp_color_vector,0))

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata",d);

    select get(getparaxe(h),"view")
    case "2d"
        TCL_SetVar("nbcol",string(2));
    case "3d"
        TCL_SetVar("nbcol",string(3));
    end
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Polyline.tcl")
endfunction
