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

set winTitle "SciPad"

#############
# Flags to trim Scipad to the host environment
# (host computer OS, underlying Tcl/Tk version...)


# On Vista (osVersion is 6.0) there is bug 2672 which happens with the 8.4
# series of Tcl/Tk only
# This flag allows for easy checking whether the workaround for this
# bug must be geared in or not

if {$tcl_platform(platform) == "windows" && $tcl_platform(osVersion) == "6.0" && !$Tk85} {
    # don't use the -parent option in tk_get*File
    set bug2672_shows_up true
} else {
    # use the -parent option in tk_get*File
    set bug2672_shows_up false
}


# On Linux with Tk8.5, there is bug 2776 which prevents from choosing
# multiple files in the tk_getOpenFile dialog
# This is a Tk bug, see the full details at:
# http://sourceforge.net/tracker/index.php?func=detail&aid=1904322&group_id=12997&atid=112997
# Here is the workaround - This should be removed when Tk bug 112997 gets
# fixed in Tk 8.5.x or at least it should be tuned so that it is geared
# in only for Tk 8.5.0 -> Tk 8.5.x but not in Tk 8.5.y with y > x

if {$tcl_platform(platform) == "unix" && $Tk85} {
    # gear in the workaround for Tk bug 112997
    option add *__tk_filedialog*takeFocus 0
} else {
    # nothing to do
}


# End of flags to trim Scipad to the host environment
# (host computer OS, underlying Tcl/Tk version...)
#############


#############
# Scilab version detection
# Scilab 5 or Scilab 4?
# Set flags value here, it will be useful throughout the rest of the code!
# The test is on whereis(scipad)=="utillib"|"scipadlib"
# This test is also consistent with scilab-gtk (i.e. Scilab5 will be set
# to false when running aside scilab-gtk)

# [SYNCEXEC] Patch start
# [SYNCEXEC] {
# [SYNCEXEC] Calling TCL_Eval* will freeze Scilab : Deadlock process
# [SYNCEXEC] Scilab Already know if we are in Scilab 4 or 5,
# [SYNCEXEC] We may set this Variable in scipad.sci instead ?

# if {$standaloneScipad} {
#     # no possible test, since launching Scipad outside of Scilab is used
#     # for debugging it, it is safe to fool Scipad by telling it Scilab 5
#     # environment is around
#     set Scilab5 true
#     set Scilab4 false
# } else {
#     set comm1 "if (whereis(scipad)==\"utillib\") then "
#     set comm2 "  TCL_SetVar(\"Scilab5\",\"false\",\"scipad\");"
#     set comm3 "  TCL_SetVar(\"Scilab4\",\"true\",\"scipad\");"
#     set comm4 "else"
#     set comm5 "  TCL_SetVar(\"Scilab5\",\"true\",\"scipad\");"
#     set comm6 "  TCL_SetVar(\"Scilab4\",\"false\",\"scipad\");"
#     set comm7 "end"
#     set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7]
#     ScilabEval_lt $fullcomm "sync" "seq"
# }

# [SYNCEXEC] Forcing value to what should have been returned.
set Scilab5 true
set Scilab4 false
# [SYNCEXEC] }
# [SYNCEXEC] Patch end


# End of Scilab version detection
#############


#############
# Flags to trim Scipad to the host Scilab version

# On Windows the cursor blink was once disabled because of what is explained at
# http://groups.google.fr/group/comp.soft-sys.math.scilab/browse_thread/thread/b07a13adc073623d/b4e07072205c0435
# On Linux the cursor blink was once disabled because of bug 865 (originated in
# bug 473)
# now both issues have been fixed, therefore:

set cursorblink true


# The break command can only be used with Scilab versions having bug 2384 fixed
# Currently (26/08/07), this is done in svn trunk and BUILD4 branches
# but nowhere else, e.g. in scilab-gtk
# Checked again (19/09/08) in scilab-gtk-4.2: it is fixed
# The flag below allows for easy adjustment of Scipad to Scilab versions,
# especially with backported Scipad versions in mind
# Bug 2384 in fact fixes the sync seq options of ScilabEval (interruptibility)

set bug2384_fixed true


# Removing all breakpoints at once is most efficiently done by a single delbpt()
# However, there is bug 2474 in the way: this command corrupts the stack,
# although this is reported by Scilab only when inside a loop
# Currently (26/08/07) this bug is fixed in svn trunk and BUILD4 branches
# but nowhere else, e.g. in scilab-gtk
# Checked again (19/09/08) in scilab-gtk-4.2: it is fixed

set bug2474_fixed true


# The debugger has been broken by the operational team of the Scilab Consortium
# and they could not fix the situation on time for Scilab 5 release, despite
# Scilab 5 has been delayed more than one year. Their desperate request is that
# the debugger be unplugged from Scilab 5...

if {$Scilab5 && !$standaloneScipad} {
    set bug2789_fixed false
} else {
    # Scilab 4.x, Scilab-gtk, or stand-alone Scipad
    set bug2789_fixed true
}


# There are other bugs, such as:
#
#   2226 (concurrent launch of many Scipad): Fixed in trunk, not fixed in BUILD4
#        nor in scilab-gtk as of 26/08/07 - Checked again 19/09/08: ditto in scilab-gtk-4.2
#
#   2393 (macrovar doesn't reset lasterror): Fixed in trunk and BUILD4, not fixed
#        in scilab-gtk as of 26/08/07 - Checked again 19/09/08: fixed in scilab-gtk-4.2
#
# but there is no workaround implemented in Scipad for them, therefore no flag here

# End of flags to trim Scipad to the host Scilab version
#############


#############
# Saved preferences
#
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
       usekeywordindent filenamesdisplaytype maxrecentfiles maxrecentencodings \
       scilabSingleQuotedStrings tabinserts lang completionbinding \
       showContinuedLines filebackupdepth bindstyle doubleclickscheme \
       colorizeenable windowsmenusorting linenumbersmargins \
       ScilabErrorMessageBox colorizeuserfuns showclosureXcross \
       exitwhenlastclosed autodetectencodinginxmlfiles"
set listofpref_list { listofrecentfiles listofrecentencodings textFont menuFont }

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
set maxrecentfiles 15
set maxrecentencodings 5
set listofrecentfiles [list]    ;# always full filenames here
set listofrecentencodings [list]
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
set colorizeuserfuns "yes"
set showclosureXcross true
set exitwhenlastclosed false
set autodetectencodinginxmlfiles true

# End of saved preferences
#############

setdefaultfonts

#############
# Other non-preferences initial settings

# Select the Scilab locale when starting Scipad for the first time
# The test ![info exists lang] should always be true
# The lang variable is set to the getlanguage() result from Scilab
# If a preferences file exists, the value of the pref file will overwrite this
# because the pref file is sourced later
# This way the code portion below is really useful only when Scipad is started
# for the very first time from a fresh Scilab installation
# Precautions are taken (try;end) against Scilab versions not havng a clue about
# getlanguage() so that this code can be used with no change in Scilab 4.x
# backports
if {![info exists lang]} {

    # try to use the same locale as Scilab
# [SYNCEXEC] Patch start
# [SYNCEXEC] {
# [SYNCEXEC] Just comment the call for now
# [SYNCEXEC] We might set this variable in scipad.sci before starting.

    #ScilabEval_lt "try;TCL_SetVar(\"lang\",convstr(getlanguage(),\"l\"),\"scipad\");end" "sync" "seq"

# [SYNCEXEC] }
# [SYNCEXEC] Patch end

    if {![info exists lang]} {
        # Select a fallback in case setting the locale through getlanguage() failed
        setdefaultScipadlangvar
    } else {
        # Select a fallback when the Scilab language is not available in Scilab
        # In fact the english locale must be set explicitely as a fallback here
        # this is because later the locale is set by ::msgcat::mclocale "$lang"
        # and when ::msgcat::mc will try to translate the string using this unknown
        # locale it will trigger ::msgcat::mcunknown which will return the label.
        # Since this label is the english string, we could be happy with this, but
        # it is not *always* the case, sometimes the label is not the english string
        if {[lsearch [getavailablelocales] $lang] == -1} {
            setdefaultScipadlangvar
        } else {
            # success: Scilab's locale is available in Scipad and will be used
        }
    }
}

set Scheme scilab
set ColorizeIt true

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

# default encoding is the system native encoding
set defaultencoding [encoding system]
set currentencoding $defaultencoding

#############
# source the user preferences file if any
# this must happen after the locale selection from Scilab's getlanguage() above
set preffilename $env(SCIHOME)/.SciPadPreferences.tcl
catch {source $preffilename}
#############

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


#############
# Additional packages

# add path for finding the additional packages in Scilab binary versions
# (bug 3806) - the paths organization is not the same in binary versions
# and in the development tree
# for tkdnd:   $env(SCIINSTALLPATH)/modules/tclsci/tcl
# for msgcat:  currently no longer distributed with Scilab (bug 3805) <TODO> fix this!
lappend ::auto_path $env(SCIINSTALLPATH)/modules/tclsci/tcl

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
    if {[::msgcat::mcload $msgsdir] == 0} {
        # no msg file found for the current locale (bug 3781)
        set lang "en_us"
        ::msgcat::mclocale "$lang"
        ::msgcat::mcload $msgsdir
    } else {
        # nothing to do, mcload succeeded at the first place
    }
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
    proc mcmax {args} {return [eval "::msgcat::mcmax $args"]}
}


# drag and drop capability using TkDnD
if { [catch {package require tkdnd}] == 0 } {
    # package is present and loaded
    set TkDnDloaded "true"
} else {
    set TkDnDloaded "false"
}


# scrolledframe (this is used in the breakpoints properties dialog)
namespace import ::scrolledframe::scrolledframe

# End of additional packages
#############


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

# single place in Scipad where the bugzilla URL is defined
set ScilabBugzillaURL {http://bugzilla.scilab.org}

# the most recent backported Scipad version can be found at this URL
# this link points to Scipad_6.152.BP1
set latestbackportedscipadversionURL {http://www.scilab.org/contrib/index_contrib.php?page=displayContribution&fileID=1130}

# URL where the Scipad debugger internals are described
set scipaddebuggerinnerworkingsdescriptionURL {http://wiki.scilab.org/Scipad_debugger_inner_beauties}

##########################################################################
# Regular expression patterns
# These are globals since used at different places of the code, or because
# they are rather generic

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
append sfdlRE {\mfunction\M[[:blank:]]*} $scontRE $saslRE

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

# Strictly positive integer number, with no leading zero, or zero itself,
# reporting
set strictlypositiveintegerREpat {\A((0)|([1-9][0-9]*))\Z}

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

# regular expression matching the start of an XML prolog and reporting
# an encoding name in that prolog
# see the XML specification http://www.w3.org/TR/xml
# see also use of this RE in proc detectencoding
set xml_prologstart_RE_rep {<\?xml[[:blank:]]+version[[:blank:]]*=[[:blank:]]*["']1.[[:digit:]]+["'][[:blank:]]+encoding[[:blank:]]*=[[:blank:]]*["']([[:alpha:]][\w.-]+)["']}

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
