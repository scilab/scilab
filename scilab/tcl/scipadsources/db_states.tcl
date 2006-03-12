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
    global pad watch watchwinicons watchwinstepicons
    global Shift_F8 Shift_F12
    global MenuEntryId
global dev_debug

    set errmess "Unknown debugstate in proc setdbmenuentriesstates_bp: please report"

    set dm $pad.filemenu.debug
    set dms $pad.filemenu.debug.step

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
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &into"]) -state disabled
        pbind all $Shift_F8 {}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step o&ver"]) -state disabled
        bind all <F8> {}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &out"]) -state disabled
        bind all <Control-F8> {}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Step by step"]) -state normal
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &into"]) -state normal
        pbind all $Shift_F8 {stepbystepinto_bp}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step o&ver"]) -state disabled
        bind all <F8> {}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &out"]) -state disabled
        bind all <Control-F8> {}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Step by step"]) -state normal
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &into"]) -state normal
        pbind all $Shift_F8 {stepbystepinto_bp}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step o&ver"]) -state disabled
        bind all <F8> {}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &out"]) -state disabled
        bind all <Control-F8> {}
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

    if {[info exists watchwinicons] && [info exists watchwinstepicons]} {
        if {[winfo exists $watch]} {
            set wi $watchwinicons
            set wis $watchwinstepicons
            if {[getdbstate] == "NoDebug"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state disabled
            } elseif {[getdbstate] == "ReadyForDebug"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state disabled
if {$dev_debug=="true"} {
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
} else {[lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled}
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state normal
            } elseif {[getdbstate] == "DebugInProgress"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state disabled
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
    ScilabEval_lt "db_funcprotscheme=funcprot();funcprot(0)" "seq"
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/FormatStringsForDebugWatch.sci\"" "seq"
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/FormatWhereForDebugWatch.sci\"" "seq"
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/ext_exists.sci\"" "seq"
    ScilabEval_lt "funcprot(db_funcprotscheme)" "seq"
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
# check if debug session is over
# if it is, remove in Scilab the breakpoints set by the user and set the
# debug state accordingly
# if it is not, set in Scilab the breakpoints set by the user (useful for
# step by step mode, where all breakpoints were removed just before the call
# to checkendofdebug_bp
    global setbptonreallybreakpointedlinescmd

    set removecomm [duplicatechars [removescilab_bp "no_output"] "\""]
    regsub -all {\"\"} $removecomm "\\\"\"" removecomm
    regsub -all {\?} $removecomm "\\\?" removecomm
    regsub -all {\$} $removecomm "\\\$" removecomm

    set cmd [duplicatechars $setbptonreallybreakpointedlinescmd "\""]
    regsub -all {\"\"} $cmd "\\\"\"" cmd
    regsub -all {\?} $cmd "\\\?" cmd
    regsub -all {\$} $cmd "\\\$" cmd

    set comm1 "\[db_l,db_m\]=where();"
    set comm2 "if size(db_l,1)==1 then"
    set comm3 "TCL_EvalStr(\"ScilabEval_lt \"\"$removecomm\"\"  \"\"seq\"\" \",\"scipad\");"
    set comm4 "TCL_EvalStr(\"setdbstate \"\"ReadyForDebug\"\" \",\"scipad\");"
    set comm5 "else"
    set comm6 "TCL_EvalStr(\"ScilabEval_lt \"\"$cmd\"\"  \"\"seq\"\" \",\"scipad\");"
    set comm7 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7]

    ScilabEval_lt "$fullcomm" "seq"
}
