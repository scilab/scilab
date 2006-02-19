proc showpopup2 {} {
    global pad mouseoversel
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    # if there is no debug session, popup menu is the edit menu
    # otherwise it is the debug menu
    if {[getdbstate] == "NoDebug"} {
        tk_popup $pad.filemenu.edit $numx $numy
    } else {
        set ta [gettextareacur]
        # If there is a selection, and the selected text contains only one active
        # tag, which is sel, and if the selected trimmed string matches a regexp
        # (constructed from help names in Scilab) then the selection is probably
        # a valid variable to watch and the quick add watch menu should pop up
        if {$mouseoversel == "true"} {
            if {[lsearch [$ta tag names sel.first] "sel"] != -1} {
                set watchvar [string trim [$ta get sel.first sel.last]]
                regexp {\A[\%_\#\!\$\?a-zA-Z][_\#\!\$\?a-zA-Z0-9]*\Z} $watchvar validwatchvar
            }
        }
        if {[info exists validwatchvar]} {
            showpopupdebugwsel $validwatchvar
        } else {
            tk_popup $pad.filemenu.debug $numx $numy
        }
    }
}

proc showpopup3 {} {
    global pad 
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    tk_popup $pad.filemenu.exec $numx $numy
}

proc showpopupfont {} {
    global pad
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    tk_popup $pad.filemenu.options $numx $numy
}

proc showpopupsource {ind} {
    global pad textareacur menuFont words
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    catch {destroy $pad.popsource}
    set tagname ""
    if {[lsearch [$textareacur tag names $ind] "libfun"] != -1} {
        set tagname libfun
    } elseif {[lsearch [$textareacur tag names $ind] "scicos"] != -1} {
        set tagname scicos
    } elseif {[lsearch [$textareacur tag names $ind] "userfun"] != -1} {
        set tagname userfun
    } else {
        return
    }
    set lrange [$textareacur tag prevrange $tagname "$ind+1c"]
    if {$lrange==""} {set lrange [$textareacur tag nextrange $tagname $ind]}
    set curterm [$textareacur get [lindex $lrange 0] [lindex $lrange 1]]
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        if {$curterm!=""} {
            if {$tagname == "userfun"} {
                set nameinitial [string range $curterm 0 0]
                set candidates $words(scilab.$tagname.$nameinitial)
                for {set i 0} {$i<[llength $candidates]} {incr i} {
                    if {[lindex [lindex $candidates $i] 0] == $curterm} {
                        set plabel [concat [mc "Jump to"] $curterm ]
                        set sourcecommand \
                            "dogotoline \"physical\" 1 \"function\" [list [lindex $candidates $i]]"
                        break
                    }
                }
            } else {
                # scicos or libfun
                set plabel [concat [mc "Open the source of"] $curterm ]
                set sourcecommand \
                    "ScilabEval_lt scipad(get_function_path(\"$curterm\"))"
            }
            menu $pad.popsource -tearoff 0 -font $menuFont
            $pad.popsource add command -label $plabel -command $sourcecommand
            tk_popup $pad.popsource $numx $numy
       }
    }
}

proc showpopupdebugwsel {watchvar} {
    global pad menuFont
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    catch {destroy $pad.popdebugwsel}
    menu $pad.popdebugwsel -tearoff 0 -font $menuFont
    set plabel [mc AddWatch $watchvar]
    $pad.popdebugwsel add command -label $plabel\
        -command "quickAddWatch_bp {$watchvar}"
    tk_popup $pad.popdebugwsel $numx $numy
}
