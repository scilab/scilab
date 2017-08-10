function ged_segs(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curarrowsize",string(h.arrow_size))
    TCL_SetVar("curthick",string(h.thickness))
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

    TCL_SetVar("nbrow",string(size(h.data,1)))

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata",d);

    TCL_SetVar("nbcolsegscolor",string(size(h.segs_color,2)))
    for i=1:size(h.segs_color,2)
        val= "segscolorVAL("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(h.segs_color(i)));
    end

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

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Segs.tcl")
endfunction
