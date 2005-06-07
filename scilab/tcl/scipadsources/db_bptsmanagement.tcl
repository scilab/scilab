proc insertremove_bp {{buf "current"}} {
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    $textarea see insert
    if {[getdbstate] == "DebugInProgress"} {
        insertremovedebug_bp $textarea
    } else {
        set infun [whichfun [$textarea index "insert linestart"] $textarea]
        if {$infun !={}} {
            set i1 "insert linestart"
            set i2 "insert lineend"
            set activetags [$textarea tag names $i1]
            if {[lsearch $activetags breakpoint] == -1} {
                $textarea tag add breakpoint $i1 $i2
            } else {
                $textarea tag remove breakpoint $i1 $i2
#                $textarea tag remove activebreakpoint $i1 $i2
            }
        }
    }
}

proc insertremovedebug_bp {textarea} {
    if {[checkscilabbusy] == "OK"} {
        set i1 "insert linestart"
        set i2 "insert lineend"
        set activetags [$textarea tag names $i1]
        if {[lsearch $activetags breakpoint] == -1} {
            set infun [whichfun [$textarea index $i1] $textarea]
            if {$infun !={} } {
                $textarea tag add breakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set setbpcomm " setbpt(\"$funname\",$lineinfun);"
                ScilabEval_lt $setbpcomm "seq"
            } else {
                # <TODO> .sce case
            }
        } else {
            set infun [whichfun [$textarea index $i1] $textarea]
            if {$infun !={} } {
                $textarea tag remove breakpoint $i1 $i2
#                $textarea tag remove activebreakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set delbpcomm " delbpt(\"$funname\",$lineinfun);"
                ScilabEval_lt $delbpcomm  "seq"
            } else {
                # <TODO> .sce case
            }
        }
    }
}

proc removeall_bp {} {
# remove all breakpoint tags from the opened buffer
# possibly existing breakpoints in Scilab are not touched
    global listoftextarea
    if {[checkscilabbusy] == "OK"} {
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
}

proc removescilab_bp {outp} {
# if $outp != "no_output", remove all the breakpoints set in Scilab
# otherwise just return the command that would do that
    global funnames listoftextarea
    if {[checkscilabbusy] == "OK"} {
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
}

proc removescilabbuffer_bp {outp textarea} {
# if $outp != "no_output", remove the breakpoints set in Scilab
# that are related to / initiated by buffer $textarea
# otherwise just return the command that would do that
    global funnames
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
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {tstart tstop} $tagranges {
        $textareacur tag remove breakpoint $tstart $tstop
        $textareacur tag add breakpoint "$tstart linestart" "$tstart lineend"
    }
    set acttagranges [$textareacur tag ranges activebreakpoint]
    foreach {tstart tstop} $acttagranges {
        $textareacur tag remove activebreakpoint $tstart $tstop
        $textareacur tag add activebreakpoint "$tstart linestart" "$tstart lineend"
    }
}
