proc updateactivebreakpoint { {itemno 3} } {
    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "if size(db_l,1)>=$itemno then"
    # the curly braces around string(db_m($itemno)) below are required
    # to avoid TCL_EvalStr to try to evaluate string(db_m($itemno))
    # this is useful for instance when the function name where the
    # breakpoint stop occurs starts with a dollar sign $
    set comm3 "TCL_EvalStr(\"updateactbreakpointtag \"+string(db_l($itemno))+\" {\"+string(db_m($itemno))+\"} \",\"scipad\");"
    set comm4 "else"
    set comm5 "TCL_EvalStr(\"updateactbreakpointtag 0 \"\"\"\" \",\"scipad\");"
    set comm6 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6]
    ScilabEval_lt "$fullcomm" "seq"
}

proc updateactbreakpointtag {{activeline -1} {activemacro -1}} {
# Show the active breakpoint
# This is done by using proc dogotoline

    # uabpt_opened is used to prevent more than one recursive call
    global backgroundtasksallowed
    global uabpt_opened_a_file       ; # used only in this proc, to differentiate between the possible two successive executions
    global afilewasopenedbyuabpt     ; # used indirectly in proc checkendofdebug_bp

    removeallactive_bp

    if {$activemacro == ""} {return}

    # during the debug, funnametofunnametafunstart cannot return
    # the wrong function, there can be no duplicates
    set funtogoto [funnametofunnametafunstart $activemacro]

    if {$funtogoto == ""} {
        if {!$uabpt_opened_a_file} {
            # in principle the breakpoint to highlight is in a libfun
            # ancillary (can happen while stepping into)
            # open the adequate file first (no background colorization,
            # so that colorization is finished when calling dogotoline
            # in the next call to updateactbreakpointtag)
            # and try again to update the activebreakpoint tag
            # note: this must be done the way below because it makes use
            # of the queueing ScilabEval "seq" instructions
            set afilewasopenedbyuabpt true
            set uabpt_opened_a_file true
            set backgroundtasksallowed false
            doopenfunsource "libfun" $activemacro
            set comm1 "TCL_EvalStr(\"updateactbreakpointtag $activeline $activemacro;\",\"scipad\");"
            set comm2 "TCL_EvalStr(\"set backgroundtasksallowed true\",\"scipad\");"
            set fullcomm [concat $comm1 $comm2]
            ScilabEval_lt "$fullcomm" "seq"
        } else {
            # the function where the breakpoint has to be highlighted
            # could still not be found after trying to open it (it is
            # perhaps not a libfun after all) - should never happen,
            # but in any case do nothing
        }
    } else {
        # the function where the breakpoint has to be highlighted has
        # been found among the opened buffers, either because it was
        # already open or because the previous call to this proc
        # performed the opening
        set uabpt_opened_a_file false
        dogotoline "logical" $activeline "function" $funtogoto
        set actpos [[lindex $funtogoto 1] index insert]
        [lindex $funtogoto 1] tag add activebreakpoint "$actpos linestart" "$actpos lineend"
    }
}

proc closecurifopenedbyuabpt {} {
# as it says, this proc closes the current buffer if proc updateactbreakpointtag
# did just open a libfun - this is used in proc checkendofdebug_bp
    global afilewasopenedbyuabpt
    if {$afilewasopenedbyuabpt} {
        closecur
    }
}

proc removeallactive_bp {} {
    global listoftextarea
    foreach textarea [filteroutpeers $listoftextarea] {
        $textarea tag remove activebreakpoint 1.0 end
    }
}
