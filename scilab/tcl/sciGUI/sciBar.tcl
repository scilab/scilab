# sciBar.tcl
# Interactive widgets
# This file is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.2 - 2004/06/13

# ----------------------------------------------------------------------------
# Function    : sciGUIBarWait
# Parameters  : winId title progress
# Description : Create a Wait Bar
# ----------------------------------------------------------------------------
proc sciGUIBarWait { {winId -1} {title ""} {progress "-1"} } {
	global sciGUITable
	set winId2 $winId
	set create 0
	if { [sciGUIExist $winId]==0 } {
		set winId2 [sciGUICreate $winId "wait"]
		set create 1
	}
	set wname [sciGUIName $winId2]
	if { $create == 1 } {
		wm protocol $wname WM_DELETE_WINDOW "sciGUIDestroy $winId2"
		$wname configure -background LightGray
		wm resizable $wname 0 0
		wm title $wname "Scilab Wait Bar ($winId2)"
		set sciGUITable(win,$winId2,data) "0"
		label $wname.txt -text "$title" -fg blue -bg LightGray
		canvas $wname.cnv -width 300 -height 18 -bd 2 -relief sunken -bg LightGray
		pack $wname.txt $wname.cnv -side top -padx 5 -pady 1 -expand 1 -fill both
	}
	if { $sciGUITable(win,$winId2,type) == "wait" } {
		set locprogress $progress
		if { $progress== "-1" } { set locprogress $sciGUITable(win,$winId2,data) }
		set sciGUITable(win,$winId2,data) $locprogress
		$wname.cnv create line 0 10 300 10 -width 20 -fill LightGray
		$wname.cnv create line 0 10 [expr 300*$locprogress] 10 -width 20 -fill red
		$wname.cnv create text 150 10 -text "[expr 100*$locprogress]%" -anchor center -fill blue
		if { $title != "" } { $wname.txt configure -text "$title" }
	}
	return $winId2
}


# ----------------------------------------------------------------------------
# Function    : sciBarProgression
# Parameters  : winId title
# Description : Create a progression bar
# ----------------------------------------------------------------------------
proc sciGUIBarProgression { {winId -1} {title ""} } {
	global sciGUITable
	set winId2 $winId
	set create 0
	if { [sciGUIExist $winId]==0 } {
		set winId2 [sciGUICreate $winId "progression"]
		set create 1
	}	
	set wname [sciGUIName $winId2]
	if { $create == 1 } {
		wm protocol $wname WM_DELETE_WINDOW "sciGUIDestroy $winId2"
		$wname configure -background LightGray
		wm resizable $wname 0 0		
		set sciGUITable(win,$winId2,data) "0 1"
		wm title $wname "Scilab Progression Bar ($winId2)"
		frame $wname.l -bd 0 -bg LightGray
		frame $wname.r -bd 0 -bg LightGray
		pack $wname.l $wname.r -side left
		label $wname.l.icon -image sciGUITable(gif,hourglass) -bg LightGray
		pack $wname.l.icon -side left -padx 5 -pady 5
		label $wname.r.mes -text "$title" -fg blue -bg LightGray
		canvas $wname.r.pb -width 200 -height 10 -bd 2 -relief sunken
		pack $wname.r.mes $wname.r.pb -side top -expand 1 -fill both
	}
	if { $sciGUITable(win,$winId2,type) == "progression" } {
		$wname.r.pb create line 0 9 200 9 -width 10 -fill LightGray
		set cpos [lindex $sciGUITable(win,$winId2,data) 0]
		set cdir [lindex $sciGUITable(win,$winId2,data) 1]		
		$wname.r.pb create line $cpos 9 [expr $cpos+40] 9 -width 10 -fill red
		incr cpos [expr $cdir*5]
		if { $cpos < 0 } {set cpos 0;set cdir 1}
		if { $cpos > 160 } {set cpos 160;set cdir -1}
		set sciGUITable(win,$winId2,data) "$cpos $cdir"
		if { $title != "" } { $wname.r.mes configure -text "$title" }
	} else {
		set winId2 $winId
	}
	return $winId2
}

# ----------------------------------------------------------------------------
# Function    : sciGUIButtonDialog
# Parameters  : winId mes btoptwinId
# Description : Make a button dialog
# ----------------------------------------------------------------------------
proc sciGUIButtonDialog { {winId -1} {mes ""} {btopt ""} {ico "" } } {
	global sciGUITable
	global goout
	set winId2 $winId
	set create 0
	if { [sciGUIExist $winId]==0 } {
		set winId2 [sciGUICreate $winId "bdialog"]
		set create 1
	}
	set wname [sciGUIName $winId2]
	if { $create == 1 } {
		$wname configure -background LightGray
		wm protocol $wname WM_DELETE_WINDOW "set goout -1"
		wm title $wname "Scilab Button Dialog ($winId2)"
		frame $wname.top -bg LightGray
		frame $wname.bot -bg LightGray
		pack $wname.top $wname.bot -side top -fill both		
		message $wname.top.mes -width 200 -text $mes -bg lightGray
		set iconn "scilab01"
		foreach q { error hourglass info passwd question warning } {
			if { $q==$ico } { set iconn $q }
		}
			
		label $wname.top.logo -image sciGUITable(gif,$iconn) -bg white
		pack $wname.top.logo $wname.top.mes -expand 1 -fill both -padx 5 -pady 5 -side left
		set i 0
		foreach but [split $btopt |] {
			set j [button $wname.bot.b$i -bg LightGray -text $but -width 10 -command "set goout $i"]
			incr i
			pack $j -side left -padx 2 -expand 1
		}
		wm resizable $wname 0 0
		tkwait variable goout
		sciGUIDestroy $winId2
	}
	incr goout
	return $goout
}

