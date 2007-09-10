#!/bin/sh
# the next line restarts using wish \
exec `which wish` "$0" "$@"

if { [info exists pad] } { 

    wm deiconify $pad
    raise $pad
    update

} else {

    set pad .scipad

    if {[file exists [file join  "$env(SCIINSTALLPATH)" "modules" "scipad"]]} {
  ## paths for scilab5
      set moduledir [file join "$env(SCIINSTALLPATH)" "modules" "scipad"]
      set sourcedir [file join "$moduledir" "tcl"]
      set intmacdir [file join "$moduledir" "macros" "scipad_internals"]
      set scicosdir [file join "$env(SCIINSTALLPATH)" "modules" "scicos" "macros" "scicos"]
      set blocksdir [file join "$env(SCIINSTALLPATH)" "modules" "scicos" "macros" "scicos_blocks"]
    } else { 
  ##paths for scilab4
      set moduledir [file join "$env(SCIINSTALLPATH)" "tcl" "scipadsources"]
      set sourcedir [file join "$moduledir"]
      set intmacdir [file join "$sourcedir"]
      set scicosdir [file join "$env(SCIINSTALLPATH)" "macros" "scicos"]
      set blocksdir [file join "$env(SCIINSTALLPATH)" "macros" "scicos_blocks"]
    }    

    set msgsdir   [file join "$sourcedir" "msg_files"]
    set iconsdir  [file join "$sourcedir" "icons"]
    set binddir   [file join "$sourcedir" "bindings"]

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
    source [file join $sourcedir fonts.tcl]
    source [file join $sourcedir tooltips.tcl]

    # now all the pure main level code
    source [file join $sourcedir defaults.tcl]
    source [file join $sourcedir mainwindow.tcl]
    source [file join $sourcedir db_init.tcl]
    source [file join $sourcedir commonbindings.tcl]

    # additional initial state operations, now that all the widgets have been set
    load_words

    loadbindings

    createmenues

    # set initial debug state
    setdbstate "NoDebug"

    rebind

    # the following update makes background tasks work on Linux
    # since bug 865 is fixed
    update
    focustextarea [gettextareacur]

    # create Scipad version string for the about box - do it here because
    # when lauching Scipad outside of Scilab the showinfo message requires
    # the Scipad window to be ready to display it
    setScipadVersionString

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
