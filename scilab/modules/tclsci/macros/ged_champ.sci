function ged_champ(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curcolored",h.colored)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curarrowsize",string(h.arrow_size))
    TCL_SetVar("curthick",string(h.thickness))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))

    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.fx)),"x")+" double array]"
    TCL_SetVar("curdata_fx",d);
    d="["+strcat(string(size(h.data.fy)),"x")+" double array]"
    TCL_SetVar("curdata_fy",d);

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

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Champ.tcl")
endfunction
