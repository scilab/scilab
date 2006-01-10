proc createmenues {} {
    global pad menuFont tcl_platform bgcolors fgcolors sourcedir
    global listoffile listoftextarea FontSize
    global FirstBufferNameInWindowsMenu
    global FirstMRUFileNameInFileMenu
    global Shift_Tab
    foreach c1 "$bgcolors $fgcolors" {global $c1}

    #destroy old menues (used when changing language)
    foreach w [winfo children $pad.filemenu] {
        catch {destroy $w}
    }
    $pad.filemenu delete 0 end

    #file menu
    menu $pad.filemenu.files -tearoff 1 -font $menuFont
    eval "$pad.filemenu  add cascade [me "&File"] \
                   -menu $pad.filemenu.files "
    eval "$pad.filemenu.files add command [me "&New"] \
                   -command \"filesetasnew\" -accelerator Ctrl+n"
    eval "$pad.filemenu.files add command [me "&Open..."] \
                   -command \"showopenwin currenttile\" -accelerator Ctrl+o"
    menu $pad.filemenu.files.openintile -tearoff 0 -font $menuFont
    eval "$pad.filemenu.files add cascade [me "Open &in new"]\
      -menu $pad.filemenu.files.openintile"
        eval "$pad.filemenu.files.openintile add command [me "&horizontal tile"] \
                       -command \"showopenwin horizontal\" -accelerator Ctrl+4"
        eval "$pad.filemenu.files.openintile add command [me "&vertical tile"] \
                       -command \"showopenwin vertical\" -accelerator Ctrl+5"
    eval "$pad.filemenu.files add command [me "Op&en source of..."] \
                   -command \"opensourceof\" "
    eval "$pad.filemenu.files add command [me "&Save"] \
                   -command \"filetosavecur\" -accelerator Ctrl+s"
    eval "$pad.filemenu.files add command [me "Save &as..."]\
                   -command \"filesaveascur\" -accelerator Ctrl+S"
    eval "$pad.filemenu.files add command [me "&Revert..."] \
                   -command {revertsaved \[gettextareacur\]} -state disabled -accelerator Ctrl+R"
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Import &Matlab file..."] \
                   -command \"importmatlab\" -accelerator F4"
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Create help s&keleton..."] \
                   -command \"helpskeleton\" -state disabled "
    eval "$pad.filemenu.files add command [me "Compile as &help page"] \
                   -command \"xmlhelpfile\" -state disabled "
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Open &function source"] \
              -command {openlibfunsource \[\[gettextareacur\] index insert\]}\
                   -state disabled -accelerator Ctrl+/"
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Print Se&tup"]\
              -command \"printsetup\" -accelerator Ctrl+P"
    eval "$pad.filemenu.files add command [me "&Print"] \
              -command {selectprint \[gettextareacur\]} -accelerator Ctrl+p"
    $pad.filemenu.files add separator
    set FirstMRUFileNameInFileMenu [expr [$pad.filemenu.files index last] + 1]
    BuildInitialRecentFilesList
    eval "$pad.filemenu.files add command [me "&Close"]\
                   -command \"closecur yesnocancel\" -accelerator Ctrl+w"
    eval "$pad.filemenu.files add command [me "E&xit"] \
                   -command \"idleexitapp\" -accelerator Ctrl+q"
    bind $pad.filemenu.files <<MenuSelect>> {+showinfo_menu_file %W}

    #edit menu
    menu $pad.filemenu.edit -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Edit"] \
               -menu $pad.filemenu.edit "
    eval "$pad.filemenu.edit add command [me "&Undo"] \
               -command {undo \[gettextareacur\]} -accelerator Ctrl+z"
    eval "$pad.filemenu.edit add command [me "&Redo"] \
               -command {redo \[gettextareacur\]} -accelerator Ctrl+Z"
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "Cu&t"] \
               -command \"cuttext\" -accelerator Ctrl+x"
    eval "$pad.filemenu.edit add command [me "&Copy"] \
               -command \"copytext\" -accelerator Ctrl+c"
    eval "$pad.filemenu.edit add command [me "&Paste"] \
               -command \"pastetext\" -accelerator Ctrl+v"
    eval "$pad.filemenu.edit add command [me "&Delete"] \
               -command \"deletetext\" -accelerator Del"
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "Select &All"] \
               -command \"selectall\" -accelerator Ctrl+a"
    #    eval "$pad.filemenu.edit add command [me "Insert &Time/Date"] \
    #               -command \"printtime\" "
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "Co&mment selection"] \
               -command \"CommentSel\" -accelerator Ctrl+m"
    eval "$pad.filemenu.edit add command [me "U&ncomment selection"] \
               -command \"UnCommentSel\" -accelerator Ctrl+M"
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "&Indent selection"] \
               -command \"IndentSel\" -accelerator Ctrl+d"
    eval "$pad.filemenu.edit add command [me "Unin&dent selection"] \
               -command \"UnIndentSel\" -accelerator Ctrl+D"

    #search menu
    menu $pad.filemenu.search -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Search"] \
               -menu $pad.filemenu.search  "
    eval "$pad.filemenu.search add command [me "&Find..."] \
               -command \"findtextdialog find\" -accelerator Ctrl+f"
    eval "$pad.filemenu.search add command [me "Find &Next"] \
               -command \"findnext\" -accelerator F3"
    eval "$pad.filemenu.search add command [me "&Replace..."] \
               -command \"findtextdialog replace\" -accelerator Ctrl+r"
    eval "$pad.filemenu.search add command [me "&Goto Line..."] \
               -command \"gotoline\" -accelerator Ctrl+g"

    # exec menu
    menu $pad.filemenu.exec -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "E&xecute"] \
              -menu $pad.filemenu.exec "
    eval "$pad.filemenu.exec add command [me "&Load into Scilab"] \
              -command \"execfile\" -accelerator Ctrl+l"
    eval "$pad.filemenu.exec add command [me "&Evaluate selection"] \
              -command \"execselection\" -accelerator Ctrl+y"

    #debug menu
    menu $pad.filemenu.debug -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Debug"] \
               -menu $pad.filemenu.debug "
    eval "$pad.filemenu.debug add command [me "&Insert/Remove breakpoint"] \
               -command \"insertremove_bp\" -accelerator F9\
               -image menubutsetbptimage -compound left "
    eval "$pad.filemenu.debug add command [me "Remove &all breakpoints"] \
               -command \"removeall_bp\" -accelerator Ctrl+F9\
               -image menubutremoveallimage -compound left "
    $pad.filemenu.debug add separator
    eval "$pad.filemenu.debug add command [me "&Configure execution..."] \
               -command \"configurefoo_bp\" -accelerator F10\
               -image menubutconfigureimage -compound left "
    $pad.filemenu.debug add separator
    eval "$pad.filemenu.debug add command [me "Go to next b&reakpoint"] \
               -command \"tonextbreakpoint_bp\" -accelerator F11\
               -image menubutnextimage -compound left "

    menu $pad.filemenu.debug.step -tearoff 1 -font $menuFont
    eval "$pad.filemenu.debug add cascade [me "&Step by step"]\
                  -menu $pad.filemenu.debug.step "
        eval "$pad.filemenu.debug.step add command [me "Step &into"] \
                 -command \"stepbystep_bp\" -accelerator Shift+F8\
                 -image menubutstepimage -compound left "
        eval "$pad.filemenu.debug.step add command [me "Step o&ver"] \
                 -command \"stepbystep_bp\" -accelerator F8\
                 -image menubutstepimage -compound left "
        eval "$pad.filemenu.debug.step add command [me "Step &out"] \
                 -command \"stepbystep_bp\" -accelerator Ctrl+F8\
                 -image menubutstepimage -compound left "

    eval "$pad.filemenu.debug add command [me "Run to c&ursor"] \
               -command \"runtocursor_bp\" -accelerator Ctrl+F11\
               -image menubutruntocursorimage -compound left "
    eval "$pad.filemenu.debug add command \
               [me "G&o on ignoring any breakpoint"] \
               -command \"goonwo_bp\" -accelerator Shift+F12\
               -image menubutgoonignorimage -compound left "
    $pad.filemenu.debug add separator
    eval "$pad.filemenu.debug add command [me "Show &watch"] \
               -command \"showwatch_bp\" -accelerator Ctrl+F12\
               -image menubutwatchimage -compound left "
    $pad.filemenu.debug add separator
    eval "$pad.filemenu.debug add command [me "&Break"] \
               -command \"break_bp\" -accelerator F12\
               -image menubutbreakimage -compound left "
    eval "$pad.filemenu.debug add command [me "Cance&l debug"] \
               -command \"canceldebug_bp\" \
               -image menubutcancelimage -compound left "

    # scheme menu
    menu $pad.filemenu.scheme -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "S&cheme"] \
               -menu $pad.filemenu.scheme "
    eval "$pad.filemenu.scheme add radiobutton [me "S&cilab"] \
               -command {changelanguage \"scilab\"} -variable Scheme \
               -value \"scilab\" "
    eval "$pad.filemenu.scheme add radiobutton [me "&XML"] \
               -command {changelanguage \"xml\"} -variable Scheme \
               -value \"xml\" "
    eval "$pad.filemenu.scheme add radiobutton [me "&none"] \
               -command {changelanguage \"none\"} -variable Scheme \
               -value \"none\" "

    # options menu
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Options"] \
               -menu $pad.filemenu.options "
    menu $pad.filemenu.options.fontsize -tearoff 0 -font $menuFont
    eval "$pad.filemenu.options add cascade [me "&Font size"]\
      -menu $pad.filemenu.options.fontsize"
        eval "$pad.filemenu.options.fontsize add radiobutton [me "&micro"] \
              -value 10 -variable FontSize -command \"setfontscipad 10\" "
        eval "$pad.filemenu.options.fontsize add radiobutton [me "&small"]\
              -value 12 -variable FontSize -command \"setfontscipad 12\" "
        eval "$pad.filemenu.options.fontsize add radiobutton [me "m&edium"] \
              -value 14 -variable FontSize -command \"setfontscipad 14\" "
        eval "$pad.filemenu.options.fontsize add radiobutton [me "&large"] \
              -value 18 -variable FontSize -command \"setfontscipad 18\" "
    eval "$pad.filemenu.options add cascade [me "&Colors"] \
               -menu $pad.filemenu.options.colors"
        menu $pad.filemenu.options.colors -tearoff 1 -font $menuFont
        foreach c $bgcolors {
            eval "$pad.filemenu.options.colors add command [me "$c"] \
                -command {colormenuoption $c} -background \[set $c\]\
                -foreground $FGCOLOR -activeforeground $FGCOLOR"
        }
        foreach c $fgcolors {
            eval "$pad.filemenu.options.colors add command [me "$c"] \
                -command {colormenuoption $c} -foreground \[set $c\] \
                -activeforeground \[set $c\] -background $BGCOLOR"
        }
        updateactiveforegroundcolormenu
    eval "$pad.filemenu.options add check [me "Colorize \'&strings\'"] \
      -command {refreshQuotedStrings}\
      -offvalue no -onvalue yes -variable scilabSingleQuotedStrings"
    eval "$pad.filemenu.options add check [me "Show c&ontinued lines"] \
      -command {tagcontlinesinallbuffers}\
      -offvalue no -onvalue yes -variable showContinuedLines"
    eval "$pad.filemenu.options add check [me "Word &Wrap"] \
      -command {foreach l \$listoftextarea \{\$l configure -wrap \$wordWrap\}}\
      -offvalue none -onvalue word -variable wordWrap"
    eval "$pad.filemenu.options add cascade [me "&Tabs and indentation"] \
               -menu $pad.filemenu.options.tabs"
        menu $pad.filemenu.options.tabs -tearoff 0 -font $menuFont
        eval "$pad.filemenu.options.tabs add check [me "Tab inserts &spaces"] \
                    -offvalue tabs -onvalue spaces -variable tabinserts"
        eval "$pad.filemenu.options.tabs add cascade  \
                [me "&Indentation spaces"]\
                -menu [tk_optionMenu $pad.filemenu.options.tabs.indentspaces \
                        indentspaces 1 2 3 4 5 6 7 8 9 10]"
    eval "$pad.filemenu.options add cascade [me "Com&pletion"] \
               -menu $pad.filemenu.options.completion"
        menu $pad.filemenu.options.completion -tearoff 0 -font $menuFont
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "&Tab"] -command {SetCompletionBinding}\
                    -value \"Tab\" -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "&Control-Tab"] -command {SetCompletionBinding}\
                    -value \"Control-Tab\" -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "&Alt-Tab"] -command {SetCompletionBinding}\
                    -value \"Alt-Tab\" -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "&Shift-Tab"] -command {SetCompletionBinding}\
                    -value \[list $Shift_Tab\] -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "C&ontrol-Alt-Tab"] -command {SetCompletionBinding}\
                    -value \"Control-Alt-Tab\" -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "S&hift-Control-Tab"] -command {SetCompletionBinding}\
                    -value \"Shift-Control-Tab\" -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "Sh&ift-Alt-Tab"] -command {SetCompletionBinding}\
                    -value \"Shift-Alt-Tab\" -variable completionbinding"
        eval "$pad.filemenu.options.completion add radiobutton \
                    [me "Shi&ft-Control-Alt-Tab"] -command {SetCompletionBinding}\
                    -value \"Shift-Control-Alt-Tab\" -variable completionbinding"
    menu $pad.filemenu.options.filenames -tearoff 0 -font $menuFont
    eval "$pad.filemenu.options add cascade [me "File&names"] \
           -menu $pad.filemenu.options.filenames "
        eval "$pad.filemenu.options.filenames add radiobutton \
            [me "&Full path"] -command {RefreshWindowsMenuLabels}\
             -value full -variable filenamesdisplaytype"
        eval "$pad.filemenu.options.filenames add radiobutton \
            [me "Full path if &ambiguous"] -command {RefreshWindowsMenuLabels}\
             -value fullifambig -variable filenamesdisplaytype"
        eval "$pad.filemenu.options.filenames add radiobutton \
                 [me "&Unambiguous pruned path"]\
                 -command {RefreshWindowsMenuLabels}\
                 -value pruned -variable filenamesdisplaytype"
    eval "$pad.filemenu.options add cascade  [me "&Recent files"]\
               -menu [tk_optionMenu $pad.filemenu.options.recent \
                    maxrecentfiles 0 1 2 3 4 5 6 7 8 9 10]"
    for {set i 0} {$i<=10} {incr i} {
        $pad.filemenu.options.recent.menu entryconfigure $i \
            -command {UpdateRecentFilesList}
    }
    eval "$pad.filemenu.options add cascade  [me "&Backup files depth"]\
               -menu [tk_optionMenu $pad.filemenu.options.backup \
                    filebackupdepth 0 1 2 3 4 5 6 7 8 9 10]"
    menu $pad.filemenu.options.locale -tearoff 0 -font $menuFont
    eval "$pad.filemenu.options add cascade [me "&Locale"] \
           -menu $pad.filemenu.options.locale "
    set msgsdir [file join $sourcedir msg_files]
    set msgFiles [lsort [glob -nocomplain -tails -directory $msgsdir *.msg]]
    foreach m $msgFiles {
        set l [file rootname $m]
        # for some reason $l might be empty under cygwin
        # this is probably due to the /cygdrive/ syntax for paths
        if {$l != ""} {
            eval "$pad.filemenu.options.locale add radiobutton \
                [me [concat $l locale ($l)]] \
                -variable lang -value $l -command relocalize"
        }
    }

    # window menu
    menu $pad.filemenu.wind -tearoff 1 -title [mc "Opened Files"] \
         -font $menuFont
    eval "$pad.filemenu add cascade [me "&Windows"] -menu $pad.filemenu.wind "
    eval "$pad.filemenu.wind add command [me "&Maximize"] \
               -command \"maximizebuffer\" -accelerator Ctrl+1 "
    eval "$pad.filemenu.wind add command [me "&Split"] \
               -command \"splitwindow vertical\" -accelerator Ctrl+2 "
    eval "$pad.filemenu.wind add command [me "S&plit (side by side)"] \
               -command \"splitwindow horizontal\" -accelerator Ctrl+3 "
    eval "$pad.filemenu.wind add command [me "Tile all &vertically"] \
               -command \"tileallbuffers vertical\" "
    eval "$pad.filemenu.wind add command [me "Tile all &horizontally"] \
               -command \"tileallbuffers horizontal\" "
    eval "$pad.filemenu.wind add command [me "Space sashes &evenly"] \
               -command \"spaceallsashesevenly\" "
    $pad.filemenu.wind add separator
    set FirstBufferNameInWindowsMenu [expr [$pad.filemenu.wind index last] + 1]
    foreach ta $listoftextarea {
        set winopened [scan $ta $pad.new%d]
        $pad.filemenu.wind add radiobutton \
            -label $listoffile("$ta",displayedname)\
            -value $winopened -variable textareaid \
            -command "montretext $ta"
    }
    bind $pad.filemenu.wind <<MenuSelect>> {+showinfo_menu_wind %W}

    # help menu
    menu $pad.filemenu.help -tearoff 1 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Help"] \
               -menu $pad.filemenu.help "
    eval "$pad.filemenu.help add command [me "&Help..."] \
               -command \"helpme\" -accelerator F1"
    eval "$pad.filemenu.help add command [me "&What's?..."] \
               -command \"helpword\" -accelerator Ctrl+F1"
    eval "$pad.filemenu.help add command [me "&About"] \
               -command \"aboutme\" -accelerator Shift+F1"
## additional hacker entries, some disabled for the moment
# in case they become enabled, entries must be added in the msg files
     $pad.filemenu.help add separator
     eval "$pad.filemenu.help add command [me "Adding &translations..."] \
              -command {textbox [file join $msgsdir AddingTranslations.txt] \
               \"[mc "Adding Scipad Translations..."]\"}"
#             -command {openfile [file join $msgsdir AddingTranslations.txt]}"
#     eval "$pad.filemenu.help add command \
#             [me "&edit msg file"] \
#             -command {openfile [file join $msgsdir \$lang.msg]}"
     eval "$pad.filemenu.help add command [me "&Bugs \& wishlist"] \
             -command {textbox [file join $sourcedir BUGS] \
              \"[mc "Scipad Known bugs"]\"}"
     eval "$pad.filemenu.help add command [me "Change&log"] \
            -command {textbox [file join $sourcedir CHANGELOG] \
            \"[mc "Changes in the Scipad codebase"]\"}"

    # now make the menu bar visible
    $pad configure -menu $pad.filemenu 

    # create array of menu entries identifiers
    # this array allows to avoid to refer to menu entries by their hardcoded id
    createarrayofmenuentriesid $pad.filemenu
}

proc disablemenuesbinds {} {
# Disable certain menu entries and bindings
# This is used to avoid event overlapping that would trigger repeated calls
# to certain procs that do not support multiple instances running at the same
# time. proc tileallbuffer (because of the textarea destroy) is such an example.
# Scipad exit (File/Exit or clicking on [x]) does not make use of this
# facility to avoid hangs - the user can always escape out
    global pad nbrecentfiles FirstBufferNameInWindowsMenu listoftextarea
    global tileprocalreadyrunning
    set tileprocalreadyrunning true
    # File/Close
    set iClose [expr [GetFirstRecentInd] + $nbrecentfiles + 1]
    $pad.filemenu.files entryconfigure $iClose -state disabled
    bind $pad <Control-w> {}
    # Windows menu entries
    set lasttoset 3
    for {set i 1} {$i<$lasttoset} {incr i} {
        $pad.filemenu.wind entryconfigure $i -state disabled
        bind $pad <Control-Key-$i> ""
    }
    # Close buttons in the tile titles
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            [getpaneframename $ta].clbutton configure -state disabled
        }
    }
}

proc restoremenuesbinds {} {
# Restore menu entries and bindings disabled previously by proc disablemenuesbinds
    global pad nbrecentfiles FirstBufferNameInWindowsMenu listoftextarea
    global tileprocalreadyrunning
    # File/Close
    set iClose [expr [GetFirstRecentInd] + $nbrecentfiles + 1]
    $pad.filemenu.files entryconfigure $iClose -state normal
    bind $pad <Control-w> {closecur yesnocancel}
    # Windows menu entries
    set lasttoset 3
    for {set i 1} {$i<$lasttoset} {incr i} {
        $pad.filemenu.wind entryconfigure $i -state normal
        bind $pad <Control-Key-$i> "$pad.filemenu.wind invoke $i"
    }
    # Close buttons in the tile titles
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            [getpaneframename $ta].clbutton configure -state normal
        }
    }
    set tileprocalreadyrunning false
}

proc getlasthiddentextareamenuind {} {
# get the index in the windows menu of the last entry of this menu
# that is not visible (i.e. packed in a pane), or "" if all buffers
# are visible
    global pad FirstBufferNameInWindowsMenu
    set found 0
    set i [$pad.filemenu.wind index end]
    while {$i >= $FirstBufferNameInWindowsMenu} {
        if {![isdisplayed $pad.new[$pad.filemenu.wind entrycget $i -value]]} {
            set found 1
            break
        }
        incr i -1
    }
    if {$found == 1} {
        return $i
    } else {
        return ""
    }
}

proc getlasthiddentextareaid {} {
# get the -value option ($textareaid) of the last entry of the windows menu
# that is not visible (i.e. packed in a pane), or "" if all buffers
# are visible
    global pad
    set i [getlasthiddentextareamenuind]
    if {$i != ""} {
        return [$pad.filemenu.wind entrycget $i -value]
    } else {
        return ""
    }
}

proc extractindexfromlabel {dm labsearched} {
# extractindexfromlabel is here to cure bugs with special filenames
# This proc should be used as a replacement for [$menuwidget index $label]
# It returns the index of entry $label in $menuwidget, even if $label is a
# number or an index reserved name (see the tcl/tk help for menu indexes)
# If $label is not found in $menuwidget, it returns -1
    global pad FirstBufferNameInWindowsMenu
    global FirstMRUFileNameInFileMenu nbrecentfiles
    if {$dm == "$pad.filemenu.wind"} {
        set startpoint $FirstBufferNameInWindowsMenu
        set stoppoint  [$dm index last]
    } elseif {$dm == "$pad.filemenu.files"} {
        set startpoint $FirstMRUFileNameInFileMenu
        set stoppoint  [expr $FirstMRUFileNameInFileMenu + $nbrecentfiles]
    } else {
        tk_messageBox -message "Unexpected menu widget in proc extractindexfromlabel ($dm): please report"
    }
    for {set i $startpoint} {$i<=$stoppoint} {incr i} {
        if {[$dm type $i] != "separator" && [$dm type $i] != "tearoff"} {
            set lab [$dm entrycget $i -label]
            if {$lab == $labsearched} {
                return $i
            }
        }
    }
    return -1
}

proc createarrayofmenuentriesid {root} {
# create a global array containing the menu entries index
# $MenuEntryId($menu_pathname.$entry_label) == menu entry id in $menu_pathname
# note: $entry_label is localized (it is the label that appears in the menu)
# For instance, in french locale, we have:
#         $MenuEntryId($pad.filemenu.files.Nouveau) == 0
    global MenuEntryId
    foreach w [winfo children $root] {
        if {[catch {set temp [$w index last]}] != 0} {
            # this is to handle the case of the tk_optionMenu menues
            set w $w.menu
        }
        for {set id 0} {$id<=[$w index last]} {incr id} {
            if {[$w type $id] == "command" || \
                [$w type $id] == "radiobutton" || \
                [$w type $id] == "checkbutton" || \
                [$w type $id] == "cascade"} {
                set MenuEntryId($w.[$w entrycget $id -label]) $id
            }
            if {[$w type $id] == "cascade"} {
                createarrayofmenuentriesid $w
            }
        }
    }
}

proc showinfo_menu_file {w} {
# display full pathname of a recent file entry of the file menu
# as a showinfo
    global pad nbrecentfiles listofrecent
    if {$nbrecentfiles > 0} {
        set rec1ind [GetFirstRecentInd]
        set recnind [expr $rec1ind + $nbrecentfiles - 1]
        set mouseentry [$w index active]
        if {$rec1ind<=$mouseentry && $mouseentry<=$recnind} {
            showinfo [lindex $listofrecent [expr $mouseentry - $rec1ind]]
        }
    }
}

proc showinfo_menu_wind {w} {
# display full pathname of a file entry of the windows menu
# as a showinfo
    global pad FirstBufferNameInWindowsMenu listoffile listoftextarea
    set mouseentry [$w index active]
    if {$FirstBufferNameInWindowsMenu<=$mouseentry} {
        foreach ta $listoftextarea {
            set ind [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            if {$ind==$mouseentry} {
                break
            }
        }
        showinfo $listoffile("$ta",fullname)
    }
}
