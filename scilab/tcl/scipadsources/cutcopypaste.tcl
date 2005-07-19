proc deletetext {} {
# cut text procedure
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
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
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                          [$textareacur index "$i1 wordend"]
    reshape_bp
    $textareacur see insert
}

proc backspacetext {} {
# cut text procedure
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
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
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i1 wordend"]
    reshape_bp
    $textareacur see insert
}

proc cuttext {} {
# cut text procedure
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}

    tk_textCut $textareacur
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 linestart"] \
                          [$textareacur index "$i1 lineend"]
    selection clear
    reshape_bp
    $textareacur see insert
}

proc copytext {} {
# copy text procedure
    set selowner [selection own]
    tk_textCopy  $selowner
}

proc pastetext {} {
# paste text procedure
    set textareacur [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    set oldSeparator [$textareacur cget -autoseparators]
    if {$oldSeparator} {
        $textareacur configure -autoseparators 0
        $textareacur edit separator
    }
    catch {
        $textareacur delete sel.first sel.last
    }
    set i1  [$textareacur index insert]
    tk_textPaste $textareacur 
    set  i2 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                          [$textareacur index "$i2 wordend"]
    reshape_bp
    $textareacur see insert
    if {$oldSeparator} {
        $textareacur edit separator
        $textareacur configure -autoseparators 1
    }
}

proc button2copypaste {w x y} {
##ES 16/11/04 -- strange I have to write a full proc for this!
## am I missing something?
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    if {[catch {selection get}] == 0} {
        clipboard clear
        set ct [selection get]
        clipboard append $ct
        selection clear
        $w mark set insert @$x,$y
        pastetext
    }
#there is still one glitch - the cursor returns at the beginning
# of the insertion point (why?) - but not on windows !
#also in windows this works as a sort of "drop selection here", but
# with glitches (which ones?)
}
