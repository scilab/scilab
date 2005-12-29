proc findinfiles {tosearchfor cas reg whword initdir globpat recursesearchindir searchforfilesonly} {
# search in the selected directory, in all the files that match
# the selected pattern, taking into account all the possible options (case
# type, regexp/standard, whole word, recurse search)
# if $searchforfilesonly is true, then files themselves are searched wrt
# to matching the glob pattern
    global allthematches cancelsearchflag openerrorfiles searchinfilesalreadyrunning

    set searchinfilesalreadyrunning 1

    # create a toplevel window where the matches will be displayed
    displaymatchresultswin
    disablesearchresultsbuttons
    set cancelsearchflag 0
    update

    # create the list of files to search in
    if {![direxists $initdir]} {
        tk_messageBox -title [mc "Directory access issue"] -icon warning \
            -message [mc "The directory you specified cannot be reached. The search won't bring any result."]
    }
    set openerrorfiles {}
    set filenames [getallfilenames $initdir $globpat $recursesearchindir]
    if {$openerrorfiles != {}} {
        set nlopenerrorfiles ""
        foreach fil $openerrorfiles {
            set nlopenerrorfiles "$nlopenerrorfiles$fil\n"
        }
        tk_messageBox -message [concat \
            [mc "Some directories or files could not be open and will be ignored during search.\n\
                 You might miss read access to them.\n\n\
                 The following ones will be ignored"] ":\n\n$nlopenerrorfiles"] \
            -icon warning -title [mc "Ignored files"] -type ok
    }

    if {!$searchforfilesonly} {
        # search for matches in each file
        set allthematches {}
        foreach filename $filenames {
            updatematchrestitle [concat [mc "Searching in file:"] "$filename"]
            set filematches [findinonefile $filename $tosearchfor $cas $reg $whword]
            foreach amatch $filematches {
                lappend allthematches $amatch
            }
            if {$cancelsearchflag} {break}
        }
    } else {
        # search for files themselves - display the matched files
        set allthematches {}
        foreach filename $filenames {
            updatematchrestitle [concat [mc "Searching in file:"] "$filename"]
            lappend allthematches [constructfilematchresult $filename]
            update
            if {$cancelsearchflag} {break}
        }
    }
    updatematchrestitle [concat [mc "End of file search:"] [llength $allthematches] [mc "matches found"]]
    enablesearchresultsbuttons
    set searchinfilesalreadyrunning 0
}

proc getallfilenames {initdir globpat recursesearch} {
# return all the file names matching $globpat in directory $initdir
    global tcl_platform cancelsearchflag

    set fnames {}

    # convert path separators to forward slashes - avoids glob side effects
    set initdir [file normalize $initdir]

    updatematchrestitle [concat [mc "Constructing file list:"] "$initdir"]

    # deal with files first
    foreach gp $globpat {
        set filematches [glob -nocomplain -directory "$initdir" -types {f} -- "$gp"]
        if {$filematches != {}} {
            # avoid duplicates (can happen for instance when $globpat is "* *.*")
            foreach fil $filematches {
                if {[lsearch -exact $fnames $fil] == -1} {
                    lappend fnames $fil
                }
            }
        }
    }

    # now deal with directories and the recurse case
    if {$recursesearch} {
        # keep the GUI responsive
        # (for cancel button, but also for all the Scipad functions)
        update
        set dirmatches [glob -nocomplain -directory "$initdir" -types {d} -- "*"]
        if {$dirmatches != {}} {
            foreach adir $dirmatches {
                eval lappend fnames [getallfilenames $adir $globpat $recursesearch]
                if {$cancelsearchflag} {break}
            }
        }
    }
    return $fnames
}

proc findinonefile {fname str cas reg whword} {
# find the string $str in file $fname (which must exist), and return
# match results
# to take advantage of the procs related to find/replace in opened buffers,
# a temporary text widget is created in the background, but not displayed.
# this text widget contains one line at a time of the file into which the
# search is performed. this allows to search into huge files without loading
# them first entirely in memory, at the expense of a slight performance
# slowdown
# output is a list of matches, each element (a match) being itself a list
# of 4 elements:
#    - the full file name in which the match has been found
#    - the three elements returned by proc doonesearch
# each match is displayed in a search results window

    global pad matchres cancelsearchflag openerrorfiles
    set filematchlist {}

    # open file
    if {[catch {open $fname r} fid] != 0} {
        # error on opening the file
        lappend openerrorfiles $fil
        return $filematchlist
    }

    # create a temporary text widget
    text $pad.fake 

    set linenumber 0

    # loop on file lines and search for matches on each single line
    while {[gets $fid line] >= 0} {
        $pad.fake insert 1.0 $line
        set listoflinematch [searchforallmatches $pad.fake $str $cas $reg 0 $whword]
        incr linenumber
        if {$listoflinematch != {}} {
            # at least one match has been found in the searched line of the file
            foreach amatch $listoflinematch {
                scan [lindex $amatch 0] "%d.%d" ypos xpos
                set pos "[expr $ypos + $linenumber - 1].$xpos"
                set len [lindex $amatch 1]
                set zero [lindex $amatch 2]
                lappend filematchlist [list $fname $pos $len $zero]
                $matchres.f1.resarea configure -state normal
                $matchres.f1.resarea insert end "$fname\($linenumber\):$line\n"
                $matchres.f1.resarea configure -state disabled
            }
        }
        # let the user see the progress, and keep the GUI responsive
        # (for cancel button, but also for all the Scipad functions)
        update
        $pad.fake delete 1.0 end
        if {$cancelsearchflag} {break}
    }

    # do the cleaning
    close $fid
    destroy $pad.fake

    return $filematchlist
}

proc constructfilematchresult {fname} {
# output is a match, i.e. a list of 4 elements:
#    - the full file name that matched the glob pattern
#    - 1.0 (index of first character in file)
#    - 0 (length of the match - to fool the display match procs)
#    - 0 (match not yet replaced - not used for find in files)
# each match is displayed in a search results window
    global pad matchres
    set pos "1.0"
    set len 0
    set zero 0
    set filematchlist [list $fname $pos $len $zero]
    $matchres.f1.resarea configure -state normal
    $matchres.f1.resarea insert end "$fname\n"
    $matchres.f1.resarea configure -state disabled
    return $filematchlist
}

proc cancelsearchinfiles {} {
# set a flag used to cancel search in files
    global cancelsearchflag
    set cancelsearchflag 1
}

proc displaymatchresultswin {} {
# show the match results window, or just clean it if already open
    global pad matchres textFont menuFont
    global SELCOLOR FGCOLOR BGCOLOR FOUNDTEXTCOLOR

    set matchres $pad.matchres

    if {[winfo exists $matchres]} {
        $matchres.f1.resarea configure -state normal
        $matchres.f1.resarea delete 1.0 end
        $matchres.f1.resarea configure -state disabled

    } else {
        catch {destroy $matchres}
        toplevel $matchres
        updatematchrestitle ""
        # prevent from closing using the upper right cross
        wm protocol $matchres WM_DELETE_WINDOW {#}
        wm resizable $matchres 1 1
        set myx 0
        set myw [winfo screenwidth $pad]
        set myh [expr [winfo screenheight $pad] / 5]
        set myy [expr [winfo screenheight $pad] *2/3]
        wm geometry $matchres "=$myw\x$myh+$myx+$myy"

        # this is for the text results area and the scrollbars
        frame $matchres.f1

        # results area
        text $matchres.f1.resarea -wrap none -exportselection 0 -font $textFont \
                -fg $FGCOLOR -bg $BGCOLOR -selectbackground $SELCOLOR 
        $matchres.f1.resarea configure -state disabled

        # scrollbars association with the results area
        scrollbar $matchres.f1.yscroll -command "$matchres.f1.resarea yview" -takefocus 0
        scrollbar $matchres.f1.xscroll -command "$matchres.f1.resarea xview" -takefocus 0 \
            -orient horizontal
        pack $matchres.f1.yscroll -side right  -expand 0 -fill y
        pack $matchres.f1.xscroll -side bottom -expand 0 -fill x
        pack $matchres.f1.resarea -side left   -expand 1 -fill both
        $matchres.f1.resarea configure -xscrollcommand "$matchres.f1.xscroll set"
        $matchres.f1.resarea configure -yscrollcommand "$matchres.f1.yscroll set"
        $matchres.f1.xscroll set [lindex [$matchres.f1.resarea xview] 0] [lindex [$matchres.f1.resarea xview] 1]
        $matchres.f1.yscroll set [lindex [$matchres.f1.resarea yview] 0] [lindex [$matchres.f1.resarea yview] 1]

        # command buttons
        frame $matchres.f2
        eval "button $matchres.f2.buttonPrev [bl "&Previous"] \
            -command \"openprevmatch $matchres.f1.resarea\" \
            -width 10 -height 1 -font $menuFont "
        eval "button $matchres.f2.buttonNext [bl "&Next"] \
            -command \"opennextmatch $matchres.f1.resarea\" \
            -width 10 -height 1 -font $menuFont "
        eval "button $matchres.f2.buttonClose [bl "&Close"] \
            -command \"destroy $matchres\" \
            -width 10 -height 1 -font $menuFont "
        eval "button $matchres.f2.buttonCancel [bl "Cance&l"] \
            -command \"cancelsearchinfiles\" \
            -width 10 -height 1 -font $menuFont "
        pack $matchres.f2.buttonPrev $matchres.f2.buttonNext \
             $matchres.f2.buttonClose $matchres.f2.buttonCancel -side left \
             -padx 6 -pady 2

        # make all this visible (order matters wrt to clipping on external resizing)
        pack $matchres.f2 -side bottom
        pack $matchres.f1 -side top -expand 1 -fill both

        # buttons bindings
        bind $matchres <Alt-[fb $matchres.f2.buttonPrev]> "openprevmatch $matchres.f1.resarea"
        bind $matchres <Alt-[fb $matchres.f2.buttonNext]> "opennextmatch $matchres.f1.resarea"
        bind $matchres <Alt-[fb $matchres.f2.buttonClose]> "after 0 destroy $matchres"
        bind $matchres <Alt-[fb $matchres.f2.buttonCancel]> "cancelsearchinfiles"

        # define the behaviour of the results area
        $matchres.f1.resarea tag configure foundtext -background $FOUNDTEXTCOLOR
        bind $matchres.f1.resarea <Double-Button-1> { openpointedmatch %W %x %y ; break }
        bind $matchres.f1.resarea <<Modified>> {break}
        # prevent from unwanted Text class bindings from triggering
        bind $matchres.f1.resarea <Button-3> {break}
        bind $matchres.f1.resarea <Shift-Button-3> {break}
        bind $matchres.f1.resarea <Control-Button-3> {break}
    }
}

proc disablesearchresultsbuttons {} {
# disable buttons of the search results window
# (used during search)
    global matchres
    $matchres.f2.buttonNext   configure -state disabled
    $matchres.f2.buttonPrev   configure -state disabled
    $matchres.f2.buttonClose  configure -state disabled
    $matchres.f2.buttonCancel configure -state normal
}

proc enablesearchresultsbuttons {} {
# re-enable buttons of the search results window
# (used after search end or cancel)
    global matchres
    $matchres.f2.buttonNext   configure -state normal
    $matchres.f2.buttonPrev   configure -state normal
    $matchres.f2.buttonClose  configure -state normal
    $matchres.f2.buttonCancel configure -state disabled
}

proc updatematchrestitle {tit} {
# change the title of the match results window to $tit preceded by
# the translation of "Scipad search results" in the current locale
    global matchres
    wm title $matchres "[mc "Scipad search results"] - $tit"
}

proc openpointedmatch {w x y} {
# display the match that was found in a file
# the match is selected by mouse click
    set posinresarea [$w index "@$x,$y"]
    # double-clicking on the last (empty) line is ignored
    if {[$w compare $posinresarea != [$w index "end-1c"]]} {
        openamatch $w $posinresarea
    }
}

proc opennextmatch {w} {
# display the match that was found in a file
# the match is selected by button next
    set curmatch [$w tag nextrange foundtext 1.0]
    if {$curmatch == ""} {
        set ypos 0
    } else {
        scan $curmatch "%d.%d" ypos xpos
    }
    set posinresarea "[expr $ypos + 1].0"
    if {$posinresarea == [$w index "end - 1l linestart"]} {
        set posinresarea 1.0
    }
    if {[$w get $posinresarea] != "\n"} {
        openamatch $w $posinresarea
    }
}

proc openprevmatch {w} {
# display the match that was found in a file
# the match is selected by button previous
    set curmatch [$w tag nextrange foundtext 1.0]
    if {$curmatch == ""} {
        set ypos 1
    } else {
        scan $curmatch "%d.%d" ypos xpos
    }
    set posinresarea "[expr $ypos - 1].0"
    if {$posinresarea == "0.0"} {
        set posinresarea [$w index "end - 2l linestart"]
    }
    if {[$w get $posinresarea] != "\n"} {
        openamatch $w $posinresarea
    }
}

proc openamatch {w posinresarea} {
# display the match that was found in a file
# posinresarea is the text widget index position in the results window
# of the match to open
# the file is opened if not already open, the cursor is placed at the match
# position, and the match is highlighted
    global pad allthematches

    wm deiconify $pad

    # highlight the selected match in the search results list
    $w tag remove foundtext 1.0 end
    $w tag add foundtext "$posinresarea linestart" "$posinresarea lineend"
    $w see $posinresarea
    
    # retrieve match coordinates
    scan $posinresarea "%d.%d" linenum xpos
    set thematch [lindex $allthematches [expr $linenum - 1]]

    # open the file (or display it if already open), and highlight the match
    if {[openfileifexists [lindex $thematch 0]]} {
        set ta [gettextareacur]
        set matchstart [lindex $thematch 1]
        $ta mark set insert $matchstart
        keyposn $ta
        $ta see $matchstart
        $ta tag remove sel 1.0 end
        $ta tag remove foundtext 1.0 end
        $ta tag add foundtext $matchstart "$matchstart + [lindex $thematch 2] char"

        # <TODO>: these bindings are required to remove the foundtext tag after having
        #         been highlighted. Once set, they will live in the textarea forever,
        #         and there will be one such binding added for each successful search!!
        bind $ta <KeyPress>    {+%W tag remove foundtext 1.0 end}
        bind $ta <ButtonPress> {+%W tag remove foundtext 1.0 end}
        bind $ta <Button-1>    {+%W tag remove foundtext 1.0 end}
    } else {
        # file was deleted between the time when the match was found and the
        # time when it was clicked in the dialog
        # depending on the user's answer, a new file is created or not, anyway
        # there is nothing to highlight
    }
}
