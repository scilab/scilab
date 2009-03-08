#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#

proc createmenues {} {
    global pad tcl_platform bgcolors fgcolors
    global sourcedir moduledir msgsdir binddir
    global listoffile listoftextarea bindset
    global FirstBufferNameInWindowsMenu
    global FirstMRUFileNameInFileMenu
    global FirstMRUEncodingInOptEncMenu
    global Shift_Tab
    foreach c1 "$bgcolors $fgcolors" {global $c1}
    global Tk85
    global currentencoding defaultencoding nbrecentencodings
    global autodetectencodinginxmlfiles

    #destroy old menues (used when changing language)
    foreach w [winfo children $pad.filemenu] {
        catch {destroy $w}
    }
    $pad.filemenu delete 0 end

    #file menu
    menu $pad.filemenu.files -tearoff 1
    eval "$pad.filemenu add cascade [me "&File"] -menu $pad.filemenu.files "
    eval "$pad.filemenu.files add command [me "&New"] [ca {filesetasnew}]"
    eval "$pad.filemenu.files add command [me "&Open..."] \
                   [ca {showopenwin currenttile}]"
    menu $pad.filemenu.files.openintile -tearoff 0
    eval "$pad.filemenu.files add cascade [me "Open &in new"]\
      -menu $pad.filemenu.files.openintile"
        eval "$pad.filemenu.files.openintile add command \
                   [me "&horizontal tile"] [ca {showopenwin horizontal}]"
        eval "$pad.filemenu.files.openintile add command \
                   [me "&vertical tile"]   [ca {showopenwin vertical}]"
    eval "$pad.filemenu.files add command [me "Op&en source of..."] \
                   -command \"opensourceof\" "
    eval "$pad.filemenu.files add command [me "&Save"] [ca {filetosavecur}]"
    eval "$pad.filemenu.files add command [me "Save &as..."]\
                   [ca {filesaveascur}]"
    eval "$pad.filemenu.files add command [me "Save a&ll"] -command \"filetosaveall\" "
    eval "$pad.filemenu.files add command [me "&Revert..."] -state disabled\
                    [ca {revertsaved [gettextareacur]}]"
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Import &Matlab file..."] \
                   [ca {importmatlab}]"
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Create help s&keleton..."] \
                   -command \"createhelpfile skeleton\" -state disabled "
    eval "$pad.filemenu.files add command [me "Create help from hea&d comments..."] \
                   -command \"createhelpfile fromsci\" -state disabled "
    eval "$pad.filemenu.files add command [me "Compile as &help page"] \
                   -command \"xmlhelpfile\" -state disabled "
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Open &function source"] \
              [ca {openlibfunsource [[gettextareacur] index insert]}]\
                   -state disabled"
    $pad.filemenu.files add separator
    eval "$pad.filemenu.files add command [me "Print Se&tup"]\
            [ca {printsetup}]"
    eval "$pad.filemenu.files add command [me "&Print"] \
            [ca {selectprint [gettextareacur]}]"
    $pad.filemenu.files add separator
    set FirstMRUFileNameInFileMenu [expr {[$pad.filemenu.files index last] + 1}]
    BuildInitialRecentFilesList
    eval "$pad.filemenu.files add command [me "&Close file"]\
                   [ca {closecurfile yesnocancel}]"
    eval "$pad.filemenu.files add command [me "E&xit"] \
                   [ca idleexitapp]"
    bind $pad.filemenu.files <<MenuSelect>> {+showinfo_menu_file %W}

    #edit menu
    menu $pad.filemenu.edit -tearoff 1
    eval "$pad.filemenu add cascade [me "&Edit"] -menu $pad.filemenu.edit "
    eval "$pad.filemenu.edit add command [me "&Undo"] \
               [ca {undo [gettextareacur]}]"
    eval "$pad.filemenu.edit add command [me "&Redo"] \
               [ca {redo [gettextareacur]}]"
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "Cu&t"] [ca {cuttext normal}]"
    eval "$pad.filemenu.edit add command [me "B&lock cut"] [ca {cuttext block}]"
    eval "$pad.filemenu.edit add command [me "&Copy"] [ca copytext]"
    eval "$pad.filemenu.edit add command [me "&Paste"] [ca {pastetext normal}]"
    eval "$pad.filemenu.edit add command [me "&Block paste"] [ca {pastetext block}]"
    eval "$pad.filemenu.edit add command [me "&Delete"] [ca deletetext]"
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "Select &All"] [ca selectall]"
    # warning when uncommenting this: the underlines of all the Edit menu should
    # be reviewed, currently there is a duplicate on letter t
    #    eval "$pad.filemenu.edit add command [me "Insert &Time/Date"] \
    #               -command \"printtime\" "
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "Co&mment selection"] \
               [ca CommentSel]"
    eval "$pad.filemenu.edit add command [me "U&ncomment selection"] \
               [ca UnCommentSel]"
    $pad.filemenu.edit add separator
    eval "$pad.filemenu.edit add command [me "&Indent selection"] \
               [ca IndentSel]"
    eval "$pad.filemenu.edit add command [me "Unin&dent selection"] \
               [ca UnIndentSel]"

    #search menu
    menu $pad.filemenu.search -tearoff 1
    eval "$pad.filemenu add cascade [me "&Search"] \
               -menu $pad.filemenu.search  "
    eval "$pad.filemenu.search add command [me "&Find..."] \
               [ca {findtextdialog find}]"
    eval "$pad.filemenu.search add command [me "Find &Next"] [ca findnext]"
    eval "$pad.filemenu.search add command [me "&Replace..."] \
               [ca {findtextdialog replace}]"
    eval "$pad.filemenu.search add command [me "&Goto Line..."] [ca gotoline]"

    # exec menu
    menu $pad.filemenu.exec -tearoff 1
    eval "$pad.filemenu add cascade [me "E&xecute"] -menu $pad.filemenu.exec "
    eval "$pad.filemenu.exec add command [me "&Load into Scilab"] \
              [ca execfile]"
    eval "$pad.filemenu.exec add command [me "Load &all into Scilab"] \
              [ca execallfiles]"
    eval "$pad.filemenu.exec add command [me "&Evaluate selection"] \
              [ca execselection]"

    #debug menu
# debug menu entries have for now hardwired accelerators, because they are
# handled elsewhere according to the debug state machine
# Their bindings are not changed when switching style
    menu $pad.filemenu.debug -tearoff 1
    eval "$pad.filemenu add cascade [me "&Debug"] -menu $pad.filemenu.debug "
    eval "$pad.filemenu.debug add command [me "&Insert/Remove breakpoint"] \
               -command \"insertremove_bp\" -accelerator F9\
               -image menubutsetbptimage -compound left "
    eval "$pad.filemenu.debug add command [me "&Edit breakpoints"] \
               -command \"showbptgui_bp\" -accelerator Ctrl+F9\
               -image menubuteditbptimage -compound left "
    eval "$pad.filemenu.debug add command [me "Remove &all breakpoints"] \
               -command \"removeallbpt_scipad_bp\" -accelerator Shift+F9\
               -image menubutremoveallimage -compound left "
    $pad.filemenu.debug add separator
    eval "$pad.filemenu.debug add command [me "&Configure execution..."] \
               -command \"configurefoo_bp\" -accelerator F10\
               -image menubutconfigureimage -compound left "
    $pad.filemenu.debug add separator
    eval "$pad.filemenu.debug add command [me "Go to next b&reakpoint"] \
               -command \"tonextbreakpoint_bp\" -accelerator F11\
               -image menubutnextimage -compound left "

    menu $pad.filemenu.debug.step -tearoff 1
    eval "$pad.filemenu.debug add cascade [me "&Step by step"]\
                  -menu $pad.filemenu.debug.step \
                  -image menubutstepimage -compound left "
        eval "$pad.filemenu.debug.step add command [me "Step &into"] \
                 -command \"stepbystepinto_bp\" -accelerator Shift+F8\
                 -image menubutstepenterimage -compound left "
        eval "$pad.filemenu.debug.step add command [me "Step o&ver"] \
                 -command \"stepbystepover_bp\" -accelerator F8\
                 -image menubutstepoverimage -compound left "
        eval "$pad.filemenu.debug.step add command [me "Step &out"] \
                 -command \"stepbystepout_bp\" -accelerator Ctrl+F8\
                 -image menubutstepexitimage -compound left "

    eval "$pad.filemenu.debug add command [me "Run to re&turn point"] \
               -command \"runtoreturnpoint_bp\" -accelerator Shift+F11\
               -image menubutruntoreturnimage -compound left "
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
    menu $pad.filemenu.scheme -tearoff 1
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
    $pad.filemenu.scheme add separator
    eval "$pad.filemenu.scheme add check [me "&Colorize"] \
      -command {switchcolorizefile}\
      -offvalue false -onvalue true -variable ColorizeIt"

    # options menu
    menu $pad.filemenu.options -tearoff 1
    eval "$pad.filemenu add cascade [me "&Options"] \
               -menu $pad.filemenu.options "
    eval "$pad.filemenu.options add command [me "&Fonts..."] \
              [ca choosefonts]"
    eval "$pad.filemenu.options add cascade [me "&Colors"] \
               -menu $pad.filemenu.options.colors"
        menu $pad.filemenu.options.colors -tearoff 1
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

    menu $pad.filemenu.options.colorizeoptions -tearoff 0
    eval "$pad.filemenu.options add cascade [me "Colori&ze"] \
      -menu $pad.filemenu.options.colorizeoptions"
        eval "$pad.filemenu.options.colorizeoptions add radiobutton [me "&Always"] \
              -value always -variable colorizeenable "
        eval "$pad.filemenu.options.colorizeoptions add radiobutton [me "Ask for &big files"]\
              -value ask -variable colorizeenable "
        eval "$pad.filemenu.options.colorizeoptions add radiobutton [me "&Never"] \
              -value never -variable colorizeenable "
        $pad.filemenu.options.colorizeoptions add separator
        eval "$pad.filemenu.options.colorizeoptions add check [me "Colorize \'&strings\'"] \
              -command {refreshQuotedStrings}\
              -offvalue no -onvalue yes -variable scilabSingleQuotedStrings"
        eval "$pad.filemenu.options.colorizeoptions add check [me "Colorize &user functions"] \
              -command {docolorizeuserfun}\
              -offvalue no -onvalue yes -variable colorizeuserfuns"
        eval "$pad.filemenu.options.colorizeoptions add check [me "Show c&ontinued lines"] \
              -command {tagcontlinesinallbuffers}\
              -offvalue no -onvalue yes -variable showContinuedLines"
    menu $pad.filemenu.options.linenumbers -tearoff 0
    eval "$pad.filemenu.options add cascade [me "Line n&umbers"] \
      -menu $pad.filemenu.options.linenumbers"
        eval "$pad.filemenu.options.linenumbers add radiobutton [me "&Hide"] \
          -command {togglelinenumbersmargins}\
          -value hide -variable linenumbersmarginsmenusetting"
        eval "$pad.filemenu.options.linenumbers add radiobutton [me "&Left aligned"] \
          -command {togglelinenumbersmargins}\
          -value left -variable linenumbersmarginsmenusetting"
        eval "$pad.filemenu.options.linenumbers add radiobutton [me "&Right aligned"] \
          -command {togglelinenumbersmargins}\
          -value right -variable linenumbersmarginsmenusetting"
    eval "$pad.filemenu.options add check [me "Word &wrap"] \
      -command {togglewordwrap}\
      -offvalue none -onvalue word -variable wordWrap"
    menu $pad.filemenu.options.doubleclick -tearoff 0
    eval "$pad.filemenu.options add cascade [me "&Double-click behavior"]\
      -menu $pad.filemenu.options.doubleclick"
        eval "$pad.filemenu.options.doubleclick add radiobutton [me "&Linux"] \
              -value Linux -variable doubleclickscheme \
              -command \"updatedoubleclickscheme\" "
        # the underline below cannot be on the first W otherwise it collides
        # with the main menu bar item with same name
        eval "$pad.filemenu.options.doubleclick add radiobutton [me "Windo&ws"] \
              -value Windows -variable doubleclickscheme \
              -command \"updatedoubleclickscheme\" "
        eval "$pad.filemenu.options.doubleclick add radiobutton [me "&Scilab"] \
              -value Scilab -variable doubleclickscheme \
              -command \"updatedoubleclickscheme\" "
    eval "$pad.filemenu.options add cascade [me "&Tabs and indentation"] \
               -menu $pad.filemenu.options.tabs"
        menu $pad.filemenu.options.tabs -tearoff 0
        eval "$pad.filemenu.options.tabs add check [me "Tab inserts &spaces"] \
                    -offvalue tabs -onvalue spaces -variable tabinserts"
        eval "$pad.filemenu.options.tabs add cascade  \
                [me "&Indentation spaces"]\
                -menu [tk_optionMenu $pad.filemenu.options.tabs.indentspaces \
                        indentspaces 1 2 3 4 5 6 7 8 9 10]"
        set tabsizemenu [tk_optionMenu $pad.filemenu.options.tabs.tabsize \
                        tabsizeinchars 1 2 3 4 5 6 7 8]
        eval "$pad.filemenu.options.tabs add cascade  \
                [me "&Tab size (characters)"] -menu $tabsizemenu"
        for {set i 0} {$i<=[$tabsizemenu index last]} {incr i} {
            $tabsizemenu entryconfigure $i -command {settabsize}
        }
        eval "$pad.filemenu.options.tabs add check [me "Use &keyword indentation"] \
                    -offvalue 0 -onvalue 1 -variable usekeywordindent"
    eval "$pad.filemenu.options add cascade [me "Com&pletion"] \
               -menu $pad.filemenu.options.completion"
        menu $pad.filemenu.options.completion -tearoff 0
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
    menu $pad.filemenu.options.filenames -tearoff 0
    eval "$pad.filemenu.options add cascade [me "File&names"] \
           -menu $pad.filemenu.options.filenames "
        eval "$pad.filemenu.options.filenames add radiobutton \
            [me "&Full path"] -command {RefreshWindowsMenuLabelsWrtPruning}\
             -value full -variable filenamesdisplaytype"
        eval "$pad.filemenu.options.filenames add radiobutton \
            [me "Full path if &ambiguous"] -command {RefreshWindowsMenuLabelsWrtPruning}\
             -value fullifambig -variable filenamesdisplaytype"
        eval "$pad.filemenu.options.filenames add radiobutton \
                 [me "&Unambiguous pruned path"]\
                 -command {RefreshWindowsMenuLabelsWrtPruning}\
                 -value pruned -variable filenamesdisplaytype"
    menu $pad.filemenu.options.windmenusort -tearoff 0
    eval "$pad.filemenu.options add cascade [me "Windows menu &sorting"] \
           -menu $pad.filemenu.options.windmenusort "
        eval "$pad.filemenu.options.windmenusort add radiobutton \
            [me "&File opening order"] -command {sortwindowsmenuentries}\
             -value openorder -variable windowsmenusorting"
        eval "$pad.filemenu.options.windmenusort add radiobutton \
            [me "&Alphabetically"] -command {sortwindowsmenuentries}\
             -value alphabeticorder -variable windowsmenusorting"
        eval "$pad.filemenu.options.windmenusort add radiobutton \
                 [me "&Most recently used"] -command {sortwindowsmenuentries}\
                 -value MRUorder -variable windowsmenusorting"
    set recentfilesnumbmenu [tk_optionMenu $pad.filemenu.options.recentf \
                    maxrecentfiles 0 1 2 3 4 5 6 7 8 9 10 15 20 50]
    eval "$pad.filemenu.options add cascade  [me "&Recent files"]\
               -menu $recentfilesnumbmenu"
    for {set i 0} {$i<=[$recentfilesnumbmenu index last]} {incr i} {
        $recentfilesnumbmenu entryconfigure $i -command {UpdateRecentFilesList}
    }
    eval "$pad.filemenu.options add cascade  [me "&Backup files depth"]\
               -menu [tk_optionMenu $pad.filemenu.options.backup \
                    filebackupdepth 0 1 2 3 4 5 6 7 8 9 10]"
    menu $pad.filemenu.options.locale -tearoff 0
    eval "$pad.filemenu.options add cascade [me "&Locale"] \
           -menu $pad.filemenu.options.locale "
    foreach l [getavailablelocales] {
        # for some reason $l might be empty under cygwin
        # this is probably due to the /cygdrive/ syntax for paths
        if {$l != ""} {
            eval "$pad.filemenu.options.locale add radiobutton \
                [me [concat $l locale]] \
                -variable lang -value $l -command relocalize"
        }
    }
# feature enabled yet not 100% ok (see bindings/issues.txt) - teasing!
    menu $pad.filemenu.options.bindings -tearoff 0
    eval "$pad.filemenu.options add cascade [me "&Bindings style"] \
           -menu $pad.filemenu.options.bindings "
    set BindFiles [lsort [globtails $binddir *.tcl]]
    foreach m $BindFiles {
        set l [file rootname $m]
        # for some reason $l might be empty under cygwin
        # this is probably due to the /cygdrive/ syntax for paths
        if {$l != ""} {
# REMOVE -state disabled below to make this option menu work
            eval "$pad.filemenu.options.bindings add radiobutton \
                [me $l]  -state disabled \
                -variable bindstyle -value $l -command \"rebind\"" 
        }
    }
    menu $pad.filemenu.options.messageboxes -tearoff 0
    eval "$pad.filemenu.options add cascade [me "Execution &errors"] \
           -menu $pad.filemenu.options.messageboxes "
        eval "$pad.filemenu.options.messageboxes add radiobutton \
            [me "In Scilab &shell only"] \
             -value false -variable ScilabErrorMessageBox"
        eval "$pad.filemenu.options.messageboxes add radiobutton \
            [me "Copied in a &message box"] \
             -value true -variable ScilabErrorMessageBox"
    eval "$pad.filemenu.options add cascade [me "Enc&oding"] \
               -menu $pad.filemenu.options.encodings"
        menu $pad.filemenu.options.encodings -tearoff 1
        set FirstMRUEncodingInOptEncMenu [expr {[$pad.filemenu.options.encodings index last] + 1}]
        BuildInitialRecentEncodingsList
        # if the MRU list of encodings is empty, then add platform system encoding, iso8859-1 and utf-8
        # these encodings are supposed to exist (distributed with Tcl)
        if {$nbrecentencodings == 0} {
            AddRecentEncoding "utf-8"
            AddRecentEncoding "iso8859-1"
            AddRecentEncoding $defaultencoding
        }
        eval "$pad.filemenu.options.encodings add cascade [me "More enc&odings"] \
                   -menu $pad.filemenu.options.encodings.more"
            menu $pad.filemenu.options.encodings.more -tearoff 1
            foreach en [lsort -dictionary [encoding names]] {
                $pad.filemenu.options.encodings.more add radiobutton -label $en \
                    -command {setencoding} -value $en -variable currentencoding
            }
        set recentencodingsnumbmenu [tk_optionMenu $pad.filemenu.options.encodings.recente \
                        maxrecentencodings 0 1 2 3 4 5 6 7 8 9 10]
        eval "$pad.filemenu.options.encodings add cascade  [me "&Recent encodings"]\
                   -menu $recentencodingsnumbmenu"
        for {set i 0} {$i<=[$recentencodingsnumbmenu index last]} {incr i} {
            $recentencodingsnumbmenu entryconfigure $i -command {UpdateRecentEncodingsList}
        }
        eval "$pad.filemenu.options.encodings add check [me "&Auto-detect encoding when loading XML files"] \
          -offvalue false -onvalue true -variable autodetectencodinginxmlfiles"
    menu $pad.filemenu.options.exitopts -tearoff 0
    eval "$pad.filemenu.options add cascade [me "E&xit options"] \
           -menu $pad.filemenu.options.exitopts "
        eval "$pad.filemenu.options.exitopts add check [me "Show closure &X"] \
          -command {toggleclosureXcross}\
          -offvalue false -onvalue true -variable showclosureXcross"
        eval "$pad.filemenu.options.exitopts add check [me "Exit on &last file close"] \
          -offvalue false -onvalue true -variable exitwhenlastclosed"

    # windows menu
    menu $pad.filemenu.wind -tearoff 1 -title [mc "Opened Files"]
    eval "$pad.filemenu add cascade [me "&Windows"] -menu $pad.filemenu.wind "
# the bindings for the next 3 items need to invoke the item itself instead
# of a proc -- therefore we leave them hardwired
    eval "$pad.filemenu.wind add command [me "&Maximize"] \
               -command \"maximizebuffer\" -accelerator Ctrl+1 "
    eval "$pad.filemenu.wind add command [me "&Split"] \
               -command \"splitwindow vertical \\\"\\\" tile\" -accelerator Ctrl+2 "
    eval "$pad.filemenu.wind add command [me "S&plit (side by side)"] \
               -command \"splitwindow horizontal \\\"\\\" tile\" -accelerator Ctrl+3 "
    if {$Tk85} {
        eval "$pad.filemenu.wind add command [me "Sp&lit file"] \
                   -command \"splitwindow vertical \\\"\\\" file\" \
                   -accelerator Ctrl+Alt+2 "
        eval "$pad.filemenu.wind add command [me "Spl&it file (side by side)"] \
                   -command \"splitwindow horizontal \\\"\\\" file\" \
                   -accelerator Ctrl+Alt+3 "
    }
    eval "$pad.filemenu.wind add command [me "Tile all &vertically"] \
               -command \"tileallbuffers vertical\" "
    eval "$pad.filemenu.wind add command [me "Tile all &horizontally"] \
               -command \"tileallbuffers horizontal\" "
    eval "$pad.filemenu.wind add command [me "Space sashes &evenly"] \
               -command \"spaceallsashesevenly\" "
    $pad.filemenu.wind add separator
    set FirstBufferNameInWindowsMenu [expr {[$pad.filemenu.wind index last] + 1}]
    foreach ta $listoftextarea {
        set winopened [gettaidfromwidgetname $ta]
        addwindowsmenuentry $winopened $listoffile("$ta",displayedname)
    }
    bind $pad.filemenu.wind <<MenuSelect>> {+showinfo_menu_wind %W}

    # help menu
    menu $pad.filemenu.help -tearoff 1
    eval "$pad.filemenu add cascade [me "&Help"] \
               -menu $pad.filemenu.help "
    eval "$pad.filemenu.help add command [me "&Help..."] [ca helpme]"
    eval "$pad.filemenu.help add command [me "&What's?..."] [ca helpword]"
    eval "$pad.filemenu.help add command [me "&About"] \
             -command aboutme -accelerator Shift+F1"
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
             -command {textbox [file join $moduledir BUGS] \
              \"[mc "Scipad Known bugs"]\"}"
     eval "$pad.filemenu.help add command [me "Change&log"] \
            -command {textbox [file join $moduledir changelog.txt] \
            \"[mc "Changes in the Scipad codebase"]\"}"

    # now make the menu bar visible
    $pad configure -menu $pad.filemenu 

    # create array of menu entries identifiers
    # this array allows to avoid to refer to menu entries by their hardcoded id
    createarrayofmenuentriesid $pad.filemenu

    # set the correct font for menues, including tk_optionMenues, that do not
    # have a -font option - also set the tab size since updatefont all will
    # call settabsize
    updatefont all
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
    global Tk85
    set tileprocalreadyrunning true
    # File/Close
    set iClose [expr {[GetFirstRecentFileInd] + $nbrecentfiles + 1}]
    $pad.filemenu.files entryconfigure $iClose -state disabled
    binddisable $pad {closecurfile yesnocancel}
    # Windows menu entries
    set lasttoset [expr {$FirstBufferNameInWindowsMenu - 2}]
    for {set i 1} {$i<=$lasttoset} {incr i} {
        $pad.filemenu.wind entryconfigure $i -state disabled
    }
    bind $pad <Control-Key-1> ""
    bind $pad <Control-Key-2> ""
    bind $pad <Control-Key-3> ""
    if {$Tk85} {
        bind $pad <Control-Alt-Key-2> ""
        bind $pad <Control-Alt-Key-3> ""
    }
    # Close and hide buttons in the tile titles
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            [getpaneframename $ta].topbar.f.clbutton configure -state disabled
            [getpaneframename $ta].topbar.f.hibutton configure -state disabled
        }
    }
}

proc restoremenuesbinds {} {
# Restore menu entries and bindings disabled previously by proc disablemenuesbinds
    global pad nbrecentfiles FirstBufferNameInWindowsMenu listoftextarea
    global tileprocalreadyrunning
    global Tk85
    # File/Close
    set iClose [expr {[GetFirstRecentFileInd] + $nbrecentfiles + 1}]
    $pad.filemenu.files entryconfigure $iClose -state normal
    bindenable $pad {closecurfile yesnocancel}
    # Windows menu entries
    set lasttoset [expr {$FirstBufferNameInWindowsMenu - 2}]
    for {set i 1} {$i<=$lasttoset} {incr i} {
        $pad.filemenu.wind entryconfigure $i -state normal
    }
    bind $pad <Control-Key-1> "$pad.filemenu.wind invoke 1"
    bind $pad <Control-Key-2> "$pad.filemenu.wind invoke 2"
    bind $pad <Control-Key-3> "$pad.filemenu.wind invoke 3"
    if {$Tk85} {
        bind $pad <Control-Alt-Key-2> "$pad.filemenu.wind invoke 4"
        bind $pad <Control-Alt-Key-3> "$pad.filemenu.wind invoke 5"
    }
    # Close and hide buttons in the tile titles
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            [getpaneframename $ta].topbar.f.clbutton configure -state normal
            [getpaneframename $ta].topbar.f.hibutton configure -state normal
        }
    }
    set tileprocalreadyrunning false
}

proc getlastvisibletextareamenuind {} {
# get the index in the windows menu of the last entry of this menu
# that is visible (i.e. packed in a pane)
# note that there is always such an entry
    global pad FirstBufferNameInWindowsMenu
    set found 0
    set i [$pad.filemenu.wind index end]
    while {$i >= $FirstBufferNameInWindowsMenu} {
        if {[isdisplayed $pad.new[$pad.filemenu.wind entrycget $i -value]]} {
            set found 1
            break
        }
        incr i -1
    }
    if {$found == 1} {
        return $i
    } else {
        # shouldn't happen
        tk_messageBox -message "Impossible case in proc getlastvisibletextareamenuind: please report"
        return ""
    }
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
# extractindexfromlabel is here to cure bugs with special filenames, and
# to deal with labels with an underlined number prepended
# This proc should be used as a replacement for [$menuwidget index $label]
# It returns the index of entry $label in $menuwidget, even if $label is a
# number or an index reserved name (see the Tcl/Tk help for menu indexes)
# The leading underlined number is not considered to be part of the label
# and is ignored
# If $label is not found in $menuwidget, it returns -1
    global pad FirstBufferNameInWindowsMenu
    global FirstMRUFileNameInFileMenu nbrecentfiles

    if {$dm == "$pad.filemenu.wind"} {
        set startpoint $FirstBufferNameInWindowsMenu
        set stoppoint  [$dm index last]
    } elseif {$dm == "$pad.filemenu.files"} {
        set startpoint $FirstMRUFileNameInFileMenu
        set stoppoint  [expr {$FirstMRUFileNameInFileMenu + $nbrecentfiles}]
    } else {
        tk_messageBox -message "Unexpected menu widget in proc extractindexfromlabel ($dm): please report"
    }

    for {set i $startpoint} {$i<=$stoppoint} {incr i} {
        if {[$dm type $i] != "separator" && [$dm type $i] != "tearoff"} {
            set lab [$dm entrycget $i -label]
            # the first 9 labels have an underlined number prepended
            # that must be removed before comparison to the searched label
            if {$i < [expr {$startpoint + 9}]} {
                regexp {^[0-9] (.*)} $lab -> lab
            }
            if {$lab == $labsearched} {
                return $i
            }
        }
    }
    return -1
}

proc setwindowsmenuentrylabel {entry lab {sortmenu "sortmenu"}} {
# set the label of entry $entry of the windows menu
# $entry is relative to the start of the menu, i.e. to entry #0,
# and not to $FirstBufferNameInWindowsMenu
# $lab is the label without any leading underlined number
# the optional argument sortmenu is a flag allowing to sort the
# Windows menu, it should always be "sortmenu" (or not given)
# the only exception being precisely when sorting the menu, i.e.
# when called from proc sortwindowsmenuentries (this would otherwise
# create an infinite recursion)
# this proc takes care of updating the entry label with or without
# an underlined number prepended, depending on the position of the
# entry in the menu (only the first 9 entries have such an
# underlined number)
    global pad FirstBufferNameInWindowsMenu
    set underlinednumber [expr {$entry - $FirstBufferNameInWindowsMenu + 1}]
    if {$underlinednumber<10} {
        # here I first used
        #   set underlinedlabel [concat $underlinednumber $lab]
        # but concat trims spaces at the beginning of $lab, which will later
        # fool the regexp matching
        #   regexp {^[0-9] (.*)} $lab -> lab
        # in proc extractindexfromlabel
        set underlinedlabel "$underlinednumber $lab"
        $pad.filemenu.wind entryconfigure $entry \
           -label $underlinedlabel -underline 0
    } else {
        $pad.filemenu.wind entryconfigure $entry \
           -label $lab
    }
    if {$sortmenu == "sortmenu"} {
        sortwindowsmenuentries
    }
}

proc addwindowsmenuentry {val lab} {
# add an entry at the end of the windows menu, and prepend an underlined
# number if possible
# inputs: $val : -value of the variable attached to the radiobutton
#         $lab : -label of the menu entry, without any leading number
    global pad
    $pad.filemenu.wind add radiobutton \
        -value $val -variable textareaid \
        -command "showtext $pad.new$val"
    setwindowsmenuentrylabel [$pad.filemenu.wind index end] $lab
}

proc sortwindowsmenuentries {} {
    global windowsmenusorting
    global pad listoftextarea listoffile
    global FirstBufferNameInWindowsMenu

    set li [list ]
    foreach ta $listoftextarea {
        if {$listoffile("$ta",thetime) == "0"} {
            # a new unsaved file has thetime = 0
            # change it to a large value (around year 2034)
            # this saves from computing the max
            # note: 2000000000 works in lsort, but not 3000000000
            # there must be some internal limit wrt numbers coding
            # (probably 31 bits + sign ; 2^31 = 2147483648)
            set thetime "2000000000"
        } else {
            set thetime $listoffile("$ta",thetime)
        }
        lappend li [list $ta $listoffile("$ta",displayedname) $thetime]
    }

    switch -- $windowsmenusorting {
        openorder {
            # nothing to do, opening order is the list in $listoftextarea
        }
        alphabeticorder {
            set li [lsort -dictionary -index 1 $li]
        }
        MRUorder {
            set li [lsort -decreasing -integer -index 2 $li]
        }
        default {
            # can't happen in principle
            tk_messageBox -message "Unexpected sort scheme in proc sortwindowsmenuentries ($windowsmenusorting): please report"
        }
    }

    set i $FirstBufferNameInWindowsMenu
    foreach item $li {
        foreach {ta lab mtim} $item {}
        set winopened [gettaidfromwidgetname $ta]
        $pad.filemenu.wind entryconfigure $i \
            -value $winopened \
            -command "showtext $pad.new$winopened"
        setwindowsmenuentrylabel $i $lab "dontsort"
        incr i
    }
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
    global pad nbrecentfiles listofrecentfiles
    if {$nbrecentfiles > 0} {
        set rec1ind [GetFirstRecentFileInd]
        set recnind [expr {$rec1ind + $nbrecentfiles - 1}]
        set mouseentry [$w index active]
        if {$rec1ind<=$mouseentry && $mouseentry<=$recnind} {
            showinfo [lindex $listofrecentfiles [expr {$mouseentry - $rec1ind}]]
        }
    }
}

proc showinfo_menu_wind {w} {
# display full pathname of a file entry of the windows menu
# as a showinfo
    global pad FirstBufferNameInWindowsMenu listoffile listoftextarea
    set mouseentry [$w index active]
    if {$mouseentry=="none"} {
        # can happen when hovering over a separator
        # we must return otherwise the last menu entry is used for
        # the showinfo
        return
    }
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

proc ca {menucommand} {
# look up the bindings array to see if there is a binding defined
# for the given command, and in case generate a -command -accelerator pair
    set event [findbinding $menucommand]
    if {$event == ""} {
        return "-command {$menucommand}"
    } else {
        regsub -all {Control} $event {Ctrl} kevent
        regsub -all {underscore} $kevent {_} kevent
        regsub -all {backslash} $kevent {\\\\} kevent
        regsub -all {slash} $kevent {/} kevent
        regsub -all {percent} $kevent {%} kevent
        regsub -all {exclam} $kevent {!} kevent
        regsub -all {Key-} $kevent {} kevent
        regsub -all {\-} $kevent {+} kevent
        regsub -all {><} $kevent { } kevent
        regsub {>\Z} $kevent {} kevent
        regsub {\A<} $kevent {} kevent
        return "-command {$menucommand} -accelerator \"$kevent\""
    }
}

proc filteroutmenuclones {listofwidgets} {
# take a list of widget names and return this list without the names
# denoting menues of type menubar or tearoff, which are menu clones
# Reference:
# http://groups.google.com/group/comp.lang.tcl/browse_frm/thread/87adc111127063bc/05efee764b23540d
    set nomenubar [list ]
    foreach item $listofwidgets {
        if {[winfo class $item] != "Menu"} {
            lappend nomenubar $item
        } else {
            if {[$item cget -type] != "menubar" && [$item cget -type] != "tearoff"} {
                lappend nomenubar $item
            } else {
                # drop it
            }
        }
    }
    return $nomenubar
}
