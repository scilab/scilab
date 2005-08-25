proc FindIt {w} {
    global SearchString SearchPos SearchDir findcase regexpcase
    global pad SearchEnd

    set textareacur [gettextareacur]
    if {[winfo exists $w]} {
        set pw $w
    } else {
        set pw $pad
    }
    if {$SearchString == ""} {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $pw -title [mc "Find"]
        return
    }

    # save the initial searching position so that wraparound can be later detected
    set OldPos $SearchPos
    delinfo

    # set case option for $textareacur search
    if {$findcase=="1"} {
        set caset "-exact"
    } else {
        set caset "-nocase"
    }

    # do the search
    if {$regexpcase != "1"} {
        # standard case, no regexp

        if {$SearchEnd == "No_end"} {
            # search in whole buffer
            set SearchPos [ $textareacur search $caset -$SearchDir \
                            -- $SearchString $SearchPos]
        } else {
            # search in a selection
            set SearchPos [ $textareacur search $caset -$SearchDir \
                            -- $SearchString $SearchPos $SearchEnd]
        }
        # compute the length of the matched string
        set len [string length $SearchString]

    } else {
        # regexp case

        if {$SearchEnd == "No_end"} {
            # search in whole buffer
            set SearchPos [ $textareacur search $caset -$SearchDir \
                            -regexp -- $SearchString $SearchPos]
        } else {
            # search in a selection
            set SearchPos [ $textareacur search $caset -$SearchDir \
                            -regexp -- $SearchString $SearchPos $SearchEnd]
        }
        # compute the length of the matched string
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

    # analyze the search results and prepare for next search
    if {$SearchPos != ""} {
        # a match has been found

        # if search wrapped, tell the user
        if {[diditwrap $OldPos $SearchPos] == "true"} {
            showinfo [mc "Wrapped around"]
        }

        # arrange for this match to be visible and tag it
        $textareacur see $SearchPos
        $textareacur mark set insert $SearchPos
        $textareacur tag remove foundtext 0.0 end
        $textareacur tag remove replacedtext 0.0 end
        $textareacur tag add foundtext $SearchPos  "$SearchPos + $len char"
        # <TODO>: these bindings are required to remove the foundtext tag after a find
        #         next triggered by F3. Once set, they will live in the textarea forever!
        bind $textareacur <KeyPress>    {+$textareacur tag remove foundtext 0.0 end}
        bind $textareacur <ButtonPress> {+$textareacur tag remove foundtext 0.0 end}

        # prevent the dialog box from hiding the matched string
        if {[winfo exists $w]} {
            MoveDialogIfFoundHidden $w
        }

        # update status bar data
        keyposn $textareacur

        # set the start position for the next search
        # backwards case: nothing to do since .text search returns the position of
        # the first matching character, therefore $SearchPos is already well set
        if {$SearchDir == "forwards"} {
            set SearchPos [$textareacur index "$SearchPos + $len char"]
        }

        # <TODO> this is for search in multiple files - make it work!
        return "matchfound"
                 
    } else {
        # no match has been found

        # inform the user that the search has failed
        if {$SearchEnd == "No_end"} {

            # no match in whole buffer
            tk_messageBox -message \
                [concat [mc "The string"] $SearchString [mc "could not be found"] ] \
                -parent $pw -title [mc "Find"]
            set SearchPos [$textareacur index "insert"]

            # <TODO> this is for search in multiple files - make it work!
            return "nomatchfound"

        } else {

            # no match in selection, ask for extending search
            set answer [tk_messageBox -message \
                [concat [mc "No match found in the selection for"] $SearchString [mc "\nWould you like to look for it in the entire text?"] ] \
                -parent $pw -title [mc "Find"] -type yesno -icon question]

            # set the start position for the next search
            if {![string compare $answer "yes"]} {

                # extend search
                if {$SearchDir == "forwards"} {
                    set SearchPos [$textareacur index "insert + $len char"]
                } else {
                    set SearchPos [$textareacur index "insert"]
                }
                set SearchEnd "No_end"
                $textareacur tag remove sel 0.0 end

                # <TODO> this is for search in multiple files - make it work!
                return "matchfound" ; # because we should do a find in the same buffer before switching

            } else {

                # don't extend search
                if {$SearchDir=="forwards"} {
                    set SearchPos [$textareacur index sel.first]
                } else {
                    set SearchPos [$textareacur index sel.last]
                }

                # <TODO> this is for search in multiple files - make it work!
                return "nomatchfound"

            }

        }

    }
}

proc ReplaceIt {once_or_all} {
    global SearchString SearchDir ReplaceString SearchPos findcase regexpcase
    global find SearchEnd

    set textareacur [gettextareacur]

    if {$SearchString == ""} {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $find -title [mc "Replace"]
        return
    }

    # save the initial searching position so that wraparound can be later detected
    set OldPos $SearchPos
    delinfo

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

    # compute the length of the matched string
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

    # analyze the search results, replace the match and prepare for next search
    if {$SearchPos != ""} {
        # a match has been found

        # if search wrapped, tell the user
        if {[diditwrap $OldPos $SearchPos] == "true"} {
            showinfo [mc "Wrapped around"]
        }

        # arrange for this match to be visible, replace it, colorize and tag it
        $textareacur see $SearchPos
        set oldSeparator [$textareacur cget -autoseparators]
        if {$oldSeparator} {
            $textareacur configure -autoseparators 0
            $textareacur edit separator
        }
        $textareacur delete $SearchPos "$SearchPos+$len char"
        $textareacur insert $SearchPos $ReplaceString
        if {$oldSeparator} {
            $textareacur edit separator
            $textareacur configure -autoseparators 1
        }
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

        # prevent the dialog box from hiding the matched string
        MoveDialogIfFoundHidden $find

        # update status bar data
        keyposn $textareacur

        # set the start position for the next search
        # backwards case: nothing to do since .text search returns the position of
        # the first matching character, therefore $SearchPos is already well set
        if {$SearchDir == "forwards"} {
            set SearchPos [$textareacur index "$SearchPos+$lenR char"]
            # $SearchEnd must be adjusted for the search to occur in the new selection
            if {$SearchEnd != "No_end" } {
                if {int([$textareacur index $SearchEnd])==int([$textareacur index $SearchPos]) } {
                    set SearchEnd [$textareacur index "$SearchEnd+[expr $lenR - $len] char"]
                }
            }
        }

        reshape_bp

        # save the matched start position
        if {$SearchDir == "forwards"} {
            set matchstartpos [$textareacur index "$SearchPos - $lenR char"]
        } else {
            set matchstartpos $SearchPos
        }

        return [list "Done" [expr $lenR - $len] $matchstartpos]

    } else {
        # no match has been found

        # inform the user that the search has failed
        set SearchPos [$textareacur index "insert"]
        if {$once_or_all == "once"} {
            tk_messageBox -message \
                [concat [mc "The string"] $SearchString [mc "could not be found"] ] \
                -parent $find -title [mc "Replace"]
          }

        return [list "No_match" 0 0]

    }
}

proc ReplaceAll {} {
    global SearchPos SearchString SearchDir

    set textareacur [gettextareacur]

    set RefPos $SearchPos
    set anotherone ""
    set NbOfReplaced 0
    set wrapped "false"
    set wrappedagain "false"
    set onetoomuch "false"
    set NewPos $RefPos
    set firstmatch "true"

    # loop on matches
    while {$anotherone != "No_match"} {

        # perform a replace
        set ReplaceItResult [ReplaceIt all]
        set anotherone [lindex $ReplaceItResult 0]

        # if there was a match
        if {$anotherone != "No_match"} {

            # prepare for next replace iteration
            update
            incr NbOfReplaced
            set PrecPos $NewPos
            set NewPos [$textareacur index [lindex $ReplaceItResult 2]]

            # Save area of the replaced text
            if {$firstmatch == "true"} {
                set reprange [$textareacur tag nextrange replacedtext 1.0]
                if {$reprange == ""} {
                    # The found text was replaced by an empty string
                    set reprange [list [$textareacur index insert] [$textareacur index insert]]
                }
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

            # detect simple and double wraparound
            if {$wrapped =="false"} {
                    set wrapped [diditwrap $PrecPos $NewPos]
            } else {
                    set wrappedagain [diditwrap $PrecPos $NewPos]
            }

            # If the search wrapped and the newly replaced text is after (or before)
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

            # loop end test
            if {$onetoomuch == "true" || $wrappedagain == "true"} {
                set anotherone "No_match"
                # remove one wrong superfluous replace
                undo $textareacur
                incr NbOfReplaced -1
            }

            set firstmatch "false"

        } else {
            # no match found, nothing to do, the while loop will stop
            # since $anotherone == "No_match"
        }
    }

    showinfo "$NbOfReplaced [mc "replacements done"]"
}

proc CancelFind {w} {
    global pad listoftextarea

    foreach textarea $listoftextarea {
        $textarea tag remove foundtext 0.0 end
        $textarea tag remove replacedtext 0.0 end

        if {[$textarea tag ranges fakeselection] != {}} {
            # there was a selection at the time the find dialog was opened, restore it
            $textarea tag add sel fakeselection.first fakeselection.last 
            $textarea tag remove fakeselection 0.0 end
            $textarea mark set insert sel.first
            $textarea see insert
        }
    }

    bind $pad <Expose> {}
    destroy $w
}

proc ResetFind {w} {
# set the start ($SearchPos) and stop ($SearchEnd) positions
# according to the different search modes available
    global SearchPos SearchEnd SearchDir SearchString
    global listoftextarea findresult findrefpos

    set textareacur [gettextareacur]

    # check whether there is a pre-existing selection
    catch {[$textareacur get sel.first sel.last]} sel
    if {$sel == "text doesn't contain any characters tagged with \"sel\""} {
        # there is no selection

        if {$SearchDir=="forwards" && [$textareacur tag ranges foundtext]!=""} {
            set len [string length $SearchString]
            set SearchPos [$textareacur index "insert + $len char"]
        } else {
            set SearchPos [$textareacur index insert]
        }

        set SearchEnd "No_end"

    } else {
        # there is a selection

        # fakeselection is the copied sel tag, required
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

    # initial settings for searching in multiple files
    if {[llength $listoftextarea] == 1} {
        $w.l.f4.f5.cbox3 configure -state disabled
    }
    # <TODO> this is for search in multiple files - make it work!
    set findresult "matchfound"
    catch {unset findrefpos}
}

proc findtext {typ} {
# display the find or replace dialog box
    global textFont menuFont
    global SearchString SearchDir ReplaceString findcase find pad regexpcase
    global multiplefiles

    if {[IsBufferEditable] == "No" && $typ=="replace"} {return}

    set find $pad.find
    catch {destroy $find}
    toplevel $find
    wm title $find [mc "Find"]
    setwingeom $find

    # entry fields
    frame $find.l
    frame $find.l.f1
    label $find.l.f1.label -text [mc "Find what:"] -width 15 -font $menuFont
    entry $find.l.f1.entry -textvariable SearchString -width 30 -font $textFont
    pack $find.l.f1.label $find.l.f1.entry -side left
    pack configure $find.l.f1.entry -expand 1 -fill x
 # next line commented since it does not sleep with -exportselection 1 on Linux
 #   $find.l.f1.entry selection range 0 end
#        bind $find.l.f1.entry <Control-c> {tk_textCopy $find.l.f1.entry}
#        bind $find.l.f1.entry <Control-v> {tk_textPaste $find.l.f1.entry}
# this doesn't work?
#        bind $find.l.f1.entry <Control-x> {tk_textCut $find.l.f1.entry}
#

    if {$typ=="replace"} {
        frame $find.l.f2
        label $find.l.f2.label2 -text [mc "Replace with:"] -width 15 -font $menuFont
        entry $find.l.f2.entry2 -textvariable ReplaceString -width 30 -font $textFont
        pack $find.l.f2.label2 $find.l.f2.entry2 -side left
        pack configure $find.l.f2.entry2 -expand 1 -fill x
        pack $find.l.f1 $find.l.f2 -side top -pady 2 -padx 8 -expand 1 -fill x
#            bind $find.l.f2.entry2 <Control-c> {tk_textCopy $find.l.f2.entry2}
#            bind $find.l.f2.entry2 <Control-v> {tk_textPaste $find.l.f2.entry2}
# this doesn't work?
#            bind $find.l.f2.entry2 <Control-x> {tk_textCut $find.l.f2.entry2}
#
    } else {
        pack $find.l.f1 -pady 4 -padx 8 -expand 1 -fill x
    }

    # buttons
    frame $find.f2
    eval "button $find.f2.button1 [bl "Find &Next"] -command \"FindIt $find\" -height 1 -width 15 -font $menuFont"
# <TODO> 1 line temporary here since only replace all works with multiple files
#    eval "button $find.f2.button1 [bl "Find &Next"] -command \"multiplefilesfind $find\" -height 1 -width 15 -font $menuFont"
    eval "button $find.f2.button2 [bl "Cance&l"] -command \"CancelFind $find\" -height 1 -width 15 -font $menuFont"
    if {$typ=="replace"} {
        eval "button $find.f2.button3 [bl "Re&place"] -command \"ReplaceIt once\" -height 1 -width 15 -font $menuFont"
# <TODO> 5 lines temporary here since only replace all works with multiple files
global listoftextarea
bind $find.f2.button1 <Enter> {if {[llength $listoftextarea] != 1} {$find.l.f4.f5.cbox3 configure -state disabled}}
bind $find.f2.button1 <Leave> {if {[llength $listoftextarea] != 1} {$find.l.f4.f5.cbox3 configure -state normal}}
bind $find.f2.button3 <Enter> {if {[llength $listoftextarea] != 1} {$find.l.f4.f5.cbox3 configure -state disabled}}
bind $find.f2.button3 <Leave> {if {[llength $listoftextarea] != 1} {$find.l.f4.f5.cbox3 configure -state normal}}
        eval "button $find.f2.button4 [bl "Replace &All"] -command \"multiplefilesreplaceall $find\" -height 1 -width 15 -font $menuFont"
        pack $find.f2.button1 $find.f2.button3 $find.f2.button4 $find.f2.button2  -pady 4
    } else {
        pack $find.f2.button1 $find.f2.button2  -pady 4
    }

    # up/down radiobutton
    frame $find.l.f4
    frame $find.l.f4.f3 -borderwidth 2 -relief groove
    eval "radiobutton $find.l.f4.f3.up [bl "&Upwards"] \
        -variable SearchDir -value \"backwards\" -command \"ResetFind $find\" -font $menuFont "
    eval "radiobutton $find.l.f4.f3.down [bl "Downward&s"] \
        -variable SearchDir -value \"forwards\" -command \"ResetFind $find\" -font $menuFont "
    pack $find.l.f4.f3.up $find.l.f4.f3.down -anchor w

    # case and regexp checkboxes
    frame $find.l.f4.f5
    eval "checkbutton $find.l.f4.f5.cbox1 [bl "Match &case"] -variable findcase -font $menuFont "
    eval "checkbutton $find.l.f4.f5.cbox2 [bl "&Regular expression"] -variable regexpcase -font $menuFont "
    eval "checkbutton $find.l.f4.f5.cbox3 [bl "In all &opened files"] -variable multiplefiles -font $menuFont "
    pack $find.l.f4.f5.cbox1 $find.l.f4.f5.cbox2 $find.l.f4.f5.cbox3 -anchor sw

    pack $find.l.f4.f5 $find.l.f4.f3 -side left -padx 10
    pack $find.l.f4 -pady 11
    pack $find.l $find.f2 -side left -padx 1

    # each widget must be bound to the events of the other widgets
        proc bindevnt {widgetnm types find} {
            bind $widgetnm <Return> "FindIt $find"
            bind $widgetnm <Alt-[fb $find.f2.button1]> "FindIt $find"
            bind $widgetnm <F3> "FindIt $find"
            if {$types=="replace"} {
                bind $widgetnm <Alt-[fb $find.f2.button3]> "ReplaceIt once"
                bind $widgetnm <Alt-[fb $find.f2.button4]> "ReplaceAll"
            }
            bind $widgetnm <Alt-[fb $find.l.f4.f5.cbox1]> { $find.l.f4.f5.cbox1 invoke }
            bind $widgetnm <Alt-[fb $find.l.f4.f5.cbox2]> { $find.l.f4.f5.cbox2 invoke }
            bind $widgetnm <Alt-[fb $find.l.f4.f3.up]> { $find.l.f4.f3.up invoke }
            bind $widgetnm <Alt-[fb $find.l.f4.f3.down]> { $find.l.f4.f3.down invoke }
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
    # after 0 in the following Alt binding is mandatory for Linux only
    # This is Tk bug 1236306
    bind $find <Alt-[fb $find.f2.button2]> "after 0 CancelFind $find"
    bind $find <Visibility> {raise $find $pad};
    bind $pad <Expose> {catch {raise $find} }

    $find.l.f1.entry selection range 0 end
    focus $find.l.f1.entry
    grab $find

    # call ResetFind to initialize all the settings
    $find.l.f4.f3.down invoke
# <TODO> 3 lines temporary here since only replace all works with multiple files
if {$typ!="replace"} {
$find.l.f4.f5.cbox3 configure -state disabled
}
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

proc multiplefilesreplaceall {w} {
# replace all in current buffer, or in all the opened buffers
    global SearchString listoftextarea multiplefiles

    if {$SearchString == ""} {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $w -title [mc "Replace"]
        return
    }

    if {$multiplefiles == 1} {
        # replace all in all the opened buffers
        set tacur [gettextareacur]
        foreach ta $listoftextarea {
            montretext $ta
            ReplaceAll
        }
        montretext $tacur
    } else {
        # replace all in the current buffer only
        ReplaceAll
    }
}

proc multiplefilesfind {w} {
# search in all the opened buffers
# <TODO> this does not work!
    global listoftextarea findresult findrefpos SearchPos
    # save initial search start position in current buffer
    if {![info exists findrefpos]} {
        set findrefpos $SearchPos
    }
    if {$findresult == "nomatchfound" || [diditwrap $findrefpos $SearchPos]} {
        nextbuffer
        unset findrefpos
    }
    set findresult [FindIt $w]
}

proc diditwrap {PrecPos NewPos} {
# Check whether the most recent replace or find wrapped (from end of text
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

proc MoveDialogIfFoundHidden {w} {
    global pad
    # offsets in pixels to take into account the window border and title bar sizes
    # <TODO>: values are probably platform-dependent
    set bordsize  3
    set titlsize 29
    set filemenusize [$pad.filemenu yposition 1]
    # coordinates of the dialog - left, right, top, bottom - screen coordinate system
    set ld [expr [winfo rootx $w] - $bordsize]
    set rd [expr $ld + [winfo width $w] + 2*$bordsize]
    set td [expr [winfo rooty $w] - $titlsize]
    set bd [expr $td + [winfo height $w] + $titlsize + $bordsize]
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
        scan [wm geometry $w] "%dx%d+%d+%d" cw ch cx cy
        set newx [expr $cx - ($rd - $ld)]
        if {$newx < 1} {
            set newx [expr [winfo screenwidth $w] - ($rd - $ld)]
        }
        set newy [expr $cy - ($bd - $td)]
        if {$newy < 1} {
            set newy [expr [winfo screenheight $w] - ($bd - $td)]
        }
        wm geometry $w "+$newx+$newy"
    }
}
