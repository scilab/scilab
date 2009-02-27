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

#################################
#    Debug settings - Part 1    #
#################################


#############
# Detect Tcl and Tk versions and set global flags to true if versions are >= 8.5
#
# This is used to improve Scipad when used with recent Tcl/Tk without
# preventing its use with older ladies
# ex of 8.5 use: a. -strictlimits option in find/replace
#                b. -stretch always option for panedwindows
#                c. proc timestamp uses clock milliseconds
#                d. peer text widgets are used when splitting
#                e. Tk bug 1169429 (relative to cursor blinking) is fixed, workaround hack removed
#                f. -topmost option of toplevels used also on Linux
#                g. string reverse (TIP #272) is used during undo/redo, improving performance drastically
#                h. the replace cursor is a nice looking block cursor
if { [package vcompare $tcl_version 8.5] >= 0 } {
    set Tcl85 1
} else {
    set Tcl85 0
}
if { [package vcompare $tk_version 8.5] >= 0 } {
    set Tk85 1
} else {
    set Tk85 0
}

# End of Tcl/Tk versions detection
#############


#############
# Debug settings for RamDebugger

# Don't forget to set this setting to no before committing!
# There is a hard link to the RamDebugger directory here!
# Anyway, there is a catch to avoid errors in case of lapse of memory...

set DebugScipadWithRamDebugger no

catch {
    if {$DebugScipadWithRamDebugger && $tcl_platform(platform) == "windows"} {
        lappend ::auto_path K:/Francois/Developpement/RamDebugger6.1.1/addons
        lappend ::auto_path D:/Scilab/Tools/RamDebugger5.5/addons
        package require commR
        comm::register Scipad 1
    }
}

# End of debug settings for RamDebugger
#############


#############
# Debug settings in case Scipad is launched outside of Scilab, e.g. from wish

# Warning: "sync" "seq" option is mandatory so that colorization
#          works with scipad somefile.sci
if {[catch {ScilabEval ";" "sync" "seq"}] != 0} {

    # Define ScilabEval to a void function, if it is unknown. This is
    # useful in order to run scipad outside of scilab (e.g. to debug it)
    proc ScilabEval args {
        # catched because may be called before the main window is ready
        # to display the message, or before the localization procs such
        # as mc are sourced
        catch {showinfo [mc "NOT CONNECTED TO SCILAB"]}
        puts $args
    }
    set sciprompt 0

    # hide the superfluous toplevel created by wish by default
    wm withdraw .

    # show the console in a fixed screen position and size
    if {$tcl_platform(platform) != "unix"} {
        console show
        console eval {wm geometry . 67x20+0+0}
        console title "Scipad debug"
    }

    # give a value to tmpdir (when Scipad is launched from Scilab,
    # this is done in scipad.sci)
    set tmpdir $env(SCIHOME)

    # deiconify now with no need to have Scilab running since
    # ScilabEval "TCL_EvalStr(\"wm deiconify $pad\",\"scipad\")" "seq"
    # at the end of scipad.tcl won't run - anyway, dynamickeywords is
    # not executed
    after idle {wm deiconify $pad}

    set standaloneScipad true

} else {

    set standaloneScipad false

}

# End of debug settings in case Scipad is launched outside of Scilab, e.g. from wish
#############


#############
# Debug settings for the latest Scipad debugger features

# Committed versions should have this attribute set to false
# so that features that are believed to be too unstable are
# disabled (usually because of issues in the Scilab parsers)
# Currently this flag is not used

set dev_debug false

# End of debug settings for the latest Scipad debugger features
#############


#############
# Debug settings for the background tasks

# Committed versions should have this attribute set to true
# since bug 865 has been fixed
# When set to true, some tasks (colorization, keyposn) are
# performed in the background

set backgroundtasksallowed true

# End of debug settings for the background tasks
#############

#############
# Debug settings to trace when certain variables are changed

if {0} {
    proc tracer {varname args} {
        global debuglog standaloneScipad
        upvar #0 $varname var
        if {$debuglog} {
            # debug log (in a file or in the console)
            log "$varname was updated to be \"$var\""
        } else {
            if {$standaloneScipad} {
                # Scipad launched from wish, no log
                puts "$varname was updated to be \"$var\""
            } else {
                # Scipad launched from Scilab, no log
                tk_messageBox -message "$varname was updated to be \"$var\""
            }
        }
    }
    trace add variable dndreallystarted write {tracer dndreallystarted}
    trace add variable dndinitiated write {tracer dndinitiated}
}

# End of debug settings to trace when certain variables are changed
#############


#############
# A useful binding triggering a message box

if {0} {
    proc dispsthg {} {
#        set str [countcontlines [gettextareacur] 1.0 [[gettextareacur] index insert]]
#        set str [time {colorize [gettextareacur] 1.0 end}]
#        set str "[getlogicallinenumbersranges allscilabbuffers "stepbystep"]\n\n[getlogicallinenumbersranges current&ancill "stepbystep"]"
        set str "[getlistofancillaries [gettextareacur] FOO1 libfun]"
        tk_messageBox -message "$str"
    }
    bind all <Control-equal> {dispsthg}
}

# End of useful binding triggering a message box
#############


#############
# A useful binding triggering a command

if {0} {
    bind all <Control-equal> { \
        puts [time {colorizestringsandcomments_sd [gettextareacur] 1.0 end}] ; \
        showinfo "colorize done (strings+comments)"
    }
    bind all <Control-equal> { \
        parray words; parray chset \
    }
    bind all <Control-equal> { \
        if {[isscilabbusy]} {tk_messageBox -message "BUSY"} else {tk_messageBox -message "IDLE"}
    }
    bind all <Control-equal> { \
        checkexecutionerror_bp \
    }
    bind all <Control-equal> { \
        runtoreturnpoint_bp \
    }
}

# End of useful binding triggering a command
#############


#############
# Miscellaneous helper procs

if {1} {

    proc arrayval {array {pattern *}} {
    # return the content of an array as a string
    # this is pretty much the same as parray but the result is returned in a string
    # instead of just printed in the console
    # this code originates from  http://wiki.tcl.tk/9788
    # see also  http://wiki.tcl.tk/13816  for a proc TK_editArray allowing
    # to edit any array in a convenient gui
    upvar $array a
    if { ![array exists a] } { error "\"$array\" isn't an array" }
    set lines [list]
    set max 0
    foreach name [array names a $pattern] {
        set len [string length $name]
        if { $len > $max } { set max $len }
    }
    set max [expr {$max + [string length $array] + 2}]
    foreach name [array names a $pattern] {
        set line [format %s(%s) $array $name]
        lappend lines [format "%-*s = %s" $max $line $a($name)]
    }
    return [join [lsort $lines] \n]
    }
    # examples
    # set parrayenv [arrayval ::env]
    # set dumpchset [arrayval chset]
    # then, from Scilab, it's possible to:  TCL_EvalStr("arrayval chset","scipad")


    proc stacktrace {} {
    # return the calling stack in a string
    # this code originates from  http://wiki.tcl.tk/16183
    # and was augmented to retrieve line numbers in addition
    global Tcl85
    set stack "Stack trace:\n"
    for {set i 1} {$i < [info level]} {incr i} {
        append stack "Level -$i: "
        if {$Tcl85} {
            set framedict [info frame -$i]
            # next line catched for easy handling of the cases where the dict
            # does not contain the "line" entry
            catch {append stack "line [dict get $framedict line] in file containing "}
        }
        set lvl [info level -$i]
        set pname [lindex $lvl 0]
        append stack "$pname\n\t\t"
        foreach value [lrange $lvl 1 end] arg [info args $pname] {
            if {$value eq ""} {
                info default $pname $arg value
            }
            append stack "$arg='$value'\n\t\t"
        }
        append stack \n
    }
    return $stack
    }

    proc dealwithbug4053 {w itag initial scitags star stop kword amatch allmatch dyn_run dyn_ran chsetar wordsar} {
    # output debug information helping in finding the cause for
    # bug 4053 (colorisation fails during opening of a file)
        global env listoffile

        array set chseta $chsetar
        array set wordsa $wordsar

        set fileid4053 [open [file join $env(SCIHOME) SciPadDebug_Bug4053.log] a]

        puts $fileid4053 "BUG 4053 HIT!"
        puts $fileid4053 [clock format [clock seconds] -format "%Y-%m-%d %H:%M:%S"]

        puts $fileid4053 [stacktrace]

        puts $fileid4053 $listoffile("$w",language)
        puts $fileid4053 $listoffile("$w",colorize)

        puts $fileid4053 $scitags ; # here, without the userfun tag
        regsub -all "scilab." [array names chseta -glob scilab\.*] "" scitags
        puts $fileid4053 $scitags

        puts $fileid4053 "chset(scilab.$itag) contains: $chseta(scilab.$itag)"
        puts $fileid4053 "info exists words(scilab.$itag.$initial) returns: [info exists wordsa(scilab.$itag.$initial)]"

        puts $fileid4053 "dynamickeywords_running is: $dyn_run"
        puts $fileid4053 "dynamickeywords_ran_once is: $dyn_ran"

        puts $fileid4053 $star
        puts $fileid4053 $stop
        puts $fileid4053 $kword
        puts $fileid4053 $amatch
        puts $fileid4053 $allmatch

        puts $fileid4053 "--- Now full content of the chset array:"
        puts $fileid4053 [arrayval chseta]
        puts $fileid4053 "--- Now full content of the words array:"
        puts $fileid4053 [arrayval wordsa]

        puts $fileid4053 "--- Now full content of the textarea:"
        puts $fileid4053 [$w get 1.0 end]

        puts $fileid4053 "----------------------------------\n"

        close $fileid4053

        tk_messageBox -message ">>>> BUG 4053 HIT <<<<\n\n\nPLEASE report to    http://bugzilla.scilab.org/show_bug.cgi?id=4053 \n\nand attach the log file named     SciPadDebug_Bug4053.log \n\nthat you can find in  SCIHOME, i.e. in \n\n$env(SCIHOME)"
    }

}

# End of miscellaneous helper procs
#############


#############
# Procs and main level code related to the Scipad debug log file

# if $debuglog is true then log mode is on
# if $loginafile is true then a log file is created in $env(SCIHOME)
# this file contains the same info as the log output in the console
# if $loginafile is false then log output is sent to the console only
# if $logdetailedbindings is true, then all the non Tcl/Tk default
# bindings are logged (i.e. not only those attached to $pad, but also
# those attached to children of $pad)
# if $logTextclassbindings is true, then all the Tcl/Tk Text class
# bindings are logged
set debuglog false
set loginafile true
set logdetailedbindings true
set logTextclassbindings false

if {$debuglog} {

    unset -nocomplain -- Scipaddebuglogfileid

    proc timestamp { { tv_sec_p "" } { tv_msec_p "" } } {
    # timestamping accurate to the millisecond
    # this proc can give back the second and millisecond information
    # separately to the caller
    # it was taken from http://wiki.tcl.tk/1035 and slightly adapted
    # to take advantage of what is available in 8.5 if present
        global Tcl85

        if { $tv_sec_p != "" } {
            upvar $tv_sec_p secs
        }
        if { $tv_msec_p != "" } {
            upvar $tv_msec_p fract
        }

        if {$Tcl85} {
            # Tcl >= 8.5, clock milliseconds is better
            set stamp [clock milliseconds]
            set secs [expr {$stamp / 1000}]
            set fract [expr {$stamp - ($secs * 1000)}]

        } else {
            # Tcl < 8.5, use what available
            set secs [clock seconds]
            set ms [clock clicks -milliseconds]
            set base [expr {$secs * 1000}]
            set fract [expr {$ms - $base}]
            if { $fract > 1000 } {
                set diff [expr {$fract / 1000}]
                incr secs $diff
                incr fract [expr {-1000 * $diff}]
            }

        }

        return $secs.[format %03d $fract]
    }

    proc log {value} {
    # quick and dirty log file facility
    # value is output in the console, and also in a file unless $loginafile is
    # set to false
    # the log file is overwritten each time Scipad starts
    # however in case a bgerror occurs, the log file is copied to another file
    # and this one is never erased by Scipad (a new filename is used for each
    # bgerror)
        global Scipaddebuglogfileid Scipaddebuglogfilename env debuglog loginafile
        global defaultencoding
        if {!$debuglog} {return}
        timestamp sec mil
        puts "[clock format $sec -format "%d/%m/%y|%T"].[format %03d $mil]|$value"
        if {$loginafile} {
            if {![info exists Scipaddebuglogfileid]} {
                set rootpath $env(SCIHOME)
                set filename "SciPadDebug"
                set Scipaddebuglogfilename [file join $rootpath $filename.log]
                set Scipaddebuglogfileid [open $Scipaddebuglogfilename w]
            } else {
                set Scipaddebuglogfileid [open $Scipaddebuglogfilename a]
            }
            fconfigure $Scipaddebuglogfileid -encoding $defaultencoding
            puts $Scipaddebuglogfileid "[clock format $sec -format "%d/%m/%y|%T"].[format %03d $mil]|$value"
            close $Scipaddebuglogfileid
        }
    }

    proc bgerror {message} {
        global errorInfo env Scipaddebuglogfilename Scipaddebuglogfileid
        log "\n-----------------------------------------"
        log "\BGERROR!:\n'$message'\n$errorInfo"
        log "\n-----------------------------------------\n"
        set savelogid 0
        set savelogname "$Scipaddebuglogfilename.BGERROR$savelogid.log"
        while {[file exists $savelogname]} {
            incr savelogid
            set savelogname "$Scipaddebuglogfilename.BGERROR$savelogid.log"
        }
        file copy -force -- $Scipaddebuglogfilename $savelogname
        tk_messageBox -message "An unexpected error occurred.\n\
                                The log file can be found in $savelogname\n\
                                Please report this issue!" -icon error
    }

    # all the procs sourced before this point will not take part in the log file
    # (but see also scipaddebug2.tcl)
    # nologprocs must be set only once in a Scilab session (on reopening Scipad
    # during the same Scilab session, because the Tcl interpreter doesn't die
    # between Scipad sessions, info procs contains all the Scipad procs, which
    # is not desired)
    if {![info exists nologprocs]} {
        set nologprocs [info procs]
    }
}

# End of procs and main level code related to the Scipad debug log file
#############


##################################
# End of debug settings - Part 1 #
##################################
