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
# |   -----------------        /execfile_bp
# |-<-|DebugInProgress|---<---/
#     -----------------
#        \     \
#         \     \
#          \     \-->-- insertremove_bp  |
#           \           removeall_bp     |
#            \          resume_bp        |
#             \         showwatch_bp     |
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
    global pad debugstate
    set dm $pad.filemenu.debug

    if {$debugstate == "NoDebug"} {
        $dm entryconfigure 1 -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure 2 -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure 4 -state normal
        bind all <F10> {configurefoo_bp}
        $dm entryconfigure 6 -state disabled
        bind all <Control-F11> {}
        $dm entryconfigure 7 -state disabled
        bind all <F11> {}
#        bind all <Shift-F11> {insertremovedebug_bp}
        $dm entryconfigure 8 -state disabled
        bind all <F12> {}
        $dm entryconfigure 9 -state disabled
        bind all <Shift-F12> {}
        $dm entryconfigure 11 -state disabled
        bind all <Control-F12> {}
        $dm entryconfigure 13 -state disabled

    } elseif {$debugstate == "ReadyForDebug"} {
        $dm entryconfigure 1 -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure 2 -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure 4 -state normal
        bind all <F10> {configurefoo_bp}
        $dm entryconfigure 6 -state normal
        bind all <Control-F11> {execfile_bp}
        $dm entryconfigure 7 -state disabled
        bind all <F11> {}
#        bind all <Shift-F11> {insertremovedebug_bp}
        $dm entryconfigure 8 -state disabled
        bind all <F12> {}
        $dm entryconfigure 9 -state disabled
        bind all <Shift-F12> {}
        $dm entryconfigure 11 -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure 13 -state disabled

    } elseif {$debugstate == "DebugInProgress"} {
        $dm entryconfigure 1 -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure 2 -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure 4 -state disabled
        bind all <F10> {}
        $dm entryconfigure 6 -state disabled
        bind all <Control-F11> {}
        $dm entryconfigure 7 -state normal
        bind all <F11> {resume_bp}
#        bind all <Shift-F11> {insertremovedebug_bp}
        $dm entryconfigure 8 -state disabled
#        bind all <F12> {stepbystep_bp}
        bind all <F12> {}
        $dm entryconfigure 9 -state normal
        bind all <Shift-F12> {goonwo_bp}
        $dm entryconfigure 11 -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure 13 -state normal

    } else {
        tk_messageBox -message "Unknown debugstate: please report"
    }
}

proc getdebuggersciancillaries_bp {} {
    global env
    ScilabEval "getf \"$env(SCIPATH)/tcl/scipadsources/FormatStringsForDebugWatch.sci\""
    ScilabEval "getf \"$env(SCIPATH)/tcl/scipadsources/FormatWhereForDebugWatch.sci\""
}

proc setdbstatevisualhint_bp {} {
    global pad debugstate
    global lang colormen
    if {$debugstate == "NoDebug"} {
        $pad.statusmes configure -background $colormen
        if {$lang == "eng"} {
            showinfo "Currently no debug session"
        } else {
            showinfo "Pas de session de débug en cours"
        }
    } elseif {$debugstate == "ReadyForDebug"} {
        $pad.statusmes configure -background SpringGreen
        if {$lang == "eng"} {
            showinfo "Ready to start debug"
        } else {
            showinfo "Prêt pour le débug"
        }
    } elseif {$debugstate == "DebugInProgress"} {
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
