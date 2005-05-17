#####################################################################
#
# About the variables used:
#
#   $pad.new$winopened
#       Opened buffers (textareas)
#       $winopened starts from 1 (1 is the first buffer opened)
#       It is never decreased. It is increased each time a new
#       file is opened.
#
#   listoftextarea
#       Contains the list of the above opened textarea names
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
#
#     listoffile("$ta",readonly)
#       0: file can be written
#       1: file can be read only
#
#     listoffile("$ta",thetime)
#       Time of the last file modification on disk
#
#     listoffile("$ta",language)
#       Language scheme. Currently can be scilab, xml, or none
#
#   The Windows menu entries are radionbuttons, with the following
#   properties:
#     -value is $winopened
#     -label is $listoffile("$ta",displayedname), $ta being $pad.new$winopened
#     All the labels of the menu are different at any time, except
#     during ambiguities removal.
#
#
#####################################################################

##################################################
# procs for brand new buffers
##################################################
proc filesetasnew {} {
    global winopened listoffile
    global listoftextarea pad radiobuttonvalue
    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",fullname) [mc "Untitled"]$winopened.sce
    set listoffile("$pad.new$winopened",displayedname) [mc "Untitled"]$winopened.sce
    set listoffile("$pad.new$winopened",save) 0
    set listoffile("$pad.new$winopened",new) 1
    set listoffile("$pad.new$winopened",thetime) 0
    set listoffile("$pad.new$winopened",language) "scilab"
    set listoffile("$pad.new$winopened",readonly) 0
    lappend listoftextarea $pad.new$winopened
    $pad.filemenu.wind add radiobutton -label $listoffile("$pad.new$winopened",displayedname) \
        -value $winopened -variable radiobuttonvalue \
        -command "montretext $pad.new$winopened"
    newfilebind
    showinfo [mc "New Script"]
    montretext $pad.new$winopened
    selection clear
}

##################################################
# procs closing buffers
##################################################
proc closecur { {quittype yesno} } {
# remove (in Scilab) the breakpoints initiated from the current buffer
# unset the variables relative to this buffer, and
# close current buffer
    removescilabbuffer_bp "with_output" [gettextareacur]
    removefuns_bp [gettextareacur]
    closefile [gettextareacur] $quittype
}

proc closefile {textarea {quittype yesno} } {
    global listoftextarea listoffile pad
    # query the modified flag
    if  [ expr [string compare [getmodified $textarea] 1] == 0 ] {
        # ask the user if buffer should be saved before closing
        set answer [tk_messageBox -message [ concat [mc "The contents of"] \
           $listoffile("$textarea",fullname) \
           [mc "may have changed, do you wish to save your changes?"] ] \
             -title [mc "Save Confirm?"] -type $quittype -icon question]
        case $answer {
            yes { filetosave $textarea; byebye $textarea }
            no {byebye $textarea}
            cancel {}
        }
    } else {
        # buffer was not modified, so just close it
        byebye $textarea
    }
}

proc byebye {textarea} {
    global listoftextarea listoffile
    global pad winopened radiobuttonvalue WMGEOMETRY
    if { [ llength $listoftextarea ] > 1 } {
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
        unset listoffile("$textarea",save) 
        unset listoffile("$textarea",new) 
        unset listoffile("$textarea",thetime) 
        unset listoffile("$textarea",language) 
        unset listoffile("$textarea",readonly) 
        # delete the textarea entry in Undoerr
##ES: the text widget was opened for $pad.new$winopened. If that is destroyed, the 
## next change font, new file or open file command causes the window to shrink.
## On the other side, the only side
## effect of not destroying it which I see is that font changes do not resize
## the window (so what? -- the resizing was not exact anyway)
#        destroy $textarea
        # place as current textarea the last
        set tatodisplay [lindex $listoftextarea end]
        montretext $tatodisplay
        set lastwin [$pad.filemenu.wind entrycget end -value]
        set radiobuttonvalue $lastwin
        RefreshWindowsMenuLabels
    } else {
        #save the geometry for the next time
        set WMGEOMETRY [eval {wm geometry $pad}] 
#        set WMGEOMETRY [winfo geometry $pad] 
#        tk_messageBox -message $WMGEOMETRY
#        set PADWIDTH [eval {winfo width $pad}]
#        set PADHEIGHT [eval {winfo height $pad}]
        killwin $pad 
        unset pad
     }   
}

proc killwin {widget} {
# kill main window and save preferences on exit
    savepreferences
    destroy $widget
}

##################################################
# procs for opening buffers from disk
##################################################

proc openlibfunsource {ind} {
    global textareacur
    # exit if the cursor is not by a libfun keyword
    if {[lsearch [$textareacur tag names $ind] "libfun"] ==-1} return
    set lrange [$textareacur tag prevrange libfun "$ind+1c"]
    if {$lrange==""} {set lrange [$textareacur tag nextrange libfun $ind]}
    set curterm [$textareacur get [lindex $lrange 0] [lindex $lrange 1]]
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        if {$curterm!=""} {
            ScilabEval "scipad(get_function_path(\"$curterm\"))" }
    }
}

proc openlistoffiles {filelist} {
# open many files at once - for use with file list provided by TkDnD
# the open dialog is not shown
# in case a directory is given, open all the files in that directory
#    tk_messageBox -message $filelist
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
}

proc showopenwin {} {
# bring up the open dialog for file selection
# if file is not already open, open it
# otherwise just switch buffers to show it
    global pad winopened textareacur listoftextarea listoffile
    global startdir
    showinfo [mc "Open file"]
    # remember the latest path used for opening files
    if {![info exists startdir]} {set startdir [pwd]}
    set file [tk_getOpenFile -filetypes [knowntypes] -parent $pad -initialdir $startdir]
    if [string compare "$file" ""] {
        set startdir [file dirname $file]
        # search for an opened existing file
        set res [lookiffileisopen "$file"]
        if {$res == 0} {
            notopenedfile $file
            set listoffile("$pad.new$winopened",thetime) [file mtime $file]
            shownewbuffer $file
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
    set alreadyopen "false"
    foreach ta $listoftextarea {
        if {$listoffile("$ta",fullname) == $file} {
            set i [extractindexfromlabel $pad.filemenu.wind $listoffile("$ta",displayedname)]
            $pad.filemenu.wind invoke $i
            set alreadyopen "true"
            break
        }
    }
    if {$alreadyopen == "false"} {
        if {[file exist $file]} {
             openfile $file
        } else {
             set answer \
                [tk_messageBox -type yesno -icon question \
                 -title [mc "File not found"] -message "[mc "The file"]\
                  $file [mc "does not exist anymore. Do you want to create an\
                  empty file with the same name?"]"]
             switch -- $answer {
               yes {openfile $file}
               no {}
               }
        }
    }
}

proc openfile {file} {
# try to open a file with filename $file (no file selection through a dialog)
# if file is not already open, open it
# otherwise just switch buffers to show it
    global pad winopened listoftextarea listoffile
    if [string compare $file ""] {
        # search for an opened existing file
        set res [lookiffileisopen "$file"]
        if {$res == 0} {
            notopenedfile $file
            if [ file exists $file ] {
                set listoffile("$pad.new$winopened",thetime) [file mtime $file]
                set listoffile("$pad.new$winopened",new) 0
                shownewbuffer $file
            } else {
                set listoffile("$pad.new$winopened",thetime) 0
                set listoffile("$pad.new$winopened",new) 1
                lappend listoftextarea $pad.new$winopened
                unsetmodified $pad.new$winopened
                montretext $pad.new$winopened
                update
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
    }
}

proc lookiffileisopen {file} {
# return zero if $file is not already open, i.e. if the full path
# of $file is not already in some $listoffile("$textarea",fullname)
    global listoffile listoftextarea
    set i 0
    set lab 0
    set fpf [file normalize $file]
    foreach textarea $listoftextarea {
        incr i
        if {$listoffile("$textarea",fullname)==$fpf} {
            set lab $i
        }
    }
    return $lab
}

proc notopenedfile {file} {
# $file is not opened - this sets the $listoffile area values for that file
# and adds a entry in the windows menu
    global winopened pad listoffile radiobuttonvalue
    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",fullname) [file normalize $file]
    set listoffile("$pad.new$winopened",displayedname) \
            [file tail $listoffile("$pad.new$winopened",fullname)]
    set listoffile("$pad.new$winopened",language) [extenstolang $file]
    set listoffile("$pad.new$winopened",new) 0
    if [ file exists $file ] {
        set listoffile("$pad.new$winopened",readonly) \
             [expr [file writable $file] == 0]
    } else {
        set listoffile("$pad.new$winopened",readonly) 0
    }
    $pad.filemenu.wind add radiobutton \
          -label $listoffile("$pad.new$winopened",displayedname) \
          -value $winopened -variable radiobuttonvalue \
          -command "montretext $pad.new$winopened"
}

proc shownewbuffer {file} {
    global pad winopened
    unsetmodified $pad.new$winopened
    openoninit $pad.new$winopened $file
    montretext $pad.new$winopened
    RefreshWindowsMenuLabels
    AddRecentFile [file normalize $file]
    update
    colorize $pad.new$winopened 1.0 end
}

proc newfilebind {} {
    global pad winopened radiobuttonvalue listundo_id
    set listundo_id("$pad.new$winopened") [new textUndoer $pad.new$winopened]
    bind $pad.new$winopened <KeyRelease> {catch {keyposn %W}}
    bind $pad.new$winopened <ButtonRelease> {catch {keyposn %W}}
    TextStyles $pad.new$winopened
    set radiobuttonvalue $winopened
    tkdndbind $pad.new$winopened
}

proc fileisopen {file} {
# file is already opened
    tk_messageBox -type ok -title [concat [mc "Open file"] $file] -message [concat \
      [mc "The file"] $file [mc "is already opened! Save the current opened\
      file to another name and reopen it from disk!"] ]
}

proc openoninit {textarea thefile} {
# open/read a file from disk or read a pipe
    global listoftextarea
    set msgWait [mc "Wait seconds while loading and colorizing file"]
    showinfo $msgWait
    lappend listoftextarea $textarea
    if [string match " " $thefile] {  
        fconfigure stdin -blocking 0
        set incoming [read stdin 1]
        if [expr [string length $incoming] == 0] {
            fconfigure stdin -blocking 1
        } else {
            fconfigure stdin -blocking 1
            $textarea insert end $incoming
            while {![eof stdin]} {
                $textarea insert end [read -nonewline stdin]
            }
        }
    } else {
        if [ file exists $thefile ] {
            set newnamefile [open $thefile r]
        } else {
            set newnamefile [open $thefile a+]
        }
        while {![eof $newnamefile]} {
            $textarea insert end [read -nonewline $newnamefile ] 
        }
        close $newnamefile
    }
}

##################################################
# procs for saving buffers to disk
##################################################
proc filetosavecur {} {
# save current buffer to disk
    filetosave [gettextareacur]
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
    global listoffile
    set msgChanged [concat [mc "The contents of "] \
                           $listoffile("$textarea",fullname) \
                           [mc "has changed on disk, save it anyway?"] ]
    set msgTitle [mc "File has changed!"]
    set myfile $listoffile("$textarea",fullname)
    if { [file exists $myfile] && $listoffile("$textarea",new) == 0 } {
        if { $listoffile("$textarea",thetime) != [file mtime $myfile]} {
            set answer [tk_messageBox -message $msgChanged -title $msgTitle \
                            -type yesnocancel -icon question]
            case $answer {
                yes { writesave $textarea $myfile}
                no {}
                cancel {}
            }
        } else {  
            writesave $textarea $myfile
        }
        return 1
    } else {
        return [eval filesaveas $textarea]
    }
}

proc filesaveas {textarea} {
# bring up the Save As... dialog so that the user can pick up a file name
# and do the save under that filename
    global listoffile pad radiobuttonvalue winopened
    global startdir
    showinfo [mc "Save as"]
    # remember the latest path used for opening files
    if {![info exists startdir]} {set startdir [pwd]}
    # proposedname is the first function name found in the buffer
    set proposedname ""
    set nextfun [$textarea search -exact -forwards -regexp\
                 "\\mfunction\\M" 0.0 end ]
    if {$nextfun != ""} {
        while {[lsearch [$textarea tag names $nextfun] "textquoted"] != -1 || \
               [lsearch [$textarea tag names $nextfun] "rem2"] != -1 } {
            set nextfun [$textarea search -exact -forwards -regexp\
                         "\\mfunction\\M" "$nextfun +8c" end]
            if {$nextfun == ""} break
        }
        if {$nextfun != ""} {
            set infun [whichfun [$textarea index "$nextfun +1l"]]
            set proposedname [lindex $infun 0]
            set nextfun [$textarea search -exact -forwards -regexp\
                         "\\mfunction\\M" "$nextfun +8c" end]
        }
    }
    if {$listoffile("$textarea",new)==0 || $proposedname==""} {
        set proposedname $listoffile("$textarea",displayedname)
    } else {
        set proposedname $proposedname.sci
    }
    set myfile [tk_getSaveFile -filetypes [knowntypes] -parent $pad \
                    -initialfile $proposedname -initialdir $startdir]
    if { [expr [string compare $myfile ""]] != 0} {
        $pad.filemenu.wind delete "$listoffile("$textarea",displayedname)"
        set listoffile("$textarea",fullname) [file normalize $myfile]
        set listoffile("$textarea",displayedname) \
            [file tail $listoffile("$textarea",fullname)]
        set listoffile("$textarea",new) 0
        set listoffile("$textarea",save) 0
        $pad.filemenu.wind add radiobutton \
           -label $listoffile("$textarea",displayedname) \
           -value $radiobuttonvalue -variable radiobuttonvalue \
           -command "montretext $textarea"
        writesave $textarea $myfile
        RefreshWindowsMenuLabels
        AddRecentFile [file normalize $myfile]
        return 1
    }
    return 0
}

proc writesave {textarea nametosave} {
# generic save function - write a file onto disk
    global listoffile
    # if the file exists, check once more if the file is writable 
    # if it doesn't, check if the directory is writable
    # (case of Save as...) (non existent files return 0 to writable)
    if {[file exists $nametosave]} {
        set listoffile("$textarea",readonly) \
          [expr [file writable $nametosave] == 0]
    } else {
        set listoffile("$textarea",readonly) \
          [expr [file writable [file dirname $nametosave]] == 0]
    }
    if {$listoffile("$textarea",readonly)==0} {
        set FileNameToSave [open $nametosave w+]
        puts -nonewline $FileNameToSave [$textarea get 0.0 end]
        close $FileNameToSave
        unsetmodified $textarea
        set listoffile("$textarea",thetime) [file mtime $nametosave] 
        set msgWait [concat [mc "File"] $nametosave [mc "saved"]]
        showinfo $msgWait
    } else {
        set msgWait [concat $nametosave [mc "cannot be written!"]]
        tk_messageBox -message $msgWait
        filesaveas $textarea
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

##################################################
# revert to saved version of current buffer
##################################################
proc revertsaved {} {
    global listoffile
    set textarea [gettextareacur]
    set thefile $listoffile("$textarea",fullname) 
    if [ file exists $thefile ] {
        set answer [tk_messageBox \
            -message [concat [mc "Revert"] $thefile [mc "to saved?"] ] \
            -type yesno -icon question -title [mc "Revert Confirm?"] ]
        if {$answer == yes} {
            set oldfile [open $thefile r]
            $textarea delete 1.0 end 
            while {![eof $oldfile]} {
                $textarea insert end [read -nonewline $oldfile ] 
            }
            close $oldfile
            set listoffile("$textarea",save) 0
            set listoffile("$textarea",thetime) [file mtime $thefile]
            modifiedtitle $textarea
            colorize $textarea 1.0 end
        }
    }
}

##################################################
# ancillaries for file commands
##################################################
proc knowntypes {} {
# list of known file types - used for filtering items is the Open and Save dialogs
    set scifiles [mc "Scilab files"]
    set xmlfiles [mc "XML files"]
    set allfiles [mc "All files"]
    set types [concat "{\"$scifiles\"" "{*.sce *.sci *.tst *.dem}}" \
                      "{\"$xmlfiles\"" "{*.xml }}" \
                      "{\"$allfiles\"" "{*.* *}}" ]
    return $types
}

proc extenstolang {file} {
# given a file extension, return the associated language scheme
    set extens [string tolower [file extension $file]] 
    if {$extens == ".xml"} {
        return "xml"
    } elseif {$extens == ".sce" | $extens == ".sci" | \
              $extens == ".tst" | $extens == ".dem"} {
        return "scilab"
    } else {
        return "none"
    }
}

proc extractindexfromlabel {dm labsearched} {
# extractindexfromlabel is here to cure bugs with special filenames
# This proc should be used as a replacement for [$menuwidget index $label]
# It returns the index of entry $label in $menuwidget, even if $label is a
# number or an index reserved name (see the tcl/tk help for menu indexes)
# If $label is not found in $menuwidget, it returns -1
    for {set i 0} {$i<=[$dm index last]} {incr i} {
       if {[$dm type $i] != "separator" && [$dm type $i] != "tearoff"} {
           set lab [$dm entrycget $i -label]
           if {$lab == $labsearched} {
               return $i
           }
       }
    }
    return -1
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
    # montretext must have been called before RefreshWindowsMenuLabels
    # so that $textareacur is up-to-date
    modifiedtitle [gettextareacur]
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
            $pad.filemenu.files insert $rec1ind command \
                       -label [file tail [lindex $listofrecent 0] ] \
                       -command "openfileifexists \"[lindex $listofrecent 0]\""
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
    global pad nbrecentfiles
    set entrylabel [lindex [amp [mc "&Close"]] 1]
    set rec1ind [expr [extractindexfromlabel $pad.filemenu.files $entrylabel] \
                      - 1 - $nbrecentfiles]
    return $rec1ind
}

proc UpdateRecentLabels {rec1ind} {
# update labels of recent files entries with file tail preceded by a number
    global pad listofrecent nbrecentfiles
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        set lab [concat [expr $i + 1] [file tail [lindex $listofrecent $i] ] ]
        set ind [expr $rec1ind + $i]
        $pad.filemenu.files entryconfigure $ind \
                   -label $lab \
                   -command "openfileifexists \"[lindex $listofrecent $i]\""
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
        $pad.filemenu.files add command \
                   -label $lab \
                   -command "openfileifexists \"[lindex $listofrecent $i]\""
        if {$i<9} {
            set ind [$pad.filemenu.files index end]
            $pad.filemenu.files entryconfigure $ind \
                   -underline 0
        }
    }
    bind $pad.filemenu.files <<MenuSelect>> {+showinfo_menu %W}
    if {$nbrecentfiles > 0} {
        $pad.filemenu.files add separator
    }
}

proc showinfo_menu {w} {
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
