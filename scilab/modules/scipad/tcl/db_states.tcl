#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#
####################################################################
#          Basic state machine for the Scipad debugger             #
####################################################################
#
# -->-|
#     |
# ---------
# |NoDebug|---->---- insertremove_bp
# ---------                 |
# | \ \    \                |
# |  \ \    \               |
# |   \ \    \----<---------|
# |    \ \
# |     \ \ configurefoo_bp(OK)---------------
# |      \ \-------->----------|ReadyForDebug|-->-- insertremove_bp  |
# |       \  canceldebug_bp    ---------------      configurefoo_bp
# |        \--------<----------| /   /\                   |
# |                             /   /  \                  |
# |canceldebug_bp   <enddebug> /   /    \                 |
# |                |goonwo_bp /   /      \                |
# |                          /   /        \-------<-------|
# |               |---->----/   /
# |   -----------------        /tonextbreakpoint_bp |
# |-<-|DebugInProgress|---<---/ runtocursor_bp      |
#     -----------------         runtoreturnpoint_bp |
#       \     \                 stepbystepinto_bp   |
#        \     \                stepbystepover_bp   |
#         \     \               stepbystepout_bp
#          \     \
#           \     \-->-- insertremove_bp      |
#            \           tonextbreakpoint_bp  |
#             \          stepbystepinto_bp    |
#              \         stepbystepover_bp    |
#               \        stepbystepout_bp     |
#                \       runtoreturnpoint_bp  |
#                 \      runtocursor_bp       |
#                  \     break_bp
#                   \             |
#                    \            |
#                     \           |
#                      \----<-----|
#
####################################################################

proc setdbstate {state} {
    global debugstate
    if {$state == "NoDebug" || $state == "ReadyForDebug"} {
        removeallactive_bp
    }
    # when entering the debug mode, the Scilab shell must be at main level
    # this is safer wrt ScilabEval, and is required for FormatWhereForWatch
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
    global Shift_F8 Shift_F9 Shift_F11 Shift_F12
    global MenuEntryId
    global bug2384_fixed

    set errmess "Unknown debugstate in proc setdbmenuentriesstates_bp: please report"

    set dm $pad.filemenu.debug
    set dms $pad.filemenu.debug.step

    if {[getdbstate] == "NoDebug"} {
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Insert/Remove breakpoint"]) -state normal
        bind all <F9> {insertremove_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Edit breakpoints"]) -state normal
        bind all <Control-F9> {showbptgui_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Remove &all breakpoints"]) -state normal
        pbind all $Shift_F9 {removeallbpt_scipad_bp}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to re&turn point"]) -state disabled
        pbind all $Shift_F11 {}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Edit breakpoints"]) -state normal
        bind all <Control-F9> {showbptgui_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Remove &all breakpoints"]) -state normal
        pbind all $Shift_F9 {removeallbpt_scipad_bp}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to re&turn point"]) -state normal
        pbind all $Shift_F11 {runtoreturnpoint_bp}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "&Edit breakpoints"]) -state normal
        bind all <Control-F9> {showbptgui_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Remove &all breakpoints"]) -state normal
        pbind all $Shift_F9 {removeallbpt_scipad_bp}
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
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to re&turn point"]) -state normal
        pbind all $Shift_F11 {runtoreturnpoint_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Run to c&ursor"]) -state normal
        bind all <Control-F11> {runtocursor_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"]) -state normal
        pbind all $Shift_F12 {goonwo_bp}
        $dm entryconfigure $MenuEntryId($dm.[mcra "Show &watch"]) -state normal
        bind all <Control-F12> {showwatch_bp}
        if {$bug2384_fixed} {
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

    if {[istoplevelopen watch]} {
        set wi $watchwinicons
        set wis $watchwinstepicons
        if {[getdbstate] == "NoDebug"} {
            [lindex $wi $MenuEntryId($dm.[mcra "&Edit breakpoints"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state disabled
            [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state disabled
            [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state disabled
            [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "Run to re&turn point"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state disabled
        } elseif {[getdbstate] == "ReadyForDebug"} {
            [lindex $wi $MenuEntryId($dm.[mcra "&Edit breakpoints"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
            [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state normal
            [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state normal
            [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "Run to re&turn point"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state normal
        } elseif {[getdbstate] == "DebugInProgress"} {
            [lindex $wi $MenuEntryId($dm.[mcra "&Edit breakpoints"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state disabled
            [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state normal
            [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state normal
            [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state normal
            [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "Run to re&turn point"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state normal
            [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state normal
            if {$bug2384_fixed} {
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state normal
            } else {
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
            }
            [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state normal
        } else {
            tk_messageBox -message $errmess
        }
    }

}

proc setdbstatevisualhints_bp {} {
    global pad
    global colormen
    if {[getdbstate] == "NoDebug"} {
        $pad.statusind2 configure -background $colormen
        showinfo [mc "Currently no debug session"]
    } elseif {[getdbstate] == "ReadyForDebug"} {
        $pad.statusind2 configure -background SpringGreen
        showinfo [mc "Ready to start debug"]
    } elseif {[getdbstate] == "DebugInProgress"} {
        $pad.statusind2 configure -background tomato3
        showinfo [mc "Debug in progress"]
    }
    updatedebugstateindicator_bp
}

proc updatedebugstateindicator_bp {} {
    global pad led_debugstate
    if {[istoplevelopen watch]} {
        if {[getdbstate] == "NoDebug"} {
            $led_debugstate itemconfigure all -image led_debugstate_NoDebug
            showinfo [mc "Currently no debug session"]
        } elseif {[getdbstate] == "ReadyForDebug"} {
            $led_debugstate itemconfigure all -image led_debugstate_ReadyForDebug
            showinfo [mc "Ready to start debug"]
        } elseif {[getdbstate] == "DebugInProgress"} {
            $led_debugstate itemconfigure all -image led_debugstate_DebugInProgress
            showinfo [mc "Debug in progress"]
        }
    }
}

proc setdebuggerbusycursor {} {
# set the busy cursor for all the children of $pad
    global pad debuggerbusycursor
    if {!$debuggerbusycursor} {
        setdebuggerwidgetbusycursor $pad
        set debuggerbusycursor true
    } else {
        # debugger busy cursor already set - do nothing
    }
}

proc setdebuggerwidgetbusycursor {w} {
    global cursorsinwidgets
    set busycursor "watch"
    # save current cursor in the global array and set busy cursor
    set cursorsinwidgets($w) [$w cget -cursor]
    $w configure -cursor $busycursor
    # ditto for children of the given widget,
    # but not for the menubars nor tearoffs (clones), which properties
    # are inherited from their master copy. See also:
    # http://groups.google.com/group/comp.lang.tcl/browse_frm/thread/87adc111127063bc/05efee764b23540d
    foreach child [filteroutmenuclones [winfo children $w]] {
        setdebuggerwidgetbusycursor $child
    }
}

proc unsetdebuggerbusycursor {} {
# unset the busy cursor for all the children of $pad,
# and restore the cursor they had before
    global pad debuggerbusycursor
    if {$debuggerbusycursor} {
        unsetdebuggerwidgetbusycursor $pad
        set debuggerbusycursor false
    } else {
        # debugger busy cursor already unset - do nothing
    }
}

proc unsetdebuggerwidgetbusycursor {w} {
    global cursorsinwidgets 
    if {[info exists cursorsinwidgets($w)]} { 
        $w configure -cursor $cursorsinwidgets($w) 
    } else {
        $w configure -cursor {} 
    }
    foreach child [filteroutmenuclones [winfo children $w]] {
        unsetdebuggerwidgetbusycursor $child
    } 
}

proc checkendofdebug_bp {{stepmode "nostep"}} {
# check if debug session is over
# if it is, remove in Scilab the breakpoints set by the user and set the
# debug state accordingly
# if it is not, set in Scilab the breakpoints set by the user (useful for
# step by step mode, where all breakpoints were removed just before the call
# to checkendofdebug_bp), check that step over did not enter an ancillary
# (fix it if it happened), check breakpoints condition expressions,
# update the hit count, decide if the debug should stop and skip no code lines
#
# warning: highly polished guru code here! don't change if you don't perfectly
#          know what you're doing...!

    global setbptonreallybreakpointedlinescmd
    global prevdbpauselevel initprevdbpauselevel

    set removecomm [duplicatechars [removeallbpt_scilab "no_output"] "\""]
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
    # or running to return point of the current function
    # note that in order to stack commands with the right order, this must
    # be a ScilabEval_lt(Tcl_EvalStr(ScilabEval_lt(TCL_EvalStr ...) seq) seq)
    switch -- $stepmode {
        "nostep"   {
            # In principle there is no need to add code here for skipping no code
            # lines since stops always occur on existing breakpoints set by the user.
            # The only exception is when Break was hit because Scilab might stop
            # on a comment. In this case, the nocode lines are skipped by calling
            # stepbystepover_bp, and a Break command is immediately issued to
            # prevent from resuming at infinitum, which would otherwise look as
            # if the user had not hit Break initially
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\] && \\\[isbreakhit_bp\\\]} {stepbystepover_bp 0 0 ; break_bp} else {updatewatchvars;unsetdebuggerbusycursor}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "into"     {
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} { stepbystepover_bp 0 0 ; if {\\\[isbreakhit_bp\\\]} {break_bp} } else {updatewatchvars;unsetdebuggerbusycursor}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "over"     {
            # stepbystepover_bp 0 1, i.e. with rescanbuffers set to true,
            # because Scipad must rescan the buffer when leaving it on
            # step over - this is required to prevent Scipad to skip
            # nested libfuns contructs
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} { stepbystepover_bp 0 1 ; if {\\\[isbreakhit_bp\\\]} {break_bp} } else {updatewatchvars;unsetdebuggerbusycursor}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "out"      {
            set skipline "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} { stepbystepover_bp 0 0 ; if {\\\[isbreakhit_bp\\\]} {break_bp} } else {updatewatchvars;unsetdebuggerbusycursor}\\\"\",\\\"\"scipad\\\"\");"
                   }
        "runtocur" {
            set skipline1 "TCL_EvalStr(\\\"\"if {!\\\[iscursorplace_bp\\\] && !\\\[isbreakhit_bp\\\]} {runtocursor_bp 0 1}\\\"\",\\\"\"scipad\\\"\");"
            set skipline2 "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} { stepbystepover_bp 0 0 ; if {\\\[isbreakhit_bp\\\]} {break_bp} } else {updatewatchvars;unsetdebuggerbusycursor}\\\"\",\\\"\"scipad\\\"\");"
            set skipline [concat $skipline1 $skipline2]
                   }
        "runtoret" {
            set skipline1 "TCL_EvalStr(\\\"\"if {!\\\[isreturnpoint_bp\\\] && !\\\[isbreakhit_bp\\\]} {runtoreturnpoint_bp 0 1}\\\"\",\\\"\"scipad\\\"\");"
            set skipline2 "TCL_EvalStr(\\\"\"if {\\\[isnocodeline \\\[gettextareacur\\\] insert\\\]} { stepbystepover_bp 0 0 ; if {\\\[isbreakhit_bp\\\]} {break_bp} } else {updatewatchvars;unsetdebuggerbusycursor}\\\"\",\\\"\"scipad\\\"\");"
            set skipline [concat $skipline1 $skipline2]
                   }
    }

    # command to check that step over (F8) didn't actually step into (Shift-F8)
    # without the precautions below, this might happen in two situations:
    # 1. Step over (F8) steps actually into (Shift-F8) libfuns when the libfun
    #    is in a nested function whose calling function contains the same
    #    libfun on the calling line, e.g. hitting F8 when the active breakpoint
    #    line is on line 3, i.e. a=evstr("1+1") will step into the evstr libfun
    #    whereas it should skip this statement:
    #       function outerfun
    #         function a=nestedfun
    #           a=evstr("1+1")
    #         endfunction
    #         b=evstr(string(nestedfun()))
    #       endfunction
    #    evstr must be breakpointed in case hitting F8 makes Scilab go out of
    #    function nestedfun. Since there is no way to know in advance whether
    #    the next step will fly out of the current function or not, evstr must
    #    be breakpointed at this point of the debug
    #    --> The solution used is to check whether a deeper level has just
    #        been reached, and if the current stop is not due to a real
    #        breakpoint set by the user (nor by the user having hit Break),
    #        then perform a stepbystepout_bp and close the ancillary
    #        file that was possibly opened by the debugger, if that
    #        did happen (if not, then the ancillary was already opened
    #        and it is not automatically closed by this proc)
    #    Note: in the example above, going out of nestedfun with F8 on the
    #    endfunction still enters the evstr ancillary, but this is desirable:
    #    the debugger goes out of nestedfun and enters the libfun ancillary
    #    from the line that called nestedfun, i.e. the same depth level is
    #    kept, which is the exact purpose of step over
#
# <TODO> Warning! The syntax below is NOT compatible with the BUILD4 environment
#        since it makes use of the result from TCL_EvalStr, which is not available
#        in BUILD4. No error will be thrown but $stoppedonarealbpt always evaluates
#        as true while $breakwashit and $stoppedinsamefun evaluate as false. The
#        desired behavior is thus not obtained for these tests
#        Note however that this syntax is available in the BUILD_412 environment
#
    set stoppedonarealbpt "TCL_EvalStr(\"lsearch -sorted -increasing -integer -exact \[getfunlinesfrombptsprops \" + db_m(3) + \"\] \" + string(db_l(3)) + \"\",\"scipad\") <> string(-1)"
    set breakwashit "TCL_EvalStr(\"isbreakhit_bp\",\"scipad\") == \"true\""
    set stoppedinsamefun "TCL_EvalStr(\"hasstoppedinthesamefun\",\"scipad\") == \"true\""
    switch -- $stepmode {
        "nostep"   { set steppedininsteadofover "%f" }
        "into"     { set steppedininsteadofover "%f" }
        "over"     { set steppedininsteadofover "(size(db_l,1) > $prevdbpauselevel) & ~($stoppedonarealbpt) & ~($breakwashit)" }
        "out"      { set steppedininsteadofover "%f" }
        "runtocur" { set steppedininsteadofover "%f" }
        "runtoret" { set steppedininsteadofover "%f" }
    }
    # 2. Step over (F8) steps actually into (Shift-F8) when the next line
    #    calls the same function as the one the debugger is currently in
    #    (recursive call) - Similar to case 1: the current function must
    #    be breakpointed on every line since there is no way to know where
    #    the next stop will be, and this makes a recursive call look like
    #    a step into. Moreover, step out (Ctrl-F8) in recursive functions
    #    does not work: it steps into instead
    #    --> The solution used is to check whether the pause level has just
    #        decreased or not; if not, and if the current stop is not due to a
    #        real breakpoint set by the user (nor by the user having hit Break),
    #        then perform a stepbystepout_bp again
    # Note: for case "out", the last step went out if (mainly):
    #    size(db_l,1) < $prevdbpauselevel
    #      i.e. there is one pause level less than at the previous stop
    #      This is for standard recursive functions to work
    #      such as the naive factorial function
    #  or
    #    (size(db_l,1) == $prevdbpauselevel) & ~($stoppedinsamefun)
    #      i.e. same level as at the previous stop, but in another function
    #      This is to allow for nested libfun calls to work, e.g.:
    #        n=[1,2,10,15];m=[2,2,3,5];
    #        a=pmodulo(modulo(pmodulo(modulo(n,m),m),m),m)
    switch -- $stepmode {
        "nostep"   { set didntwentout "%f" }
        "into"     { set didntwentout "%f" }
        "over"     { set didntwentout "%f" }
        "out"      { set didntwentout "(~( (size(db_l,1) < $prevdbpauselevel) | ((size(db_l,1) == $prevdbpauselevel) & ~($stoppedinsamefun)) )) & \
                                       ~($stoppedonarealbpt) & \
                                       ~($breakwashit)" }
        "runtocur" { set didntwentout "%f" }
        "runtoret" { set didntwentout "%f" }
    }

    # breakpoints have an associated expression that is evaluated when the
    # breakpoint location is reached.
    # A breakpoint is hit only if the expression is true (or has changed,
    # depending on what the user selected)
    # the hit condition is in Scilab language and is checked here
    # then the hit count is updated and is compared to the hit condition
    # in order to decide whether the debug should stop at the reached
    # position or not
    # Note: in "nostep" mode, the break command must be tested to set $muststop
    # to true when the user has hit Break because in this case Scilab doesn't stop
    # on a real breakpoint as it would be expected when $stepmode is "nostep"
#
# <TODO> Warning! The syntax below is NOT compatible with the BUILD4 environment
#        since it makes use of the result from TCL_EvalStr, which is not available
#        in BUILD4. An error will be thrown "undefined index" or something alike
#        Note however that this syntax is available in the BUILD_412 environment
#
    switch -- $stepmode {
        "nostep"   {
                    # the syntax below would create a too long ScilabEval command
                    # (bug 2654, tagged as RESOLVED LATER, now REOPENED after complaining...)
                    #set ID "\" + TCL_EvalStr(\"where2bptID \" + db_m(3) + \" \" + string(db_l(3)) ,\"scipad\") + \""
                    # thus we must to go through a Scilab variable instead (drawback: possible name collision in Scilab)
                    # Note that the hardcoded 3 is always the good index where to look for the function name and line
                    # number coresponding to the current stop. The structure of the where() output indeed is:
                    #   !execstr         !
                    #   !pause           !
                    #   !currentfun      !    // function where Scilab is at the current stop --> always 3rd value
                    #   !calllevelfun    !    // function calling the function where Scilab is at the current stop
                    #   !<uppper levels> !
                    #   !execstr         !
                    set getbptIDcommand "db_BPTID=TCL_EvalStr(\"where2bptID \" + db_m(3) + \" \" + string(db_l(3)) ,\"scipad\");"
                    set isbreakpointenabled       "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",enable)\",\"scipad\") == \"true\""
                    set isconditiontypeistrue     "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",conditiontype)\",\"scipad\") == \"istrue\""
                    set isconditiontypehaschanged "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",conditiontype)\",\"scipad\") == \"haschanged\""
                    set prevconditionvalue        "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",curexpvalue)\",\"scipad\")"
                    set rawhitcondition "evstr(TCL_GetVar(\"\"bptsprops(\"+db_BPTID+\",expression)\"\",\"\"scipad\"\"))"
                    set evalerrorproofedhitcondition "\[db_condexprresult,db_evstrierr\]=evstr(\"$rawhitcondition\"); \
                                                      if db_evstrierr<>0 then db_condexprresult=emptystr();lasterror(%t);end; \
                                                      db_condexprresult=FormatStringsForWatch(db_condexprresult);"

                    set isbpthit "($isbreakpointenabled & ( ($isconditiontypeistrue & db_condexprresult==FormatStringsForWatch(%t)) | \
                                                            ($isconditiontypehaschanged & db_condexprresult<>$prevconditionvalue) \
                                                          ) \
                                  )"
                    set inchitcount "TCL_EvalStr(\"incr bptsprops(\"+db_BPTID+\",hitcount)\",\"scipad\");"

                    # note that it's really the *current* hit count here,
                    # i.e. the hit count after execution of $inchitcount
                    # in the huge ScilabEval below
                    set currenthitcount      "evstr(TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcount)\",\"scipad\"))"
                    set hitcountthreshold    "evstr(TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcountthreshold)\",\"scipad\"))"
                    set isbreakifalways      "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcountcondition)\",\"scipad\") == \"always\""
                    set isbreakifequalto     "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcountcondition)\",\"scipad\") == \"equalto\""
                    set isbreakifmultipleof  "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcountcondition)\",\"scipad\") == \"multipleof\""
                    set isbreakifgreaterthan "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcountcondition)\",\"scipad\") == \"greaterthan\""
                    set isbreakiflessthan    "TCL_GetVar(\"bptsprops(\"+db_BPTID+\",hitcountcondition)\",\"scipad\") == \"lessthan\""

                    set muststop "($breakwashit) | ($isbpthit & ($isbreakifalways | \
                                                                 ( $isbreakifequalto & ($currenthitcount == $hitcountthreshold) ) | \
                                                                 ( $isbreakifmultipleof & (modulo($currenthitcount,$hitcountthreshold) == 0) ) | \
                                                                 ( $isbreakifgreaterthan & ($currenthitcount > $hitcountthreshold) ) | \
                                                                 ( $isbreakiflessthan & ($currenthitcount < $hitcountthreshold) ) \
                                                                ) \
                                                       )"
                   }
        "into"     {
                    set evalerrorproofedhitcondition ""
                    set isbpthit "%f"
                    set inchitcount ""
                    set getbptIDcommand ""
                    set muststop "%t"
                   }
        "over"     {
                    set evalerrorproofedhitcondition ""
                    set isbpthit "%f"
                    set inchitcount ""
                    set getbptIDcommand ""
                    set muststop "%t"
                   }
        "out"      {
                    set evalerrorproofedhitcondition ""
                    set isbpthit "%f"
                    set inchitcount ""
                    set getbptIDcommand ""
                    set muststop "%t"
                   }
        "runtocur" {
                    set evalerrorproofedhitcondition ""
                    set isbpthit "%f"
                    set inchitcount ""
                    set getbptIDcommand ""
                    set muststop "%t"
                   }
        "runtoret" {
                    set evalerrorproofedhitcondition ""
                    set isbpthit "%f"
                    set inchitcount ""
                    set getbptIDcommand ""
                    set muststop "%t"
                   }
    }

    # now create the full command to be ScilabEval-ed at the end
    # of each debug command
    # note: all this is highly polished and should only be changed after
    # having thought twice (hmm, no, three times). For instance, some
    # lines below contain identical code but must not be factorized
    # because this would change execution order of this complicated
    # code structure (TCL_EvalStr(ScilabEval(TCL_EvalStr...) seq) associated
    # to stepbystepout_bp or skiplines, which launch further ScilabEvals
    # with seq options)
    set comm1  "\[db_l,db_m\]=where();"
    set comm2  "if size(db_l,1)==1 then"
    set comm3      "TCL_EvalStr(\"ScilabEval_lt \"\"$removecomm\"\"  \"\"seq\"\" \",\"scipad\");"
    set comm4      "TCL_EvalStr(\"setdbstate \"\"ReadyForDebug\"\" \",\"scipad\");"
    set comm5      "TCL_EvalStr(\"scedebugcleanup_bp\",\"scipad\");"
    set comm6      "TCL_EvalStr(\"checkexecutionerror_bp\",\"scipad\");"
    set comm7      "TCL_EvalStr(\"updatewatchvars;unsetdebuggerbusycursor\",\"scipad\");"
    set comm8      "TCL_SetVar(\"prevdbpauselevel\",$initprevdbpauselevel,\"scipad\");"
    set comm9  "else"
    set comma1     "if $steppedininsteadofover then"
    set comma2         "TCL_EvalStr(\"ScilabEval_lt TCL_EvalStr(\"\"closecurifopenedbyuabpt\"\",\"\"scipad\"\") seq\",\"scipad\");"
    set comma3         "TCL_EvalStr(\"ScilabEval_lt {TCL_EvalStr(\"\"set afilewasopenedbyuabpt false\"\",\"\"scipad\"\")} seq\",\"scipad\");"
    set comma4         "TCL_EvalStr(\"stepbystepout_bp 0 0\",\"scipad\");"
    set commb1     "elseif $didntwentout then"
    set commb2         "TCL_EvalStr(\"ScilabEval_lt {TCL_EvalStr(\"\"set afilewasopenedbyuabpt false\"\",\"\"scipad\"\")} seq\",\"scipad\");"
    set commb3         "TCL_EvalStr(\"stepbystepout_bp 0 0\",\"scipad\");"
    set commc1     "else"
    set commd1         "$getbptIDcommand;"
    set commd2         "$evalerrorproofedhitcondition;"
    set commd3         "TCL_EvalStr(\"saveallconditionsvalues\",\"scipad\");"
    set commd4         "if ($isbpthit) then $inchitcount;end;"
    set commd5         "if ~($muststop) then"
    set commd6             "TCL_EvalStr(\"ScilabEval_lt {TCL_EvalStr(\"\"set afilewasopenedbyuabpt false\"\",\"\"scipad\"\")} seq\",\"scipad\");"
    set commd7             "TCL_EvalStr(\"resume_bp 0\",\"scipad\");"
    set commd8         "else"
    set commd9            "TCL_EvalStr(\"ScilabEval_lt {TCL_EvalStr(\"\"set afilewasopenedbyuabpt false\"\",\"\"scipad\"\")} seq\",\"scipad\");"
    set commda            "TCL_EvalStr(\"ScilabEval_lt \"\"$skipline\"\"  \"\"seq\"\" \",\"scipad\");"
    set commdb            "TCL_SetVar(\"prevdbpauselevel\",size(db_l,1),\"scipad\");"
    set commdc         "end;"
    set commc2     "end;"
    set comm10     "TCL_EvalStr(\"ScilabEval_lt \"\"$cmd\"\" \"\"seq\"\" \",\"scipad\");"
    set comm11     "TCL_EvalStr(\"ScilabEval_lt {TCL_EvalStr(\"\"resetbreakhit_bp\"\",\"\"scipad\"\")} seq\",\"scipad\");"
    set comm12 "end;"
#    set comm12 "end;TCL_EvalStr(\"hidewrappercode\",\"scipad\");"

    set comma [concat $comma1 $comma2 $comma3 $comma4]
    set commb [concat $commb1 $commb2 $commb3]
    set commd [concat $commd1 $commd2 $commd3 $commd4 $commd5 $commd6 \
                      $commd7 $commd8 $commd9 $commda $commdb $commdc]
    set commc [concat $commc1 $commd $commc2]

    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 \
                         $comm8 $comm9 $comma $commb $commc \
                         $comm10 $comm11 $comm12]

    # do it!
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
                                 set errmsg  \"\"\"+stripblanks(strsubst(strcat(db_str,ascii(10)),\"\"\"\",\"\\\"\"\"))+\"\"\"; \
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
