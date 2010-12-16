# sciBrowseVar.tcl
# Variable Browser
# This file is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.2 - 2004/06/24

# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarINI
# Parameters  : 
# Description : Initialice Variable Browse
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarINI { } {
	global sciGUITable
	set sciGUITable(browse,show) "1 2 4 5 6 8 10 15 16 17"
	set sciGUITable(browse,chshow) 1
	set sciGUITable(browse,dshow) ""		
	set sciGUITable(browse,tnames) "{real or complex} polynomial boolean sparce {sparce boolean} integer {graphic handles} string {un-compiled func.} {function lib.} list tlist mlist pointer"
	set sciGUITable(browse,tnumbe) {1 2 3 5 6 8 9 10 11 13 14 15 16 17 128}
	set fid [open [file join $sciGUITable(internal,path) "tcl" "sciGUI" "data" "dontshow.txt"] r]
	while { [eof $fid]==0 } {
		gets $fid lineRead
		set lineRead [string trimleft "$lineRead"]
		lappend sciGUITable(browse,dshow) "$lineRead"
	}	
	close $fid
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarFilterQuit
# Parameters  : winId op
# Description : Destroy widget and keep the last configuration
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarFilterQuit { winId op } {
	global sciGUITable
	set sciGUITable(browse,show) ""
	foreach q {1 2 4 5 6 8 9 10 11 13 14 15 16 17 128} {
		if { $sciGUITable(win,$winId,data,bt$q) } { lappend sciGUITable(browse,show) $q }
		unset sciGUITable(win,$winId,data,bt$q)		
	}
	set cvar "[ [sciGUIName $winId].r.txt get 1.0 end ]"
	set cvar [split "$cvar" "\n"]
	set sciGUITable(browse,dshow) ""
	foreach qwe $cvar {
		if { $qwe!="" } { lappend sciGUITable(browse,dshow) "$qwe" }
	}	
	sciGUIDestroy $winId	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarFilter
# Parameters  : 
# Description : Show filter widget
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarFilter {} {
	global sciGUITable
	set winId [sciGUICreate -1 "browsevarfilter"]
	set w [sciGUIName $winId]
	wm title $w "Scilab BrowseVar Filter ($winId)"
	wm protocol $w WM_DELETE_WINDOW "sciGUIBrowseVarFilterQuit $winId 0"
	wm resizable $w 0 0
	$w configure -background LightGray
	frame $w.l -bd 2 -bg LightGray -relief ridge
	frame $w.r -bd 2 -bg LightGray -relief ridge	
	grid config $w.l -column 0 -row 0 -columnspan 1 -rowspan 1 -sticky "n"
	grid config $w.r -column 1 -row 0 -columnspan 1 -rowspan 1 -sticky "n"		  	
	label $w.l.btlab -text "Show:" -bg LightGray -fg blue
	checkbutton $w.l.bt1 -text "real or complex" -bg LightGray -variable sciGUITable(win,$winId,data,bt1) -relief flat
	checkbutton $w.l.bt2 -text "polynomial" -bg LightGray -variable sciGUITable(win,$winId,data,bt2) -relief flat
	checkbutton $w.l.bt4 -text "boolean" -bg LightGray -variable sciGUITable(win,$winId,data,bt4) -relief flat
	checkbutton $w.l.bt5 -text "sparce" -bg LightGray -variable sciGUITable(win,$winId,data,bt5) -relief flat
	checkbutton $w.l.bt6 -text "sparce boolean" -bg LightGray -variable sciGUITable(win,$winId,data,bt6) -relief flat
	checkbutton $w.l.bt8 -text "integer" -bg LightGray -variable sciGUITable(win,$winId,data,bt8) -relief flat
	checkbutton $w.l.bt9 -text "graphic handles" -bg LightGray -variable sciGUITable(win,$winId,data,bt9) -relief flat
	checkbutton $w.l.bt10 -text "string" -bg LightGray -variable sciGUITable(win,$winId,data,bt10) -relief flat
	checkbutton $w.l.bt11 -text "un-compiled func." -bg LightGray -variable sciGUITable(win,$winId,data,bt11) -relief flat
	checkbutton $w.l.bt13 -text "compiled func." -bg LightGray -variable sciGUITable(win,$winId,data,bt13) -relief flat
	checkbutton $w.l.bt14 -text "function lib." -bg LightGray -variable sciGUITable(win,$winId,data,bt14) -relief flat
	checkbutton $w.l.bt15 -text "list" -bg LightGray -variable sciGUITable(win,$winId,data,bt15) -relief flat
	checkbutton $w.l.bt16 -text "tlist" -bg LightGray -variable sciGUITable(win,$winId,data,bt16) -relief flat
	checkbutton $w.l.bt17 -text "mlist" -bg LightGray -variable sciGUITable(win,$winId,data,bt17) -relief flat
	checkbutton $w.l.bt128 -text "pointer" -bg LightGray -variable sciGUITable(win,$winId,data,bt128) -relief flat
	grid config $w.l.btlab -column 0 -row 0 -columnspan 1 -rowspan 1 -sticky "w" 
	set i 1
	foreach q {1 2 4 5 6 8 9 10 11 13 14 15 16 17 128 } {
		set vl "deselect"
		if { [lsearch $sciGUITable(browse,show) $q]>-1 } {set vl "select"}
		$w.l.bt$q $vl
		grid config $w.l.bt$q -column 0 -row $i -columnspan 1 -rowspan 1 -sticky "w"
		incr i
	}
	checkbutton $w.r.lab -text "Don't show :" -bg LightGray -fg blue -variable sciGUITable(browse,chshow) -relief flat
	pack $w.r.lab -side top
	text $w.r.txt -width 25 -height 20 -wrap word -yscrollcommand "$w.r.sb set"
	scrollbar $w.r.sb -command "$w.r.txt yview"
	pack $w.r.txt $w.r.sb -side left -fill both -pady 4
	$w.r.txt delete 1.0 end
	set sciGUITable(browse,dshow) [lsort $sciGUITable(browse,dshow)]
	foreach wq $sciGUITable(browse,dshow) {	$w.r.txt insert end "$wq\n" }
	button $w.ok -text "Ok" -command "sciGUIBrowseVarFilterQuit $winId 1" -width 8
	button $w.cancel -text "Cancel" -command "sciGUIBrowseVarFilterQuit $winId 0" -width 8
	grid config $w.ok -column 0 -row 1 -columnspan 1 -rowspan 1 -sticky "w" 
	grid config $w.cancel -column 1 -row 1 -columnspan 1 -rowspan 1 -sticky "e"
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarDelete
# Parameters  : winId
# Description : Delete a Scilab variable
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarDelete { winId } {
	global sciGUITable
	set var $sciGUITable(win,$winId,data,cVar)
	if { $var > 0 } {
		set varname $sciGUITable(win,$winId,data,$var,3)
		set answer [sciGUIButtonDialog -1 "Do you really want to delete $varname?" "yes|no"]
		if {$answer==1} { ScilabEval "clear $varname ;browsevar();" }
	}
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarEdit
# Parameters  : winId
# Description : Edit a Scilab Variable
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarEdit { winId } {
	global sciGUITable
	set var $sciGUITable(win,$winId,data,cVar)
	if { $var > 0 } {
		set varname $sciGUITable(win,$winId,data,$var,3)
		ScilabEval "editvar $varname;"
	}
}

# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarProcFile
# Parameters  : winId
# Description : Read variable information 
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarProcFile { winId } {
	global sciGUITable
	for { set j 1 } { $j<=$sciGUITable(win,$winId,data,nVar) } {incr j} {
		foreach k {0 1 2 3} {
			unset sciGUITable(win,$winId,data,$j,$k)
		}
	}
	set sciGUITable(win,$winId,data,nVar) 0
	set sciGUITable(win,$winId,data,cVar) 0
	set filename [file join $sciGUITable(win,$winId,data,tmpPath) "browsevar.txt"]
	set fid [open $filename r]
	while { [eof $fid]==0 } {
		gets $fid lineRead
		set lineRead [string trimleft "$lineRead"]
		if { [regexp "^<d>*" "$lineRead"] } {
			incr sciGUITable(win,$winId,data,nVar)
			set cvar $sciGUITable(win,$winId,data,nVar)
			set lineRead [string range "$lineRead" 3 end]
			foreach re {0 1 2 3} {
				set sciGUITable(win,$winId,data,$cvar,$re) [lindex "$lineRead" $re]
			}
		}			
		if { [regexp "^<m>*" "$lineRead"] } {
			set w "[sciGUIName $winId].bottom.msg"
			set lineRead [string range "$lineRead" 3 end]
			set using [lindex "$lineRead" 1]
			set total [lindex "$lineRead" 0]
			set pto   [lindex "$lineRead" 2]
			$w configure -text "You are using $using of $total ($pto%)"
		}
	}
	close $fid
}

# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarDraw
# Parameters  : winId
# Description : Display variable information
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarDraw { winId } {
    global sciGUITable
    set w "[sciGUIName $winId].center.cnv"
    $w delete all
    set p 1
    
    #F.Leray
    #Browse all the var. to know the max dimension.
    
    set y 1

    set maxk2 [string length "$sciGUITable(win,$winId,data,$y,3)"]
    set maxk3 [string length "$sciGUITable(win,$winId,data,$y,1)"]
    set maxk5 [string length "$sciGUITable(win,$winId,data,$y,2)"]
    
#     puts "1ere fois"
#     puts "maxk2 = $maxk2"
#     puts "maxk3 = $maxk3"
#     puts "maxk5 = $maxk5"
#     puts "$sciGUITable(win,$winId,data,$y,3)"
#     puts "$sciGUITable(win,$winId,data,$y,1)"
#     puts "$sciGUITable(win,$winId,data,$y,2)"

    for {set y 2} {$y <= $sciGUITable(win,$winId,data,nVar)} {incr y} {
	
	set tmp_maxk2 [string length "$sciGUITable(win,$winId,data,$y,3)"]
	if { $tmp_maxk2 > $maxk2 } {
	    set maxk2 $tmp_maxk2
	}

	set tmp_maxk3 [string length "$sciGUITable(win,$winId,data,$y,1)"]
	if { $tmp_maxk3 > $maxk3 } {
	    set maxk3 $tmp_maxk3
	}
	
	set tmp_maxk5 [string length "$sciGUITable(win,$winId,data,$y,2)"]
	if { $tmp_maxk5 > $maxk5 } {
	    set maxk5 $tmp_maxk5
	}
    }
    
#     puts "AV x8+1 : "
#     puts "maxk2 = $maxk2"
#     puts "maxk3 = $maxk3"
#     puts "maxk5 = $maxk5"
    
    set maxk2 [expr $maxk2*8+1]
    set maxk3 [expr $maxk3*8+1]
    set maxk5 [expr $maxk5*8+1]
    
#     puts "maxk2 = $maxk2"
#     puts "maxk3 = $maxk3"
#     puts "maxk5 = $maxk5"
    
#     puts "150 inter1 = [expr 22+$maxk2]"
#     puts "250 inter2 = [expr 22+$maxk2+$maxk3+$maxk5]"
#     puts "235 inter3 = [expr 22+$maxk2+$maxk3]"

    
    for {set y 1} {$y <= $sciGUITable(win,$winId,data,nVar)} {incr y} {
	set yL [expr $p*18+2]
	if { $y==$sciGUITable(win,$winId,data,cVar) } {
	    set cco "red";set cbb "yellow"
	} else {
	    set cco "black";set cbb "white"			
	}
	switch $sciGUITable(win,$winId,data,$y,0) {
	    1 {set iconName "tp01";set typNam "real or complex"}
	    2 {set iconName "tp02";set typNam "polynomial"}
	    4 {set iconName "tp04";set typNam "boolean"}
	    5 {set iconName "tp05";set typNam "sparce"}
	    6 {set iconName "tp06";set typNam "sparce boolean"}
	    8 {set iconName "tp08";set typNam "integer"}
	    9 {set iconName "tp00";set typNam "graphic handles"}
	    10 {set iconName "tp10";set typNam "string"}
	    11 {set iconName "tp00";set typNam "un-compiled func."}
	    13 {set iconName "tp00";set typNam "compiled func."}
	    14 {set iconName "tp00";set typNam "function lib."}
	    15 {set iconName "tp15";set typNam "list"}
	    16 {set iconName "tp16";set typNam "tlist"}
	    17 {set iconName "tp17";set typNam "mlist"}
	    128 {set iconName "tp00";set typNam "pointer"}
	    default {set iconName "tp00";set typNam "unknow"}
		}
	
	set putvar 0
	if { [lsearch $sciGUITable(browse,show) $sciGUITable(win,$winId,data,$y,0)]>-1 } { set putvar 1 }
	if { $sciGUITable(browse,chshow) } {
	    if { [lsearch $sciGUITable(browse,dshow) $sciGUITable(win,$winId,data,$y,3)]>-1 } { set putvar 0 }
	}
	
	if { $putvar==1 } {
	    set k(0) [$w create rectangle 22 [expr $yL-9] 350 [expr $yL+9] -fill $cbb -outline $cbb]
	    set k(1) [$w create image 0 $yL -image sciGUITable(gif,$iconName) -anchor w]
	    set k(2) [$w create text  22 $yL -text "$sciGUITable(win,$winId,data,$y,3)" -anchor w -fill $cco]
	    set tmno "$sciGUITable(win,$winId,data,$y,1)"
	    set k(3) [$w create text [expr 22+$maxk2] $yL -text "$tmno" -anchor w -fill $cco]
	    set k(4) [$w create text [expr 22+$maxk2+$maxk3+$maxk5] $yL -text "$typNam" -anchor w -fill $cco]
	    set k(5) [$w create text [expr 22+$maxk2+$maxk3] $yL -text "$sciGUITable(win,$winId,data,$y,2)" -anchor w -fill $cco]
	    foreach op {0 1 2 4 5} {
		$w bind $k($op) <1> "set sciGUITable(win,$winId,data,cVar) $y; sciGUIBrowseVarDraw $winId"
	    }
	    incr p
	}
    }
    $w config -scrollregion [$w bbox all]
}



# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVarQuit
# Parameters  : winId
# Description : Destroy Variable Browser widget
# ----------------------------------------------------------------------------
proc sciGUIBrowseVarQuit { winId } {
	global sciGUITable
	for {set cvar 1} {$cvar <= $sciGUITable(win,$winId,data,nVar)} {incr cvar} {
		foreach re {0 1 2 3} {
			unset sciGUITable(win,$winId,data,$cvar,$re)
		}
	}
	unset sciGUITable(win,$winId,data,nVar)
	unset sciGUITable(win,$winId,data,cVar)
	unset sciGUITable(win,$winId,data,localrefresh)
	unset sciGUITable(win,$winId,data,tmpPath)
	sciGUIDestroy $winId
}

# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseVar
# Parameters  : tmpPath
# Description : Variable browser GUI
# ----------------------------------------------------------------------------
proc sciGUIBrowseVar { tmpPath } {
	global sciGUITable
	set create 1
	foreach winId $sciGUITable(win,id) {
		if { [sciGUIGetType $winId]=="browsevar" } { set create 0; break; }
	}	
	if { $create==1 } {
		set winId [sciGUICreate -1 "browsevar"]
		set w [sciGUIName $winId]
		wm title $w "Scilab BrowseVar($winId)"
		wm protocol $w WM_DELETE_WINDOW "sciGUIBrowseVarQuit $winId"
		wm resizable $w 0 1
		$w configure -background white
		frame $w.top -bd 0 -background white
		label $w.top.logo -image sciGUITable(gif,scilab01) -bg white
		label $w.top.mes01 -text "Variable Browser" -font $sciGUITable(font,1) -bg white
		pack $w.top -expand 0
		pack $w.top.logo -side left
		pack $w.top.mes01 -side right
		frame $w.center -background white -bd 0
   
  		canvas $w.center.cnv -width 350 -height 100 -bg white -relief sunken -yscrollcommand "$w.center.sb set"
  		scrollbar $w.center.sb -command "$w.center.cnv yview"
  		pack $w.center.cnv $w.center.sb -side left -fill y -expand 1  -anchor w

 	        pack $w.center -side top -expand 1 -fill both

# F.Leray 19.04.05 : add a canvas to have a scrollbar on x
	      	frame $w.center2 -background white -bd 0 
	        canvas $w.center2.cnv2 -width 0 -height 0 -bg white -relief sunken -xscrollcommand "$w.center2.sb2 set"
  	        scrollbar $w.center2.sb2 -command "$w.center.cnv xview" -orient horizontal
  	        pack $w.center2.cnv2 $w.center2.sb2 -side left -fill x -expand 1 -anchor n
     	        pack $w.center2 -side top -fill both
		
	        frame $w.bottom -background LightGray -bd 2 -relief sunken
		button $w.bottom.edit -width 25 -height 25 -image sciGUITable(icon,iconPencil) -command "sciGUIBrowseVarEdit $winId"
		balloonhelp $w.bottom.edit "Edit variable"
		button $w.bottom.delete -width 25 -height 25 -image sciGUITable(icon,iconTrash) -command "sciGUIBrowseVarDelete $winId"
		balloonhelp $w.bottom.delete "Delete variable"
		button $w.bottom.refresh -width 25 -height 25 -image sciGUITable(icon,iconReload) -command "ScilabEval browsevar()"
		balloonhelp $w.bottom.refresh "Refresh table"
		button $w.bottom.filter -width 25 -height 25 -image sciGUITable(icon,iconFilter) -command "sciGUIBrowseVarFilter"
		balloonhelp $w.bottom.filter "Filter preferences"
		button $w.bottom.quit -text "quit"
		balloonhelp $w.bottom.quit "Quit"
		label $w.bottom.msg -text "" -bg LightGray
		pack $w.bottom.edit $w.bottom.delete $w.bottom.refresh $w.bottom.filter $w.bottom.msg -side left -expand 0 -padx 3 -pady 3
		pack $w.bottom -side top -fill x -expand 0
		set sciGUITable(win,$winId,data,nVar) 0
		set sciGUITable(win,$winId,data,cVar) 0
		set sciGUITable(win,$winId,data,localrefresh) "sciGUIBrowseVarDraw $winId"
	}
	set sciGUITable(win,$winId,data,tmpPath) "$tmpPath"
	sciGUIBrowseVarProcFile $winId
	sciGUIBrowseVarDraw $winId	
}


