function ged_fec(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("zbmin",string(h.z_bounds(1)))
    TCL_SetVar("zbmax",string(h.z_bounds(2)))
    //  TCL_SetVar("nbrow",string(size(h.data,1)))
    //  TCL_SetVar("nbcol",string(size(h.data,2)))
    //  TCL_SetVar("nbrowTri",string(size(h.triangles,1)))
    //  TCL_SetVar("nbcolTri",string(size(h.triangles,2)))

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata_data",d);
    d="["+strcat(string(size(h.triangles)),"x")+" double array]"
    TCL_SetVar("curdata_triangles",d);

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

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Fec.tcl")
endfunction
