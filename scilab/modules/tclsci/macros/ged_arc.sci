function ged_arc(h)
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
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    TCL_SetVar("nbcol",string(size(h.data,2)))
    for i=1:size(h.data,2)
        val= "arcVAL("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(h.data(2)));
    end
    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curcolor",string(h.foreground))
    TCL_SetVar("curback",string(h.background))
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curlinemode",h.line_mode)
    TCL_SetVar("curfillmode",h.fill_mode)
    TCL_SetVar("curthick",string(h.thickness))

    ax=getparaxe(h);
    // Arc data
    select ax.view
    case "2d"
        drawlater();
        ax.view="3d"  //strange behavior in 3D... seems to be bugged!!
        TCL_SetVar("Xval",string(h.data(1)))
        TCL_SetVar("Yval",string(h.data(2)))
        TCL_SetVar("Zval",string(h.data(3)))
        TCL_SetVar("Wval",string(h.data(4)))
        TCL_SetVar("Hval",string(h.data(5)))
        TCL_SetVar("A1val",string(h.data(6)))
        TCL_SetVar("A2val",string(h.data(7)))
        ax.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("Xval",string(h.data(1)))
        TCL_SetVar("Yval",string(h.data(2)))
        TCL_SetVar("Zval",string(h.data(3)))
        TCL_SetVar("Wval",string(h.data(4)))
        TCL_SetVar("Hval",string(h.data(5)))
        TCL_SetVar("A1val",string(h.data(6)))
        TCL_SetVar("A2val",string(h.data(7)))
    end
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Arc.tcl")
endfunction
