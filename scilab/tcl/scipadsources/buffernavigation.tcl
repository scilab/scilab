#####################################################################
#
# About the implementation of paned windows:
#
#   $textareaid
#       This is the unique identifier of the text widget displaying
#       the content of a given file. It is the same as $winopened
#
#   $pad.new$textareaid
#       Buffer name. This is the unique pathname of the text widget
#       displaying the content of a file. This is usually referred to
#       as $textarea, or $ta for short
#
#   A main paned window is created in mainwindow.tcl, whose name is
#   $pad.pw0. This panedwindow is the root of the paned windows tree
#   and is never destroyed
#
#   At the time a textarea must be packed (displayed in Scipad), it
#   is packed in an existing pane of an existing paned window (proc
#   packbuffer). More precisely, the textarea is packed into a frame
#   which was previously added as a pane of the panedwindow
#
#   On invocation of the Split or Tile commands, textareas are packed
#   in new panes (more precisely frames added as panes) of possibly
#   new paned windows. These new paned windows are nested in the
#   existing hierarchy (proc packnewbuffer). Nesting paned windows
#   is required because a panedwindow has a single orientation which
#   is common to all its panes
#
#   The hierarchy is partially coded in a global array: pwframe
#   This array is such that $pwframe($textarea) contains the widget
#   into which $textarea is packed. This is always a frame (that was
#   itself added as a pane in a panedwindow)
#   There is no variable fully describing the hierarchical tree: this
#   is managed by the packing algorithm of Tk. This tree can however
#   be retrieved in a nested list (proc getpwtree), which is used during
#   the merging process of the panedwindows, i.e. during removal of
#   superfluous nest levels of panedwindows
#
#   proc getpaneframename allows to retrieve the frame name in which
#   a textarea is currently packed. It returns "none" if the textarea
#   passed as argument is not visible (displayed). This allows for an
#   easy check of whether a textarea is displayed or not (proc isdisplayed)
#   or to get the total number of panes in Scipad (proc gettotnbpanes)
#
#   proc getpwname allows to retrieve the paned window name in which
#   a widget is packed. This widget is usually a frame (pane) of a
#   panedwindow in which a textarea is packed, but it can also be a
#   full panedwindow
#
#   proc createpaneframename constructs the frame name in which a
#   textarea is to be packed and stores this name in pwframe. If
#   possible, $pad.new$textareaid will be packed in a frame whose
#   name is $targetpw.f$textareaid
#
#   Example:
#   Consider a simple tiling of Scipad: one upper pane, and a lower
#   pane itself divided into two horizontal panes:
#
#       -------------------------------------------------
#       |                Upper pane                     |
#       |                                               |
#       ------------------------------------------------- 
#       |                Lower pane                     |
#       |-----------------------|-----------------------|
#       ||  Lower left pane     |   Lower right pane   ||
#       ||                      |                      ||
#       |-----------------------------------------------|
#       -------------------------------------------------
#
#   $pad.pw0 contains the whole thing. It has a vertical orientation and
#   contains two panes:
#      $pad.pw0.f1
#         This ends with f$textareaid, therefore this is a frame.
#         It is the frame added to $pad.pw0 as the first pane
#         It displays a textarea since it is a frame, and this frame is
#         $pad.new1 since $pwframe($pad.new1)=$pad.pw0.f1
#      $pad.pw0.pw1
#         This ends with pw$somenumber, therefore this is a panedwindow.
#         It was added as the second pane of $pad.pw0. It is not a frame
#         but a nested paned window whose orientation is horizontal. This
#         paned window contains again two panes:
#            $pad.pw0.pw1.f2
#               This is a frame containing $pad.new2 since
#               $pwframe($pad.new2)=$pad.pw0.pw1.f2
#            $pad.pw0.pw1.f3
#               This is again a frame containing $pad.new3 since
#               $pwframe($pad.new3)=$pad.pw0.pw1.f3
#
#   Note that in this example $pad.pw0.pw1.f$X contains $pad.new$Y
#   where $X == $Y, but this is not always the case
#
#####################################################################

proc packnewbuffer {textarea targetpw forcetitlebar {where ""}} {
# this packs a textarea buffer in a new pane that will be added in an existing panedwindow
    global pad FontSize menuFont

    # everything is packed in a frame whose name is provided by createpaneframename
    set tapwfr [createpaneframename $textarea $targetpw]

    # create frames for widget layout
    # this is the main frame that is added as a pane
    frame $tapwfr -borderwidth 2
    
    # this is for the top bar containing the pane title (file name)
    # and the close button
    frame $tapwfr.topbar
    button $tapwfr.clbutton -text [mc "Close"] -font $menuFont \
        -command "focustextarea $textarea; closecur yesnocancel"
    pack $tapwfr.clbutton  -in $tapwfr.topbar -side right  -expand 0 -fill none
    
    # this is for the text widget and the y scroll bar
    frame $tapwfr.top
    pack $tapwfr.top -side top -expand 1 -fill both
    
    # this is where the text widget is packed
    frame $tapwfr.topleft
    pack $tapwfr.topleft   -in $tapwfr.top    -side left   -expand 1 -fill both
    
    # this is where the y scrollbar is packed
    frame $tapwfr.topright
    pack  $tapwfr.topright -in $tapwfr.top    -side right  -expand 0 -fill both 
    
    # this is for the x scroll bar at the bottom of the pane
    frame $tapwfr.bottom
    pack $tapwfr.bottom                       -side bottom -expand 0 -fill both

    $targetpw add $tapwfr -minsize [expr $FontSize * 2]

    if {$targetpw == "$pad.pw0"} {
        pack $targetpw -side top -expand 1 -fill both
    }

    $targetpw paneconfigure $tapwfr -after $where

    scrollbar $tapwfr.yscroll -command "$textarea yview" -takefocus 0
    scrollbar $tapwfr.xscroll -command "$textarea xview" -takefocus 0 \
        -orient horizontal

    label $tapwfr.panetitle -font $menuFont
    bind $tapwfr.topbar    <ButtonRelease-1> "focustextarea $textarea"
    bind $tapwfr.panetitle <ButtonRelease-1> "focustextarea $textarea"
    bind $tapwfr.topbar    <Double-Button-1> "focustextarea $textarea; \
                                              $pad.filemenu.wind invoke 1"
    bind $tapwfr.panetitle <Double-Button-1> "focustextarea $textarea; \
                                              $pad.filemenu.wind invoke 1"
    pack $tapwfr.panetitle -in $tapwfr.topbar -expand 1 -fill none

    pack $textarea       -in $tapwfr.topleft  -side left   -expand 1 -fill both
    pack $tapwfr.yscroll -in $tapwfr.topright -side right  -expand 1 -fill y
    pack $tapwfr.xscroll -in $tapwfr.bottom   -side bottom -expand 1 -fill x

    if {[gettotnbpanes] > 1 || $forcetitlebar == 1} {
        pack $tapwfr.topbar -side top -expand 0 -fill both -in $tapwfr -before $tapwfr.top
    }

    $textarea configure -xscrollcommand "managescroll $tapwfr.xscroll"
    $textarea configure -yscrollcommand "managescroll $tapwfr.yscroll"
    $tapwfr.xscroll set [lindex [$textarea xview] 0] [lindex [$textarea xview] 1]
    $tapwfr.yscroll set [lindex [$textarea yview] 0] [lindex [$textarea yview] 1]

    spacesashesevenly $targetpw
}

proc packbuffer {textarea} {
# this packs a textarea buffer in an existing pane of an existing panedwindow
# this pane is always the current one
# the text widget is packed in the frame that contained the current textarea
    global pad pwframe

    pack forget [gettextareacur]
    set curtapwfr [getpaneframename [gettextareacur]]
    unset pwframe([gettextareacur])

    $curtapwfr.yscroll configure -command "$textarea yview"
    $curtapwfr.xscroll configure -command "$textarea xview"

    $curtapwfr.clbutton configure -command "closefile $textarea yesnocancel"

    bind $curtapwfr.topbar    <ButtonRelease-1> "focustextarea $textarea"
    bind $curtapwfr.panetitle <ButtonRelease-1> "focustextarea $textarea"
    bind $curtapwfr.topbar    <Double-Button-1> "focustextarea $textarea; \
                                                 $pad.filemenu.wind invoke 1"
    bind $curtapwfr.panetitle <Double-Button-1> "focustextarea $textarea; \
                                                 $pad.filemenu.wind invoke 1"

    pack $textarea -in $curtapwfr.topleft -side left -expand 1 -fill both

    $textarea configure -xscrollcommand "managescroll $curtapwfr.xscroll"
    $textarea configure -yscrollcommand "managescroll $curtapwfr.yscroll"

    $curtapwfr.xscroll set [lindex [$textarea xview] 0] [lindex [$textarea xview] 1]
    $curtapwfr.yscroll set [lindex [$textarea yview] 0] [lindex [$textarea yview] 1]

    set pwframe($textarea) $curtapwfr
}

proc montretext {textarea} {
# display a textarea in the current pane
# this textarea becomes the current one
    # prevent from displaying the same buffer in two or more panes
    # <TODO>: make use of peer text widgets (Tk 8.5) instead
    if {![isdisplayed $textarea]} {
        packbuffer $textarea
    }
    focustextarea $textarea
}

proc focustextarea {textarea} {
# Set all the settings such that $textarea becomes the current one
    global pad Scheme listoffile textareaid

    # clear the selection when leaving a buffer
    set oldta [gettextareacur]
    if {($oldta != $textarea) && [$oldta tag ranges sel] != ""} {
        $oldta tag remove sel 0.0 end
        selection clear
    }

    # set the new buffer as current
    settextareacur $textarea
    modifiedtitle $textarea
    focus $textarea
    keyposn $textarea
    set Scheme $listoffile("$textarea",language)
    schememenus $textarea
    highlighttextarea $textarea
    TextStyles $textarea
    set textareaid [scan $textarea $pad.new%d]
}

proc maximizebuffer {} {
    global pad listoftextarea

    disablemenuesbinds

    # Remove the existing tiling
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            destroypaneframe $ta
        }
    }

    # Pack the current buffer only
    packnewbuffer [gettextareacur] $pad.pw0 0
    highlighttextarea [gettextareacur]

    restoremenuesbinds
}

proc splitwindow {neworient} {
# split current window:
#    add a vertical pane if $neworient is "vertical"
#    add an horizontal pane if $neworient is "horizontal"
# splitting always starts from the current textarea, i.e
# everything appears to happen as if the *current* textarea
# is split
    global pad pwmaxid FontSize listoftextarea

    disablemenuesbinds

    # retrieve the orientation of the pane in which the current textarea is packed
    set tacur [gettextareacur]
    set tapwfr [getpaneframename $tacur]
    set pwname [getpwname $tapwfr]
    set curorient [$pwname cget -orient]

    # if there is a single existing pane, this prevents from creating an unnecessary
    # level of nested panedwindows
    if {$curorient != $neworient && [llength [$pwname panes]] == 1} {
        set curorient $neworient
        $pwname configure -orient $curorient
    }

    if {$curorient == $neworient} {
        # no need for a new panedwindow, just add a pane with one of the hidden
        # buffers inside, or an empty file otherwise

        # make sure that the possibly single pane before now shows its title bar
        pack $tapwfr.topbar -side top -expand 0 -fill both -in $tapwfr -before $tapwfr.top
        modifiedtitle $tacur "panesonly"

        # select the buffer to pack
        if {[llength $listoftextarea] > [gettotnbpanes]} {
            # if there is a hidden buffer, use it
            set newta $pad.new[getlasthiddentextareaid]
        } else {
            # otherwise create an empty textarea
            # <TODO>: use a peer text widget (Tk 8.5) and remove createnewtextarea
            set newta [createnewtextarea]
        }

        # pack it
        packnewbuffer $newta $pwname 1 $tapwfr
        focustextarea $newta

    } else {
        # a new panedwindow is needed

        # save position and geometry of current textarea, then remove it
        set ind [expr [lsearch [$pwname panes] $tapwfr] - 1]
        if {$ind != -1} {
            set aftopt [lindex [$pwname panes] $ind]
            set befopt ""
        } else {
            set aftopt ""
            set befopt [lindex [$pwname panes] 1]
        }
        set panewidth [winfo width $tapwfr]
        set paneheigth [winfo height $tapwfr]
        destroypaneframe $tacur nohierarchydestroy

        # create the new panedwindow, and pack it at the right position
        incr pwmaxid
        set newpw $pwname.pw$pwmaxid
        panedwindow $newpw -orient $neworient -opaqueresize true
        $pwname paneconfigure $newpw -after $aftopt -before $befopt \
            -width $panewidth -height $paneheigth -minsize [expr $FontSize * 2]

        # pack the previously existing textarea first, then a hidden buffer,
        # or an empty file otherwise
        # <TODO>: use a peer text widget here
        packnewbuffer $tacur $newpw 1
        focustextarea $tacur

        # select the buffer to pack
        if {[llength $listoftextarea] > [gettotnbpanes]} {
            # if there is a hidden buffer, use it
            set newta $pad.new[getlasthiddentextareaid]
        } else {
            # otherwise create an empty textarea
            set newta [createnewtextarea]
        }

        # pack it
        packnewbuffer $newta $newpw 1
        focustextarea $newta
    }

    restoremenuesbinds
}

proc createnewtextarea {} {
# this is a partial copy of proc filesetasnew that is just here to wait for 8.5 and peer text widgets
# <TODO>: get rid of this!
    global winopened listoffile
    global listoftextarea pad textareaid
    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",fullname) [mc "Untitled"]$winopened.sce
    set listoffile("$pad.new$winopened",displayedname) [mc "Untitled"]$winopened.sce
    set listoffile("$pad.new$winopened",new) 1
    set listoffile("$pad.new$winopened",thetime) 0
    set listoffile("$pad.new$winopened",language) "scilab"
    set listoffile("$pad.new$winopened",readonly) 0
    lappend listoftextarea $pad.new$winopened
    $pad.filemenu.wind add radiobutton -label $listoffile("$pad.new$winopened",displayedname) \
        -value $winopened -variable textareaid \
        -command "montretext $pad.new$winopened"
    newfilebind
    showinfo [mc "New Script"]
    selection clear
    return $pad.new$winopened
}

proc tileallbuffers {tileorient} {
    global pad listoftextarea

    disablemenuesbinds

    # Remove the existing tiling
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            destroypaneframe $ta
        }
    }

    # Configure the main panedwindow for the new orientation of panes
    $pad.pw0 configure -orient $tileorient

    # Pack the new panes
    if {[llength $listoftextarea] == 1} {
        set showtiletitle 0
    } else {
        set showtiletitle 1
    }
    foreach ta $listoftextarea {
        packnewbuffer $ta $pad.pw0 $showtiletitle
    }
    highlighttextarea [gettextareacur]
    updatepanestitles

    restoremenuesbinds
}

proc getpwname {tapwfr} {
# get the paned window name in which the widget $tapwfr is packed
# tapwfr is usually a frame (pane) of a panedwindow in which a textarea is
# packed, but it can also be a full panedwindow
# in fact this proc takes a widget name as an input and returns the winfo
# parent widget:
#          .wid1.wid2.wid3.wid4  -->  .wid1.wid2.wid3
# however, the implementation below is preferred since winfo parent returns
# errors when the parent has already been destroyed by proc destroypaneframe
# destroying an already destroyed widget causes no error and this simplifies
# the hierarchy destruction (see destroypaneframe)
    return [string replace $tapwfr [string last . $tapwfr] end]
}

proc getpaneframename {textarea} {
# get the frame name in which $textarea is currently packed
# or "none" if $textarea is not packed
    global pwframe
    if {[info exists pwframe($textarea)]} {
        return $pwframe($textarea)
    } else {
        return "none"
    }
}

proc gettafromwidget {w} {
# get the textarea name that is currently packed into $w
# (if $w is a frame, otherwise return "none")
    global pwframe listoftextarea
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            if {$pwframe($ta) == $w} {
                return $ta
            }
        }
    }
    return "none"
}

proc createpaneframename {textarea targetpw} {
# construct the frame name in which $textarea is to be packed
# and store this name in the global array pwframe
    global pad pwframe
    set id [scan $textarea $pad.new%d]
    set paneframename $targetpw.f$id
    while {[winfo exists $paneframename]} {
        incr id
        set paneframename $targetpw.f$id
    }
    set pwframe($textarea) $paneframename
    return $paneframename
}

proc destroypaneframe {textarea {hierarchy "destroyit"}} {
# forget about the frame and pane in which $textarea is packed
    global pad pwframe

    set tapwfr [getpaneframename $textarea]
    set pwname [getpwname $tapwfr]
    $pwname forget $tapwfr
    destroy $tapwfr

    if {$hierarchy == "destroyit"} {
        # the containing (parent) panedwindow itself must be destroyed if
        # there is no remaining panes, but don't destroy the main panedwindow
        # this check is made up to the main level of paned window
        while {[$pwname panes] == "" && $pwname != "$pad.pw0"} {
            destroy $pwname
            set pwname [getpwname $pwname]
        }
        mergepanedwindows $pwname
    }

    unset pwframe($textarea)
}

proc mergepanedwindows {pwname} {
# merge remaining panedwindows according to the new geometry
# because otherwise after many open/close of tiled buffers the grey
# borders do accumulate

    set pa [$pwname panes]
    if {[llength $pa] == 1} {
        # merge only when there is one single pane

        set pa [lindex $pa 0]
        if {[isapanedwindow $pa]} {
            # and only when this single pane contains a panedwindow (not a frame)

            $pwname configure -orient [$pa cget -orient]

            # create list of impacted widgets, i.e. those that need to be repacked
            set torepack [getpwtree $pa]

            # forget the old packing
            foreach w $torepack {
                destroywidget $w
            }

            # forget the superfluous level
            $pwname forget $pa
            destroy $pa

            # pack in the panedwindow one level above
            foreach w $torepack {
                repackwidget $w $pwname
            }
			spaceallsashesevenly
        }
    }
}

proc destroywidget {w} {
# recursive ancillary for proc mergepanedwindows

    if {[llength $w] == 2 && [isaframe [lindex $w 0]]} {
        # $w is a frame node list
        destroypaneframe [gettafromwidget [lindex $w 0]] nohierarchydestroy

    } elseif {[llength $w] == 3 && [isapanedwindow [lindex $w 0]]} {
        # $w is a panedwindow node list (see format in getpwtree)
        foreach sw [lindex $w 2] {
            destroywidget $sw
        }

    } else {
        # can't happen in principle
		tk_messageBox -message "Unexpected widget in proc destroywidget ($w): please report"
    }
}

proc repackwidget {w pwname} {
# recursive ancillary for proc mergepanedwindows
	global pwmaxid FontSize

    if {[llength $w] == 2 && [isaframe [lindex $w 0]]} {
        # $w is a frame node list, just pack it
        set ta [lindex $w 1]
        packnewbuffer $ta $pwname 1
        focustextarea $ta

    } elseif {[llength $w] == 3 && [isapanedwindow [lindex $w 0]]} {
        # $w is a panedwindow node list (see format in getpwtree)

        # insert a new paned window after last existing pane in current paned window
        set lastexistingpane [lindex [$pwname panes] end]
        incr pwmaxid
        set newpw $pwname.pw$pwmaxid
        panedwindow $newpw -orient [lindex $w 1] -opaqueresize true
        $pwname paneconfigure $newpw -after $lastexistingpane -minsize [expr $FontSize * 2]

		# repack anything that was previously in this paned window
        foreach sw [lindex $w 2] {
			repackwidget $sw $newpw
        }

    } else {
        # can't happen in principle
		tk_messageBox -message "Unexpected widget in proc repackwidget ($w): please report"
    }
}

proc getpwtree {root} {
# get the panedwindow hierarchical (sub-)tree in a nested list whose
# elements are either:
#    a list with two elements: { frame name textarea } if the child 
# is a frame;
# or a list of three elements: { pw_pathname orientation {sub-nodes} }
# in case the child is a paned window.
# The tree traversal starts from the panedwindow node $root

    set res {}
    foreach w [winfo child $root] {
        if {[isapanedwindow $w]} {
            lappend res [list $w [$w cget -orient] [getpwtree $w]]
        } elseif {[isaframe $w]} {
            lappend res [list $w [gettafromwidget $w]]
        } else {
            # sub-elements of a frame, such as panetitle, they are not
            # interesting parts of the tree - nothing to do, just go on
        }
    }
    return $res
}

proc isaframe {w} {
# check whether the widget passed as an argument is a frame or not
# return 1 if yes, or 0 otherwise
    return [isasomething $w f]
}

proc isapanedwindow {w} {
# check whether the widget passed as an argument is a paned window or not
# return 1 if yes, or 0 otherwise
    return [isasomething $w pw]
}

proc isasomething {w something} {
# check whether the widget passed as an argument is a "something" or not
# return 1 if yes, or 0 otherwise
# "something" can be "pw" for a paned window, or "f" for a frame
    set lastelt [lindex [split $w .] end]
    scan $lastelt $something%d somethingid
    if {[info exists somethingid]} {
        return 1
    } else {
        return 0
    }
}
proc isdisplayed {textarea} {
# check whether $textarea is currently packed, i.e. visible
# return 1 if yes, or 0 otherwise
    if {[getpaneframename $textarea] != "none"} {
        return 1
    } else {
        return 0
    }
}

proc gettotnbpanes {} {
# compute the total number of panes displaying a textarea
    global listoftextarea
    set tot 0
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {incr tot}
    }
    return $tot
}

proc getlistofpw {} {
# create the list of used paned windows
    global listoftextarea
    set pwfound {}
    foreach ta $listoftextarea {
        if {![isdisplayed $ta]} {continue}
        set tapwfr [getpaneframename $ta]
        set pwname [getpwname $tapwfr]
        if {[lsearch $pwfound $pwname] == -1} {
            lappend pwfound $pwname
        }
    }
    return [lsort $pwfound]
}

proc spaceallsashesevenly {} {
# space evenly all the sashes of all the existing paned windows
    foreach pw [getlistofpw] {
        spacesashesevenly $pw
    }
}

proc spacesashesevenly {pwname} {
# space evenly the sashes attached to the panes of paned window $pwname
    update
    set nbpanes [llength [$pwname panes]]
    set paneheight [expr [winfo height $pwname] / $nbpanes]
    set panewidth  [expr [winfo width  $pwname] / $nbpanes]
    for {set i 0} {$i < [expr $nbpanes - 1]} {incr i} {
        set paneposx [expr $panewidth  * ($i + 1)]
        set paneposy [expr $paneheight * ($i + 1)]
        $pwname sash place $i $paneposx $paneposy
    }
}

proc managescroll {scrbar a b} {
# this is only to add a catch to the command normally used
# this catch is required because the text widget may trigger scroll commands
# automatically when it is not packed in a pane,
# e.g. on $textarea configure -someoption
    catch {$scrbar set $a $b}
}

proc nextbuffer {} {
    global pad listoftextarea listoffile textareaid
    global FirstBufferNameInWindowsMenu
    set textarea [gettextareacur]
    set curbuf [expr [lsearch $listoftextarea $textarea] + $FirstBufferNameInWindowsMenu]
    incr curbuf
    set nbuf [expr [llength $listoftextarea] + $FirstBufferNameInWindowsMenu]
    if {$curbuf >= $nbuf} {
        set curbuf $FirstBufferNameInWindowsMenu
    }
    set textareaid [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf - $FirstBufferNameInWindowsMenu]]
    #keypress must replace the selection if buffers are switched
    set existsSel [[gettextareacur] tag nextrange sel 1.0]
    if {$existsSel != {}} {
        [gettextareacur] tag add sel [lindex $existsSel 0] [lindex $existsSel 1]
    }
}

proc prevbuffer {} {
   global pad listoftextarea listoffile textareaid
    global FirstBufferNameInWindowsMenu
    set textarea [gettextareacur]
    set curbuf [expr [lsearch $listoftextarea $textarea] + $FirstBufferNameInWindowsMenu]
    incr curbuf -1
    set nbuf [expr [llength $listoftextarea] + $FirstBufferNameInWindowsMenu - 1]
    if {$curbuf < $FirstBufferNameInWindowsMenu} {
        set curbuf $nbuf
    }
    set textareaid [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf - $FirstBufferNameInWindowsMenu]]
    #keypress must replace the selection if buffers are switched
    set existsSel [[gettextareacur] tag nextrange sel 1.0]
    if {$existsSel != {}} {
        [gettextareacur] tag add sel [lindex $existsSel 0] [lindex $existsSel 1]
    }
}

proc gotoline {} {
# Pop up the go to line dialog
    global pad textFont menuFont unklabel
    global physlogic linetogo curfileorfun funtogoto

    # Obtain all the functions defined in all the opened buffers, and create a list
    # containing function name, textarea where this function is defined and physical
    # line number where the function definition starts. Format is:
    # { funcname1 textarea1 funstartline1 funcname2 textarea2 funstartline2 ...}
    set funtogotolist {}
    set fundefs [getallfunsinalltextareas]
    foreach {ta fundefsinta} $fundefs {
        foreach {funcname funcline funstartline} $fundefsinta {
            if {$funcname != "0NoFunInBuf"} {
                lappend funtogotolist $funcname $ta $funstartline
            }
        }
    }
    # Set default values for the other the choices of the box
    if {![info exists linetogo]} {set linetogo 1}
    if {![info exists funtogoto]} {
        set funtogoto [list $unklabel 0 0]
    } else {
        # Do not propose a function defined in a closed buffer (in case the goto box is reopened
        # after a buffer has been closed)
        set stillexists "false"
        foreach {fn ta fsl} $funtogotolist {
            if {$fn == [lindex $funtogoto 0]} {
                set stillexists true
                break
            }
        }
        if {$stillexists == "false"} {
            set funtogoto [list $unklabel 0 0]
        }
    }

    # Create dialog geometry and pack widgets
    set gotln $pad.gotln
    catch {destroy $gotln}
    toplevel $gotln
    wm title $gotln [mc "Goto Line?"]
    setwingeom $gotln

    label $gotln.l1 -text [mc "Go to:"] -font $menuFont
    pack $gotln.l1 -anchor w -pady 5

    frame $gotln.f1
    eval "radiobutton $gotln.f1.rbut1 [bl "&logical line"]  \
        -variable physlogic -value \"logical\"  -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    eval "radiobutton $gotln.rbut2 [bl "&physical line"] \
        -variable physlogic -value \"physical\" -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    entry $gotln.f1.en1 -textvariable linetogo -width 8 -font $textFont
    pack $gotln.f1.rbut1 $gotln.f1.en1 -side left
    pack $gotln.f1 $gotln.rbut2 -anchor w

    frame $gotln.f2
    eval "radiobutton $gotln.f2.rbut3 [bl "in &function"]  \
        -variable curfileorfun -value \"function\"     -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    eval "radiobutton $gotln.rbut4 [bl "in &current file"] \
        -variable curfileorfun -value \"current_file\" -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    menubutton $gotln.f2.mb -text [lindex $funtogoto 0] -indicatoron 1 \
        -font $textFont

    menu $gotln.f2.mb.om1 -tearoff 0 -font $menuFont
    $gotln.f2.mb configure -menu $gotln.f2.mb.om1
    foreach {funname ta funstartline} $funtogotolist {
        $gotln.f2.mb.om1 add radiobutton -label $funname -font $menuFont \
            -variable funtogoto -value [list $funname $ta $funstartline] \
            -command "updatemenubutlabelgoto $gotln"
    }
    pack $gotln.f2.rbut3 $gotln.f2.mb -side left
    pack $gotln.f2 $gotln.rbut4 -anchor w

    button $gotln.ok -text [mc "OK"] -font $menuFont -width 8 \
        -command "dogotoline ; destroy $gotln"
    button $gotln.cancel -text [mc "Cancel"] -font $menuFont -width 8 \
        -command "destroy $gotln"
    pack $gotln.ok $gotln.cancel -side left -padx 5 -pady 5 -fill none -expand yes

    focus $gotln.f1.en1
    $gotln.f1.en1 selection range 0 end

    bind $gotln <Return> {if {[[winfo toplevel %W].ok cget -state] == "normal"} { \
                              dogotoline ; destroy [winfo toplevel %W] \
                          }}
    bind $gotln <Escape> {destroy [winfo toplevel %W]}

    bind $gotln <Alt-[fb $gotln.f1.rbut1]> \
        {[winfo toplevel %W].f1.rbut1 invoke}
    bind $gotln <Alt-[fb $gotln.rbut2]> \
        {[winfo toplevel %W].rbut2 invoke}
    bind $gotln <Alt-[fb $gotln.f2.rbut3]> \
        {[winfo toplevel %W].f2.rbut3 invoke}
    bind $gotln <Alt-[fb $gotln.rbut4]> \
        {[winfo toplevel %W].rbut4 invoke}

    # Default choices
    if {$physlogic    == ""} {$gotln.f1.rbut1 invoke}
    if {$curfileorfun == ""} {$gotln.f2.rbut3 invoke}

    # Validation of the entry to prevent the user to enter nasty things
    $gotln.f1.en1 configure -validate all -vcmd "updateOKbuttonstategoto $gotln %P"
}

proc updatemenubutlabelgoto {w} {
# Set value of -text for the menu button label of the go to line box
# It contains the currently selected function name to go to
    global funtogoto
    $w.f2.mb configure -text [lindex $funtogoto 0]
    # if a function has been selected it is likely the goto will use it, hence invoke this choice
    $w.f2.rbut3 invoke
    updateOKbuttonstategoto $w
}

proc updateOKbuttonstategoto {w {entryfieldvalue "not_given"}} {
# Prevent from launching gotos with inconsistent choices, and do some
# minimal controls on the user input
# Note: Scheme or $listoffile("$textarea",language) does not need to be checked since
# getallfunsintextarea deals with it and does not return functions from anything else
# than a Scilab scheme buffer
    global unklabel physlogic curfileorfun linetogo Scheme
    if {$entryfieldvalue == "not_given"} {set entryfieldvalue $linetogo}
    if {($curfileorfun == "current_file" && $physlogic == "logical" && $Scheme != "scilab") || \
        ($curfileorfun == "function" && [$w.f2.mb cget -text] == $unklabel) || \
        ($entryfieldvalue <= 0) || ![string is integer -strict $entryfieldvalue] } {
        $w.ok configure -state disabled
    } else {
        $w.ok configure -state normal
    }
    # Validation of the entry widget always succeeds, so that the textvariable
    # is updated. The validation result is made known by the OK button state
    return 1
}

proc dogotoline {} {
# Actually perform a go to line number ... taking into account all the possible
# choices from the user in the dialog box. These choices are known by the four
# global variables below (this is required and sufficient)

    global physlogic linetogo curfileorfun funtogoto

    if {$curfileorfun == "current_file"} {
        if {$physlogic == "physical"} {
            # go to physical line in current file
            set ta [gettextareacur]
            $ta mark set insert "$linetogo.0"
            catch {keyposn $ta}
            $ta see insert

        } else {
            # go to logical line in current file
            # this option enabled in the goto line dialog only for scilab scheme files
            set ta [gettextareacur]
            set endpos [$ta index end]
            set offset 0
            set curphysline 1.0
            set curlogicline $curphysline
            while {$linetogo != $curlogicline && [$ta compare $curphysline < $endpos]} {
                incr offset
                set curphysline [$ta index "$offset.0"]
                set contlines [countcontlines $ta 1.0 $curphysline]
                set curlogicline [$ta index "$curphysline - $contlines l"]
            }
            $ta mark set insert $curphysline
            catch {keyposn $ta}
            $ta see insert

        }

    } else {
        if {$physlogic == "physical"} {
            # go to physical line in function
            set textarea [lindex $funtogoto 1]
            set absoluteline [$textarea index "[lindex $funtogoto 2] + 1c + $linetogo lines -1l"]
            # check that the end of the function is after the position to go to
            set infun [whichfun $absoluteline $textarea]
            if {$infun == {}} {
                # target line is before function definition line - should never happen since
                # negative or null line numbers are forbidden in the entry widget!
                showinfo [mc "Outside of function definition"]
                set absoluteline [$textarea index [lindex $funtogoto 2]]
            } else {
                # target line is after the beginning of the function definition
                if {[lindex $infun 0] != [lindex $funtogoto 0]} {
                    # target line is after the end of the function definition,
                    # we will jump to the start of the function definition instead
                    showinfo [mc "Outside of function definition"]
                    set absoluteline [$textarea index [lindex $funtogoto 2]]
                } else {
                    # target position is between function and endfunction lines (inclusive)
                    set absoluteline [$textarea index "[lindex $funtogoto 2] + $linetogo lines -1l"]
                }
            }
            montretext $textarea
            $textarea mark set insert $absoluteline
            catch {keyposn $textarea}
            $textarea see insert

        } else {
            # go to logical line in function
            set textarea [lindex $funtogoto 1]
            set funstart [lindex $funtogoto 2]
            set infun [whichfun [$textarea index "$funstart + 1c"] $textarea]
            set offset 0
            # <TODO> This while loop could be improved (proc whichfun takes time to execute)
            # Its purpose is to make the line number in the buffer correspond to $linetogo
            while {$infun != "" && [lindex $infun 1] != $linetogo} {
                incr offset
                set infun [whichfun [$textarea index "$funstart + $offset l"] $textarea]
            }
            if {[lindex $infun 0] == [lindex $funtogoto 0]} {
                # target logical line is between function and endfunction
                set targetline [$textarea index "$funstart + $offset l"]
            } else {
                # target logical line is after the endfunction,
                # we will jump to the start of the function definition instead
                showinfo [mc "Outside of function definition"]
                set targetline [$textarea index $funstart]
            }
            montretext $textarea
            $textarea mark set insert $targetline
            catch {keyposn $textarea}
            $textarea see insert

        }
    }

}
