function ged_axis(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curseg",h.tics_segment)
    TCL_SetVar("curcolor",string(h.tics_color))
    TCL_SetVar("curticsstyle",h.tics_style)
    TCL_SetVar("curfontcolor",string(h.labels_font_color))
    TCL_SetVar("curfontsize",string(h.labels_font_size))
    TCL_SetVar("nbcolX",string(size(h.xtics_coord,2)))
    TCL_SetVar("nbcolY",string(size(h.ytics_coord,2)))
    TCL_SetVar("xticscoord",sci2exp(h.xtics_coord,0))
    TCL_SetVar("yticscoord",sci2exp(h.ytics_coord,0))
    TCL_SetVar("cursubtics",string(h.sub_tics))
    TCL_SetVar("curticslabel",sci2exp(h.tics_labels,0))
    TCL_SetVar("curticsdir",string(h.tics_direction))

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

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Axis.tcl")
endfunction
