#cut text procedure
proc deletetext {} {
    global textareacur

    set cuttexts [selection own]
    if {$cuttexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {   	
	    [gettextareacur] delete "insert" "insert +1c"
	} else {
	    $cuttexts delete sel.first sel.last
	    selection clear
	}
    } else {
        [gettextareacur] delete "insert" "insert +1c"
    }
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] \
	[[gettextareacur] index "$i1 wordend"]
# FV 13/05/04
    reshape_bp
}

#cut text procedure
proc backspacetext {} {
    global textareacur
    set cuttexts [selection own]
    if {$cuttexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {   	
	    [gettextareacur] delete "insert-1c" "insert"
	} else {
	    $cuttexts delete sel.first sel.last
	    selection clear
	}
    } else {
        [gettextareacur] delete "insert-1c" "insert"
    }
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] \
	[[gettextareacur] index "$i1 wordend"]
# FV 13/05/04
    reshape_bp
}

#cut text procedure
proc cuttext {} {
    global textareacur
    tk_textCut [gettextareacur]
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 linestart"] \
	[[gettextareacur] index "$i1 lineend"]
    selection clear
# FV 13/05/04
    reshape_bp
}

#copy text procedure
proc copytext {} {
    global textareacur
    tk_textCopy  [gettextareacur]
#ES: why? just copying does not alter the buffer
#    inccount [gettextareacur]
#ES: isn't it nicer if it stays selected?
#    selection clear
}

#paste text procedure
proc pastetext {} {
    global textareacur
    global tcl_platform
    if {"$tcl_platform(platform)" == "unix"} {
	catch {
	    [gettextareacur] delete sel.first sel.last
	}
    } else {
	catch {
	    [gettextareacur] delete sel.first sel.last
	}
    }
    set i1  [[gettextareacur] index insert]
    tk_textPaste [gettextareacur] 
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i2 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] \
	[[gettextareacur] index "$i2 wordend"]
#added by ES (but might also be unhandy)
#    $textareacur tag add sel $i1 $i2
# FV 13/05/04
    reshape_bp
}
