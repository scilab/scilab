# Undo/Redo procs using the text widget embedded mechanism (Tk 8.4 and above)
# The modified flag is automatically handled

proc undo {textarea} {
# Performs an undo
    global listoffile
    undoredo $textarea <<Undo>>
    incr listoffile("$textarea",redostackdepth)
    # update menues contextually
    keyposn $textarea
}

proc redo {textarea} {
# Performs a redo
    global listoffile
    undoredo $textarea <<Redo>>
    incr listoffile("$textarea",redostackdepth) -1
    # update menues contextually
    keyposn $textarea
}

proc undoredo {textarea action} {
# To colorize the range of characters that were changed by an undo
# or redo, a string comparison between before and after the operation
# is done. A better mechanism has been asked for in Tk request 1217222
    global buffermodifiedsincelastsearch
    if {[IsBufferEditable] == "No"} {return}
    set bef [$textarea get 1.0 end]
    event generate $textarea $action 
    set aft [$textarea get 1.0 end]
    set pref [commonPrefix $bef $aft]
    set i1 [$textarea index "1.0 + [string length $pref] chars"]
    set pref [commonPrefix [srevert $bef] [srevert $aft]]
    set i2 [$textarea index "end - [string length $pref] chars"]
    tagcontlines $textarea
    set uplimit [getstartofcolorization $textarea $i1]
    set dnlimit [getendofcolorization $textarea $i2]
    colorize $textarea $uplimit $dnlimit
    reshape_bp
    keyposn $textarea
    set buffermodifiedsincelastsearch true
}

proc ismodified {textarea} {
# Returns true if the buffer in parameter was modified
    return [$textarea edit modified]
}

proc changedmodified {textarea} {
# Update the visual indications showing that the buffer was modified or not
# This includes title bar, colorization of the windows menu entry and
# colorization of an area in the status bar
    global closeinitialbufferallowed
    set closeinitialbufferallowed false
    modifiedtitle $textarea
}

proc resetmodified {textarea} {
# Reset the modified flag and the undo/redo stacks for the given textarea,
# and update the visual indications relative to the modified state
    global listoffile
    $textarea edit reset
    set listoffile("$textarea",redostackdepth) 0
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

proc srevert s {
# Revert a string, e.g. "abc" becomes "cba"
# This is an ancillary for undo/redo
# Source: http://wiki.tcl.tk/44 - Additional string functions
    set l [string length $s]
    set res ""
    while {$l} {append res [string index $s [incr l -1]]}
    set res
}
