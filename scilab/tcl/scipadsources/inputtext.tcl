proc blinkbrace {w pos brace} {
    global bracefind fno
    if {[regexp \\$brace "\{\}\[\]\(\)"] == 0} return
    switch $brace {
        \{ { set findbs {[{}]}; set bs "\}";\
                            set dir {-forwards}; set bn brace }
        \} { set findbs {[{}]}; set bs "\{"; \
                            set dir {-backwards}; set bn brace }
        \[ { set findbs {[][]}; set bs "\]"; \
                            set dir {-forwards}; set bn bracket }
        \] { set findbs {[][]}; set bs "\["; \
                            set dir {-backwards}; set bn bracket }
        \( { set findbs {[()]}; set bs "\)"; set dir {-forwards};\
                            set bn parenthesis }
        \) { set findbs {[()]}; set bs "\("; \
                            set dir {-backwards}; set bn parenthesis }
    }
    set p [set i [$w index $pos-1c]]
    set d 1
    while {$d > 0} {
        if {$dir == "-backwards"} {
            set p [$w search $dir -regexp $findbs $p 1.0]
        } else {
            set p [$w search $dir -regexp $findbs $p+1c end]
        }
        if {$p == {}} {
            set d -1
        } else {
            if {"[$w get $p $p+1c]" == "$bs"} {
                incr d -1
            } else {
                incr d 1
            }
        }
    }
    if {$d == 0} {
        if {$dir == "-backwards"} {
            $w tag add sel $p $pos
        } else {
            $w tag add sel $pos-1c $p+1c
        }
        $w see $p
        update idletasks
        after 300
        $w tag remove sel 1.0 end
        selection clear
        $w see insert
    } else {
        showinfo [concat [mc "No corresponding <"] $bs [mc "> found!"] ]
    }
}

proc insblinkbrace {w brace} {
    if {[IsBufferEditable] == "No"} {return}
    puttext $w $brace
    blinkbrace $w insert $brace 
}

proc insertnewline {w} {
    global buffermodifiedsincelastsearch
    if {[IsBufferEditable] == "No"} {return}
    set n {}
    $w mark set p1 {insert linestart}
    set c [$w get p1 {p1+1c}]
    while {$c == { } || $c == "\t"} {
        $w mark set p1 {p1+1c}
        set c [$w get p1 {p1+1c}]
    }
    set n [$w get {insert linestart} p1]
    $w mark unset p1
    puttext $w "\n$n"
    set c [$w get insert "insert+1c"]
    while {$c == " " || $c == "\t"} {
        $w delete insert "insert+1c"
        set c [$w get insert "insert+1c"]
    }
    set buffermodifiedsincelastsearch true
}

proc inserttab {w} {
    global indentspaces tabinserts buffermodifiedsincelastsearch
    set textarea [gettextareacur]
    if {[IsBufferEditable] == "No"} {return}
    set selstart [lindex [$textarea tag nextrange sel 0.0] 0]
    if {$selstart != ""} {
        # there is a selection, put 1st column of selection in col
        scan $selstart "%d.%d" line col
    } else {
        # there is no selection
        set col -1
    }
    if {$col == 0} {
        # there is a selection starting at the 1st column
        IndentSel
    } else {
        if {$tabinserts == "spaces"} {
            # insert spaces up to the next tab stop
            set curpos [$textarea index insert]
            scan $curpos "%d.%d" curline curcol
            set nexttabstop [expr ($curcol / $indentspaces + 1) * $indentspaces]
            set nbtoinsert [expr $nexttabstop - $curcol]
            set toinsert ""
            for {set x 0} {$x<$nbtoinsert} {incr x} {
                append toinsert " "
            }
            puttext $w $toinsert
        } else {
            # insert a tab character
            puttext $w "\x9"
        }
    }
    set buffermodifiedsincelastsearch true
}

proc puttext {w text} {
    global listoffile buffermodifiedsincelastsearch
    if {[IsBufferEditable] == "No"} {return}
    set listoffile("$w",redostackdepth) 0
    set oldSeparator [$w cget -autoseparators] ;# in case this proc is called from another proc
    if {$oldSeparator} {
        $w configure -autoseparators 0 ;# so only one undo is required to undo text replacement
        $w edit separator
    }
    set cuttexts [selection own]
    if {[string range $cuttexts 0 [expr [string length [gettextareacur]]-1]] == [gettextareacur]} {
        if [catch {selection get -selection PRIMARY} sel] {

        } else {
            $cuttexts delete sel.first sel.last
            selection clear
        }
    }
    set i1 [$w index insert]
    $w insert insert $text
    set i2 [$w index insert]
    if {$i1 != $i2} {
        tagcontlines $w
        set uplimit [getstartofcolorization $w $i1]
        set dnlimit [getendofcolorization $w $i2]
        colorize $w $uplimit $dnlimit
    }
    reshape_bp
    $w see insert
    if {$oldSeparator} {
        $w edit separator
        $w configure -autoseparators 1
    }
    set buffermodifiedsincelastsearch true
}


proc printtime {} {
#procedure to set the time change %R to %I:%M for 12 hour time display
    global listoffile buffermodifiedsincelastsearch
    if {[IsBufferEditable] == "No"} {return}
    set listoffile("[gettextareacur]",redostackdepth) 0
    [gettextareacur] insert insert [clock format [clock seconds] \
                    -format "%R %p %D"]
    set buffermodifiedsincelastsearch true
}


proc IsBufferEditable {} {
    if {[getdbstate]=="DebugInProgress"} {
        showinfo [mc "Code editing is not allowed during debug!"]
        bell
        return "No"
    } else {
        return "Yes"
    }
}
