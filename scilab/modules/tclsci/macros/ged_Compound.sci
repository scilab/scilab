function ged_Compound(h)
    global ged_handle;ged_handle=h;
    TCL_SetVar("curvis",h.visible)
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Compound.tcl")
endfunction
