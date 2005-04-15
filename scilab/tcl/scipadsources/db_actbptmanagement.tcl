proc updateactivebreakpoint { {itemno 3} } {
    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "if size(db_l,1)>=$itemno then"
    set comm3 "TCL_EvalStr(\"scipad eval {updateactivebreakpointtag  \"+string(db_l($itemno))+\" \"+string(db_m($itemno))+\"}\");"
    set comm4 "else"
    set comm5 "TCL_EvalStr(\"scipad eval {updateactivebreakpointtag 0 \"\"\"\"}\");"
    set comm6 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6]
    ScilabEval "$fullcomm" "seq"
}

proc updateactivebreakpointtag {{activeline -1} {activemacro -1} {updatecursorpos "No"}} {
    global listoftextarea pad
    set textareafun ""
    foreach textarea $listoftextarea {
        $textarea tag remove activebreakpoint 1.0 end
        set nextfun [$textarea search -exact -forwards -regexp\
                     "\\mfunction\\M" 1.0 end ]
        while {$nextfun != ""} {
            while {[lsearch [$textarea tag names $nextfun] "textquoted"] != -1 || \
                   [lsearch [$textarea tag names $nextfun] "rem2"] != -1 } {
                set nextfun [$textarea search -exact -forwards -regexp\
                             "\\mfunction\\M" "$nextfun +8c" end]
                if {$nextfun == ""} {break}
            }
            if {$nextfun != ""} {
                set infun [whichfun [$textarea index "$nextfun +1l"] $textarea]
                set funname [lindex $infun 0]
                if {$funname == $activemacro} {
                    set textareafun $textarea
                    set funstart [lindex $infun 3]
                    break
                }
                set nextfun [$textarea search -exact -forwards -regexp\
                             "\\mfunction\\M" "$nextfun +8c" end ]
                if {$nextfun == ""} {break}
            }
        }
    }
    if {$textareafun != "" && $activeline > 0} {
        set windmenuindex [expr [lsearch $listoftextarea $textareafun] + 1]
        $pad.filemenu.wind invoke $windmenuindex
        set infun [whichfun [$textareafun index "$funstart + 1c"] $textareafun]
        set offset 0
        # <TODO> This while loop could be improved (proc whichfun takes time to execute)
        # Its purpose is to make the line number in the buffer correspond to $activeline
        # reported by where() in Scilab (see proc updateactivebreakpoint)
        while {[lindex $infun 0] == $activemacro && [lindex $infun 1] != $activeline} {
            incr offset
            set infun [whichfun [$textareafun index "$funstart + $offset l"] $textareafun]
        }
        if {[lindex $infun 0] == $activemacro} {
            set actline [expr $funstart + $offset ]
            $textareafun tag add activebreakpoint "$actline linestart" "$actline lineend"
            $textareafun see $actline
            if {$updatecursorpos != "No"} {
                $textareafun mark set insert "$funstart + $offset l"
            }
        }
    }
}

proc removeallactive_bp {} {
    global listoftextarea
    foreach textarea $listoftextarea {
        $textarea tag remove activebreakpoint 0.0 end
    }
}

