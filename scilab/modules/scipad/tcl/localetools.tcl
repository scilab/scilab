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
proc amp {s} {
# Given a string s containing an ampersand (&),
# this provides a list {n clean_string}
# n is the char index just after the ampersand
# clean_str is the string without the ampersand
# Reference: http://wiki.tcl.tk/3665
# Improved to provide the right index of the character to underline
# when the string contains one or more escape characters (\)
    set s_noescape [string map { "\\" "" } $s]
    set i_noescape [string first & $s_noescape]
    set i [string first & $s]
    return [list $i_noescape [string replace $s $i $i]]
}

proc me {mlabel} {
# Given a menu entry label containing an ampersand,
# this provides the -label and -underline options to use in menu commands
# It also takes care of the locale thanks to the msgcat package
    set tl [mc $mlabel]
    set clu [amp $tl]
    return "-label \"[lindex $clu 1]\" -underline [lindex $clu 0]"
}

proc bl {mlabel} {
# Given a button label containing an ampersand,
# this provides the -text and -underline options to use
# It also takes care of the locale thanks to the msgcat package
    set tl [mc $mlabel]
    set clu [amp $tl]
    return "-text \"[lindex $clu 1]\" -underline [lindex $clu 0]"
}

proc mcra {mlabel} {
# Given a menu entry label containing an ampersand,
# this provides the localized -label text
# It is equivalent to a mc command followed by removing the ampersand
    set tl [mc $mlabel]
    set clu [amp $tl]
    return [lindex $clu 1]
}

proc mcmaxra {args} {
# Given several strings each one containing or not an ampersand,
# this returns the length of the longest translated string, taking
# the ampersand case into account, i.e. :
#     max length among the ampersanded strings is mcmax of these strings
# minus 1 (because of the ampersand, that is supposed to be present also
# in the translated string if it is present in the untranslated string)
#     max length among the strings without ampersand is mcmax of these
# strings (an untranslated string with no ampersand is supposed to translate
# into a string without ampersand)
# return value is the maximum of these maxima
    set cmda   [list mcmax]
    set cmdnoa [list mcmax]
    # mcmax is applied separately on the elements that contain an ampersand
    # and on elements that do not contain an ampersand
    foreach arg $args {
        if {[string first "&" $arg] == -1} {
            lappend cmdnoa $arg
        } else {
            lappend cmda $arg
        }
    }
    set maxlen_amp   [eval $cmda]
    set maxlen_noamp [eval $cmdnoa]
    if {$maxlen_amp > $maxlen_noamp} {
        return [expr {$maxlen_amp - 1}]
    } else {
        return $maxlen_noamp
    }
}

proc fb {w} {
# Flexible binding ancillary
# Given a widget containing a -text (or -label) and a -underline option,
# this provides the lower case character to use in (alt) bindings in the
# dialog that contains the widget
# In any error case, the returned character is the ampersand, which is the
# less bad one to return since there will hopefully be no binding to it
    if { [catch {set blet [string index [$w cget -text] [$w cget -underline]]}] } {
        if { [catch {set blet [string index [$w cget -label] [$w cget -underline]]}] } {
            set blet "ampersand"
        }
    }
    if {$blet == ""} {set blet "ampersand"}
    return [string tolower $blet]
}

proc relocalize {} {
global lang msgsdir
# carry on all the changes when the locale is changed on the fly via
# the Options/Locale menu
    global pad listoftextarea
    ::msgcat::mclocale $lang
# the names of the locales are common for all languages (each one is the
# native language name), and are defined in a separate file.
# the common definition can anyway be overridden by a definition in the
# $msgsdir/$lang.msg file
    source [file join "$msgsdir" "localenames.tcl"]
    ::msgcat::mcload $msgsdir
    createmenues
    setdbmenuentriesstates_bp
    updatepanestitles
    set bestwidth [mcmaxra "Hide" \
                           "Close"]
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            [getpaneframename $ta].clbutton configure -text [mc "Close"] -width $bestwidth
            [getpaneframename $ta].hibutton configure -text [mc "Hide"]  -width $bestwidth
            # tooltips on hide and close buttons do not need to be updated
            # because localization occurs on the fly each time the tooltip
            # displays its string content
        }
    }
    focustextarea [gettextareacur]
# labels in opened non-modal dialogs are not updated, but let's not 
# pretend too much... Same for the title of unsaved buffers named 
# UntitledN.sce, and ditto for the call stack area text
}

proc listlocalize {inlist} {
# return a list which elements are the localized versions of $inlist
# in the current locale
    set outlist [list ]
    foreach elt $inlist {
        lappend outlist [mc $elt]
    }
    return $outlist
}

proc getavailablelocales {} {
# return a list of available Scipad locales
# this is achieved by scanning the names of the message files
    global msgsdir
    set loclist [list ]
    set msgFiles [lsort [globtails $msgsdir *.msg]]
    foreach m $msgFiles {
        lappend loclist [file rootname $m]
    }
    return $loclist
}

proc setdefaultScipadlangvar {} {
# set default lang variable
    global Scilab5 lang
    if {$Scilab5} {
        set lang "en_US"
    } else {
        set lang "eng"
    }
}
