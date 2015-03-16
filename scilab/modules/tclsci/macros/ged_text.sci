function ged_text(h)
    global ged_handle; ged_handle=h
    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curfontforeground",string(h.font_foreground))
    ged_fontarray = ["Monospaced" "Symbol" "Serif",..
    "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
    "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
    "SansSerif Bold Italic"];
    TCL_SetVar("curfontstyle",ged_fontarray(h.font_style+1))
    TCL_SetVar("curfontsize",string(h.font_size))
    TCL_SetVar("curfontangle",string(h.font_angle))
    TCL_SetVar("curtextboxmode",h.text_box_mode)
    TCL_SetVar("curtext",h.text)
    TCL_SetVar("curforeground",string(h.foreground))
    TCL_SetVar("curbackground",string(h.background))
    TCL_SetVar("curboxmode",h.box)
    TCL_SetVar("curlinemode",h.line_mode);
    TCL_SetVar("curfillmode",h.fill_mode);
    //TCL_SetVar("curPosition",h.data) ;

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

    if size(h.text,"*") == 1 then
        txt =""""+ h.text +"""" ; // one double quote at each side
    else
        txt = "["+strcat(string(size(h.text)),"x")+" string array]"
    end
    TCL_SetVar("curtext", txt );
    TCL_SetVar( "textBoxWidth", string( h.text_box(1) ) ) ;
    TCL_SetVar("textBoxHeight", string( h.text_box(2) ) ) ;
    TCL_SetVar("curAlignment",string(h.alignment) ) ;

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Text.tcl")
endfunction
