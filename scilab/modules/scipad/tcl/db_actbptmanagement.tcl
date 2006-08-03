proc updateactivebreakpoint { {itemno 3} } {
    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "if size(db_l,1)>=$itemno then"
    # the curly braces around string(db_m($itemno)) below are required
    # to avoid TCL_EvalStr to try to evaluate string(db_m($itemno))
    # this is useful for instance when the function name where the
    # breakpoint stop occurs starts with a dollar sign $
    set comm3 "TCL_EvalStr(\"scipad eval {updateactivebreakpointtag \"+string(db_l($itemno))+\" {\"+string(db_m($itemno))+\"} }\");"
    set comm4 "else"
    set comm5 "TCL_EvalStr(\"scipad eval {updateactivebreakpointtag 0 \"\"\"\"}\");"
    set comm6 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6]
    ScilabEval_lt "$fullcomm" "seq"
}

proc updateactivebreakpointtag {{activeline -1} {activemacro -1}} {
# Show the active breakpoint
# This is done by using proc dogotoline
# <TODO> this might fail if the same function name can be found in more
#        than one single buffer - see funnametofunnametafunstart

    # uabpt_opened is used to prevent more than one recursive call
    global uabpt_opened_a_file backgroundtasksallowed
    
    removeallactive_bp

    if {$activemacro == ""} {return}

    set funtogoto [funnametofunnametafunstart $activemacro]

    if {$funtogoto == ""} {
        if {!$uabpt_opened_a_file} {
            # in principle the breakpoint to highlight is in a libfun
            # ancillary (can happen while stepping into)
            # open the adequate file first (no background colorization,
            # so that colorization is finished when calling dogotoline
            # in the next call to updateactivebreakpointtag)
            # and try again to update the activebreakpoint tag
            # note: this must be done the way below because it makes use
            # of the queueing ScilabEval "seq" instructions
            set uabpt_opened_a_file true
            set backgroundtasksallowed false
            doopenfunsource "libfun" $activemacro
            set comm1 "TCL_EvalStr(\"updateactivebreakpointtag $activeline $activemacro;\",\"scipad\");"
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

proc removeallactive_bp {} {
    global listoftextarea
    foreach textarea $listoftextarea {
        $textarea tag remove activebreakpoint 0.0 end
    }
}

