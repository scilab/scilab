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
#   $pad.new$winopened
#       Opened buffers (textareas)
#       This is the unique identifier of the text widget displaying
#       the content of a given file
#       This text widget is packed in a frame that is itself added
#       as a pane in possibly nested panedwindows
#
#   listoftextarea
#       Contains the list of the above opened textarea names.
#       Order of the elements matters (increasing order mandatory,
#       i.e. $pad.newX must be placed before $pad.newY if X < Y)
#
#
#   A textarea name $ta can be used as a pointer to file or buffer
#   attributes:
#
#     listoffile("$ta",fullname)
#       Full path+name of the file on disk that is displayed in $ta
#
#     listoffile("$ta",displayedname)
#       Displayed name of the file on disk that is displayed in $ta
#       This may be:
#         - the shortest unambiguous reference to that file
#         - the full pathname of that file
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
#     All the labels of the menu are different at any time, except
#     during ambiguities removal (In the Options menu, item Filenames does
#     not propose any option that would lead to ambiguous file names)
#     It is very important to maintain this property throughout the code
#
#
#####################################################################

##################################################
# procs for brand new buffers
##################################################
proc filesetasnew {} {
    global winopened listoffile
    global listoftextarea pad textareaid

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
    $pad.filemenu.wind add radiobutton -label $listoffile("$pad.new$winopened",displayedname) \
        -value $winopened -variable textareaid \
        -command "montretext $pad.new$winopened"
    newfilebind
    showinfo [mc "New Script"]
    montretext $pad.new$winopened
    selection clear
    resetmodified $pad.new$winopened
}

##################################################
# procs closing buffers
##################################################
proc closecur { {quittype yesno} } {
# remove (in Scilab) the breakpoints initiated from the current buffer
# unset the variables relative to this buffer, and
# close current buffer
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    disablemenuesbinds
    set outvalue [closefile [gettextareacur] $quittype]
    catch {restoremenuesbinds} ; # catched because otherwise pad is unknown after last buffer close
    return $outvalue
}

proc closefile {textarea {quittype yesno} } {
    global listoffile pad
    global closeinitialbufferallowed
    # query the modified flag
    if  {[ismodified $textarea]} {
        # ask the user if buffer should be saved before closing
        set answer [tk_messageBox -message [ concat [mc "The contents of"] \
           $listoffile("$textarea",fullname) \
           [mc "may have changed, do you wish to save your changes?"] ] \
             -title [mc "Save Confirm?"] -type $quittype -icon question]
        switch -- $answer {
            yes { filetosave $textarea; byebye $textarea }
            no {byebye $textarea}
            cancel {return "Canceled"}
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

        if {[llength $listoftextarea] <= [gettotnbpanes]} {
            destroypaneframe $textarea
        }

        # place as current textarea the last one that is not already visible
        set i [getlasthiddentextareamenuind]
        if {$i == ""} {set i $FirstBufferNameInWindowsMenu}
        $pad.filemenu.wind invoke $i
        RefreshWindowsMenuLabels
        
        # remove tile title if there is a single pane
        if {[gettotnbpanes] == 1} {
            set visibletapwfr [lindex [array get pwframe] 1]
            pack forget $visibletapwfr.topbar
        }
    
    } else {
        killwin $pad
    }
}

proc killwin {widget} {
# kill main window and save preferences on exit
    global pad WMGEOMETRY
    #save the geometry for the next time
    set WMGEOMETRY [eval {wm geometry $pad}] 
    savepreferences
    destroy $widget
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
# exitapp will fail for instance when the user tries to close Scipad
# using [x] or File/Exit when Scipad is opening files at the same time
# (dnd of a directory with many files)
    global pad
    if {[catch {after idle {exitapp yesnocancel}}]} {after idle {killwin $pad}}
}

proc exitapp { {quittype yesno} } {
# exit Scipad
    global listoftextarea
    if {[getdbstate] == "DebugInProgress"} {
        ScilabEval_lt "delbpt();abort" "sync" "seq"
        cleantmpScilabEvalfile
    }
    foreach textarea $listoftextarea {
        set wascanceled [closecur $quittype]
        if {$wascanceled == "Canceled"} {break}
    }
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
    setwingeom $opensof
    wm resizable $opensof 1 1 

    frame $opensof.f1
    label $opensof.f1.l1 -text [mc "Open source of:"] -font $menuFont
    entry $opensof.f1.entry \
        -width 30 -font $textFont -exportselection 0 \
        -validate key -validatecommand "updatecompletions %P %d"
    pack $opensof.f1.l1 $opensof.f1.entry -side left
    pack configure $opensof.f1.entry -expand 1 -fill x -padx 5
    pack $opensof.f1

    frame $opensof.f2
    set opensoflb $opensof.f2.lb
    scrollbar $opensof.f2.sb -command "$opensoflb yview"
    listbox $opensoflb -height 6 -width 15 -font $textFont \
        -yscrollcommand "$opensof.f2.sb set" -takefocus 0
    pack $opensoflb $opensof.f2.sb -side left -padx 2
    pack configure $opensoflb -expand 1 -fill both 
    pack configure $opensof.f2.sb -fill y
    pack $opensof.f2 -expand 1 -fill both

    set opensofbOK $opensof.f3.buttonOK
    frame $opensof.f3
    button $opensofbOK -text "OK" \
            -command "OKopensourceof $opensof" \
            -width 10 -height 1 -font $menuFont
    button $opensof.f3.buttonCancel -text [mc "Cancel"] \
            -command "destroy $opensof" \
            -width 10 -height 1 -font $menuFont
    pack $opensofbOK $opensof.f3.buttonCancel -side left -padx 10
    pack $opensof.f3 -pady 4 -after $opensof.f2

    bind $opensof <Return> {OKopensourceof %W}
    bind $opensof <Double-Button-1> {OKopensourceof %W}
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

    global textFont bgcolors fgcolors
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
    global env words
    switch $keywtype {
        "libfun" {
            ScilabEval_lt "scipad(get_function_path(\"$nametoopen\"))"
        }
        "scicos" {
            set scicosdir [file join "$env(SCIPATH)" macros scicos \
                           $nametoopen.sci]
            set blocksdir [file join "$env(SCIPATH)" macros \
                          scicos_blocks "*" $nametoopen.sci]
            set filetoopen [glob $scicosdir $blocksdir]
            ScilabEval_lt "scipad(\"$filetoopen\")"
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

proc openlistoffiles {filelist} {
# open many files at once - for use with file list provided by TkDnD
# the open dialog is not shown
# in case a directory is given, open all the files in that directory
    disablemenuesbinds
    foreach f $filelist {
        regsub "^file:" $f "" f
# in unix, .* files are not matched by *, but .* matches . and ..
# If we don't exclude them, we have infinite recursion
        if {[file tail $f] == "." | [file tail $f] == ".."} continue
        if {[file isfile $f] == 1} {
            openfile $f
        } elseif {[file isdirectory $f] == 1} {
            openlistoffiles [glob -nocomplain -directory $f -types hidden *]
            openlistoffiles [glob -nocomplain -directory $f -types {f d} *]
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
    restoremenuesbinds
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
    global pad winopened textareacur listoffile
    global startdir
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    showinfo [mc "Open file"]
    # remember the latest path used for opening files
    if {![info exists startdir]} {set startdir [pwd]}
    set file [tk_getOpenFile -filetypes [knowntypes] -parent $pad -initialdir $startdir]
    if [string compare "$file" ""] {
        if [fileunreadable $file] return 
        set startdir [file dirname $file]
        # search for an opened existing file
        set res [lookiffileisopen "$file"]
        if {$res == 0} {
            notopenedfile $file
            set listoffile("$pad.new$winopened",thetime) [file mtime $file]
            shownewbuffer $file $tiledisplay
            reshape_bp
            showinfo " "
            newfilebind
        } else {
            fileisopen $file
            $pad.filemenu.wind invoke $res
        }
        selection clear
    }
    [gettextareacur] mark set insert "1.0"
    keyposn [gettextareacur]
}

proc openfileifexists {file} {
# wrapper to openfile, but issues a warning if the file does not exist (anymore).
# if file is already open, action is the same as hitting the entry in the windows
# menu
    global listoftextarea listoffile pad
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    set alreadyopen "false"
    foreach ta $listoftextarea {
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

proc openfile {file} {
# try to open a file with filename $file (no file selection through a dialog)
# if file is not already open, open it
# otherwise just switch buffers to show it
# return value:
#    0 if file could not be open
#    1 if file could be open or displayed (switched buffers)
    global pad winopened listoftextarea listoffile
    global closeinitialbufferallowed
    if [fileunreadable $file] {return 0}
    if [string compare $file ""] {
        # search for an opened existing file
        set res [lookiffileisopen "$file"]
        if {$res == 0} {
            notopenedfile $file
            if [ file exists $file ] {
                set listoffile("$pad.new$winopened",thetime) [file mtime $file]
                set listoffile("$pad.new$winopened",new) 0
                shownewbuffer $file "currenttile"
            } else {
                set listoffile("$pad.new$winopened",thetime) 0
                set listoffile("$pad.new$winopened",new) 1
                lappend listoftextarea $pad.new$winopened
                if {$closeinitialbufferallowed == true} {
                    set closeinitialbufferallowed false
                    closefile $pad.new1
                }
                montretext $pad.new$winopened
                RefreshWindowsMenuLabels
            }
            $pad.new$winopened mark set insert "1.0"
            keyposn $pad.new$winopened
            newfilebind
        } else {
            fileisopen $file
            $pad.filemenu.wind invoke $res
        }
        selection clear
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
    set lab 0
    set fpf [file normalize $file]
    foreach textarea $listoftextarea {
        if {$listoffile("$textarea",fullname)==$fpf} {
            set lab [extractindexfromlabel $pad.filemenu.wind $listoffile("$textarea",displayedname)]
            break
        }
    }
    return $lab
}

proc notopenedfile {file} {
# $file is not opened - this sets the $listoffile area values for that file
# and adds an entry in the windows menu
    global winopened pad listoffile textareaid
    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",fullname) [file normalize $file]
    set listoffile("$pad.new$winopened",displayedname) \
            [file tail $listoffile("$pad.new$winopened",fullname)]
    set listoffile("$pad.new$winopened",language) [extenstolang $file]
    setlistoffile_colorize "$pad.new$winopened" $listoffile("$pad.new$winopened",fullname)
    set listoffile("$pad.new$winopened",new) 0
    if [ file exists $file ] {
        set listoffile("$pad.new$winopened",readonly) \
             [expr [file writable $file] == 0]
    } else {
        set listoffile("$pad.new$winopened",readonly) 0
    }
    set listoffile("$pad.new$winopened",redostackdepth) 0
    set listoffile("$pad.new$winopened",progressbar_id) ""
    $pad.filemenu.wind add radiobutton \
          -label $listoffile("$pad.new$winopened",displayedname) \
          -value $winopened -variable textareaid \
          -command "montretext $pad.new$winopened"
}

proc shownewbuffer {file tiledisplay} {
    global pad winopened closeinitialbufferallowed
    if [fileunreadable $file] return
    openfileondisk $pad.new$winopened $file $tiledisplay
    resetmodified $pad.new$winopened
    if {$tiledisplay == "currenttile"} {
        montretext $pad.new$winopened
    } else {
        set closeinitialbufferallowed false
        # pack the new buffer in the splitted window
        splitwindow $tiledisplay $pad.new$winopened
    }
    RefreshWindowsMenuLabels
    AddRecentFile [file normalize $file]
    tagcontlines $pad.new$winopened
    backgroundcolorize $pad.new$winopened
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
    foreach ta $listoftextarea {
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
        set proposedname $listoffile("$textarea",displayedname)
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
        set ilab [extractindexfromlabel $pad.filemenu.wind \
                  $listoffile("$textarea",displayedname)]
        set listoffile("$textarea",fullname) [file normalize $myfile]
        set listoffile("$textarea",displayedname) \
            [file tail $listoffile("$textarea",fullname)]
        set listoffile("$textarea",new) 0
        $pad.filemenu.wind entryconfigure $ilab \
           -label $listoffile("$textarea",displayedname)
        RefreshWindowsMenuLabels
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
        set readonlyflag [expr [file writable $nametosave] == 0]
    } else {
# >>>>>>>>>>>>>>Patch starts<<<<<<<<<<<<<<<
# This patch is intended to have limited lifetime and should be removed
# once Tcl bugs 1193497 and 1344540 are fixed, i.e. file writable be
# based on the ACL list on Windows directories instead of on the read-only
# attribute
# Once removed, non regression on bug 1785 should be tested, it should
# still work with no further change
        # To fix the Windows bad behavior when wanting to write a 
        # file in an exotic directory where read-only is set
        set readonlyflag 0
# >>>>>>>>>>>>>>End of patch<<<<<<<<<<<<<<<
# original code before patch is the following line only:
#        set readonlyflag [expr [file writable [file dirname $nametosave]] == 0]
    }
    if {$readonlyflag==0} {
        # writefileondisk catched to deal with unexpected errors (should be none!)
        if {[catch {writefileondisk $textarea $nametosave}] == 0} {
            resetmodified $textarea
            set listoffile("$textarea",thetime) [file mtime $nametosave]
            set listoffile("$textarea",readonly) $readonlyflag
            set msgWait [concat [mc "File"] $nametosave [mc "saved"]]
            showinfo $msgWait
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
    global filebackupdepth
    if {$nobackupskip} {
        backupfile $nametosave $filebackupdepth
    }
    set FileNameToSave [open $nametosave w+]
    puts -nonewline $FileNameToSave [$textarea get 0.0 end]
    close $FileNameToSave
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
            if { [ file exists ${fname}.bak ] } {
                file rename -force ${fname}.bak ${fname}.ba2
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
        if [fileunreadable $thefile] return
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
            set listoffile("$textarea",thetime) [file mtime $thefile]
            montretext $textarea
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
        set listoffile("$textarea",thetime) [file mtime $listoffile("$textarea",fullname)]
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
    if {$w != $pad} return
    foreach ta $listoftextarea {
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
                      "{\"$xmlfiles\"" "{*.xml}}" \
                      "{\"$allfiles\"" "{*.* *}}" ]
    return $types
}

proc extenstolang {file} {
# given a file extension, return the associated language scheme
    set extens [string tolower [file extension $file]] 
    if {$extens == ".xml"} {
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
# make sure that the file, if it exists, can be read at all   
# return value:
#   1  $file exists and is not readable
#   0  otherwise ($file does not exist or if it exists it is readable)
    if {[file exists $file]}  {
        if {[file readable $file]==0} {
            tk_messageBox -title [mc "Unreadable file"] \
                -message [concat [mc "The file"] $file \
                         [mc "exists but is not readable!"]] \
                -icon warning -type ok
            return 1
        } else {
            # try to actually read the file from disk
            # if this fails then the file is probably locked by another
            # process - file readable should have reported this (this is
            # Tcl bug 1394972) and when it will correctly answer 0 this
            # code should be removed and just replaced by
            # return 0
            # Note: this bug has been marked as WONTFIX in the Tcl tracker
            # since it is an OS limitation. On Windows there is no way to
            # know before trying to open a file that it is locked.
            # A test with constraint "knownBug" has been added in fCmd.test
            # (in the Tcl test suite)
            if {[catch {set fileid [open $file r]}] == 0} {
                # the file readable answers true and file can really be read
                # this is the usual case
                close $fileid
                return 0
            } else {
                # the file cannot be read despite file readable answers true
                # meaning that the file is probably locked
                tk_messageBox -title [mc "Unreadable file"] \
                    -message [concat [mc "The file"] $file \
                             [mc "exists but is not readable!"]] \
                    -icon warning -type ok
                return 1
            }
        }
    } else {
        return 0
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
# this proc is supposed to be used when opening or creating files only
# listoffile("$ta",colorize) can also be changed after opening of the
# file through the Scheme menu and proc switchcolorizefile

    global pad listoffile colorizeenable

    # arbitrary size in bytes above which Scipad will ask for colorization
    set sizelimit 100000

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
        set listoffile("$ta",colorize) false
    }
}

##################################################
# procedures dealing with pruned file names
##################################################
proc RefreshWindowsMenuLabels {} {
# Reset all labels to file tails, then remove ambiguities
# by expanding names as necessary
    global listoffile listoftextarea pad filenamesdisplaytype
    if {$filenamesdisplaytype != "full"} {
        # Reset all to file tails
        foreach ta $listoftextarea {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            set pn [file tail $listoffile("$ta",fullname)]
            set listoffile("$ta",displayedname) $pn
            lappend ind $i $pn
        }
        foreach {i pn} $ind {
            $pad.filemenu.wind entryconfigure $i -label $pn
    #numbers in front of the labels (needs further work)
    #        $pad.filemenu.wind entryconfigure $i -label "$i $pn"
    #        if {$i<10} {$pad.filemenu.wind entryconfigure $i -underline 0}
        }
        # Detect duplicates and remove ambiguities
        foreach ta $listoftextarea {
            set tochange [IsPrunedNameAmbiguous $ta]
            if {$tochange != ""} {
                RemoveAmbiguity $tochange
            }
        }
    } else {
        # always full file names are displayed, even if unambiguous
        foreach ta $listoftextarea {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            set pn $listoffile("$ta",fullname)
            set listoffile("$ta",displayedname) $pn
            lappend ind $i $pn
        }        
        foreach {i pn} $ind {
            $pad.filemenu.wind entryconfigure $i -label $pn
        }
    }
    updatepanestitles
}

proc IsPrunedNameAmbiguous {ta} {
# Returns the list of textareas containing pruned file names
# identical to the pruned file name attached to $ta
    global listoffile listoftextarea
    set pn $listoffile("$ta",displayedname)
    set whichta ""
    foreach ta1 $listoftextarea {
        if {$listoffile("$ta1",displayedname)==$pn} {
            lappend whichta $ta1
        }
    }
    if {[llength $whichta] == 1} {set whichta ""}
    return $whichta
}

proc RemoveAmbiguity {talist} {
# $talist containing the list of textareas attached to ambiguous
# file tails, expand file names as necessary to remove ambiguities
    global listoffile pad filenamesdisplaytype
    if {$filenamesdisplaytype == "fullifambig"} {
        # full file names are displayed if tails are ambiguous
        foreach ta $talist {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            set en $listoffile("$ta",fullname)
            set listoffile("$ta",displayedname) $en
            $pad.filemenu.wind entryconfigure $i -label $en
        }
    } else {
        # assert: $filenamesdisplaytype must be "pruned"
        # unambiguous pruned file names are displayed
        foreach ta $talist {
            set mli("$ta") [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            $pad.filemenu.wind entryconfigure $mli("$ta") -label $ta
        }
        CreateUnambiguousPrunedNames $talist
        foreach ta $talist {
            $pad.filemenu.wind entryconfigure $mli("$ta") -label $listoffile("$ta",displayedname)
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
    set ta1 [lindex $talist [expr [llength $talist] - 1]]
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
        for {set i $ind("$ta")} {$i <= [expr $ind("$ta") + $nleveltokeep]} {incr i} {
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
        return [expr $FirstMRUFileNameInFileMenu - 1]
    } else {
        return $FirstMRUFileNameInFileMenu
    }
}

proc UpdateRecentLabels {rec1ind} {
# update labels of recent files entries with file tail preceded by a number
    global pad listofrecent nbrecentfiles
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        set lab [concat [expr $i + 1] [file tail [lindex $listofrecent $i] ] ]
        set ind [expr $rec1ind + $i]
        # [list [lindex $listofrecent $i]] automatically escapes special characters
        $pad.filemenu.files entryconfigure $ind \
                   -label $lab \
                   -command "openfileifexists [list [lindex $listofrecent $i]]"
        if {$i<9} {
            $pad.filemenu.files entryconfigure $ind \
                   -underline 0
        } else {
            # this is a hack to remove the underline by setting it after the
            # end of the entry
            set len [expr [string length $lab] + 100]
            $pad.filemenu.files entryconfigure $ind \
                   -underline $len
        }
    }
}

proc BuildInitialRecentFilesList {} {
    global pad listofrecent nbrecentfiles
    set nbrecentfiles [llength $listofrecent]
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        set lab [concat [expr $i + 1] [file tail [lindex $listofrecent $i] ] ]
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
        set firstind [expr $rec1ind + $maxrecentfiles]
        set lastind  [expr $rec1ind + [llength $listofrecent] - 1]
        $pad.filemenu.files delete $firstind $lastind
        set listofrecent [lreplace $listofrecent $maxrecentfiles end]
        incr nbrecentfiles [expr - ($lastind - $firstind + 1)]
        if {$maxrecentfiles == 0} {
            # remove the separator
            $pad.filemenu.files delete $firstind
        }
    }
}
