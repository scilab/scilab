####################################################################
#          Basic state machine for the Scipad debugger             #
####################################################################
#
# -->-|
#     |
# ---------
# |NoDebug|---->---- insertremove_bp  |
# ---------          removeall_bp     |
# |   \    \         showwatch_bp     |
# |    \    \               |
# |     \    \----<---------|
# |      \
# |       \ configurefoo_bp(OK) ---------------
# |        \-------->----------|ReadyForDebug|-->-- insertremove_bp  |
# |                            ---------------      removeall_bp     |
# |                             /   / \             configurefoo_bp  |
# |canceldebug_bp   <enddebug> /   /   \            showwatch_bp     |
# |                |goonwo_bp /   /     \                 |
# |                          /   /       \-------<--------|
# |               |---->----/   /
# |   -----------------        /tonextbreakpoint_bp
# |-<-|DebugInProgress|---<---/
#     -----------------
#        \     \
#         \     \
#          \     \-->-- insertremove_bp     |
#           \           removeall_bp        |
#            \          tonextbreakpoint_bp |
#             \         showwatch_bp        |
#              \              |
#               \----<--------|
####################################################################

proc setdbstate {state} {
    global debugstate
    if {$state == "NoDebug"} {
        removeallactive_bp
    }
    if {$state == "ReadyForDebug"} {
        if {$debugstate == "NoDebug"} {
            getdebuggersciancillaries_bp
        }
        removeallactive_bp
    }
    # Before this point, $state is the new debug state, and $debugstate is the old one
    set debugstate $state
    setdbmenuentriesstates_bp
    setdbstatevisualhint_bp
}

proc getdbstate {} {
    global debugstate
    return $debugstate
}

proc setdbmenuentriesstates_bp {} {
    global pad watch watchwinicons

    set errmess "Unknown debugstate in proc setdbmenuentriesstates_bp: please report"

    set dm $pad.filemenu.debug
    if {[getdbstate] == "NoDebug"} {
        $dm entryconfigure 1 -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure 2 -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure 4 -state normal
        bind all <F10> {configurefoo_bp}
        $dm entryconfigure 6 -state disabled
        bind all <F11> {}
        $dm entryconfigure 7 -state disabled
        bind all <F12> {}
        $dm entryconfigure 8 -state disabled
        bind all <Shift-F12> {}
        $dm entryconfigure 10 -state disabled
        bind all <Control-F12> {}
        $dm entryconfigure 12 -state disabled

    } elseif {[getdbstate] == "ReadyForDebug"} {
        $dm entryconfigure 1 -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure 2 -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure 4 -state normal
        bind all <F10> {configurefoo_bp}
        $dm entryconfigure 6 -state normal
        bind all <F11> {tonextbreakpoint_bp}
        $dm entryconfigure 7 -state disabled
        bind all <F12> {}
        $dm entryconfigure 8 -state disabled
        bind all <Shift-F12> {}
        $dm entryconfigure 10 -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure 12 -state disabled

    } elseif {[getdbstate] == "DebugInProgress"} {
        $dm entryconfigure 1 -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure 2 -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure 4 -state disabled
        bind all <F10> {}
        $dm entryconfigure 6 -state normal
        bind all <F11> {tonextbreakpoint_bp}
        $dm entryconfigure 7 -state disabled
#        bind all <F12> {stepbystep_bp}
        bind all <F12> {}
        $dm entryconfigure 8 -state normal
        bind all <Shift-F12> {goonwo_bp}
        $dm entryconfigure 10 -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure 12 -state normal

    } else {
        tk_messageBox -message $errmess
    }

    if {[info exists watchwinicons]} {
        if {[winfo exists $watch]} {
            set wi $watchwinicons
            if {[getdbstate] == "NoDebug"} {
                [lindex $wi 4] configure -state normal
                [lindex $wi 6] configure -state disabled
                [lindex $wi 8] configure -state disabled
                [lindex $wi 12] configure -state disabled
            } elseif {[getdbstate] == "ReadyForDebug"} {
                [lindex $wi 4] configure -state normal
                [lindex $wi 6] configure -state normal
                [lindex $wi 8] configure -state disabled
                [lindex $wi 12] configure -state disabled
            } elseif {[getdbstate] == "DebugInProgress"} {
                [lindex $wi 4] configure -state disabled
                [lindex $wi 6] configure -state normal
                [lindex $wi 8] configure -state normal
                [lindex $wi 12] configure -state normal
            } else {
                tk_messageBox -message $errmess
            }
        }
    }

}

proc getdebuggersciancillaries_bp {} {
    global env
    ScilabEval "getf \"$env(SCIPATH)/tcl/scipadsources/FormatStringsForDebugWatch.sci\""
    ScilabEval "getf \"$env(SCIPATH)/tcl/scipadsources/FormatWhereForDebugWatch.sci\""
}

proc setdbstatevisualhint_bp {} {
    global pad
    global lang colormen
    if {[getdbstate] == "NoDebug"} {
        $pad.statusmes configure -background $colormen
        if {$lang == "eng"} {
            showinfo "Currently no debug session"
        } else {
            showinfo "Pas de session de débug en cours"
        }
    } elseif {[getdbstate] == "ReadyForDebug"} {
        $pad.statusmes configure -background SpringGreen
        if {$lang == "eng"} {
            showinfo "Ready to start debug"
        } else {
            showinfo "Prêt pour le débug"
        }
    } elseif {[getdbstate] == "DebugInProgress"} {
        $pad.statusmes configure -background tomato3
        if {$lang == "eng"} {
            showinfo "Debug in progress"
        } else {
            showinfo "Débug en cours"
        }
    }
}

proc checkendofdebug_bp {} {
    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "if size(db_l,1)==1 then"
    set comm3 "TK_EvalStr(\"scipad eval {setdbstate \"\"ReadyForDebug\"\" }\");"
    set comm4 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4]
    ScilabEval "$fullcomm" "seq"
}
