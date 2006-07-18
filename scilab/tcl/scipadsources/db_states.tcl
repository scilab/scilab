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
# |                             /   /  \            showwatch_bp
# |canceldebug_bp   <enddebug> /   /    \                 |
# |                |goonwo_bp /   /      \                |
# |                          /   /        \-------<-------|
# |               |---->----/   /
# |   -----------------        /tonextbreakpoint_bp |
# |-<-|DebugInProgress|---<---/ runtocursor_bp      |
#     -----------------         stepbystepinto_bp   |
#        \     \                stepbystepover_bp   |
#         \     \               stepbystepout_bp
#          \     \
#           \     \-->-- insertremove_bp     |
#            \           removeall_bp        |
#             \          tonextbreakpoint_bp |
#              \         stepbystepinto_bp   |
#               \        stepbystepover_bp   |
#                \       stepbystepout_bp    |
#                 \      showwatch_bp        |
#                  \     runtocursor_bp      |
#                   \    break_bp
#                    \           |
#                     \----<-----|
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
    # when entering the debug mode, the Scilab shell must be at main level
    # this is safer wrt ScilabEval, and is required for FormatWhereForDebugWatch
    if {$state == "DebugInProgress"} {
        ScilabEval_lt "abort" "seq"
    }
    # Before this point, $state is the new debug state, and $debugstate is the old one
    set debugstate $state
    setdbmenuentriesstates_bp
    setdbstatevisualhints_bp
    updatebptcomplexityindicators_bp
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
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "S&cilab"]) -state normal
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&XML"]) -state normal
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&none"]) -state normal
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&Colorize"]) -state normal

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
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step o&ver"]) -state normal
        bind all <F8> {stepbystepover_bp}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &out"]) -state normal
        bind all <Control-F8> {stepbystepout_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state normal
        bind all <Control-F11> {runtocursor_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"]) -state disabled
        pbind all $Shift_F12 {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Show &watch"]) -state normal
        bind all <Control-F12> {showwatch_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Break"]) -state disabled
        bind all <F12> {}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Cance&l debug"]) -state normal
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&Colorize"]) -state disabled
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "S&cilab"]) -state disabled
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&XML"]) -state disabled
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&none"]) -state disabled

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
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step o&ver"]) -state normal
        bind all <F8> {stepbystepover_bp}
        $dms entryconfigure $MenuEntryId($dms.[mcra "Step &out"]) -state normal
        bind all <Control-F8> {stepbystepout_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state normal
        bind all <Control-F11> {runtocursor_bp}
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
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "S&cilab"]) -state disabled
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&XML"]) -state disabled
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&none"]) -state disabled
        $pad.filemenu.scheme entryconfigure $MenuEntryId($pad.filemenu.scheme.[mcra "&Colorize"]) -state disabled

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
                [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state normal
            } elseif {[getdbstate] == "DebugInProgress"} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state normal
                [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state normal
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
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
    # Currently localization in .sci files is only used by the debugger, therefore
    # getf LocalizeForScipad.sci is safely done here
    ScilabEval_lt "getf \"$env(SCIPATH)/tcl/scipadsources/LocalizeForScipad.sci\"" "seq"
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

proc managewatchontop_bp {} {
# tell the windows manager to keep or not the watch window on top
# of the other windows
#   Windows: use wm attributes -topmost
#   Linux:   use wm transient if Tk version is before 8.5
#            use wm topmost if Tk version is 8.5 or above
# TIP #231 implements wm topmost for X11 environments and is included in
# Tk starting from Tcl/Tk8.5a4
# wm transient seems to work equally well on Windows, but since
# there is -topmost for win, let's use the latter
    global watch watchalwaysontop tcl_platform pad
    global Tk85
    if {[info exists watch]} {
        if {$watchalwaysontop} {
            if {$tcl_platform(platform) == "windows"} {
                wm attributes $watch -topmost 1
            } else {
                # Linux case
                if {$Tk85} {
                    wm attributes $watch -topmost 1
                } else {
                    # the following is minimal but could be enough
                    # more is difficult, see http://wiki.tcl.tk/3014
                    wm transient $watch $pad
                }
            }
        } else {
            if {$tcl_platform(platform) == "windows"} {
                wm attributes $watch -topmost 0
            } else {
                if {$Tk85} {
                    wm attributes $watch -topmost 0
                } else {
                    wm transient $watch
                }
            }
        }
    }
}

proc checkendofdebug_bp {{stepmode "nostep"}} {
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

    # command to skip lines with no executable code (for step by step)
    # stepbystepinto_bp command is just rerun, with Scilab busy checks disabled
    # note that since checkendofdebug_bp is called at the end of each step, the
    # command below is an if and not a while
    # same principle is used for skipping breakpoints when running to cursor
    # note also that in order to stack commands with the right order, this must
    # be a ScilabEval_lt(Tcl_EvalStr(ScilabEval_lt(TCL_EvalStr ...) seq) seq)
    switch -- $stepmode {
        "nostep"   {
            # no need to define code for skipping no code lines since
            # stops always occur on existing breakpoints set by the user
            set skipline ""
                   }
        "into"     {
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} {stepbystepover_bp 0 0}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "over"     {
            # stepbystepover_bp 0 1, i.e. with rescanbuffers set to true,
            # because Scipad must rescan the buffer when leaving it on
            # step over - this is required to prevent Scipad to skip
            # nested libfuns contructs
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} {stepbystepover_bp 0 1}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "out"      {
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} {stepbystepover_bp 0 0}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "runtocur" {
            set skipline1 "TCL_EvalStr(\\\"\"if {!\\\[iscursorplace_bp\\\]} {runtocursor_bp 0 1}\\\"\",\\\"\"scipad\\\"\");"
            set skipline2 "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} {stepbystepover_bp 0 0}\\\"\",\\\"\"scipad\\\"\");"
            set skipline [concat $skipline1 $skipline2]
                   }
    }

    set comm1  "\[db_l,db_m\]=where();"
    set comm2  "if size(db_l,1)==1 then"
    set comm3    "TCL_EvalStr(\"ScilabEval_lt \"\"$removecomm\"\"  \"\"seq\"\" \",\"scipad\");"
    set comm4    "TCL_EvalStr(\"setdbstate \"\"ReadyForDebug\"\" \",\"scipad\");"
    set comm5    "TCL_EvalStr(\"scedebugcleanup_bp\",\"scipad\");"
    set comm6    "TCL_EvalStr(\"checkexecutionerror_bp\",\"scipad\");"
    set comm7  "else"
    set comm8    "TCL_EvalStr(\"ScilabEval_lt \"\"$cmd\"\"  \"\"seq\"\" \",\"scipad\");"
    set comm9    "TCL_EvalStr(\"ScilabEval_lt \"\"$skipline\"\"  \"\"seq\"\" \",\"scipad\");"
#    set comm10 "end;TCL_EvalStr(\"hidewrappercode\",\"scipad\");"
    set comm10 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8 $comm9 $comm10]

    ScilabEval_lt "$fullcomm" "seq"
}

proc checkexecutionerror_bp {} {
# detect whether there was an execution error or not in Scilab
# since the previous call to this proc
# this proc makes use of lasterror(%f), i.e. without clearing the error.
# in case an error occurred previously ($errnum is non zero), display
# the error information in the call stack area of the watch window
# and blink the offending line
    ScilabEval_lt "\[db_str,db_n,db_l,db_func\]=lasterror(%f);\
                   TCL_EvalStr(\"global errnum errline errmsg errfunc callstackcontent; \
                                 set errnum  \"+string(db_n)+\"; \
                                 set errline \"+string(db_l)+\"; \
                                 set errfunc \"\"\"+strsubst(db_func,\"\"\"\",\"\\\"\"\")+\"\"\"; \
                                 set errmsg  \"\"\"+stripblanks(db_str)+\"\"\"; \
                                 if {\$errnum != 0} { \
                                     bell; \
                                     set errtext \[mc \"\"Error \"\"\]; \
                                     append errtext \$errnum \"\" -- << \"\" \$errmsg \"\" >>\n\"\" \
                                                    \[mc \"\"at line \"\"\] \$errline \[mc \"\" of \"\"\] \
                                                     \$errfunc \"\"\n\"\" \"\"\n\"\" \
                                                    \[mc \"\"Scilab is back at the main level now.\"\"\]; \
                                     set callstackcontent \$errtext; \
                                     updatewatch_bp; \
                                     blinkline \$errline \$errfunc; \
                                 } ; \
                               \" , \"scipad\" )" \
                  "seq"
}

proc clearscilaberror {} {
# send lasterror(%t), which has the effect of clearing the last
# error in Scilab. In Scilab the lasterror output becomes:
#
#    -->[str,n,line,func]=lasterror(%f)
#     func  =
#    
#    
#     line  =
#    
#        0.
#     n  =
#    
#        0.
#     str  =
#    
#         []
#
    if {[isscilabbusy 5]} {return}
    ScilabEval_lt "lasterror(%t)" "seq"
}
