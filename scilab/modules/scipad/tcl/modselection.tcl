proc TextClosestGap_scipad {w x y} {
# this is a bulletproof wrapper to the Tk private function
# ::tk::TextClosestGap which is useful for selections
# since ::tk::TextClosestGap is not part of the public interface
# of the text widget, a fallback using only public interface
# functions must exist to guard against private functions changes
# in Tk
    if {[catch {::tk::TextClosestGap $w $x $y} ind] != 0} {
        set ind [$w index @$x,$y]
    }
    return $ind
}

proc selectall {} {
    # end position to select is end-1c so that the final \n in the textarea
    # is not selected (it is anyway not part of the actual text entered by
    # the user, but is needed in any text widget
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

proc gettatextstring {ta indices} {
# construct a single string from the text identified by $indices in textarea $ta
# $indices is a list of $start $stop text widget indices
# warning: this list is supposed to identify contiguous lines, i.e. a block
#          selection
    set prevsto [lindex $indices 1]
    set fullstring [$ta get [lindex $indices 0] $prevsto]
    foreach {sta sto} [lreplace $indices 0 1] {
        # if there are multiple ranges, split the selected lines with
        # a \n in the clipboard, but only if the range does not
        # already have a trailing \n in the previous line
        if {[$ta get "$prevsto-1c" $prevsto] != "\n"} {
            append fullstring "\n"
        }
        append fullstring [$ta get $sta $sto]
        set prevsto $sto
    }
    return $fullstring
}

proc gettaselind {textarea {supportmultiple "single"}} {
# return the selection as a list of start / stop indices if there is one
# in $textarea, or an empty string if there is none
# if there are multiple selections (this can only be the case for a block
# selection), return the list of selected text indices if $supportmultiple
# is not set to "single"
# the optional input parameter supportmultiple might contain
#    single: no multiple (i.e. block) selection allowed
#    any   : any selection allowed
# with "any", the calling proc must handle multiple (non contiguous)
# selections
# with "single", multiple selections are collapsed to a single selection
# which is the first range of the multiple selection
# the X selection is not used nor changed, it's the sel tag of $textarea
# that is checked
    set selranges [$textarea tag ranges sel]
    if {$supportmultiple == "single" && [llength $selranges] > 2} {
        # collapse possibly existing multiple selection to a single selection
        # being the first sel range
        foreach {sta sto} [lreplace $selranges 0 1] {
            $textarea tag remove sel $sta $sto
        }
        set selranges [$textarea tag nextrange sel 1.0]
        if {$selranges != {}} {
            $textarea mark set insert sel.last
        }
    }
    return $selranges
}

proc isposinsel {w x y} {
# return true if the pixel position @$x,$y in textarea $w is strictly
# inside the (possibly block) selection of textarea $w
    set testpos [$w index @$x,$y]
    set insel false
    foreach {sta sto} [gettaselind $w any] {
        if {[$w compare $sta < $testpos] && [$w compare $testpos < $sto]} {
            set insel true
            break
        }
    }
    return $insel
}

proc startblockselection {w x y} {
# initiate a block selection
    global wordWrap pad MenuEntryId blockseltoggledwordwrap blockselanchor

    # temporarily force no word wrapping
    if {$wordWrap != "none"} {
        $pad.filemenu.options invoke $MenuEntryId($pad.filemenu.options.[mcra "Word &wrap"])
        set blockseltoggledwordwrap true
    } else {
        set blockseltoggledwordwrap false
    }

    set blockselanchor [list $x $y]
    $w tag remove sel 1.0 end
}

proc endblockselection {w} {
# finish a block selection
    global pad MenuEntryId blockseltoggledwordwrap blockselanchor

    unset -nocomplain -- blockselanchor

    # restore word wrapping
    if {$blockseltoggledwordwrap} {
        $pad.filemenu.options invoke $MenuEntryId($pad.filemenu.options.[mcra "Word &wrap"])
        set blockseltoggledwordwrap false
    }
}

proc selectblock {w x y} {
# extend the block selection from the block selection anchor point,
# i.e. the point where the mouse was when button-1 was pressed,
# to the current mouse position

    global blockselanchor

    if {![info exists blockselanchor]} {
        # Shift-Control was hit while a normal selection had started
        startblockselection $w $x $y
    }

    set anchorx [lindex $blockselanchor 0]
    set anchory [lindex $blockselanchor 1]
    set cornerx $x
    set cornery $y

    # avoid border effects when the mouse goes outside of the textarea
    if {$cornery < 1} {
        set cornery 1
    }
    if {$cornery > [winfo height $w]} {
        set cornery [winfo height $w]
    }

    # possible cases at this point (A=anchor, C=corner):
    #    A      A     C      C    A    C    AC    CA
    #     C    C       A    A     C    A
    set inverted false
    if {$cornery < $anchory} {
        set temp $cornery
        set cornery $anchory
        set anchory $temp
        set inverted true
    }
    if {$cornerx < $anchorx} {
        set temp $cornerx
        set cornerx $anchorx
        set anchorx $temp
    }
    # possible cases at this point:
    #            anchorx  cornerx    anchorx=cornerx    anchorx  cornerx
    #
    # anchory->     A                       A              A        C
    #
    # cornery->              C              C
    #
    # at this point we always have $anchory <= $cornery
    # and $anchorx <= $cornerx, so that @$anchory,$anchorx
    # and @$cornery,$cornerx are the indices of respectively
    # the upper left corner and the lower right corner of
    # the block selection

    # finally, tag the lines!
    $w tag remove sel 1.0 end
    # note: since $anchory and $corney are pixel coordinates, one could
    #       have thought of using {incr i $lineheigth} instead of {incr i}
    #       but this would cause incredible headaches to have the cursor
    #       never after the end of the selection, and never inside it
    #       so I forgot any optimization here and run the loop pixel
    #       by pixel, eventually tagging the same portion of text multiple
    #       times
    for {set i $anchory} {$i <= $cornery} {incr i} {
        set sta [TextClosestGap_scipad $w $anchorx $i]
        set sto [TextClosestGap_scipad $w $cornerx $i]
        if {[$w compare $sta == $sto]} {
            # @$x,$y must be used otherwise empty lines might be forgotten when
            # block-selecting from farther than the middle of the line
            set sta [$w index @$anchorx,$i]
            set sto $sta
            if {[$w compare $sta != "end - 1 c"]} {
                set dlinfo [$w dlineinfo $sta]
                set linex [lindex $dlinfo 0]
                set linewidth [lindex $dlinfo 2]
                if {[$w compare [TextClosestGap_scipad $w [expr {$linex+$linewidth}] $i] <= $sta]} {
                    # the line is shorter than the block left limit
                    # in this case, tag the \n at the end of this line
                    # note that the test above works even if the textarea
                    # has been scrolled horizontally: $linex is negative
                    # in this case, and this is what is needed
                    set sto "$sto + 1l linestart"
                }
            }
        }
        $w tag add sel $sta $sto
    }

    # update the insertion cursor position
    if {$inverted} {
        catch {$w mark set insert sel.first}
    } else {
        catch {$w mark set insert sel.last}
    }
}

proc CommentSel {} {
# comment the selection, or insert "//" at the current point
# if there is no selection
# note: block selection is supported

    if {[IsBufferEditable] == "No"} {return}

    set ta [gettextareacur]

    if {[gettaselind $ta any] == {}} {
        # if there is no selection, simply insert // at the insert point
        puttext $ta "//" "forceinsert"

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
# note: block selection is supported

    if {[IsBufferEditable] == "No"} {return}

    set ta [gettextareacur]

    if {[gettaselind $ta any] == {}} {
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
# note: block selection is supported

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
    set selindices [gettaselind $ta any]

    if {$selindices == {}} {
        # if there is no selection, select the current line and indent
        # if this selected line is not empty
        if {[selectline] != ""} {
            IndentSel
        }

    } else {
        # there is a selection in the current textarea
        # extend selection to the beginning of the line,
        # but only if this is not a block selection
        if {[llength $selindices] == 2} {
            set firstind [lindex $selindices 0]
            $ta tag add sel "$firstind linestart" $firstind
        }

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
        if {[gettaselind $ta any] == {}} {
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
# note: block selection is supported

    global indentspaces

    if {[IsBufferEditable] == "No"} {return}

    set ta [gettextareacur]
    set selindices [gettaselind $ta any]

    if {$selindices == {}} {
        # if there is no selection, select the current line and unindent
        # if this selected line is not empty
        if {[selectline] != ""} {
            UnIndentSel
        }

    } else {
        # there is a selection in the current textarea
        # extend selection to the beginning of the line,
        # but only if this is not a block selection
        if {[llength $selindices] == 2} {
            set firstind [lindex $selindices 0]
            $ta tag add sel "$firstind linestart" $firstind
        }

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
# note: multiple (i.e. block) selection is supported: the regexp is
# applied range by range (i.e. line by line in case of a block
# selection)

    # application of the regexp should be seen as an atomic operation,
    # even for multiple (block) selections
    set oldSeparator [$ta cget -autoseparators]
    if {$oldSeparator} {
        $ta configure -autoseparators 0
        $ta edit separator
    }

    set selindices [gettaselind $ta any]

    # prevent puttext from messing selections
    $ta tag remove sel 1.0 end
    set newselindices [list ]

    foreach {sta sto} $selindices {
        # these marks will maintain the selection boundaries correctly
        # even during puttext below
        $ta mark set leftbound [$ta index $sta]
        $ta mark gravity leftbound left
        $ta mark set rightbound [$ta index $sto]
        $ta mark gravity rightbound right

        set seltext [$ta get leftbound rightbound]

        # remove spaces at the beginning of each line of the selected string
        regsub -all -line -- $REpat $seltext $repltext newtext

        # delete selection range, preserve bounds marks
        $ta mark set insert $sto
        $ta delete $sta $sto

        # note that puttext (correctly) collapses multiple selections
        puttext $ta $newtext "forceinsert"

        # save new selection bounds, and cleanup marks
        lappend newselindices [$ta index leftbound] [$ta index rightbound]
        $ta mark unset leftbound rightbound
    }

    # restore selection with updated bounds
    foreach {sta sto} $newselindices {
        $ta tag add sel $sta $sto
    }

    # application of the regexp should be seen as an atomic operation,
    # even for multiple (block) selections
    if {$oldSeparator} {
        $ta edit separator
        $ta configure -autoseparators 1
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
