proc showpopup2 {} {
    global pad mouseoversel
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    # if there is no debug session, popup menu is the edit menu
    # otherwise it is the debug menu
    if {[getdbstate] == "NoDebug"} {
        tk_popup $pad.filemenu.edit $numx $numy
    } else {
        set ta [gettextareacur]
        if {$mouseoversel == "true"} { 
            showpopupdebugwsel [[gettextareacur] index current]
        } else {
            tk_popup $pad.filemenu.debug $numx $numy
        }
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

proc showpopupsource {ind} {
    global pad textareacur menuFont
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    catch {destroy $pad.popsource}
    if {[lsearch [$textareacur tag names $ind] "libfun"] ==-1} return
    set lrange [$textareacur tag prevrange libfun "$ind+1c"]
    if {$lrange==""} {set lrange [$textareacur tag nextrange libfun $ind]}
    set curterm [$textareacur get [lindex $lrange 0] [lindex $lrange 1]]
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        if {$curterm!=""} {
            set sourcecommand "scipad(get_function_path(\"$curterm\"))"
            menu $pad.popsource -tearoff 0 -font $menuFont
            set plabel [concat [mc "Open the source of"] $curterm ]
            $pad.popsource add command -label $plabel\
              -command "ScilabEval $sourcecommand"
        }
    }
    tk_popup $pad.popsource $numx $numy
}

proc showpopupdebugwsel {ind} {
    global pad textareacur menuFont
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    catch {destroy $pad.popdebugwsel}
    menu $pad.popdebugwsel -tearoff 0 -font $menuFont
    set plabel [mc "Add watch"]
    $pad.popdebugwsel add command -label $plabel\
        -command "quickAddWatch_bp"
    tk_popup $pad.popdebugwsel $numx $numy
}
