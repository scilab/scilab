#########################
#    Debug settings     #
#########################

# Don't forget to set this setting to no before committing!
# There is a hard link to the RamDebugger directory here!
# Anyway, there is a catch to avoid errors in case of lapse of memory...

set DebugScipadWithRamDebugger no

catch {
    if {$DebugScipadWithRamDebugger && $tcl_platform(platform) == "windows"} {
        lappend ::auto_path K:/Francois/Developpement/RamDebugger/addons
        lappend ::auto_path D:/RamDebugger/addons
        package require commR
        comm::register Scipad 1
    }
}

# Define ScilabEval to a void function, if it is unknown. This is
# useful in order to run scipad outside of scilab (e.g. to debug it)
if {[catch {ScilabEval ";"}] != 0} {
    proc ScilabEval args { 
        showinfo [mc "NOT CONNECTED TO SCILAB"]
        puts $args
    }
    set sciprompt 0
    wm withdraw .
    if {$tcl_platform(platform) != "unix"} {console show}
}

# Committed versions should have this attribute set to false
# In that case, the Run to Cursor and Break commands are hidden
# since there are issues with them in the Scilab parsers
set dev_debug "false"

# To trace when certain variables are changed
if {0} {
    proc tracer {varname args} {
        upvar #0 $varname var
        showinfo "$varname was updated to be \"$var\""
    }
    trace add variable watchvsashcoord write {tracer watchvsashcoord}
}

# A useful binding triggering a message box
if {0} {
    proc dispsthg {} {
        set str [countcontlines [gettextareacur] 1.0 [[gettextareacur] index insert]]
        tk_messageBox -message "$str"
    }
    bind all <Control-=> {dispsthg}
}

#########################
# End of debug settings #
#########################

set winTitle "SciPad"
set version "Version 5.28"

# all one needs in order to add a new retrievable preference is:
#  - add the variable name to $listofpref below, if it is not a list
#  - add the variable name to $listofpref_list below, if it is a list
#  - use the variable when needed in the program, such that 
#    it is visible at main level (i.e. globalize it)
#  - if needed, assign an initial fallback value here

# group here for convenience all the color settings
set bgcolors {BGCOLOR SELCOLOR BREAKPOINTCOLOR FOUNDTEXTCOLOR}
set fgcolors {FGCOLOR CURCOLOR PARCOLOR BRAKCOLOR \
       BRACCOLOR PUNCOLOR KEYWCOLOR OPCOLOR QTXTCOLOR \
       REMCOLOR XMLCOLOR NUMCOLOR LFUNCOLOR PDEFCOLOR}
set colorpref "$bgcolors $fgcolors"

# those are the preferences which are going to be saved
set listofpref "$colorpref wordWrap FontSize \
       WMGEOMETRY printCommand actbptextFont indentspaces \
       filenamesdisplaytype maxrecentfiles scilabSingleQuotedStrings \
       tabinserts lang"
set listofpref_list { listofrecent }

# default options which can be overriden
set wordWrap "none"
set BGCOLOR "snow1"
set FGCOLOR "black"
set CURCOLOR "red"
set PARCOLOR "magenta3"
set BRAKCOLOR "DarkGoldenrod4"
set BRACCOLOR "red"
set PUNCOLOR "turquoise4"
set KEYWCOLOR "blue2"
set PDEFCOLOR "purple"
set LFUNCOLOR \#006874
set OPCOLOR "blue4"
set QTXTCOLOR "red"
set REMCOLOR "green4"
set XMLCOLOR "orange"
set NUMCOLOR "yellow4"
set SELCOLOR "PaleGreen"
set BREAKPOINTCOLOR "pink"
set FOUNDTEXTCOLOR "green2"
set FontSize 12
set WMGEOMETRY 600x480
set printCommand lpr
set actbptextFont "-Adobe-courier-bold-R-Normal-*-[expr $FontSize + 2]-*"
set indentspaces 2
set filenamesdisplaytype "pruned"  ;# "pruned" or "full" or "fullifambig"
set maxrecentfiles 4
set listofrecent [list]    ;# always full filenames here
set scilabSingleQuotedStrings "yes"
set tabinserts "spaces"    ;# "spaces" or "tabs"

# other non-pref initial settings
if { ![info exists lang] } { set lang "eng" }
set Scheme scilab
if {$tcl_platform(platform) != "unix"} {
    set cursorblink "true"
} else {
    set cursorblink "false"
}
set mouseoversel "false"
set dndinitiated "false"

# this array will contain frame ids such that $pwframe($textarea) is the
# frame pathname in which $textarea is packed, or "none" if it is not packed
array unset pwframe

# source the user preferences file if any
set preffilename $env(SCIHOME)/.SciPadPreferences.tcl
catch {source $preffilename}

set REPLACEDTEXTCOLOR $FOUNDTEXTCOLOR
set FAKESELCOLOR $SELCOLOR

# message files and localization to avoid ifs on $lang
package require msgcat
namespace import -force msgcat::*
::msgcat::mclocale "$lang"
::msgcat::mcload [file join "$env(SCIPATH)" "tcl" "scipadsources" "msg_files"]

# drag and drop capability using TkDnD
if { [catch {package require tkdnd}] == 0 } {
    # package is present and loaded
    set TkDnDloaded "true"
} else {
    set TkDnDloaded "false"
}

# variable used to track changes to the initial buffer
# so that Scipad can automatically close it if first
# action in Scipad is a successful file/open
set closeinitialbufferallowed true

# The following must be unset otherwise the goto line box can produce errors
# when it is opened again with the same file that was opened before in another
# textarea
catch {unset physlogic linetogo curfileorfun funtogoto}
