proc showinfo {message} {
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
    set infun [whichfun $indexin]
    $pad.statusind2 configure -state normal
    $pad.statusind2 configure -text " "
    if {$listoffile("$textarea",language) == "scilab" } {
        if {$infun !={} } {
            set funname [lindex $infun 0]
            set lineinfun [lindex $infun 1]
            $pad.statusind2 configure -text [concat [mc "Line"] $lineinfun [mc "in"] $funname]
            $pad.filemenu.files entryconfigure 8 -state normal
        } else {
            $pad.statusind2 configure -text " "
            $pad.filemenu.files entryconfigure 8 -state disabled
        }
    }
    if {[lsearch [$textarea tag names $indexin] "libfun"]!=-1} {
        $pad.filemenu.files entryconfigure 11 -state normal
    } else {
        $pad.filemenu.files entryconfigure 11 -state disabled
    }
}

proc modifiedtitle {textarea} {
# set the Scipad window title to the name of the file displayed in $textarea
# add tags (modified, readonly) 
    global pad winTitle listoffile 
    set fname $listoffile("$textarea",displayedname)
    set ind [extractindexfromlabel $pad.filemenu.wind $fname]
    set mod1 ""; set mod2 ""
    if {$listoffile("$textarea",readonly) == 1} { 
        set mod1 [mc " \[ReadOnly\]"]
    }
    if {$listoffile("$textarea",save) == 1} { 
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
    wm title $pad "$winTitle - $fname$mod1$mod2"
    if {$listoffile("$textarea",save) ==1 && \
          $listoffile("$textarea",thetime) !=0} { 
        $pad.filemenu.files entryconfigure 4 -state normal
        bind $pad <Control-R> {revertsaved}
    } else {
        $pad.filemenu.files entryconfigure 4 -state disabled
        bind $pad <Control-R> {}
    }
} 
