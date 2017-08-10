function ged_grayplot(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curdatamapping",h.data_mapping)


    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.z)),"x")+" double array]"
    TCL_SetVar("curdata_z",d);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Grayplot.tcl")

endfunction
