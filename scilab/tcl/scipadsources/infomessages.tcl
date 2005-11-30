proc showinfo {message} {
# Temporarily display a string in the message area
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " "
    $pad.statusmes configure -text "$message"
    after 5000 catch delinfo
}

proc delinfo {} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " " 
}

proc keyposn {textarea} {
# launch a keyposn command in background
# dokeyposn is a bit long for very large buffers (execution time is
# around 300 ms for 15000 lines buffers when cursor is at the buffer end),
# entirely due to the inefficient whichfun)
# if many keyposn are waiting for execution, e.g. when the user types
# quicker than the position can be updated by Scipad, there can be many
# keyposn commands pending -> first delete them since they are now pointless
# and launch only the latest command
# dokeyposn is catched to deal more easily with buffers that were closed
# before the command could be processed
    global backgroundtasksallowed
    if {$backgroundtasksallowed} {
        after cancel [list after 1 "catch \"dokeyposn $textarea\""]
        after idle   [list after 1 "catch \"dokeyposn $textarea\""]
    } else {
        dokeyposn $textarea
    }
}

proc dokeyposn {textarea} {
# this proc gets the posn and sets the statusbar, enables context menues, etc.
    global pad listoffile
    global MenuEntryId

    # update the status bars data
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind configure -text " "
    scan $indexin "%d.%d" ypos xpos
    incr xpos
    $pad.statusind configure -text [concat [mc "Line:"] \
        $ypos [mc "Column:"] $xpos]
    set infun [whichfun $indexin $textarea]
    $pad.statusind2 configure -state normal
    $pad.statusind2 configure -text " "
    if {$listoffile("$textarea",language) == "scilab" } {
        if {$infun != {} } {
            # display logical line number in current function
            set funname   [lindex $infun 0]
            set lineinfun [lindex $infun 1]
            $pad.statusind2 configure -text [concat [mc "Line"]\
                 $lineinfun [mc "in"] $funname]
            # create help skeleton enabled since we're in a Scilab function
            $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help s&keleton..."]) -state normal
        } else {
            # display logical line number in current buffer
            set contlines [countcontlines $textarea 1.0 $indexin]
            set logicline [$textarea index "$indexin - $contlines l"]
            scan $logicline "%d.%d" ylogicpos xlogicpos
            $pad.statusind2 configure -text [concat [mc "Logical line:"] $ylogicpos]
            # create help skeleton disabled since we're outside any Scilab function
            $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help s&keleton..."]) -state disabled
        }
    }

    # enable Open function source contextually
    if {[lsearch [$textarea tag names $indexin] "libfun"]!=-1 || \
        [lsearch [$textarea tag names $indexin] "scicos"]!=-1 } {
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Open &function source"]) -state normal
    } else {
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Open &function source"]) -state disabled
    }

    # enable Undo if the buffer was modified
    if {[ismodified $textarea]} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Undo"]) -state normal
        bind Text <Control-z> {undo %W}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Undo"]) -state disabled
        bind Text <Control-z> {}
    }

    # enable Redo if the redo stack is not empty
    if {$listoffile("$textarea",redostackdepth) == 0} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Redo"]) -state disabled
        bind Text <Control-Z> {}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Redo"]) -state normal
        bind Text <Control-Z> {redo %W}
    }

    # enable Cut and Copy and Evaluate selection if there is a selection
    if {[catch {selection get -selection PRIMARY}] == 1} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "Cu&t"]) -state disabled
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Copy"]) -state disabled
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Evaluate selection"]) -state disabled
        bind Text <Control-x> {}
        bind $pad <Control-c> {}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "Cu&t"]) -state normal
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Copy"]) -state normal
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Evaluate selection"]) -state normal
        bind Text <Control-x> {cuttext}
        bind $pad <Control-c> {copytext}
    }

    # enable Paste if the clipboard contains something
    if {[catch {clipboard get}] == 1} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Paste"]) -state disabled
        bind Text <Control-v> {}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Paste"]) -state normal
        bind Text <Control-v> {pastetext}
    }
}

proc modifiedtitle {textarea {panesonly "false"}} {
# Set the Scipad window title to the name of the file displayed in $textarea
# and add tags (modified, readonly)
# Do the same for the pane title if it exists (i.e. if not maximized)
# Update also the visual indications of the modified state of the buffer.
# This includes title bar, colorization of the windows menu entry and
# colorization of an area in the status bar
    global pad winTitle listoffile
    global MenuEntryId
    set fname $listoffile("$textarea",displayedname)
    set ind [extractindexfromlabel $pad.filemenu.wind $fname]
    set mod1 ""; set mod2 ""
    if {$listoffile("$textarea",readonly) == 1} { 
        set mod1 [mc " \[ReadOnly\]"]
    }
    if {[ismodified $textarea]} {
        set mod2 [mc " (modified)"]
        if {$ind !=-1} {
            $pad.filemenu.wind entryconfigure $ind -background Salmon \
              -activebackground LightSalmon
        }
        $pad.statusind configure -background Salmon
    } else {  
        if {$ind !=-1} {
            $pad.filemenu.wind entryconfigure $ind -background "" \
               -activebackground ""
        }
        $pad.statusind configure -background [$pad.filemenu cget -background]
    }
    if {$panesonly == "false"} {
        wm title $pad "$winTitle - $fname$mod1$mod2"
    }
    if {[isdisplayed $textarea]} {
        [getpaneframename $textarea].panetitle configure \
          -text "$fname$mod1$mod2"
    }
    if {[ismodified $textarea] && \
          $listoffile("$textarea",thetime) !=0} { 
        $pad.filemenu.files entryconfigure \
          $MenuEntryId($pad.filemenu.files.[mcra "&Revert..."]) -state normal
        bind $pad <Control-R> {revertsaved [gettextareacur]}
    } else {
        $pad.filemenu.files entryconfigure \
          $MenuEntryId($pad.filemenu.files.[mcra "&Revert..."]) -state disabled
        bind $pad <Control-R> {}
    }
}

proc updatepanestitles {} {
    global listoftextarea
    # update file names in panes only (not in Scipad title bar)
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            modifiedtitle $ta "panesonly"
        }
    }
    # update file name in Scipad title bar (and in the current textarea pane)
    modifiedtitle [gettextareacur]
}
