proc blinkchars {w sta sto} {
# blink characters in $w between indices $sta and $sto
# $sta is supposed to be before $sto in $w
    $w tag add sel $sta $sto
    $w see $sto
    update idletasks
    after 300
    $w tag remove sel $sta $sto
    selection clear
    $w see insert
}

proc blinkbrace {w pos brace} {
# blink an entity delimited by matching brackets, braces or parenthesis
    if {[regexp \\$brace "\{\}\[\]\(\)"] == 0} return

    switch $brace {
        \{ { set findbs {[{}]}; set bs "\}";\
                            set dir {-forwards} }
        \} { set findbs {[{}]}; set bs "\{"; \
                            set dir {-backwards} }
        \[ { set findbs {[][]}; set bs "\]"; \
                            set dir {-forwards} }
        \] { set findbs {[][]}; set bs "\["; \
                            set dir {-backwards} }
        \( { set findbs {[()]}; set bs "\)"; \
                            set dir {-forwards} }
        \) { set findbs {[()]}; set bs "\("; \
                            set dir {-backwards} }
    }
    set p [$w index $pos-1c]
    set d 1
    while {$d > 0} {
        if {$dir == "-backwards"} {
            set p [$w search $dir -regexp $findbs $p 1.0]
        } else {
            set p [$w search $dir -regexp $findbs "$p+1c" end]
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
            blinkchars $w $p $pos
        } else {
            blinkchars $w $pos-1c $p+1c
        }
    } else {
        showinfo [concat [mc "No corresponding <"] $bs [mc "> found!"] ]
    }
}

proc blinkquote {w pos char} {
# blink quotes that define a character string
# <TODO>: - take care of quotes inside comments
#         - blink single quotes (beware of the transpose case)
    global listoffile

    if {$listoffile("$w",language) != "scilab"} {return}
    if {[regexp \\$char "\""] == 0} {return}

    if {[iscontinuedline $w "$pos - 1 l"]} {
        set sta [getstartofcontline $w "$pos - 1 l"]
    } else {
        set sta [$w index "$pos linestart"]
    }
    if {[iscontinuedline $w $pos]} {
        set sto [getendofcontline $w $pos]
    } else {
        set sto [$w index "$pos lineend"]
    }
    set i [$w index "$pos - 1 c"]

    set pat {"[^"]*(?:""[^"]*)*"}

    proc tryright {w pat i pos sto} {
    # try to find and blink a closing quote (i.e. at the right of the
    # quote that was just typed)
        set allind1 [regexp -all -indices -inline -- $pat [$w get $i $sto]]
        set allind2 [regexp -all -indices -inline -- $pat [$w get $pos $sto]]

        if {[llength $allind1] == 0} {
            # no closing quote found
            return
        } else {
            # a closing quote has been found after the
            # quote that was just typed
            set p [$w index "$i + [lindex [lindex $allind1 0] 1] c + 1 c"]
            blinkchars $w $i $p
        }
    }

    # try first to find an opening quote matching the quote
    # that was just typed (i.e. try towards the left)
    set allind1 [regexp -all -indices -inline -- $pat [$w get $sta $pos]]
    set allind2 [regexp -all -indices -inline -- $pat [$w get $sta $i]]

    if {[llength $allind1] == 0} {
        # the quote just typed is an opening quote
        # search for the closing quote
        tryright $w $pat $i $pos $sto
        return
    }

    if {[llength $allind1] != [llength $allind2]} {
        # the quote just typed closes a string
        # that starts before this quote
        set p [$w index "$sta + [lindex [lindex $allind1 end] 0] c"]
        blinkchars $w $p $pos
    } else {
        foreach {i1 j1} [lindex $allind1 end] {}
        foreach {i2 j2} [lindex $allind2 end] {}
        if {$i1 == $i2} {
            if {$j1 == $j2} {
                # the quote just typed is an opening quote
                # search for the closing quote
                tryright $w $pat $i $pos $sto
            } else {
                # the quote just typed closes a string
                # that contains a quote, e.g. "abc""def"
                set p [$w index "$sta + $i1 c"]
                blinkchars $w $p $pos
            }
        } else {
            # shouldn't happen
            tk_messageBox -message "Impossible case #1 in blinkquote"
        }
    }
}

proc insblinkbrace {w brace} {
    if {[IsBufferEditable] == "No"} {return}
    puttext $w $brace
    blinkbrace $w insert $brace 
}

proc insblinkquote {w quote} {
    if {[IsBufferEditable] == "No"} {return}
    puttext $w $quote
    blinkquote $w insert $quote 
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
        backgroundcolorizeuserfun
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
