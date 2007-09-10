#################################
#    Debug settings - Part 1    #
#################################


#############
# Debug settings for RamDebugger

# Don't forget to set this setting to no before committing!
# There is a hard link to the RamDebugger directory here!
# Anyway, there is a catch to avoid errors in case of lapse of memory...

set DebugScipadWithRamDebugger no

catch {
    if {$DebugScipadWithRamDebugger && $tcl_platform(platform) == "windows"} {
        lappend ::auto_path K:/Francois/Developpement/RamDebugger5.5/addons
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
        showinfo [mc "NOT CONNECTED TO SCILAB"]
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
# Flags to adjust Scipad to the host Scilab version

# The break command can only be used with Scilab versions having bug 2384 fixed
# Currently (30/04/07), this is done in svn trunk and BUILD4 branches
# but nowhere else, e.g. in scilab-gtk
# The flag below allows for easy adjustment of Scipad to Scilab versions,
# especially with backported Scipad versions in mind
# Bug 2384 in fact fixes the sync seq options of ScilabEval (interruptibility)

set bug2384_fixed true

# End of flags to adjust Scipad to the host Scilab version
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
