#############################################
#to get the line number
#included by Matthieu PHILIPPE 21/11/2001 from linenum.pth
set colormen [$pad.filemenu cget -background]
entry $pad.statusind -relief groove -state disabled -background $colormen
# added by Matthieu PHILIPPE 07/12/2001
# this addes an entry widget to dsplay information !
entry $pad.statusmes -relief groove -state disabled -background $colormen \
    -foreground blue
# FV 21/05/04, added a second statusind to display the line number in functions
entry $pad.statusind2 -relief groove -state disabled -background $colormen
pack $pad.statusind2 $pad.statusind -in $pad.bottombottommenu -side right -expand 0
pack $pad.statusmes -in $pad.bottombottommenu -side bottom -expand 0 -fill x

# added by Matthieu PHILIPPE 07/12/2001
# this addes an entry widget to display information !
proc delinfo {} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes delete 0 end 
    $pad.statusmes configure -state disabled
}

proc showinfo {message} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes delete 0 end 
    $pad.statusmes insert 0 "$message"
    $pad.statusmes configure -state disabled
    after 5000 catch delinfo
}

# this proc gets the posn and sets the statusbar
proc keyposn {textarea} {
    global pad lang listoffile
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind delete 0 end 
    # changement Matthieu PHILIPPE 30/11/2001
    scan $indexin "%d.%d" ypos xpos
    if {$lang == "eng"} {
        $pad.statusind insert 0 "Line: $ypos   Column: $xpos"
    } else {
        $pad.statusind insert 0 "Ligne: $ypos   Colonne: $xpos"
    }
    $pad.statusind configure -state disabled
#ES 16/5/2004 - show additionally the logical line in a function
#FV 21/05/04 - changed to add a status indicator for line in fun
#ES 27/5/04 - only in scilab schema
    set infun [whichfun $indexin]
    $pad.statusind2 configure -state normal
    $pad.statusind2 delete 0 end
    if {$listoffile("$textarea",language) == "scilab" } {
	if {$infun !={} } {
	    set funname [lindex $infun 0]
	    set lineinfun [lindex $infun 1]
	    if {$lang == "eng"} {		
		$pad.statusind2 insert 0 "Line $lineinfun in $funname"
	    } else {
		$pad.statusind2 insert 0 "Ligne $lineinfun de $funname"
	    } 
	} else {
	    $pad.statusind2 delete 0 end
	}
    }
    $pad.statusind2 configure -state disabled
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
    global pad listoffile lang 
    set fname $listoffile("$textarea",filename)
    if {$listoffile("$textarea",save) ==1} { 
       if {$lang == "eng"} {
          settitle "$fname - (modified)"
       } else {
          settitle "$fname - (modifié)"
       }
       $pad.filemenu.wind entryconfigure [$pad.filemenu.wind index "$fname"]\
                          -background Salmon -activebackground LightSalmon
       $pad.statusind configure -background Salmon
    } else {  
       settitle "$fname"
       $pad.filemenu.wind entryconfigure [$pad.filemenu.wind index "$fname"]\
                          -background "" -activebackground ""
       $pad.statusind configure -background [$pad.filemenu cget -background]
     }
}
