proc deletetext {} {
# cut text procedure
    global textareacur
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
    setmodified $textareacur
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i1 wordend"]
    reshape_bp
}

proc backspacetext {} {
# cut text procedure
    global textareacur
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
    setmodified $textareacur
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i1 wordend"]
    reshape_bp
}

proc cuttext {} {
# cut text procedure
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    # test to avoid setting the modified flag if cut nothing
    if {[catch {selection get -selection CLIPBOARD}] == 0} {
        setmodified $textareacur
    }
    tk_textCut $textareacur
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 linestart"] \
                              [$textareacur index "$i1 lineend"]
    selection clear
    reshape_bp
}

proc copytext {} {
# copy text procedure
      set selowner [selection own]
      tk_textCopy  $selowner
}

proc pastetext {} {
# paste text procedure
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    catch {
        $textareacur delete sel.first sel.last
    }
    set i1  [$textareacur index insert]
    tk_textPaste $textareacur 
    # test to avoid setting the modified flag if paste nothing
    if {[catch {selection get -selection CLIPBOARD}] == 0} {
        setmodified $textareacur
    }
    set  i2 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i2 wordend"]
    reshape_bp
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
