proc blinkbrace {w pos brace} {
    global	bracefind fno

    switch $brace {
        \{	{ set findbs {[{}]}; set bs "\}";\
                            set dir {-forwards}; set bn brace }
        \}	{ set findbs {[{}]}; set bs "\{"; \
                            set dir {-backwards}; set bn brace }
        \[	{ set findbs {[][]}; set bs "\]"; \
                            set dir {-forwards}; set bn bracket }
        \]	{ set findbs {[][]}; set bs "\["; \
                            set dir {-backwards}; set bn bracket }
        \(	{ set findbs {[()]}; set bs "\)"; set dir {-forwards};\
                            set bn parenthesis }
        \)	{ set findbs {[()]}; set bs "\("; \
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
}

proc puttext {w text} {
    global winTitle
    if {[IsBufferEditable] == "No"} {return}
    set rem 0
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
    if {$i1 != $i2 || $rem} {
        colorize $w $i1 [$w index "$i2+1l linestart"]
    }
    reshape_bp
    $w see insert
    inccount [gettextareacur]
}

proc getccount {textarea} {
    global listoffile
    return $listoffile("$textarea",save)
}

proc inccount {textarea} {
    global listoffile
    set listoffile("$textarea",save) 1
    modifiedtitle $textarea
}

proc outccount {textarea} {
    global listoffile
    set listoffile("$textarea",save) 0
    modifiedtitle $textarea
}

proc printtime {} {
#procedure to set the time change %R to %I:%M for 12 hour time display
    global textareacur
    if {[IsBufferEditable] == "No"} {return}
    [gettextareacur] insert insert [clock format [clock seconds] \
                    -format "%R %p %D"]
    inccount [gettextareacur]
}

proc wraptext {} {
# binding for wordwrap
    global wordWrap
    global textareacur
    if [expr [string compare $wordWrap word] == 0] {
        set wordWrap none	
    } else {
        set wordWrap word
    }
    [gettextareacur] configure -wrap $wordWrap
}

proc IsBufferEditable {} {
    if {[getdbstate]=="DebugInProgress"} {
        showinfo [mc "Code editing is not allowed during debug!"]
        return "No"
    } else {
        return "Yes"
    }
}
