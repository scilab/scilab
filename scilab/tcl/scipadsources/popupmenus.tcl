##ES: this comes from a more recent version of Tknotepad: 0.7.8
proc showpopup2 {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    tk_popup $pad.filemenu.edit $numx $numy
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
