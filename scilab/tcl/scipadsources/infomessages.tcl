proc showinfo {message} {
# Temporarily display a string in the message area
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " "
    $pad.statusmes configure -text "$message"
    after 5000 catch delinfo
}

proc delinfo {} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " " 
}

proc keyposn {textarea} {
# this proc gets the posn and sets the statusbar, enables context menues, etc.
    global pad listoffile
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind configure -text " "
    scan $indexin "%d.%d" ypos xpos
    incr xpos
    $pad.statusind configure -text [concat [mc "Line:"] $ypos [mc "Column:"] $xpos]
    set infun [whichfun $indexin $textarea]
    $pad.statusind2 configure -state normal
    $pad.statusind2 configure -text " "
    if {$listoffile("$textarea",language) == "scilab" } {
        if {$infun != {} } {
            # display logical line number in current function
            set funname   [lindex $infun 0]
            set lineinfun [lindex $infun 1]
            $pad.statusind2 configure -text [concat [mc "Line"] $lineinfun [mc "in"] $funname]
            # create help skeleton enabled since we're in a Scilab function
            $pad.filemenu.files entryconfigure 8 -state normal
        } else {
            # display logical line number in current buffer
            set contlines [countcontlines $textarea 1.0 $indexin]
            set logicline [$textarea index "$indexin - $contlines l"]
            scan $logicline "%d.%d" ylogicpos xlogicpos
            $pad.statusind2 configure -text [concat [mc "Logical line:"] $ylogicpos]
            # create help skeleton disabled since we're outside any Scilab function
            $pad.filemenu.files entryconfigure 8 -state disabled
        }
    }
    if {[lsearch [$textarea tag names $indexin] "libfun"]!=-1} {
        $pad.filemenu.files entryconfigure 11 -state normal
    } else {
        $pad.filemenu.files entryconfigure 11 -state disabled
    }
}

proc modifiedtitle {textarea {panesonly "false"}} {
# Set the Scipad window title to the name of the file displayed in $textarea
# and add tags (modified, readonly)
# Do the same for the pane title if it exists (i.e. if not maximized)
# Update also the visual indications of the modified state of the buffer.
# This includes title bar, colorization of the windows menu entry and
# colorization of an area in the status bar
    global pad winTitle listoffile
    set fname $listoffile("$textarea",displayedname)
    set ind [extractindexfromlabel $pad.filemenu.wind $fname]
    set mod1 ""; set mod2 ""
    if {$listoffile("$textarea",readonly) == 1} { 
        set mod1 [mc " \[ReadOnly\]"]
    }
    if {[ismodified $textarea]} {
        set mod2 [mc " (modified)"]
        if {$ind !=-1} {
            $pad.filemenu.wind entryconfigure $ind -background Salmon \
              -activebackground LightSalmon
        }
        $pad.statusind configure -background Salmon
    } else {  
        if {$ind !=-1} {
            $pad.filemenu.wind entryconfigure $ind -background "" \
               -activebackground ""
        }
        $pad.statusind configure -background [$pad.filemenu cget -background]
    }
    if {$panesonly == "false"} {
        wm title $pad "$winTitle - $fname$mod1$mod2"
    }
    if {[isdisplayed $textarea]} {
        [getpaneframename $textarea].panetitle configure -text "$fname$mod1$mod2"
    }
    if {[ismodified $textarea] && \
          $listoffile("$textarea",thetime) !=0} { 
        $pad.filemenu.files entryconfigure 4 -state normal
        bind $pad <Control-R> {revertsaved}
    } else {
        $pad.filemenu.files entryconfigure 4 -state disabled
        bind $pad <Control-R> {}
    }
}

proc updatepanestitles {} {
    global listoftextarea
    # update file names in panes only (not in Scipad title bar)
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            modifiedtitle $ta "panesonly"
        }
    }
    # update file name in Scipad title bar (and in the current textarea pane)
    modifiedtitle [gettextareacur]
}
