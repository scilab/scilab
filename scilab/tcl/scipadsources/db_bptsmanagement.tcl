proc insertremove_bp {{buf "current"}} {
    if {[getdbstate] == "DebugInProgress"} {
        insertremovedebug_bp $buf
    } else {
        if {$buf == "current"} {
            set textarea [gettextareacur]
        } else {
            set textarea $buf
        }
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

proc insertremovedebug_bp {{buf "current"}} {
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
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
                ScilabEval $setbpcomm  "seq"
            } else {
                # <TODO> .sce case
            }
        } else {
            # There was bug(s) in scilab routines/interf/matsys.f file
            # on this one: in -1-> mode, setbpt("foo",linenum) worked, but
            # delbpt("foo",linenum) did not (bp was removed from dispbpt
            # list but execution still stopped at linenum)
            # The correction has been submitted to Scilab team (see
            # bugzilla #718), is part of Scilab CVS and is included
            # in Scilab 3.0. If the fix is not installed, the following
            # will fail.
            set infun [whichfun [$textarea index $i1] $textarea]
            if {$infun !={} } {
                $textarea tag remove breakpoint $i1 $i2
#                $textarea tag remove activebreakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set delbpcomm " delbpt(\"$funname\",$lineinfun);"
                ScilabEval $delbpcomm  "seq"
            } else {
                # <TODO> .sce case
            }
        }
    }
}

proc removeall_bp {} {
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
    global funnames listoftextarea
    if {[checkscilabbusy] == "OK"} {
        set delbpcomm ""
        if {$funnames != ""} {
            foreach fun $funnames {
                set delbpcomm [concat $delbpcomm "delbpt(\"$fun\");"]
            }
            foreach textarea $listoftextarea {
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
            }
            if {$outp != "no_output"} {
                ScilabEval "$delbpcomm" "seq"
            }
        } else {
            # <TODO> .sce case
        }
        return $delbpcomm
    }
}

proc removeallactive_bp {} {
    global listoftextarea
    foreach textarea $listoftextarea {
        $textarea tag remove activebreakpoint 0.0 end
    }
}

proc reshape_bp {} {
    set textareacur [gettextareacur]
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {tstart tstop} $tagranges {
        $textareacur tag remove breakpoint $tstart $tstop
        $textareacur tag add breakpoint "$tstart linestart" "$tstart lineend"
    }
}
