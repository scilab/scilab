proc getcompletions {tok {mode ""}} {
# return the completions for $tok, i.e. the list of keywords
# that start with $tok
# output is a list of elements being themselves lists of 2 elements
# each such element is the list {tag completion}, completion
# being one of the possible completions for tag
# tag is by now one of {command intfun predef libfun scicos}
# if there's no completion in tag, {tag completion} is omitted
# the output list is sorted by alphabetical order of completions

    global chset words listoffile

    set tokinitial [string index $tok 0]

    set compl [list ]

    # recover the tags list from the mode
    if {$mode == ""} {
        set mode $listoffile("[gettextareacur]",language)
    } else {
        # currently, mode can only be "scilab" to have the completion
        # feature, therefore the caller of proc getcompletions must have
        # given "scilab" to mode
    }
    regsub -all "$mode." [array names chset -glob $mode\.*] "" tags

    foreach tag $tags {
        set indofinitial [string first $tokinitial $chset($mode.$tag)]
        if {$indofinitial == -1} {
            # there can be no completion in $mode.$tag if the
            # initials mismatch
            continue
        } else {
            # candidates start with the same initial as $tok
            set candidates \
                $words($mode.$tag.[string index $chset($mode.$tag) $indofinitial])
            set complfound {}
            if {$tag == "userfun"} {
                # special case : $words($mode.$tag) does not only contain
                # the functions names, therefore $candidates shall be reworked
                set funcand {}
                foreach elt $candidates {
                    lappend funcand [lindex $elt 0]
                }
                set candidates $funcand
            }
            foreach cand $candidates {
                if {[string match "$tok*" $cand]} {
                    # beginning of candidate keyword matches the token
                    lappend complfound [list $tag $cand]
                }
            }
            if {$complfound != {}} {
                lappend compl $complfound
            }
        }
    }

    set scompl {}
    foreach elt $compl {
        foreach posscompl $elt {
            lappend scompl $posscompl
        }
    }
    set scompl [lsort -index 1 -dictionary $scompl]

    return $scompl
}

proc popup_completions {} {
# get the characters before the cursor in the active textarea,
# get the N possible completions, and
#   if N=0, ring a bell
#   if N=1, pass the completion to puttext
#   if 1<N<16, popup the possible completions menu
#   if N>15, ring a bell

    global pad textFont menuFont bgcolors fgcolors chset listoffile
    global currentselcompl tcl_platform
    foreach c1 "$bgcolors $fgcolors" {global $c1}
    global buffermodifiedsincelastsearch

    if {[IsBufferEditable] == "No"} {return}
    set ta [gettextareacur]

    # no completion is possible if cursor is at the beginning of a line
    if {[$ta compare [$ta index insert] == [$ta index "insert linestart"]]} {
        bell
        return
    }

    # if there is a non-space character at the cursor position, no completion
    if {[$ta get insert] != " " && [$ta get insert] != "\n"} {
        bell
        return
    }

    # look for start of word to be completed
    set scilabnameschars "%_#!\$?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    set ind [$ta index "insert - 1 c"]
    while {![$ta compare [$ta index $ind] == [$ta index "$ind linestart"]] && \
           [string first [$ta get $ind] $scilabnameschars ] != -1 } {
        set ind [$ta index "$ind - 1 c"]
    }
    if {[string first [$ta get $ind] $scilabnameschars ] == -1} {
        set ind [$ta index "$ind + 1 c"]
    }
    set startofword [$ta get $ind [$ta index insert]]

    # get possible completions and count them
    set compl [getcompletions $startofword]
    set nbcompl [llength $compl]

    if {$nbcompl == 0} {
        bell
        showinfo [mc "No possible completion"]
        return

    } elseif {$nbcompl == 1} {
        completewith [lindex [lindex $compl 0] 1] $ind $ta
        set buffermodifiedsincelastsearch true
        return

    } elseif {$nbcompl > 15} {
        bell
        showinfo [concat $nbcompl [mc "possible completions"]]
        return

    } else {
        set buffermodifiedsincelastsearch true
        # create a popup "menu" containing the colorized completions
        # note: because of all this:
        #   http://wiki.tcl.tk/3713
        #   http://wiki.tcl.tk/10643
        #   http://sourceforge.net/tracker/index.php?func=detail&aid=585003&group_id=12997&atid=112997
        #   http://groups.google.fr/group/comp.lang.tcl/browse_thread/thread/be584c844d3cb442/15630c85782a19a2
        # the popup "menu" is not a regular tk popup menu, but a text
        # widget posted using the place geometry manager
        catch {destroy $pad.popcompl}
        text $pad.popcompl -font $menuFont \
                -bd 1 -relief solid -padx 2 -pady 2 -background $BGCOLOR
        set xpaddingspace [expr {([$pad.popcompl cget -bd] + [$pad.popcompl cget -padx]) * 2}]
        set ypaddingspace [expr {([$pad.popcompl cget -bd] + [$pad.popcompl cget -pady]) * 2}]
        set popw $xpaddingspace
        set poph $ypaddingspace
        foreach posscompl $compl {
            set tag [lindex $posscompl 0]
            set completedword [lindex $posscompl 1]
            $pad.popcompl insert end "$completedword\n"
            # compute popup size
            set itemw [font measure [$pad.popcompl cget -font] "$completedword\n"]
            if {$popw < [expr {$itemw + $xpaddingspace}]} {
                set popw [expr {$itemw + $xpaddingspace}]
            }
            # colorization settings
            $pad.popcompl tag add $tag "insert - 1c linestart" insert
        }
        # remove superfluous last newline
        $pad.popcompl delete insert
        # colorization settings
        set mode $listoffile("[gettextareacur]",language)
        regsub -all "$mode." [array names chset -glob $mode\.*] "" tags
        foreach tag $tags {
            switch -- $tag {
                command {set col $COMMCOLOR}
                intfun  {set col $INTFCOLOR}
                predef  {set col $PDEFCOLOR}
                libfun  {set col $LFUNCOLOR}
                scicos  {set col $SCICCOLOR}
                userfun {set col $USERFUNCOLOR}
                default {set col black ;# shouldn't happen}
            }
            $pad.popcompl tag configure $tag \
                -foreground $col -background $BGCOLOR
            # unix menu-like appearance of the selection in the popup
            if {$tcl_platform(platform) == "unix"} {
                set platformoptions "-relief raised -borderwidth 2"
            } else {
                set platformoptions ""
            }
            eval "$pad.popcompl tag configure sel$tag $platformoptions \
                    -foreground $col -background [shade \
                    [$pad.popcompl tag cget $tag -foreground] \
                    [$pad.popcompl tag cget $tag -background] 0.5]"
        }

        # display the popup menu
        # this is again tricky if we want to have the correct height of
        # the popup for all font sizes:
        #   1. place the widget offscreen so that is is not visible, and
        #      give him an enormous height to be sure all the entries will
        #      have enough space to be displayed
        #   2. update to force the (invisible) display
        #   3. compute height of the popup
        #   4. compute correct position wrt to the textarea, and correct
        #      this position if the popup once posted would extend beyond
        #      the screen
        #   5. place again the widget, this time at correct location and
        #      with correct width and height
        place $pad.popcompl -in $ta \
                -x [winfo screenwidth $pad] -y [winfo screenheight $pad] \
                -height [winfo screenheight $pad] -width [winfo screenwidth $pad]
        update
        # compute height of the popup
        for {set i 1} {$i <= $nbcompl} {incr i} {
            $pad.popcompl see "$i.0"
            set linebbox [$pad.popcompl dlineinfo "$i.0"]
            incr poph [lindex $linebbox 3]
        }
        # compute position for the popup menu in $ta coordinate system
        $ta see insert ; # otherwise dlineinfo returns an empty string
        set startofline [$ta get "insert linestart" insert]
        set startoflinewidth [font measure $textFont $startofline]
        set insertbbox [$ta dlineinfo insert]
        set posx [expr {[lindex $insertbbox 0] + $startoflinewidth}]
        set posy [expr {[lindex $insertbbox 1] + [lindex $insertbbox 3]}]
        if {[expr {$posx + $popw}] > [winfo width $ta]} {
            set posx [expr {$posx - $popw}]
        }
        if {[expr {$posy + $poph}] > [winfo height $ta]} {
            set posy [expr {$posy - $poph - [lindex $linebbox 3]}]
        }
        # place at final position
        place $pad.popcompl -in $ta \
                -x $posx -y $posy \
                -height $poph -width $popw

        # pre-select the first possible completion
        set curseltag [lindex [lindex $compl 0] 0]
        $pad.popcompl tag add sel$curseltag 1.0 2.0
        set currentselcompl 0

        # further configuration options
        $pad.popcompl configure -state disabled
        focus $pad.popcompl
        update ; # is this update really needed (Linux case grab failed error?)
        grab $pad.popcompl

        # usability bindings
        # [string map {"%" "%%"} $compl] is needed in the binding to prevent
        # Tcl from interpreting the possible % char as a substitution char, which
        # would lead to inserted completions starting by ?? (unknown completion)
        bind $pad.popcompl <Escape> \
                "destroy $pad.popcompl ; focus $ta ; break"
        bind $pad.popcompl <Return> \
                "completewith [list [string map {"%" "%%"} $compl]] $ind $ta ; destroy $pad.popcompl ; focus $ta ; break"
        bind $pad.popcompl <KP_Enter> \
                [bind $pad.popcompl <Return>]
        bind $pad.popcompl <Down> \
                "selectnextcompletion %W [list [string map {"%" "%%"} $compl]] ; break"
        bind $pad.popcompl <Up> \
                "selectpreviouscompletion %W [list [string map {"%" "%%"} $compl]] ; break"
        bind $pad.popcompl <Next> \
                "selectlastcompletion %W [list [string map {"%" "%%"} $compl]] ; break"
        bind $pad.popcompl <Prior> \
                "selectfirstcompletion %W [list [string map {"%" "%%"} $compl]] ; break"
        bind $pad.popcompl <Motion> \
                "selectmouseoverlaycompletion %W %x %y [list [string map {"%" "%%"} $compl]] $popw $poph ; break"
        bind $pad.popcompl <Button-1> \
                "completewithmouseselected %W %x %y [list [string map {"%" "%%"} $compl]] $ind $ta $popw $poph ; break"
        bind $pad.popcompl <KeyPress>  {popup_completions_again_KP %W %A %K %s ; break}
        bind $pad.popcompl <BackSpace> {popup_completions_again_BS %W ; break}
        bind $pad.popcompl <Delete>    {}

        return
    }
}

proc selectnextcompletion {w compl} {
# highlight the next possible completion in the popup
    global currentselcompl

    unselectcompletion $w $compl $currentselcompl

    set lastcompl [expr {[llength $compl] - 1}]
    incr currentselcompl
    if {$currentselcompl > $lastcompl} {
        set currentselcompl 0
    }

    selectcompletion $w $compl $currentselcompl
}

proc selectpreviouscompletion {w compl} {
# highlight the previous possible completion in the popup
    global currentselcompl

    unselectcompletion $w $compl $currentselcompl

    incr currentselcompl -1
    if {$currentselcompl < 0} {
        set currentselcompl [expr {[llength $compl] - 1}]
    }

    selectcompletion $w $compl $currentselcompl
}

proc unselectcompletion {w compl cn} {
# un-highlight completion identified by $cn
    set curseltag [lindex [lindex $compl $cn] 0]
    $w tag remove sel$curseltag 1.0 end
}

proc selectcompletion {w compl cn} {
# highlight completion identified by $cn
    set newpos "[expr {$cn + 1}].0"
    set curseltag [lindex [lindex $compl $cn] 0]
    $w tag add sel$curseltag $newpos "$newpos + 1l linestart"
}

proc selectlastcompletion {w compl} {
# highlight the last possible completion in the popup
    selectcompletionnumber $w $compl [expr {[llength $compl] - 1}]
}

proc selectfirstcompletion {w compl} {
# highlight the first possible completion in the popup
    selectcompletionnumber $w $compl 0
}

proc selectcompletionnumber {w compl cn} {
# highlight possible completion number $cn from the possible completions
# in the popup
    global currentselcompl
    unselectcompletion $w $compl $currentselcompl
    selectcompletion $w $compl $cn
    set currentselcompl $cn
}

proc selectmouseoverlaycompletion {w x y compl popw poph} {
    global pad
    # highlight on mouse movement only when Scipad has focus
    if {[focus -displayof $pad] != ""} {
        completionmouseselect $w $x $y $compl $popw $poph
    }
}

proc completewithmouseselected {w x y compl ind ta popw poph} {
    completionmouseselect $w $x $y $compl $popw $poph $ind $ta
}

proc completionmouseselect {w x y compl popw poph {ind ""} {ta ""}} {
# ancillary for selectmouseoverlaycompletion and completewithmouseselected
# (just to factor the code a bit more)
# if ind is provided, then it means that the proc was called by
# completewithmouseselected otherwise it was called by
# selectmouseoverlaycompletion
# call from completewithmouseselected:
#   destroy the popup if mouse is clicked outside of it, or
#   highlight the completion clicked in the popup, wait a bit
#   and pass this completion to the textarea
# call from selectmouseoverlaycompletion:
#   simply highlight the completion currently pointed by the mouse

    global pad currentselcompl

    # check if the mouse was clicked inside the popup or not
    if {$x < 0 || $x > $popw || $y < 0 || $y > $poph} {
        set inside false
    } else {
        set inside true
    }

    if {$inside} {
        unselectcompletion $w $compl $currentselcompl

        set mousepos [$w index "@$x,$y linestart"]
        scan [expr {$mousepos - 1}] "%d.%d" currentselcompl junk
        set curseltag [lindex [lindex $compl $currentselcompl] 0]
        $w tag add sel$curseltag $mousepos "$mousepos + 1l linestart"

        if {$ind != ""} {
            # completewithmouseselected
            completewith [lindex [lindex $compl $currentselcompl] 1] $ind $ta
            destroy $pad.popcompl
            focus $ta
        } else {
            # selectmouseoverlaycompletion - nothing special to do
        }
        set currentselcompl $currentselcompl
    } else {
        if {$ind != ""} {
            # completewithmouseselected
            # user clicked outside of the popup
            destroy $w
            focus $ta
        } else {
            # selectmouseoverlaycompletion - nothing special to do
        }
    }
}

proc completewith {str ind ta} {
# insert the completed word $str after deletion of the start of word
# if str is a single word, just insert it (used for single possible
# completion)
# if str is a list of completions, insert the completion identified by
# the global variable currentselcompl (used with the popup)
# start index for deletion is identified by index $ind in textarea $ta
# end index for deletion is the insert index
    global currentselcompl
    set oldSeparator [$ta cget -autoseparators] ;# in case this proc is called from another proc
    if {$oldSeparator} {
        $ta configure -autoseparators 0 ;# so only one undo is required to undo text replacement
        $ta edit separator
    }

    if {[llength $str] != 1} {
        set str [lindex [lindex $str $currentselcompl] 1]
    }

    $ta delete $ind [$ta index insert]
    puttext $ta $str "replaceallowed"
    if {$oldSeparator} {
        $ta edit separator
        $ta configure -autoseparators 1
    }
}

proc popup_completions_again_KP {w character keysym modstate} {
# insert the key pressed in the textarea and recompute the popup
# handling capital letters is a bit tricky:
# when the user hits say Shift-a, two KeyPress events are actually
# fired:
#   1. The first one has %A empty, %K (keysym) == Shift_L/R, and 
#      %s (state) == 0. This is just the single shift key
#   2. The second one has %A empty, %K the keysym corresponding to
#      the key letter, and %s has bit 1 set, thus
#      indicating that shift is pressed along with the letter

    destroy $w
    focus [gettextareacur]

    if {$character != ""} {
        # non-shifted character
        puttext [gettextareacur] $character "replaceallowed"
        popup_completions

    } elseif {$keysym == "Shift_L" || $keysym == "Shift_R"} {
        # first event firing when the user hits a shift key
        popup_completions

    } elseif {[expr {$modstate & 1}] != 0} {
        # modstate (actually %s field of the KeyPress event) has
        # bit 1 set (LSB) if the shift key is pressed
        puttext [gettextareacur] $character "replaceallowed"
        popup_completions

    } else {
        # do nothing, key combination not wanted during completion
    }
}

proc popup_completions_again_BS {w} {
# erase the character at the left of the cursor in the textarea
# and recompute the popup
    destroy $w
    focus [gettextareacur]
    backspacetext 
    popup_completions
}

proc gettagfromkeyword {keyw} {
# return the tag associated with the keyword $keyw
# $keyw is supposed to exist as a tagged word
# $keyw is supposed to be unique among tags since the first corresponding
# tag is returned

    global chset words

    set mode "scilab"
    regsub -all "$mode." [array names chset -glob $mode\.*] "" tags
    set tokinitial [string index $keyw 0]

    foreach tag $tags {
        if {[string first $tokinitial $chset($mode.$tag)] == -1} {
            # no such initial in this tag,
            # therefore $keyw cannot be tagged with $tag
        } else {
            set candidates $words($mode.$tag.$tokinitial)
            if {$tag == "userfun"} {
                # special case : $words($mode.$tag) does not only contain
                # the functions names, therefore $candidates shall be reworked
                set funcand {}
                foreach elt $candidates {
                    lappend funcand [lindex $elt 0]
                }
                set candidates $funcand
            }
            foreach cand $candidates {
                if {$keyw == $cand} {
                    return $tag
                }
            }
        }
    }
    return "error" ;# shouldn't happen
}

proc SetCompletionBinding {} {
# set the binding used for displaying the completions popup menu
# the default binding for the old key combination is retored
# no change is needed in this proc if new entries are added in the options menu
# provided the new entry does not overload some binding already used for
# another purpose

    global oldcompletionbinding completionbinding
    global Shift_Tab

    if {![info exists oldcompletionbinding]} {
        # this is the first call of the proc (from commonbindings.tcl)
    } else {
        # the proc was called from the options/completion menu

        # remove the previous binding
        pbind Text $oldcompletionbinding {}

        # restore the default binding for the old key combination
        if {[lequal $oldcompletionbinding "Tab"]} {
            bind Text <Tab> {inserttab %W ; break}
        } elseif {[lequal $oldcompletionbinding $Shift_Tab]} {
            pbind Text $Shift_Tab {UnIndentSel ; break}
        }
    }

    pbind Text $completionbinding {popup_completions ; break}
    set oldcompletionbinding $completionbinding
}

proc lequal {a b} {
# test whether list $a and list $b are equal
# this proc was copied from http://wiki.tcl.tk/43
    if {[llength $a] != [llength $b]} {return 0}
    if {[lindex $a 0] == $a} {return [string equal $a $b]}
    foreach i $a j $b {if {![lequal $i $j]} {return 0}}
    return 1
}
