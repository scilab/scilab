proc FindIt {w} {
    global SearchString SearchPos SearchDir findcase regexpcase
    global textareacur pad SearchEnd SearchPosI
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
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos $SearchEnd]
            }
        } else {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos $SearchEnd]
            }
        }
        set len [string length $SearchString]
        if {$SearchPos != ""} {
            [gettextareacur] see $SearchPos
            [gettextareacur] mark set insert $SearchPos
            [gettextareacur] tag remove sel 0.0 end
            [gettextareacur] tag add sel $SearchPos  "$SearchPos + $len char"
            if {$SearchDir == "forwards"} {
                set SearchPos "$SearchPos + $len char"
            }         
        } else {
            if {$SearchEnd == "No_end"} {
                tk_messageBox -message \
                    [concat [mc "The string"] $SearchString [mc "could not be found"] ] \
                    -parent $pw -title [mc "Find"]
                set SearchPos insert
            } else {
                set answer [tk_messageBox -message \
                    [concat [mc "No match found in the selection for"] $SearchString [mc "\nWould you like to look for it in the entire text?"] ] \
                    -parent $pw -title [mc "Find"] -type yesno -icon question]
                if {![string compare $answer "yes"]} {
                    if {$SearchDir == "forwards"} {
                        set SearchPos "insert + $len char"
                    } else {
                        set SearchPos insert
                    }
                    set SearchEnd "No_end"
                } else {
                    set SearchPos $SearchPosI
                }
            }
        }
    } else {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $pw -title [mc "Find"]
    }
    focus [gettextareacur]
}

proc ReplaceIt {once_or_all} {
    global SearchString SearchDir ReplaceString SearchPos findcase regexpcase
    global textareacur find SearchEnd
    if {$SearchString != ""} {
        if {$findcase=="1"} {
            set caset "-exact"
        } else {
            set caset "-nocase"
        }
        if {$regexpcase != "1"} {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos $SearchEnd]
            }
        } else {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos $SearchEnd]
            }
        }
        set len [string length $SearchString]
        if {$SearchPos != ""} {
            [gettextareacur] see $SearchPos
            [gettextareacur] delete $SearchPos "$SearchPos+$len char"
            [gettextareacur] insert $SearchPos $ReplaceString
            colorize [gettextareacur] \
              [[gettextareacur] index "$SearchPos linestart"] \
              [[gettextareacur] index "$SearchPos lineend"]
            [gettextareacur] mark set insert $SearchPos
            [gettextareacur] tag remove sel 0.0 end
            set lenR [string length $ReplaceString]
            [gettextareacur] tag add sel $SearchPos  "$SearchPos + $lenR char"
            if {$SearchDir == "forwards"} {
                set SearchPos "$SearchPos+$lenR char"
                # $SearchEnd must be adjusted for the search to occur in the new selection
                if {$SearchEnd != "No_end" } {
                    if {int([[gettextareacur] index $SearchEnd])==int([[gettextareacur] index $SearchPos]) } {
                        set SearchEnd "$SearchEnd+[expr $lenR - $len] char"
                    }
                }
            }         
            inccount [gettextareacur]
            reshape_bp
            focus [gettextareacur]
            return [list "Done" $SearchPos]
        } else {
            set SearchPos insert
            if {$once_or_all == "once"} {
                tk_messageBox -message \
                    [concat [mc "The string"] $SearchString [mc "could not be found"] ] \
                    -parent $find -title [mc "Replace"]
              }
            return [list "No_match" $SearchPos]
        }
    } else {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $find -title [mc "Replace"]
    }
}

proc ReplaceAll {} {
    global SearchPos SearchString find
    if {$SearchString != ""} {
    # The following has been a little bit reworked to account for the possibility that the user,
    # without checking the Match case box, tries to replace a string S1 with a string S2 that is
    # only different from S1 by the character case 
        set ReplaceItResult [ReplaceIt once]
        set anotherone [lindex $ReplaceItResult 0]
        set RefPos [lindex $ReplaceItResult 1]
        while {$anotherone != "No_match"} {
            set ReplaceItResult [ReplaceIt all]
            set anotherone [lindex $ReplaceItResult 0]
            set NewPos [lindex $ReplaceItResult 1]
            if {$NewPos == $RefPos} {
                set anotherone "No_match"
            }
        }
    } else {
        tk_messageBox -message [mc "You are searching for an empty string!"] \
                      -parent $find -title [mc "Replace"]
    }
}

proc CancelFind {w} {
    global textareacur pad
    [gettextareacur] tag delete tg1
    bind $pad <Expose> {};
    destroy $w
}

proc ResetFind {} {
    global SearchPos SearchEnd SearchPosI SearchDir
    catch {[[gettextareacur] get sel.first sel.last]} sel
    if {$sel == "text doesn't contain any characters tagged with \"sel\""} {
        set SearchPos insert
        set SearchEnd "No_end"
    } else {
        if {$SearchDir=="forwards"} {
            set SearchPos [[gettextareacur] index sel.first]
            set SearchEnd [[gettextareacur] index sel.last]
        } else {
            set SearchPos [[gettextareacur] index sel.last]
            set SearchEnd [[gettextareacur] index sel.first]
        }
        set SearchPosI $SearchPos
    }
}

proc findtext {typ} {
# procedure to find text
    global SearchString SearchDir ReplaceString findcase c find pad regexpcase lang
    if {[IsBufferEditable] == "No" && $typ=="replace"} {return}
    set find $pad.find
    catch {destroy $find}
    toplevel $find
    wm title $find [mc "Find"]
    setwingeom $find
    frame $find.l
    frame $find.l.f1
    label $find.l.f1.label -text [mc "Find what:"] -width 11
    entry $find.l.f1.entry  -textvariable SearchString -width 30 
    pack $find.l.f1.label $find.l.f1.entry -side left
    $find.l.f1.entry selection range 0 end
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
        pack $find.f2.button3 $find.f2.button4 $find.f2.button2  -pady 4
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
    bind $find <Escape> "CancelFind $find"

     # each widget must be bound to the events of the other widgets
    proc bindevnt {widgetnm types find} {
        if {$types=="replace"} {
            bind $widgetnm <Return> "ReplaceIt once"
            bind $widgetnm <Control-p> "ReplaceIt once"
            bind $widgetnm <Control-a> "ReplaceAll"
        } else {
            bind $widgetnm <Return> "FindIt $find"
            bind $widgetnm <Control-n> "FindIt $find"
        }
        bind $widgetnm <Control-c> { $find.l.f4.f5.cbox1 invoke }
        bind $widgetnm <Control-r> { $find.l.f4.f5.cbox2 invoke }
        bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
        bind $widgetnm <Control-s> { $find.l.f4.f3.down invoke }
    }
    if {$typ == "replace"} {
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
    bind $find <Control-l> "CancelFind $find"
    bind $find <Visibility> {raise $find $pad};
    bind $pad <Expose> {catch {raise $find} }
    focus $find.l.f1.entry
    grab $find
}

proc findnext {typof} {
# proc for find next
    global SearchString SearchDir ReplaceString findcase c find
    if [catch {expr [string compare $SearchString "" ] }] {
        findtext $typof
    } else {
        FindIt $find
    }
}
