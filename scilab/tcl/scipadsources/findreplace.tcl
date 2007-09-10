proc findtextdialog {typ} {
# display the find or replace dialog box
    global find pad textFont menuFont
    global SearchString SearchDir ReplaceString caset regexpcase
    global searchinsel wholeword multiplefiles indoffirstmatch
    global listoftextarea
    global searchindir initdir fileglobpat searchinfilesalreadyrunning
    global searchforfilesonly
    global listoftagsforfind searchintagged
    global findreplaceboxalreadyopen

    if {[IsBufferEditable] == "No" && $typ=="replace"} {return}

    if {$findreplaceboxalreadyopen} {
        tk_messageBox -message [mc "Dialog box already open!"] -icon warning
        return
    }
    set findreplaceboxalreadyopen true

    set find $pad.find
    catch {destroy $find}
    toplevel $find
    wm title $find [mc "Find"]
    wm withdraw $find
    # cancelfind must be called when closing the dialog with the upper right
    # cross, otherwise the fakeselection tag is not deleted
    wm protocol $find WM_DELETE_WINDOW {cancelfind}

    # save the possibly existing selection
    # fakeselection is a copy of the sel tag, required because:
    # 1. the sel tag is not visible when focus is out of $pad
    # 2. since the Find/Replace entry boxes now have -exportselection 1
    #    the sel tag actually disappears from the textarea during the
    #    dialog build
    # note wrt Tk85:
    # using -inactiveselectbackground (Tk8.5 only) instead of using a fake
    # selection tag that mimics the real selection (sel tag) could have been
    # a good idea, but it only addresses point 1. above. Moreover, there are
    # currently good reasons for clearing the selection when switching buffers
    # so that Scipad can have only one (possibly block) selection at a time.
    # Even with the -inactiveselectbackground option set, there would
    # therefore be no visible selection in any non focussed buffer
    set selindices [gettaselind [gettextareacur] any]
    if {$selindices != ""} {
        # there is a selection
        eval "[gettextareacur] tag add fakeselection $selindices"
        [gettextareacur] tag raise foundtext fakeselection
        [gettextareacur] tag raise replacedtext fakeselection
        set seltexts [gettatextstring [gettextareacur] $selindices]
    } else {
        set seltexts ""
    }

    # entry fields
    frame $find.u
    frame $find.u.f1
    if {$typ=="replace"} {
        set bestwidth [mcmaxra "Find what:" "Replace with:"]
    } else {
        set bestwidth [mcmaxra "Find what:"]
    }
    label $find.u.f1.label -text [mc "Find what:"] \
        -width $bestwidth -font $menuFont
    entry $find.u.f1.entry -textvariable SearchString \
        -width 30 -font $textFont -exportselection 1
    menubutton $find.u.f1.mb -indicatoron 0 -text ">" \
        -font $menuFont
    menu $find.u.f1.mb.om1 -tearoff 0 -font $menuFont
    $find.u.f1.mb configure -menu $find.u.f1.mb.om1
    foreach {pattern label} [regexpsforfind] {
        $find.u.f1.mb.om1 add command -font $menuFont \
            -label "$label      $pattern" \
            -command [list insertregexpforfind $pattern]
    }
    pack $find.u.f1.label $find.u.f1.entry $find.u.f1.mb -side left
    pack configure $find.u.f1.entry -expand 1 -fill x
    if {$typ=="replace"} {
        frame $find.u.f2
        label $find.u.f2.label2 -text [mc "Replace with:"] \
            -width $bestwidth -font $menuFont
        entry $find.u.f2.entry2 -textvariable ReplaceString \
            -width 30 -font $textFont -exportselection 1
        pack $find.u.f2.label2 $find.u.f2.entry2 -side left
        pack configure $find.u.f2.entry2 -expand 1 -fill x
        pack $find.u.f1 $find.u.f2 -side top -pady 2 -padx 8 -expand 1 -fill x
    } else {
        pack $find.u.f1 -pady 4 -padx 8 -expand 1 -fill x
    }

    # buttons
    frame $find.f2
    if {$typ=="replace"} {
        set bestwidth [mcmaxra "Find &Next" "Cance&l" \
                               "Re&place" "Replace &All"]
    } else {
        set bestwidth [mcmaxra "Find &Next" "Cance&l"]
    }
    eval "button $find.f2.button1 [bl "Find &Next"] \
        -command \"multiplefilesfindreplace $find findit\" \
        -width $bestwidth -font \[list $menuFont\] "
    eval "button $find.f2.button2 [bl "Cance&l"] \
        -command \"cancelfind\" \
        -width $bestwidth -font \[list $menuFont\] "
    if {$typ == "replace"} {
        eval "button $find.f2.button3 [bl "Re&place"] \
            -command \"multiplefilesfindreplace $find replaceit\" \
            -width $bestwidth -font \[list $menuFont\] "
        eval "button $find.f2.button4 [bl "Replace &All"] \
            -command \"multiplefilesreplaceall $find\" \
            -width $bestwidth -font \[list $menuFont\] "
        pack $find.f2.button1 $find.f2.button3 $find.f2.button4 \
            $find.f2.button2 -padx 2 -pady 4
    } else {
        pack $find.f2.button1 $find.f2.button2 -padx 2 -pady 4
    }

    pack $find.u -expand 1 -fill x

    frame $find.l

    frame $find.l.f4

    frame $find.l.f4.f1

    # up/down radiobutton
    labelframe $find.l.f4.f1.f1 -borderwidth 2 -relief groove \
        -text [mc "Direction"] -font $menuFont
    eval "radiobutton $find.l.f4.f1.f1.up [bl "&Upwards"] \
        -variable SearchDir -value \"backwards\" \
        -command \"unset -nocomplain -- indoffirstmatch\" \
        -font \[list $menuFont\] "
    eval "radiobutton $find.l.f4.f1.f1.down [bl "&Downwards"] \
        -variable SearchDir -value \"forwards\" \
        -command \"unset -nocomplain -- indoffirstmatch\" \
        -font \[list $menuFont\] "
    pack $find.l.f4.f1.f1.up $find.l.f4.f1.f1.down -anchor w

    # tagged text options
    labelframe $find.l.f4.f1.f2 -borderwidth 2 -relief groove \
        -text [mc "Text types"] -font $menuFont
    frame $find.l.f4.f1.f2.f1
    set searchtagslb $find.l.f4.f1.f2.f1.lb
    scrollbar $find.l.f4.f1.f2.sbx -command "$searchtagslb xview" \
        -orient horizontal
    scrollbar $find.l.f4.f1.f2.f1.sby -command "$searchtagslb yview"
    listbox $searchtagslb -height 4 -width 15 -font $menuFont -takefocus 1 \
        -xscrollcommand "$find.l.f4.f1.f2.sbx set" \
        -yscrollcommand "$find.l.f4.f1.f2.f1.sby set" \
        -selectmode extended -exportselection 0 -activestyle none
    pack $searchtagslb $find.l.f4.f1.f2.f1.sby -side left -padx 1
    pack $find.l.f4.f1.f2.f1 $find.l.f4.f1.f2.sbx -side top -pady 1
    pack configure $searchtagslb -expand 1 -fill both 
    pack configure $find.l.f4.f1.f2.sbx    -fill x
    pack configure $find.l.f4.f1.f2.f1.sby -fill y
    # populate the listbox with the available tags
    $searchtagslb delete 0 end
    foreach tagname [tagnamesforfind] {
        $searchtagslb insert end $tagname
    }
    # remember what was selected previously in the tags listbox
    if {[llength $listoftagsforfind] > 0} {
        set i 0
        foreach tag [tagsforfind "all"] {
            if {[lsearch $listoftagsforfind $tag] != -1} {
                $searchtagslb selection set $i
            }
            incr i
        }
    }
    # update the global value of what is selected in the tags listbox
    # every time it has perhaps changed: update every time the mouse
    # clicked in the listbox, or every time the listbox looses focus
    # (in case the keyboard is used to change the listbox content)
    bind $searchtagslb <ButtonRelease-1> \
        {set listoftagsforfind [tagsforfind "onlyselected"] ; \
         resetfind \
        }
    bind $searchtagslb <FocusOut> \
        {set listoftagsforfind [tagsforfind "onlyselected"] ; \
         resetfind \
        }
    pack $find.l.f4.f1.f1 $find.l.f4.f1.f2 -side top -anchor nw
    pack configure $find.l.f4.f1.f1 -anchor nw -expand 1 -fill x

    # whole word, case, regexp, all files, in selection, and in directory checkboxes
    labelframe $find.l.f4.f5 -borderwidth 2 -relief groove \
        -text [mc "Find options"] -font $menuFont
    eval "checkbutton $find.l.f4.f5.cbox0 [bl "Match &whole word only"] \
        -variable wholeword  \
        -command \"resetfind\" \
        -font \[list $menuFont\] "
    eval "checkbutton $find.l.f4.f5.cbox1 [bl "Match &case"] \
        -variable caset -onvalue \"-exact\" -offvalue \"-nocase\" \
        -command \"resetfind\" -font \[list $menuFont\] "
    eval "checkbutton $find.l.f4.f5.cbox2 [bl "&Regular expression"] \
        -variable regexpcase  -onvalue \"regexp\" -offvalue \"standard\" \
        -command \"resetfind ; setregexpstatesettings \" \
        -font \[list $menuFont\] "
    eval "checkbutton $find.l.f4.f5.cbox3 [bl "In all &opened files"] \
        -variable multiplefiles \
        -command \"resetfind ; \
                   $find.l.f4.f5.cbox4 deselect ; \
                   if {[string compare $typ find] == 0} { \
                       $find.l.f4.f5.cbox5 deselect ; searchindirdisabled ; \
                   } ; \
                   setsearchintagsettings\" \
         -font \[list $menuFont\] "
    eval "checkbutton $find.l.f4.f5.cbox4 [bl "In &selection only"] \
        -variable searchinsel \
        -command \"tryrestoreseltag \[gettextareacur\] ; resetfind ; \
                   $find.l.f4.f5.cbox3 deselect ; \
                   if {[string compare $typ find] == 0} { \
                       $find.l.f4.f5.cbox5 deselect ; searchindirdisabled ; \
                   }\" \
        -font \[list $menuFont\] "
    eval "checkbutton $find.l.f4.f5.cbox7 [bl "In typed te&xt"] \
        -variable searchintagged \
        -command \"resetfind ; setsearchintagsettings\" -font \[list $menuFont\] "
    if {$typ == "find"} {
        eval "checkbutton $find.l.f4.f5.cbox5 [bl "In a director&y"] \
            -variable searchindir \
            -command \"resetfind ; togglesearchindir\" \
            -font \[list $menuFont\] "
    }
    if {$typ == "find"} {
        pack $find.l.f4.f5.cbox0 $find.l.f4.f5.cbox1 $find.l.f4.f5.cbox2 \
            $find.l.f4.f5.cbox3 $find.l.f4.f5.cbox4 $find.l.f4.f5.cbox7 \
            $find.l.f4.f5.cbox5 \
            -anchor w -expand 1 -fill y
    } else {
        pack $find.l.f4.f5.cbox0 $find.l.f4.f5.cbox1 $find.l.f4.f5.cbox2 \
            $find.l.f4.f5.cbox3 $find.l.f4.f5.cbox4 \
            $find.l.f4.f5.cbox7 \
            -anchor w -expand 1 -fill y
    }

    if {$typ == "find"} {
        # settings for search in files
        labelframe $find.b  -borderwidth 2 -relief groove \
            -text [mc "Find in files search options"] -font $menuFont
        frame $find.b.f0
        eval "checkbutton $find.b.f0.cbox6 [bl "Directory r&ecurse search"] \
            -variable recursesearchindir \
            -command \"resetfind\" \
            -font \[list $menuFont\] "
        eval "checkbutton $find.b.f0.cbox8 [bl "Include &hidden files"] \
            -variable searchhiddenfiles \
            -onvalue true -offvalue false \
            -font \[list $menuFont\] "
        pack $find.b.f0.cbox6 $find.b.f0.cbox8 -side left -anchor w \
            -fill x -expand 1
        pack configure $find.b.f0.cbox6 -fill none
        pack $find.b.f0 -anchor w -fill x -expand 1
        frame $find.b.f1
        set bestwidth [mcmaxra "In files/file types:" \
                               "In directory:"]
        label $find.b.f1.labelt -text [mc "In files/file types:"] \
            -width $bestwidth -font $menuFont
        entry $find.b.f1.entryt -textvariable fileglobpat \
            -width 30 -font $textFont -exportselection 1
        menubutton $find.b.f1.mbselectpat -text ">" -indicatoron 0 \
            -relief raised -font $menuFont
        menu $find.b.f1.mbselectpat.pat -tearoff 0 -font $menuFont
        $find.b.f1.mbselectpat configure -menu $find.b.f1.mbselectpat.pat
        set predefsearchinfilespatterns [knowntypes]
        foreach item $predefsearchinfilespatterns {
            foreach {patname patlist} $item {
                $find.b.f1.mbselectpat.pat add command -label $patname \
                -font $menuFont -command "getsearchpattern [list $patlist]"
            }
        }
        frame $find.b.f2
        label $find.b.f2.labeld -text [mc "In directory:"] \
            -width $bestwidth -font $menuFont
        entry $find.b.f2.entryd -textvariable initdir \
            -width 30 -font $textFont -exportselection 1
        button $find.b.f2.buttonselectdir -text "..." \
            -command \"getinitialdirforsearch\" \
            -font $menuFont

        pack $find.b.f1.labelt $find.b.f1.entryt $find.b.f1.mbselectpat \
            -side left -padx 2
        pack $find.b.f1 -anchor w -expand 1 -fill x
        pack $find.b.f2.labeld $find.b.f2.entryd $find.b.f2.buttonselectdir \
            -side left -padx 2
        pack $find.b.f2 -anchor w -expand 1 -fill x
        pack configure $find.b.f1.entryt -expand 1 -fill x -padx 5 -pady 5
        pack configure $find.b.f2.entryd -expand 1 -fill x -padx 5 -pady 5
    }

    pack $find.l.f4.f5 $find.l.f4.f1 -side left -padx 5 -anchor n -expand 1 -fill y
    pack $find.l.f4 -pady 5 -anchor w
    pack $find.l $find.f2 -side left -padx 1

    bind $find <Return> "multiplefilesfindreplace $find findit"
    bind $find <Alt-[fb $find.f2.button1]> "multiplefilesfindreplace $find findit"
    bind $find <F3> "multiplefilesfindreplace $find findit"
    if {$typ=="replace"} {
        bind $find <Alt-[fb $find.f2.button3]> "multiplefilesfindreplace $find replaceit"
        bind $find <Alt-[fb $find.f2.button4]> "multiplefilesreplaceall $find"
    }
    bind $find <Alt-[fb $find.l.f4.f5.cbox0]> { $find.l.f4.f5.cbox0 invoke }
    bind $find <Alt-[fb $find.l.f4.f5.cbox1]> { $find.l.f4.f5.cbox1 invoke }
    bind $find <Alt-[fb $find.l.f4.f5.cbox2]> { $find.l.f4.f5.cbox2 invoke }
    bind $find <Alt-[fb $find.l.f4.f5.cbox3]> { $find.l.f4.f5.cbox3 invoke }
    bind $find <Alt-[fb $find.l.f4.f5.cbox4]> { $find.l.f4.f5.cbox4 invoke }
    bind $find <Alt-[fb $find.l.f4.f5.cbox7]> { $find.l.f4.f5.cbox7 invoke }
    if {$typ=="find"} {
        bind $find <Alt-[fb $find.l.f4.f5.cbox5]> { $find.l.f4.f5.cbox5 invoke }
        bind $find <Alt-[fb $find.b.f0.cbox6]> { $find.b.f0.cbox6 invoke }
        bind $find <Alt-[fb $find.b.f0.cbox8]> { $find.b.f0.cbox8 invoke }
    }
    bind $find <Alt-[fb $find.l.f4.f1.f1.up]>   { $find.l.f4.f1.f1.up    invoke }
    bind $find <Alt-[fb $find.l.f4.f1.f1.down]> { $find.l.f4.f1.f1.down  invoke }
    bind $find <Escape> "cancelfind"
    # after 0 in the following Alt binding is mandatory for Linux only
    # This is Tk bug 1236306 (still unfixed in Tk8.4.15 and Tk 8.5a6)
    bind $find <Alt-[fb $find.f2.button2]> "after 0 cancelfind"
    bind $find <Visibility> {raise $find $pad ; focus $find.u.f1.entry}
    bind $pad  <Expose>     {catch {raise $find ; focus $find.u.f1.entry}}

    focus $find.u.f1.entry
    update
    grab $find

    setwingeom $find
    wm deiconify $find

    # the directory entry box is a drop target for text/plain content
    if {$typ == "find"} {
        dndbinddirentrybox $find.b.f2.entryd
    }

    # initial settings for direction
    $find.l.f4.f1.f1.down invoke

    # initial settings for searching in selection
    if {$seltexts == ""} {
        # there is no selection, find in selection must be disabled
        $find.l.f4.f5.cbox4 deselect
        $find.l.f4.f5.cbox4 configure -state disabled
    } else {
        # there is a selection
        # if the selection is more than one line,
        #   preselect the find in selection box
        # otherwise
        #   use that selection as the string to search for
        if {[regexp {\n} $seltexts]} {
            # this is a multiline single selection,
            # or a block selection (i.e. multiple ranges)
            $find.l.f4.f5.cbox4 select
        } else {
            # this is a single line single selection
            $find.l.f4.f5.cbox4 deselect
            $find.u.f1.entry delete 0 end
            $find.u.f1.entry insert 0 $seltexts
        }
    }

    # this must be done here and not before because the validatecommand is
    # called and resetfind uses the searchinsel value set by the tests on
    # $seltexts above
    $find.u.f1.entry configure -validate key \
        -validatecommand {resetfind ; return 1}

    # initial settings for searching in multiple files
    if {$seltexts != ""} {
        $find.l.f4.f5.cbox3 deselect
    }
    if {[llength [filteroutpeers $listoftextarea]] == 1} {
        $find.l.f4.f5.cbox3 configure -state disabled
    }

    # initial settings for search in files from a directory
    if {$typ == "find"} {
        if {$fileglobpat == ""} {
            $find.b.f1.mbselectpat.pat invoke 0
        }
        if {$initdir == ""} {
            set initdir [file normalize "."]
        }
        if {$searchindir} {
            searchindirenabled
        } else {
            searchindirdisabled
        }
        if {$multiplefiles || $searchinsel || $searchinfilesalreadyrunning} {
            $find.l.f4.f5.cbox5 deselect ; searchindirdisabled
        }
        if {$searchinfilesalreadyrunning} {
            $find.l.f4.f5.cbox5 configure -state disabled
        }
    } else {
        # needed otherwise searchindir does not exist when hitting Find Next
        # in the replace box
        set searchindir 0
    }
    set searchforfilesonly 0

    # initial settings for regexp generator state
    setregexpstatesettings

    # initial settings for search in typed (tagged) text
    setsearchintagsettings

    # preselect the entry field -
    $find.u.f1.entry selection range 0 end

    # arrange for the entry boxes selection to be erased when pasting
    bind $find.u.f1.entry <Control-v> { \
        if {[%W selection present]} { \
            %W delete sel.first sel.last \
        } ; \
        # no need to event generate %W <<Paste>> since Tk does it for us (class binding)! \
    }
    if {$typ == "replace"} {
        bind $find.u.f2.entry2 <Control-v> [bind $find.u.f1.entry <Control-v>]
    }
    if {$typ == "find"} {
        bind $find.b.f1.entryt <Control-v> [bind $find.u.f1.entry <Control-v>]
        bind $find.b.f2.entryd <Control-v> [bind $find.u.f1.entry <Control-v>]
    }

    # initialize all the remaining startup settings
    resetfind
}

proc tryrestoreseltag {textarea} {
# restore the sel tag in $textarea if there is a fakeselection in
# this textarea, and if the search in selection checkbox is checked
    global searchinsel
    if {$searchinsel} {
        set fsrange [$textarea tag ranges fakeselection]
        if {$fsrange != {}} {
            eval "$textarea tag add sel $fsrange"
        }
    }
}

proc setregexpstatesettings {} {
    global find regexpcase
    if {$regexpcase == "regexp"} {
        $find.u.f1.mb configure -state normal
    } else {
        $find.u.f1.mb configure -state disabled
    }
}

proc setsearchintagsettings {} {
    global find searchintagged listoftagsforfind
    global multiplefiles listoffile listoftextarea

    set candoit false
    if {$multiplefiles} {
        # to allow for searching in tagged text, one must have at least one file
        # with scilab language scheme, and with colorization switched on
        # since these attributes are the same for all peers of a textarea
        # we can narrow the search to the filtered list of textareas
        foreach ta [filteroutpeers $listoftextarea] {
            if {$listoffile("$ta",language) == "scilab" && \
                $listoffile("$ta",colorize) } {
                set candoit true
                break
            }
        }
    } else {
        # the current file must be of scilab scheme with colorization switched on
        if {$listoffile("[gettextareacur]",language) == "scilab" && \
            $listoffile("[gettextareacur]",colorize) } {
            set candoit true
        }
    }

    if {$searchintagged && $candoit} {
        $find.l.f4.f1.f2.f1.lb configure -state normal
    } else {
        $find.l.f4.f1.f2.f1.lb selection clear 0 end
        set listoftagsforfind [list ]
        $find.l.f4.f1.f2.f1.lb configure -state disabled
    }
    if {!$candoit} {
        $find.l.f4.f5.cbox7 deselect
        $find.l.f4.f5.cbox7 configure -state disabled
    }
}

proc togglesearchindir {} {
    global searchindir
    if {$searchindir} {
        searchindirenabled
    } else {
        searchindirdisabled
    }
}

proc searchindirenabled {} {
    global find
    $find.l.f4.f5.cbox3 deselect
    $find.l.f4.f5.cbox4 deselect
    $find.b.f0.cbox6 configure -state normal
    $find.b.f0.cbox8 configure -state normal
    $find.b.f1.labelt configure -state normal
    $find.b.f1.entryt configure -state normal
    $find.b.f1.mbselectpat configure -state normal
    $find.b.f2.labeld configure -state normal
    $find.b.f2.entryd configure -state normal
    $find.b.f2.buttonselectdir configure -state normal
    $find.l.f4.f1.f1.up configure -state disabled
    $find.l.f4.f1.f1.down invoke
    $find.l.f4.f5.cbox7 deselect
    $find.l.f4.f5.cbox7 configure -state disabled
    $find.l.f4.f1.f2.f1.lb selection clear 0 end
    $find.l.f4.f1.f2.f1.lb configure -state disabled
    pack $find.b -before $find.l -side bottom -padx 5 -pady 4 -anchor w \
        -expand 1 -fill x
    setsearchintagsettings
}

proc searchindirdisabled {} {
    global find 
    $find.b.f0.cbox6 configure -state disabled
    $find.b.f0.cbox8 configure -state disabled
    $find.b.f1.labelt configure -state disabled
    $find.b.f1.entryt configure -state disabled
    $find.b.f1.mbselectpat configure -state disabled
    $find.b.f2.labeld configure -state disabled
    $find.b.f2.entryd configure -state disabled
    $find.b.f2.buttonselectdir configure -state disabled
    $find.l.f4.f5.cbox7 configure -state normal
    $find.l.f4.f1.f1.up configure -state normal
    $find.l.f4.f1.f2.f1.lb configure -state normal
    pack forget $find.b
    setsearchintagsettings
}

proc getinitialdirforsearch {} {
    global find initdir
    set oldinitdir $initdir
    set initdir [tk_chooseDirectory -parent $find -mustexist 1]
    if {$initdir == ""} {
        # the user clicked Cancel in the choose directory dialog
        set initdir $oldinitdir
    }
}

proc getsearchpattern {pat} {
    global fileglobpat
    set fileglobpat "$pat"
}

proc togglesearchdir {} {
    global SearchDir
    if {$SearchDir == "forwards"} {
        set SearchDir "backwards"
    } else {
        set SearchDir "forwards"
    }
}

proc reversefindnext {} {
    togglesearchdir
    findnext
    togglesearchdir
}

proc findnext {} {
# proc for find next without opening the dialog if possible
# (only for find in already opened files)
    global SearchString find searchindir
    global buffermodifiedsincelastsearch
    if {[info exists SearchString]} {
        if {$SearchString != "" && !$searchindir && \
                !$buffermodifiedsincelastsearch} {
            multiplefilesfindreplace $find findit
        } else {
            findtextdialog "find"
        }
    } else {
        findtextdialog "find"
    }
}

proc multiplefilesfindreplace {w frit} {
# search and maybe replace in all the opened buffers, or in the current one
# depending on the state of the "search in all files" checkbox
# $frit contains the proc name that will be called to perform the action
# to find, $frit must be "findit"
# to replace, $frit must be "replaceit"
    global SearchString SearchDir regexpcase
    global listofmatch indoffirstmatch indofcurrentmatch
    global multiplefiles listoftextarea indofcurrentbuf listoftextarea_nopeers
    global prevfindres
    global find searchindir caset wholeword recursesearchindir fileglobpat initdir
    global searchinfilesalreadyrunning searchforfilesonly

    set pw [setparentwname $w]

    # get rid of the empty search string case
    if {$SearchString == ""} {
        if {$searchindir} {
            # an empty search string with search in files/dir set means that
            # we want to search for files themselves, not for a string in files
            # let's put a message box for confirmation since I myself forgot that feature...
            set answ [tk_messageBox -icon question -type yesno -parent $pw \
                    -title [mc "Search for file names"] \
                    -message [mc "You have given an empty search string. Are you sure you want to search among file names?"] ]
            switch -- $answ {
                yes { set searchforfilesonly 1 }
                no  { return }
            }
        } else {
            searchstringemptymessagebox $pw
            return
        }
    }

    # escape the characters that have a meaning in a Tcl string or a regexp
    set tosearchfor [escapespecialchars $SearchString]

    # check if the regexp given or constructed is valid
    if {$regexpcase == "regexp" && ![isregexpstringvalid $tosearchfor $pw]} {
        return
    }

    if {$searchindir && !$searchinfilesalreadyrunning} {
        # search in files from a directory
        cancelfind
        findinfiles $tosearchfor $caset $regexpcase $wholeword $initdir $fileglobpat $recursesearchindir $searchforfilesonly
        return
    }

    if {!$multiplefiles} {
        # search in current buffer only
        $frit $w $pw [gettextareacur] $tosearchfor $regexpcase
        # display next match after a replace
        if {$frit == "replaceit"} {
            findit $w $pw [gettextareacur] $tosearchfor $regexpcase
        }
        return
    }

    # if we did not return before this point, we are searching in all the
    # opened buffers for a non-empty string

    if {![info exists indofcurrentbuf]} {
        # this is the first search in multiple buffers

        # create list of textareas without any peers, and ensure that the
        # current textarea is kept and is the first element of the list
        # note: $listoftextarea_nopeers is only used in this proc and is not
        #       maintained elsewhere even if it is a global
        #       It must not be used anywhere else since it is not ordered
        #       as listoftextarea
        set listoftextarea_nopeers [shiftlistofta $listoftextarea [gettextareacur]]
        set listoftextarea_nopeers [filteroutpeers $listoftextarea_nopeers]

        # start from the current buffer, which is #0 because of
        # the call to shiftlistofta above
        set indofcurrentbuf 0

    } else {
        # this is not the first search in multiple buffers

        # switch to next buffer if there is no more match in this one
        if {$prevfindres != "searchagain"} {
            incr indofcurrentbuf
            if {$indofcurrentbuf == [llength $listoftextarea_nopeers] } {
                set indofcurrentbuf 0
                # the search has looped on all the buffers
                showinfo [mc "Back at the first file searched!"]
            }
            set newta [lindex $listoftextarea_nopeers $indofcurrentbuf]
            showtext $newta
            # set insertion cursor at the beginning or end of buffer
            # this is required when looping through buffers for proc
            # getnextmatch to work correctly: on first match search in
            # a buffer, when there is no selection (which is the case
            # when searching multiple files) getnextmatch returns the
            # first match after the insertion cursor
            if {$SearchDir == "forwards"} {
                $newta mark set insert 1.0
            } else {
                $newta mark set insert end
            }
            # erase listofmatch so that the next call to $frit will
            # reconstruct it
            unset -nocomplain -- listofmatch
            unset -nocomplain -- indoffirstmatch
            unset -nocomplain -- indofcurrentmatch
        }

    }

    # perform a search
    set buftosearchin [lindex $listoftextarea_nopeers $indofcurrentbuf]
    set prevfindres [$frit $w $pw $buftosearchin $tosearchfor $regexpcase]

    if {$prevfindres == "mustswitchnow"} {
        multiplefilesfindreplace $w findit
    } else {
        # display next match after a replace
        if {$frit == "replaceit"} {
            findit $w $pw $buftosearchin $tosearchfor $regexpcase
        }
    }

}

proc findit {w pw textarea tosearchfor reg} {
# find a match in $textarea and display this match
# output result can be "bufferdone", i.e. there is no more match to look for,
# or searchagain, meaning that a further search will provide a new result,
# or mustswitchnow, meaning that a buffer switch followed by a search should be
# performed now (see end of proc multiplefilesfindreplace)
# or noextend, which is returned when there was no match in selection and the
# user didn't want to extend the search to the entire buffer

    global SearchString caset multiplefiles SearchDir searchinsel wholeword
    global listoffile listofmatch listoftextarea
    global buffermodifiedsincelastsearch
    global listoftagsforfind

    set buffermodifiedsincelastsearch false

    # do the search and get the match positions and the length of the matches
    # and do it only once (per buffer) in a search session
    if {![info exists listofmatch]} {
        set listofmatch [searchforallmatches $textarea $tosearchfor \
                             $caset $reg $searchinsel $wholeword \
                             $listoftagsforfind]
    }

    # analyze the search results:
    #   look if the search failed and ask for extending if possible
    if {$listofmatch == {}} {
        # no match has been found

        # inform the user that the search has failed
        if {!$searchinsel} {

            # no match in whole buffer
            notfoundmessagebox $SearchString $textarea $pw "Find"
            return "bufferdone"

        } else {

            # no match in selection, ask for extending search
            set answer [tk_messageBox -message \
                [concat [mc "No match found in the selection for"] $SearchString \
                        [mc "\nWould you like to look for it in the entire text?"] ] \
                -parent $pw -title [mc "Find"] -type yesno -icon question]

            if {$answer == "yes"} {
                # extend search simply by removing the selection tag in the textarea
                # therefore the next call to findit will do a search in the full buffer
                # the selection will be restored later on by proc cancelfind
                # or by checking again the "search in selection only" box
                $textarea tag remove sel 1.0 end
                # no search in selection allowed once search has been extended
                $w.l.f4.f5.cbox4 deselect
                resetfind
                set findres [findit $w $pw $textarea $tosearchfor $reg]
            } else {
                # don't extend search, nothing to do
                set findres "noextend"
            }
            return $findres

        }
    }

    # analyze the search results:
    #    if we did not return before this point, then there is at least one match

    # select the match to display from the list of matches
    foreach {mpos mlen wraparound looped alreadyreplaced} \
            [getnextmatch $textarea $SearchDir $searchinsel] {}

    if {$alreadyreplaced} {
        showinfo [mc "No more matches"]
        bell
        return "mustswitchnow"
    }

    # if search looped, then we have already displayed
    # all the matches in this textarea
    # if we're searching in a single file, just go on redisplaying matches
    # otherwise return so that a new textarea can be searched
    if {$looped == "true" && $multiplefiles} {
        # return here to avoid processing of the repeated match
        return "mustswitchnow"
    }

    if {$looped == "true"} {
        set findres "bufferdone"
    } else {
        set findres "searchagain"
    }

    # if search wrapped, tell the user
    if {$wraparound == "true"} {
        showinfo [mc "Wrapped around"]
        bell
    } else {
        delinfo
    }

    # arrange for this match to be visible and tag it
    $textarea see $mpos
    $textarea mark set insert $mpos
    foreach ta [filteroutpeers $listoftextarea] {
        # this must be done for each ta, not only for $textarea
        # because of the switch buffer case in tile mode
        # however since this is only playing with tags, there is
        # no need to do it for peers
        $ta tag remove foundtext 1.0 end
        $ta tag remove replacedtext 1.0 end
    }
    $textarea tag add foundtext $mpos  "$mpos + $mlen char"

    # the following three bindings are required to remove the foundtext tag
    # after a find next triggered by F3. In order to erase them after use,
    # the binding is redefined in the binded script itself to what this script
    # was before binded instructions are added (+ statement) here
    # the foundtext tag removal instruction is also prevented from cumulating
    # (e.g. when hitting F3 repeatedly with no action in the textarea in
    # between)
    # last note, this must be done for KeyPress and ButtonPress of course, but
    # also for Button-1 since:
    #    a. this event is more specific than ButtonPress and will therefore be
    #       triggered when the user clicks Button-1
    #    b. the binded script for Button-1 contains a {break}, which is needed
    #       for dnd but prevents here the wanted ButtonPress binding to trigger
    set bindtext [bind $textarea <KeyPress>]
    if {[string first " tag remove foundtext 1.0 end" $bindtext] == -1} {
        bind $textarea <KeyPress> "+%W tag remove foundtext 1.0 end ; bind %W <KeyPress> [list $bindtext]"
    }
    set bindtext [bind $textarea <ButtonPress>]
    if {[string first " tag remove foundtext 1.0 end" $bindtext] == -1} {
        bind $textarea <ButtonPress> "+%W tag remove foundtext 1.0 end ; bind %W <ButtonPress> [list $bindtext]"
    }
    set bindtext [bind $textarea <Button-1>]
    if {[string first " tag remove foundtext 1.0 end" $bindtext] == -1} {
        bind $textarea <Button-1> "+%W tag remove foundtext 1.0 end ; bind %W <Button-1> [list $bindtext]"
    }

    # prevent the dialog box from hiding the match string
    if {[winfo exists $w]} {
        MoveDialogIfTaggedTextHidden $w $textarea foundtext
    }

    # update status bar data
    keyposn $textarea

    return $findres
}

proc replaceit {w pw textarea tosearchfor reg {replacesingle 1}} {
# find a match in $textarea, and replace this match
# output result can be "bufferdone", i.e. there is no more match to look for,
# or searchagain, meaning that a further search will provide a new result,
# or mustswitchnow, meaning that a buffer switch followed by a search should be
# performed now (see end of proc multiplefilesfindreplace)
# or noextend, which is returned when there was no match in selection and the
# user didn't want to extend the search to the entire buffer
# $replacesingle toggles the popup messageboxes and infos in the status bar,
# as well as the background colorization of user functions and keyposn (do
# not need to be done for each single replace of a replace all)

    global SearchString ReplaceString caset multiplefiles SearchDir searchinsel wholeword
    global listoffile listofmatch listoftextarea
    global buffermodifiedsincelastsearch
    global listoftagsforfind

    set buffermodifiedsincelastsearch false

    # if there is no already found matching text (Find Next was not hit)
    # therefore perform a search first get the match positions and
    # the length of the matches
    # and do it only once (per buffer) in a search session
    set newlistofmatches false
    if {![info exists listofmatch]} {
        set listofmatch [searchforallmatches $textarea $tosearchfor \
                             $caset $reg $searchinsel $wholeword \
                             $listoftagsforfind]
        set newlistofmatches true
    }

    # analyze the search results
    #   look if the search failed
    if {$listofmatch == {}} {
        # no match has been found

        # inform the user that the search has failed
        if {!$searchinsel} {

            # no match in whole buffer
            if {$replacesingle} {
                notfoundmessagebox $SearchString $textarea $pw "Replace"
            }
            return "bufferdone"

        } else {

            # no match in selection, ask for extending search
            set answer [tk_messageBox -message \
                [concat [mc "No match found in the selection for"] $SearchString \
                        [mc "\nWould you like to look for it in the entire text?"] ] \
                -parent $pw -title [mc "Replace"] -type yesno -icon question]

            if {$answer == "yes"} {
                # extend search simply by removing the selection tag in the textarea
                # therefore the next call to replaceit will do a search in the full buffer
                # the selection will be restored later on by proc cancelfind
                # or by checking again the "search in selection only" box
                $textarea tag remove sel 1.0 end
                # no search in selection allowed once search has been extended
                $w.l.f4.f5.cbox4 deselect
                resetfind
                set replres [replaceit $w $pw $textarea $tosearchfor $reg 1]
            } else {
                # don't extend search, nothing to do
                set replres "noextend"
            }
            return $replres

        }
    }

    # analyze the search results:
    #    if we did not return before this point, then there is at least one match

    # select the match to replace from the list of matches
    set foundtextrange [$textarea tag ranges foundtext]
    if {$foundtextrange == {} || !$replacesingle || $newlistofmatches} {
        # there is no already found matching text (Find Next was not hit)
        # therefore get next match
        foreach {mpos mlen wraparound looped alreadyreplaced} \
            [getnextmatch $textarea $SearchDir $searchinsel] {}
    } else {
        # there is an already found matching text (Find Next was hit before)
        # therefore take this one for the replace process
        foreach {mpos mlen wraparound looped alreadyreplaced} [getcurrentmatch $textarea] {}
    }

    # if we have already replaced all the matches in this textarea,
    # we don't want to replace further therefore just return so that
    # a new textarea can be searched
    if {$alreadyreplaced} {
        # tell the user there is no more matches in this buffer
        if {$replacesingle} {
            showinfo [mc "No more matches"]
            bell
        }
        # return here to avoid processing of the repeated match
       return "mustswitchnow"
    }

    if {$looped == "true"} {
        set replres "bufferdone"
    } else {
        set replres "searchagain"
    }

    # if search wrapped, tell the user when replacing items one by one only
    if {$wraparound == "true" && $replacesingle} {
        showinfo [mc "Wrapped around"]
        bell
    } else {
        delinfo
    }

    # arrange for this match to be visible, replace it, colorize it and tag it
    $textarea see $mpos
    set oldSeparator [$textarea cget -autoseparators]
    if {$oldSeparator} {
        $textarea configure -autoseparators 0
        $textarea edit separator
    }
    $textarea delete $mpos "$mpos + $mlen char"
    # the test on ReplaceString shouldn't be needed and is only here to
    # work around Tk bug 1275237 (fixed since Tk8.4.13RC1 or Tk 8.5a4RC3)
    if {$ReplaceString != ""} {
        $textarea insert $mpos $ReplaceString
    }
    if {$oldSeparator} {
        $textarea edit separator
        $textarea configure -autoseparators 1
    }
    tagcontlines $textarea
    set uplimit [getstartofcolorization $textarea $mpos]
    set dnlimit [getendofcolorization $textarea $mpos]
    colorize $textarea $uplimit $dnlimit
    if {$replacesingle} {
        backgroundcolorizeuserfun
    }
    $textarea mark set insert $mpos
    foreach ta [filteroutpeers $listoftextarea] {
        # this must be done for each ta, not only for $textarea
        # because of the switch buffer case in tile mode
        # however since this is only playing with tags, there is
        # no need to do it for peers
        $ta tag remove foundtext 1.0 end
        $ta tag remove replacedtext 1.0 end
    }
    set lenR [string length $ReplaceString]
    $textarea tag add replacedtext $mpos  "$mpos + $lenR char"
    # If replacement occurred starting at the first selected character or
    # up to the last selected character, then fakeselection should be extended
    # by hand since tags have no gravity in Tcl
    # This is most simply done by always tagging the replaced text as fakeselection
    # for all replace positions located in the selection
    if {[$textarea tag ranges fakeselection] != {}} {
        if {[$textarea compare fakeselection.first <= $mpos] && \
            [$textarea compare $mpos <= fakeselection.last]} {
                $textarea tag add fakeselection $mpos  "$mpos + $lenR char"
        }
    }

    # tag the current match as now being replaced, and shift the
    # remaining not yet replaced matches on the same line
    setcurmatchasreplaced $textarea $lenR

    # prevent the dialog box from hiding the match string
    MoveDialogIfTaggedTextHidden $w $textarea replacedtext

    # update status bar data
    if {$replacesingle} {
        keyposn $textarea
    }

    # update breakpoint tags
    reshape_bp

    return $replres
}

proc multiplefilesreplaceall {w} {
# search and replace all matches in all the opened buffers, or in the current one
# depending on the state of the "search in all files" checkbox
    global SearchString regexpcase
    global listofmatch indoffirstmatch indofcurrentmatch
    global multiplefiles listoftextarea
    global prevfindres

    set pw [setparentwname $w]

    # get rid of the empty search string case
    if {$SearchString == ""} {
        searchstringemptymessagebox $pw
        return
    }

    # escape the characters that have a meaning in a Tcl string or a regexp
    set tosearchfor [escapespecialchars $SearchString]

    # check if the regexp given or constructed is valid
    if {$regexpcase == "regexp" && ![isregexpstringvalid $tosearchfor $pw]} {
        return
    }

    if {!$multiplefiles} {
        # search in current buffer only
        replaceall $w $pw [gettextareacur] $tosearchfor $regexpcase
        return
    }

    # if we did not return before this point, we are replacing a non-empty
    # string in all the opened buffers (but ignoring peers)

    set totreplaced 0
    foreach ta [filteroutpeers $listoftextarea] {
        incr totreplaced [replaceall $w $pw $ta $tosearchfor $regexpcase]
    }

    showinfo "$totreplaced [mc "replacements done"]"
    if {$totreplaced == 0} {
        notfoundinallmessagebox $SearchString $pw "Replace"
    }

    # userfun colorization must be done here because replacement
    # might have changed a user function - do it only once, and not
    # after each single replacement of a replace all, even not
    # after each buffer in which replace all occurs
    backgroundcolorizeuserfun

    # update status bar data - do it once only, same as above
    keyposn [gettextareacur]
}

proc replaceall {w pw textarea tosearchfor reg} {
# find all matches in $textarea, and replace these matches

    global SearchString
    global listofmatch indoffirstmatch indofcurrentmatch
    global listoffile multiplefiles
    global buffermodifiedsincelastsearch
    global listoftagsforfind
    global pad caset searchinsel wholeword ;# these are for the progress bar

    set buffermodifiedsincelastsearch false

    # erase listofmatch so that the first call to replaceit will
    # reconstruct it
    unset -nocomplain -- listofmatch
    unset -nocomplain -- indoffirstmatch
    unset -nocomplain -- indofcurrentmatch

    set nbofreplaced 0

    # initialize progress bar
    # note that efficiency is maintained: listofmatches is
    # computed here and only once (it won't be done later
    # in proc replaceit, even not during the first call)
    set replprogressbar [Progress $pad.rpb]
    pack $replprogressbar -fill both -expand 0 -before $pad.pw0 -side bottom
    set listofmatch [searchforallmatches $textarea $tosearchfor \
                         $caset $reg $searchinsel $wholeword \
                         $listoftagsforfind]
    set nbofmatches [llength $listofmatch]
    if {$nbofmatches == 0} {
        # avoid division by zero error in the progress bar computations
        set nbofmatches 1
    }

    # loop on matches and update progress bar
    set prevfindres "searchagain"
    while {$prevfindres == "searchagain"} {
        set prevfindres [replaceit $w $pw $textarea $tosearchfor $reg 0]
        SetProgress $replprogressbar $nbofreplaced $nbofmatches $listoffile("$textarea",displayedname)
        update idletasks
        incr nbofreplaced
    }
    incr nbofreplaced -1

    destroy $replprogressbar

    if {!$multiplefiles} {
        showinfo "$nbofreplaced [mc "replacements done"]"
        if {$nbofreplaced == 0 && $prevfindres != "noextend"} {
            notfoundmessagebox $SearchString $textarea $pw "Replace"
        }
        # userfun colorization must be done here because replacement
        # might have changed a user function - do it only once, and not
        # after each single replacement of a replace all, even not
        # after each buffer in which replace all occurs
        backgroundcolorizeuserfun
        # update status bar data - do it once only, same as above
        keyposn $textarea
    }

    # erase listofmatch so that the next call to a find/replace command
    # will reconstruct it - needed if for instance the user hits find next
    # after a replace all: find next gets the next match regardless of its
    # "alreadyreplaced" status
    unset -nocomplain -- listofmatch
    unset -nocomplain -- indoffirstmatch
    unset -nocomplain -- indofcurrentmatch

    return $nbofreplaced
}

proc searchforallmatches {textarea str cas reg ssel whword listoftags} {
# search for the matches in the provided $textarea for the string $str
# taking into account the case argument $cas, the regexp argument $reg
# and the "whole word" flag
# the search occurs in the selection if $ssel == 1, or in the full
# textarea otherwise
# block selection search is supported
# all matches are returned in a list, and always in the "forwards" order
# each element of the return list follows the format described in proc
# doonesearch
# an empty $matchlist return result means there is no match in $textarea 

    set matchlist {}

    foreach {start stop} [getsearchlimits $textarea $ssel] {

        set match [doonesearch $textarea $start $stop $str "forwards" $cas $reg $whword $listoftags]
        while {[lindex $match 0] != ""} {
            lappend matchlist $match
            set mleng [lindex $match 1]
            if {$mleng == 0} {
                # a match was found but its length is zero - can happen while
                # searching for a regexp, for instance ^ or \A
                # in that case, let's artificially set match length to 1 (only
                # in this loop) to avoid endless loop
                set mleng 1
            }
            set start [$textarea index "[lindex $match 0] + $mleng c"]
            # $ta search with $start == $stop == [$ta index end] wraps around
            # this is not wanted (and contrary to the Tcl man page for text)
            # this is Tk bug 1513517
            # once this bug is fixed, the three lines below can be removed
            if {[$textarea compare $start == $stop]} {
                break
            }
            set match [doonesearch $textarea $start $stop $str "forwards" $cas $reg $whword $listoftags]
        }

    }

    return $matchlist
}

proc getsearchlimits {textarea ssel} {
# decide about the start and stop positions of a forwards search:
# if there is no selection, then start = 1.0 and stop = last char of
# $textarea, and return a 2-elements list with start and stop
# if there is a selection, then return [$textarea tag ranges fakeselection]
# block selection is therefore supported as well as single range selection
    if {!$ssel} {
        # there is no existing selection (or the user asked not to search
        # in selection)
        set sta 1.0
        set sto [$textarea index end]
        set selranges [list $sta $sto]
    } else {
        set selranges [$textarea tag ranges fakeselection]
    }
    return $selranges
}

proc doonesearch {textarea sta sto str dir cas reg whword listoftags} {
# perform one search operation in $textarea between start position $sta
# and stop position $sto, taking into account the case argument $cas,
# the regexp argument $reg, the direction $dir and the "whole word" flag
# the searched string is $str
# return value: a list containing:
#   if found:
#     the position of the match, the length of the match in character counts
#     and the number 0 (this flag is used by replaceit and is set to 1 when
#     the match has been replaced)
#   if not found:
#     the three elements "" 0 0
# note:
#   the match length is specific to each match and cannot be considered as
#   a constant for all the matches - for instance, regexp searching for
#   \m(\w)*\M (i.e. any word) can provide different match lengths
    global Tk85

    # create the options list
    set optlist [list $cas -$dir -count MatchLength]
    if {$reg == "regexp"} {
        lappend optlist -regexp
    }
    if {$Tk85} {
        # this option doesn't work as expected before Tk cvs of 10/10/05
        # see http://groups.google.fr/group/comp.lang.tcl/browse_thread/thread/e80f2586408ab598/2a3660c107cd21ba?lnk=raot&hl=fr#2a3660c107cd21ba
        # and Tk bug 1281228 - solved in the final 8.5 release
        lappend optlist -strictlimits
    }

    # quotes around $str are mandatory to find strings that include spaces
    set MatchPos [ eval "$textarea search $optlist -- \"$str\" $sta $sto" ]

    if {$MatchPos != ""} {
        # check if the match found satisfies the required constraints
        while {![arefindconstraintssatisfied $whword $listoftags $textarea $MatchPos $MatchLength]} {
            set sta [$textarea index "$MatchPos + $MatchLength c"]
            set MatchPos [ eval "$textarea search $optlist -- \"$str\" $sta $sto" ]
            if {$MatchPos == ""} {
                # no match candidates left
                break
            }
        }
    }

    # if no match found, return zero length
    if {$MatchPos == ""} {
        set MatchLength 0
    }
    return [list $MatchPos $MatchLength 0]
}

proc arefindconstraintssatisfied {whword listoftags textarea MatchPos MatchLength} {
# check if some or all find constraints below are satisfied or not
# 1 - whole word
#     if $whword is true, then the whole word constraint is checked
# 2 - tagged text
#     if listoftags is non empty, then the tagged text constraint is checked
# return true or false

    set OK false

    if {!$whword} {
        # normal search mode (no whole word)

        if {[llength $listoftags] == 0} {
            # neither whole word, nor tagged text
            set OK true
        } else {
            # not whole word, but tagged text
            if {[istagged $textarea $MatchPos $MatchLength $listoftags]} {
                set OK true
            }
        }

    } else {
        # whole word search mode

        if {[llength $listoftags] == 0} {
            # whole word, but not tagged text
            if {[iswholeword $textarea $MatchPos $MatchLength]} {
                set OK true
            }
        } else {
            # whole word and tagged text
            if {[iswholeword $textarea $MatchPos $MatchLength] && \
                [istagged $textarea $MatchPos $MatchLength $listoftags]} {
                set OK true
            }
        }

    }

    return $OK
}

proc getcurrentmatch {textarea} {
# get current match position and length in an already existing (non empty) list
# of matches
# return value is a list:
#    { match_position match_length has_wrapped_flag has_looped_flag alreadyreplaced_flag}
#    has_wrapped_flag is always false
#    has_looped_flag is always false
    global listofmatch indoffirstmatch indofcurrentmatch
    set currentmatch [lindex $listofmatch $indofcurrentmatch]
    set curmatchpos [lindex $currentmatch 0]
    set curmatchlen [lindex $currentmatch 1]
    set haswrapped false
    set haslooped false
    set alreadyreplaced [lindex $currentmatch 2]
    return [list $curmatchpos $curmatchlen $haswrapped $haslooped $alreadyreplaced]
}

proc getnextmatch {textarea dir ssel} {
# get next match position and length in an already existing (non empty) list
# of matches. Only matches that are not yet tagged as already replaced ones
# are returned
# direction $dir of searching is taken into account, as well as the possibly
# existing selection ($ssel is 1 if there is a selection and the user asked
# for searching in it)
# return value is a list:
#    { match_position match_length has_wrapped_flag has_looped_flag alreadyreplaced_flag}
#    has_wrapped_flag is true if and only if to get the next match we had
#        to pass the end of the file or selection ("forwards" case), or the
#        beginning of the file or selection ("backwards" case)
#    has_looped_flag is true if and only if the entire list of matches has
#        been traversed. In this case $match_position is again the first
#        match position
#    alreadyreplaced_flag is 1 if and only if there is no more not yet
#        replaced matches in $listofmatch - in this case the other output
#        values should be ignored by the calling procedure

    foreach {mpos mlen alreadyreplaced wraparound looped} \
            [getnextmatchany $textarea $dir $ssel] {}

    set firstpos $mpos
    while {$alreadyreplaced} {
        foreach {mpos mlen alreadyreplaced wraparound looped} \
                [getnextmatchany $textarea $dir $ssel] {}
        if {$mpos == $firstpos} {
            break
        }
    }

    return [list $mpos $mlen $wraparound $looped $alreadyreplaced]
}

proc getnextmatchany {textarea dir ssel} {
# return the next match and ignores the alreadyreplaced tag value
# see proc getnextmatch for details
    global listofmatch indoffirstmatch indofcurrentmatch haswrappedbefore

    if {![info exists indoffirstmatch]} {
        # this is the first request for a match in the listofmatch

        # get the match located just after or before...
        if {!$ssel} {
            #... the insertion cursor if there is no selection
            set pos [$textarea index insert]
        } else {
            #... the selection (first range) start or (last range) end if
            # there is a selection
            if {$dir == "forwards"} {
                set pos [$textarea index fakeselection.first]
            } else {
                set pos [$textarea index fakeselection.last]
            }
        }

        set indoffirstmatch [getnexteltid $textarea $pos $listofmatch $dir]
        set haswrapped false
        set haswrappedbefore false

        if {$indoffirstmatch == [llength $listofmatch]} {
            set indoffirstmatch 0
            set haswrapped true
        }
        if {$indoffirstmatch == -1} {
            set indoffirstmatch [expr {[llength $listofmatch] - 1}]
            set haswrapped true
        }

        if {$haswrapped == "true"} {
            set haswrappedbefore true
        }

        set indofcurrentmatch $indoffirstmatch
        set haslooped false

    } else {
        # this is not the first request for a match in the listofmatch

        set haswrapped false
        if {$dir == "forwards"} {
            incr indofcurrentmatch
            if {$indofcurrentmatch == [llength $listofmatch]} {
                set indofcurrentmatch 0
                set haswrapped true
            }
        } else {
            incr indofcurrentmatch -1
            if {$indofcurrentmatch == -1} {
                set indofcurrentmatch [expr {[llength $listofmatch] - 1}]
                set haswrapped true
            }
        }

        set haslooped false
        if {$indofcurrentmatch == $indoffirstmatch} {
            set haslooped true
        }

        # haswrappedbefore is now used to set haslooped
        # the test on $indofcurrentmatch above is not enough when playing
        # with the Find Next button during a replace session
        if {$haswrappedbefore == "true" && $haswrapped == "true"} {
            set haslooped true
        }

        if {$haswrapped == "true"} {
            set haswrappedbefore true
        }

    }

    set currentmatch [lindex $listofmatch $indofcurrentmatch]
    set curmatchpos [lindex $currentmatch 0]
    set curmatchlen [lindex $currentmatch 1]
    set curmatchrep [lindex $currentmatch 2]
    return [list $curmatchpos $curmatchlen $curmatchrep $haswrapped $haslooped]
}

proc getnexteltid {textarea aval alist dir} {
# get the index in $alist of the element that comes after or before $aval
# (depending on the search direction $dir: respectively "forwards" and
# "backwards")
# comparison is performed with .text compare, i.e. as text widget indices
# $alist has the format described in proc searchforallmatches, it is already
# ordered by match position in forward order
# $aval does not need to be an element of $alist
    if {$dir == "forwards"} {
        set id 0
        set lval [lindex [lindex $alist $id] 0]
        set compres [$textarea compare $aval >= $lval]
        while {$compres == 1 && $id < [expr {[llength $alist] - 1}]} {
            incr id
            set lval [lindex [lindex $alist $id] 0]
            set compres [$textarea compare $aval >= $lval]
        }
        if {$compres == 0} {
            return $id
        } else {
            # for all the elements of $alist, $aval >= $lval is wrong
            return [expr {$id + 1}] ; # i.e. [llength $alist]
        }
    } else {
        set id [expr {[llength $alist] - 1}]
        set lval [lindex [lindex $alist $id] 0]
        set compres [$textarea compare $aval <= $lval]
        while {$compres == 1 && $id > 0} {
            incr id -1
            set lval [lindex [lindex $alist $id] 0]
            set compres [$textarea compare $aval <= $lval]
        }
        if {$compres == 0} {
            return $id
        } else {
            # for all the elements of $alist, $aval <= $lval is wrong
            return [expr {$id - 1}] ; # i.e. -1
        }
    }
}

proc setcurmatchasreplaced {textarea lenR} {
# tag the current match as now being replaced, and shift the
# remaining not yet replaced matches on the same line
# this must be done otherwise $listofmatch items could be
# out of sync with the positions of the matches in the text
# widget after a replace
# the amount of shift is $lenR (length of the replacement
# string) minus the length of the match
    global listofmatch indofcurrentmatch

    # tag the current match as a replaced match
    set curmatch [lindex $listofmatch $indofcurrentmatch]
    set curmatch [lreplace $curmatch 2 2 1]
    set listofmatch [lreplace $listofmatch $indofcurrentmatch $indofcurrentmatch $curmatch]

    # shift certain matches
    set lendiff [expr {$lenR - [lindex $curmatch 1]}]
    scan [lindex $curmatch 0] "%d.%d" curmatchlin curmatchcol
    set lm $listofmatch
    set i 0
    foreach mat $lm {
        scan [lindex $mat 0] "%d.%d" malin macol
        if {$malin == $curmatchlin && $macol > $curmatchcol} {
            # shift only the matches located on the same line and after $curmatch
            set newpos $malin.[expr {$macol + $lendiff}]
            set listofmatch [lreplace $listofmatch $i $i [lreplace $mat 0 0 $newpos]]
        }
        incr i
    }
}

proc resetfind {} {
# reset the find/replace settings to their initial default values
# so that the next find or replace will scan the buffer(s) again
# and create a new list of matches
    global listofmatch indoffirstmatch indofcurrentmatch
    global indofcurrentbuf

    # reset the search data (current buffer)
    unset -nocomplain -- listofmatch
    unset -nocomplain -- indoffirstmatch
    unset -nocomplain -- indofcurrentmatch

    # reset the search data (multi-buffer search)
    unset -nocomplain -- indofcurrentbuf
}

proc cancelfind {} {
# end of a find/replace session
    global pad listoftextarea find
    global listoftagsforfind
    global findreplaceboxalreadyopen

    foreach textarea [filteroutpeers $listoftextarea] {
        $textarea tag remove foundtext 1.0 end
        $textarea tag remove replacedtext 1.0 end

        set fsrange [$textarea tag ranges fakeselection]
        if {$fsrange != {}} {
            # there was a selection at the time the find dialog was opened, restore it
            eval "$textarea tag add sel $fsrange"
            $textarea tag remove fakeselection 1.0 end
        }
    }

    set listoftagsforfind [tagsforfind "onlyselected"]

    bind $pad <Expose> {}
    destroy $find
    set findreplaceboxalreadyopen false
}

proc setparentwname {w} {
# set the window pathname that will be used as parent for the messageboxes
    # if the dialog is open (this is always true for replace or replaceall
    # but may be false for find because of the F3 - Find Next case)...
    global pad
    if {[winfo exists $w]} {
        #... use it as parent for the messageboxes
        set pw $w
    } else {
        #... else use the toplevel
        set pw $pad
    }
    return $pw
}

proc escapespecialchars {str} {
# escape certain special characters that have a meaning in a Tcl string,
# or in a regexp
# these chars are \ " $ [ ] ?
# use of this proc:
#    - find/replace strings
#    - watched variable names (debugger)
#    - Scilab function arguments in the configure box (debugger)
# note: braces {} are not dealt with in this proc because:
#    - they can be found or replaced without escaping
#    - they can't be part of Scilab variables names
    set str [string map {"\\" "\\\\"} $str]
    set str [string map {"\"" "\\\""} $str]
    set str [string map {"\$" "\\\$"} $str]
    set str [string map {"\[" "\\\["} $str]
    set str [string map {"\]" "\\\]"} $str]
    set str [string map {"\?" "\\\?"} $str]
    return $str
}

proc iswholeword {textarea mpos mlen} {
# return true if the match given in argument is a "whole word", i.e.
# if the character before it and the character after it is a blank
# character (space or tab), or a newline, or beginning or end of the text

    set OKleft 0
    set OKright 0
    set pat "\[ \n\t\]"

    set prevc [$textarea index "$mpos - 1c"]
    set nextc [$textarea index "$mpos + $mlen c"]

    # check if the match starts at the first character
    if {[$textarea compare $prevc == $mpos]} {
        set OKleft 1
    }

    # check if the match ends at the last character
    if {$nextc == ""} {
        set OKright 1
    }

    # check previous character against "whole word" boundaries class
    if {[string match $pat [$textarea get $prevc]]} {
        set OKleft 1
    }

    # check next character against "whole word" boundaries class
    if {[string match $pat [$textarea get $nextc]]} {
        set OKright 1
    }

    if {$OKleft && $OKright} {
        return 1
    } else {
        return 0
    }
}

proc istagged {textarea MatchPos MatchLength listoftags} {
# check if the match found is correctly and entirely tagged as expected, i.e.
# that each character of the match is tagged by at least one element of
# $listoftags
# return true or false
# if $listoftags is an empty list, always return true

    # make the check quicker when not searching for tagged text
    if {[llength $listoftags] == 0} {
        return true
    }

    set OK true
    set i 0
    while {$i<$MatchLength} {
        set pos [$textarea index "$MatchPos + $i c"]
        set tagsatpos [$textarea tag names $pos]
        # look at the intersection of the two lists of tags
        set emptyintersect true
        foreach tag $tagsatpos {
            if {[lsearch -exact $listoftags $tag] != -1} {
                set emptyintersect false
                # as soon as the intersection is not void, it's enough to
                # know that the current character of the match is OK and
                # there is no need to compute the entire intersection set
                break
            }
        }
        if {$emptyintersect} {
            set OK false
            # no need to check the remaining characters of the match
            break
        }
        incr i
    }

    return $OK
}

proc isregexpstringvalid {tosearchfor pw} {
# possible errors in regexp compilation are detected at this point
# this can happen easily since the user can enter any expression and
# call for a regexp search

    # attempt to regexp search
    # what is inside the catch{} should be identical (i.e. the eval is needed)
    # to the line actually performing the search in proc doonesearch
    if {[catch { eval "[gettextareacur] search -regexp -- \"$tosearchfor\" 1.0 end" } ]} {
        tk_messageBox -message \
            [mc "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped."] \
            -parent $pw -title [mc "Find"] -icon error
        return 0
    } else {
        return 1
    }
}

proc searchstringemptymessagebox {pw} {
# display a message box telling that the string to search for is empty
    tk_messageBox -message [mc "You are searching for an empty string!"] \
                  -parent $pw -title [mc "Find"]
}

proc notfoundmessagebox {SearchString textarea pw mbtitle} {
# display a message box telling that the searched string could not be found
    global listoffile
    tk_messageBox -message \
        [concat [mc "The string"] $SearchString [mc "could not be found in"] \
                $listoffile("$textarea",fullname) ] \
        -parent $pw -title [mc $mbtitle]
}

proc notfoundinallmessagebox {SearchString pw mbtitle} {
# display a message box telling that the searched string could not be found
#in all the opened buffers
    tk_messageBox -message \
        [concat [mc "The string"] $SearchString \
                [mc "could not be found in any of the opened files !"] ] \
        -parent $pw -title [mc $mbtitle]
}

proc MoveDialogIfTaggedTextHidden {w textarea tagname} {
# this proc checks whether the intersection between a dialog and a rectangular
# area of text is empty or not
# if the intersection is empty, it does nothing
# otherwise it moves the dialog in such a way that it does not hide the tagged
# text anymore
# the dialog is identified by its pathname $w
# the text area is identified by its textarea name and a tag name, and this tag
# *must* extend onto a single line - this is currently not a problem because this
# proc is called with either $tagname == foundtext or $tagname == replacedtext,
# and, at least with Tk 8.4 find/replace does not cross newlines for matching -
# warning: this will change with Tk 8.5
# it is assumed that only one contiguous portion of text is tagged with $tagname
# if no character in $textarea is tagged by $tagname, then the text span
# considered defaults to the insertion cursor location
    global pad textFont

    # coordinates of the _d_ialog - left, right, top, bottom - screen coordinate system
    foreach {ww wh wdl wdt} [totalGeometry $w] {}
    set ld $wdl
    set rd [expr {$wdl + $ww}]
    set td $wdt
    set bd [expr {$wdt + $wh}]

    # get _t_agged text area coordinates relative to the $textarea coordinate system
    if {[catch {set taggedlcoord [$textarea dlineinfo $tagname.first]} ]} {
        set taggedlcoord [$textarea dlineinfo insert]
    }
    foreach {taggedtextpos taggedtextend} [$textarea tag ranges $tagname] {}
    if {![info exists taggedtextpos]} {
        set taggedtextpos [$textarea index insert]
        set taggedtextend $taggedtextpos
    }
    scan $taggedtextpos "%d.%d" ypos xpos
    set taggedtext  [$textarea get $taggedtextpos $taggedtextend]
    set startofline [$textarea get $ypos.0 $taggedtextpos]
    set taggedtextwidth  [font measure $textFont $taggedtext]
    set startoflinewidth [font measure $textFont $startofline]
    set lineheight [lindex $taggedlcoord 3]
    set lt1 [expr {[lindex $taggedlcoord 0] + $startoflinewidth}]
    set rt1 [expr {$lt1 + $taggedtextwidth}]
    set tt1 [lindex $taggedlcoord 1]
    set bt1 [expr {$tt1 + $lineheight}]

    # convert tagged text coordinates into screen coordinate system
    set lta [winfo rootx $textarea]
    set tta [winfo rooty $textarea]
    set lt [expr {$lt1 + $lta}]
    set rt [expr {$rt1 + $lta}]
    set tt [expr {$tt1 + $tta}]
    set bt [expr {$bt1 + $tta}]

    # check if the dialog overlaps the tagged text (intersection of two rectangles)
    if { ! ( ($ld > $rt) || ($lt > $rd) || ($td > $bt) || ($tt > $bd) ) } {
        # the two rectangles intersect, move the dialog
        set newx [expr {$ld - ($rd - $ld)}]
        if {$newx < 1} {
            set newx [expr {[winfo screenwidth $w] - ($rd - $ld)}]
        }
        set newy [expr {$td - ($bd - $td)}]
        if {$newy < 1} {
            set newy [expr {[winfo screenheight $w] - ($bd - $td)}]
        }
        wm geometry $w "+$newx+$newy"
    }
}

proc totalGeometry {{w .}} {
# compute the total width and height of a window, as well as its top left
# corner position
# this differs from wm geometry in that it includes the decorations width
# and height
# this proc was initially copied from http://wiki.tcl.tk/11291
# related information can be found in http://wiki.tcl.tk/11502
# it has been slightly modified to accomodate for negative values coming
# from wm geometry
# this improvement has been propagated to the wiki
    set geom [wm geometry $w]
 #   regexp -- {([0-9]+)x([0-9]+)\+([0-9]+)\+([0-9]+)} $geom -> \
      width height decorationLeft decorationTop
    scan $geom "%dx%d+%d+%d" width height decorationLeft decorationTop
    set contentsTop [winfo rooty $w]
    set contentsLeft [winfo rootx $w]

    # Measure left edge, and assume all edges except top are the
    # same thickness
    set decorationThickness [expr {$contentsLeft - $decorationLeft}]

    # Find titlebar and menubar thickness
    set menubarThickness [expr {$contentsTop - $decorationTop}]

    incr width [expr {2 * $decorationThickness}]
    incr height $decorationThickness
    incr height $menubarThickness

    return [list $width $height $decorationLeft $decorationTop]
}

proc regexpsforfind {} {
# return the following flat list:
#   {regexppattern1 name1 regexppattern2 name2 ... }
# with localized names
# this list is used in the find/replace box to insert ready cooked regexps
# in the find entry place
# note that is is useless to add regexps that would match across text lines
# because the text widget search -regexp engine does only support matching
# inside single lines of text (well, this is true for Tk 8.4 and should
# change with Tk 8.5)
    global scommRE_rep scilabnameREpat dotcontlineRE_rep
    global floatingpointnumberREpat_rep rationalnumberREpat_rep
    return [list \
        {.} [mc "Any single character"] \
        {*} [mc "Zero or more"] \
        {+} [mc "One or more"] \
        {?} [mc "Zero or one"] \
        {[]} [mc "Any one character in the set"] \
        {[^]} [mc "Any one character not in the set"] \
        {|} [mc "Or"] \
        {^} [mc "Beginning of line"] \
        {$} [mc "End of line"] \
        {\m} [mc "Beginning of word"] \
        {\M} [mc "End of word"] \
        {[[:alpha:]]} [mc "A letter"] \
        {[[:digit:]]} [mc "A decimal digit"] \
        {[[:alnum:]]} [mc "An alphanumeric (letter or digit)"] \
        {[[:blank:]]} [mc "A space or tab character"] \
        $rationalnumberREpat_rep [mc "A rational number"] \
        $floatingpointnumberREpat_rep [mc "A floating point number"] \
        $scommRE_rep [mc "A Scilab comment"] \
        $scilabnameREpat [mc "A Scilab name"] \
        $dotcontlineRE_rep [mc "A Scilab continued line (with dots)"] \
    ]
}

proc insertregexpforfind {pat} {
# insert input argument $pat (a regexp pattern) in the "Search for"
# entry place of the find/replace dialog
    global find
    $find.u.f1.entry selection clear
    $find.u.f1.entry insert insert $pat
    # on windows at least, when clicking on the menubutton and maintaining
    # mouse button down while selecting the menu option, then the button
    # does not return to the flat state automatically (the buttonrelease
    # event must be directed to the wrong widget, which is the menu
    # attached to the menubutton) - therefore generate the adequate event
    # by hand
    event generate $find.u.f1.mb <ButtonRelease-1>
}

proc tagsandtagnamesforfind {} {
# return the following flat list:
#   {internaltagname1 tagnametodisplay1 internaltagname2 tagnametodisplay2 ... }
# with localized tagnametodisplay
# this list is used in the find/replace box to search in tagged text only
# only tags useful for a text search are returned here, not all tags of the
# textarea
    return [list \
        textquoted  [mc "QTXTCOLOR"] \
        rem2        [mc "REMCOLOR"] \
        xmltag      [mc "XMLCOLOR"] \
        number      [mc "NUMCOLOR"] \
        intfun      [mc "INTFCOLOR"] \
        command     [mc "COMMCOLOR"] \
        predef      [mc "PDEFCOLOR"] \
        libfun      [mc "LFUNCOLOR"] \
        userfun     [mc "USERFUNCOLOR"] \
        scicos      [mc "SCICCOLOR"] \
    ]
}

proc tagnamesforfind {} {
# return the ordered list of all tag names from proc tagsandtagnamesforfind
    set tagnames [list ]
    foreach {tag tagname} [tagsandtagnamesforfind] {
        lappend tagnames $tagname
    }
    return $tagnames
}

proc tagsforfind {onlyselected} {
# return the ordered list of tags from proc tagsandtagnamesforfind
# if $onlyselected == "onlyselected", then only tags selected in
#    the find dialog listbox are returned
# otherwise all tags are returned
    global find
    set tags [list ]
    set i 0
    foreach {tag tagname} [tagsandtagnamesforfind] {
        if {$onlyselected == "onlyselected"} {
            if {[$find.l.f4.f1.f2.f1.lb selection includes $i]} {
                lappend tags $tag
            }
        } else {
            lappend tags $tag
        }
        incr i
    }
    return $tags
}
