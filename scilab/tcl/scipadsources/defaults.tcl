set winTitle "SciPad"
set version "Version 3.37"

# all one needs in order to add a new retrievable preference is:
#  - add the variable name to $listofpref below, if it is not a list
#  - add the variable name to $listofpref_list below, if it is a list
#  - use the variable when needed in the program, such that 
#    it is visible at main level (i.e. globalize it)
#  - if needed, assign an initial fallback value here

# those are the preferences which are going to be saved
set listofpref {wordWrap BGCOLOR FGCOLOR CURCOLOR PARCOLOR BRAKCOLOR \
       BRACCOLOR PUNCOLOR KEYWCOLOR OPCOLOR TXTCOLOR QTXTCOLOR \
       REMCOLOR XMLCOLOR NUMCOLOR SELCOLOR BREAKPOINTCOLOR FontSize \
       LFUNCOLOR PDEFCOLOR WMGEOMETRY printCommand actbptextFont indentspaces \
       filenamesdisplaytype maxrecentfiles }
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
set TXTCOLOR $FGCOLOR
set QTXTCOLOR "red"
set REMCOLOR "green4"
set XMLCOLOR "orange"
set NUMCOLOR "yellow4"
set SELCOLOR "PaleGreen"
set BREAKPOINTCOLOR "pink"
set FontSize 12 
set WMGEOMETRY 600x480
set printCommand lpr
set actbptextFont "-Adobe-courier-bold-R-Normal-*-[expr $FontSize + 2]-*"
set indentspaces 2
set filenamesdisplaytype "pruned"  ;# "pruned" or "full"
set maxrecentfiles 4
set listofrecent [list]    ;# always full filenames here

#other non-pref initial settings
if { ![info exists lang] } { set lang "eng" }
set Scheme scilab

# source the user preferences file if any
set preffilename $env(HOME)/.SciPadPreferences.tcl
catch {source $preffilename}

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
