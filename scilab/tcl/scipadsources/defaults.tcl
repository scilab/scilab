set winTitle "SciPad"
set version "Version 6.0"


# detect Tcl and Tk version and set global flags to true if version is >= 8.5
# this is used to improve Scipad when used with recent Tcl/Tk without
# preventing its use with older ladies
# ex of 8.5 use: -strictlimits option in find/replace
#                -stretch always option for panedwindows
#                proc timestamp uses clock milliseconds
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

# all one needs in order to add a new retrievable preference is:
#  - add the variable name to $listofpref below, if it is not a list
#  - add the variable name to $listofpref_list below, if it is a list
#  - use the variable when needed in the program, such that 
#    it is visible at main level (i.e. globalize it)
#  - if needed, assign an initial fallback value here

# group here for convenience all the color settings
set bgcolors {BGCOLOR SELCOLOR BREAKPOINTCOLOR FOUNDTEXTCOLOR \
       CONTLINECOLOR}
set fgcolors {FGCOLOR CURCOLOR PARCOLOR BRAKCOLOR \
       BRACCOLOR PUNCOLOR INTFCOLOR COMMCOLOR OPCOLOR QTXTCOLOR \
       REMCOLOR XMLCOLOR NUMCOLOR LFUNCOLOR PDEFCOLOR SCICCOLOR}
set colorpref "$bgcolors $fgcolors"

# those are the preferences which are going to be saved
set listofpref "$colorpref wordWrap FontSize \
       WMGEOMETRY printCommand actbptextFont indentspaces \
       filenamesdisplaytype maxrecentfiles scilabSingleQuotedStrings \
       tabinserts lang completionbinding showContinuedLines \
       filebackupdepth"
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
set COMMCOLOR "magenta4"
set PDEFCOLOR "purple"
set LFUNCOLOR "cyan4"
set SCICCOLOR "OrangeRed"
set OPCOLOR "blue4"
set QTXTCOLOR "red4"
set REMCOLOR "green4"
set XMLCOLOR "orange"
set NUMCOLOR "yellow4"
set SELCOLOR "PaleGreen"
set BREAKPOINTCOLOR "pink"
set FOUNDTEXTCOLOR "green2"
set CONTLINECOLOR "lemonchiffon"
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
set completionbinding "Control-Tab"
set showContinuedLines "yes"
set filebackupdepth 0

# other non-pref initial settings

if { ![info exists lang] } { set lang "eng" }

set Scheme scilab

# On Windows the cursor blink was once disabled because of what is explained at
# http://groups.google.fr/group/comp.soft-sys.math.scilab/browse_thread/thread/b07a13adc073623d/b4e07072205c0435
# On Linux the cursor blink was once disabled because of bug 865 (originated in
# bug 473)
# now both issues have been fixed, therefore:
set cursorblink true

# Drag and drop feature initial state
set mouseoversel "false"
set dndinitiated "false"

# this array will contain frame ids such that $pwframe($textarea) is the
# frame pathname in which $textarea is packed, or "none" if it is not packed
array unset pwframe

# source the user preferences file if any
set preffilename $env(SCIHOME)/.SciPadPreferences.tcl
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

# variable used to track changes to the initial buffer
# so that Scipad can automatically close it if first
# action in Scipad is a successful file/open
set closeinitialbufferallowed true

# The following must be unset otherwise the goto line box can produce errors
# when it is opened again with the same file that was opened before in another
# textarea
catch {unset physlogic linetogo curfileorfun funtogoto}

# guard variable used to prevent more than one simultaneous launch of the
# find/replace box, which can happen during startup when hammering
# Scipad with ctrl-r ctrl-f or the opposite
set findreplaceboxalreadyopen false

# variable used to prevent more than one single instance of any of the tile
# procs from running concurrently, e.g. maximize and splitwindow
# for some unknown reason, disabling the bindings in proc disablemenuesbinds
# does not *always* prevent concurrent running, so this was needed
set tileprocalreadyrunning false

# variable used to prevent launching simultaneously multiple searches in files
# during search in file, the other functionalities of Scipad are however enabled
set searchinfilesalreadyrunning 0

# variable used to detect buffer changes between two Find Next commands
# with F3
set buffermodifiedsincelastsearch false

# default Find/Replace direction
set SearchDir "forwards"

# Find/Replace color tags
set REPLACEDTEXTCOLOR $FOUNDTEXTCOLOR
set FAKESELCOLOR $SELCOLOR

# some commands cannot be executed while colorization is in progress, and
# this variable is used to prevent them from being executed in such a case
set nbfilescurrentlycolorized 0

# identifier of the progressbar for background colorization - increments only
set progressbarId 0

##########################################################################
# Regular expression patterns
# These are globals since used at different places of the code

# Scilab names character class for first character of names
# From help names: Names of variables and functions must begin with a letter
# or one of the following special characters ' % ', ' _ ', ' # ', ' ! ', ' $ ', ' ? '
set sncc1RE {[[:alpha:]%_#!?$]}

# Scilab names character class for all but first character of names
# From help names: Next characters may be letters or digits or any special
# character in ' _ ', ' # ', ' ! ', ' $ ', ' ? '
# Note that \w already contains the underscore
set sncc2RE {[\w#!?$]}

# Scilab names regexp, reporting version and non-reporting version
set snRE_rep {}
append snRE_rep {(} $sncc1RE $sncc2RE * {)}
set snRE {}
append snRE {(?:} $sncc1RE $sncc2RE * {)}

# Any number of blanks (spaces or tabs), possibly none
set sblRE {[[:blank:]]*}

# Scilab comment
set scommRE {(?://[^\n]*)}

# Scilab continuation mark
# Note: more than one continued lines is a match
set scontRE {}
append scontRE {(?:} $sblRE {\.{2,}} $sblRE $scommRE {?} {\n} {)} {*}

# List of Scilab names separated by commas, allowing blanks and continuations anywhere
# Note: empty list is NOT a match
set slnRE {}
append slnRE $snRE {(?:} $scontRE $sblRE {,} $scontRE $sblRE $snRE {)*}

# List of Scilab names enclosed in brackets
# Note: empty brackets [] is a match
set sbklnRE {}
append sbklnRE {\[} $scontRE $sblRE {(?:} $slnRE {)?} $scontRE $sblRE {\]}

# List of Scilab names enclosed in parenthesis
# Note: empty parenthesis () is a match
set spalnRE {}
append spalnRE {\(} $scontRE $sblRE {(?:} $slnRE {)?} $scontRE $sblRE {\)}

# Scilab function definition regexp (left part, i.e. up to but not including function name)
set sfdlRE {}
append sfdlRE {\mfunction\M[[:blank:]]+} $scontRE {(?:} {(?:} $sbklnRE {|} $snRE {)} $scontRE $sblRE {=} $scontRE {)} {?} $sblRE

# Scilab function definition regexp (right part, i.e. from but not including function name)
set sfdrRE {}
append sfdrRE $sblRE $scontRE $sblRE {(?:} $spalnRE {)} {?} $scontRE $sblRE {(?:} {[;,]} {|} {(?:} $sblRE $scommRE {)} {|} {\n} {)}

set funlineREpat1 $sfdlRE
set funlineREpat2 $sfdrRE
set scilabnameREpat $snRE_rep

###

# regular expression matching a continued line identified as such because
# it has trailing dots possibly followed by a comment
set dotcontlineRE {(?:^(?:[^/]/?)*\.{2,} *(?://.*)?$)}

# maximum level of nesting for brackets and braces
# if a higher level of nesting exists in a continued line of the textarea,
# then continued lines detection algorithm will be fooled
set constructsmaxnestlevel 3

# regular expression matching a continued line identified as such because
# it has unbalanced brackets possibly followed by a comment
set bracketscontlineRE {(?:^(?:(?:[^/"']/?)*(?:["'][^"']*["'])*)*\[[^\]]*(?:(?:(?:}
append bracketscontlineRE [createnestregexp $constructsmaxnestlevel {[} {]}]
append bracketscontlineRE {)*[^\]]*)*\n)+(?:(?:[^/]/?)*\.{2,} *(?://.*)?\n)*)}

# regular expression matching a continued line identified as such because
# it has unbalanced braces possibly followed by a comment
set bracescontlineRE   {(?:^(?:(?:[^/"']/?)*(?:["'][^"']*["'])*)*\{[^\}]*(?:(?:(?:}
append bracescontlineRE [createnestregexp $constructsmaxnestlevel "{" "}"]
append bracescontlineRE {)*[^\}]*)*\n)+(?:(?:[^/]/?)*\.{2,} *(?://.*)?\n)*)}

# End of regular expression patterns
##########################################################################
