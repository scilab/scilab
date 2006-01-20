proc tkdndbind {w} {
# Drag and drop feature using TkDnD

    global TkDnDloaded pad dndinitiated sourcetextarea
    global savedsel savedseli1 savedseli2 dnddroppos
    global debuglog logdetailedbindings

    # Abort if TkDnD package is not available
    if {$TkDnDloaded != "true"} {
        bind $w <ButtonRelease-1>  { \
            if {[info exists listoffile("%W",fullname)]} { \
                focustextarea %W ; \
            } \
        }
        return
    }

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
            if {$sourcetextarea == "%W" && [%W tag ranges sel] != ""} {
                # drag and drop in the same Scipad buffer
                #
                # if the mouse went out of the buffer during drag, and
                # finally drops in the same buffer as the drag buffer
                # the selection has been lost in the process because of
                # focustextarea below in the <DragEnter> event, thus the
                # drop process is the same as if two different buffers
                # were involved (-> else clause of this if above)
                #
                # here we didn't fly out of the buffer, sel tag still here
                if { ! ([%W compare sel.first <= [%W index @%x,%y]] && \
                        [%W compare [%W index @%x,%y] < sel.last] ) || \
                        [%W compare sel.first == [%W index @%x,%y]] } {
                    # the mouse, in fact the destination mark cursor, is not
                    # inside the selection
                    if {"%A" == "copy"} {
                        %W tag remove sel 0.0 end
                    }
                    puttext %W %D
                } else {
                    # drop occurs at the same place as drag, do nothing
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
            unset dnddroppos
        }
    }

    dnd bindsource $w text/plain {
        set sourcetextarea %W
        return $savedsel
    }

    dnd bindtarget $w text/plain <Drag> {
        set dndreallystarted "true"
        set dnddroppos [%W index @%x,%y]
        %W mark set insert $dnddroppos
        %W see insert
        if {%x <= 10} { %W xview scroll -1 units }
        if {%x >= [expr [winfo width  %W] - 10]} { %W xview scroll 1 units }
        if {%y <= 10} { %W yview scroll -1 units }
        if {%y >= [expr [winfo height %W] - 10]} { %W yview scroll 1 units }
        update idletasks
        if {[lsearch "%m" "Control"] != -1} {
            return copy
        } else {
            return move
        }
    }

    dnd bindtarget $w text/plain <DragEnter> {
        # select the target textarea - update makes the cursor visible
        # focus -force makes cursor visible even for drags from outside of Scipad
        focus -force %W
        focustextarea %W
        update
        stopcursorblink ; # needed for drags from the outside
        return %a
    }

    # break prevents class binding from triggering
    bind $w <Button-1>         { if {[Button1BindTextArea %W %x %y] == "true"} {break} }
    bind $w <Control-Button-1> { if {[Button1BindTextArea %W %x %y] == "true"} { \
                                     break ; \
                                 } else { \
                                     %W tag remove sel 0.0 end ; \
                                 } \
                               }
    # if {[info exists listoffile("%W",fullname]} in the else clause below
    # is required to solve a bug reported in the newsgroup at
    # http://groups.google.com/group/comp.soft-sys.math.scilab/browse_thread/thread/21cee717df71ab0/d5957386272e17b3
    # http://groups.google.com/group/comp.soft-sys.math.scilab/browse_thread/thread/875daad29986228e/ae651d1a948ba67c
    # The double click in the file/open box fires two <ButtonRelease-1>
    # events to the first buffer ($pad.new1), and when these events come
    # to Scipad this buffer has already been closed thanks to the automagical
    # close of the initial buffer
    bind $w <ButtonRelease-1>  { if {$dndinitiated == "true"} { \
                                     set dndinitiated "false" ; \
                                 } else { \
                                     if {[info exists listoffile("%W",fullname)]} { \
                                         focustextarea %W ; \
                                     } \
                                 } \
                               }
    bind $w <Button1-Leave>    { if {$dndinitiated == "true"} {break} }
    bind $w <Double-Button-1>  { if {$dndinitiated == "true"} {break} }
    bind $w <Triple-Button-1>  { if {$dndinitiated == "true"} {break} }
    bind $w <Button1-Motion>   { if {$dndinitiated == "true"} {break} }

    # Nice cursor change when mouse is over the selection
    $w tag bind sel <Enter> {%W configure -cursor hand2 ; set mouseoversel "true"}
    $w tag bind sel <Leave> {%W configure -cursor xterm ; set mouseoversel "false"}

    # debug log settings: log bindings for the new textarea
    if {$debuglog && $logdetailedbindings} {
        foreach sequ [bind $w] {
            set script [bind $w $sequ]
            bind $w $sequ "log \"\n----------------------\" ; \
                           log \"Bind $w $sequ triggered!\"; \
                           $script; \
                           log \"End of bind $w $sequ\"; \
                           log \"\n----------------------\n\" "
        }
    }

}

proc Button1BindTextArea { w x y } {
    global dndinitiated savedsel savedseli1 savedseli2 dndreallystarted
    $w mark set insert @$x,$y
    set dndreallystarted "false"
    if {[$w tag ranges sel] != ""} {
        if { [$w compare sel.first <= [$w index @$x,$y]] && \
             [$w compare [$w index @$x,$y] < sel.last] } {
            set dndinitiated "true"
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
    return $dndreallystarted
}

############################
# Temporary cursor blinking start/stop
#
# 1. proc dostopcursorblink implements a hack to ensure that the cursor is
#    on during a drag, so that it is visible
#    This is a workaround to avoid Tk bug 1169429
#    See https://sourceforge.net/tracker/?func=detail&atid=112997&aid=1169429&group_id=12997
#    The proper fix is included in any Tk version strictly greater than 8.5.0 beta 3
#
# 2. Launching the procs in the background prevents from having the dnd locks
#    issues that were previously reported in the newsgroup:
#    http://groups.google.fr/group/comp.soft-sys.math.scilab/browse_thread/thread/b07a13adc073623d/b4e07072205c0435
#
# Warning: if !$backgroundtasksallowed (this should only be a debug setting),
# the dnd locks issues are not fixed

proc stopcursorblink {} {
    global cursorblink backgroundtasksallowed
    if {$cursorblink} {
        if {$backgroundtasksallowed} {
            after cancel {dorestorecursorblink}
            after idle {dostopcursorblink}
        } else {
            dostopcursorblink
        }
    }
}

proc restorecursorblink {} {
    global cursorblink backgroundtasksallowed
    if {$cursorblink} {
        if {$backgroundtasksallowed} {
            after cancel {dostopcursorblink}
            after idle {dorestorecursorblink}
        } else {
            dorestorecursorblink
        }
    }
}

proc dostopcursorblink {} {
    global listoftextarea
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

proc dorestorecursorblink {} {
    global listoftextarea
    foreach ta $listoftextarea {
        $ta configure -insertofftime 500
    }
}

# End of temporary cursor blinking start/stop
############################
