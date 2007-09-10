proc insertremove_bp {{buf "current"}} {
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    $textarea see insert
    if {[getdbstate] == "DebugInProgress"} {
        insertremovedebug_bp $textarea
    } elseif {![isnocodeline $textarea insert]} {
        set i1 [getrealstartofcontline $textarea "insert linestart"]
        set i2 [$textarea index "$i1 lineend"]
        # the following is to prevent from breakpointing the function
        # definition line (would need to say later setbpt("foo",0)
        # which is forbidden)
        set canbreakpoint true
        set infun [whichfun [$textarea index "$i1+1c"] $textarea]
        if {$infun != {}} {
            set lineinfun [lindex $infun 1]
            if {$lineinfun == 1} {
                set canbreakpoint false
            }
        }
        if {$canbreakpoint} {
            set activetags [$textarea tag names $i1]
            if {[lsearch $activetags breakpoint] == -1} {
                $textarea tag add breakpoint $i1 $i2
            } else {
                $textarea tag remove breakpoint $i1 $i2
            }
            updatebptcomplexityindicators_bp
        } else {
            showinfo [mc "No breakpoint here!"]
        }
    } else {
        showinfo [mc "No breakpoint here!"]
    }
}

proc insertremovedebug_bp {textarea} {
    global setbptonreallybreakpointedlinescmd
    if {[isscilabbusy 5]} {return}
    set i1 [getrealstartofcontline $textarea "insert linestart"]
    set i2 [$textarea index "$i1 lineend"]
    if {![isnocodeline $textarea $i1] &&
        [lsearch [$textarea tag names $i1] "db_wrapper"] == -1} {
        set infun [whichfun [$textarea index $i1] $textarea]
        if {$infun != {} } {
            # during debug, because .sce files debug uses a wrapper function,
            # and because no buffer modification is allowed, $infun is almost
            # always non empty and contains valid function data - the only case
            # where this is wrong is when the user tries to breakpoint the
            # function declaration line
            set activetags [$textarea tag names $i1]
            if {[lsearch $activetags breakpoint] == -1} {
                $textarea tag add breakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr {[lindex $infun 1] - 1}]
                set setbpcomm "setbpt(\"$funname\",$lineinfun);"
                append setbptonreallybreakpointedlinescmd $setbpcomm
                ScilabEval_lt $setbpcomm "seq"
            } else {
                $textarea tag remove breakpoint $i1 $i2
                set funname [lindex $infun 0]
                set lineinfun [expr {[lindex $infun 1] - 1}]
                set delbpcomm "delbpt(\"$funname\",$lineinfun);"
                append setbptonreallybreakpointedlinescmd $delbpcomm
                ScilabEval_lt $delbpcomm  "seq"
            }
            updatebptcomplexityindicators_bp
        } else {
            showinfo [mc "No breakpoint here!"]
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
    foreach textarea [filteroutpeers $listoftextarea] {
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
# <TODO>: couldn't this proc just output a single "delbpt"?
#         in the past it was not possible to delete all breakpoints
#         with this single command, but now it is!
    global funnames listoftextarea
    if {$outp != "no_output"} {
        if {[isscilabbusy 5]} {return}
    }
    set delbpcomm ""
    if {$funnames != ""} {
        foreach fun $funnames {
            set delbpcomm [concat $delbpcomm "delbpt(\"$fun\");"]
        }
        foreach textarea [filteroutpeers $listoftextarea] {
            set delbpcomm [concat $delbpcomm [removescilabbuffer_bp "no_output" $textarea]]
        }
        if {$outp != "no_output"} {
            ScilabEval_lt "$delbpcomm" "seq"
        }
    } else {
        # <TODO> .sce case if some day the parser uses pseudocode noops
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
                set lineinfun [expr {[lindex $infun 1] - 1}]
                if {[lsearch $funnames $funname] == -1} {
                    set delbpcomm [concat $delbpcomm "delbpt(\"$funname\",$lineinfun);"]
                }
            }
        }
        if {$outp != "no_output"} {
            ScilabEval_lt "$delbpcomm" "seq"
        }
    } else {
        # <TODO> .sce case if some day the parser uses pseudocode noops
    }
    return $delbpcomm
}

proc reshape_bp {} {
    set textareacur [gettextareacur]
    # remove breakpoints on lines that now contain no executable code
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {tstart tstop} $tagranges {
        if {[isnocodeline $textareacur $tstart]} {
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
    # recount the breakpoints for display in the watch window
    updatewatch_bp
}

proc countallbreakpointedlines {} {
# count the number of breakpointed lines in all the opened buffers
    global listoftextarea
    set N 0
    foreach ta [filteroutpeers $listoftextarea] {
        incr N [llength [$ta tag ranges breakpoint]]
    }
    # divide by 2 since $ta tag ranges returns 2 elements for each breakpoint
    return [expr {$N / 2}]
}

proc countallbreakpointedmacros {} {
# look in all the opened buffers and count the number of macros that
# contain breakpointed lines
    global listoftextarea
    set N 0
    foreach ta [filteroutpeers $listoftextarea] {
        # to take into account functions of the same name defined in
        # different buffers, macrlst is initialized for each buffer
        set macrlst [list ]
        foreach {i j} [$ta tag ranges breakpoint] {
            # use position $j rather than $i to avoid problems when the
            # function declaration line is breakpointed
            set funname [lindex [whichfun "$j - 1c" $ta] 0]
            if {[lsearch -exact $macrlst $funname] == -1} {
                incr N
                lappend macrlst $funname
            }
        }
    }
    return $N
}

proc getreallybptedlines {fnam} {
# parse $setbptonreallybreakpointedlinescmd to extract the list of all the
# breakpointed lines of function named $fnam
# all the occurrences of $fnam in $setbptonreallybreakpointedlinescmd are
# taken into account, not just the first one
# the output list of breakpointed lines is ordered in increasing order
# an empty list is returned if $fnam does not have any breakpoint
    global setbptonreallybreakpointedlinescmd
    set bptlineslist [list ]
    set str [string map {"setbpt(" ""} $setbptonreallybreakpointedlinescmd]
    set str [string map {");" " "} $str]
    set list1 [split [string trim $str] " "]
    # at this point $list1 is something like {"foo1",n1} {"foo2",n2} ...
    foreach item $list1 {
        # get breakpointed function name
        set fun [lindex [split $item "\""] 1]
        if {$fun != $fnam} {
            continue
        }
        # in $setbptonreallybreakpointedlinescmd, we can only have one
        # breakpointed line per statement, i.e. only "foo1",n1 and never
        # a range of lines such as "foo1",n1:n2
        set lin [lindex [split $item ","] 1]
        lappend bptlineslist $lin
    }
    return [lsort -integer $bptlineslist]
}
