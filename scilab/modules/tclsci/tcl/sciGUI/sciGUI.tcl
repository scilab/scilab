# sciGUI.tcl
# Main TK/TCL
# This file is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.1 - 2004/06/13

# ----------------------------------------------------------------------------
# Initial declarations and global variables
# ----------------------------------------------------------------------------
global sciGUITable
global tcl_platform


# ----------------------------------------------------------------------------
# Function    : sciGUIGenFileName
# Parameters  : 
# Description : Return an incremental filename
# ----------------------------------------------------------------------------
proc sciGUIGenFileName { } {
	global sciGUITable
	incr sciGUITable(internal,gname)
	return "sciGUI_$sciGUITable(internal,gname).txt"
}


# ----------------------------------------------------------------------------
# Function    : sciGUILoadExtra
# Parameters  : 
# Description : Load external libs
# ----------------------------------------------------------------------------
proc sciGUILoadExtra { } {
	global sciGUITable
	global tcl_platform
	global auto_path
	set ff [file join $sciGUITable(internal,path) "tcl" "sciGUI" "local_extra"]
	# Balloonn help
	source [file join $ff "balloonhelp.tcl"]
	# Notebook
	source [file join $ff "rnotebook.tcl"]
	# Spin button
	source [file join $ff "spinbutton.tcl"]
	# Combo Box
	source [file join $ff "combobox-2.3" "combobox.tcl"]
}


# ----------------------------------------------------------------------------
# Function    : sciGUIDOInit
# Parameters  : path
# Description : Initialice Tk/Tcl sciGUI core
# ----------------------------------------------------------------------------
proc sciGUIDoInit { {path ""} } {
	global sciGUITable
	global tcl_platform
	set sciGUITable(internal,gname) 0
	set sciGUITable(internal,path) $path
	set sciGUITable(win,id) 0
	set sciGUITable(win,0,type) "null"
	set sciGUITable(icon,name) ""
	set sciGUITable(font,1) {-*-helvetica-bold-r-*-*-24}
	set sciGUITable(font,2) {-*-helvetica-normal-r-*-*-10}
	sciGUIIconLoad
	sciGUIGifLoad
	sciGUILoadExtra
	return 1
}

# ----------------------------------------------------------------------------
# Function    : sciGUIGetType
# Parameters  : winId
# Description : Return type of window
# ----------------------------------------------------------------------------
proc sciGUIGetType { winId } {
	global sciGUITable
	set pos [lsearch $sciGUITable(win,id) $winId]
	set rt ""
	if {$pos!=-1} {
		set rt "$sciGUITable(win,$winId,type)"
	}
	return $rt	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIName
# Parameters  : winId
# Description : Return window name
# ----------------------------------------------------------------------------
proc sciGUIName { winId } {	
	return ".sciGUIwin$winId"
}


# ----------------------------------------------------------------------------
# Function    : sciGUIDestroy
# Parameters  : winId
# Description : Destroy window and basic information asociated
# ----------------------------------------------------------------------------
proc sciGUIDestroy { winId } {
	global sciGUITable
	if { [sciGUIExist $winId]==1 } {
		destroy [sciGUIName $winId]
		set ind [lsearch $sciGUITable(win,id) $winId]
		set sciGUITable(win,id) [lreplace $sciGUITable(win,id) $ind $ind]
		unset sciGUITable(win,$winId,type)
		unset sciGUITable(win,$winId,data)			
	}
	return $winId
}


# ----------------------------------------------------------------------------
# Function    : sciGUICreate
# Parameters  : winId type wdestroy
# Description : Create windows
# ----------------------------------------------------------------------------
proc sciGUICreate { {winId -1} {type "none"} } {
	global sciGUITable
	set winId2 $winId
	if { $winId == -1 } {
		set winId2 [lindex $sciGUITable(win,id) end]
		set pass 1
		while { $pass==1 } {
			set pass [sciGUIExist $winId2]
			if { $pass==1 } { incr winId2 }
		}
	}
	set wname [sciGUIName $winId2]
	catch { destroy $wname }
	toplevel $wname
	wm title $wname ""	
	lappend sciGUITable(win,id) $winId2
	set sciGUITable(win,$winId2,type) $type
	set sciGUITable(win,$winId2,data) ""
	return $winId2
}


# ----------------------------------------------------------------------------
# Function    : sciGUIExist
# Parameters  : winId
# Description : Retur 1 if window exist, else 0
# ----------------------------------------------------------------------------
proc sciGUIExist { winId } {
	global sciGUITable
	set exist [lsearch $sciGUITable(win,id) $winId]
	set output 0
	if { $exist > -1 } { set output 1 }
	return $output
}


# ----------------------------------------------------------------------------
# Function    : sciGUIGifLoad
# Parameters  : 
# Description : Load initial gif files
# ----------------------------------------------------------------------------
proc sciGUIGifLoad { } {
	global sciGUITable
	set fid [open [file join $sciGUITable(internal,path) "tcl" "sciGUI" "data" "giftable.txt"] r]
	while { [eof $fid]==0 } {
		gets $fid lineRead
		set lineRead [string trimleft $lineRead]
		set li [string first ":" "$lineRead"]
		if { $li>-1 } {
			#set pos [llength $sciGUITable(icon,name)]
			set nam [string range "$lineRead" 0 [expr $li-1]]
			set lfile [string range "$lineRead" [expr $li+1] end]
			if { [string first "#" "$nam"] < 0 } {
				set filename [file join $sciGUITable(internal,path) "tcl" "sciGUI" "data" $lfile]
				image create photo sciGUITable(gif,$nam) -file $filename
			}			
		}	
		
	}
	close $fid
}


# ----------------------------------------------------------------------------
# Function    : sciGUIIconLoad
# Parameters  : 
# Description : Load initial icons
# ----------------------------------------------------------------------------
proc sciGUIIconLoad { } { 
	global sciGUITable
	set fid [open [file join $sciGUITable(internal,path) "tcl" "sciGUI" "data" "icontable.txt"] r]
	while { [eof $fid]==0 } {
		gets $fid lineRead
		set lineRead [string trimleft $lineRead]
		set li [string first ":" "$lineRead"]
		if { $li>-1 } {
			set pos [llength $sciGUITable(icon,name)]
			set nam [string range "$lineRead" 0 [expr $li-1]]
			lappend sciGUITable(icon,name) $nam
			set sciGUITable(icon,$pos) [string range "$lineRead" [expr $li+1] end]
			image create photo sciGUITable(icon,$nam) -data $sciGUITable(icon,$pos)
		}	
	}
	close $fid
}


