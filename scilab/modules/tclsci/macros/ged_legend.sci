function ged_legend(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curforeground",string(h.foreground))
    ged_fontarray = ["Monospaced" "Symbol" "Serif",..
    "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
    "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
    "SansSerif Bold Italic"];
    TCL_SetVar("curfontstyle",ged_fontarray(h.font_style+1))
    TCL_SetVar("curfontsize",string(h.font_size))
    TCL_SetVar("curtext",h.text)

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Legend.tcl")
endfunction
