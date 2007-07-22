proc blinkchars {w sta sto} {
# blink characters in $w between indices $sta and $sto
# $sta is supposed to be before $sto in $w
    $w tag add sel $sta $sto
    $w see $sto
    update idletasks
    after 300
    $w tag remove sel $sta $sto
    $w see insert
}

proc blinkbrace {w pos brace} {
# blink an entity delimited by matching brackets, braces or parenthesis

    # in very special cases, $brace contains two characters
    # this happens for instance when typing ^( : both characters appear
    # at the same time in $brace, and this would produce a Tcl error
    # "couldn't compile regexp" in blinkbrace because ^ has a meaning as a
    # regexp - this is fixed by sending only the last character of $brace
    # to proc blinkbrace
    set brace [string range $brace end end]

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

    set sta [getrealstartofcontline $w $pos]
    set sto [getrealendofcontline   $w $pos]
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
# this proc gets called when the user types a brace, one of ()[]{}
    if {[IsBufferEditable] == "No"} {return}
    puttext $w $brace "replaceallowed"
    blinkbrace $w insert $brace 
}

proc insblinkquote {w quote} {
    if {[IsBufferEditable] == "No"} {return}
    puttext $w $quote "replaceallowed"
    blinkquote $w insert $quote 
}

proc insertnewline {w} {
    global buffermodifiedsincelastsearch
    global listoffile
    global tabinserts indentspaces usekeywordindent

    if {[IsBufferEditable] == "No"} {return}

    # set the list of keywords that will create a forward indentation
    # (Indent) on next line, and the list of keywords that will create
    # a backward indentation (UnIndent) on current line
    if {$listoffile("$w",language) == "scilab"} {
        set fwindentkwlist [list \
                for while \
                if else elseif \
                select case \
                function ]
        set bwindentkwlist [list \
                else elseif \
                end \
                endfunction ]
    } else {
        set fwindentkwlist [list ]
        set bwindentkwlist [list ]
    }

    # get the leading indentation characters on the line where
    # the cursor is at the time enter is pressed
    set n {}
    $w mark set p1 {insert linestart}
    set c [$w get p1 {p1+1c}]
    while {$c == { } || $c == "\t"} {
        $w mark set p1 {p1+1c}
        set c [$w get p1 {p1+1c}]
    }
    set n [$w get {insert linestart} p1]

    if {$usekeywordindent} {
        # get the first word of the line where enter has been pressed
        set endind [$w index {p1 wordend}]
        if {[$w compare $endind > insert]} {
            set endind insert
        }
        set kw [$w get p1 $endind]
        $w mark unset p1
    }

    # so only one undo is required to undo indentation
    set oldSeparator [$w cget -autoseparators]
    if {$oldSeparator} {
        $w configure -autoseparators 0
        $w edit separator
    }

    # insert a CR char and insert the same indentation as the previous line
    # note that if there is a block selection, puttext will first collapse
    # it to its first range (line)
    # note further that the insert mode must be forced, otherwise there is no
    # means to create a new line while in replace mode
    puttext $w "\n$n" "forceinsert"

    # remove possible indentation chars located after the insertion above
    set c [$w get insert "insert+1c"]
    while {$c == " " || $c == "\t"} {
        $w delete insert "insert+1c"
        set c [$w get insert "insert+1c"]
    }

    if {$usekeywordindent} {

        # if the starting keyword should destroy one indentation chunk
        if {[lsearch -exact $bwindentkwlist $kw] != -1} {
            # remove one indentation from the line on which the cursor was
            # when the user hit enter
            $w mark set p1 insert
            $w mark set insert "insert - 1 l linestart"
            UnIndentSel
            $w tag remove sel 1.0 end
            $w mark set insert p1
            $w mark unset p1
            # remove one indentation from the current line also
            if {$tabinserts == "spaces"} {
                set maxbackspace $indentspaces
            } else {
                set maxbackspace 1
            }
            for {set i 0} {$i<$maxbackspace} {incr i} {
                # don't cross the start of line boundary while erasing chars
                if {[$w compare insert > "insert linestart"]} {
                    backspacetext
                }
            }
        }

        # if the starting keyword should create one indentation chunk
        if {[lsearch -exact $fwindentkwlist $kw] != -1} {
            # if the line after insertion of \n$n above is not a blank line,
            # i.e. if the user did hit enter in the middle of a line, then
            # indent, otherwise insert a tab char (or indentation spaces)
            if {[string trim [selectline]] != ""} {
                IndentSel
            } else {
                # remove selection is useful when line contains only blanks
                $w tag remove sel 1.0 end
                inserttab $w
            }
            $w tag remove sel 1.0 end
       }
    }

    if {$oldSeparator} {
        $w edit separator
        $w configure -autoseparators 1
    }

    set buffermodifiedsincelastsearch true
}

proc inserttab {w} {
# if there is a selection in $w, and if this selection starts at column 0,
# then indent this selection
# otherwise insert a "tab", i.e. either (depending on the option selected)
# a real tab character or a sufficient number of spaces to go to the next
# tab stop
# note: block selection is supported: if there is a block selection, the
# behaviour is always to indent

    global indentspaces tabinserts buffermodifiedsincelastsearch

    if {[IsBufferEditable] == "No"} {return}

    set taselind [gettaselind $w any]

    if {$taselind != {}} {
        # there is a selection, put 1st column of selection in col
        scan [lindex $taselind 0] "%d.%d" line col
    } else {
        # there is no selection
        set col -1
    }

    if {$col == 0 || [llength $taselind] > 2} {
        # there is a selection starting at the 1st column, or the
        # selection is a block selection
        IndentSel
    } else {
        # there is no selection
        if {$tabinserts == "spaces"} {
            # insert spaces up to the next tab stop
            set curpos [$w index insert]
            scan $curpos "%d.%d" curline curcol
            set nexttabstop [expr {($curcol / $indentspaces + 1) * $indentspaces}]
            set nbtoinsert [expr {$nexttabstop - $curcol}]
            set toinsert ""
            for {set x 0} {$x<$nbtoinsert} {incr x} {
                append toinsert " "
            }
            puttext $w $toinsert "replaceallowed"
        } else {
            # insert a tab character
            puttext $w "\x9" "replaceallowed"
        }
    }

    set buffermodifiedsincelastsearch true
}

proc puttext {w text insertorreplace} {
# input text $text in textarea $w
# note: existing block selection, if any, gets collapsed in the process
# and then deleted resulting in the apparent (desired) result that the text
# passed to this proc replaced the first line (range) of the block selection
# only
# the parameter $insertorreplace allows to:
#   - ignore the replace mode if set to "forceinsert"
#   - let replace happen (when in this mode) if set to "replaceallowed"

    global listoffile buffermodifiedsincelastsearch
    global Tk85
    global textinsertmode

    if {[IsBufferEditable] == "No"} {return}

    # stop/restorecursorblink is primarily to fix bug 2239
    # but:
    # We want that when typing in a widget the line numbers in its margin be
    # updated automatically, i.e. we want that proc managescroll (which calls
    # updatelinenumbersmargin) be called automatically by Tk for each peer
    # To achieve this, we need to perform a configure action with any option
    # on all the peer text widgets, i.e. we must use $ta configure -anyoption xxx
    # This is quite easily done by stopping and restoring cursor blinking, that
    # does precisely $ta configure -insertofftime xx
    # Note that we must in principle do it only for peers since typing in a widget
    # cannot change any other non-peer text widget, but it's easier to do it for
    # all textareas, and it's also needed to fix bug 2239
    stopcursorblink

    foreach ta [getfullpeerset $w] {
        set listoffile("$ta",redostackdepth) 0
    }

    set oldSeparator [$w cget -autoseparators] ;# in case this proc is called from another proc
    if {$oldSeparator} {
        $w configure -autoseparators 0 ;# so only one undo is required to undo text replacement
        $w edit separator
    }

    # check whether a selection exists, collapse any block selection
    # and delete it
    if {[gettaselind $w single] != ""} {
        $w delete sel.first sel.last
        set aselectionwasdeleted true
    } else {
        set aselectionwasdeleted false
    }

    set i1 [$w index insert]

    if {$textinsertmode || $insertorreplace!="replaceallowed"} {

        $w insert insert $text

    } else {

        # if there was initially a selection, then no further delete should occur
        if {$aselectionwasdeleted} {
            set replacelength 0
        } else {
            set replacelength [string length $text]
        }
        # don't span the \n at the end of the line
        if {[$w compare "insert + $replacelength c" > "insert lineend"]} {
            set replaceendind "insert lineend"
        } else {
            set replaceendind "insert + $replacelength c"
        }
        # do the replace
        if {$Tk85} {
            $w replace insert $replaceendind $text
        } else {
            # emulate the .text replace command of Tk8.5 with 8.4 commands only
            $w delete insert $replaceendind
            $w insert insert $text
        }

    }

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

    restorecursorblink ; # see comments above
}

proc printtime {} {
#procedure to set the time change %R to %I:%M for 12 hour time display
    global listoffile buffermodifiedsincelastsearch
    if {[IsBufferEditable] == "No"} {return}
    foreach ta [getfullpeerset [gettextareacur]] {
        set listoffile("$ta",redostackdepth) 0
    }
    [gettextareacur] tag remove sel 1.0 end
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

proc toggleinsertreplacemode {} {
# switch insert mode to replace mode in textareas, or vice versa
# the visual appearance of the text cursor is also changed
    global Tk85 textinsertmode
    global textinsertcursorwidth textreplacecursorwidth
    global textinsertcursorborderwidth textreplacecursorborderwidth
    global listoftextarea

    set textinsertmode [expr !$textinsertmode]

    if {$textinsertmode} {
        # cursor is the insert cursor (I shaped)
        # note: peers do not share a common cursor --> no filteroutpeers here!
        if {$Tk85} {
            foreach w $listoftextarea {
                $w configure -blockcursor false
            }
        } else {
            foreach w $listoftextarea {
                $w configure -insertwidth $textinsertcursorwidth \
                             -insertborderwidth $textinsertcursorborderwidth
            }
        }
    } else {
        # cursor is the replace cursor (block shape)
        # note: peers do not share a common cursor --> no filteroutpeers here!
        if {$Tk85} {
            foreach w $listoftextarea {
                $w configure -blockcursor true
            }
        } else {
            foreach w $listoftextarea {
                $w configure -insertwidth $textreplacecursorwidth \
                             -insertborderwidth $textreplacecursorborderwidth
            }
        }
    }
}
