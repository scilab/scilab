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
        # text deletion must be done at once and not range by range!
        eval "$textareacur delete $selindices"
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
        # text deletion must be done at once and not range by range!
        eval "$textareacur delete $selindices"
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

proc cuttext {mode {tocutinblockmode ""}} {
# cut text procedure: copy current selection into the clipboard,
# and remove the selected text from the current buffer
# note: block selection is supported
# if $mode is "normal": usual cut
#     all the text tagged with sel is sent to the clipboard and is deleted
#     from the textarea
# if $mode is "block": block cut
#     all the text tagged with sel is sent to the clipboard (unless
#     $tocutinblockmode is not empty), but deletion of the tagged
#     text occurs only for line selections having more than a single \n
# the second argument tocutinblockmode is optional and can only be given
# when $mode=="block". If it is given, it contains the indices of the text
# to cut in the current textarea, otherwise the text to cut is extracted
# from the selection. If it is given, the text is not sent to the clipboard
# (it is simply deleted from the current textarea)

    global listoffile buffermodifiedsincelastsearch

    if {[IsBufferEditable] == "No"} {return}

    set textareacur [gettextareacur]

    if {$mode == "block" && $tocutinblockmode != ""} {
        set dndmode true
    } else {
        set dndmode false
    }

    if {$dndmode} {
        set selindices $tocutinblockmode
    } else {
        set selindices [gettaselind $textareacur any]
        if {$selindices == {}} {return}
    }

    stopcursorblink ; # see comments in proc puttext
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }

    if {!$dndmode} {
        sendtoclipboard $textareacur $selindices
    }

    # save first sel position, so that the cursor can be placed there
    # after the cut - this position being before everything selected
    # (that will be deleted), it won't change in the following process
    # therefore no adjustment is needed when placing the cursor there
    set firstselpos [lindex $selindices 0]
    set i1 $firstselpos
    set i2 [lindex $selindices end] ; # an overkill, but how else?

    # now cut it! note that tk_textCut being designed to work with a
    # single range selection, this command cannot be used here directly
    # text deletion must be done at once and not range by range!
    if {$mode == "normal"} {
        # normal cut
        eval "$textareacur delete $selindices"
    } else {
        # $mode == "block", this is block cut
        blockcuttext $textareacur $selindices
    }

    $textareacur tag remove sel 1.0 end
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i2]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    backgroundcolorizeuserfun
    reshape_bp
    if {!$dndmode} {
       $textareacur mark set insert $firstselpos
    }
    $textareacur see insert
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
    restorecursorblink ; # see comments in proc puttext
}

proc blockcuttext {w selindices} {
# deletes the characters identified by the selindices list, but do this in
# "block-style", i.e. do not cut empty lines
    set selindicestodelete [list ]
    foreach {sta sto} $selindices {
        # a selected line must be deleted if and only if it does not
        # contain only a \n
        set lines [split [$w get $sta $sto] "\n"]
        set charcount 0
        foreach lineoftext $lines {
            set linelength [string length $lineoftext]
            if {$linelength != 0} {
                set sta1 [$w index "$sta + $charcount c"]
                set sto1 [$w index "$sta1 + $linelength c"]
                lappend selindicestodelete $sta1 $sto1
            }
            incr charcount [expr {$linelength + 1}]
        }
    }
    if {$selindicestodelete != {}} {
        eval "$w delete $selindicestodelete"
    }
}

proc copytext {} {
# copy text procedure: copy current selection into the clipboard
# note: block selection is supported

    set textareacur [gettextareacur]
    set selindices [gettaselind $textareacur any]

    if {$selindices == {}} {return}

    stopcursorblink ; # see comments in proc puttext

    # now copy it! note that tk_textCopy being designed to work with a
    # single range selection, this command cannot be used here directly
    sendtoclipboard $textareacur $selindices

    restorecursorblink ; # see comments in proc puttext
}

proc sendtoclipboard {ta indices} {
# copy in the clipboard the text identified by $indices in textarea $ta
# $indices is a list of $start $stop text widget indices
# warning: this list is supposed to identify contiguous lines, i.e. a block
#          selection
    clipboard clear -displayof $ta
    clipboard append -displayof $ta [gettatextstring $ta $indices]
}

proc pastetext {mode {topasteinblockmode ""}} {
# paste text procedure: copy clipboard content into the current buffer
# where the insert cursor is
# note: block selection is supported (i.e. it is first collapsed
# to its first range and then replaced by the pasted text)
# if $mode is "normal": usual paste
#     the clipboard content to paste is viewed as a single string that can
#     contain \n
# if $mode is "block": block paste
#     the clipboard content to paste is viewed as a a number of strings each
#     separated by \n
# the second argument topasteinblockmode is optional and can only be given
# when $mode=="block". If it is given, it contains the text to paste,
# otherwise the text to paste is extracted from the clipboard

    global listoffile buffermodifiedsincelastsearch

    if {[IsBufferEditable] == "No"} {return}

    set textareacur [gettextareacur]

    if {$mode == "block" && $topasteinblockmode != ""} {
        set topaste $topasteinblockmode
    } else {
        # retrieve the clipboard content, if there is none, simply return
        if {[catch {selection get -displayof $textareacur -selection CLIPBOARD} topaste]} {
            return
        }
    }

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

    # now paste it!
    # note that the replace mode is not tested: paste is always an insert,
    # whether in normal or in block mode
    # this is a choice: if the user wants to paste over existing text he
    # should select this text first
    set i1 [$textareacur index insert]
    if {$mode == "normal"} {
        # normal paste
        $textareacur insert insert $topaste
    } else {
        # $mode == "block", this is block paste
        # while the block selection process is a pixel-based one,
        # this paste is an index-based paste, not a pixel-based one
        # the difference is only apparent with tabs and/or non fixed width fonts
        set listoflines [split $topaste "\n"]
        set n 0
        foreach aline $listoflines {
            $textareacur insert insert $aline
            incr n
            $textareacur mark set insert "$i1 + $n l"
            # make room for next line if end of buffer has been reached
            if {[$textareacur compare insert == "end - 1 c"] && \
                $n < [llength $listoflines]} {
                puttext $textareacur "\n" "forceinsert"
                $textareacur mark set insert "$i1 + $n l"
            }
        }
    }
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
# we have to write a full proc for this because we need
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
        showtext $w ; # see comment on showtext below
        $w mark set insert [TextClosestGap_scipad $w $x $y]
        pastetext normal
    } else {
        # in principle, focustextarea $w would be enough here since $w is
        # already visible (it has just been clicked!)
        # but because of Scilab bug 1544, [selection get] might last 10 s
        # or so during which the Tcl event loop is active, allowing the user
        # to Ctrl-n or anything else that switches buffers and hides the one
        # that was Button-2 clicked for pasting. Then one of the following
        # errors would be triggered:
        #   can't read "listoffile(".scipad.new1",displayedname)": no such element in array
        #   invalid command name "none"
        # Even without bug 1544, using proc showtext instead of focustextarea
        # is an extra precaution that could indeed be needed because it also
        # lauches recolorization of user functions, which might change on paste
        showtext $w
    }
    # there is still one glitch - the cursor returns at the beginning
    # of the insertion point (why?) - but not on windows !
    # also in windows this works as a sort of "drop selection here", but
    # with glitches (which ones?)
    restorecursorblink ; # see comments in proc puttext
}
