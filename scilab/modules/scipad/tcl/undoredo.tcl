# Undo/Redo procs using the text widget embedded mechanism (Tk 8.4 and above)
# The modified flag is automatically handled

proc undo {textarea} {
# Performs an undo
    global listoffile
    undoredo $textarea <<Undo>>
    foreach ta [getfullpeerset $textarea] {
        incr listoffile("$ta",redostackdepth)
    }
    # update menues contextually
    keyposn $textarea
}

proc redo {textarea} {
# Performs a redo
    global listoffile
    undoredo $textarea <<Redo>>
    foreach ta [getfullpeerset $textarea] {
        incr listoffile("$ta",redostackdepth) -1
    }
    # update menues contextually
    keyposn $textarea
}

proc undoredo {textarea action} {
# To colorize the range of characters that were changed by an undo
# or redo, a string comparison between before and after the operation
# is done. A better mechanism has been asked for in Tk request 1217222
    global buffermodifiedsincelastsearch
    global linenumbersmargins
    if {[IsBufferEditable] == "No"} {return}
    set bef [$textarea get 1.0 end]
    event generate $textarea $action 
    set aft [$textarea get 1.0 end]
    set pref [commonPrefix $bef $aft]
    set i1 [$textarea index "1.0 + [string length $pref] chars"]
    set pref [commonPrefix [sreverse $bef] [sreverse $aft]]
    set i2 [$textarea index "end - [string length $pref] chars"]
    tagcontlines $textarea
    if {[$textarea compare $i2 < $i1]} {
        set tmp $i1
        set i1 $i2
        set i2 $tmp
    }
    set uplimit [getstartofcolorization $textarea $i1]
    set dnlimit [getendofcolorization $textarea $i2]
    colorize $textarea $uplimit $dnlimit
    backgroundcolorizeuserfun
    reshape_bp
    keyposn $textarea
    set buffermodifiedsincelastsearch true
    # explicitely calling updatelinenumbersmargin is needed when the
    # undo/redo operation occurs in a textarea that does entirely fit
    # in the alloted space (i.e. the scrollbars are disabled)
    if {$linenumbersmargins != "hide"} {
        foreach ta [getfullpeerset $textarea] {
            updatelinenumbersmargin $ta
        }
    }
}

proc isanymodified {} {
# Return true if any buffer was modified
    global listoftextarea
    set ret false
    # the modified flag is shared among peers, no need to check all of them
    foreach textarea [filteroutpeers $listoftextarea] {
        if {[ismodified $textarea]} {
            set ret true
            break
        }
    }
    return $ret
}

proc ismodified {textarea} {
# Return true if the buffer in parameter was modified
    return [$textarea edit modified]
}

proc changedmodified {textarea} {
# Update the visual indications showing that the buffer was modified or not
# This includes title bar, colorization of the windows menu entry and
# colorization of an area in the status bar
    global closeinitialbufferallowed listoftextarea

    # this test is needed because proc changedmodified is binded to the Text
    # class, i.e. it might trigger also for non textareas such as the call
    # stack widget of the watch window
    if {[lsearch -exact $listoftextarea $textarea] == -1} {
        return
    }

    set closeinitialbufferallowed false
    modifiedtitle $textarea
}

proc resetmodified {textarea} {
# Reset the modified flag and the undo/redo stacks for the given textarea,
# and update the visual indications relative to the modified state
    global listoffile
    $textarea edit reset
    foreach ta [getfullpeerset $textarea] {
        set listoffile("$ta",redostackdepth) 0
    }
    $textarea edit modified false ;# <<Modified>> event is automatically generated
}

proc commonPrefix {a b} {
# Return the common prefix between two strings
# This is an ancillary for undo/redo
# Source: http://wiki.tcl.tk/44 - Additional string functions
    set res {}
    foreach i [split $a ""] j [split $b ""] {
        if {$i==$j} {append res $i} else break
    }
    set res
}

proc sreverse s {
# Reverse a string, e.g. "abc" becomes "cba"
# This is an ancillary for undo/redo
# Source: http://wiki.tcl.tk/44 - Additional string functions
# TIP #272 (String and List Reversal Operations) is however be part of
# Tcl8.5, and is used if available because it's performing at a
# tremendously better performance level- TIP #272 is in "Final" state,
# meaning that the corresponding implementation has been included in
# the core (this was done 9 Nov. 06)
    global Tcl85
    if {$Tcl85} {
        return [string reverse $s]
    } else {
        set l [string length $s]
        set res ""
        while {$l} {append res [string index $s [incr l -1]]}
        set res
    }
}

proc listreverse l {
# Reverse a list, e.g. {a b c} becomes {c b a}
# Source: http://wiki.tcl.tk/43 - Additional list functions
# TIP #272 (String and List Reversal Operations) is however be part of
# Tcl8.5, and is used if available because it's performing at a
# tremendously better performance level- TIP #272 is in "Final" state,
# meaning that the corresponding implementation has been included in
# the core (this was done 9 Nov. 06)
    global Tcl85
    if {$Tcl85} {
        return [lreverse $l]
    } else {
        set res {}
        set i [llength $l]
        while {$i} {lappend res [lindex $l [incr i -1]]}
        set res
    }
}
