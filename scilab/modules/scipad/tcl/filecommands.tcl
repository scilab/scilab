#####################################################################
#
# About the variables used:
#
#   $winopened
#       Opened buffer ID.
#       $winopened starts from 1 (1 is the first buffer opened)
#       It is never decreased
#       It is increased each time a new file is opened
#       A file saved as another name keeps its $winopened value
#       The radionbutton entry in the windows menu has a -value
#       option containing the $winopened consistent with the name
#       of the menu entry (see below at the end of this comment)
#
#   $textareaid
#       This is the unique identifier of the text widget displaying
#       the content of a given file. When a new textarea is created,
#       it is given the $winopened value in $textareaid
#
#   $pad.new$textareaid
#       Buffer name. This is the unique pathname of the text widget
#       displaying the content of a file. This is usually referred to
#       as $textarea, or $ta for short
#       This text widget is packed in a frame that is itself added
#       as a pane in possibly nested panedwindows
#
#   listoftextarea
#       Contains the list of the above opened textarea names.
#       Order of the elements matters (increasing order mandatory,
#       i.e. $pad.newX must be placed before $pad.newY if X < Y)
#       The order of the buffers in $listoftextarea is actually the
#       order of opening of the buffers (note: the order in the
#       Windows menu is only a display order)
#
#
#   A textarea name $ta can be used as a pointer to file or buffer
#   attributes:
#
#     listoffile("$ta",fullname)
#       Full path+name of the file on disk that is displayed in $ta
#       Exception: new file not yet saved on disk. In this case it is
#       the same as listoffile("$ta",displayedname)
#
#     listoffile("$ta",displayedname)
#       Displayed name of the file on disk that is displayed in $ta
#       This may be:
#         - the shortest unambiguous reference to that file
#         - the full pathname of that file
#       The displayed name does not contain the underlined number shown
#       in the Windows menu - This number is only found in the -label of
#       this menu
#
#     listoffile("$ta",new)
#       0: file was opened from disk
#       1: file is a new file
#
#     listoffile("$ta",save)
#       0: file is unmodified
#       1: file has been modified and should be saved before leaving Scipad
#      Note: Starting from Scipad 5.3, this setting is deprecated and replaced
#            by the text widget embedded modified flag associated to the undo
#            stack - it shouldn't be found any longer in the source code
#
#     listoffile("$ta",readonly)
#       0: file can be written
#       1: file is read only
#
#     listoffile("$ta",thetime)
#       Time of the last file modification on disk
#
#     listoffile("$ta",language)
#       Language scheme. Currently can be scilab, xml, or none
#
#     listoffile("$ta",redostackdepth)
#       Depth of the redo stack. Used for enabling/disabling the redo menu entry
#
#     listoffile("$ta",progressbar_id)
#       If colorization is in progress, this is the progressbar identifier
#       Otherwise it's an empty string
#
#     listoffile("$ta",colorize)
#       true: file gets colorized
#       false: no colorization for this file
#
#   The windows menu entries are radionbuttons, with the following
#   properties:
#     -value is $winopened
#     -label is $listoffile("$ta",displayedname), $ta being $pad.new$winopened
#     This label is prepended by an underlined number (for the first 9 entries
#     only)
#     This label is also appended by a peer identifier <X> if relevant
#     All the labels of the menu are different at any time, except
#     during ambiguities removal (In the Options menu, item Filenames does
#     not propose any option that would lead to ambiguous file names)
#     It is very important to maintain this property throughout the code
#     because extractindexfromlabel relies on this and is used everywhere.
#     Actually what must be different is the real filename, i.e. the label
#     without the leading underlined number and without the peer identifier
#
#
#####################################################################

##################################################
# procs for brand new buffers
##################################################
proc filesetasnew {} {
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
    showtext $pad.new$winopened
    resetmodified $pad.new$winopened
}

##################################################
# procs closing buffers
##################################################
proc closecurfile {quittype} {
# close current file, i.e. close the current textarea and also
# all its peers
# confirmation for saving is asked only once
    set peerslist [getpeerlist [gettextareacur]]
    set wascanceled [closecur $quittype]
    if {$wascanceled == "Canceled"} {
        # do nothing
    } else {
        # assert: $wascanceled == "Done"
        # close peers without asking again for confirmation
        foreach peerta $peerslist {
            showtext $peerta
            closecur "NoSaveQuestion"
        }
    }
    # there must be no code here for the Cancel button to work OK
    return $wascanceled
}

proc closecurtile {quittype} {
# close current tile
# this proc closes the current tile, i.e. the current textarea
# if the current textarea has peers, no confirmation is asked (and
# this is justified because there is at least one peer that contain
# the file information)
# if the current textarea has no peers, the normal confirmation
# process is required (which is required to prevent the user from
# loosing data
    if {[llength [getpeerlist [gettextareacur]]] >= 1} {
        closecur "NoSaveQuestion"
    } else {
        closecur $quittype
    }
}

proc closecur { {quittype yesno} } {
# close current textarea
# if the current textarea is the last one to contain a given file, then
# the file is closed (as seen from the user, actually the file closure on
# disk occurred right after reading it from disk)
# possible options for $quittype are yesno, yesnocancel and "NoSaveQuestion"
# return value can be "Done" or "Canceled" (see proc closefile), which is used
# by proc exitapp
# this proc first checks that there is no tile proc running (attempts to
# close a buffer while splitting buffers for instance would throw errors)
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    disablemenuesbinds
    set outvalue [closefile [gettextareacur] $quittype]
    catch {restoremenuesbinds} ; # catched because otherwise pad is unknown after last buffer close
    return $outvalue
}

proc closefile {textarea {quittype yesno} } {
# close textarea $textarea
# possible options for $quittype are yesno, yesnocancel and "NoSaveQuestion"
# return value can be:
#   - "Done": the file has been closed, after having been perhaps saved first
#             if the user answered yes to the confirmation question (if such
#             a question was required to be asked)
#   - "Canceled": the user answered cancel to the confirmation question
# the return value is used by proc exitapp
# this proc does not check that tile procs might be running, therefore it
# should only be called from proc closecur
    global listoffile pad
    global closeinitialbufferallowed

    # query the modified flag, ask for user confirmation only if adequate
    if  {[ismodified $textarea] && \
         $quittype != "NoSaveQuestion"} {

        # ask the user if buffer should be saved before closing
        set answer [tk_messageBox -message [ concat [mc "The contents of"] \
           $listoffile("$textarea",fullname) \
           [mc "may have changed, do you wish to save your changes?"] ] \
             -title [mc "Save Confirm?"] -type $quittype -icon question]
        switch -- $answer {
            yes {
                filetosave $textarea
                byebye $textarea
            }
            no {
                byebye $textarea
            }
            cancel {
                return "Canceled"
            }
        }

    } else {
        # buffer was not modified, so just close it
        set closeinitialbufferallowed false
        byebye $textarea
    }

    return "Done"
}

proc byebye {textarea} {
    global listoftextarea listoffile
    global pad FirstBufferNameInWindowsMenu pwframe

    if { [llength $listoftextarea] > 1 } {

        # check on Scilab busy needed to remove breakpoints

        if {![isscilabbusy]} {
            removescilabbuffer_bp "with_output" $textarea
        }

        removefuns_bp $textarea

        focustextarea $textarea

        # delete the textarea entry in the listoftextarea
        set listoftextarea [lreplace $listoftextarea [lsearch \
              $listoftextarea $textarea] [lsearch $listoftextarea $textarea]]

        # delete the windows menu entry
        set ilab [extractindexfromlabel $pad.filemenu.wind \
                  $listoffile("$textarea",displayedname)]
        $pad.filemenu.wind delete $ilab

        # refresh peer identifiers in the windows menu and title bars
        #   first decrease peer id for required buffers
        foreach {dname removedpeerid} [removepeerid $listoffile("$textarea",displayedname)] {}
        foreach peerta [getpeerlist $textarea] {
            set ilab [extractindexfromlabel $pad.filemenu.wind \
                     $listoffile("$peerta",displayedname)]
            foreach {dname peerid} [removepeerid $listoffile("$peerta",displayedname)] {}
            if {$peerid > $removedpeerid} {
                set dname [appendpeerid $dname [expr {$peerid - 1}]]
                set listoffile("$peerta",displayedname) $dname
                setwindowsmenuentrylabel $ilab $dname
            }
        }
        #   second, if there is only one peer buffer remaining, then remove its peer id
        set peerslist [getpeerlist $textarea]
        if {[llength $peerslist] == 1} {
            set ilab [extractindexfromlabel $pad.filemenu.wind \
                     $listoffile("$peerslist",displayedname)]
            foreach {dname peerid} [removepeerid $listoffile("$peerslist",displayedname)] {}
            set listoffile("$peerslist",displayedname) $dname
            setwindowsmenuentrylabel $ilab $dname
        }
        # refresh panes titles (actually only needed for peers)
        updatepanestitles

        # delete the textarea entry in listoffile
        unset listoffile("$textarea",fullname)
        unset listoffile("$textarea",displayedname)
        unset listoffile("$textarea",new)
        unset listoffile("$textarea",thetime)
        unset listoffile("$textarea",language)
        unset listoffile("$textarea",colorize)
        unset listoffile("$textarea",readonly)
        unset listoffile("$textarea",redostackdepth)
        unset listoffile("$textarea",progressbar_id)

        # the rest of this proc is similar to proc hidetext,
        # but not identical
        # unpack the textarea
        if {[llength $listoftextarea] <= [gettotnbpanes]} {
            destroypaneframe $textarea
        }

        # destroy the textarea widget itself
        destroy $textarea

        # place as current textarea the last one that is not already visible
        set i [getlasthiddentextareamenuind]
        if {$i == ""} {set i $FirstBufferNameInWindowsMenu}
        $pad.filemenu.wind invoke $i
        RefreshWindowsMenuLabelsWrtPruning

        # remove tile title if there is a single pane
        if {[gettotnbpanes] == 1} {
            set visibletapwfr [lindex [array get pwframe] 1]
            pack forget $visibletapwfr.topbar
        }

    } else {
        killscipad
    }
}

proc killscipad {} {
# kill main window and save preferences on exit
    global pad WMGEOMETRY WMSTATE

    #save the geometry for the next time
    set WMSTATE [wm state $pad]
    if {$WMSTATE == "zoomed"} {
        # restore normal (i.e. non-maximized) state before saving
        # the output of wm geometry, otherwise dimensions are large
        # when relaunching Scipad
        wm state $pad "normal"
    }
    set WMGEOMETRY [wm geometry $pad]

    savepreferences

    destroy $pad

    # The following line is needed in case Scipad is launched directly
    # in wish (debug purposes)
    # The two instructions *must* be in this order, and in a single catch
    # structure:
    # Scipad in Scilab: console hide throws an error (catched), destroy . is
    # not executed (it must not be executed because otherwise Scipad cannot
    # be launched again after first close)
    # Scipad in wish: both instructions are executed and are needed, especially
    # the destroy . because this kills the wish process that would still be
    # alive in the computer otherwise
    catch {console hide ; destroy .}
    unset pad
}

proc idleexitapp {} {
# exit Scipad
# this proc is called when quitting Scipad from Scipad (file menu, or
# upper-right cross of the window)
# exitapp will fail for instance when the user tries to close Scipad
# using [x] or File/Exit when Scipad is opening files at the same time
# (dnd of a directory with many files)
    global pad
    if {[catch {after idle {exitapp yesnocancel}}]} {after idle {killscipad}}
}

proc exitapp { {quittype yesno} } {
# exit Scipad
# this proc is called when quitting Scipad from Scilab (see scilab.quit),
# or it can be called by proc idleexitapp
    global listoftextarea

    # stop searching in files
    cancelsearchinfiles

    # stop debugger and clean Scilab state
    if {[getdbstate] == "DebugInProgress"} {
        # note that the abort below is useless: it executes in a ScilabEval sync,
        # i.e. in a dedicated parser, and just aborts in that parser without
        # interfering with what's happening in the main parser
        # <TODO>: find a way to abort in the main parser when this parser is busy
        #         hint: this is just impossible with the current amount of control
        #               we have on Scilab from Tcl!
        ScilabEval_lt "\"delbpt();abort\"" "sync" "seq"
        cleantmpScilabEvalfile
    }

    # close each buffer one after the other
    foreach textarea [shiftlistofta [filteroutpeers $listoftextarea] [gettextareacur]] {
        showtext $textarea
        set wascanceled [closecurfile $quittype]
        if {$wascanceled == "Canceled"} {
            break
        }
    }
    # there must be no code here for the Cancel button to work OK
}

##################################################
# procs for opening buffers from disk
##################################################
proc opensourceof {} {
# open a dialog for selection of the function whose source the user
# wants to open
    global pad menuFont textFont
    global opensoflb opensofbOK

    if {[isscilabbusy 0]} {return}

    set opensof $pad.opensof
    catch {destroy $opensof}
    toplevel $opensof
    wm title $opensof [mc "Open source of..."]

    frame $opensof.f1
    label $opensof.f1.l1 -text [mc "Open source of:"] -font $menuFont
    entry $opensof.f1.entry \
        -font $textFont -exportselection 0 \
        -validate key -validatecommand "updatecompletions %P %d"
    pack $opensof.f1.l1 $opensof.f1.entry -side left
    pack configure $opensof.f1.entry -expand 1 -fill x -padx 5
    pack $opensof.f1 -fill x

    frame $opensof.f2
    set opensoflb $opensof.f2.lb
    scrollbar $opensof.f2.sb -command "$opensoflb yview"
    listbox $opensoflb -height 4 -width 10 -font $textFont \
        -yscrollcommand "$opensof.f2.sb set" -takefocus 0
    pack $opensoflb $opensof.f2.sb -side left -padx 2
    pack configure $opensoflb -expand 1 -fill both 
    pack configure $opensof.f2.sb -fill y
    pack $opensof.f2 -expand 1 -fill both

    set opensofbOK $opensof.f3.buttonOK
    frame $opensof.f3
    set bestwidth [mcmaxra "OK" \
                           "Cancel"]
    button $opensofbOK -text "OK" \
            -command "OKopensourceof $opensof" \
            -width $bestwidth -font $menuFont
    button $opensof.f3.buttonCancel -text [mc "Cancel"] \
            -command "destroy $opensof" \
            -width $bestwidth -font $menuFont
    pack $opensofbOK $opensof.f3.buttonCancel -side left -padx 10
    pack $opensof.f3 -pady 4 -after $opensof.f2

    # arrange for the buttons to disappear last
    # when the window size is reduced
    pack configure $opensof.f2 -after $opensof.f3
    pack configure $opensof.f2 -side bottom
    pack configure $opensof.f3 -side bottom
    pack configure $opensof.f1 -after $opensof.f3

    update
    setwingeom $opensof
    wm resizable $opensof 1 1 
    wm minsize $opensof [winfo width $opensof] [winfo height $opensof]

    bind $opensof <Return> {OKopensourceof %W}
    # bind to the listbox only, otherwise quick clicks on the scrollbar
    # triggers a validation of the currently selected item, which is
    # not desirable
    bind $opensof.f2.lb <Double-Button-1> {OKopensourceof %W}
    bind $opensof <Escape> "destroy $opensof"
    bind $opensof <Up>   {scrollarrows_bp $opensoflb up}
    bind $opensof <Down> {scrollarrows_bp $opensoflb down}
    bind $opensof <MouseWheel> {if {%D<0} {scrollarrows_bp $opensoflb down}\
                                          {scrollarrows_bp $opensoflb up}   }
    $opensofbOK configure -state disable
    focus $opensof.f1.entry
}

proc updatecompletions {partialfunnametoopen edittype} {
# update the completions list in the listbox of the "open source of" dialog

    global bgcolors fgcolors
    foreach c1 "$bgcolors $fgcolors" {global $c1}
    global opensoflb opensofbOK

    $opensoflb delete 0 end

    set compl [getcompletions $partialfunnametoopen "scilab"]

    # populate the listbox with the possible completions
    foreach posscompl $compl {
        set tag [lindex $posscompl 0]
        set completedword [lindex $posscompl 1]
        # colorization stuff
        switch -- $tag {
            libfun  {set col $LFUNCOLOR}
            scicos  {set col $SCICCOLOR}
            userfun {set col $USERFUNCOLOR}
            default {}
        }
        if {$tag == "libfun" || $tag == "scicos" || $tag == "userfun"} {
            $opensoflb insert end $completedword
            $opensoflb itemconfigure end \
                    -foreground $col -selectforeground $col\
                    -background $BGCOLOR
            $opensoflb itemconfigure end -selectbackground [shade \
                [$opensoflb itemcget end -selectforeground] \
                [$opensoflb itemcget end -background] 0.5]
        }
    }

    set nbcompl [$opensoflb size]
    if {$nbcompl == 0} {
        # ring the bell only when inserting chars
        if {$edittype == 1} {
            bell
        }
        $opensofbOK configure -state disabled
    } else {
        $opensoflb selection set 0
        $opensoflb see 0
        $opensofbOK configure -state normal
    }

    return 1
}

proc OKopensourceof {w} {
# get the function name to open, and open the corresponding file
    global opensoflb opensofbOK
    if {[$opensofbOK cget -state] == "normal"} {
        set nametoopen [$opensoflb get [$opensoflb curselection]]
        set keywtype [gettagfromkeyword $nametoopen]
        doopenfunsource $keywtype $nametoopen
        destroy [winfo toplevel $w]
    } else {
        bell
    }
}

proc openlibfunsource {ind} {
    global textareacur
    if {[isscilabbusy 0]} {return}
    # exit if the cursor is not by a libfun or a scicos or a userfun keyword
    set keywtype ""
    if {[lsearch [$textareacur tag names $ind] "scicos"] !=-1} \
       {set keywtype scicos}
    if {[lsearch [$textareacur tag names $ind] "libfun"] !=-1} \
       {set keywtype libfun}
    if {[lsearch [$textareacur tag names $ind] "userfun"] !=-1} \
       {set keywtype userfun}
    if {$keywtype==""} return
    set lrange [$textareacur tag prevrange $keywtype "$ind+1c"]
    if {$lrange==""} {set lrange [$textareacur tag nextrange $keywtype $ind]}
    set curterm [$textareacur get [lindex $lrange 0] [lindex $lrange 1]]
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        if {$curterm!=""} {
            doopenfunsource $keywtype $curterm
        }
    }
}

proc doopenfunsource {keywtype nametoopen} {
# do the function source file opening
# $nametoopen is the keyword whose source is to be opened
# $keywtype is its associated tag (must be libfun or scicos
# or userfun to be relevant)
    global words scicosdir blocksdir
    switch $keywtype {
        "libfun" {
            # the simplest thing to do here would have been to
            #   ScilabEval_lt "scipad(get_function_path(\"$nametoopen\"))" "seq"
            # but unfortunately this does not work when used from the debugger
            # call stack area (the shell goes deeper one level and execution is
            # delayed), therefore a more complicated solution here
            set comm1 "TCL_EvalStr(\"set fullfunpathtoopen \"+strsubst(get_function_path(\"$nametoopen\"),\"\\\",\"/\"),\"scipad\");"
            set comm2 "TCL_EvalStr(\"openfile \"\"\$fullfunpathtoopen\"\"\",\"scipad\");"
            set fullcomm [concat $comm1 $comm2]
            ScilabEval_lt $fullcomm "seq"
        }
        "scicos" {
            set globpat_scicosdir [file join "$scicosdir" $nametoopen.sci]
            set globpat_blocksdir [file join "$blocksdir" "*" $nametoopen.sci]
            set filetoopen [glob $globpat_scicosdir $globpat_blocksdir]
            # for the same reason as above, simply issuing
            #   ScilabEval_lt "scipad(\"$filetoopen\")" "seq"
            # does not always work
            ScilabEval_lt "TCL_EvalStr(\"openfile \"\"$filetoopen\"\"\",\"scipad\");" "seq"
        }
        "userfun" {
            set nameinitial [string range $nametoopen 0 0]
            set candidates $words(scilab.$keywtype.$nameinitial)
            for {set i 0} {$i<[llength $candidates]} {incr i} {
                if {[lindex [lindex $candidates $i] 0] == $nametoopen} {
                    dogotoline "physical" 1 "function" [lindex $candidates $i]
                    break
                }
            }
        }
    }
}

proc openlistoffiles {filelist tiledisplay} {
# open many files at once - for use with file list provided by TkDnD
# the open dialog is not shown
# in case a directory is given, open all the files in that directory
    foreach f $filelist {
        regsub "^file:" $f "" f
        # in unix, .* files are not matched by *, but .* matches . and ..
        # If we don't exclude them, we have infinite recursion
        if {[file tail $f] == "." | [file tail $f] == ".."} continue
        if {[file isfile $f] == 1} {
            openfile $f $tiledisplay
        } elseif {[file isdirectory $f] == 1} {
            openlistoffiles [glob -nocomplain -directory $f -types hidden *] $tiledisplay
            openlistoffiles [glob -nocomplain -directory $f -types {f d} *] $tiledisplay
        } else {
            # In windows this never happened to us, but linux applications
            # allow sometimes drag of e.g. http:// or ftp://; moreover
            # spaces in namefiles can produce unexpected results
            tk_messageBox -title [mc "Weird drag&drop"] -message [concat $f \
            [ mc "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism." ] ]
        }
    }
}

proc showopenwin {tiledisplay} {
# bring up the open dialog for file selection
# if file is not already open, open it
# otherwise just switch buffers to show it
# the new buffer is displayed in the current tile if
# $tiledisplay == "currenttile", otherwise in a new tile
# created by splitting the current one horizontally
# in which case $tiledisplay == "horizontal", or
# vertically in which case $tiledisplay == "vertical"
    global pad winopened listoffile
    global startdir
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    showinfo [mc "Open file"]
    # remember the latest path used for opening files
    if {![info exists startdir]} {set startdir [pwd]}
    set file [tk_getOpenFile -filetypes [knowntypes] -parent $pad \
                             -initialdir $startdir -multiple 1]
    if {[llength $file] > 0} {
        set startdir [file dirname [lindex $file 0]]
        openlistoffiles $file $tiledisplay
    }
}

proc openfileifexists {file} {
# wrapper to openfile, but issues a warning if the file does not exist (anymore).
# if file is already open, action is the same as hitting the entry in the windows
# menu
    global listoftextarea listoffile pad
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    set alreadyopen "false"
    foreach ta [filteroutpeers $listoftextarea] {
        if {$listoffile("$ta",fullname) == $file} {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            $pad.filemenu.wind invoke $i
            set alreadyopen "true"
            break
        }
    }
    if {$alreadyopen == "true"} {
        return 1
    } else {
        if {[file exist $file]} {
             return [openfile $file]
        } else {
             set answer \
                [tk_messageBox -type yesno -icon question \
                 -title [mc "File not found"] -message "[mc "The file"]\
                  $file [mc "does not exist anymore. Do you want to create an\
                  empty file with the same name?"]"]
             switch -- $answer {
               yes {return [openfile $file]}
               no  {return 0}
               }
        }
    }
}

proc openfile {file {tiledisplay "currenttile"}} {
# try to open a file with filename $file (no file selection through a dialog)
# if file is not already open, open it
# otherwise just switch buffers to show it
# return value:
#    0 if file could not be open
#    1 if file could be open or displayed (switched buffers)
    global pad winopened listoftextarea listoffile
    global closeinitialbufferallowed startdir

#hack for bringing up the chooser, if $file is a directory
# on windows this has to precede the check for readable,
#  because a directory is "unreadable"
    if {[file isdirectory $file]} {
        set startdir $file
        showopenwin currenttile; 
        return
    }

    if {[fileunreadable $file]} {return 0}

    # ignore windows shortcut since nothing is implemented to follow them
    # and to open the target they point to
    if {[fileiswindowsshortcut $file]} {return 0}

    if {[string compare $file ""]} {
        # search for an opened existing file
        set res [lookiffileisopen "$file"]
        if {$res == 0} {
            notopenedfile $file
            if {[file exists $file]} {
                set listoffile("$pad.new$winopened",thetime) [file mtime $file]
                set listoffile("$pad.new$winopened",new) 0
                shownewbuffer $file $tiledisplay
            } else {
                set listoffile("$pad.new$winopened",thetime) 0
                set listoffile("$pad.new$winopened",new) 1
                lappend listoftextarea $pad.new$winopened
                if {$closeinitialbufferallowed == true} {
                    set closeinitialbufferallowed false
                    closefile $pad.new1
                }
                showtext $pad.new$winopened
                RefreshWindowsMenuLabelsWrtPruning
            }
            $pad.new$winopened mark set insert "1.0"
            keyposn $pad.new$winopened
            newfilebind
        } else {
            fileisopen $file
            $pad.filemenu.wind invoke $res
        }
        return 1
    } else {
        return 0
    }
}

proc lookiffileisopen {file} {
# Return zero if $file is not already open, i.e. if the full path
# of $file is not already in some $listoffile("$textarea",fullname)
# If the file is already open, return the number of the windows menu
# entry to invoke in order to display this buffer
    global pad listoffile listoftextarea
    set ilab 0
    set fpf [file normalize $file]
    foreach textarea [filteroutpeers $listoftextarea] {
        if {$listoffile("$textarea",fullname) == $fpf} {
            set ilab [extractindexfromlabel $pad.filemenu.wind $listoffile("$textarea",displayedname)]
            break
        }
    }
    return $ilab
}

proc notopenedfile {file} {
# $file is not opened - this sets the $listoffile area values for that file
# and adds an entry in the windows menu
    global winopened pad listoffile
    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",fullname) [file normalize $file]
    set listoffile("$pad.new$winopened",displayedname) \
            [file tail $listoffile("$pad.new$winopened",fullname)]
    set listoffile("$pad.new$winopened",language) [extenstolang $file]
    setlistoffile_colorize "$pad.new$winopened" $listoffile("$pad.new$winopened",fullname)
    set listoffile("$pad.new$winopened",new) 0
    if {[file exists $file]} {
        set listoffile("$pad.new$winopened",readonly) [fileunwritable $file]
    } else {
        set listoffile("$pad.new$winopened",readonly) 0
    }
    set listoffile("$pad.new$winopened",redostackdepth) 0
    set listoffile("$pad.new$winopened",progressbar_id) ""

    addwindowsmenuentry $winopened $listoffile("$pad.new$winopened",displayedname)
}

proc shownewbuffer {file tiledisplay} {
    global pad winopened closeinitialbufferallowed
    if {[fileunreadable $file]} {return}
    openfileondisk $pad.new$winopened $file $tiledisplay
    resetmodified $pad.new$winopened
    # colorization must be launched before showing the textarea
    # so that foreground colorization while stepping into
    # libfun ancillaries works (example with calfrq)
    backgroundcolorize $pad.new$winopened
    tagcontlines $pad.new$winopened
    if {$tiledisplay == "currenttile"} {
        showtext $pad.new$winopened
    } else {
        set closeinitialbufferallowed false
        # pack the new buffer in a splitted window
        splitwindow $tiledisplay $pad.new$winopened
    }
    RefreshWindowsMenuLabelsWrtPruning
    AddRecentFile [file normalize $file]
}

proc newfilebind {} {
    global pad winopened textareaid
    bind $pad.new$winopened <KeyRelease> {catch {keyposn %W}}
    TextStyles $pad.new$winopened
    set textareaid $winopened
    tkdndbind $pad.new$winopened
}

proc fileisopen {file} {
# file is already opened
    tk_messageBox -type ok -title [concat [mc "Open file"] $file] -message [concat \
      [mc "The file"] $file [mc "is already opened! Save the current opened\
      file to another name and reopen it from disk!"] ]
}

proc openfileondisk {textarea thefile tiledisplay} {
# really open/read a file from disk
# all readability tests have normally been done before
    global listoftextarea pad closeinitialbufferallowed
    set msgWait [mc "Wait seconds while loading and colorizing file"]
    showinfo $msgWait
    lappend listoftextarea $textarea
    if {$closeinitialbufferallowed == true && $tiledisplay == "currenttile" } {
        set closeinitialbufferallowed false
        closefile $pad.new1
    }
    set newnamefile [open $thefile r]
    while {![eof $newnamefile]} {
        $textarea insert end [read -nonewline $newnamefile ]
    }
    close $newnamefile
}

##################################################
# procs for saving buffers to disk
##################################################
proc filetosavecur {} {
# save current buffer to disk
    filetosave [gettextareacur]
}

proc filetosaveall {} {
# save all modified buffers to disk
    global listoftextarea
    foreach ta [filteroutpeers $listoftextarea] {
        if {[ismodified $ta]} {
            filetosave $ta
        }
    }
}

proc filesaveascur {} {
# save current buffer to disk as...
    filesaveas [gettextareacur]
}

proc filetosave {textarea} {
# check if file on disk has changed (i.e. did another application modify
# the file on disk?) since last loading in Scipad
# and ask the user if he still wants to save in that case
# if yes or if there was no change on disk, perform the save on disk
# note: the modification check is almost pointless now since
# Scipad performs this check each time it gets focus - it is kept
# to deal with the perverse case where some external process modifies
# the file while Scipad keeps focus
    global listoffile
    if {[fileexistsondisk $textarea]} {
        if {[filetimeondiskisdifferent $textarea]} {
            set msgChanged [concat [mc "The contents of "] \
                                   $listoffile("$textarea",fullname) \
                                   [mc "has changed on disk, save it anyway?"] ]
            set msgTitle [mc "File has changed!"]
            set answer [tk_messageBox -message $msgChanged -title $msgTitle \
                            -type yesnocancel -icon question]
            switch -- $answer {
                yes {
                      if {![writesave $textarea $listoffile("$textarea",fullname)]} {
                          filesaveas $textarea
                      }
                    }
                no     {}
                cancel {}
            }
        } else {
            if {![writesave $textarea $listoffile("$textarea",fullname)]} {
                filesaveas $textarea
            }
        }
    } else {
        filesaveas $textarea
    }
}

proc filesaveas {textarea} {
# bring up the Save As... dialog so that the user can pick up a file name
# and do the save under that filename
    global listoffile pad
    global startdir

    # filesaveas cannot be executed since it uses getallfunsintextarea
    # which needs the colorization results
    if {[colorizationinprogress]} {return}

    showinfo [mc "Save as"]

    # remember the latest path used for opening files
    # proposedname is the first function name found in the buffer
    if {![info exists startdir]} {set startdir [pwd]}
    set proposedname ""
    set firstfuninfo [lindex [getallfunsintextarea $textarea] 1]
    if {[lindex $firstfuninfo 0] != "0NoFunInBuf"} {
        set proposedname [lindex $firstfuninfo 0]
    }
    if {$listoffile("$textarea",new)==0 || $proposedname==""} {
        foreach {proposedname peerid} [removepeerid $listoffile("$textarea",displayedname)] {}
    } else {
        set proposedname $proposedname.sci
    }

    set writesucceeded 0
    set myfile [tk_getSaveFile -filetypes [knowntypes] -parent $pad \
                    -initialfile $proposedname -initialdir $startdir]

    if {$myfile != ""} {
        set startdir [file dirname $myfile]
        set writesucceeded [writesave $textarea $myfile]
        while {!$writesucceeded} {
            set myfile [tk_getSaveFile -filetypes [knowntypes] -parent $pad \
                            -initialfile $proposedname -initialdir $startdir]
            if {$myfile != ""} {
                set startdir [file dirname $myfile]
                set writesucceeded [writesave $textarea $myfile]
            } else {
                break
            }
        }
    }

    if {$writesucceeded} {
        foreach ta [getfullpeerset $textarea] {
            set listoffile("$ta",fullname) [file normalize $myfile]
            set listoffile("$ta",new) 0
        }
        set ilab [extractindexfromlabel $pad.filemenu.wind \
                  $listoffile("$textarea",displayedname)]
        foreach {dname peerid} [removepeerid $listoffile("$textarea",displayedname)] {}
        set listoffile("$textarea",displayedname) \
                [file tail $listoffile("$textarea",fullname)]
        set dname [appendpeerid $listoffile("$textarea",displayedname) $peerid]
        set listoffile("$textarea",displayedname) $dname
        setwindowsmenuentrylabel $ilab $listoffile("$textarea",displayedname)

        # refresh peer identifiers in the windows menu and title bars
        foreach peerta [getpeerlist $textarea] {
            set ilab [extractindexfromlabel $pad.filemenu.wind \
                     $listoffile("$peerta",displayedname)]
            foreach {dname peerid} [removepeerid $listoffile("$peerta",displayedname)] {}
            set dname [appendpeerid $listoffile("$textarea",displayedname) $peerid]
            set listoffile("$peerta",displayedname) $dname
            setwindowsmenuentrylabel $ilab $dname
        }
        # refresh panes titles (actually only needed for peers)
        updatepanestitles

        RefreshWindowsMenuLabelsWrtPruning
        AddRecentFile $listoffile("$textarea",fullname)
    }
}

proc writesave {textarea nametosave} {
# generic save function - write a file onto disk if it is writable
# return value:
#    1  file was successfully written
#    0  otherwise
    global listoffile
    # if the file exists, check if the file is writable 
    # if it doesn't, check if the directory is writable
    # (case of Save as...) (non existent files return 0 to writable)
    if {[file exists $nametosave]} {
        set readonlyflag [fileunwritable $nametosave]
    } else {
        set readonlyflag [fileunwritable [file dirname $nametosave]]
    }
    if {$readonlyflag==0} {
        # writefileondisk catched to deal with unexpected errors (should
        # be none!)
        if {[catch {writefileondisk $textarea $nametosave}] == 0} {
            resetmodified $textarea
            foreach ta [getfullpeerset $textarea] {
                set listoffile("$ta",thetime) [file mtime $nametosave]
                set listoffile("$ta",readonly) $readonlyflag
            }
            set msgWait [concat [mc "File"] $nametosave [mc "saved"]]
            showinfo $msgWait
            # windows menu entries must be sorted so that order is
            # correct when sorting by MRU (the file that was just saved
            # has become the most recently used)
            sortwindowsmenuentries
            return 1
        } else {
            set msgWait [concat $nametosave [mc "cannot be written!"]]
            tk_messageBox -message $msgWait -icon error -type ok -title [mc "Save As"]
            return 0
        }
    } else {
        set msgWait [concat $nametosave [mc "cannot be written!"]]
        tk_messageBox -message $msgWait -icon error -type ok -title [mc "Save As"]
        return 0
    }
}

proc writefileondisk {textarea nametosave {nobackupskip 1}} {
# really write the file onto the disk
# all writability tests have normally been done before
    global filebackupdepth tcl_platform
    global pad

    if {$nobackupskip} {
        backupfile $nametosave $filebackupdepth
    }

    # open in mode "w" means "WRONLY CREAT TRUNC", and this should work OK
    # for all files, hidden or not, IMHO
    # but experience shows that "w" fails with existing hidden files on
    # Windows. See Tcl bug 1622579:
    # http://sourceforge.net/tracker/index.php?func=detail&aid=1622579&group_id=10894&atid=110894
    # However, the TRUNC flag is really needed (i.e. there should be the
    # full "w" mode) to overwrite files, otherwise writing overwrites
    # without truncating to zero length when opening, which results in
    # a wrong file when the new version is smaller than the previous
    # version on disk
    # Therefore, hidden files are treated specially on windows: first
    # the hidden flag is removed, the file is saved on disk and finally
    # the hidden flag is set back
    set specialtreatment 0
    if {$tcl_platform(platform) == "windows" && [file exists $nametosave]} {
        if {[file attributes $nametosave -hidden]} {
            set specialtreatment 1
        }
    }

    if {$specialtreatment} {
        file attributes $nametosave -hidden 0
    }

    # since save is allowed during debug (thus providing a mean to
    # save modifications performed before the debug was launched)
    # there is a need to filter out lines of the .sce debug wrapper
    # so that they are not included in the saved file
    # the solution used dumps the content of the buffer to save into
    # a temporary text widget and removes everything tagged as db_wrapper
    # before writing the file on disk
    # this needs more memory than the natural approach of saving line by
    # line, but it avoids a lot of headaches in determining which is the
    # last line not tagged (this line must be saved with the -nonewline
    # option of puts)
    set dbwrapranges [$textarea tag ranges db_wrapper]
    text $pad.temptextwidget
    $pad.temptextwidget insert 1.0 [$textarea get 1.0 "end-1c"]
    if {$dbwrapranges != {}} {
        eval "$pad.temptextwidget delete $dbwrapranges"
    }

    set FileNameToSave [open $nametosave w]
    puts -nonewline $FileNameToSave [$pad.temptextwidget get 1.0 end]
    close $FileNameToSave

    destroy $pad.temptextwidget

    if {$specialtreatment} {
        file attributes $nametosave -hidden 1
    }
}

proc savepreferences {} {
  global env listofpref listofpref_list
  set preffilename [file join $env(SCIHOME) .SciPadPreferences.tcl]
  catch {
    set preffile [open $preffilename w]
    foreach opt $listofpref {
        global $opt
        puts $preffile [concat "set $opt" \{[set $opt]\}]
    }
    foreach opt $listofpref_list {
        global $opt
        set value ""
        foreach item $opt {
            set value [concat $value [set $item]]
        }
        puts $preffile "set $opt \[list $value\]"
    }
    close $preffile
  }
}

proc backupfile { fname { levels 10 } } {
# before writing to a file $fname, call: backupfile $fname
# and the file will not get overwritten.
#
# renames like so: .bak, .ba2, .ba3, .ba4, etc.
#
# this proc was taken from http://wiki.tcl.tk/1641 and slightly adapted

    if {$levels==0} {return}

    if { [ catch {
        if { [ file exists $fname ] } {
            set dir [ file dirname $fname ]
            set files [ glob -nocomplain -path ${fname} .ba* ]
            set i $levels
            while { [ incr i -1 ] } {
                if { [ lsearch -exact $files ${fname}.ba$i ] > -1 } {
                    file rename -force ${fname}.ba$i ${fname}.ba[ incr i ]
                    incr i -1
                }
            }
            if {$levels >= 2} {
                if { [ file exists ${fname}.bak ] } {
                    file rename -force ${fname}.bak ${fname}.ba2
                }
            }
            file rename -force $fname ${fname}.bak
        }
    } err ] } {
        return -code error "backupfile($fname $levels): $err"
    }
}

##################################################
# revert to saved version of current buffer
##################################################
proc revertsaved {textarea {ConfirmFlag "ConfirmNeeded"}} {
    global listoffile
    set thefile $listoffile("$textarea",fullname) 
    if [ file exists $thefile ] {
# check for the perverse case that someone changed the file to unreadable
# in the meantime
        if {[fileunreadable $thefile]} {return}
        if {$ConfirmFlag == "ConfirmNeeded"} {
            set answer [tk_messageBox \
                -message [concat [mc "Revert"] $thefile [mc "to saved?"] ] \
                -type yesno -icon question -title [mc "Revert Confirm?"] ]
        } else {
            set answer "yes"
        }
        if {$answer == yes} {
            set oldfile [open $thefile r]
            $textarea delete 1.0 end 
            while {![eof $oldfile]} {
                $textarea insert end [read -nonewline $oldfile ] 
            }
            close $oldfile
            resetmodified $textarea
            foreach ta [getfullpeerset $textarea] {
                set listoffile("$ta",thetime) [file mtime $thefile]
            }
            showtext $textarea
            tagcontlines $textarea
            backgroundcolorize $textarea
        }
    }
}

proc checkiffilechangedondisk {textarea} {
# check if file on disk has changed (i.e. did another application modify
# the file on disk?) since last loading in Scipad
    global listoffile
    set msgChanged [concat [mc "The contents of "] \
                           $listoffile("$textarea",fullname) \
                           [mc "has been modified outside of Scipad. Do you want to reload it?"] ]
    set msgTitle [mc "File has changed!"]
    if {[filehaschangedondisk $textarea]} {
        # note: if thetime is not updated first, we may enter a recursion:
        # if revertsaved pops up the "unreadable file" warning, there is once
        # more a change of focus, which triggers this proc recursively.
        # An a posteriori rationale: the decision whether to keep the version
        # on disk or the one in memory is itself the most recent editing action.
        foreach ta [getfullpeerset $textarea] {
            set listoffile("$ta",thetime) [file mtime $listoffile("$textarea",fullname)]
        }
        set answer [tk_messageBox -message $msgChanged \
                        -title $msgTitle -type yesno -icon question]
        switch -- $answer {
            yes {revertsaved $textarea NoConfirm}
            no  {}
        }
    }
}

proc checkifanythingchangedondisk {w} {
# check if any opened buffer has changed on disk
    global listoftextarea pad
    # The test on $w below prevents from asking the confirmation question
    # multiple times since more than one single FocusIn event can be fired
    # e.g when focus is set to Scipad by clicking on a text widget: the binding
    # fires for .scipad but also for .scipad.newX because .scipad is in the
    # bindtags list for .scipad.newX
    if {$w != $pad} {
        return
    }
    foreach ta [filteroutpeers $listoftextarea] {
        checkiffilechangedondisk $ta
    }
}

##################################################
# ancillaries for file commands
##################################################
proc knowntypes {} {
# list of known file types - used for filtering items in the Open and Save dialogs
# and in the Find dialog
    set scifiles [mc "Scilab files"]
    set cosfiles [mc "Scicos files"]
    set xmlfiles [mc "XML files"]
    set allfiles [mc "All files"]
    set types [concat "{\"$scifiles\"" "{*.sce *.sci *.tst *.dem}}" \
                      "{\"$cosfiles\"" "{*.cosf}}" \
                      "{\"$xmlfiles\"" "{*.xml *.xsd *.dtd}}" \
                      "{\"$allfiles\"" "{*.* *}}" ]
    return $types
}

proc extenstolang {file} {
# given a file extension, return the associated language scheme
    set extens [string tolower [file extension $file]] 
    if {$extens == ".xml" | $extens == ".xsd" | $extens == ".dtd"} {
        return "xml"
    } elseif {$extens == ".sce" | $extens == ".sci" | $extens == ".cosf" | \
              $extens == ".tst" | $extens == ".dem"} {
        return "scilab"
    } else {
        return "none"
    }
}

proc direxists {dir} {
# check whether $dir is an existing directory
# return value:
#   1  $dir exists and is a directory
#   0  otherwise
    set ret 0
    if {[file exists $dir]} {
        if {[file isdirectory $dir]} {
            set ret 1
        }
    }
    return $ret
}

proc fileunreadable {file} {
# check readability of $file, and tell the user if $file is not readable
# (but keep silent if it is readable)
# $file might be a file name or a directory name, but if it is a directory
# name the return value will always be 1 (normally a directory is never given
# to proc fileunreadable in Scipad so this doesn't matter)
# return value:
#   1  $file exists and is not readable
#   0  otherwise (either $file does not exist or if it exists it is readable)
# note that this proc will answer 0 (not unreadable, i.e. readable), any time
# $file does not exist, and this is the intended behaviour
# comments about the way readability is checked:
#   The instruction file readable has a number of bugs in Tcl:
#     - Tcl bug 1394972: file readable answers 1 or 0 but the file is
#       locked therefore cannot actually be read
#       Note: this bug has been marked as WONTFIX in the Tcl tracker
#       since it is an OS limitation. On Windows there is no way to
#       know before trying to open a file that it is locked.
#       A test with constraint "knownBug" has been added in fCmd.test
#       (in the Tcl test suite)
#     - Tcl bug 1613456: file readable checks wrong permissions on a
#       Samba share - this bug has been filed in the Tcl tracked because
#       it is the root cause for Scilab bug 2243, at least for the part
#       of this bug related to readability (there is also a writability
#       issue reported in bug 2243)
#   The quest for a reliable way to check readability of a file has
# therefore led to directly trying to read the file and see whether it
# works or not. This approach still suffers from possible race conditions
# (but there would be such races anyway with file readable) but is
# believed to be better than to wait for the above bugs be possibly fixed

    if {![file exists $file]}  {
        return 0
    } else {
        if {[catch {set fileid [open $file r]}] == 0} {
            # the file can really be read
            close $fileid
            return 0
        } else {
            # the file cannot be read
            tk_messageBox -title [mc "Unreadable file"] \
                -message [concat [mc "The file"] $file \
                         [mc "exists but is not readable!"]] \
                -icon warning -type ok
            return 1
        }
    }
}

proc fileunwritable {file} {
# check writability of $file
# $file might be a file name or a directory name
# return value:
#   1  $file is not writable
#   0  otherwise
# note that this proc may answer 0 (not unwritable, i.e. writable), even
# if $file does not exist, and this is the intended behaviour: a new
# file can be written somewhere as soon as sufficient permissions exist
# comments about the way writability is checked:
#   The instruction file writable has a number of bugs in Tcl:
#     - Tcl bug 1613456: file writable checks wrong permissions on a
#       Samba share - this bug has been filed in the Tcl tracked because
#       it is the root cause for Scilab bug 2243, at least for the part
#       of this bug related to writability (there is also a readability
#       issue reported in bug 2243)
#     - Scilab bug 2319 seems to be also a writability problem on a
#       special distributed filesystem (DFS)
#   The quest for a reliable way to check writability of a file has
# therefore led to directly trying to write the file and see whether it
# works or not. This approach still suffers from possible race conditions
# (but there would be such races anyway with file writable) but is
# believed to be better than to wait for the above bugs be possibly fixed
#   If $file is a directory, then a test file is created in $file to
# check for writability in directory $file
#   Test files are immediately deleted after creation

    if {[file isdirectory $file]} {
        set file [file join $file Scipadwritabilitytestfile.txt ]
    }

    if {[file exists $file]}  {
        set preexistingfile true
    } else {
        set preexistingfile false
    }

    if {[catch {set fileid [open $file a]}] == 0} {
        # the file can really be written
        close $fileid
        if {!$preexistingfile} {
            # if the file did not exist before trying to open it in
            # append mode, then the open command created it above
            # the file should then be suppressed
            file delete -- $file
        }
        return 0
    } else {
        # the file cannot be written
        return 1
    }
}

proc filehaschangedondisk {ta} {
# return value:
#   1  the file opened in $ta exists on disk, is not a new file, and its last
#      saving time is different from the modify date retrieved from the file
#      on disk
#   0  otherwise
    global listoffile
    if {[fileexistsondisk $ta]} {
        if {[filetimeondiskisdifferent $ta]} {
            return 1
        }
    }
    return 0
}

proc fileexistsondisk {ta} {
# return value:
#   1  the file opened in $ta exists on disk and is not a new file
#   0  otherwise
    global listoffile
    if { [file exists $listoffile("$ta",fullname)] && \
         $listoffile("$ta",new) == 0 } {
        return 1
    }
    return 0
}

proc filetimeondiskisdifferent {ta} {
# return value:
#   1  the file saving time on disk is different from the time as known by
#      Scipad
#   0  otherwise
    global listoffile
    if {$listoffile("$ta",thetime) != [file mtime $listoffile("$ta",fullname)]} {
        return 1
    }
    return 0
}

proc setlistoffile_colorize {ta fullfilename} {
# set listoffile("$ta",colorize) to the right value according to the colorize
# option "always", "ask" or "never"
# the fullfilename parameter can be set to "", which is used for new files not
# yet saved on disk, and which have therefore no size - colorize defaults to
# true in the "ask" case
# this proc is supposed to be used only when:
#   - opening files from disk
#   - creating new files (not when creating peers)
# listoffile("$ta",colorize) can also be changed after opening of the
# file through the Scheme menu and proc switchcolorizefile

# note: no need in this proc to loop on [getfullpeerset $ta]
#       because it is only called when opening or creating a new file
#       (that has then no peers)

    global pad listoffile colorizeenable

    # arbitrary size in bytes above which Scipad will ask for colorization
    set sizelimit 130000

    if {$colorizeenable == "always"} {
        set listoffile("$ta",colorize) true

    } elseif {$colorizeenable == "ask"} {
        if {[catch {file size $fullfilename} fsize] != 0} {
            # file size cannot be obtained, maybe the file does not exist
            set listoffile("$ta",colorize) true
        } else {
            if {$fsize > $sizelimit} {
                set answ [tk_messageBox \
                     -message [mc "This file is rather large and colorization might take some time.\n\nColorize anyway?"] \
                     -title [mc "File size warning"] \
                     -parent $pad -icon warning -type yesno]
                if {$answ == "yes"} {
                    set listoffile("$ta",colorize) true
                } else {
                    set listoffile("$ta",colorize) false
                }
            } else {
                set listoffile("$ta",colorize) true
            }
        }

    } else {
        # assert: $colorizeenable == "never"
        set listoffile("$ta",colorize) false
    }
}

proc fileiswindowsshortcut {filename} {
# check whether $filename denotes a Windows shortcut or not   
# return value:
#   1  $filename is a Windows shortcut
#   0  otherwise (includes the case when $filename does not exist, as well
#      as anything else)

    # catched to allow to call this proc even if $filename does not exist
    # for instance
    if {[catch {iswindowsshortcut $filename} isashortcut] != 0} {
        return 0
    }

    if {$isashortcut} {
        tk_messageBox -title [mc "Windows shortcut file"] \
            -message [concat [mc "The file"] $filename \
                     [mc "is a Windows shortcut and will not be opened!"]] \
            -icon warning -type ok
        return 1
    } else {
        return 0
    }
}

proc iswindowsshortcut {filename} {
# on anything else than windows, return 0
# on windows, return 1 if $filename is a shortcut, and false otherwise
# $filename should contain the final (invisible) .lnk extension, but this
# is not the way this proc checks that the argument is a shortcut. Rather
# it reads the first 4 bytes of the file in binary mode and decides of the
# result based on those bytes
# See: http://www.i2s-lab.com/Papers/The_Windows_Shortcut_File_Format.pdf
# Notes:
#  . $filename is supposed to exist and be readable
#  . The GUID is not checked since it might change in the future when
#    Microsoft decides to change the shortcut file format
    global tcl_platform

    if {$tcl_platform(platform) != "windows"} {
        return 0
    }

    # $filetype might be undefined after reading the first bytes, if the file
    # size is zero for instance, therefore define a fallback value here so
    # that the check on that value will return false below
    set filetype {}

    set ch [open $filename r]
    fconfigure $ch -encoding binary -translation binary -eofchar {}
    binary scan [read $ch 4] i filetype
    close $ch

    # shortcut files are identified by their four first bytes being 0000004Ch
    # i.e. 76 in decimal, or letter "L" in ASCII
    if {$filetype != "76"} {
        return 0
    } else {
        return 1
    }
}

proc globtails {directoryname pat} {
# this is a replacement for the construct
#     glob -nocomplain -tails -directory $directoryname $pat
# because there is a Tcl bug with the -tails option with old
# Tcl versions (for instance with 8.4.1, at least on cygwin)
    set matchfiles [glob -nocomplain -directory $directoryname $pat]
    for {set i 0} {$i < [llength $matchfiles]} {incr i} {
        set matchfiles [lreplace $matchfiles $i $i [file tail [lindex $matchfiles $i]]]
    }
    return $matchfiles
}

##################################################
# procedures dealing with pruned file names
##################################################
proc RefreshWindowsMenuLabelsWrtPruning {} {
# Reset all labels to file tails, then remove ambiguities
# by expanding names as necessary
    global listoffile listoftextarea pad filenamesdisplaytype
    if {$filenamesdisplaytype != "full"} {
        # Reset all to file tails
        foreach ta $listoftextarea {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            foreach {dname peerid} [removepeerid $listoffile("$ta",displayedname)] {}
            set pn [file tail $listoffile("$ta",fullname)]
            lappend ind $i $pn $ta $peerid
        }
        foreach {i pn ta peerid} $ind {
            set pn [appendpeerid $pn $peerid]
            set listoffile("$ta",displayedname) $pn
            setwindowsmenuentrylabel $i $pn
        }
        # Detect duplicates and remove ambiguities
        foreach ta $listoftextarea {
            set tochange [IsPrunedNameAmbiguous $ta]
            if {$tochange != ""} {
                RemoveAmbiguity $tochange $ind
            }
        }
    } else {
        # always full file names are displayed, even if unambiguous
        foreach ta $listoftextarea {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            foreach {dname peerid} [removepeerid $listoffile("$ta",displayedname)] {}
            set pn [appendpeerid $listoffile("$ta",fullname) $peerid]
            set listoffile("$ta",displayedname) $pn
            lappend ind $i $pn
        }        
        foreach {i pn} $ind {
            setwindowsmenuentrylabel $i $pn
        }
    }
    updatepanestitles
}

proc IsPrunedNameAmbiguous {ta} {
# Returns the list of textareas containing pruned file names
# identical to the pruned file name attached to $ta
# Note also that peers of $ta are never included in the output list
    global listoffile listoftextarea

    foreach {pn peerid} [removepeerid $listoffile("$ta",displayedname)] {}

    set peerlist [getpeerlist $ta]

    set whichta ""
    foreach ta1 $listoftextarea {
        foreach {pn1 peerid1} [removepeerid $listoffile("$ta1",displayedname)] {}
        # $ta1 is identified as being ambiguous with $ta if and only if:
        #   - displayednames (which at this point are file tails) are the same
        # and
        #   - $ta1 is not a peer of $ta (note that a text widget is never
        #     identified by Tk as a peer of itself, therefore this second
        #     condition is true if $ta1==$ta)
        if {$pn1 == $pn && [lsearch -exact $peerlist $ta1] == -1} {
            lappend whichta $ta1
        }
    }

    # if there is only one element in the ambiguous textarea list, then
    # this element is $ta itself and there is actually no ambiguity
    if {[llength $whichta] == 1} {
        set whichta ""
    }

    return $whichta
}

proc RemoveAmbiguity {talist indlist} {
# $talist containing the list of textareas attached to ambiguous
# file tails, expand file names as necessary to remove ambiguities
# $indlist contains a flat list of chunks of four elements, ordered
# in the same order as $listoftextarea (one chunk for each entry in
# $listoftextarea):
#   - index in the windows menu of the entry
#   - file tail of the textarea fullname having this index in the windows menu
#   - textarea pathname
#   - peer identifier (or -1 in Tk 8.4 or if it is not a peer)
    global listoffile pad filenamesdisplaytype

    if {$filenamesdisplaytype == "fullifambig"} {

        # full file names are displayed if tails are ambiguous
        foreach ta $talist {
            # do it for the ambiguous displayedname of $ta
            foreach {i pn ta1 peerid} $indlist {
                if {$ta1 == $ta} {
                    # now $i and $peerid have the good value
                    break
                }
            }
            set en [appendpeerid $listoffile("$ta",fullname) $peerid]
            set listoffile("$ta",displayedname) $en
            setwindowsmenuentrylabel $i $en

            # do it also for peers of $ta
            foreach peerta [getpeerlist $ta] {
                foreach {i pn ta1 peerid} $indlist {
                    if {$ta1 == $peerta} {
                        # now $i and $peerid have the good value
                        break
                    }
                }
                set en [appendpeerid $listoffile("$ta",fullname) $peerid]
                set listoffile("$peerta",displayedname) $en
                setwindowsmenuentrylabel $i $en
            }

        }

    } else {

        # assert: $filenamesdisplaytype must be "pruned"
        # unambiguous pruned file names are displayed

        # from the fullnames, create the unambiguous displayednames without
        # changing the labels of the windows menu
        CreateUnambiguousPrunedNames $talist
        foreach ta $talist {

            # update the menu label for the ambiguous displayedname of $ta
            foreach {i pn ta1 peerid} $indlist {
                if {$ta1 == $ta} {
                    # now $i and $peerid have the good value
                    break
                }
            }
            set lab [appendpeerid $listoffile("$ta",displayedname) $peerid]
            set listoffile("$ta",displayedname) $lab
            setwindowsmenuentrylabel $i $lab

            # update also the menu label for peers of $ta
            foreach peerta [getpeerlist $ta] {
                foreach {i pn ta1 peerid} $indlist {
                    if {$ta1 == $peerta} {
                        # now $i and $peerid have the good value
                        break
                    }
                }
                set lab [appendpeerid $listoffile("$ta",displayedname) $peerid]
                set listoffile("$peerta",displayedname) $lab
                setwindowsmenuentrylabel $i $lab
            }

        }
    }
}

proc CreateUnambiguousPrunedNames {talist} {
# Example:
# From D:/a1/b1/c1/d1/afile.sci
#      E:/a1/b2/c1/d1/afile.sci   as full pathnames
# create b1/c1/d1/afile.sci
#        b2/c1/d1/afile.sci       as pruned names
    global listoffile
    # split full names into a number $ind("$ta") of elements
    foreach ta $talist {
        set elts("$ta") [file split $listoffile("$ta",fullname)]
        set ind("$ta") [llength $elts("$ta")]
    }
    # determine number of levels to keep in pruned filenames
    # and index of the first element to keep for each textarea
    set completetalist $talist
    set ta1 [lindex $talist [expr {[llength $talist] - 1}]]
    set talist [lreplace $talist end end]
    set leveltokeep "true"
    set nleveltokeep 0
    while {$leveltokeep == "true"} {
        incr nleveltokeep
        foreach ta $talist {
            incr ind("$ta") -1
        }
        incr ind("$ta1") -1
        set tocompare [lindex $elts("$ta1") $ind("$ta1")]
        foreach ta $talist {
            if { [lindex $elts("$ta") $ind("$ta")] != $tocompare } {
                set leveltokeep "false"
            }
        }
    }
    # create new minimally pruned unambiguous filenames
    foreach ta $completetalist {
        set newname ""
        for {set i $ind("$ta")} {$i <= [expr {$ind("$ta") + $nleveltokeep}]} {incr i} {
            set tojoin [lindex $elts("$ta") $i]
            set newname [file join $newname $tojoin]
        }
        set listoffile("$ta",displayedname) $newname
    }
}

##################################################
# procedures dealing with the recent files list
# displayed in the file menu
##################################################
proc AddRecentFile {filename} {
# add a new recent file item to the file menu
# if there is already the max number of entries, then shift them
# one line down and insert $filename at the top
    global pad listofrecent maxrecentfiles nbrecentfiles
    if {$maxrecentfiles == 0} {return}
    # first check if the new entry is already present
    set present "false"
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        if {[lindex $listofrecent $i] == $filename} {
            set present "true"
            set pospresent $i
        }
    }
    set rec1ind [GetFirstRecentInd]
    if {$present == "false"} {
        # add the new entry
        if {$nbrecentfiles == 0} {
            incr rec1ind
            $pad.filemenu.files insert $rec1ind separator
        }
        # update the file menu
        if {$nbrecentfiles < $maxrecentfiles} {
            incr nbrecentfiles
            # insert new entry
            set listofrecent [linsert $listofrecent 0 $filename]
            # [list [lindex $listofrecent $i]] automatically escapes special characters
            $pad.filemenu.files insert $rec1ind command \
                       -label [file tail [lindex $listofrecent 0] ] \
                       -command "openfileifexists [list [lindex $listofrecent 0]]"
            # update menu entries (required to update the numbers)
            UpdateRecentLabels $rec1ind
        } else {
            # forget last entry of the list and insert new entry
            set listofrecent [lreplace $listofrecent end end]
            set listofrecent [linsert $listofrecent 0 $filename]
            # update menu entries
            UpdateRecentLabels $rec1ind
        }
    } else {
        # move the existing entry to the top of the list
        set listofrecent [lreplace $listofrecent $pospresent $pospresent]
        set listofrecent [linsert $listofrecent 0 $filename]
        UpdateRecentLabels $rec1ind
    }
}

proc GetFirstRecentInd {} {
# get index of first recent file item in the file menu
    global FirstMRUFileNameInFileMenu nbrecentfiles
    if {$nbrecentfiles == 0} {
        return [expr {$FirstMRUFileNameInFileMenu - 1}]
    } else {
        return $FirstMRUFileNameInFileMenu
    }
}

proc UpdateRecentLabels {rec1ind} {
# update labels of recent files entries with file tail preceded by a number
    global pad listofrecent nbrecentfiles
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        if {$i<9} {
            set lab [concat [expr {$i + 1}] [file tail [lindex $listofrecent $i] ] ]
        } else {
            set lab [file tail [lindex $listofrecent $i] ]
        }
        set ind [expr {$rec1ind + $i}]
        # [list [lindex $listofrecent $i]] automatically escapes special characters
        $pad.filemenu.files entryconfigure $ind \
                   -label $lab \
                   -command "openfileifexists [list [lindex $listofrecent $i]]"
        if {$i<9} {
            $pad.filemenu.files entryconfigure $ind -underline 0
        } else {
            $pad.filemenu.files entryconfigure $ind -underline -1
        }
    }
}

proc BuildInitialRecentFilesList {} {
    global pad listofrecent nbrecentfiles
    set nbrecentfiles [llength $listofrecent]
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        if {$i<9} {
            set lab [concat [expr {$i + 1}] [file tail [lindex $listofrecent $i] ] ]
        } else {
            set lab [file tail [lindex $listofrecent $i] ]
        }
        # [list [lindex $listofrecent $i]] automatically escapes special characters
        $pad.filemenu.files add command \
                   -label $lab \
                   -command "openfileifexists [list [lindex $listofrecent $i]]"
        if {$i<9} {
            set ind [$pad.filemenu.files index end]
            $pad.filemenu.files entryconfigure $ind \
                   -underline 0
        }
    }
    if {$nbrecentfiles > 0} {
        $pad.filemenu.files add separator
    }
}

proc UpdateRecentFilesList {} {
    global pad listofrecent maxrecentfiles nbrecentfiles
    if {$maxrecentfiles >= [llength $listofrecent]} {
        # nothing to do, maxrecentfiles was just increased
        # this is handled by AddRecentFile
        return
    } else {
        # maxrecentfiles was decreased
        # forget the entries in listofrecent, and update the file menu
        set rec1ind [GetFirstRecentInd]
        set firstind [expr {$rec1ind + $maxrecentfiles}]
        set lastind  [expr {$rec1ind + [llength $listofrecent] - 1}]
        $pad.filemenu.files delete $firstind $lastind
        set listofrecent [lreplace $listofrecent $maxrecentfiles end]
        incr nbrecentfiles [expr {- ($lastind - $firstind + 1)}]
        if {$maxrecentfiles == 0} {
            # remove the separator
            $pad.filemenu.files delete $firstind
        }
    }
}
