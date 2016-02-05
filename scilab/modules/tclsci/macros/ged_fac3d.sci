function ged_fac3d(h)
    global ged_handle; ged_handle=h

    //  if (h.clip_box==[])
    //    TCL_SetVar("old_Xclipbox","")
    //    TCL_SetVar("old_Yclipbox","")
    //    TCL_SetVar("old_Wclipbox","")
    //    TCL_SetVar("old_Hclipbox","")
    //    TCL_SetVar("Xclipbox","")
    //    TCL_SetVar("Yclipbox","")
    //    TCL_SetVar("Wclipbox","")
    //    TCL_SetVar("Hclipbox","")
    //   else
    //    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    //    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    //  end
    //  TCL_SetVar("curclipstate",h.clip_state);

    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curcolormode",string(h.color_mode))
    TCL_SetVar("colorflagToggle",string(h.color_flag))
    TCL_SetVar("curforeground",string(h.foreground))
    TCL_SetVar("curhiddencolor",string(h.hiddencolor))
    TCL_SetVar("curthick",string(h.thickness))

    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))
    TCL_SetVar("curlinemode",h.surface_mode)


    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.z)),"x")+" double array]"
    TCL_SetVar("curdata_z",d);

    TCL_EvalStr("set flagCOLOR 0")
    if(h.data(1)==["3d" "x" "y" "z" "color"])
        TCL_EvalStr("set flagCOLOR 1")
        d="["+strcat(string(size(h.data.color)),"x")+" integer array]"
        TCL_SetVar("curdata_color",d);
    end


    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Fac3d.tcl")
endfunction
