#file menu
menu $pad.filemenu.files -tearoff 0 -font $menuFont
    eval "$pad.filemenu  add cascade [me "&File"] \
	               -menu $pad.filemenu.files "
    eval "$pad.filemenu.files add command [me "&New"] \
                   -command \"filesetasnew\" -accelerator Ctrl+n"
    eval "$pad.filemenu.files add command [me "&Open..."] \
                   -command \"showopenwin\" -accelerator Ctrl+o"
    eval "$pad.filemenu.files add command [me "&Save"] \
                   -command \"filetosavecur\" -accelerator Ctrl+s"
    eval "$pad.filemenu.files add command [me "Save &as..."]\
                   -command \"filesaveascur\" -accelerator Ctrl+S"
    eval "$pad.filemenu.files add command [me "&Revert..."] \
                   -command \"revertsaved\" -state disabled -accelerator Ctrl+R"
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
    if {"$tcl_platform(platform)" == "unix"} {
        eval "$pad.filemenu.files add command [me "Print Se&tup"]\
                   -command \"printseupselection\" -accelerator Ctrl+P"
        eval "$pad.filemenu.files add command [me "&Print"] \
                   -command {selectprint \[gettextareacur\]} -accelerator Ctrl+p"
        $pad.filemenu.files add separator
    }
    BuildInitialRecentFilesList
    eval "$pad.filemenu.files add command [me "&Close"]\
                   -command \"closecur yesnocancel\" -accelerator Ctrl+w"
    eval "$pad.filemenu.files add command [me "E&xit"] \
                   -command \"exitapp yesnocancel\" -accelerator Ctrl+q"

#edit menu
menu $pad.filemenu.edit -tearoff 0 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Edit"] \
               -menu $pad.filemenu.edit "
    eval "$pad.filemenu.edit add command [me "&Undo"] \
               -command \"undo_menu_proc\" -accelerator Ctrl+z"
    eval "$pad.filemenu.edit add command [me "&Redo"] \
               -command \"redo_menu_proc\" -accelerator Ctrl+Z"
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
menu $pad.filemenu.search -tearoff 0 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Search"] \
               -menu $pad.filemenu.search  "
    eval "$pad.filemenu.search add command [me "&Find..."] \
               -command \"findtext find\" -accelerator Ctrl+f"
    eval "$pad.filemenu.search add command [me "Find &Next"] \
               -command \"findnext find\" -accelerator F3"
    eval "$pad.filemenu.search add command [me "&Replace..."] \
               -command \"findtext replace\" -accelerator Ctrl+r"
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

    menu $pad.filemenu.debug.step -tearoff 0 -font $menuFont
    eval "$pad.filemenu.debug add cascade [me "&Step by step"] -menu $pad.filemenu.debug.step "
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
    eval "$pad.filemenu.debug add command [me "G&o on ignoring any breakpoint"] \
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
menu $pad.filemenu.scheme -tearoff 0 -font $menuFont
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
      -menu $pad.filemenu.options.fontsize "
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

# window menu
menu $pad.filemenu.wind -tearoff 1 -title [mc "Opened Files"] -font $menuFont
    eval "$pad.filemenu add cascade [me "&Windows"] -menu $pad.filemenu.wind "
    $pad.filemenu.wind add radiobutton \
               -label "$listoffile("$pad.new$winopened",displayedname)"\
               -value $winopened -variable radiobuttonvalue \
               -command "montretext $pad.new$winopened"

# help menu
menu $pad.filemenu.help -tearoff 0 -font $menuFont
    eval "$pad.filemenu add cascade [me "&Help"] \
               -menu $pad.filemenu.help "
    eval "$pad.filemenu.help add command [me "&Help..."] \
               -command \"helpme\" -accelerator F1"
    eval "$pad.filemenu.help add command [me "&What's?..."] \
               -command \"helpword\" -accelerator Ctrl+F1"
    eval "$pad.filemenu.help add command [me "&About"] \
               -command \"aboutme\" -accelerator Shift+F1"

# now make the menu bar visible
$pad configure -menu $pad.filemenu 

# remember fontsize
setfontscipad $FontSize

# set initial debug state
setdbstate "NoDebug"
