set winTitle "SciPad"

# detect Tcl and Tk version and set global flags to true if version is >= 8.5
# this is used to improve Scipad when used with recent Tcl/Tk without
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

# all one needs in order to add a new retrievable preference is:
#  - add the variable name to $listofpref below, if it is not a list
#  - add the variable name to $listofpref_list below, if it is a list
#  - use the variable when needed in the program, such that 
#    it is visible at main level (i.e. globalize it)
#  - if needed, assign an initial fallback value here

# group here for convenience all the color settings
set bgcolors {BGCOLOR BGLNMARGCOLOR SELCOLOR \
        BREAKPOINTCOLOR FOUNDTEXTCOLOR CONTLINECOLOR}
set fgcolors {FGCOLOR FGLNMARGCOLOR CURCOLOR PARCOLOR BRAKCOLOR \
        BRACCOLOR PUNCOLOR INTFCOLOR COMMCOLOR OPCOLOR QTXTCOLOR \
        REMCOLOR XMLCOLOR NUMCOLOR LFUNCOLOR PDEFCOLOR SCICCOLOR \
        USERFUNCOLOR}
set colorpref "$bgcolors $fgcolors"

# those are the preferences which are going to be saved
set listofpref "$colorpref wordWrap \
       WMGEOMETRY WMSTATE printCommand indentspaces tabsizeinchars \
       usekeywordindent \
       filenamesdisplaytype maxrecentfiles scilabSingleQuotedStrings \
       tabinserts lang completionbinding showContinuedLines \
       filebackupdepth bindstyle doubleclickscheme colorizeenable \
       windowsmenusorting linenumbersmargins ScilabErrorMessageBox"
set listofpref_list { listofrecent textFont menuFont }

# default options which can be overriden
set wordWrap "none"
set BGCOLOR "snow1"
set BGLNMARGCOLOR lightgrey
set FGCOLOR "black"
set FGLNMARGCOLOR black
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
set USERFUNCOLOR {#02a5f2}
set WMGEOMETRY 600x480
set WMSTATE normal
set printCommand lpr
set indentspaces 2
set tabsizeinchars 4
set usekeywordindent 1  ; # use smart keyword indentation: 0 (no) or 1 (yes)
set filenamesdisplaytype "pruned"  ;# "pruned" or "full" or "fullifambig"
set maxrecentfiles 4
set listofrecent [list]    ;# always full filenames here
set scilabSingleQuotedStrings "yes"
set tabinserts "spaces"    ;# "spaces" or "tabs"
set completionbinding "Control-Tab"
set showContinuedLines "yes"
set filebackupdepth 0
set bindstyle "mac-pc"
set doubleclickscheme "Scilab"  ;# "Scilab" or "Windows" or "Linux"
set colorizeenable "always"     ;# "always" or "ask" or "never"
set windowsmenusorting "openorder" ; # "openorder", or "alphabeticorder", or "MRUorder"
set linenumbersmargins "right" ; # "hide" (line numbers are not displayed), or "left" (they are left-aligned), or "right" (right-aligned)
set ScilabErrorMessageBox true

setdefaultfonts

# other non-pref initial settings

if { ![info exists lang] } { set lang "eng" }

set Scheme scilab
set ColorizeIt true

# On Windows the cursor blink was once disabled because of what is explained at
# http://groups.google.fr/group/comp.soft-sys.math.scilab/browse_thread/thread/b07a13adc073623d/b4e07072205c0435
# On Linux the cursor blink was once disabled because of bug 865 (originated in
# bug 473)
# now both issues have been fixed, therefore:
set cursorblink true

# by default, the cursor in textareas is the insert cursor
set textinsertmode true
set textinsertcursorwidth 3         ; # in pixels
set textinsertcursorborderwidth 2   ; # in pixels
set textreplacecursorwidth 6        ; # in pixels
set textreplacecursorborderwidth 0  ; # in pixels

# Drag and drop feature initial state
set mouseoversel "false"
set dndinitiated "false"

# this array will contain frame ids such that $pwframe($textarea) is the
# frame pathname in which $textarea is packed, or "none" if it is not packed
array unset pwframe

# source the user preferences file if any
set preffilename $env(SCIHOME)/.SciPadPreferences.tcl
catch {source $preffilename}

# ensure the menu option setting for line numbers is consistent with
# the default or the value from the preferences file
set linenumbersmarginsmenusetting $linenumbersmargins

# recompute $textfontsize and $menufontsize from the preferences file fonts
if {[lsearch $textFont "-size"] != -1} {
    set textfontsize [lindex $textFont \
            [expr {[lsearch $textFont "-size"] + 1}] ]
    set textfontsize [expr {round($textfontsize * [tk scaling])}]
} else {
    set textfontsize 12
}
if {[lsearch $menuFont "-size"] != -1} {
    set menufontsize [lindex $menuFont \
            [expr {[lsearch $menuFont "-size"] + 1}] ]
    set menufontsize [expr {round($menufontsize * [tk scaling])}]
} else {
    set menufontsize 12
}

# message files and localization to avoid ifs on $lang
if {[catch {package require msgcat}] == 0} {
    # package is present and loaded
    namespace import -force msgcat::*
    ::msgcat::mclocale "$lang"
# the names of the locales are common for all languages (each one is the
# native language name), and are defined in a separate file.
# the common definition can anyway be overridden by a definition in the
# $msgsdir/$lang.msg file
    source [file join "$msgsdir" "localenames.tcl"]
    ::msgcat::mcload $msgsdir
} else {
    # package is not present, define default fallbacks
    namespace eval msgcat {
        proc ::msgcat::mc {str} {return $str}
        proc ::msgcat::mclocale {args} {
            tk_messageBox -message "Package msgcat is not present - Localization features are disabled." \
                          -icon warning -title "Tcl msgcat package not found"
        }
        proc ::msgcat::mcload {args} {}
        proc ::msgcat::mcset {args} {}
        proc ::msgcat::mcmax {args} {
            set le 0
            foreach arg $args {
                set argle [string length $arg]
                if {$argle > $le} {set le $argle}
            }
            return $le
        }
    }
    proc mc {str} {return $str}
    proc mcmax {args} {return [eval {::msgcat::mcmax $args}]}
}

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

# variable used to prevent more than one single instance of any of the tile
# procs from running concurrently, e.g. maximize and splitwindow
# for some unknown reason, disabling the bindings in proc disablemenuesbinds
# does not *always* prevent concurrent running, so this was needed
set tileprocalreadyrunning false

# guard variable used to prevent more than one simultaneous launch of the
# find/replace box, which can happen during startup when hammering
# Scipad with ctrl-r ctrl-f or the opposite
set findreplaceboxalreadyopen false

# variable used to prevent launching simultaneously multiple searches in files
# during search in file, the other functionalities of Scipad are however enabled
set searchinfilesalreadyrunning 0

# variable used to detect buffer changes between two Find Next commands
# with F3
set buffermodifiedsincelastsearch false

# default find/replace direction
set SearchDir "forwards"

# find/replace color tags
set REPLACEDTEXTCOLOR $FOUNDTEXTCOLOR
set FAKESELCOLOR $SELCOLOR

# no item is selected by default in the tags list of the find/replace dialog
set listoftagsforfind [list ]

# some commands cannot be executed while colorization is in progress, and
# this variable is used to prevent them from being executed in such a case
set nbfilescurrentlycolorized 0

# identifier of the progressbar for background colorization - increments only
set progressbarId 0

# Scilab limit for the length of names (see help names)
set maxcharinascilabname 24

# this variable is used to restore the word wrap mode after a block selection
set blockseltoggledwordwrap false

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

# Negated Scilab names character class (non reporting)
# Note: this is neither the negation of $sncc1RE nor the negation of $sncc2RE
set notsnccRE {(?:[^\w%#!?$])}
# Negated Scilab names character class lookahead
set notsncclookaheadRE {(?=[^\w%#!?$])}

# Scilab names regexp, reporting version and non-reporting version
set snRE_rep {}
append snRE_rep {(} $sncc1RE $sncc2RE * {)}
set snRE {}
append snRE {(?:} $sncc1RE $sncc2RE * {)}

# Any number of blanks (spaces or tabs), possibly none
set sblRE {[[:blank:]]*}

# Scilab comment, reporting version and non-reporting version
set scommRE_rep {(//[^\n]*)}
set scommRE {(?://[^\n]*)}

# Scilab continuation mark
# Note: more than one continued line is a single match
# scontRE does not need an actual continued line to match (* case)
# scontRE1 does need an actual continued line to match (+ case)
set scontRE {}
append scontRE {(?:} $sblRE {\.{2,}} $sblRE $scommRE {?} {\n} {)} {*}
set scontRE1 {}
append scontRE1 {(?:} $sblRE {\.{2,}} $sblRE $scommRE {?} {\n} {)} {+}
set scontRE2 {}
append scontRE2 {(?:} $scontRE1 $sblRE {)}

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

# Scilab left part of an assignment such as "[a,b,c] =" or just "a"
set saslRE {}
append saslRE {(?:} {(?:} $sbklnRE {|} $snRE {)} $scontRE $sblRE {=} $scontRE {)} {?} $sblRE

# Scilab right part of an assignment, without the function name, such as "(a,b,c) =" or just "a" or "()"
set sasrRE {}
append sasrRE $sblRE $scontRE $sblRE {(?:} $spalnRE {)} {?} $scontRE $sblRE {(?:} {[;,]} {|} {(?:} $sblRE $scommRE {)} {|} {\n} {)}

# Scilab function definition regexp (left part, i.e. up to but not including function name)
set sfdlRE {}
append sfdlRE {\mfunction\M[[:blank:]]+} $scontRE $saslRE

# Scilab function definition regexp (right part, i.e. from but not including function name)
set sfdrRE {}
append sfdrRE $sasrRE

set funlineREpat1 $sfdlRE
set funlineREpat2 $sfdrRE
set scilabnameREpat $snRE_rep

# regular expression matching a line starting with a return statement of a
# Scilab function, i.e. either
#   endfunction          or
#   [..] = resume(...)   or
#   [..] = return(...)
# note that the regexp starts with the \A constraint, i.e it matches at the
# beginning of the string, therefore the text passed to the regexp engine
# must be a single line of a scilab text buffer (and not an entire buffer)
# if this is a nuisance, replace \A by ^ and use regexp -line option
# note also that we don't care about what is after the return statement, we
# only need the return statement be the first executable instruction of the
# line
set sresRE {}
append sresRE {\A} $sblRE {(} {(endfunction)} {|}
append sresRE                 {(} $saslRE {(return)|(resume)} $sasrRE {)}
append sresRE             {)}

###

# Floating point number, reporting version and non-reporting version
set floatingpointnumberREpat_rep {((\.\d+)|(\m\d+(\.\d*)?))([deDE][+\-]?\d{1,3})?\M}
set floatingpointnumberREpat {(?:(?:\.\d+)|(?:\m\d+(?:\.\d*)?))(?:[deDE][+\-]?\d{1,3})?\M}

# Rational number, reporting version
set rationalnumberREpat_rep      {((\.\d+)|(\m\d+(\.\d*)?))\M}

###

# regular expression matching a continued line identified as such because
# it has trailing dots possibly followed by a comment
set dotcontlineRE_rep {(^([^/]/?)*\.{2,} *(//.*)?$)}
set dotcontlineRE {(?:^(?:[^/]/?)*\.{2,} *(?://.*)?$)}

# maximum level of nesting for brackets and braces
# if a higher level of nesting exists in a continued line of the textarea,
# then continued lines detection algorithm will be fooled
set constructsmaxnestlevel 3

# regular expression matching a continued line identified as such because
# it has unbalanced brackets possibly followed by a comment
set bracketscontlineRE {(?:^(?:(?:[^/"']/?)*(?:[^\w%#!?$]["'][^"']*["'])*)*\[[^\]]*(?:(?:(?:}
append bracketscontlineRE [createnestregexp $constructsmaxnestlevel {[} {]}]
append bracketscontlineRE {)*[^\]]*)*\n)+(?:(?:[^/]/?)*\.{2,} *(?://.*)?\n)*)}

# regular expression matching a continued line identified as such because
# it has unbalanced braces possibly followed by a comment
set bracescontlineRE   {(?:^(?:(?:[^/"']/?)*(?:[^\w%#!?$]["'][^"']*["'])*)*\{[^\}]*(?:(?:(?:}
append bracescontlineRE [createnestregexp $constructsmaxnestlevel "{" "}"]
append bracescontlineRE {)*[^\}]*)*\n)+(?:(?:[^/]/?)*\.{2,} *(?://.*)?\n)*)}

###

# Scilab string, delimited by single quotes or double quotes, with no
# continuation  dots nor comments (for this see proc colorizestringsandcomments_sd)
# reporting version and non-reporting version
set sstrRE   {(?:(?:["'][^"']*["'])+)}
set sstrRE_rep {((?:["'][^"']*["'])+)}

# Scilab matrix of strings (with no continuation dots nor comments)
set smstRE_rep {}
append smstRE_rep {\[} $sblRE {(?:} $sstrRE $sblRE {[,;]?} $sblRE {)+} {\]}

# Scilab matrix of strings or string (all with no continuation dots nor comments)
set ssmsRE {}
append ssmsRE {(?:} $smstRE_rep {)|(?:} $sstrRE {)}

###

# the user might want the same behaviour on Windows as on Linux for
# double-clicking - this is bug 1792, see also
# http://groups.google.fr/group/comp.lang.tcl/browse_thread/thread/659fd6c1f41d9a81/eb2a841ac335580e
# note: the need to say catch {tcl_endOfWord} is due to Tk bug 1517768
# with this catch, tcl_wordchars and tcl_nonwordchars become known to wish
# however, the need to do this leads to infinite loop when $debuglog == true
# the second time Scipad is launched in the same Scilab session
# no attempt to fix this is made because it only happens in debuglog
# mode (due to the renaming of the procs) and because it is a Tk bug
# to have to say catch {tcl_endOfWord}
if {![info exists tcl_wordchars]} {
    catch {tcl_endOfWord}
}
set tcl_wordchars_linux {[a-zA-Z0-9_]}
set tcl_nonwordchars_linux {[^a-zA-Z0-9_]}
set tcl_wordchars_windows {\S}
set tcl_nonwordchars_windows {\s}
set tcl_wordchars_scilab {[\w%#!?$]}
set tcl_nonwordchars_scilab {[^\w%#!?$]}
updatedoubleclickscheme

# End of regular expression patterns
##########################################################################
