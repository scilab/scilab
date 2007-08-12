#####################################################################
#
# About the implementation of paned windows:
#
#   $textareaid
#       This is the unique identifier of the text widget displaying
#       the content of a given file.
#       When a new textarea is created, it is given the $winopened
#       value in $textareaid
#       Peer text widgets are just normal textareas, with their
#       textareaid value
#
#   $pad.new$textareaid
#       Buffer name. This is the unique pathname of the text widget
#       displaying the content of a file. This is usually referred to
#       as $textarea, or $ta for short
#       Peer text widgets are just normal textareas
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

proc packnewbuffer {textarea targetpw forcetitlebar {whereafter ""} {wherebefore ""}} {
# this packs a textarea buffer in a new pane that will be added in an existing panedwindow
    global pad textfontsize menuFont linenumbersmargins wordWrap
    global Tk85

    # everything is packed in a frame whose name is provided by createpaneframename
    set tapwfr [createpaneframename $textarea $targetpw]

    # create frames for widget layout
    # this is the main frame that is added as a pane
    frame $tapwfr -borderwidth 2

    # this is for the top bar containing the pane title (file name)
    # and the hide and close buttons
    frame $tapwfr.topbar
    set bestwidth [mcmaxra "Hide" \
                           "Close"]
    button $tapwfr.clbutton -text [mc "Close"] \
        -font $menuFont -width $bestwidth \
        -command "focustextarea $textarea; closecurtile yesnocancel"
    pack $tapwfr.clbutton  -in $tapwfr.topbar -side right  -expand 0 -fill none
    button $tapwfr.hibutton -text [mc "Hide"] -font $menuFont \
        -width $bestwidth \
        -command "hidetext $textarea"
    pack $tapwfr.hibutton  -in $tapwfr.topbar -side right  -expand 0 -fill none

    # this is for the text widget, its margin, and the y scroll bar
    frame $tapwfr.top
    pack $tapwfr.top -side top -expand 1 -fill both

    # this is where the text widget and its margin are packed
    frame $tapwfr.topleft
    pack $tapwfr.topleft   -in $tapwfr.top    -side left   -expand 1 -fill both

    # this is where the y scrollbar is packed
    frame $tapwfr.topright
    pack  $tapwfr.topright -in $tapwfr.top    -side right  -expand 0 -fill both 

    # this is for the x scroll bar at the bottom of the pane
    if {$wordWrap == "none"} {
        frame $tapwfr.bottom
        pack $tapwfr.bottom                   -side bottom -expand 0 -fill both
    }

    $targetpw add $tapwfr -minsize [expr {$textfontsize * 2}]
    if {$Tk85} {
        $targetpw paneconfigure $tapwfr -stretch always
    }

    if {$targetpw == "$pad.pw0"} {
        pack $targetpw -side top -expand 1 -fill both
    }

    $targetpw paneconfigure $tapwfr -after $whereafter -before $wherebefore

    scrollbar $tapwfr.yscroll -command "$textarea yview" -takefocus 0
    if {$wordWrap == "none"} {
        scrollbar $tapwfr.xscroll -command "$textarea xview" -takefocus 0 \
            -orient horizontal
    }

    label $tapwfr.panetitle -font $menuFont
    bind $tapwfr.topbar    <ButtonRelease-1> "focustextarea $textarea"
    bind $tapwfr.panetitle <ButtonRelease-1> "focustextarea $textarea"
    bind $tapwfr.topbar    <Double-Button-1> "focustextarea $textarea; \
                                              $pad.filemenu.wind invoke 1"
    bind $tapwfr.panetitle <Double-Button-1> "focustextarea $textarea; \
                                              $pad.filemenu.wind invoke 1"
    bind $tapwfr.topbar    <Button-2>        {switchbuffersinpane %W}
    bind $tapwfr.panetitle <Button-2>        {switchbuffersinpane %W}
    bind $tapwfr.panetitle <Enter> "update_bubble_panetitle enter %W \[winfo pointerxy $pad\] $textarea"
    bind $tapwfr.panetitle <Leave> "update_bubble_panetitle leave %W \[winfo pointerxy $pad\] $textarea"
    bind $tapwfr.clbutton  <Enter> "update_bubble enter %W \[winfo pointerxy $pad\] \[mc \"Close this tile\"\]"
    bind $tapwfr.clbutton  <Leave> "update_bubble leave %W \[winfo pointerxy $pad\] \[mc \"Close this tile\"\]"
    bind $tapwfr.hibutton  <Enter> "update_bubble enter %W \[winfo pointerxy $pad\] \[mc \"Close this tile, keep content\"\]"
    bind $tapwfr.hibutton  <Leave> "update_bubble leave %W \[winfo pointerxy $pad\] \[mc \"Close this tile, keep content\"\]"
    pack $tapwfr.panetitle -in $tapwfr.topbar -expand 1 -fill none

    pack $textarea       -in $tapwfr.topleft  -side left   -expand 1 -fill both
    pack $tapwfr.yscroll -in $tapwfr.topright -side right  -expand 1 -fill y
    if {$wordWrap == "none"} {
        pack $tapwfr.xscroll -in $tapwfr.bottom   -side bottom -expand 1 -fill x
    }

    if {[gettotnbpanes] > 1 || $forcetitlebar == 1} {
        pack $tapwfr.topbar -side top -expand 0 -fill both -in $tapwfr -before $tapwfr.top
    }

    $textarea configure -xscrollcommand "managescroll $tapwfr.xscroll"
    $textarea configure -yscrollcommand "managescroll $tapwfr.yscroll"

    if {$linenumbersmargins != "hide"} {
        addlinenumbersmargin $textarea
    }

    spacesashesevenly $targetpw
}

proc packbuffer {textarea} {
# this packs a textarea buffer in an existing pane of an existing panedwindow
# this pane is always the current one
# the text widget is packed in the frame that contained the current textarea
    global pad pwframe wordWrap

    # remove scrollbars commands so that an unpacked textarea does not later
    # update the scrollbar, which depending on the order these commands are
    # launched, can lead to wrong scrollbars sizes and scrollbars flashes
    if {[winfo exists [gettextareacur]]} {
        [gettextareacur] configure -xscrollcommand ""
        [gettextareacur] configure -yscrollcommand ""
    }

    pack forget [gettextareacur]
    set curtapwfr [getpaneframename [gettextareacur]]
    unset pwframe([gettextareacur])

    $curtapwfr.yscroll configure -command "$textarea yview"
    if {$wordWrap == "none"} {
        $curtapwfr.xscroll configure -command "$textarea xview"
    }

    $curtapwfr.clbutton configure -command "focustextarea $textarea; closecurtile yesnocancel"
    $curtapwfr.hibutton configure -command "hidetext $textarea"

    bind $curtapwfr.topbar    <ButtonRelease-1> "focustextarea $textarea"
    bind $curtapwfr.panetitle <ButtonRelease-1> "focustextarea $textarea"
    bind $curtapwfr.topbar    <Double-Button-1> "focustextarea $textarea; \
                                                 $pad.filemenu.wind invoke 1"
    bind $curtapwfr.panetitle <Double-Button-1> "focustextarea $textarea; \
                                                 $pad.filemenu.wind invoke 1"
    bind $curtapwfr.topbar    <Button-2>        {switchbuffersinpane %W}
    bind $curtapwfr.panetitle <Button-2>        {switchbuffersinpane %W}
    bind $curtapwfr.panetitle <Enter> "update_bubble_panetitle enter %W \[winfo pointerxy $pad\] $textarea"
    bind $curtapwfr.panetitle <Leave> "update_bubble_panetitle leave %W \[winfo pointerxy $pad\] $textarea"
    bind $curtapwfr.clbutton  <Enter> "update_bubble enter %W \[winfo pointerxy $pad\] \[mc \"Close this tile\"\]"
    bind $curtapwfr.clbutton  <Leave> "update_bubble leave %W \[winfo pointerxy $pad\] \[mc \"Close this tile\"\]"
    bind $curtapwfr.hibutton  <Enter> "update_bubble enter %W \[winfo pointerxy $pad\] \[mc \"Close this tile, keep content\"\]"
    bind $curtapwfr.hibutton  <Leave> "update_bubble leave %W \[winfo pointerxy $pad\] \[mc \"Close this tile, keep content\"\]"

    pack $textarea -in $curtapwfr.topleft -side left -expand 1 -fill both

    $textarea configure -xscrollcommand "managescroll $curtapwfr.xscroll"
    $textarea configure -yscrollcommand "managescroll $curtapwfr.yscroll"

    set pwframe($textarea) $curtapwfr
}

proc montretext {textarea} {
# old name of showtext, left for compatibility (backporting)
# with plotprofile.sci of BUILD_4 Scilab
  showtext $textarea
}

proc showtext {textarea} {
# if $textarea is not currently visible, pack it in the current pane,
# and make this textarea the current one
# if $textarea is already visible in some other pane than the
# current one, simply switch to this textarea
# WARNING: this proc is called from outside Scipad (plotprofile.sci)
#          changes here should be assessed with their full consequences...

    if {![isdisplayed $textarea]} {
        packbuffer $textarea
    }

    focustextarea $textarea
    backgroundcolorizeuserfun
}

proc hidetext {textarea} {
# hide a textarea currently packed
# this is different from closing a tile since hiding keeps the textarea
# entry in the windows menu (and in listoftexarea)
# it is purely an unpacking action
    global pad listoftextarea pwframe tileprocalreadyrunning

    if {$tileprocalreadyrunning} {return}
    disablemenuesbinds

    # unpack the textarea
    destroypaneframe $textarea

    # place as current textarea the last one that is already visible
    $pad.filemenu.wind invoke [getlastvisibletextareamenuind]

    # remove tile title if there is a single pane
    if {[gettotnbpanes] == 1} {
        set visibletapwfr [lindex [array get pwframe] 1]
        pack forget $visibletapwfr.topbar
    }

    restoremenuesbinds
}

proc focustextarea {textarea} {
# Set all the settings such that $textarea becomes the current one
    global pad Scheme ColorizeIt listoffile textareaid
    global buffermodifiedsincelastsearch

    # clear the selection when leaving a buffer - check first that the
    # textarea still exists because it might have been destroyed when
    # focustextarea is called after closure of the current file
    set oldta [gettextareacur]
    if {[winfo exists $oldta]} {
        if {($oldta != $textarea) && [gettaselind $oldta any] != ""} {
            $oldta tag remove sel 1.0 end
        }
    }

    # set the new buffer as current
    settextareacur $textarea
    modifiedtitle $textarea
    focus $textarea
    keyposn $textarea
    set Scheme $listoffile("$textarea",language)
    set ColorizeIt $listoffile("$textarea",colorize)
    schememenus $textarea
    highlighttextarea $textarea
    TextStyles $textarea
    set textareaid [gettaidfromwidgetname $textarea]
    set buffermodifiedsincelastsearch true
}

proc maximizebuffer {} {
    global pad listoftextarea tileprocalreadyrunning

    if {$tileprocalreadyrunning} {return}
    disablemenuesbinds

    set curta [gettextareacur]

    # Remove the existing tiling
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            destroypaneframe $ta
        }
    }

    # Pack the current buffer only
    packnewbuffer $curta $pad.pw0 0
    highlighttextarea $curta

    restoremenuesbinds
}

proc splitwindow {neworient tatopack {splitmode tile}} {
# split current window:
#    add a vertical pane if $neworient is "vertical"
#    add an horizontal pane if $neworient is "horizontal"
# splitting always starts from the current textarea, i.e
# everything appears to happen as if the *current* textarea
# is split
# if $tatopack != "" then the textarea $tatopack will be packed in the new
# pane, otherwise:
#    - with Tk 8.5: behaviour depends on $splitmode as follows:
#        - if $splitmode == "file", a new peer is created
#        - if $splitmode == "tile", the last hidden textarea is used, and if
#          there is none an empty textarea is created (Tk 8.4 behaviour)
#    - with Tk 8.4: the last hidden textarea is used, and if there is none
#      an empty textarea is created
# $splitmode is only used with Tk 8.5 :
    global pad pwmaxid textfontsize listoftextarea tileprocalreadyrunning
    global Tk85

    if {$tileprocalreadyrunning} {return}
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
        # no need for a new panedwindow, just add a pane with the textarea whose
        # name was provided as argument, or:
        #     a hidden buffer, or an empty file if there is none (Tk 8.4)
        #     create and pack a new peer of the current textarea (Tk 8.5)

        # make sure that the possibly single pane before now shows its title bar
        pack $tapwfr.topbar -side top -expand 0 -fill both -in $tapwfr -before $tapwfr.top
        modifiedtitle $tacur "panesonly"

        # select the buffer to pack
        if {$tatopack != ""} {
            # if the name of the textarea to pack was provided, use it
            # this happens with the commands from the file menu
            set newta $tatopack
        } else {
            # the name of of the textarea to pack was not provided,
            # this happens with the commands from the windows menu
            if {$Tk85 && $splitmode == "file"} {
                # create a peer text widget
                set newta [createpeertextwidget $tacur]
            } else {
                # Tk 8.4 case, or $splitmode == "tile" in Tk 8.5
                if {[llength $listoftextarea] > [gettotnbpanes]} {
                    # if there is a hidden buffer, use it
                    set newta $pad.new[getlasthiddentextareaid]
                } else {
                    # otherwise create an empty textarea
                    set newta [createnewemptytextarea]
                }
            }
        }

        # pack it
        packnewbuffer $newta $pwname 1 $tapwfr
        focustextarea $newta

    } else {
        # a new panedwindow is needed

        # save position and geometry of current textarea, then remove it
        set ind [expr {[lsearch [$pwname panes] $tapwfr] - 1}]
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
            -width $panewidth -height $paneheigth -minsize [expr {$textfontsize * 2}]

        # pack the previously existing textarea first, then the textarea whose
        # name was provided as argument, or:
        #     a hidden buffer, or an empty file if there is none (Tk 8.4)
        #     create and pack a new peer of the current textarea (Tk 8.5)

        packnewbuffer $tacur $newpw 1
        focustextarea $tacur

        # select the buffer to pack
        if {$tatopack != ""} {
            # if the name of the textarea to pack was provided, use it
            # this happens with the commands from the file menu
            set newta $tatopack
        } else {
            # the name of of the textarea to pack was not provided,
            # this happens with the commands from the windows menu
            if {$Tk85 && $splitmode == "file"} {
                # create a peer text widget
                set newta [createpeertextwidget $tacur]
            } else {
                # Tk 8.4 case, or $splitmode == "tile" in Tk 8.5
                if {[llength $listoftextarea] > [gettotnbpanes]} {
                    # if there is a hidden buffer, use it
                    set newta $pad.new[getlasthiddentextareaid]
                } else {
                    # otherwise create an empty textarea
                    set newta [createnewemptytextarea]
                }
            }
        }

        # pack it
        packnewbuffer $newta $newpw 1
        focustextarea $newta
    }

    updatepanestitles
    backgroundcolorizeuserfun
    restoremenuesbinds
}

proc createnewemptytextarea {} {
# this is a partial copy of proc filesetasnew
# it creates a new empty textarea
    global winopened listoffile
    global listoftextarea pad

    # ensure that the cursor is changed to the default cursor
    event generate [gettextareacur] <Leave>

    incr winopened

    dupWidgetOption [gettextareacur] $pad.new$winopened

    set listoffile("$pad.new$winopened",fullname) [mc "Untitled"]$winopened.sce
    set listoffile("$pad.new$winopened",displayedname) [mc "Untitled"]$winopened.sce
    set listoffile("$pad.new$winopened",new) 1
    set listoffile("$pad.new$winopened",thetime) 0
    set listoffile("$pad.new$winopened",language) "scilab"
    setlistoffile_colorize "$pad.new$winopened" ""
    set listoffile("$pad.new$winopened",readonly) 0
    set listoffile("$pad.new$winopened",redostackdepth) 0
    set listoffile("$pad.new$winopened",progressbar_id) ""
    lappend listoftextarea $pad.new$winopened

    addwindowsmenuentry $winopened $listoffile("$pad.new$winopened",displayedname)

    newfilebind
    showinfo [mc "New script"]
    return $pad.new$winopened
}

proc createpeertextwidget {ta} {
# create a new peer text widget of $ta
    global winopened listoffile
    global listoftextarea pad

    # ensure that the cursor is changed to the default cursor
    event generate [gettextareacur] <Leave>

    # the "original" text widget receives peer id <1>, so that
    # any text widget that has peers has a peer id <X>
    # note that this should however not be used directly in order
    # to detect which textareas are or have peers - use of the
    # if {[getpeerlist $ta] == {}} contraption below is the
    # recommended way to know this
    if {[getpeerlist $ta] == {}} {
        # the peer to create is the first one - add a peerid tag to the
        # existing textarea
        set listoffile("$ta",displayedname) [appendpeerid $listoffile("$ta",displayedname) 1]
    } else {
        # nothing to do on the existing textarea name, the peer to create
        # is not the first one, there is already a buffer with the <1> peer
        # id tag
    }

    incr winopened

    set newta [$ta peer create $pad.new$winopened]

    eval "$newta configure [nondefOpts $ta]"

    # create peer displayedname
    set dispname [appendpeerid $listoffile("$ta",displayedname) \
            [expr {[llength [getpeerlist $ta]] + 1}]]

    set listoffile("$newta",fullname) $listoffile("$ta",fullname)
    set listoffile("$newta",displayedname) $dispname
    set listoffile("$newta",new) $listoffile("$ta",new)
    set listoffile("$newta",thetime) $listoffile("$ta",thetime)
    set listoffile("$newta",language) $listoffile("$ta",language)
    set listoffile("$newta",colorize) $listoffile("$ta",colorize)
    set listoffile("$newta",readonly) $listoffile("$ta",readonly)
    set listoffile("$newta",redostackdepth) $listoffile("$ta",redostackdepth)
    set listoffile("$newta",progressbar_id) $listoffile("$ta",progressbar_id)
    lappend listoftextarea $newta

    addwindowsmenuentry $winopened $listoffile("$pad.new$winopened",displayedname)

    newfilebind
    showinfo [mc "New view on the same file"]

    return $newta
}

proc tileallbuffers {tileorient} {
    global pad listoftextarea tileprocalreadyrunning

    if {$tileprocalreadyrunning} {return}
    disablemenuesbinds

    # Remove the existing tiling
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            destroypaneframe $ta
        }
    }

    # Configure the main panedwindow for the new orientation of panes
    $pad.pw0 configure -orient $tileorient

    # Decide whether the tile title should be displayed
    if {[llength $listoftextarea] == 1} {
        set showtiletitle 0
    } else {
        set showtiletitle 1
    }

    # Arrange the list of textareas in such a way that the current one
    # will be packed first
    set tacur [gettextareacur]
    set talisttopack [shiftlistofta $listoftextarea $tacur]

    # Pack the new panes
    foreach ta $talisttopack {
        packnewbuffer $ta $pad.pw0 $showtiletitle
    }
    highlighttextarea $tacur
    updatepanestitles

    backgroundcolorizeuserfun
    restoremenuesbinds
}

proc shiftlistofta {intalist ta} {
# arrange the list $intalist in such a way that element $ta
# comes first. Example: If $ta is the 3rd one:
#     input  ($intalist) : a b c d e f
#     output ($outtalist): c d e f a b
# $ta must be an element of $intalist
    set posta [lsearch -sorted $intalist $ta]
    set outtalist [lrange $intalist $posta end]
    set eltstomove [lrange $intalist 0 [expr {$posta - 1}]]
    foreach elt $eltstomove {
        lappend outtalist $elt
    }
    return $outtalist
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

proc gettaidfromwidgetname {w} {
# parse the widget name $w to extract the textarea id, which is the trailing
# number in the widget name
# $w is supposed (this is not checked) to be a text widget (named $pad.newX)
# return value is number X
    global pad
    set taid ""
    scan $w $pad.new%d taid
    if {$taid == ""} {
        tk_messageBox -message "Unexpected widget was received by proc gettaidfromwidgetname: $w"
    }
    return $taid
}

proc createpaneframename {textarea targetpw} {
# construct the frame name in which $textarea is to be packed
# and store this name in the global array pwframe
    global pad pwframe
    set id [gettaidfromwidgetname $textarea]
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
# i.e. this destroys the packing of $textarea without destroying
# the textarea itself
# the optional argument $hierarchy might be "destroyit" or
# "nohierarchydestroy", the former being the normal mode, and the
# latter being used when repacking to tell this proc not to destroy
# the conatining panedwindow itself if there is no remaining pane
    global pad pwframe

    # remove scrollbars commands so that an unpacked textarea does not
    # update the scrollbar, which depending on the order these commands
    # are launched, can lead to wrong scrollbars sizes and scrollbars flashes
    $textarea configure -xscrollcommand ""
    $textarea configure -yscrollcommand ""

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
    }

    unset pwframe($textarea)

    if {$hierarchy == "destroyit"} {
        mergepanedwindows1 $pwname
        mergepanedwindows2 $pwname
    }
}

proc mergepanedwindows1 {pwname} {
# merge remaining panedwindows according to the new geometry
# because otherwise after many open/close of tiled buffers the grey
# borders do accumulate
#
# Step 1: this proc merges panedwindows in the sense that it removes
# superfluous levels of panedwindow nesting
#
# Example:
#   If getpwtree outputs:
# {.scipad.pw0.pw1 horizontal {{.scipad.pw0.pw1.f2 .scipad.new2}
#                              {.scipad.pw0.pw1.f3 .scipad.new3}}}
#   then it means that there is a single panedwindow pw1 that is
# packed in $pad.pw0. This level pw1 can (must) be removed and this
# is what this proc is performing by repacking directly in pw0 all
# what was in pw1

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


proc mergepanedwindows2 {pwname} {
# merge remaining panedwindows according to the new geometry
# because otherwise after many open/close of tiled buffers the grey
# borders do accumulate
#
# Step 2: this proc merges panedwindows in the sense that it removes
# superfluous panedwindows
#
# Example:
#   If getpwtree outputs:
# {.scipad.pw0.pw1 horizontal {{.scipad.pw0.pw1.f2 .scipad.new2}}}
# {.scipad.pw0.pw2 horizontal {{.scipad.pw0.pw2.f1 .scipad.new1}}}
#   then it means that there are two panedwindows pw1 and pw2 that are
# packed in $pad.pw0. These levels pw1 and pw2 can be removed because
# they contain each one single pane. Removal of pw1 and pw2 and
# repacking their content directly into $pad.pw0 is what this proc is
# performing

    global pad

    # merge only when [$pwname panes] contains simple elements, i.e
    # frames or panedwindows with one single pane
    set pwinside "false"
    set doit "true"
    foreach pa [$pwname panes] {
        if {[isapanedwindow $pa]} {
            set pwinside "false"
            if {[llength [$pa panes]] != 1} {
                set doit "false"
                break
            }
        } else {
            # $pa is a frame, nothing to do, keep $doit value
        }
    }

    # if there is no panedwindow in $pwname children list,
    # and if there is more than one pane,
    # i.e. if all the children are frames and if there is more than one child,
    # then don't merge
    if {$pwinside == "false" && [llength [$pwname panes]] != 1} {
        set doit "false"
    }

    # determine parent panedwindow name
    set parpw [getpwname $pwname]
    if {$parpw == $pad} {
        set doit "false"
    }

    if {$doit == "true"} {
        # save position (in the packing order) of the panedwindow to destroy
        set ind [expr {[lsearch [$parpw panes] $pwname] - 1}]
        if {$ind != -1} {
            set aftopt [lindex [$parpw panes] $ind]
            set befopt ""
        } else {
            set aftopt ""
            set befopt [lindex [$parpw panes] 1]
        }

        # create list of impacted widgets, i.e. those that need to be repacked
        set torepack [getpwtree $pwname]

        # forget the old packing
        foreach w $torepack {
            destroywidget $w
        }

        # forget the superfluous level
        $parpw forget $pwname
        destroy $pwname

        # pack in the panedwindow one level above
        foreach w $torepack {
            repackwidget $w $parpw $aftopt $befopt
        }
        spaceallsashesevenly
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

proc repackwidget {w pwname {aftopt ""} {befopt ""}} {
# recursive ancillary for proc mergepanedwindows
    global pwmaxid textfontsize

    if {[llength $w] == 2 && [isaframe [lindex $w 0]]} {
        # $w is a frame node list, just pack it
        set ta [lindex $w 1]
        packnewbuffer $ta $pwname 1 $aftopt $befopt
        focustextarea $ta

    } elseif {[llength $w] == 3 && [isapanedwindow [lindex $w 0]]} {
        # $w is a panedwindow node list (see format in getpwtree)

        # insert a new paned window after last existing pane in current paned window
        set lastexistingpane [lindex [$pwname panes] end]
        incr pwmaxid
        set newpw $pwname.pw$pwmaxid
        panedwindow $newpw -orient [lindex $w 1] -opaqueresize true
        $pwname paneconfigure $newpw -after $lastexistingpane -minsize [expr {$textfontsize * 2}]

        # repack anything that was previously in this paned window
        foreach sw [lindex $w 2] {
            repackwidget $sw $newpw
        }

    } else {
        # can't happen in principle
        tk_messageBox -message "Unexpected widget in proc repackwidget ($w): please report"
    }
}

proc getpwtree {{root ""}} {
# get the panedwindow hierarchical (sub-)tree in a nested list whose
# elements are either:
#    a list with two elements: { frame name textarea } if the child 
# is a frame;
# or a list of three elements: { pw_pathname orientation {sub-nodes} }
# in case the child is a paned window.
# The tree traversal starts from the panedwindow node $root and is
# ordered thanks to the use of [$root panes] ([winfo children $root]
# does not order the children in the packing order!)

    global pad
    if {$root == ""} {set root $pad.pw0}
    set res {}
    foreach w [$root panes] {
        if {[isapanedwindow $w]} {
            lappend res [list $w [$w cget -orient] [getpwtree $w]]
        } elseif {[isaframe $w]} {
            lappend res [list $w [gettafromwidget $w]]
        } else {
            # sub-elements of a frame, such as panetitle, they are not
            # interesting parts of the tree - nothing to do, just go on
            tk_messageBox -message "Unexpected widget in proc getpwtree ($w): please report"
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
# well, I could have used [winfo class ] instead...
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

proc getlistofhiddenta {} {
# retrieve the list of currently hidden textareas
    global listoftextarea
    set listofhidden {}
    foreach ta $listoftextarea {
        if {![isdisplayed $ta]} {
            lappend listofhidden $ta
        }
    }
    return $listofhidden
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

proc getlistofpw {{root ""}} {
# create the list of used paned windows
    global pad
    if {$root == ""} {set root $pad.pw0}
    set res $root
    foreach w [$root panes] {
        if {[isapanedwindow $w]} {
            set res [concat $res [getlistofpw $w]]
        }
    }
    return $res
}

proc getpeerlist {ta} {
# wrapup to [$ta peer names] taking into account the fact that peers are
# available only from Tk 8.5
# warning: [$ta peer names] returns a list that does NOT contain $ta itself
# this proc is designed so that code such as
#    foreach peer [getpeerlist $ta] {
#        do_the_right_thing
#    }
# will work either with Tk 8.4 or 8.5 without the need to test for the
# value of $Tk85. In 8.4 since there is no peer the loop above will just
# do nothing because the return value of proc getpeerlist is {}
    global Tk85
    if {$Tk85} {
        return [$ta peer names]
    } else {
        return [list ]
    }
}

proc getfullpeerset {ta} {
# return the full peers set, i.e. a flat list composed of $ta itself first,
# plus [getpeerlist $ta] appended
# this proc is designed so that code such as
#    foreach ta1 [getfullpeerset $ta] {
#        do_the_right_thing
#    }
# will work either with Tk 8.4 or 8.5 without the need to test for the
# value of $Tk85. In 8.4 since there is no peer the loop above will just
# execute once because the return value of [getfullpeerset $ta] is $ta
    return [linsert [getpeerlist $ta] 0 $ta]
}

proc filteroutpeers {talist} {
# $talist being a list of textareas, return a list of textareas where
# no peer shows up
# this is done by descending $talist and checking whether each element
# is a peer of a preceding one or not, which is decided based on getpeerlist
# (i.e. [$ta peer names]), and NOT on the presence of the peer id tag <X>
# in the displayedname
# there is no such thing as the "original" text widget from which peers
# are deduced. All peers are equivalent, including the first textarea
# from which the first peer was created
# in the algorithm below, the first item in $talist that has not yet
# been identified as a peer is kept in $outlist
    set nopeertalist [list ]
    set peertalist [list ]
    foreach ta $talist {
        foreach ta1 [getpeerlist $ta] {
            lappend peertalist $ta1
        }
        if {[lsearch -exact $peertalist $ta] == -1} {
            lappend nopeertalist $ta
        }
    }
    return $nopeertalist
}

proc appendpeerid {fname peerid} {
# append peer identifier to $fname, i.e. if fname contains
# "dir1/dir2/helloworld.sci", return "dir1/dir2/helloworld.sci <X>"
# where X is a (potentially multiple characters) number
# X is the given $peerid parameter if this value is greater than
# zero (otherwise it is ignored, which is very useful to avoid to
# test on $peerid != -1 elsewhere in the code before calling proc
# appendpeerid)
# if $fname already contains a peer identifier, it is first removed
    global Tk85
    if {!$Tk85} {
        return $fname
    } else {
        foreach {peerfname oldpeerid} [removepeerid $fname] {}
        if {$peerid > 0} {
            append peerfname " <$peerid>"
        }
        return $peerfname
    }
}

proc removepeerid {fname} {
# remove peer identifier prepended to $fname, i.e. if fname contains
# the string "dir1/dir2/helloworld.sci <3>"
# then return the list {dir1/dir2/helloworld.sci 3}
# if $fname does not contain a peer identifier, then return {$fname -1}
    global Tk85
    if {!$Tk85} {
        return [list $fname -1]
    } else {
        set pos1 [string last " <" $fname]
        if {$pos1 == -1} {
            # no peer id in $fname
            return [list $fname -1]
        } else {
            set filename [string replace $fname $pos1 end]
            set peerid [string replace $fname 0 "$pos1+1"]
            set peerid [string replace $peerid end end]
            return [list $filename $peerid]
        }
    }
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
    set paneheight [expr {[winfo height $pwname] / $nbpanes}]
    set panewidth  [expr {[winfo width  $pwname] / $nbpanes}]
    for {set i 0} {$i < [expr {$nbpanes - 1}]} {incr i} {
        set paneposx [expr {$panewidth  * ($i + 1)}]
        set paneposy [expr {$paneheight * ($i + 1)}]
        $pwname sash place $i $paneposx $paneposy
        update idletasks
    }
}

proc spaceallsasheskeeprelsizes {} {
# space all the sashes of all the existing paned windows
# while keeping their relative sizes in each panedwindow
# this proc is only used with Tk < 8.5 since in Tk 8.5
# this behavior is better obtained with -stretch always
# option for all panes

    set pwlist [getlistofpw]

    # save the current sashes positions before updating the display
    foreach pw $pwlist {
        set nbpanes($pw) [llength [$pw panes]]
        for {set i 0} {$i < [expr {$nbpanes($pw) - 1}]} {incr i} {
            set sashxy($pw,$i) [$pw sash coord $i]
        }
        set pwheight($pw) [winfo height $pw]
        set pwwidth($pw)  [winfo width  $pw]
    }

    update

    # set new panes sizes and sashes positions
    foreach pw $pwlist {
        set pwheight2 [winfo height $pw]
        set pwwidth2  [winfo width  $pw]
        # warning: here a case where expr must not be braced
        set incfacty [expr $pwheight2. / $pwheight($pw)]
        set incfactx [expr $pwwidth2.  / $pwwidth($pw) ]
        for {set i 0} {$i < [expr {$nbpanes($pw) - 1}]} {incr i} {
            set newx [expr {round([lindex $sashxy($pw,$i) 0] * $incfactx)}]
            set newy [expr {round([lindex $sashxy($pw,$i) 1] * $incfacty)}]
            $pw sash place $i $newx $newy
            update idletasks
        }
    }

}

proc managescroll {scrbar a b} {
# this is primarily only to add a catch to the command normally used
# this catch is required because the text widget may trigger scroll commands
# automatically when it is not packed in a pane,
# e.g. on $textarea configure -someoption
# note: this seems to happen because textareas are not yet destroyed when they
# have just been unpacked. Therefore the binding to the scrollbar might still
# be alive at this point for hidden textareas
# 2nd benefit, thanks to this proc, updating the margin does not need
# to redefine a lot of bindings relative to the textarea view adjustment
# such as MouseWheel, Key-Return, Key-Down, etc - quick and elegant
# solution
    global listoftextarea linenumbersmargins

    catch {$scrbar set $a $b}

    # proc managescroll might be called when scrolling text widgets other
    # than textareas, for instance the info box of the help menu. In such
    # a case, do not attempt to update line numbers
    # note: catched for the same reason as above
    catch {
        set ta [gettafromwidget [winfo parent $scrbar]]
        if {[lsearch -exact $listoftextarea $ta] != -1} {
            if {$linenumbersmargins != "hide"} {
                updatelinenumbersmargin $ta
            }
        }
    }
}

proc updatelinenumbersmargin {ta} {
# delete the line numbers in the margin of textarea $ta, and
# re-enter all of them taking into account the possibly new end of $ta
# the width of the margin is also updated, depending on the number of
# digits to display
# Algorithm:
#   Instead of having a text widget with the same number of lines as the
# textarea, with one line number in each of its line (hence line 1 always
# contains number 1), and arranging for yview to fit in both (this was
# my first idea), now I have a text widget that is never scrolled and
# whose first line contains the first line number of the textarea aside
# This is much more performant, and fits also nicely with wrapped lines
# Wrapped lines detection is done this way:
#   - retrieve the display line height
#   - loop from the first displayed line (not from start of textarea!)
#      . if the line number of the beginning of display line is equal to
#        the line number of the previous line plus one, then the current
#        line did not wrap, and its line number must be inserted in the
#        margin
#      . otherwise the current display line wrapped and an empty line must
#        be inserted in the margin
#   - until last display line (i.e. when current y in pixels is greater
#     than winfo height $ta)
    global linenumbersmargins

    # assert: when entering this proc, we have $linenumbersmargins != "hide"

    set tamargin [getpaneframename $ta].margin

    # start modifying margin content
    $tamargin configure -state normal

    $tamargin delete 1.0 end

    set endindex [$ta index end]
    scan $endindex "%d.%d" yend xend
    set nbyendchar [string length [expr {$yend - 1}]]

    # find out the height of the textarea, which will be the upper bound in
    # the loop below
    set winfoheight [winfo height $ta]

    # initialization values
    scan [$ta index @0,0] "%d.%d" prevline_p1 junk
    set curheight [lindex [$ta dlineinfo @0,0] 1]
    set spacepad ""

    while {$curheight <= $winfoheight} {
        scan [$ta index @0,$curheight] "%d.%d" linenum dropthis
        if {$linenum == $prevline_p1} {
            set prevline_p1 [expr {$linenum + 1}]
        } else {
            set linenum ""
        }
        if {$linenumbersmargins == "right"} {
            set spacepad [string repeat " " [expr {$nbyendchar - [string length $linenum]}]]
        } else {
            # spacepad is the empty string
        }
        $tamargin insert end "$spacepad$linenum\n"
        # line height might be different for each line, therefore it cannot
        # be seen as a constant in this while loop
        set lineheightinpix [lindex [$ta dlineinfo @0,$curheight] 3]
        incr curheight $lineheightinpix
    }

    # end of modification of margin content
    $tamargin configure -state disabled

    # update margin width according to the size of what it displays
    $tamargin configure -width $nbyendchar

    # scroll the line numbers margin to align them perfectly with the textarea
    # this is only actually needed for Tk 8.5 but doesn't hurt in 8.4
    # in 8.4 $topinpix is always 1 since the first displayed line in the
    # textarea is never clipped by the top of the textarea
    # in 8.5 $topinpix might be negative, indicating a clipping of the first
    # displayed line
    set topinpix [lindex [$ta dlineinfo @0,0] 1]
    if {$topinpix < 0} {
        set hiddenpartof1stline [expr {- $topinpix}]
        set marginheight [winfo height $tamargin]
        set fractomoveto [expr {double($hiddenpartof1stline) / $marginheight}]
        $tamargin yview moveto $fractomoveto
    }
}

proc togglelinenumbersmargins {} {
# for all visible textareas, show or hide line numbers in a textarea margin
# when entering this proc, $linenumbersmarginsmenusetting has the newly
# selected value in the options menu, and $linenumbersmargins is the old
# value
    global linenumbersmargins linenumbersmarginsmenusetting

    # nothing to do if the selected option is the same as the old one
    if {$linenumbersmargins == $linenumbersmarginsmenusetting} {
        return
    }

    if {$linenumbersmargins == "hide"} {
        # from "hide" to "left" or "right"
        showlinenumbersmargins
    } elseif {$linenumbersmarginsmenusetting == "hide"} {
        # from "left" or "right" to "hide"
        hidelinenumbersmargins
    } else {
        # from "left" to "right", or from "right" to "left"
        hidelinenumbersmargins ; update idletasks
        showlinenumbersmargins
    }

    set linenumbersmargins $linenumbersmarginsmenusetting
}

proc showlinenumbersmargins {} {
# for all visible textareas, show line numbers in a textarea margin
    global listoftextarea
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            addlinenumbersmargin $ta
        }
    }
}

proc addlinenumbersmargin {ta} {
# create a textarea margin where line numbers will be displayed
    global textFont bgcolors fgcolors
    foreach c1 "$bgcolors $fgcolors" {global $c1}

    set tapwfr [getpaneframename $ta]

    # margin for line numbers
    # -width 1 ensures that the bottom scrollbar shows up
    # whatever the size of the textarea
    # the correct -width is anyway set in proc updatelinenumbersmargin
    text $tapwfr.margin -bd 0 -font $textFont \
            -background $BGLNMARGCOLOR -foreground $FGLNMARGCOLOR \
            -height 1 -width 1 -takefocus 0 -state disabled

    # let the user think he cannot select in the margin
    $tapwfr.margin configure -selectbackground [$tapwfr.margin cget -background]
    $tapwfr.margin configure -selectforeground [$tapwfr.margin cget -foreground]
    $tapwfr.margin configure -selectborderwidth 0

    bind $tapwfr.margin <ButtonRelease-1> { set ta [gettafromwidget [winfo parent %W]] ; \
                                            if {[info exists listoffile("$ta",fullname)]} { \
                                                focustextarea $ta ; \
                                            } \
                                          }

    # prevent unwanted Text class bindings from triggering
    bind $tapwfr.margin <Button-3> {break}
    bind $tapwfr.margin <Shift-Button-3> {break}
    bind $tapwfr.margin <Control-Button-3> {break}
    bind $tapwfr.margin <ButtonRelease-2> {break}
    bind $tapwfr.margin <Button-1> {break}
    bind $tapwfr.margin <Button-2> {break}
    bind $tapwfr.margin <Motion> {break}
    bind $tapwfr.margin <MouseWheel> {break}
    bind $tapwfr.margin <Button1-Leave> {break} ; # prevents autoscan

    pack $tapwfr.margin -in $tapwfr.topleft -before $ta -side left \
            -expand 0 -fill both -padx 2

    updatelinenumbersmargin $ta
}

proc hidelinenumbersmargins {} {
# for all visible textareas, hide line numbers from the textarea margin
    global listoftextarea
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            removelinenumbersmargin $ta
        }
    }
}

proc removelinenumbersmargin {ta} {
# remove a textarea margin where line numbers are displayed
    set tapwfr [getpaneframename $ta]
    pack forget $tapwfr.margin
    destroy $tapwfr.margin
}

proc switchbuffersinpane {w} {
# switch buffers inside a single pane - only the hidden buffers are switched
    event generate $w <ButtonRelease-1>
    set talist [getlistofhiddenta]
    set tacur [gettextareacur]
    lappend talist $tacur
    set talist [lsort $talist]
    set toshow [expr {[lsearch -sorted $talist $tacur] + 1}]
    if {$toshow == [llength $talist]} {
        set toshow 0
    }
    showtext [lindex $talist $toshow]
}

proc nextbuffer {type} {
# switch to the next buffer
# $type is either "all" or "visible"
    global pad listoftextarea listoffile
    global FirstBufferNameInWindowsMenu
    set nbuf [expr {[llength $listoftextarea] + $FirstBufferNameInWindowsMenu}]
    set ta [gettextareacur]
    set curbuf [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
    set initialcurbuf $curbuf
    incr curbuf
    if {$curbuf >= $nbuf} {
        set curbuf $FirstBufferNameInWindowsMenu
    }
    set found false
    while {!$found && $curbuf!=$initialcurbuf} {
        if {$type == "visible"} {
            set candidatetaid [$pad.filemenu.wind entrycget $curbuf -value]
            if {[isdisplayed $pad.new$candidatetaid]} {
                set found true
            } else {
                incr curbuf
                if {$curbuf >= $nbuf} {
                    set curbuf $FirstBufferNameInWindowsMenu
                }
            }
        } else {
            set found true
        }
    }
    if {$found} {
        $pad.filemenu.wind invoke $curbuf
    }
}

proc prevbuffer {type} {
# switch to the previous buffer
# $type is either "all" or "visible"
    global pad listoftextarea listoffile
    global FirstBufferNameInWindowsMenu
    set nbuf [expr {[llength $listoftextarea] + $FirstBufferNameInWindowsMenu - 1}]
    set ta [gettextareacur]
    set curbuf [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
    set initialcurbuf $curbuf
    incr curbuf -1
    if {$curbuf < $FirstBufferNameInWindowsMenu} {
        set curbuf $nbuf
    }
    set found false
    while {!$found && $curbuf!=$initialcurbuf} {
        if {$type == "visible"} {
            set candidatetaid [$pad.filemenu.wind entrycget $curbuf -value]
            if {[isdisplayed $pad.new$candidatetaid]} {
                set found true
            } else {
                incr curbuf -1
                if {$curbuf < $FirstBufferNameInWindowsMenu} {
                    set curbuf $nbuf
                }
            }
        } else {
            set found true
        }
    }
    if {$found} {
        $pad.filemenu.wind invoke $curbuf
    }
}

proc gotoline {} {
# Pop up the go to line dialog
    global pad textFont menuFont unklabel
    global physlogic linetogo curfileorfun funtogoto

    # gotoline cannot be executed since it uses getallfunsintextarea
    # which needs the colorization results
    if {[colorizationinprogress]} {return}

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
    # Set default values for the other choices of the box
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
    wm withdraw $gotln

    label $gotln.l1 -text [mc "Go to:"] -font $menuFont
    pack $gotln.l1 -anchor w -pady 5 -padx 3

    frame $gotln.f1
    frame $gotln.f1.f1l
    set bestwidth [mcmaxra "&logical line" \
                           "&physical line"]
    eval "radiobutton $gotln.f1.f1l.rbut1 [bl "&logical line"]  \
            -variable physlogic -value \"logical\" \
            -command \"updateOKbuttonstategoto $gotln\" \
            -width $bestwidth -anchor w -font \[list $menuFont\] "
    eval "radiobutton $gotln.f1.f1l.rbut2 [bl "&physical line"] \
            -variable physlogic -value \"physical\" \
            -command \"updateOKbuttonstategoto $gotln\" \
            -width $bestwidth -anchor w -font \[list $menuFont\] "
    entry $gotln.f1.en1 -textvariable linetogo \
            -justify center \
            -width 8 -font $textFont
    pack $gotln.f1.f1l.rbut1 $gotln.f1.f1l.rbut2 -anchor w
    pack $gotln.f1.en1 -side right
    pack $gotln.f1.f1l
    pack $gotln.f1 -anchor w

    frame $gotln.f2
    eval "radiobutton $gotln.f2.rbut3 [bl "in &function"]  \
            -variable curfileorfun -value \"function\" \
            -command \"updateOKbuttonstategoto $gotln\" \
            -anchor w -font \[list $menuFont\] "
    eval "radiobutton $gotln.rbut4 [bl "in &current file"] \
            -variable curfileorfun -value \"current_file\" \
            -command \"updateOKbuttonstategoto $gotln\" \
            -anchor w -font \[list $menuFont\] "
    menubutton $gotln.f2.mb -text [lindex $funtogoto 0] -indicatoron 1 \
        -font $textFont

    menu $gotln.f2.mb.om1 -tearoff 0 -font $menuFont
    $gotln.f2.mb configure -menu $gotln.f2.mb.om1
    foreach {funname ta funstartline} $funtogotolist {
        $gotln.f2.mb.om1 add radiobutton -label $funname -font $menuFont \
            -variable funtogoto -value [list $funname $ta $funstartline] \
            -command "updatemenubutlabelgoto $gotln"
    }
    pack $gotln.f2.rbut3 $gotln.f2.mb -side left -anchor w
    pack $gotln.f2 $gotln.rbut4 -anchor w

    set bestwidth [mcmaxra "OK" \
                           "Cancel"]
    button $gotln.ok -text [mc "OK"] \
            -font $menuFont -width $bestwidth \
            -command "dogotoline ; destroy $gotln"
    button $gotln.cancel -text [mc "Cancel"] \
            -font $menuFont -width $bestwidth \
            -command "destroy $gotln"
    pack $gotln.ok $gotln.cancel -side left -padx 5 -pady 5 -fill none -expand yes

    update idletasks
    setwingeom $gotln
    wm deiconify $gotln

    focus $gotln.f1.en1
    $gotln.f1.en1 selection range 0 end

    bind $gotln <Return> {if {[[winfo toplevel %W].ok cget -state] == "normal"} { \
                              dogotoline ; destroy [winfo toplevel %W] \
                          }}
    bind $gotln <Escape> {destroy [winfo toplevel %W]}

    bind $gotln <Alt-[fb $gotln.f1.f1l.rbut1]> \
        {[winfo toplevel %W].f1.f1l.rbut1 invoke}
    bind $gotln <Alt-[fb $gotln.f1.f1l.rbut2]> \
        {[winfo toplevel %W].f1.f1l.rbut2 invoke}
    bind $gotln <Alt-[fb $gotln.f2.rbut3]> \
        {[winfo toplevel %W].f2.rbut3 invoke}
    bind $gotln <Alt-[fb $gotln.rbut4]> \
        {[winfo toplevel %W].rbut4 invoke}

    # Default choices
    if {$physlogic    == ""} {$gotln.f1.f1l.rbut1 invoke}
    if {$curfileorfun == ""} {$gotln.f2.rbut3 invoke}
    if {$funtogotolist == {}} {
        # preselect physical line in buffer if there is no function definition
        $gotln.f1.f1l.rbut2 invoke
        $gotln.rbut4 invoke
    } else {
        # preselect the first function found if no previous choice was made
        if {[lindex $funtogoto 0] == $unklabel} {
            $gotln.f2.mb.om1 invoke 0
        }
    }

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

proc dogotoline {{physlogic_ "useglobals"} {linetogo_ ""} {curfileorfun_ ""} {funtogoto_ ""}} {
# Actually perform a go to line number ... taking into account all the possible
# choices from the user in the dialog box. These choices are known by the four
# global variables below (this is required and sufficient)

    global physlogic linetogo curfileorfun funtogoto

    # if no input parameter is given, use the globals
    if {$physlogic_ == "useglobals"} {
        set physlogic_ $physlogic
        set linetogo_ $linetogo
        set curfileorfun_ $curfileorfun
        set funtogoto_ $funtogoto
    }

    if {$curfileorfun_ == "current_file"} {
        if {$physlogic_ == "physical"} {
            # go to physical line in current file
            set ta [gettextareacur]
            $ta mark set insert "$linetogo_.0"
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
            while {$linetogo_ != $curlogicline && [$ta compare $curphysline < $endpos]} {
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
        if {$physlogic_ == "physical"} {
            # go to physical line in function
            set textarea [lindex $funtogoto_ 1]
            set absoluteline [$textarea index "[lindex $funtogoto_ 2] + 1c + $linetogo_ lines -1l"]
            # check that the end of the function is after the position to go to
            set infun [whichfun $absoluteline $textarea]
            if {$infun == {}} {
                # target line is before function definition line - should never
                # happen when called from the goto dialog since negative or
                # null line numbers are forbidden in the entry widget!
                # however, whichfun says (correctly) that $absoluteline is
                # outside of function when $absoluteline is on the same
                # line as the keyword function, but before it because of
                # indentation - this is not an error and Scipad goes to
                # the correct line
                set absoluteline [$textarea index [lindex $funtogoto_ 2]]
            } else {
                # target line is after the beginning of the function definition
                if {[lindex $infun 0] != [lindex $funtogoto_ 0]} {
                    # target line is after the end of the function definition,
                    # we will jump to the start of the function definition instead
                    showinfo [mc "Outside of function definition"]
                    set absoluteline [$textarea index [lindex $funtogoto_ 2]]
                } else {
                    # target position is between function and endfunction lines (inclusive)
                    set absoluteline [$textarea index "[lindex $funtogoto_ 2] + $linetogo_ lines -1l"]
                }
            }
            showtext $textarea
            $textarea mark set insert $absoluteline
            catch {keyposn $textarea}
            $textarea see insert

        } else {
            # go to logical line in function
            set textarea [lindex $funtogoto_ 1]
            set funstart [lindex $funtogoto_ 2]
            set offset 0
            set nbcl 0
            while {[expr {$offset - $nbcl + 1}] != $linetogo_} {
                incr offset
                set nbcl [countcontlines $textarea $funstart "$funstart + $offset l"]
            }
            set infun [whichfun [$textarea index "$funstart + $offset l"] $textarea]
            if {[lindex $infun 0] == [lindex $funtogoto_ 0]} {
                # target logical line is between function and endfunction
                set targetline [$textarea index "$funstart + $offset l"]
            } else {
                # target logical line is after the endfunction,
                # we will jump to the start of the function definition instead
                showinfo [mc "Outside of function definition"]
                set targetline [$textarea index $funstart]
            }
            showtext $textarea
            $textarea mark set insert $targetline
            catch {keyposn $textarea}
            $textarea see insert

        }
    }

}
