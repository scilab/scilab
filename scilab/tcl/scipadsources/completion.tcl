proc getcompletions {tok} {
# return the completions for $tok, i.e. the list of keywords
# that start with $tok
# output is a list of elements being themselves lists of 2 elements
# each such element is the list {tag completion}, completion
# being one of the possible completions for tag
# tag is one of {comm intfun predef libfun scicos}
# if there's no completion in tag, {tag completion} is omitted
# the output list is sorted by alphabetical order of completions

    global chset words

    set tokinitial [string index $tok 0]

    set compl [list ]
    set mode "scilab"
    set tags {comm intfun predef libfun scicos}
 
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
            foreach cand $candidates {
                if {[string match "$tok*" $cand] && $tok != $cand} {
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
#   if 1<N<10, popup the possible completions menu
#   if N>9, ring a bell

    global pad textFont menuFont bgcolors fgcolors
    foreach c1 "$bgcolors $fgcolors" {global $c1}

    if {[IsBufferEditable] == "No"} {return}
    set ta [gettextareacur]

    # no completion is possible is cursor is at the beginning of a line
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
    set compl [getcompletions $startofword]

    # count number of completions
    set nbcompl [llength $compl]

    if {$nbcompl == 0} {
        bell
        showinfo [mc "No possible completion"]
        return

    } elseif {$nbcompl == 1} {
        completewith [lindex [lindex $compl 0] 1] $ind $ta
        return

    } elseif {$nbcompl > 15} {
        bell
        showinfo [concat $nbcompl [mc "possible completions"]]
        return

    } else {
        # create a popup menu containing the colorized completions
        catch {destroy $pad.popcompl}
        menu $pad.popcompl -tearoff 0 -font $menuFont \
            -title [mc "Possible completions"]
        foreach posscompl $compl {
            set tag [lindex $posscompl 0]
            set completedword [lindex $posscompl 1]
            $pad.popcompl add command -label $completedword \
                -command "completewith $completedword $ind $ta"
            # colorization settings
            switch -- $tag {
                comm    {set col $COMMCOLOR}
                intfun  {set col $INTFCOLOR}
                predef  {set col $PDEFCOLOR}
                libfun  {set col $LFUNCOLOR}
                scicos  {set col $SCICCOLOR}
                default {set col black ;# shouldn't happen}
            }
            $pad.popcompl entryconfigure last \
                -foreground $col -activeforeground $col\
                -background $BGCOLOR
            $pad.popcompl entryconfigure last -activebackground [shade \
                [$pad.popcompl entrycget last -activeforeground] \
                [$pad.popcompl entrycget last -background] 0.5]
        }

        # compute position for the popup menu, first in $ta coordinate system
        $ta see insert ; # otherwise dlineinfo returns an empty string
        set startofline [$ta get "insert linestart" insert]
        set startoflinewidth [font measure $textFont $startofline]
        set insertbbox [$ta dlineinfo insert]
        set posx [expr [lindex $insertbbox 0] + $startoflinewidth]
        set posy [expr [lindex $insertbbox 1] + [lindex $insertbbox 3]]
        # now in screen coordinate system
        set posx [expr $posx + [winfo rootx $ta]]
        set posy [expr $posy + [winfo rooty $ta]]

        # display the popup menu
        tk_popup $pad.popcompl $posx $posy

# below, unsuccessfull attempts to have the tcl event loop working while the menu
# is posted (would allow for updating the list of completions while the user goes
# on typing when the menu is already posted)
# fundamental Windows issue here.
# See http://wiki.tcl.tk/3713 and http://wiki.tcl.tk/10643

#$pad.popcompl clone $pad.popcompl.clone tearoff
#$pad.popcompl.clone configure -title [$pad.popcompl cget -title]
#$pad.popcompl.clone post $posx $posy

#after 1000 {bell;event generate $pad.popcompl <<MenuSelect>>}
#tk_popup $pad.popcompl $posx $posy
#bind $pad.popcompl <<MenuSelect>> {update;bell;after 1000 {event generate $pad.popcompl <<MenuSelect>>}}

        return
    }
}

proc completewith {str ind ta} {
# insert the completed word $str after deletion of the start of word
# start index for deletion is identified by index $ind in textarea $ta
# end index for deletion is the insert index
    set oldSeparator [$ta cget -autoseparators] ;# in case this proc is called from another proc
    if {$oldSeparator} {
        $ta configure -autoseparators 0 ;# so only one undo is required to undo text replacement
        $ta edit separator
    }
    $ta delete $ind [$ta index insert]
    puttext $ta $str
    if {$oldSeparator} {
        $ta edit separator
        $ta configure -autoseparators 1
    }
}

proc gettagfromkeyword {keyw} {
# return the tag associated with the keyword $keyw
# $keyw is supposed to exist as a tagged word
# $keyw is supposed to be unique among tags since the first corresponding
# tag is returned

    global chset words

    set mode "scilab"
    set tags {comm intfun predef libfun scicos}
    set tokinitial [string index $keyw 0]

    foreach tag $tags {
        if {[string first $tokinitial $chset($mode.$tag)] == -1} {
            # no such initial in this tag,
            # therefore $keyw cannot be tagged with $tag
        } else {
            set candidates $words($mode.$tag.$tokinitial)
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
        # this is the first call of the proc (from bindings.tcl)
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
