proc FindIt {w} {
    global SearchString SearchPos SearchDir findcase regexpcase
    global pad SearchEnd find
    set textareacur [gettextareacur]
    if {[winfo exists $w]} {
        set pw $w
    } else {
        set pw $pad
    }
    if {$SearchString!=""} {
        if {$findcase=="1"} {
            set caset "-exact"
        } else {
            set caset "-nocase"
        }
        if {$regexpcase != "1"} {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ $textareacur search $caset -$SearchDir \
                                -- $SearchString $SearchPos]
            } else {
                set SearchPos [ $textareacur search $caset -$SearchDir \
                                -- $SearchString $SearchPos $SearchEnd]
            }
            set len [string length $SearchString]
        } else {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ $textareacur search $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos]
            } else {
                set SearchPos [ $textareacur search $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos $SearchEnd]
            }
            if {$SearchPos != "" } {
                # lineend below to be revisited once 8.5 is used (browse for TIP113)
                if {$findcase=="1"} {
                    regexp         -- $SearchString [$textareacur get $SearchPos "$SearchPos lineend"] res
                } else {
                    regexp -nocase -- $SearchString [$textareacur get $SearchPos "$SearchPos lineend"] res
                }
                set len [string length $res]
            } else {
                set len [string length $SearchString]
            }
        }
        if {$SearchPos != ""} {
            $textareacur see $SearchPos
            $textareacur mark set insert $SearchPos
            $textareacur tag remove foundtext 0.0 end
            $textareacur tag remove replacedtext 0.0 end
            $textareacur tag add foundtext $SearchPos  "$SearchPos + $len char"
            # <TODO>: these bindings are required to remove the foundtext tag after a find
            #         next triggered by F3. Once set, they will live in the textarea forever!
            bind $textareacur <KeyPress>    {+$textareacur tag remove foundtext 0.0 end}
            bind $textareacur <ButtonPress> {+$textareacur tag remove foundtext 0.0 end}
            if {[winfo exists $find]} {
                MoveDialogIfFoundHidden
            }
            keyposn $textareacur
            if {$SearchDir == "forwards"} {
                set SearchPos [$textareacur index "$SearchPos + $len char"]
            }         
        } else {
            if {$SearchEnd == "No_end"} {
                tk_messageBox -message \
                    [concat [mc "The string"] $SearchString [mc "could not be found"] ] \
                    -parent $pw -title [mc "Find"]
                set SearchPos [$textareacur index "insert"]
            } else {
                set answer [tk_messageBox -message \
                    [concat [mc "No match found in the selection for"] $SearchString [mc "\nWould you like to look for it in the entire text?"] ] \
                    -parent $pw -title [mc "Find"] -type yesno -icon question]
                if {![string compare $answer "yes"]} {
                    if {$SearchDir == "forwards"} {
                        set SearchPos [$textareacur index "insert + $len char"]
                    } else {
                        set SearchPos [$textareacur index "insert"]
                    }
                    set SearchEnd "No_end"
                    $textareacur tag remove sel 0.0 end
                } else {
                    if {$SearchDir=="forwards"} {
                        set SearchPos [$textareacur index sel.first]
                    } else {
                        set SearchPos [$textareacur index sel.last]
                    }
                }
            }
        }
    } else {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $pw -title [mc "Find"]
    }
}

proc ReplaceIt {once_or_all} {
    global SearchString SearchDir ReplaceString SearchPos findcase regexpcase
    global find SearchEnd
    set textareacur [gettextareacur]
    if {$SearchString != ""} {
        set foundtextrange [$textareacur tag ranges foundtext]
        if {$foundtextrange == {}} {
            # there is no already found matching text (Find Next was not hit)
            # therefore perform a search first
            if {$findcase=="1"} {
                set caset "-exact"
            } else {
                set caset "-nocase"
            }
            if {$regexpcase != "1"} {
                if {$SearchEnd == "No_end"} {
                    set SearchPos [ $textareacur search $caset -$SearchDir \
                                    -- $SearchString $SearchPos]
                } else {
                    set SearchPos [ $textareacur search $caset -$SearchDir \
                                    -- $SearchString $SearchPos $SearchEnd]
                }
            } else {
                if {$SearchEnd == "No_end"} {
                    set SearchPos [ $textareacur search $caset -$SearchDir \
                                    -regexp -- $SearchString $SearchPos]
                } else {
                    set SearchPos [ $textareacur search $caset -$SearchDir \
                                    -regexp -- $SearchString $SearchPos $SearchEnd]
                }
            }
        } else {
            # there is already some matching text found (Find Next was hit)
            # therefore just set the location where the replace should occur
            set SearchPos [lindex $foundtextrange 0]
        }
        if {$regexpcase != "1"} {
            set len [string length $SearchString]
        } else {
            if {$SearchPos != "" } {
                # lineend below to be revisited once 8.5 is used (browse for TIP113)
                if {$findcase=="1"} {
                    regexp         -- $SearchString [$textareacur get $SearchPos "$SearchPos lineend"] res
                } else {
                    regexp -nocase -- $SearchString [$textareacur get $SearchPos "$SearchPos lineend"] res
                }
                set len [string length $res]
            } else {
                set len [string length $SearchString]
            }
        }
        if {$SearchPos != ""} {
            $textareacur see $SearchPos
            $textareacur delete $SearchPos "$SearchPos+$len char"
            $textareacur insert $SearchPos $ReplaceString
            colorize $textareacur \
                [$textareacur index "$SearchPos linestart"] \
                [$textareacur index "$SearchPos lineend"]
            $textareacur mark set insert $SearchPos
            $textareacur tag remove foundtext 0.0 end
            $textareacur tag remove replacedtext 0.0 end
            set lenR [string length $ReplaceString]
            $textareacur tag add replacedtext $SearchPos  "$SearchPos + $lenR char"
            # If replacement occurred starting at the first selected character or
            # up to the last selected character, then fakeselection should be extended
            # by hand since tags have no gravity in tcl
            # This is most simply done by always tagging the replaced text as fakeselection
            if {[$textareacur tag ranges fakeselection] != {}} {
                $textareacur tag add fakeselection $SearchPos  "$SearchPos + $lenR char"
            }
            MoveDialogIfFoundHidden
            keyposn $textareacur
            if {$SearchDir == "forwards"} {
                set SearchPos [$textareacur index "$SearchPos+$lenR char"]
                # $SearchEnd must be adjusted for the search to occur in the new selection
                if {$SearchEnd != "No_end" } {
                    if {int([$textareacur index $SearchEnd])==int([$textareacur index $SearchPos]) } {
                        set SearchEnd [$textareacur index "$SearchEnd+[expr $lenR - $len] char"]
                    }
                }
            }         
            setmodified $textareacur
            reshape_bp
            if {$SearchDir == "forwards"} {
                set matchstartpos [$textareacur index "$SearchPos - $lenR char"]
            } else {
                set matchstartpos $SearchPos
            }
            return [list "Done" [expr $lenR - $len] $matchstartpos]
        } else {
            set SearchPos [$textareacur index "insert"]
            if {$once_or_all == "once"} {
                tk_messageBox -message \
                    [concat [mc "The string"] $SearchString [mc "could not be found"] ] \
                    -parent $find -title [mc "Replace"]
              }
            return [list "No_match" 0 0]
        }
    } else {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $find -title [mc "Replace"]
    }
}

proc ReplaceAll {} {
    global SearchPos SearchString find SearchDir

    proc diditwrap {PrecPos NewPos} {
    # Check whether the most recent replace wrapped (from end of text
    # to its beginning, or the opposite)
        global SearchDir
        set textareacur [gettextareacur]
        set didwrap "false"
        if {$SearchDir == "forwards"} {
            if {[$textareacur compare $NewPos < $PrecPos]} {
                set didwrap "true"
            }
        } else {
            if {[$textareacur compare $NewPos > $PrecPos]} {
                set didwrap "true"
            }
        }
        return $didwrap
    }

    set textareacur [gettextareacur]
    if {$SearchString != ""} {
        set RefPos $SearchPos
        set anotherone ""
        set NbOfReplaced 0
        set wrapped "false"
        set wrappedagain "false"
        set onetoomuch "false"
        set NewPos $RefPos
        set firstmatch "true"
        while {$anotherone != "No_match"} {
            set ReplaceItResult [ReplaceIt all]
            set anotherone [lindex $ReplaceItResult 0]
            if {$anotherone != "No_match"} {
                update
                incr NbOfReplaced
                set PrecPos $NewPos
                set NewPos [$textareacur index [lindex $ReplaceItResult 2]]
                # Save area of the replaced text
                if {$firstmatch == "true"} {
                    set reprange [$textareacur tag nextrange replacedtext 1.0]
                    set firstreplstart [lindex $reprange 0]
                    set lastreplstart  [lindex $reprange 1]
                }
                # Reference position must be ajusted if text has been replaced on the
                # same line as and before the initial start position.
                # This reference position is used to detect when all the text has been
                # searched once, meaning that the replace all must end. This test is
                # only used when the text to replace is included in the new replacement
                # text (if not, then the loop ends because $anotherone == "No_match")
                if {int([$textareacur index $RefPos])==int([$textareacur index $NewPos]) && \
                    [$textareacur compare $NewPos < $RefPos] } {
                    set RefPos [$textareacur index "$RefPos+[lindex $ReplaceItResult 1] char"]
                }
                if {$wrapped =="false"} {
                        set wrapped [diditwrap $PrecPos $NewPos]
                } else {
                        set wrappedagain [diditwrap $PrecPos $NewPos]
                }
                # If the search wrapepd and the newly replaced text is after (or before)
                # the reference position, then replace all must end
                if {$SearchDir == "forwards"} {
                    if {$wrapped == "true" && [$textareacur compare $NewPos > $RefPos]} {
                        set onetoomuch "true"
                    }
                } else {
                    if {$wrapped == "true" && [$textareacur compare $NewPos < $RefPos]} {
                        set onetoomuch "true"
                    }
                }
                # If the newly replaced text starts inside the first replaced area, then
                # it means that there is a single match of the searched text in the buffer
                # and replace all must end
                if {[$textareacur compare $firstreplstart <= $NewPos] && \
                    [$textareacur compare $NewPos < $lastreplstart] && \
                    $firstmatch == "false"} {
                    set onetoomuch "true"
                }
                if {$onetoomuch == "true" || $wrappedagain == "true"} {
                    set anotherone "No_match"
                    # remove the wrong superfluous replace (two calls to undo_menu_proc needed)
                    undo_menu_proc
                    undo_menu_proc
                    incr NbOfReplaced -1
                }
                set firstmatch "false"
            }
        }
        showinfo "$NbOfReplaced [mc "replacements done"]"
    } else {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $find -title [mc "Replace"]
    }
}

proc CancelFind {w} {
    global pad
    set textareacur [gettextareacur]
    $textareacur tag remove foundtext 0.0 end
    $textareacur tag remove replacedtext 0.0 end
    if {[$textareacur tag ranges fakeselection] != {}} {
        # there was a selection at the time the find dialog was opened, restore it
        $textareacur tag add sel fakeselection.first fakeselection.last 
        $textareacur tag remove fakeselection 0.0 end
        $textareacur mark set insert sel.first
        $textareacur see insert
    }
    bind $pad <Expose> {}
    destroy $w
}

proc ResetFind {} {
    global SearchPos SearchEnd SearchDir SearchString
    set textareacur [gettextareacur]
    catch {[$textareacur get sel.first sel.last]} sel
    if {$sel == "text doesn't contain any characters tagged with \"sel\""} {
        if {$SearchDir=="forwards" && [$textareacur tag ranges foundtext]!=""} {
            set len [string length $SearchString]
            set SearchPos [$textareacur index "insert + $len char"]
        } else {
            set SearchPos [$textareacur index insert]
        }
        set SearchEnd "No_end"
    } else {
        # there is a selection - fakeselection is the copied sel tag, required
        # because the sel tag is not visible when focus is out of $pad
        $textareacur tag add fakeselection sel.first sel.last
        $textareacur tag raise foundtext fakeselection
        $textareacur tag raise replacedtext fakeselection
        if {$SearchDir=="forwards"} {
            if {[$textareacur tag ranges foundtext]!=""} {
                set len [string length $SearchString]
                set SearchPos [$textareacur index "insert + $len char"]
            } else {
                set SearchPos [$textareacur index sel.first]
            }
            set SearchEnd [$textareacur index sel.last]
        } else {
            if {[$textareacur tag ranges foundtext]!=""} {
                set SearchPos [$textareacur index "insert"]
            } else {
                set SearchPos [$textareacur index sel.last]
            }
            set SearchEnd [$textareacur index sel.first]
        }
    }
}

proc findtext {typ} {
# procedure to find text
    global SearchString SearchDir ReplaceString findcase find pad regexpcase
    if {[IsBufferEditable] == "No" && $typ=="replace"} {return}
    set find $pad.find
    catch {destroy $find}
    toplevel $find
    wm title $find [mc "Find"]
    setwingeom $find
    frame $find.l
    frame $find.l.f1
    label $find.l.f1.label -text [mc "Find what:"] -width 11
    entry $find.l.f1.entry -textvariable SearchString -width 30
    pack $find.l.f1.label $find.l.f1.entry -side left
 # next line commented since it does not sleep with -exportselection 1 on Linux
 #   $find.l.f1.entry selection range 0 end
#        bind $find.l.f1.entry <Control-c> {tk_textCopy $find.l.f1.entry}
#        bind $find.l.f1.entry <Control-v> {tk_textPaste $find.l.f1.entry}
# this doesn't work?
#        bind $find.l.f1.entry <Control-x> {tk_textCut $find.l.f1.entry}
#
    if {$typ=="replace"} {
        frame $find.l.f2
        label $find.l.f2.label2 -text [mc "Replace with:"] -width 11
        entry $find.l.f2.entry2  -textvariable ReplaceString -width 30 
        pack $find.l.f2.label2 $find.l.f2.entry2 -side left
        pack $find.l.f1 $find.l.f2 -side top -pady 2
#            bind $find.l.f2.entry2 <Control-c> {tk_textCopy $find.l.f2.entry2}
#            bind $find.l.f2.entry2 <Control-v> {tk_textPaste $find.l.f2.entry2}
# this doesn't work?
#            bind $find.l.f2.entry2 <Control-x> {tk_textCut $find.l.f2.entry2}
#
    } else {
        pack $find.l.f1 -pady 4
    }
    frame $find.f2
    eval "button $find.f2.button1 [bl "Find &Next"] -command \"FindIt $find\" -height 1 -width 15"
    eval "button $find.f2.button2 [bl "Cance&l"] -command \"CancelFind $find\" -height 1 -width 15"
    if {$typ=="replace"} {
        eval "button $find.f2.button3 [bl "Re&place"] -command \"ReplaceIt once\" -height 1 -width 15"
        eval "button $find.f2.button4 [bl "Replace &All"] -command ReplaceAll -height 1 -width 15"
        pack $find.f2.button1 $find.f2.button3 $find.f2.button4 $find.f2.button2  -pady 4
    } else {
        pack $find.f2.button1 $find.f2.button2  -pady 4
    }
    frame $find.l.f4
    frame $find.l.f4.f3 -borderwidth 2 -relief groove
    eval "radiobutton $find.l.f4.f3.up [bl "&Upwards"] \
        -variable SearchDir -value \"backwards\" -command \"ResetFind\" "
    eval "radiobutton $find.l.f4.f3.down [bl "Downward&s"] \
        -variable SearchDir -value \"forwards\" -command \"ResetFind\" "
    $find.l.f4.f3.down invoke
    pack $find.l.f4.f3.up $find.l.f4.f3.down -side left
    frame $find.l.f4.f5
    eval "checkbutton $find.l.f4.f5.cbox1 [bl "Match &case"] -variable findcase"
    eval "checkbutton $find.l.f4.f5.cbox2 [bl "&Regular expression"] -variable regexpcase"
    pack $find.l.f4.f5.cbox1 $find.l.f4.f5.cbox2 -anchor sw
    pack $find.l.f4.f5 $find.l.f4.f3 -side left -padx 10
    pack $find.l.f4 -pady 11
    pack $find.l $find.f2 -side left -padx 1

    # each widget must be bound to the events of the other widgets
    proc bindevnt {widgetnm types find} {
        bind $widgetnm <Return> "FindIt $find"
        bind $widgetnm <Control-n> "FindIt $find"
        bind $widgetnm <F3> "FindIt $find"
        if {$types=="replace"} {
            bind $widgetnm <Control-p> "ReplaceIt once"
            bind $widgetnm <Control-a> "ReplaceAll"
        }
        bind $widgetnm <Control-c> { $find.l.f4.f5.cbox1 invoke }
        bind $widgetnm <Control-r> { $find.l.f4.f5.cbox2 invoke }
        bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
        bind $widgetnm <Control-s> { $find.l.f4.f3.down invoke }
    }
    if {$typ == "replace"} {
        bindevnt $find.l.f2.entry2 $typ $find
        bindevnt $find.f2.button3 $typ $find
        bindevnt $find.f2.button4 $typ $find
    } else {
        bindevnt $find.f2.button1 $typ $find
        bindevnt $find.f2.button2 $typ $find
    }
    bindevnt $find.l.f4.f3.up  $typ $find
    bindevnt $find.l.f4.f3.down $typ $find
    bindevnt $find.l.f4.f5.cbox1 $typ $find
    bindevnt $find.l.f4.f5.cbox2 $typ $find
    bindevnt $find.l.f1.entry $typ $find	
    bind $find <Escape> "CancelFind $find"
    bind $find <Control-l> "CancelFind $find"
    bind $find <Visibility> {raise $find $pad};
    bind $pad <Expose> {catch {raise $find} }
    focus $find.l.f1.entry
    grab $find
}

proc findnext {typof} {
# proc for find next
    global SearchString find
    if [catch {expr [string compare $SearchString "" ] }] {
        findtext $typof
    } else {
        FindIt $find
    }
}

proc MoveDialogIfFoundHidden {} {
    global pad find
    # offsets in pixels to take into account the window border and title bar sizes
    # <TODO>: values are probably platform-dependent
    set bordsize  3
    set titlsize 29
    set filemenusize [$pad.filemenu yposition 1]
    # coordinates of the find dialog - left, right, top, bottom - screen coordinate system
    set ld [expr [winfo rootx $find] - $bordsize]
    set rd [expr $ld + [winfo width $find] + 2*$bordsize]
    set td [expr [winfo rooty $find] - $titlsize]
    set bd [expr $td + [winfo height $find] + $titlsize + $bordsize]
    # coordinates of the main window - screen coordinate system
    set lt [expr [winfo rootx $pad] - $bordsize]
    set rt [expr $lt + [winfo width $pad] + 2*$bordsize]
    set tt [expr [winfo rooty $pad] - $titlsize - $filemenusize]
    set bt [expr $tt + [winfo height $pad] + $titlsize + $filemenusize + $bordsize]
    # get found text area coordinates relative to the main window coordinate system
    set textareacur [gettextareacur]
    if {[catch {set foundlcoord [$textareacur dlineinfo foundtext.first]} ]} {
        if {[catch {set foundlcoord [$textareacur dlineinfo replacedtext.first]} ]} {
            set foundlcoord [$textareacur dlineinfo insert]
        }
    }
    set lf1 [lindex $foundlcoord 0]
    set rf1 [expr $lf1 + [lindex $foundlcoord 2]]
    set tf1 [lindex $foundlcoord 1]
    set bf1 [expr $tf1 + [lindex $foundlcoord 3]]
    # convert found text coordinates to screen coordinate system
    set lf [expr $lt + $bordsize + $lf1]
    set rf [expr $lt + $bordsize + $rf1]
    set tf [expr $tt + $titlsize + $filemenusize + $bordsize + $tf1]
    set bf [expr $tt + $titlsize + $filemenusize + $bordsize + $bf1]
    # check if the dialog overlaps the text found (intersection of two rectangles)
    if { ! ( ($ld > $rf) || ($lf > $rd) || ($td > $bf) || ($tf > $bd) ) } {
        # the two rectangles intersect, move the dialog
        scan [wm geometry $find] "%dx%d+%d+%d" cw ch cx cy
        set newx [expr $cx - ($rd - $ld)]
        if {$newx < 1} {
            set newx [expr [winfo screenwidth $find] - ($rd - $ld)]
        }
        set newy [expr $cy - ($bd - $td)]
        if {$newy < 1} {
            set newy [expr [winfo screenheight $find] - ($bd - $td)]
        }
        wm geometry $find "+$newx+$newy"
    }
}
