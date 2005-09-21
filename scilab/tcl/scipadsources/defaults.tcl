set winTitle "SciPad"
set version "Version 5.39"


# detect Tk version and set a global flag to true if this version is >= 8.5
# this is used to improve Scipad when used with recent Tks without preventing
# use with older ladies (ex: -strictlimits option in find/replace)
if { [package vcompare $tk_version 8.5] >= 0 } {
    set Tk85 1
} else {
    set Tk85 0
}

# all one needs in order to add a new retrievable preference is:
#  - add the variable name to $listofpref below, if it is not a list
#  - add the variable name to $listofpref_list below, if it is a list
#  - use the variable when needed in the program, such that 
#    it is visible at main level (i.e. globalize it)
#  - if needed, assign an initial fallback value here

# group here for convenience all the color settings
set bgcolors {BGCOLOR SELCOLOR BREAKPOINTCOLOR FOUNDTEXTCOLOR}
set fgcolors {FGCOLOR CURCOLOR PARCOLOR BRAKCOLOR \
       BRACCOLOR PUNCOLOR INTFCOLOR COMMCOLOR OPCOLOR QTXTCOLOR \
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
set INTFCOLOR "blue2"
set COMMCOLOR "RoyalBlue1"
set PDEFCOLOR "purple"
set LFUNCOLOR \#006874
set OPCOLOR "blue4"
set QTXTCOLOR "darkred"
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

# variable used to prevent more than one single isntance of any of the tile
# procs from running concurrently, e.g. maximize and splitwindow
# for some unknown reason, disabling the bindings in proc disablemenuesbinds
# does not *always* prevent concurrent running, so this was needed
set tileprocalreadyrunning false
