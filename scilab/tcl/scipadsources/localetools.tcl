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
global lang sourcedir
# carry on all the changes when the locale is changed on the fly via
# the Options/Locale menu
    global pad listoftextarea
    ::msgcat::mclocale $lang
    ::msgcat::mcload [file join $sourcedir msg_files]
    createmenues
    setdbmenuentriesstates_bp
    updatepanestitles
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            [getpaneframename $ta].clbutton configure -text [mc "Close"]
        }
    }
    keyposn [gettextareacur]
# labels in opened non-modal dialogs are not updated, but let's not 
# pretend too much... Same for the title of unsaved buffers named 
# UntitledN.sce
}
