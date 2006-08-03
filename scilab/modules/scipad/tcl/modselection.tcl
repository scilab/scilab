proc selectall {} {
    [gettextareacur] tag add sel 1.0 end-1c
    [gettextareacur] mark set insert end
    # update menues contextually
    keyposn [gettextareacur]
}

proc selectline {} {
# ancillary for un/comment, un/indent
    global textareacur
    set i1 [$textareacur index "insert linestart"]
    set i2 [$textareacur index "insert lineend"]
    selection clear
    $textareacur tag add sel $i1 $i2
    set seltext ""
    catch {set seltext [selection get]}
    return $seltext
}

proc CommentSel {} {
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    set seltexts [selection own]
    if {[string range $seltexts 0 [expr [string length $textareacur]-1]]\
             == $textareacur} {
        if [catch {selection get -selection PRIMARY} sel] {
            if {[selectline] != ""} CommentSel
        } else {
            set uctext [selection get]
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            scan $i2 %d.%d l2 c2
# insert // at the beginning of the selected string and at each newline,
#  but not after the last newline, if the selection end at column 0
# (no idea how but it works!)
            regsub -all -line {^(?!\Z)} $uctext  "//" ctext
            puttext $textareacur $ctext
# restore the selection including the // characters added
            if {$c2==0} {
              $textareacur tag add sel $i1 $i2
            } else {
              $textareacur tag add sel $i1 $i2+2c
            }
        }
    } else {
        if {[selectline] != ""} CommentSel
    }
}

proc UnCommentSel {} {
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    set seltexts [selection own]
    if {[string range $seltexts 0 [expr [string length $textareacur]-1]]\
             == $textareacur} {
        if [catch {selection get -selection PRIMARY} sel] {
            if {[selectline] != ""} {UnCommentSel}
        } else {
            set ctext [selection get]
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            if { $i2<$i1 } {
                set i3 $i2; set i2 $i1; set i1 $i3
            } 
            regsub -all -line "^\\s*//" $ctext "" uctext
            puttext $textareacur $uctext
            # after many deglitches, this only sometimes loses the selection
            # for <one line
            $textareacur tag add sel $i1 [$textareacur index insert] 
        }
    } else {
        if {[selectline] != ""} UnCommentSel
    }
}

proc IndentSel {} {
# just copied from CommentSel
    global textareacur indentspaces
    if {[IsBufferEditable] == "No"} {return}
    set skip ""
    for {set x 0} {$x<$indentspaces} {incr x} {
        append skip " "
    }
    set seltexts [selection own]
    if {[string range $seltexts 0 [expr [string length $textareacur]-1]]\
             == $textareacur} {
        if [catch {selection get -selection PRIMARY} sel] {
            if {[selectline] != ""} {IndentSel}
        } else {
            set uctext [selection get]
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            scan $i2 %d.%d l2 c2
# insert $skip at the beginning of the selected string and at each newline,
#  but not after the last newline, if the selection end at column 0
# (no idea how but it works!)
            regsub -all -line {^(?!\Z)} $uctext $skip ctext
# clean up lines which contain only spaces
            regsub -all -line "\ *\n" $ctext "\n" ctext1
            puttext $textareacur $ctext1
# restore the selection including the spaces added
            if {$c2==0} {
              $textareacur tag add sel $i1 $i2
            } else {
              $textareacur tag add sel $i1 "$i2+$indentspaces c"
            }
        }
    } else {
        if {[selectline] != ""} IndentSel
    }
}

proc UnIndentSel {} {
# just copied from UncommentSel
    global textareacur indentspaces
    if {[IsBufferEditable] == "No"} {return}
    set seltexts [selection own]
    if {[string range $seltexts 0 [expr [string length $textareacur]-1]]\
             == $textareacur} {
        if [catch {selection get -selection PRIMARY} sel] {
            if {[selectline] != ""} {UnIndentSel}
        } else {
            set ctext [selection get]
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            if { $i2<$i1 } {
                set i3 $i2; set i2 $i1; set i1 $i3
            }
            regsub -all -line "^ {1,$indentspaces}" $ctext "" uctext
            puttext $textareacur $uctext
            # as above in UnCommentSel
            $textareacur tag add sel $i1 [$textareacur index insert]
        }
    } else {
        if {[selectline] != ""} UnIndentSel
    }
}

proc updatedoubleclickscheme {} {
    global doubleclickscheme tcl_wordchars tcl_nonwordchars
    global tcl_wordchars_linux tcl_nonwordchars_linux
    global tcl_wordchars_windows tcl_nonwordchars_windows 
    global tcl_wordchars_scilab tcl_nonwordchars_scilab
    if {$doubleclickscheme == "Linux"} {
        set tcl_wordchars $tcl_wordchars_linux
        set tcl_nonwordchars $tcl_nonwordchars_linux
    } elseif {$doubleclickscheme == "Windows"} {
        set tcl_wordchars $tcl_wordchars_windows
        set tcl_nonwordchars $tcl_nonwordchars_windows
    } else {
        # "Scilab"
        set tcl_wordchars $tcl_wordchars_scilab
        set tcl_nonwordchars $tcl_nonwordchars_scilab
    }
}
