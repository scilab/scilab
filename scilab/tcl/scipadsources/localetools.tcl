proc amp {s} {
# Given a string s containing an ampersand (&),
# this provides a list {n clean_string}
# n is the char index just after the ampersand
# clean_str is the string without the ampersand
# Reference: http://wiki.tcl.tk/3665
    set i [string first & $s]
    return [list $i [string replace $s $i $i]]
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

proc relocalize {} {
global lang sourcedir
# carry on all the changes when the locale is changed on the fly via
# the Options/Locale menu
    ::msgcat::mclocale $lang
    ::msgcat::mcload [file join $sourcedir msg_files]
    createmenues
    modifiedtitle [gettextareacur]
    keyposn [gettextareacur]
#labels in opened non-modal dialogs are not updated, but let's not 
# pretend too much... Same for the title of unsaved buffers named 
# UntitledN.sce
}
