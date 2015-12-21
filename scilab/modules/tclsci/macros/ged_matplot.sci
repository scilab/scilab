function ged_matplot(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata",d);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Matplot.tcl")

endfunction
