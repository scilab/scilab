proc blinkbrace {w pos brace} {
	global	bracefind fno lang

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
            if {$lang == "eng"} {
                showinfo "No corresponding <$bs> found!"
            } else {
                showinfo "Aucun <$bs> correspondant trouvé !"
            }
	}
}

proc insblinkbrace {w brace} {
    puttext $w $brace
    blinkbrace $w insert $brace 
}

proc insquote {w quote} {
    $w insert insert $quote {textquoted}
}

########


# Added by Matthieu PHILIPPE 07/12/2001
# code coms from EDDI.tcl
proc insertnewline {w} {
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
### 

proc puttext {w text} {
    global winTitle
#   global pad
    set rem 0
    set cuttexts [selection own]
# FV 13/05/04, next line corrected (see bug #723)
# FV 07/06/04, further improved
#    if {[string range $cuttexts 0 [expr [string length $pad]-1]] == $pad} {}
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
# FV 13/05/04
    reshape_bp
    $w see insert
# FV 14/06/04
    inccount [gettextareacur]
}

# this sets saveTextMsg to 1 for message boxes
proc getccount {textarea} {
    global listoffile
    return $listoffile("$textarea",save)
}

proc inccount {textarea} {
    global listoffile
    global saveTextMsg

    set saveTextMsg 1 
    set listoffile("$textarea",save) 1
##ES 23/9/2003
    modifiedtitle $textarea
}
# this resets saveTextMsg to 0
proc outccount {textarea} {
    global listoffile
    global saveTextMsg
    set saveTextMsg 0 
    set listoffile("$textarea",save) 0
##ES 23/9/2003
    modifiedtitle $textarea
}

proc bindtext {textarea} {
    bind $textarea <Control-v> {pastetext}
    bind $textarea <Control-o> {}
    bind $textarea <Control-f> {}
    bind $textarea <KeyPress> { if {{%A} != {{}}} {puttext %W %A}}
    bind $textarea <Delete> { deletetext}
    bind $textarea <BackSpace> { backspacetext}
    bind $textarea <parenright> { if {{%A} != {{}}} {insblinkbrace %W %A}}
    bind $textarea <bracketright> { if {{%A} != {{}}} {insblinkbrace %W %A}} 
    bind $textarea <braceright>  { if {{%A} != {{}}} {insblinkbrace %W %A}}
}


#procedure to set the time change %R to %I:%M for 12 hour time display
proc printtime {} {
    global textareacur
    [gettextareacur] insert insert [clock format [clock seconds] \
					-format "%R %p %D"]
    inccount [gettextareacur]
}

# binding for wordwrap
proc wraptext {} {
    global wordWrap
    global textareacur
    if [expr [string compare $wordWrap word] == 0] {
	set wordWrap none	
    } else {
	set wordWrap word
    }
    [gettextareacur] configure -wrap $wordWrap
}

