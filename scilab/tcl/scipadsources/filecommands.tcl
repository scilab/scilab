# new file
# Matthieu PHILIPPE 14/12/2001
proc filesetasnewmat {} {
    global winopened
    global listoffile
    global tcl_platform
    global fileName
    global listundo_id
    global listoftextarea
    global pad
    global radiobuttonvalue
    global lang

    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",filename) [pwd]/Untitled$winopened.sce
    set listoffile("$pad.new$winopened",save) 0
    set listoffile("$pad.new$winopened",new) 1
    set listoffile("$pad.new$winopened",thetime) 0
#ES 27/5/04
    set listoffile("$pad.new$winopened",language) "scilab"
    set listundo_id("$pad.new$winopened") [new textUndoer $pad.new$winopened]
    lappend listoftextarea $pad.new$winopened
    bind $pad.new$winopened <KeyRelease> {keyposn %W}
    bind $pad.new$winopened <ButtonRelease> {keyposn %W}
    TextStyles $pad.new$winopened
    $pad.filemenu.wind add radiobutton -label "[pwd]/Untitled$winopened.sce" \
        -value $winopened -variable radiobuttonvalue \
        -command "montretext $pad.new$winopened"
    set radiobuttonvalue $winopened
    if {$lang == "eng"} {
        showinfo "New Script"
    } else {
        showinfo "Nouveau script"
    }
    montretext $pad.new$winopened
    set fileName " "
    #inccount $pad.new$winopened
    selection clear
}

# new file
proc filesetasnew {textarea} {
        switchcase filetosave $textarea setTextTitleAsNew $textarea
}

proc setTextTitleAsNew {textarea} {
        global winTitle fileName pad
        #$textarea delete 0.0 end
        set fileName " "
        wm title $pad $winTitle
        outccount $textarea
}

# kill main window
proc killwin {widget} {
        destroy $widget
}

# close app
# added by matthieu PHILIPPE 16/12/2001
proc closecur {} {
    removefuns_bp [gettextareacur]
    closefile [gettextareacur]
}

proc closefile {textarea} {
    global listoftextarea
    global listoffile
    global pad
    global lang

    proc byebye {textarea} {
        global listoftextarea
        global listoffile
        global pad winopened radiobuttonvalue wm WMGEOMETRY
        if {  [ llength $listoftextarea ] > 1 } {
            # delete the textarea entry in the listoftextarea
            set listoftextarea [lreplace $listoftextarea [lsearch \
                   $listoftextarea $textarea] [lsearch $listoftextarea \
                                                   $textarea]]
            # delte the menu windows entry
            $pad.filemenu.wind delete [$pad.filemenu.wind index \
                                           $listoffile("$textarea",filename)]
            # delete the textarea entry in listoffile
            unset listoffile("$textarea",filename) 
            unset listoffile("$textarea",save) 
            unset listoffile("$textarea",new) 
            unset listoffile("$textarea",thetime) 
            #ES 27/5/04
            unset listoffile("$textarea",language) 
            # delete the textarea entry in Undoerr
##ES: the text widget was opened for $pad.textarea. If that is destroyed, the 
## next change font, new file or open file command causes the window to shrink.
## On the other side, the only side
## effect of not destroying it which I see is that font changes do not resize
## the window (so what? -- the resizing was not exact anyway)
#            destroy $textarea
            # place as current textarea the last 
            montretext [lindex $listoftextarea end]
##ES 30/9/03 
            set lastwin [$pad.filemenu.wind entrycget end -value]
            set radiobuttonvalue $lastwin
##
        } else {
#ES 18/10/2003: save the geometry for the next time
            set WMGEOMETRY [eval {wm geometry $pad}] 
            killwin $pad 
            unset pad
        }   
    }
    #### end of bye bye

    if  [ expr [string compare [getccount $textarea] 1] == 0 ] {
        if {$lang == "eng"} {
            set answer [tk_messageBox -message "The contents of \
               $listoffile("$textarea",filename) may have changed, do you wish\
                   to to save your changes?"\
                       -title "Save Confirm?" -type yesnocancel -icon question]
        } else {
            set answer [tk_messageBox -message "Voulez-vous enregistrer les \
               modifications apportées à $listoffile("$textarea",filename) ?" \
                 -title "Confirmer sauver ?" -type yesnocancel -icon question]
        }
        case $answer {
            yes { filetosave $textarea; byebye $textarea }
            no {byebye $textarea}
            cancel {}
        }
    } else {
        byebye $textarea
    }
}

#ES 27/5/04
proc extenstolang {file} {
            set extens [string tolower [file extension $file]] 
            if {$extens == ".xml"} {return "xml"
            } elseif {$extens == ".sce" | $extens == ".sci"} {return "scilab"
            } else {return "none"}
}


proc openfile {file} {
    global listoffile
    global FGCOLOR BGCOLOR textFont taille wordWrap 
    global pad winopened textareacur listoftextarea
    global tcl_platform listundo_id
    global radiobuttonvalue lang
    if [string compare "$file" ""] {
        # search for a opened existing file
        if { [catch {[$pad.filemenu.wind index "$file"]} res]} {
            # not opened file
            incr winopened
            dupWidgetOption [gettextareacur] $pad.new$winopened
            set listoffile("$pad.new$winopened",filename) "$file"
#ES 27/5/04
            set listoffile("$pad.new$winopened",language) [extenstolang $file]
            set listoffile("$pad.new$winopened",new) 0
#ES: the following line has to be brought here for modifiedtitle to work
            $pad.filemenu.wind add radiobutton -label "$file" \
                -value $winopened -variable radiobuttonvalue \
                -command "montretext $pad.new$winopened"
            if [ file exists "$file" ] {
                set listoffile("$pad.new$winopened",thetime) [file mtime $file]
                setTextTitleAsNew $pad.new$winopened
                montretext $pad.new$winopened
                openoninit $pad.new$winopened "$file"
                outccount $pad.new$winopened
                update
                colorize $pad.new$winopened 1.0 end
            } else {
                set listoffile("$pad.new$winopened",thetime) 0
                setTextTitleAsNew $pad.new$winopened
##added ES 4/9/2003
                lappend listoftextarea $pad.new$winopened
                montretext $pad.new$winopened
#
                update
            }
##added ES 11/9/2003
#            tkTextSetCursor $pad.new$winopened "1.0"
            $pad.new$winopened mark set insert "1.0"
            keyposn $pad.new$winopened
#
            set listundo_id("$pad.new$winopened") \
                [new textUndoer $pad.new$winopened]
            bind $pad.new$winopened <KeyRelease> {keyposn %W}
            bind $pad.new$winopened <ButtonRelease> {keyposn %W}
            TextStyles $pad.new$winopened
            set radiobuttonvalue $winopened
        } else {
            # file is already opened
            if {$lang == "eng"} {
                 tk_messageBox -type ok -title "Open file" -message \
                   "This file is already opened! Save the current opened\
                    file to an another name and reopen it from disk!"
            } else {
                 tk_messageBox -type ok -title "Ouvrir fichier" -message \
               "Ce fichier est déjà ouvert! Sauvez-le sous un\
                autre nom et rouvrez-le à partir du disque !"
            }
        }
        selection clear
    }
}


#open an existing file
proc filetoopen {textarea} {
    #switchcase filetosave $textarea showopenwin $textarea
    showopenwin $textarea
##added ES 11/9/2003
#    tkTextSetCursor [gettextareacur] "1.0"
    [gettextareacur] mark set insert "1.0"
    keyposn [gettextareacur]
#
}

# generic save function
proc writesave {textarea nametosave} {
    global lang
    set FileNameToSave [open $nametosave w+]
    puts -nonewline $FileNameToSave [$textarea get 0.0 end]
    close $FileNameToSave
    outccount $textarea
    if {$lang == "eng"} {
        set msgWait "File $nametosave saved"
    } else {
        set msgWait \
         "Fichier $nametosave sauvegardé"
    }
    showinfo $msgWait
}

#save a file
#
#added by Matthieu PHILIPPE 14/12/2001
proc filetosavecur {} {
    filetosave [gettextareacur]
}

proc filetosave {textarea} {
    global listoffile
    global lang

    if {$lang == "eng"} {
        set msgChanged "The contents of $listoffile("$textarea",filename) \
            has changed on Disk, Save it anyway?"
        set msgTitle "File has changed!"
    } else {
        set msgChanged "Le contenu de $listoffile("$textarea",filename) a\
            changé sur le disque, êtes-vous sûr de vouloir le sauvegarder ?"
        set msgTitle "Le fichier a changé"
    }

    # save the opened file from disk, if not, user has to get a file name.
    # we would verify if the file has not been modify by another application
    if { [file exists $listoffile("$textarea",filename)] && \
             $listoffile("$textarea",new) == 0  } {
        if { $listoffile("$textarea",thetime) != [file mtime \
                    $listoffile("$textarea",filename)]} {
            set answer [tk_messageBox -message $msgChanged -title $msgTitle \
                            -type yesnocancel -icon question]
            case $answer {
                yes { writesave $textarea $listoffile("$textarea",filename);
                       set listoffile("$textarea",thetime) \
                             [file mtime $listoffile("$textarea",filename)]}
                no {}
                cancel {}
            }
        } else {  
            writesave $textarea $listoffile("$textarea",filename)
            set listoffile("$textarea",thetime) \
                [file mtime $listoffile("$textarea",filename)] 
        }
        return 1
    } else {
        return [eval filesaveas $textarea]
    }
}

#save a file as
#
#added by Matthieu PHILIPPE 14/12/2001
proc filesaveascur {} {
    filesaveas [gettextareacur]
}

proc filesaveas {textarea} {
    global listoffile
    global pad
    global radiobuttonvalue
    global winopened
    global lang

    if {$lang == "eng"} {
        set types {
            {"Scilab files" {*.sce *.sci }} 
            {"XML files" {*.xml }} 
            {"All files" {*.* *}}
        }
        showinfo "Save As"
    } else {
        set types {
            {"Fichiers Scilab" {*.sce *.sci }} 
            {"Fichiers XML" {*.xml }} 
            {"Tous les fichiers" {*.* *}}
        }
        showinfo "Enregistrer sous"
    }
    set myfile [tk_getSaveFile -filetypes $types -parent $pad \
                    -initialfile $listoffile("$textarea",filename)]
    if { [expr [string compare $myfile ""]] != 0} {
       $pad.filemenu.wind delete "$listoffile("$textarea",filename)"
        set listoffile("$textarea",filename) $myfile
        set listoffile("$textarea",new) 0
        set listoffile("$textarea",save) 0
##ES: brought up for modifiedtitle to work
        $pad.filemenu.wind add radiobutton -label "$myfile" \
           -value $radiobuttonvalue -variable radiobuttonvalue \
           -command "montretext $textarea"
        writesave  $textarea $myfile
        set listoffile("$textarea",thetime) \
           [file mtime $listoffile("$textarea",filename)] 
        settitle $myfile
        return 1
    }
    return 0
}


# bring up open win
# modified by Matthieu PHILIPPE 16/12/2001
# added Open multifiles
# added show an opened file
proc showopenwin {textarea} {
    global listoffile
    global FGCOLOR BGCOLOR textFont taille wordWrap 
    global pad winopened textareacur listoftextarea
    global tcl_platform listundo_id
    global radiobuttonvalue lang
    if {$lang == "eng"} {
        set types {
            {"Scilab files" {*.sce *.sci }} 
            {"XML files" {*.xml }} 
            {"All files" {*.* *}}
        }
        #showinfo "Open file"
    } else {
        set types {
            {"Fichiers Scilab" {*.sce *.sci }} 
            {"Fichiers XML" {*.xml }} 
            {"Tous les fichiers" {*.* *}}
        }
        #showinfo "Ouvrir le fichier"
    }
    set file [tk_getOpenFile -filetypes $types -parent $pad]
    #####
    if [string compare "$file" ""] {
        # search for a opened existing file
        if { [catch {[$pad.filemenu.wind index "$file"]} res]} {
            # not opened file
            incr winopened
            dupWidgetOption [gettextareacur] $pad.new$winopened
            set listoffile("$pad.new$winopened",filename) "$file"
            set listoffile("$pad.new$winopened",new) 0
            set listoffile("$pad.new$winopened",thetime) [file mtime $file]
#ES 27/5/04
            set listoffile("$pad.new$winopened",language) [extenstolang $file]
#ES: the following line has to be brought here for modifiedtitle to work - what
# afterwards opening fails?
            $pad.filemenu.wind add radiobutton -label "$file" \
                -value $winopened -variable radiobuttonvalue \
                -command "montretext $pad.new$winopened"
            #####
            setTextTitleAsNew $pad.new$winopened
            montretext $pad.new$winopened
            openoninit $pad.new$winopened "$file"
            outccount $pad.new$winopened
            update
            colorize $pad.new$winopened 1.0 end
# FV 13/05/04
          reshape_bp
          showinfo " "
            set listundo_id("$pad.new$winopened") \
                [new textUndoer $pad.new$winopened]
            bind $pad.new$winopened <KeyRelease> {keyposn %W}
            bind $pad.new$winopened <ButtonRelease> {keyposn %W}
            TextStyles $pad.new$winopened
            set radiobuttonvalue $winopened
        } else {
            # file is already opened
            if {$lang == "eng"} {
                 tk_messageBox -type ok -title "Open file" -message \
                   "This file is already opened! Save the current\
                    opened file to an another name and reopen\
                    it from disk!"
            } else {
                 tk_messageBox -type ok -title "Ouvrir fichier" -message \
                  "Ce fichier est déjà ouvert ! Sauvez-le sous un\
                   autre nom et rouvrez-le à partir du disque !"
            }
            $pad.filemenu.wind invoke $res
        }
        selection clear
    }
}

# proc to open files or read a pipe
proc openoninit {textarea thefile} {
    global listoftextarea
    global lang

    if {$lang == "eng"} {
        set msgWait "Wait seconds while loading and colorizing file"
    } else {
        set msgWait \
         "Patientez un instant le temps du chargement et de la colorisation"
    }
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
        settitle $thefile
    }
    #colorize $textarea 1.0 end
}

# generic case switcher for message box
proc switchcase {yesfn argyesfn nofn argnofn} {
    global saveTextMsg lang
    if [ expr [string compare [getccount [gettextareacur]] 1] ==0 ] {
        if {$lang == "eng"} {
            set answer [tk_messageBox -message \
             "The contents of this file may have changed, do you wish\
              to save your changes?"\
                    -title "Save Confirm?" -type yesnocancel -icon question]
        } else {
            set answer [tk_messageBox -message \
                "Voulez-vous enregistrer les modifications apportées à\
                  ce fichier ?" \
                  -title "Confirmer sauver ?" -type yesnocancel -icon question]
        }
        case $answer {
             yes { if {[eval $yesfn $argyesfn] == 1} { $nofn $argnofn} }
             no {$nofn  $argnofn}
             cancel {}
        }
    } else {
           $nofn $argnofn
    }
}
