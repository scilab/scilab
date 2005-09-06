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
# This is done by fooling proc dogotoline
    global physlogic linetogo curfileorfun funtogoto
    removeallactive_bp
    if {$activemacro == ""} {return}
    if {![info exists physlogic]} {
        # go to line dialog was never opened before
        set flag "undefined"
    } else {
        # save current values so that they will stay when the user reopens the go to line dialog
        set flag "they_exist"
        set temp1 $physlogic
        set temp2 $linetogo
        set temp3 $curfileorfun
        set temp4 $funtogoto
    }
    set fundefs [getallfunsinalltextareas]
    set funtogoto ""
    foreach {ta fundefsinta} $fundefs {
        foreach {funcname funcline funstartline} $fundefsinta {
            if {$funcname == $activemacro} {
                set funtogoto [list $funcname $ta $funstartline]
                break
            }
        }
    }
    set physlogic "logical"
    set linetogo $activeline
    set curfileorfun "function"
    if {$funtogoto != ""} {
        dogotoline
        set actpos [[lindex $funtogoto 1] index insert]
        [lindex $funtogoto 1] tag add activebreakpoint "$actpos linestart" "$actpos lineend"
    }
    if {$flag == "undefined"} {
        unset physlogic linetogo curfileorfun funtogoto
    } else {
        set physlogic $temp1
        set linetogo $temp2
        set curfileorfun $temp3
        set funtogoto $temp4
    }
}

proc removeallactive_bp {} {
    global listoftextarea
    foreach textarea $listoftextarea {
        $textarea tag remove activebreakpoint 0.0 end
    }
}

