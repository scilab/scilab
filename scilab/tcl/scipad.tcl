#!/bin/sh
# the next line restarts using wish \
# exec `which wish` "$0" "$@"

if { [info exists pad] } { 
    raise $pad
    update
} else {
    set pad .scipad

toplevel $pad

set fileName " "
set textareacur $pad.textarea
set saveTextMsg 0

set sourcedir [file join "$env(SCIPATH)" "tcl" "scipadsources"]

source [file join $sourcedir defaults.tcl]

# added by Matthieu PHILIPPE
if { ![info exists lang] } { set lang "eng" }
set chset()                 {} 
set words()                 {}  
set listoffile("$pad.textarea",filename)   "[pwd]/Untitled.sce"
set listoffile("$pad.textarea",save) 0; 
# set flag to know if the editing file was change by user
set listoffile("$pad.textarea",new) 1; # is not an opened file from disk
set listoffile("$pad.textarea",thetime) 0; # set the time of the last modify
# set the scheme for colorization and allowed operations
set listoffile("$pad.textarea",language) "scilab"; 
set listoftextarea $pad.textarea
set winopened 0
set radiobuttonvalue 0

source [file join $sourcedir mainwindow.tcl]

source [file join $sourcedir menues.tcl]

source [file join $sourcedir popupmenus.tcl]

source [file join $sourcedir scilabexec.tcl]

source [file join $sourcedir debugger.tcl]

source [file join $sourcedir whichfun.tcl]

source [file join $sourcedir textarea.tcl]

source [file join $sourcedir infomessages.tcl]

source [file join $sourcedir filecommands.tcl]

source [file join $sourcedir helps.tcl]

source [file join $sourcedir print.tcl]

source [file join $sourcedir cutcopypaste.tcl]

source [file join $sourcedir findreplace.tcl]

source [file join $sourcedir buffernavigation.tcl]

source [file join $sourcedir bindings.tcl]

source [file join $sourcedir undoredo.tcl]

source [file join $sourcedir colorize.tcl]

source [file join $sourcedir modselection.tcl]

source [file join $sourcedir inputtext.tcl]

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


