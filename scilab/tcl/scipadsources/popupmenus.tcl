##ES: this comes from a more recent version of Tknotepad: 0.7.8
proc showpopup2 {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
# FV 07/07/04, if there is no debug session, popup menu is the edit menu
# otherwise it is the debug menu
    if {[getdbstate] == "NoDebug"} {
        tk_popup $pad.filemenu.edit $numx $numy
    } else {
        tk_popup $pad.filemenu.debug $numx $numy
    }
}
proc showpopup3 {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    tk_popup $pad.filemenu.exec $numx $numy
}
proc showpopupfont {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    tk_popup $pad.filemenu.options $numx $numy
}
