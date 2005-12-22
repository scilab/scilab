####################################################################
#          Basic state machine for the Scipad debugger             #
####################################################################
#
# -->-|
#     |
# ---------
# |NoDebug|---->---- insertremove_bp  |
# ---------          removeall_bp     |
# | \ \    \         showwatch_bp     |
# |  \ \    \               |
# |   \ \    \----<---------|
# |    \ \
# |     \ \ configurefoo_bp(OK)---------------
# |      \ \-------->----------|ReadyForDebug|-->-- insertremove_bp  |
# |       \  canceldebug_bp    ---------------      removeall_bp     |
# |        \--------<----------| /   /\             configurefoo_bp  |
# |                             /   /  \            showwatch_bp     |
# |canceldebug_bp   <enddebug> /   /    \                 |
# |                |goonwo_bp /   /      \                |
# |                          /   /        \-------<-------|
# |               |---->----/   /
# |   -----------------        /tonextbreakpoint_bp
# |-<-|DebugInProgress|---<---/|runtocursor_bp
#     -----------------
#        \     \
#         \     \
#          \     \-->-- insertremove_bp     |
#           \           removeall_bp        |
#            \          tonextbreakpoint_bp |
#             \         showwatch_bp        |
#              \        runtocursor_bp      |
#               \       break_bp
#                \              |
#                 \----<--------|
#
####################################################################

proc setdbstate {state} {
    global debugstate
    if {$state == "ReadyForDebug"} {
        if {[info exists debugstate]} {
            if {$debugstate == "NoDebug"} {
                getdebuggersciancillaries_bp
            }
        }
    }
    if {$state == "NoDebug" || $state == "ReadyForDebug"} {
        removeallactive_bp
    }
    # Before this point, $state is the new debug state, and $debugstate is the old one
    set debugstate $state
    setdbmenuentriesstates_bp
    setdbstatevisualhints_bp
}

proc getdbstate {} {
    global debugstate
    return $debugstate
}

proc setdbmenuentriesstates_bp {} {
    global pad watch watchwinicons
    global Shift_F12
    global MenuEntryId
global dev_debug

    set errmess "Unknown debugstate in proc setdbmenuentriesstates_bp: please report"

    set dm $pad.filemenu.debug
    if {[getdbstate] == "NoDebug"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Insert/Remove breakpoint"]) -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Remove &all breakpoints"]) -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Configure execution..."]) -state normal
        bind all <F10> {configurefoo_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Go to next b&reakpoint"]) -state disabled
        bind all <F11> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Step by step"]) -state disabled
        bind all <F8> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state disabled
        bind all <Control-F11> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"]) -state disabled
        pbind all $Shift_F12 {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Show &watch"]) -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Break"]) -state disabled
        bind all <F12> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Cance&l debug"]) -state disabled

    } elseif {[getdbstate] == "ReadyForDebug"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Insert/Remove breakpoint"]) -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Remove &all breakpoints"]) -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Configure execution..."]) -state normal
        bind all <F10> {configurefoo_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Go to next b&reakpoint"]) -state normal
        bind all <F11> {tonextbreakpoint_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Step by step"]) -state disabled
        bind all <F8> {}
if {$dev_debug=="true"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state normal
        bind all <Control-F11> {runtocursor_bp}
} else {
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state disabled
        bind all <Control-F11> {}
}
        $dm entryconfigure $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"]) -state disabled
        pbind all $Shift_F12 {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Show &watch"]) -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Break"]) -state disabled
        bind all <F12> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Cance&l debug"]) -state normal

    } elseif {[getdbstate] == "DebugInProgress"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Insert/Remove breakpoint"]) -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Remove &all breakpoints"]) -state normal
        bind all <Control-F9> {removeall_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Configure execution..."]) -state disabled
        bind all <F10> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Go to next b&reakpoint"]) -state normal
        bind all <F11> {tonextbreakpoint_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Step by step"]) -state disabled
#        bind all <F8> {stepbystep_bp}
        bind all <F8> {}
if {$dev_debug=="true"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state normal
        bind all <Control-F11> {runtocursor_bp}
} else {
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state disabled
        bind all <Control-F11> {}
}
        $dm entryconfigure $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"]) -state normal
        pbind all $Shift_F12 {goonwo_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Show &watch"]) -state normal
        bind all <Control-F12> {showwatch_bp}
if {$dev_debug=="true"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Break"]) -state normal
        bind all <F12> {break_bp}
} else {
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Break"]) -state disabled
        bind all <F12> {}
}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Cance&l debug"]) -state normal

    } else {
        tk_messageBox -message $errmess
    }

    if {[info exists watchwinicons]} {
        if {[winfo exists $watch]} {
            set wi $watchwinicons
            if {[getdbstate] == "NoDebug"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state disabled
            } elseif {[getdbstate] == "ReadyForDebug"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
if {$dev_debug=="true"} {
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
} else {[lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled}
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state normal
            } elseif {[getdbstate] == "DebugInProgress"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
if {$dev_debug=="true"} {
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
} else {[lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled}
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state normal
if {$dev_debug=="true"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state normal
} else {[lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled}
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state normal
            } else {
                tk_messageBox -message $errmess
            }
        }
    }

}

proc getdebuggersciancillaries_bp {} {
    global env
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/FormatStringsForDebugWatch.sci\""
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/FormatWhereForDebugWatch.sci\""
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/ext_exists.sci\""
}

proc setdbstatevisualhints_bp {} {
    global pad
    global colormen
    if {[getdbstate] == "NoDebug"} {
        $pad.statusmes configure -background $colormen
        showinfo [mc "Currently no debug session"]
    } elseif {[getdbstate] == "ReadyForDebug"} {
        $pad.statusmes configure -background SpringGreen
        showinfo [mc "Ready to start debug"]
    } elseif {[getdbstate] == "DebugInProgress"} {
        $pad.statusmes configure -background tomato3
        showinfo [mc "Debug in progress"]
    }
    updatedebugstateindicator_bp
}

proc updatedebugstateindicator_bp {} {
    global watch debugstateindicator pad
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            $debugstateindicator configure -background [$pad.statusmes cget -background]
        }
    }
}

proc checkendofdebug_bp {} {
    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "if size(db_l,1)==1 then"
    set comm3 "TCL_EvalStr(\"scipad eval {setdbstate \"\"ReadyForDebug\"\" }\");"
    set comm4 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4]
    ScilabEval_lt "$fullcomm" "seq"
}
