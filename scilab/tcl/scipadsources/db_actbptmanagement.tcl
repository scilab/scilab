proc updateactivebreakpoint {} {
    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "TK_EvalStr(\"scipad eval {updateactivebreakpointtag  \"+string(db_l(2))+\" \"+string(db_m(2))+\"}\");"
    set fullcomm [concat $comm1 $comm2]
    ScilabEval "     $fullcomm"  "seq"
#    while {[checkscilabbusy "nomessage"] == "busy"} {}
}

proc updateactivebreakpointtag {{activeline -1} {activemacro -1}} {
    global listoftextarea pad
    if {$activeline == -1} {return}
    set textareafun ""
    foreach textarea $listoftextarea {
        $textarea tag remove activebreakpoint 0.0 end
        set nextfun [$textarea search -exact -forwards -regexp\
                     "\\mfunction\\M" 0.0 end ]
        while {$nextfun != ""} {
            while {[lsearch [$textarea tag names $nextfun] "textquoted"] != -1 || \
                   [lsearch [$textarea tag names $nextfun] "rem2"] != -1 } {
                set nextfun [$textarea search -exact -forwards -regexp\
                             "\\mfunction\\M" "$nextfun +8c" end]
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
            }
        }
    }
    if {$textareafun != ""} {
        set windmenuindex [expr [lsearch $listoftextarea $textareafun] + 1]
        $pad.filemenu.wind invoke $windmenuindex
        set infun [whichfun [$textareafun index "$funstart +1l"] $textareafun]
        set offset 1
        # <TODO> This while loop could be improved (proc whichfun takes time to execute)
        # Its purpose is to make the line number in the buffer correspond to $activeline
        # reported by where() in Scilab
        while {[lindex $infun 1] != $activeline} {
            incr offset
            set infun [whichfun [$textareafun index "$funstart + $offset l"] $textareafun]
        }
        set actline [expr $funstart + $offset ]
        $textareafun tag add activebreakpoint "$actline linestart" "$actline lineend"
        $textareafun see $actline
    }
}
