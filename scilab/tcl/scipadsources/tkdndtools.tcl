proc tkdndbind {w} {
# Drag and drop feature using TkDnD

    global TkDnDloaded pad dndinitiated sourcetextarea
    global savedsel savedseli1 savedseli2 dnddroppos

    # Abort if TkDnD package is not available
    if {$TkDnDloaded != "true"} {return}

    # To ensure that the widgets have been created before binding to them
    update idletasks

    # Drag and drop files or directories to Scipad - Just a few lines!
    dnd bindtarget $pad text/uri-list <Drop> "openlistoffiles %D"
    dnd bindtarget $pad text/uri-list <Drag> "return %A"
    dnd bindtarget $w   text/uri-list <Drop> "openlistoffiles %D"
    dnd bindtarget $w   text/uri-list <Drag> "return %A"

    # Drag and drop text within Scipad, or coming from the outside - More complicated!
    dnd bindtarget $w text/plain <Drop> {
        if {[info exists sourcetextarea]} {
            # drag from Scipad, drop in Scipad
            if {$sourcetextarea == "%W"} {
                # drag and drop in the same Scipad buffer
                if {[%W tag ranges sel] != ""} {
                    if { ! ([%W compare sel.first <= [%W index @%x,%y]] && \
                            [%W compare [%W index @%x,%y] <= sel.last] ) } {
                        if {"%A" == "copy"} {
                            %W tag remove sel 0.0 end
                        }
                        puttext %W %D
                    }
                }
            } else {
                # drag from a Scipad buffer, drop in another Scipad buffer
                %W tag remove sel 0.0 end
                puttext %W %D
                if {"%A" == "move"} {
                    focustextarea $sourcetextarea
                    $sourcetextarea tag add sel $savedseli1 $savedseli2
                    deletetext
                }
            }
            unset sourcetextarea
        } else {
            # drag from outside of Scipad, drop in Scipad
            %W tag remove sel 0.0 end
            puttext %W %D   
        }
        restorecursorblink ; # needed for drags from the outside
        set dndinitiated "false"
        if {[info exists dnddroppos]} {
            %W mark set insert $dnddroppos
            unset dnddroppos
        }
    }

    dnd bindsource $w text/plain {
        set sourcetextarea %W
        return $savedsel
    }

    dnd bindtarget $w text/plain <Drag> {
        %W mark set insert @%x,%y
        %W see insert
        set dnddroppos [%W index @%x,%y]
        if {%x <= 10} { %W xview scroll -1 units }
        if {%x >= [expr [winfo width  %W] - 10]} { %W xview scroll 1 units }
        if {%y <= 10} { %W yview scroll -1 units }
        if {%y >= [expr [winfo height %W] - 10]} { %W yview scroll 1 units }
        update idletasks
        if {"%m" == "Control"} {
            return copy
        } else {
            return move
        }
    }

    dnd bindtarget $w text/plain <DragEnter> {
        # select the target textarea - update makes the cursor visible
        # focus -force makes cursor visible even for drags from Scipad's outside
        focus -force %W
        focustextarea %W
        update
        stopcursorblink ; # needed for drags from the outside
        return %a
    }

    # break prevents class binding from triggering
    bind $w <Button-1>         { if {[Button1BindTextArea %W %x %y] == "true"} {break} }
    bind $w <Control-Button-1> { if {[Button1BindTextArea %W %x %y] == "true"} {break} }
    bind $w <ButtonRelease-1>  { if {$dndinitiated == "true"} { \
                                     set dndinitiated "false" ; \
                                 } else { \
                                     focustextarea %W ; \
                                 } \
                               }
    bind $w <Button1-Leave>    { if {$dndinitiated == "true"} {break} }
    bind $w <Double-Button-1>  { if {$dndinitiated == "true"} {break} }
    bind $w <Triple-Button-1>  { if {$dndinitiated == "true"} {break} }
    bind $w <Button1-Motion>   { if {$dndinitiated == "true"} {break} }

    # Nice cursor change when mouse is over the selection
    $w tag bind sel <Enter> {%W configure -cursor hand2 ; set mouseoversel "true"}
    $w tag bind sel <Leave> {%W configure -cursor xterm ; set mouseoversel "false"}
}

proc Button1BindTextArea { w x y } {
    global dndinitiated savedsel savedseli1 savedseli2
    $w mark set insert @$x,$y
    set retvalue "false"
    if {[$w tag ranges sel] != ""} {
        if { [$w compare sel.first <= [$w index @$x,$y]] && \
             [$w compare [$w index @$x,$y] < sel.last] } {
            set dndinitiated "true"
            set retvalue "true"
            set savedseli1 [$w index sel.first]
            set savedseli2 [$w index sel.last ]
            set savedsel [$w get sel.first sel.last]
            stopcursorblink
            dnd drag $w -actions {copy move}
            # (Windows) dnd drag returns only after the drop action (in case
            # mouse has moved) or after button release (if no mouse move)
            # if drop has been called by dnd drag, cursor blinking is already
            # restored but if the mouse did not move then drop has not been
            # called and cursor blinking must be restored here
            restorecursorblink
        }
    }
    return $retvalue
}

proc stopcursorblink {} {
# What is inside the if {$cursorblink == "true"} is needed to ensure that
# the cursor is on during a drag. This is a workaround to avoid Tk bug 1169429
# See https://sourceforge.net/tracker/?func=detail&atid=112997&aid=1169429&group_id=12997
# The proper fix is included in any Tk version strictly greater than 8.5.0 beta 3
    global cursorblink listoftextarea
    if {$cursorblink == "true"} {
        foreach ta $listoftextarea {
            $ta configure -insertofftime 2
        }
        update
        after 5
        foreach ta $listoftextarea {
            $ta configure -insertofftime 0
        }
        update
    }
}

proc restorecursorblink {} {
    global cursorblink listoftextarea
    if {$cursorblink == "true"} {
        foreach ta $listoftextarea {
            $ta configure -insertofftime 500
        }
    }
}
