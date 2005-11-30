proc deletetext {} {
# cut text procedure
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    set listoffile("$textareacur",redostackdepth) 0
    set cuttexts [selection own]
    if {[string range $cuttexts 0 [expr [string length $textareacur]-1]] \
            == $textareacur} {
        if [catch {selection get -selection PRIMARY} sel] {
            $textareacur delete "insert" "insert +1c"
        } else {
            $cuttexts delete sel.first sel.last
            selection clear
        }
    } else {
        $textareacur delete "insert" "insert +1c"
    }
    set i1 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i1]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    reshape_bp
    $textareacur see insert
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
}

proc backspacetext {} {
# cut text procedure
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    set listoffile("$textareacur",redostackdepth) 0
    set cuttexts [selection own]
    if {[string range $cuttexts 0 [expr [string length $textareacur]-1]]\
             == $textareacur} {
        if [catch {selection get -selection PRIMARY} sel] {
            $textareacur delete "insert-1c" "insert"
        } else {
            $cuttexts delete sel.first sel.last
            selection clear
        }
    } else {
        $textareacur delete "insert-1c" "insert"
    }
    set i1 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i1]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    reshape_bp
    $textareacur see insert
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
}

proc cuttext {} {
# cut text procedure
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    set listoffile("$textareacur",redostackdepth) 0
    tk_textCut $textareacur
    set i1 [$textareacur index insert]
    tagcontlines $textareacur
    set uplimit [getstartofcolorization $textareacur $i1]
    set dnlimit [getendofcolorization $textareacur $i1]
    colorize $textareacur [$textareacur index $uplimit] \
                          [$textareacur index $dnlimit]
    selection clear
    reshape_bp
    $textareacur see insert
    # update menues contextually
    keyposn $textareacur
    set buffermodifiedsincelastsearch true
}

proc copytext {} {
# copy text procedure
    set selowner [selection own]
    tk_textCopy  $selowner
}

proc pastetext {} {
# paste text procedure
    global listoffile buffermodifiedsincelastsearch
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    set listoffile("$textareacur",redostackdepth) 0
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    catch {
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
    global textareacur listoffile buffermodifiedsincelastsearch
    if {[IsBufferEditable] == "No"} {return}
    if {[catch {selection get}] == 0} {
        set listoffile("$textareacur",redostackdepth) 0
        clipboard clear
        set ct [selection get]
        clipboard append $ct
        selection clear
        $w mark set insert @$x,$y
        pastetext
        set buffermodifiedsincelastsearch true
    }
#there is still one glitch - the cursor returns at the beginning
# of the insertion point (why?) - but not on windows !
#also in windows this works as a sort of "drop selection here", but
# with glitches (which ones?)
}
