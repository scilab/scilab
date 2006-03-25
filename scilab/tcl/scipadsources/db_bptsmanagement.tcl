proc insertremove_bp {{buf "current"}} {
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    $textarea see insert
    if {[getdbstate] == "DebugInProgress"} {
        insertremovedebug_bp $textarea
    } elseif {![isnocodeline insert]} {
        set infun [whichfun [$textarea index "insert linestart"] $textarea]
        if {$infun !={}} {
            set i1 [getrealstartofcontline $textarea "insert linestart"]
            set i2 [$textarea index "$i1 lineend"  ]
            set activetags [$textarea tag names $i1]
            if {[lsearch $activetags breakpoint] == -1} {
                $textarea tag add breakpoint $i1 $i2
            } else {
                $textarea tag remove breakpoint $i1 $i2
            }
        } else {
            # <TODO> .sce case
        }
    } else {
        showinfo [mc "No breakpoint here!"]
    }
}

proc insertremovedebug_bp {textarea} {
    global setbptonreallybreakpointedlinescmd
    if {[isscilabbusy 5]} {return}
    set i1 [getrealstartofcontline $textarea "insert linestart"]
    set i2 [$textarea index "$i1 lineend"  ]
    if {![isnocodeline $i1]} {
        set activetags [$textarea tag names $i1]
        if {[lsearch $activetags breakpoint] == -1} {
            set infun [whichfun [$textarea index $i1] $textarea]
            if {$infun !={}} {
                $textarea tag add breakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set setbpcomm " setbpt(\"$funname\",$lineinfun);"
                append setbptonreallybreakpointedlinescmd $setbpcomm
                ScilabEval_lt $setbpcomm "seq"
            } else {
                # <TODO> .sce case
            }
        } else {
            set infun [whichfun [$textarea index $i1] $textarea]
            if {$infun !={}} {
                $textarea tag remove breakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set delbpcomm " delbpt(\"$funname\",$lineinfun);"
                append setbptonreallybreakpointedlinescmd $delbpcomm
                ScilabEval_lt $delbpcomm  "seq"
            } else {
                # <TODO> .sce case
            }
        }
    } else {
        showinfo [mc "No breakpoint here!"]
    }
}

proc removeall_bp {} {
# remove all breakpoint tags from the opened buffer
# possibly existing breakpoints in Scilab are not touched
    global listoftextarea
    if {[isscilabbusy 5]} {return}
    foreach textarea $listoftextarea {
        set saveinsert [$textarea index insert]
        set tagranges [$textarea tag ranges breakpoint]
        foreach {bp_start bp_stop} $tagranges {
            $textarea mark set insert $bp_start
            insertremove_bp $textarea
        }
        $textarea mark set insert $saveinsert
    }
}

proc removescilab_bp {outp} {
# if $outp != "no_output", remove all the breakpoints set in Scilab
# otherwise just return the command that would do that
    global funnames listoftextarea
    if {$outp != "no_output"} {
        if {[isscilabbusy 5]} {return}
    }
    set delbpcomm ""
    if {$funnames != ""} {
        foreach fun $funnames {
            set delbpcomm [concat $delbpcomm "delbpt(\"$fun\");"]
        }
        foreach textarea $listoftextarea {
            set delbpcomm [concat $delbpcomm [removescilabbuffer_bp "no_output" $textarea]]
        }
        if {$outp != "no_output"} {
            ScilabEval_lt "$delbpcomm" "seq"
        }
    } else {
        # <TODO> .sce case
    }
    return $delbpcomm
}

proc removescilabbuffer_bp {outp textarea} {
# if $outp != "no_output", remove the breakpoints set in Scilab
# that are related to / initiated by buffer $textarea
# otherwise just return the command that would do that
    global funnames
    # note: check on Scilab busy flag must have been done by the calling proc
    set delbpcomm ""
    if {$funnames != ""} {
        set tagranges [$textarea tag ranges breakpoint]
        foreach {tstart tstop} $tagranges {
            set infun [whichfun [$textarea index $tstart] $textarea]
            if {$infun !={} } {
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                if {[lsearch $funnames $funname] == -1} {
                    set delbpcomm [concat $delbpcomm "delbpt(\"$funname\",$lineinfun);"]
                }
            }
        }
        if {$outp != "no_output"} {
            ScilabEval_lt "$delbpcomm" "seq"
        }
    } else {
        # <TODO> .sce case
    }
    return $delbpcomm
}

proc reshape_bp {} {
    set textareacur [gettextareacur]
    # remove breakpoints on lines that now contain no executable code
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {tstart tstop} $tagranges {
        if {[isnocodeline $tstart]} {
            $textareacur tag remove breakpoint $tstart $tstop
        }
    }
    # refresh breakpoint tags (needed after line editings)
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {tstart tstop} $tagranges {
        $textareacur tag remove breakpoint $tstart $tstop
        $textareacur tag add breakpoint "$tstart linestart" "$tstart lineend"
    }
    # refresh active breakpoint tags (needed after line editings)
    set acttagranges [$textareacur tag ranges activebreakpoint]
    foreach {tstart tstop} $acttagranges {
        $textareacur tag remove activebreakpoint $tstart $tstop
        $textareacur tag add activebreakpoint "$tstart linestart" "$tstart lineend"
    }
}
