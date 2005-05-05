proc tkdndbind {w} {
# Drag and drop feature using TkDnD
    global TkDnDloaded savedsel extendsel cursorblink
    if {$TkDnDloaded == "true"} {

# To ensure that the widgets have been created before binding to them
        update idletasks

# Drag and drop files or directories to Scipad - Just two lines!
        dnd bindtarget $w text/uri-list <Drop> "openlistoffiles %D"
        dnd bindtarget $w text/uri-list <Drag> "return %A"

# Drag and drop text within Scipad - More complicated!
        dnd bindtarget $w text/plain <Drop> { \
            if {[%W tag ranges sel] != ""} { \
                if { ! ([%W compare sel.first <= [%W index @%x,%y]] && \
                        [%W compare [%W index @%x,%y] <= sel.last] ) } { \
                    if {"%A" == "copy"} { \
                        %W tag remove sel 0.0 end \
                    } ; \
                    puttext %W %D ; \
                } ; \
            } ; \
            if {$cursorblink == "true"} { \
                %W configure -insertofftime 500 \
            } \
        }
        dnd bindsource $w text/plain { \
            if {[%W tag ranges sel] != ""} { \
                set savedsel [%W get sel.first sel.last] \
            } else { \
                set savedsel "" \
            } ; \
            return $savedsel \
        }
        dnd bindtarget $w text/plain <Drag> { \
            %W mark set insert @%x,%y ; \
            %W see insert ; \
            if {%x <= 10} { %W xview scroll -1 units } ; \
            if {%y <= 10} { %W yview scroll -1 units } ; \
            update idletasks ; \
            if {"%m" == "Control"} { \
                return copy \
            } else { \
                return move \
            } \
        }
        bind $w <Button-1>         { Button1BindText %W %x %y }
        bind $w <Control-Button-1> { Button1BindText %W %x %y }
        bind $w <ButtonRelease-1> {
            set extendsel "false" ; \
            tk::CancelRepeat ; \
            keyposn %W \
        }
        bind $w <Button1-Motion> { \
            if {$extendsel == "true"} { \
                set tk::Priv(x) %x ; \
                set tk::Priv(y) %y ; \
                tk::TextSelectTo %W %x %y \
            } elseif {$extendsel == "false"} { ; \
                if {[%W tag ranges sel] != ""} { \
                    if { [%W compare sel.first <= [%W index @%x,%y]] && \
                         [%W compare [%W index @%x,%y] <= sel.last] } { \
                        set extendsel "ignoremotion" ; \
                        dnd drag %W -actions {copy move} \
                    } \
                } ; \
            } ; \
        }
        $w tag bind sel <Enter> {%W configure -cursor hand2 ; set mouseoversel "true"}
        $w tag bind sel <Leave> {%W configure -cursor xterm ; set mouseoversel "false"}
        set extendsel "false"
    }
}

proc Button1BindText { w x y } {
# What is inside the if {$cursorblink == "true"} is needed to ensure that
# the cursor is on during a drag. This is a workaround to avoid tk bug 1169429
# See https://sourceforge.net/tracker/?func=detail&atid=112997&aid=1169429&group_id=12997
# The proper fix is included in any tk version strictly greater than 8.5.0 beta 3
    global extendsel cursorblink
    set extendsel "true"
    tk::TextButton1 $w $x $y ; \
    if {[$w tag ranges sel] != ""} { \
        if { [$w compare sel.first <= [$w index @$x,$y]] && \
             [$w compare [$w index @$x,$y] <= sel.last] } { \
            if {$cursorblink == "true"} { \
                $w configure -insertofftime 2 ; update ; \
                after 5 ; \
                $w configure -insertofftime 0 ; update \
            } ; \
            dnd drag $w -actions {copy move} \
        } \
    } ; \
    $w tag remove sel 0.0 end \
}
