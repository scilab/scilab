#############################################
# added by Matthieu PHILIPPE 07/12/2001
# this addes an entry widget to display information !
proc delinfo {} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " " 
}

proc showinfo {message} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " "
    $pad.statusmes configure -text "$message"
    after 5000 catch delinfo
}

# this proc gets the posn and sets the statusbar
proc keyposn {textarea} {
    global pad lang listoffile
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind configure -text " "
    # changement Matthieu PHILIPPE 30/11/2001
    scan $indexin "%d.%d" ypos xpos
    incr xpos
    if {$lang == "eng"} {
        $pad.statusind configure -text "Line: $ypos   Column: $xpos"
    } else {
        $pad.statusind configure -text "Ligne: $ypos   Colonne: $xpos"
    }
#ES 16/5/2004 - show additionally the logical line in a function
#FV 21/05/04 - changed to add a status indicator for line in fun
#ES 27/5/04 - only in scilab schema
    set infun [whichfun $indexin]
    $pad.statusind2 configure -state normal
    $pad.statusind2 configure -text " "
    if {$listoffile("$textarea",language) == "scilab" } {
	if {$infun !={} } {
	    set funname [lindex $infun 0]
	    set lineinfun [lindex $infun 1]
	    if {$lang == "eng"} {		
		$pad.statusind2 configure -text "Line $lineinfun in $funname"
	    } else {
		$pad.statusind2 configure -text "Ligne $lineinfun de $funname"
	    } 
	} else {
	    $pad.statusind2 configure -text " "
	}
    }
}

# this proc just sets the title to what it is passed
proc settitle {WinTitleName} {
    global winTitle fileName 
    global pad
    wm title $pad "$winTitle - $WinTitleName"
    set fileName $WinTitleName
}

###added by ES 24/9/2003
proc modifiedtitle {textarea} {
# FV 07/06/04 changed $pad.filemenu.wind index for extractindexfromlabel
# This fixes the scipad 0 bug as well as corrects side effects on some 
#  filenames    
    global pad listoffile lang 
    set fname $listoffile("$textarea",filename)
    set ind [extractindexfromlabel $pad.filemenu.wind $fname]
    if {$listoffile("$textarea",save) ==1} { 
       if {$lang == "eng"} {
          settitle "$fname - (modified)"
       } else {
          settitle "$fname - (modifié)"
       }
       if {$ind !=-1} {
           $pad.filemenu.wind entryconfigure $ind -background Salmon \
             -activebackground LightSalmon
       }
       $pad.statusind configure -background Salmon
    } else {  
       settitle "$fname"
       if {$ind !=-1} {
           $pad.filemenu.wind entryconfigure $ind -background "" \
              -activebackground ""
       }
       $pad.statusind configure -background [$pad.filemenu cget -background]
     }
} 
