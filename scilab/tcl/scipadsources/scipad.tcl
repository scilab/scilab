#!/bin/sh
# the next line restarts using wish \
exec `which wish` "$0" "$@"

if { [info exists pad] } { 

    wm deiconify $pad
    raise $pad
    update

} else {

    set pad .scipad

    set sourcedir [file join "$env(SCIPATH)" "tcl" "scipadsources"]

    # load first some debug settings
    source [file join $sourcedir scipaddebug1.tcl]

    # now all the proc source files
    source [file join $sourcedir popupmenus.tcl]
    source [file join $sourcedir scilabexec.tcl]
    source [file join $sourcedir whichfun.tcl]
    source [file join $sourcedir filecommands.tcl]
    source [file join $sourcedir print.tcl]
    source [file join $sourcedir cutcopypaste.tcl]
    source [file join $sourcedir findreplace.tcl]
    source [file join $sourcedir findinfiles.tcl]
    source [file join $sourcedir buffernavigation.tcl]
    source [file join $sourcedir colorize.tcl]
    source [file join $sourcedir completion.tcl]
    source [file join $sourcedir modselection.tcl]
    source [file join $sourcedir inputtext.tcl]
    source [file join $sourcedir helps.tcl]
    source [file join $sourcedir textarea.tcl]
    source [file join $sourcedir infomessages.tcl]
    source [file join $sourcedir undoredo.tcl]
    source [file join $sourcedir debugger.tcl] 
    source [file join $sourcedir localetools.tcl] 
    source [file join $sourcedir tkdndtools.tcl] 
    source [file join $sourcedir platformbind.tcl] 
    source [file join $sourcedir menues.tcl]
    source [file join $sourcedir progressbar.tcl]

    # now all the pure main level code
    source [file join $sourcedir defaults.tcl]
    source [file join $sourcedir mainwindow.tcl]
    source [file join $sourcedir db_init.tcl]
    source [file join $sourcedir bindings.tcl]

    # additional initial state operations, now that all the widgets have been set
    load_words

    createmenues

    setfontscipad $FontSize

    # set initial debug state
    setdbstate "NoDebug"

    # the following update makes background tasks work on Linux
    # since bug 865 is fixed
    update
    focustextarea [gettextareacur]

    # finally source debug settings that must be executed after the procs definition
    source [file join $sourcedir scipaddebug2.tcl]

    # deiconify "seq"entially so that this will be done after completion
    # of dynamickeywords
    # this way the user has no control on Scipad before the initialization
    # sequence is completely finished, including full execution of
    # dynamickeywords. This is needed to avoid starting to colorize a file
    # (when opened through the file menu) before the words and chset arrays
    # have been populated
    ScilabEval_lt "TCL_EvalStr(\"wm deiconify $pad\",\"scipad\")" "seq"
}
