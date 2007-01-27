proc deletetext {} {
# cut text procedure (delete key hit, i.e. erases the character
# at the right of the cursor)
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    stopcursorblink ; # see comments in proc puttext
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    set selindices [gettaselind $textareacur any]
    if {$selindices == {}} {
        $textareacur delete "insert" "insert+1c"
        set first [$textareacur index insert]
        set last $first
    } else {
        foreach {sta sto} $selindices {
            $textareacur delete $sta $sto
        }
        set first [lindex $selindices 0  ]
        set last  [lindex $selindices end]
    }
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $first]
    set dnlimit [getendofcolorization $textareacur $last]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    backgroundcolorizeuserfun
    reshape_bp
    $textareacur see insert
    if {$oldSeparator} {
        $textareacur edit separator
        $textareacur configure -autoseparators 1
    }
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
    restorecursorblink ; # see comments in proc puttext
}

proc backspacetext {} {
# cut text procedure (backspace key hit, i.e. erases the character
# at the left of the cursor)
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    stopcursorblink ; # see comments in proc puttext
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    set selindices [gettaselind $textareacur any]
    if {$selindices == {}} {
        $textareacur delete "insert-1c" "insert"
        set first [$textareacur index insert]
        set last $first
    } else {
        foreach {sta sto} $selindices {
            $textareacur delete $sta $sto
        }
        set first [lindex $selindices 0  ]
        set last  [lindex $selindices end]
    }
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $first]
    set dnlimit [getendofcolorization $textareacur $last]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    backgroundcolorizeuserfun
    reshape_bp
    $textareacur see insert
    if {$oldSeparator} {
        $textareacur edit separator
        $textareacur configure -autoseparators 1
    }
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
    restorecursorblink ; # see comments in proc puttext
}

proc cuttext {} {
# cut text procedure: copy current selection into the clipboard,
# and remove the selected text from the current buffer
# note: block selection is supported

    global listoffile buffermodifiedsincelastsearch lineend

    if {[IsBufferEditable] == "No"} {return}

    set textareacur [gettextareacur]
    set selindices [gettaselind $textareacur any]

    if {$selindices == {}} {return}

    stopcursorblink ; # see comments in proc puttext
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }

    # now cut it! note that tk_textCut being designed to work with a
    # single range selection, this command cannot be used here directly
	clipboard clear -displayof $textareacur
    clipboard append -displayof $textareacur \
        [$textareacur get [lindex $selindices 0] [lindex $selindices 1]]
    foreach {sta sto} [lreplace $selindices 0 1] {
        # if there is a block selection, split the selected lines with
        # a lineend in the clipboard
        clipboard append -displayof $textareacur \
            "$lineend[$textareacur get $sta $sto]"
    }
    # text deletion must be done at once and not range by range!
	eval "$textareacur delete $selindices"

    $textareacur tag remove sel 1.0 end
    $textareacur see insert
    set i1 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i1]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    backgroundcolorizeuserfun
    reshape_bp
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
    restorecursorblink ; # see comments in proc puttext
}

proc copytext {} {
# copy text procedure: copy current selection into the clipboard
# note: block selection is supported

    global lineend

    set textareacur [gettextareacur]
    set selindices [gettaselind $textareacur any]

    if {$selindices == {}} {return}

    stopcursorblink ; # see comments in proc puttext

    # now copy it! note that tk_textCopy being designed to work with a
    # single range selection, this command cannot be used here directly
	clipboard clear -displayof $textareacur
    clipboard append -displayof $textareacur \
        [$textareacur get [lindex $selindices 0] [lindex $selindices 1]]
    foreach {sta sto} [lreplace $selindices 0 1] {
        # if there is a block selection, split the selected lines with
        # a lineend in the clipboard
        clipboard append -displayof $textareacur \
            "$lineend[$textareacur get $sta $sto]"
    }

    restorecursorblink ; # see comments in proc puttext
}

proc pastetext {} {
# paste text procedure: copy clipboard content into the current buffer
# where the insert cursor is
# note: block selection is supported (i.e. it is first collapsed
# to its first range and then replaced by the pasted text)

    global listoffile buffermodifiedsincelastsearch

    if {[IsBufferEditable] == "No"} {return}

    set textareacur [gettextareacur]

    stopcursorblink ; # see comments in proc puttext
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }

    # if there is a selection, delete it before pasting
    # if the selection is a block selection, collapse it first
    # down to its first range
    if {[gettaselind $textareacur single] != {}} {
        $textareacur delete sel.first sel.last
    }
    set i1 [$textareacur index insert]
    tk_textPaste $textareacur 
    set  i2 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i2]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    backgroundcolorizeuserfun
    reshape_bp
    $textareacur see insert
    if {$oldSeparator} {
        $textareacur edit separator
        $textareacur configure -autoseparators 1
    }
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
    restorecursorblink ; # see comments in proc puttext
}

proc button2copypaste {w x y} {
##ES 16/11/04 -- we have to write a full proc for this because we need
# to take care of colorization, insert only when editable, etc
    global textareacur listoffile
    # the target textarea gets focused, even if paste is forbidden there
    if {[IsBufferEditable] == "No"} {focustextarea $w; return}
    stopcursorblink ; # see comments in proc puttext
    if {[catch {selection get}] == 0} {
        foreach ta [getfullpeerset $textareacur] {
            set listoffile("$ta",redostackdepth) 0
        }
        clipboard clear
        set ct [selection get]
        clipboard append $ct
        selection clear
        focustextarea $w
        $w mark set insert @$x,$y
        pastetext
    } else {
        focustextarea $w
    }
    #there is still one glitch - the cursor returns at the beginning
    # of the insertion point (why?) - but not on windows !
    #also in windows this works as a sort of "drop selection here", but
    # with glitches (which ones?)
    restorecursorblink ; # see comments in proc puttext
}
