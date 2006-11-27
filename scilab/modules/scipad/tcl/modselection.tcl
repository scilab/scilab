proc selectall {} {
    [gettextareacur] tag add sel 1.0 end-1c
    [gettextareacur] mark set insert end
    # update menues contextually
    keyposn [gettextareacur]
}

proc selectline {} {
# ancillary for un/comment, un/indent
# select a physical line of text
    set ta [gettextareacur]
    set i1 [$ta index "insert linestart"]
    set i2 [$ta index "insert lineend"]
    $ta tag remove sel 1.0 end
    $ta tag add sel $i1 $i2
    set seltext [$ta get $i1 $i2]
    return $seltext
}

proc CommentSel {} {
# comment the selection, or insert "//" at the current point
# if there is no selection

    if {[IsBufferEditable] == "No"} {return}

    set ta [gettextareacur]
    set tasel [$ta tag nextrange sel 1.0]

    if {$tasel == ""} {
        # if there is no selection, simply insert // at the insert point
        puttext $ta "//"

    } else {
        # there is a selection in the current textarea
        # note: do NOT extend selection to the beginning of the line!
        
        # insert // at the beginning of the selected string and at each
        # newline, but not after the last newline, if the selection ends
        # at column 0 - note that this matches empty lines (correct)
        apply_RE_to_sel_text $ta {^(?!\Z)} "//"
    }
}

proc UnCommentSel {} {
# uncomment the selection, or the current line if there is no selection
# if the selection or current line are not commented, do nothing

    if {[IsBufferEditable] == "No"} {return}

    set ta [gettextareacur]
    set tasel [$ta tag nextrange sel 1.0]

    if {$tasel == ""} {
        # if there is no selection, select the current line and uncomment
        # if this selected line is not empty
        if {[selectline] != ""} {
            UnCommentSel
        }

    } else {
        # there is a selection in the current textarea
        # note: do NOT extend selection to the beginning of the line!

        # remove leading //, possibly prepended with spaces
        apply_RE_to_sel_text $ta {^\s*//} ""
    }
}

proc IndentSel {} {
# indent the selection, or the current line if there is no selection
    global tabinserts indentspaces

    if {[IsBufferEditable] == "No"} {return}

    if {$tabinserts == "spaces"} {
        set skip ""
        for {set x 0} {$x<$indentspaces} {incr x} {
            append skip " "
        }
    } else {
        # tab inserts tabs, then $skip is one tab character
        set skip "\x9"
    }

    set ta [gettextareacur]
    set tasel [$ta tag nextrange sel 1.0]

    if {$tasel == ""} {
        # if there is no selection, select the current line and indent
        # if this selected line is not empty
        if {[selectline] != ""} {
            IndentSel
        }

    } else {
        # there is a selection in the current textarea
        # extend selection to the beginning of the line
        $ta tag add sel "sel.first linestart" sel.last

        # so only one undo is required to undo indentation
        set oldSeparator [$ta cget -autoseparators]
        if {$oldSeparator} {
            $ta configure -autoseparators 0
            $ta edit separator
        }

        # remove leading spaces or tabs
        # this is useful when reindenting with tabs (or spaces) a text
        # already indented with spaces (respectively tabs)
        if {$tabinserts == "spaces"} {
            # remove leading tabs
            apply_RE_to_sel_text $ta {^\t*} ""
        } else {
            # remove leading spaces
            apply_RE_to_sel_text $ta {^ *} ""
        }

        # maybe the removal collapsed the selection, then select again
        set tasel [$ta tag nextrange sel 1.0]
        if {$tasel == ""} {
            if {[selectline] != ""} {
                IndentSel
            }
        } else {
            # insert $skip at the beginning of the selected string and at each
            # newline, but not after the last newline, if the selection ends at
            # column 0 - moreover, do not modify (match) empty lines, so that
            # there is no need to clean up lines which would otherwise contain
            # only spaces
            apply_RE_to_sel_text $ta {^(.)(?!\Z)} "$skip\\1"
        }

        if {$oldSeparator} {
            $ta edit separator
            $ta configure -autoseparators 1
        }
    }
}

proc UnIndentSel {} {
# unindent the selection, or the current line if there is no selection
# if the selection or current line are not indented, do nothing
    global indentspaces

    if {[IsBufferEditable] == "No"} {return}

    set ta [gettextareacur]
    set tasel [$ta tag nextrange sel 1.0]

    if {$tasel == ""} {
        # if there is no selection, select the current line and unindent
        # if this selected line is not empty
        if {[selectline] != ""} {
            UnIndentSel
        }

    } else {
        # there is a selection in the current textarea
        # extend selection to the beginning of the line
        $ta tag add sel "sel.first linestart" sel.last

        # remove up to $indentspaces leading spaces or one tab character
        # at the beginning of each line
        apply_RE_to_sel_text $ta "(^ {1,$indentspaces})|(^\\t)" ""
    }
}

proc apply_RE_to_sel_text {ta REpat repltext} {
# ancillary for un/comment and un/indent
# the textarea $ta is supposed to have a selection
# in that selection, a regsub is performed using $REpat and the
# replacement text $repltext
# the resulting text replaces the original one in $ta
# the selection is updated according to the changes that were done
    # these marks will maintain the selection boundaries correctly
    # even during puttext below
    $ta mark set leftbound [$ta index sel.first]
    $ta mark gravity leftbound left
    $ta mark set rightbound [$ta index sel.last]
    $ta mark gravity rightbound right
    set seltext [$ta get leftbound rightbound]
    # remove spaces at the beginning of each line of the selected string
    regsub -all -line -- $REpat $seltext $repltext newtext
    puttext $ta $newtext
    # restore the selection, and cleanup marks
    $ta tag add sel leftbound rightbound
    $ta mark unset leftbound rightbound
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
