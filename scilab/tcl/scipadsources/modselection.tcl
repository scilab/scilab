proc selectall {} {
    [gettextareacur] tag add sel 1.0 end
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
    if {$seltexts != "" } {
        if [catch {selection get -selection PRIMARY} sel] {	    
            if {[selectline] != ""} CommentSel
        } else {
            set uctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            # I thought this shouldn't match a newline as last character of the string...
            regsub -all -line "(?!.\Z)^" $uctext  "//" ctext
            # glitches: are they because of my misunderstanding or bugs in regsub? 
            # this fixes the trailing "\n//"
            if {[string range $ctext end-1 end]=="//"} {
                set ctext [string range $ctext 0 end-2]
                set i2 $i2-2c
            }
            # this fixes another glitch - missing "//" at the second line if
            # the first is empty
            if {[string range $ctext 0 2]=="//\n"} {
                set ctext1 [string range $ctext 3 end]
                set ctext "//\n//"
                append ctext $ctext1
            }
            puttext $textareacur $ctext
            $textareacur tag add sel $i1 $i2+2c
        }
    } else {
        if {[selectline] != ""} CommentSel
    }
}

proc UnCommentSel {} {
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    set seltexts [selection own]
    if {$seltexts != "" } {
        if [catch {selection get -selection PRIMARY} sel] {	    
            if {[selectline] != ""} {UnCommentSel}
        } else {
            set ctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            if { $i2<$i1 } {
                set i3 $i2; set i2 $i1; set i1 $i3
            } 
            regsub -all -line "^\\s*//" $ctext  "" uctext
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
    if {$seltexts != "" } {
        if [catch {selection get -selection PRIMARY} sel] {
            if {[selectline] != ""} {IndentSel}
        } else {
            set uctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            # I thought this shouldn't match a newline as last character of the string...
            regsub -all -line "(?!.\Z)^" $uctext $skip ctext
            # glitches: are they because of my misunderstanding or bugs in regsub? 
            # this fixes the trailing "\n  "
            if {[string range $ctext end-1 end]==$skip} {
                set ctext [string range $ctext 0 end-$indentspaces]
                set i2 "$i2-$indentspaces c"
            }
            # this fixes another glitch - missing "  " at the second line if
            # the first is empty
            if {[string range $ctext 0 $indentspaces]=="$skip\n"} {
                set ctext1 [string range $ctext [expr $indentspaces+1] end]
                set ctext "$skip\n  "
                append ctext $ctext1
            }
            #clean up lines which contain only spaces
            regsub -all -line "\ *\n" $ctext "\n" ctext1
            puttext $textareacur $ctext1
            $textareacur tag add sel $i1 "$i2+$indentspaces c"
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
    if {$seltexts != ""} {
        if [catch {selection get -selection PRIMARY} sel] {	    
            if {[selectline] != ""} {UnIndentSel}
        } else {
            set ctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            if { $i2<$i1 } {
                set i3 $i2; set i2 $i1; set i1 $i3
            }
            regsub -all -line "^ {1,$indentspaces}" $ctext  "" uctext
            puttext $textareacur $uctext
            # as above in UnCommentSel
            $textareacur tag add sel $i1 [$textareacur index insert]
        }
    } else {
        if {[selectline] != ""} UnIndentSel
    }
}
