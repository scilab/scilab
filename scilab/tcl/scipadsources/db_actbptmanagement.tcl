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
#        than one single buffer
    removeallactive_bp
    if {$activemacro == ""} {return}
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
    if {$funtogoto != ""} {
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

