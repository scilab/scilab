proc insertremove_bp {} {
    set textareacur [gettextareacur]
    set infun [whichfun [$textareacur index insert]]
    if {$infun !={}} {
        set i1 "insert linestart"
        set i2 "insert lineend"
        set activetags [$textareacur tag names $i1]
        if {[string first breakpoint $activetags] == -1} {
            $textareacur tag add breakpoint $i1 $i2
        } else {
            $textareacur tag remove breakpoint $i1 $i2
            $textareacur tag remove activebreakpoint $i1 $i2
        }
    }
}

proc removeall_bp {} {
    set textareacur [gettextareacur]
    set saveinsert [$textareacur index insert]
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {bp_start bp_stop} $tagranges {
        $textareacur mark set insert $bp_start
        insertremove_bp
    }
    $textareacur mark set insert $saveinsert
}

proc insertremovedebug_bp {} {
    set textareacur [gettextareacur]
    if {[checkscilabbusy] == "OK"} {
        set i1 "insert linestart"
        set i2 "insert lineend"
        set activetags [$textareacur tag names $i1]
        if {[string first breakpoint $activetags] == -1} {
            $textareacur tag add breakpoint $i1 $i2
            set infun [whichfun [$textareacur index $i1]]
            if {$infun !={} } {
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set setbpcomm " setbpt(\"$funname\",$lineinfun);"
                ScilabEval $setbpcomm
            } else {
                # <TODO> .sce case
            }
        } else {
            # There was bug(s) in scilab routines/interf/matsys.f file
            # on this one: in -1-> mode, setbpt("foo",linenum) worked, but
            # delbpt("foo",linenum) did not (bp was removed from dispbpt
            # list but execution still stopped at linenum)
            # The correction has been submitted to Scilab team (see
            # bugzilla #718), is part of Scilab CVS and will be included
            # in Scilab 3.0. If the patch is not installed, the following
            # will fail.
            $textareacur tag remove breakpoint $i1 $i2
            $textareacur tag remove activebreakpoint $i1 $i2
            set infun [whichfun [$textareacur index $i1]]
            if {$infun !={} } {
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set delbpcomm " delbpt(\"$funname\",$lineinfun);"
                ScilabEval $delbpcomm
            } else {
                # <TODO> .sce case
            }
        }
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
