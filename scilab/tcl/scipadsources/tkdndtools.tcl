proc tkdndbind {w} {
# Drag and drop feature using TkDnD
    global TkDnDloaded savedsel extendsel
    if {$TkDnDloaded == "true"} {

# Drag and drop files or directories to Scipad - Just two lines!
        dnd bindtarget $w text/uri-list <Drop> {openlistoffiles %D}
        dnd bindtarget $w text/uri-list <Drag> {return %A}

# Drag and drop text within Scipad - More complicated!
        dnd bindtarget $w text/plain <Drop> { \
            if {"%A" == "copy"} { \
                %W tag remove sel 0.0 end \
            } ; \
            puttext %W %D \
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
        bind Text <Button-1>         { Button1BindText %W %x %y }
        bind Text <Control-Button-1> { Button1BindText %W %x %y }
        bind Text <ButtonRelease-1> {
            set extendsel "false" ; \
            tk::CancelRepeat \
        }
        bind Text <Button1-Motion> { \
            if {$extendsel == "true"} { \
                set tk::Priv(x) %x ; \
                set tk::Priv(y) %y ; \
                tk::TextSelectTo %W %x %y \
            } else { ; \
                if {[%W tag ranges sel] != ""} { \
                    if { [%W compare sel.first <= [%W index @%x,%y]] && \
                         [%W compare [%W index @%x,%y] <= sel.last] } { \
                        dnd drag %W -actions {copy move} \
                    } \
                } ; \
            } ; \
        }
        $w tag bind sel <Enter> {%W configure -cursor hand2}
        $w tag bind sel <Leave> {%W configure -cursor xterm}
        set extendsel "false"
    }
}

proc Button1BindText { w x y } {
    global extendsel
    set extendsel "true"
    tk::TextButton1 $w $x $y ; \
    if {[$w tag ranges sel] != ""} { \
        if { [$w compare sel.first <= [$w index @$x,$y]] && \
             [$w compare [$w index @$x,$y] <= sel.last] } { \
            dnd drag $w -actions {copy move} \
        } \
    } ; \
    $w tag remove sel 0.0 end \
}
