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
        lappend ::auto_path D:/RamDebugger5.5/addons
        package require commR
        comm::register Scipad 1
    }
}

# End of debug settings for RamDebugger
#############


#############
# Debug settings in case Scipad is launched outside of Scilab, e.g. from wish

if {[catch {ScilabEval ";"}] != 0} {

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
}

# End of debug settings in case Scipad is launched outside of Scilab, e.g. from wish
#############


#############
# Debug settings for the new debugger commands (Run to cursor and Break)

# Committed versions should have this attribute set to false
# In that case, the Run to Cursor and Break commands are hidden
# since there are issues with them in the Scilab parsers
set dev_debug "false"

# End of debug settings for the new debugger commands (Run to cursor and Break)
#############


#############
# Debug settings to trace when certain variables are changed

if {0} {
    proc tracer {varname args} {
        upvar #0 $varname var
        showinfo "$varname was updated to be \"$var\""
    }
    trace add variable watchvsashcoord write {tracer watchvsashcoord}
}

# End of debug settings to trace when certain variables are changed
#############


#############
# A useful binding triggering a message box

if {0} {
    proc dispsthg {} {
        set str [countcontlines [gettextareacur] 1.0 [[gettextareacur] index insert]]
        tk_messageBox -message "$str"
    }
    bind all <Control-equal> {dispsthg}
}

# End of useful binding triggering a message box
#############


#############
# Procs and main level code related to the Scipad debug log file

# if $debuglog is true then log mode is on
# if $loginafile is true then a log file is created in $env(SCIHOME)
# this file contains the same info as the log output in the console
# if $loginafile is false then log output is sent to the console only
set debuglog false
set loginafile true

if {$debuglog} {

    unset -nocomplain -- Scipaddebuglogfileid

    proc log {value} {
    # quick and dirty log file facility
    # value is output in the console, and also in a file unless $loginafile is
    # set to false
    # the log file is overwritten each time Scipad starts
    # however in case an bgerror occurs, the log file is copied to another file
    # and this one is never erased by Scipad (a new filename is used for each bgerror)
        global Scipaddebuglogfileid Scipaddebuglogfilename env debuglog loginafile
        if {!$debuglog} {return}
        set seconds [clock seconds]
        puts "[clock format $seconds -format "%d/%m/%y|%T|"]$value"
        if {$loginafile} {
            if {![info exists Scipaddebuglogfileid]} {
                set rootpath $env(SCIHOME)
                set filename "SciPadDebug"
                set Scipaddebuglogfilename [file join $rootpath $filename.log]
                set Scipaddebuglogfileid [open $Scipaddebuglogfilename w]
            }
            puts $Scipaddebuglogfileid "[clock format $seconds -format "%d/%m/%y|%T|"]$value"
        }
    }

    proc bgerror {message} {
        global errorInfo env Scipaddebuglogfilename Scipaddebuglogfileid
        log "\n-----------------------------------------"
        log "\BGERROR!:\n'$message'\n$errorInfo"
        log "\n-----------------------------------------\n"
        close $Scipaddebuglogfileid
        set savelogid 0
        set savelogname "$Scipaddebuglogfilename.BGERROR$savelogid.log"
        while {[file exists $savelogname]} {
            incr savelogid
            set savelogname "$Scipaddebuglogfilename.BGERROR$savelogid.log"
        }
        file copy -force -- $Scipaddebuglogfilename $savelogname
        set Scipaddebuglogfileid [open $Scipaddebuglogfilename a+]
        tk_messageBox -message "An unexpected error occurred.\n\
                                The log file can be found in $savelogname\n\
                                Please report this issue!" -icon error
    }

    # all the procs sourced before this point will not take part in the log file
    # but see also scipaddebug2.tcl)
    set nologprocs [info procs]
}

# End of procs and main level code related to the Scipad debug log file
#############


##################################
# End of debug settings - Part 1 #
##################################
