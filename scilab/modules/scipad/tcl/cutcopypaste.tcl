proc deletetext {} {
# cut text procedure (delete key hit, i.e. erases the character
# at the right of the cursor)
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    if {[gettaselind $textareacur] == {}} {
        $textareacur delete "insert" "insert+1c"
    } else {
        $textareacur delete sel.first sel.last
    }
    set i1 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i1]
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
}

proc backspacetext {} {
# cut text procedure (backspace key hit, i.e. erases the character
# at the left of the cursor)
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    if {[gettaselind $textareacur] == {}} {
        $textareacur delete "insert-1c" "insert"
    } else {
        $textareacur delete sel.first sel.last
    }
    set i1 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i1]
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
}

proc cuttext {} {
# cut text procedure
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    tk_textCut $textareacur
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
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
}

proc copytext {} {
# copy text procedure
    tk_textCopy [gettextareacur]
}

proc pastetext {} {
# paste text procedure
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    foreach ta [getfullpeerset $textareacur] {
        set listoffile("$ta",redostackdepth) 0
    }
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    if {[gettaselind $textareacur] != {}} {
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
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
}

proc button2copypaste {w x y} {
##ES 16/11/04 -- we have to write a full proc for this because we need
# to take care of colorization, insert only when editable, etc
    global textareacur listoffile
#the target textarea gets focused, even if paste is forbidden there
    if {[IsBufferEditable] == "No"} {focustextarea $w; return}
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
}
