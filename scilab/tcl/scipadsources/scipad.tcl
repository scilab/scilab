#!/bin/sh
# the next line restarts using wish \
exec `which wish` "$0" "$@"

if { [info exists pad] } { 
    raise $pad
    update
} else {
    set pad .scipad

    set sourcedir [file join "$env(SCIPATH)" "tcl" "scipadsources"]

    # load first all the proc source files
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
    source [file join $sourcedir localetools.tcl] 
    source [file join $sourcedir tkdndtools.tcl] 

    # now all the pure main level code
    source [file join $sourcedir defaults.tcl]
    source [file join $sourcedir mainwindow.tcl]
    source [file join $sourcedir db_init.tcl]
    source [file join $sourcedir bindings.tcl]
    source [file join $sourcedir menues.tcl]

    #define ScilabEval to a void function, if it is unknown. This is
    # useful in order to run scipad outside of scilab (e.g. to debug it)
    if {[catch {ScilabEval ";"}] != 0} {
        proc ScilabEval args { 
            showinfo [mc "NOT CONNECTED TO SCILAB"]
            puts $args
        }
        set sciprompt 0
    }

    load_words

    keyposn $textareacur

    update idletasks
}


