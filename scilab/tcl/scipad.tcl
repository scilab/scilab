#!/bin/sh
# the next line restarts using wish \
# exec `which wish` "$0" "$@"

if { [info exists pad] } { 
    raise $pad
    update
} else {
    set pad .scipad

#ES 12/6/04: load first all the proc source files
    set sourcedir [file join "$env(SCIPATH)" "tcl" "scipadsources"]

    source [file join $sourcedir popupmenus.tcl]
    source [file join $sourcedir scilabexec.tcl]
    source [file join $sourcedir whichfun.tcl]
    source [file join $sourcedir filecommands.tcl]
    source [file join $sourcedir print.tcl]
    source [file join $sourcedir cutcopypaste.tcl]
    source [file join $sourcedir findreplace.tcl]
    source [file join $sourcedir buffernavigation.tcl]
    source [file join $sourcedir colorize.tcl]
    source [file join $sourcedir modselection.tcl]
    source [file join $sourcedir inputtext.tcl]
    source [file join $sourcedir helps.tcl]
    source [file join $sourcedir textarea.tcl]
    source [file join $sourcedir infomessages.tcl]
    source [file join $sourcedir undoredo.tcl]
    source [file join $sourcedir debugger.tcl] 

#ES 12/06/04: now all the pure main level code
    source [file join $sourcedir defaults.tcl]
    source [file join $sourcedir mainwindow.tcl]
    source [file join $sourcedir menues.tcl]
    source [file join $sourcedir bindings.tcl]


#added by Matthieu PHILIPPE 21/11/2001 from linenum.pth
    $textareacur mark set insert "1.0"
#tkTextSetCursor $textareacur "1.0"
    keyposn $textareacur
##geometry in what units? The width is more than 65 columns, though it's 
## resized proportionally
#wm geometry $pad 65x24 

# catch the kill of the windowmanager
    wm protocol $pad WM_DELETE_WINDOW exitapp

    load_words
    update idletasks
}


