#cut text procedure
proc deletetext {} {
    global textareacur
    set cuttexts [selection own]
# FV 07/06/04, next line corrected (see bug #723)
#    if {$cuttexts != "" } {}
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
    inccount $textareacur
    # Added by Matthieu PHILIPPE
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i1 wordend"]
# FV 13/05/04
    reshape_bp
}

#cut text procedure
proc backspacetext {} {
    global textareacur
    set cuttexts [selection own]
# FV 07/06/04, next line corrected (see bug #723)
#    if {$cuttexts != "" } {}
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
    inccount $textareacur
    # Added by Matthieu PHILIPPE
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i1 wordend"]
# FV 13/05/04
    reshape_bp
}

#cut text procedure
proc cuttext {} {
# FV 07/06/04, removed superfluous global
    global textareacur
# FV 14/06/04, added test to avoid setting the modified flag if cut nothing
    if {[catch {selection get -selection CLIPBOARD}] == 0} {
        inccount $textareacur
    }
    tk_textCut $textareacur
    # Added by Matthieu PHILIPPE
    set  i1 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 linestart"] \
                              [$textareacur index "$i1 lineend"]
    selection clear
# FV 13/05/04
    reshape_bp
}

#copy text procedure
#ES 16/11/04
proc copytext {} {
      set selowner [selection own]
      tk_textCopy  $selowner
}

#paste text procedure
proc pastetext {} {
# FV 07/06/04, removed superfluous funny test and globals
    global textareacur
    catch {
        $textareacur delete sel.first sel.last
    }
    set i1  [$textareacur index insert]
    tk_textPaste $textareacur 
# FV 14/06/04, added test to avoid setting the modified flag if paste nothing
    if {[catch {selection get -selection CLIPBOARD}] == 0} {
        inccount $textareacur
    }
    # Added by Matthieu PHILIPPE
    set  i2 [$textareacur index insert]
    colorize $textareacur [$textareacur index "$i1 wordstart"] \
                              [$textareacur index "$i2 wordend"]
#added by ES (but might also be unhandy)
#    $textareacur tag add sel $i1 $i2
# FV 13/05/04
    reshape_bp
}

proc button2copypaste {w x y} {
##ES 16/11/04 -- strange I have to write a full proc for this!
## am I missing something?
  global textareacur
  clipboard clear;
  set ct ""
  catch {set ct [selection get]}; 
  clipboard append $ct; 
  selection clear; 
  $w mark set insert @$x,$y;
  pastetext; 
#there is still one glitch - the cursor returns at the beginning
# of the insertion point (why?)
#also in windows this works as a sort of "drop selection here", but
# with glitches
}
