proc FindIt {w} {
# FV 13/05/04, regexp mode added
    global SearchString SearchPos SearchDir findcase regexpcase
    global textareacur pad
    global lang SearchEnd SearchPosI
#    [gettextareacur] tag configure sel -background green
# Francois VOGEL, 21/04/04
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
# Francois VOGEL, 21/04/04, removed $limit to allow for wrap-around (both directions)
# and find in selected text only
#	if {$SearchDir == "forwards"} {
#	    set limit end
#	} else {
#	    set limit 1.0
#	}
#	set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
#			    -- $SearchString $SearchPos $limit]
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
#	    tkTextSetCursor [gettextareacur] $SearchPos
            [gettextareacur] mark set insert $SearchPos
            [gettextareacur] tag remove sel 0.0 end
	      [gettextareacur] tag add sel $SearchPos  "$SearchPos + $len char"
	      if {$SearchDir == "forwards"} {
                set SearchPos "$SearchPos + $len char"
            }         
	} else {
# Francois VOGEL, 21/04/04, added message box
#	    set SearchPos "0.0"
            if {$SearchEnd == "No_end"} {
                if {$lang == "eng"} {
                    tk_messageBox -message "No match found for $SearchString" -parent $pw -title "Find"
                } else {
                    tk_messageBox -message "La chaîne $SearchString n'a pu être trouvée" -parent $pw -title "Rechercher"
                }
                set SearchPos insert
            } else {
                if {$lang == "eng"} {
                    set answer [tk_messageBox -message "No match found in the selection for $SearchString\nWould you like to look for it in the entire text?" \
                                  -parent $pw -title "Find" -type yesno -icon question]
                } else {
                    set answer [tk_messageBox -message "La chaîne $SearchString n'a pu être trouvée dans la sélection.\nVoulez-vous rechercher dans la totalité du texte ?" \
                                  -parent $pw -title "Rechercher" -type yesno -icon question]
                }
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
# Francois VOGEL, 21/04/04
        if {$lang == "eng"} {
            tk_messageBox -message "You are searching for an empty string!" -parent $pw -title "Find"
        } else {
            tk_messageBox -message "La chaîne à rechercher est vide!" -parent $pw -title "Rechercher"
        }
    }
    focus [gettextareacur]
}

proc ReplaceIt {once_or_all} {
# FV 13/05/04, regexp mode added
    global SearchString SearchDir ReplaceString SearchPos findcase regexpcase
    global textareacur
    global find lang SearchEnd
# Francois VOGEL, 21/04/04
    if {$SearchString != ""} {
        if {$findcase=="1"} {
            set caset "-exact"
        } else {
            set caset "-nocase"
        }
# Francois VOGEL, 21/04/04, removed $limit to allow for wrap-around (both directions)
# and replace in selected text only
#	if {$SearchDir == "forwards"} {
#	    set limit end
#	} else {
#	    set limit 1.0
#	}
#	set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
#			    -- $SearchString $SearchPos $limit]
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
# Francois VOGEL, 21/04/04, $SearchEnd must be adjusted for the search to occur in the new selection
                if {$SearchEnd != "No_end" } {
                    if {int([[gettextareacur] index $SearchEnd])==int([[gettextareacur] index $SearchPos]) } {
                        set SearchEnd "$SearchEnd+[expr $lenR - $len] char"
                    }
                }
            }         
            inccount [gettextareacur]
# FV 13/05/04
            reshape_bp
            focus [gettextareacur]
            return "Done"
        } else {
# Francois VOGEL, 21/04/04, added message box
#	    set SearchPos "0.0"
            set SearchPos insert
            if {$once_or_all == "once"} {
                if {$lang == "eng"} {
                    tk_messageBox -message "No match found for $SearchString" -parent $find -title "Replace"
                } else {
                    tk_messageBox -message "La chaîne $SearchString n'a pu être trouvée" -parent $find -title "Remplacer"
                }
            }
            return "No_match"
        }
    } else {
# Francois VOGEL, 21/04/04
        if {$lang == "eng"} {
            tk_messageBox -message "You are searching for an empty string!" -parent $find -title "Find"
        } else {
            tk_messageBox -message "La chaîne à rechercher est vide!" -parent $find -title "Rechercher"
        }
    }
#    inccount [gettextareacur]
}

proc ReplaceAll {} {
      global SearchPos SearchString lang find
# Francois VOGEL, 21/04/04
#      if {$SearchString != ""} {
#          ReplaceIt
#          while {$SearchPos!="0.0"} {
#              ReplaceIt
#          }
#      }
    if {$SearchString != ""} {
        set anotherone [ReplaceIt once]
        while {$anotherone != "No_match"} {
            set anotherone [ReplaceIt all]
        }
    } else {
        if {$lang == "eng"} {
            tk_messageBox -message "You are searching for an empty string!" -parent $find -title "Find"
        } else {
            tk_messageBox -message "La chaîne à rechercher est vide!" -parent $find -title "Rechercher"
        }
    }
}

proc CancelFind {w} {
    global textareacur pad
    [gettextareacur] tag delete tg1
    bind $pad <Expose> {};
#ajout pour mettre a la fenetre Search devant le scipad !
    destroy $w
}

proc ResetFind {} {
#    global SearchPos
#    set SearchPos insert
# Francois VOGEL, 21/04/04, added find in current selection
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

# procedure to find text
proc findtext {typ} {
# FV 13/05/04, regexp mode added
    global SearchString SearchDir ReplaceString findcase c find pad lang regexpcase
    set find $pad.find
    catch {destroy $find}
    toplevel $find
    if {$lang == "eng"} {
        wm title $find "Find"
    } else {
        wm title $find "Rechercher"
    }
    setwingeom $find
# Francois VOGEL, 21/04/04, this is already done by invoking down radiobutton below
# as I added -command on this radiobutton to take care of the case where find shall
# look backwards in selection
#    ResetFind
    frame $find.l
    frame $find.l.f1
    if {$lang == "eng"} {
        label $find.l.f1.label -text "Find what:" -width 11
    } else {
        label $find.l.f1.label -text "Rechercher :" -width 11
    }
    entry $find.l.f1.entry  -textvariable SearchString -width 30 
    pack $find.l.f1.label $find.l.f1.entry -side left
    $find.l.f1.entry selection range 0 end
#ES 
#        bind $find.l.f1.entry <Control-c> {tk_textCopy $find.l.f1.entry}
#        bind $find.l.f1.entry <Control-v> {tk_textPaste $find.l.f1.entry}
# this doesn't work?
#        bind $find.l.f1.entry <Control-x> {tk_textCut $find.l.f1.entry}
#
    if {$typ=="replace"} {
        frame $find.l.f2
        if {$lang == "eng"} {
            label $find.l.f2.label2 -text "Replace with:" -width 11
        } else {
            label $find.l.f2.label2 -text "Remplacer par :" -width 11
        }
        entry $find.l.f2.entry2  -textvariable ReplaceString -width 30 
        pack $find.l.f2.label2 $find.l.f2.entry2 -side left
        pack $find.l.f1 $find.l.f2 -side top -pady 2
#ES 
#            bind $find.l.f2.entry2 <Control-c> {tk_textCopy $find.l.f2.entry2}
#            bind $find.l.f2.entry2 <Control-v> {tk_textPaste $find.l.f2.entry2}
# this doesn't work?
#            bind $find.l.f2.entry2 <Control-x> {tk_textCut $find.l.f2.entry2}
#
    } else {
        pack $find.l.f1 -pady 4
    }
    frame $find.f2
    if {$lang == "eng"} {
        button $find.f2.button1 -text "Find Next" -command "FindIt $find" \
            -width 10 -height 1 -underline 5 
        button $find.f2.button2 -text "Cancel" -command "CancelFind $find"\
            -width 10 -underline 5
    } else {
        button $find.f2.button1 -text "Rechercher suivant" -command \
            "FindIt $find" -width 15 -height 1 -underline 16 
        button $find.f2.button2 -text "Annuler" \
            -command "CancelFind $find" -width 15 -underline 4	    
    }
    if {$typ=="replace"} {
        if {$lang == "eng"} {
            button $find.f2.button3 -text "Replace" -command "ReplaceIt once"\
                -width 10 -height 1 -underline 2
            button $find.f2.button4 -text "Replace All" \
                -command ReplaceAll -width 10 -height 1 -underline 8
        } else {
            button $find.f2.button3 -text "Remplacer" -command "ReplaceIt once" \
                -width 15 -height 1 -underline 3
            button $find.f2.button4 -text "Remplacer tout" \
                -command ReplaceAll -width 15 -height 1 -underline 10
        }
        pack $find.f2.button3 $find.f2.button4 $find.f2.button2  -pady 4
    } else {
        pack $find.f2.button1 $find.f2.button2  -pady 4
    }
    frame $find.l.f4
    frame $find.l.f4.f3 -borderwidth 2 -relief groove
# Francois VOGEL 21/04/04, added -command on these two radiobuttons
    if {$lang == "eng"} {
        radiobutton $find.l.f4.f3.up -text "Up" -underline 0 \
            -variable SearchDir -value "backwards" -command "ResetFind"
        radiobutton $find.l.f4.f3.down -text "Down"  -underline 0 \
            -variable SearchDir -value "forwards" -command "ResetFind"
    } else {
        radiobutton $find.l.f4.f3.up -text "Vers le haut" -underline 10 \
            -variable SearchDir -value "backwards" -command "ResetFind"
        radiobutton $find.l.f4.f3.down -text "Vers le bas"  -underline 9 \
            -variable SearchDir -value "forwards" -command "ResetFind"
    } 
    $find.l.f4.f3.down invoke
    pack $find.l.f4.f3.up $find.l.f4.f3.down -side left
    frame $find.l.f4.f5
    if {$lang == "eng"} {
        checkbutton $find.l.f4.f5.cbox1 -text "Match case" \
            -variable findcase -underline 0
    } else {
        checkbutton $find.l.f4.f5.cbox1 -text "Respecter la casse" \
            -variable findcase -underline 0
    }
    if {$lang == "eng"} {
        checkbutton $find.l.f4.f5.cbox2 -text "Regular expression" \
            -variable regexpcase -underline 0
    } else {
        checkbutton $find.l.f4.f5.cbox2 -text "Expression régulière" \
            -variable regexpcase -underline 13
    }
    pack $find.l.f4.f5.cbox1 $find.l.f4.f5.cbox2 -anchor sw
    pack $find.l.f4.f5 $find.l.f4.f3 -side left -padx 10
    pack $find.l.f4 -pady 11
    pack $find.l $find.f2 -side left -padx 1
#	bind $find <Escape> "destroy $find" ##this hangs scipad
    bind $find <Escape> "CancelFind $find"

     # each widget must be bound to the events of the other widgets
    proc bindevnt {widgetnm types find} {
        global lang
        if {$lang == "eng"} {
            if {$types=="replace"} {
                bind $widgetnm <Return> "ReplaceIt once"
                bind $widgetnm <Control-p> "ReplaceIt once"
                bind $widgetnm <Control-a> "ReplaceAll"
            } else {
                bind $widgetnm <Return> "FindIt $find"
                bind $widgetnm <Control-n> "FindIt $find"
            }
            bind $widgetnm <Control-m> { $find.l.f4.f5.cbox1 invoke }
            bind $widgetnm <Control-r> { $find.l.f4.f5.cbox2 invoke }
            bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
            bind $widgetnm <Control-d> { $find.l.f4.f3.down invoke }
        } else {
            if {$types=="replace"} {
                bind $widgetnm <Return> "ReplaceIt once"
                bind $widgetnm <Control-p> "ReplaceIt once"
                bind $widgetnm <Control-t> "ReplaceAll"
            } else {
                bind $widgetnm <Return> "FindIt $find"
                bind $widgetnm <Control-n> "FindIt $find"
            }
            bind $widgetnm <Control-r> { $find.l.f4.f5.cbox1 invoke }
            bind $widgetnm <Control-g> { $find.l.f4.f5.cbox2 invoke }
            bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
            bind $widgetnm <Control-a> { $find.l.f4.f3.down invoke }
        }
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
#    bind $find <Control-c> "destroy $find";
#    bind $find <Control-c> "CancelFind $find";
    bind $find <Control-l> "CancelFind $find"
    bind $find <Visibility> {raise $find $pad};
#ajout pour mettre a la fenetre Search devant le scipad !
# FV 21/05/04, removed $pad to cure 100% CPU bug
#    bind $pad <Expose> {catch {raise $find $pad} };
    bind $pad <Expose> {catch {raise $find} }
#ajout pour mettre a la fenetre Search devant le scipad !
    focus $find.l.f1.entry
    grab $find
}

# proc for find next
proc findnext {typof} {
	global SearchString SearchDir ReplaceString findcase c find
	if [catch {expr [string compare $SearchString "" ] }] {
		findtext $typof
	} else {
	 	FindIt $find
	}
}
