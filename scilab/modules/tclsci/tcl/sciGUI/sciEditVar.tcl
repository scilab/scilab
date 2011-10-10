

set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd

variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/modules/tclsci/tcl/BWidget"]
namespace inscope :: package require BWidget
package require BWidget




# sciEditVar.tcl
# Implements a matrix editor
# This file is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.1


# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarChangeColWidth
# Parameters  : winId col
# Description : Change the column width
# ----------------------------------------------------------------------------
proc sciGUIEditVarChangeColWidthDo { winId2 lastval winId col } {
	global sciGUITable
	sciGUIDestroy $winId2
	set newWidthCar $sciGUITable(win,$winId,data,cwc,$col)
	set sciGUITable(win,$winId,data,cwc,$col) $newWidthCar
	if { $sciGUITable(win,$winId,data,cwc,$col)!=$lastval } {		
		set w "[sciGUIName $winId].editor"
		$w.editzone delete entryTag
		$w.editzone create window -100 -100 -window $w.editzone.entry -anchor nw -tags entryTag
		$w.editzone.entry configure -width $newWidthCar
		set bb1 [$w.editzone bbox entryTag]
		set deltaPixX [expr [lindex $bb1 2] - [lindex $bb1 0] + 2]
		set widtCell [expr $sciGUITable(win,$winId,data,cwp,$col)-$sciGUITable(win,$winId,data,cwp,[expr $col-1])]
		set deltaPixX2 [expr $deltaPixX-$widtCell]
		for { set j $col } { $j<=$sciGUITable(win,$winId,data,nj) } { incr j } {
			incr sciGUITable(win,$winId,data,cwp,$j) $deltaPixX2
		}
		set w1 "[sciGUIName $winId].editor.editzone"
		set w2 "[sciGUIName $winId].editor.jaxes"
		for { set j $col } { $j<=$sciGUITable(win,$winId,data,nj) } { incr j } {
				$w1 move col$j $deltaPixX2 0
				$w2 move colj$j $deltaPixX2 0
				$w2 move coltxt$j $deltaPixX2 0
			for { set i 1 } { $i <=$sciGUITable(win,$winId,data,ni) } { incr i } {
				$w1 move item($i,$j) $deltaPixX2 0
			}
		}
		incr sciGUITable(win,$winId,data,jall) $deltaPixX2
		set jall $sciGUITable(win,$winId,data,jall)
		set iall $sciGUITable(win,$winId,data,iall)
		set di $sciGUITable(win,$winId,data,di)	
		
		for { set i 0 } { $i<=$sciGUITable(win,$winId,data,ni) } { incr i } {
			set qw [$w1 coords row$i]
			set qw [lreplace $qw 2 2 $jall]
			$w1 coords row$i $qw		
		}
		set di $sciGUITable(win,$winId,data,di)
		set x [expr $sciGUITable(win,$winId,data,cwp,$col) -3] 
		for { set i 1 } { $i<=$sciGUITable(win,$winId,data,ni) } { incr i } {
			$w.editzone delete item($i,$col)
			set y [expr ($i-0.5)*$di]
			sciGUIEditVarPutCell $w.editzone $x $y "$sciGUITable(win,$winId,data,$i,$col)" $deltaPixX item($i,$col) $di
			
		}
	
		set qw [$w1 coords bground]
		set qw [lreplace $qw 2 2 $jall]
		$w1 coords bground $qw
		$w1 config -scrollregion "0 0 $jall $iall"
		$w2 config -scrollregion "0 0 $jall $di"
	}
}
# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarChangeColWidth
# Parameters  : winId col
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarChangeColWidth { winId col } {
	global sciGUITable
	set winId2 [sciGUICreate -1 "editvar_change_width" ]
	set w [sciGUIName $winId2]
	set lastval $sciGUITable(win,$winId,data,cwc,$col)
	label $w.msg -text "Set new width for column $col:"
	entry $w.nwid -textvariable sciGUITable(win,$winId,data,cwc,$col) -width 3 
	button $w.apply -text "ok" -command "sciGUIEditVarChangeColWidthDo $winId2 $lastval $winId $col"
	pack $w.msg $w.nwid $w.apply -side left -fill x
	focus $w.nwid
	#grab $w	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarReFreshX
# Parameters  : winId c1 c2 c3
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarReFreshX { winId c1 c2 {c3 ""} } {
	global sciGUITable
	set w "[sciGUIName $winId].editor"
	if { $c3!= "" } {
		$w.editzone xview $c1 $c2 $c3
		$w.jaxes xview $c1 $c2 $c3
	} else {
		$w.editzone xview $c1 $c2
		$w.jaxes xview $c1 $c2
	}	
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarReFreshY
# Parameters  : winId c1 c2 c3
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarReFreshY { winId c1 c2 {c3 ""} } {
	global sciGUITable
	set w "[sciGUIName $winId].editor"
	if { $c3!= "" } {
		$w.editzone yview $c1 $c2 $c3
		$w.iaxes yview $c1 $c2 $c3
	} else {
		$w.editzone yview $c1 $c2
		$w.iaxes yview $c1 $c2
	}
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarPutCell
# Parameters  : winName x y text wid tag
# Description : Try to fix
# ----------------------------------------------------------------------------
proc sciGUIEditVarPutCell { winName x y txt wid tag di} {
	set loctxt "$txt"
	set ok 1
	set loccol black
	while { $ok==1 } {
		$winName create text $x $y -text "$loctxt" -anchor e -width $wid -tag $tag -fill $loccol
		set bb [$winName bbox $tag]
		set ldi [expr [lindex $bb 3] - [lindex $bb 1]]	
		if { $ldi>=$di } {			
			set loctxt [string range "$loctxt" 1 end]
			$winName delete $tag
			set loccol blue
		} else {
			set ok 0
		}
	}	
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarUpdateScilab
# Parameters  : winId
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarUpdateScilab { winId } {
    global sciGUITable
    set newNbRow $sciGUITable(win,$winId,data,ni)
    set newNbCol $sciGUITable(win,$winId,data,nj)
    ScilabEval "$sciGUITable(win,$winId,data,name)=editvar_get($winId);"
}


# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarDrawGrid
# Parameters  : winId
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarDrawGrid { winId } {
	global sciGUITable
	set w "[sciGUIName $winId].buttons.update"
	$w configure -command "sciGUIEditVarUpdateScilab $winId"
	set w "[sciGUIName $winId].buttons.msg1"
	$w configure -text "editing variable -> $sciGUITable(win,$winId,data,name)"
	set w "[sciGUIName $winId].editor"
	set ni $sciGUITable(win,$winId,data,ni)
	set nj $sciGUITable(win,$winId,data,nj)
	set defw 350
	set defh 150
	set defcc 15

	canvas $w.editzone -width $defw -height $defh -yscrollcommand "$w.sbi set" -xscrollcommand "$w.sbj set"
	set k0 [entry $w.editzone.entry -width $defcc -bd 0 -relief flat -background yellow]
	bind $k0 <Return> "sciGUIEditVarEditCellAdv $winId 1 0"
	bind $k0 <Right> "sciGUIEditVarEditCellAdv $winId 0 1"
	bind $k0 <Left> "sciGUIEditVarEditCellAdv $winId 0 -1"
	bind $k0 <Up> "sciGUIEditVarEditCellAdv $winId -1 0"
	bind $k0 <Down> "sciGUIEditVarEditCellAdv $winId 1 0"
	
	$w.editzone create window -100 -100 -window $w.editzone.entry -anchor nw -tags entryTag
	set bb1 [$w.editzone bbox entryTag]
	set dj [expr [lindex $bb1 2] - [lindex $bb1 0] + 2]
	set dj2 [expr $dj/2]
	set di [expr [lindex $bb1 3] - [lindex $bb1 1] + 2]
	set di2 [expr $di/2]
	set jall [expr $dj*$nj + 1 ]
	set iall [expr $di*$ni + 1 ]
	$w.editzone delete entryTag
	set sciGUITable(win,$winId,data,jall) $jall
	set sciGUITable(win,$winId,data,iall) $iall
	set sciGUITable(win,$winId,data,di) $di

	$w.editzone create rectangle 0 0 $jall $iall -fill white -width 0 -tag bground

	canvas $w.jaxes -width $defw -height $di -xscrollcommand "$w.sbj set"
	set sciGUITable(win,$winId,data,cwp,0) 0
	set sciGUITable(win,$winId,data,cwc,0) 0
	for { set j 1 } { $j<=$nj } { incr j } {
		$w.jaxes create text [expr $j*$dj-3] $di2 -text $j -anchor e -tag coltxt$j
		set sciGUITable(win,$winId,data,cwc,$j) $defcc
		set sciGUITable(win,$winId,data,cwp,$j) [expr $dj*$j]
	}
	$w.jaxes config -scrollregion "0 0 $jall $di"

	canvas $w.iaxes -height $defh -yscrollcommand "$w.sbi set"
	for { set i 1 } { $i<=$ni } { incr i } {
		$w.iaxes create text 5 [expr ($i-0.5)*$di] -text $i -anchor w
	}
	set q [$w.iaxes bbox all]
	set wi [expr [lindex $q 2] - [lindex $q 0] + 10 ]
	$w.iaxes config -width $wi
	$w.iaxes config -scrollregion "0 0 $wi $iall"

	scrollbar $w.sbj -orient horizontal -command "sciGUIEditVarReFreshX $winId"
	scrollbar $w.sbi -orient vertical -command "sciGUIEditVarReFreshY $winId"

	grid config $w.jaxes -column 1 -row 0 -sticky "snew" 
	grid config $w.iaxes -column 0 -row 1 -sticky "snew" 
	grid config $w.editzone -column 1 -row 1 -sticky "snew" 
	grid config $w.sbi -column 2 -row 1 -sticky "snew"
	grid config $w.sbj -column 1 -row 2 -sticky "snew"
	grid columnconfigure $w 1 -weight 1
	grid rowconfigure $w 1 -weight 1

	for { set i 1 } { $i<=$ni } { incr i } {
		for { set j 1 } { $j<=$nj } { incr j } {
			set y [expr ($i-0.5)*$di]
			set x [expr $j*$dj-3]
			sciGUIEditVarPutCell $w.editzone $x $y "$sciGUITable(win,$winId,data,$i,$j)" $dj item($i,$j) $di
		}
	}

	for { set i 0 } { $i<=$ni } { incr i } {
		$w.editzone create line 0 [expr $i*$di] $jall [expr $i*$di] -tag row$i
	}

	for { set j 0 } { $j<=$nj } { incr j } {
		$w.editzone create line [expr $j*$dj] 0 [expr $j*$dj] $iall -tag col$j
		$w.jaxes create line [expr $j*$dj] 0 [expr $j*$dj] $di -tag colj$j
		$w.jaxes bind colj$j <Any-Enter> "$w.jaxes configure -cursor sb_h_double_arrow"
		$w.jaxes bind colj$j <Any-Leave> "$w.jaxes configure -cursor arrow"
		$w.jaxes bind colj$j <1> "sciGUIEditVarChangeColWidth $winId $j"
	}

	set bb2 [$w.editzone bbox all]
	$w.editzone config -scrollregion "0 0 $jall $iall"

	bind $w.editzone <1> "sciGUIEditVarEdit $winId %x %y"	
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarTrunc
# Parameters  : n
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarTrunc { n } {	
	set h [string first "." "$n" ]
	incr h -1	
	return [string range "$n" 0 $h]
}


# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarEditCell
# Parameters  : winId di dj
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarEditCellAdv { winId di dj } {
	global sciGUITable
	set ci [expr $sciGUITable(win,$winId,data,last_i) + $di]
	set cj [expr $sciGUITable(win,$winId,data,last_j) + $dj]
	sciGUIEditVarEditCell $winId $ci $cj
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarEditCell
# Parameters  :  winId ci cj
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarEditCell { winId ci cj } {
	global sciGUITable
	set w "[sciGUIName $winId].buttons.msg2"
	$w configure -text "cell -> ($ci,$cj)"
	set w "[sciGUIName $winId].editor"
	set di $sciGUITable(win,$winId,data,di)
	if { $ci>=$sciGUITable(win,$winId,data,ni) } { set ci $sciGUITable(win,$winId,data,ni) }
	if { $cj>=$sciGUITable(win,$winId,data,nj) } { set cj $sciGUITable(win,$winId,data,nj) }
	if { $ci<=1 } { set ci 1 }
	if { $cj<=1 } { set cj 1 }
	set li $sciGUITable(win,$winId,data,last_i)
	set lj $sciGUITable(win,$winId,data,last_j)

	if { $li>=0 & $lj>=0 } {
		set xtarget [expr $sciGUITable(win,$winId,data,cwp,[expr $cj-1]) + 2]
		set ytarget [expr ($ci-1)*$di+2]
		$w.editzone create window $xtarget $ytarget -window $w.editzone.entry -anchor nw -tags entryTag
		$w.editzone.entry configure -width $sciGUITable(win,$winId,data,cwc,$cj)
		focus $w.editzone.entry
		if { ($li*$lj)>0 } {
			set sciGUITable(win,$winId,data,$li,$lj) [ $w.editzone.entry get ]
			set dj [expr $sciGUITable(win,$winId,data,cwp,$lj) - $sciGUITable(win,$winId,data,cwp,[expr $lj-1])]
			$w.editzone delete item($li,$lj)
			set y [expr ($li-0.5)*$di]
			set x [expr $sciGUITable(win,$winId,data,cwp,$lj)-3]
			sciGUIEditVarPutCell $w.editzone $x $y "$sciGUITable(win,$winId,data,$li,$lj)" $dj item($li,$lj) $di
		}
	}
	$w.editzone.entry delete 0 end
	$w.editzone.entry insert end "$sciGUITable(win,$winId,data,$ci,$cj)"
	set sciGUITable(win,$winId,data,last_i) $ci
	set sciGUITable(win,$winId,data,last_j) $cj	
}



# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarEdit
# Parameters  : winId x y
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIEditVarEdit { winId x y } {
	global sciGUITable

	set w "[sciGUIName $winId].editor"
	set jall $sciGUITable(win,$winId,data,jall)
	set iall $sciGUITable(win,$winId,data,iall)
	set ni $sciGUITable(win,$winId,data,ni)
	set nj $sciGUITable(win,$winId,data,nj)
	set di $sciGUITable(win,$winId,data,di)	
	set px [$w.editzone canvasx $x]
	set py [$w.editzone canvasy $y]
	if { $py<=$iall } {
		set ci [sciGUIEditVarTrunc [expr $py/$di]]
		incr ci
	} else {
		set ci -1
	}
	
	if { $px<=$jall } {
		for {set j 0 } { $j<$nj } { incr j } {
			if { $px>=$sciGUITable(win,$winId,data,cwp,$j) } {
				if { $px<=$sciGUITable(win,$winId,data,cwp,[expr $j+1]) } {
					set cj [expr $j+1]
					break
				}
			}
		}		
	} else {
		set cj -1
	}
	sciGUIEditVarEditCell $winId $ci $cj	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarQuit
# Parameters  : winID
# Description : Quit editor
# ----------------------------------------------------------------------------
proc sciGUIEditVarQuit { winId } {
	global sciGUITable
	unset sciGUITable(win,$winId,data,name) 
	unset sciGUITable(win,$winId,data,type) 
	unset sciGUITable(win,$winId,data,ni)
	for { set j 0 } { $j<=$sciGUITable(win,$winId,data,nj) } { incr j } {
		unset sciGUITable(win,$winId,data,cwp,$j)
		unset sciGUITable(win,$winId,data,cwc,$j)
	}
	unset sciGUITable(win,$winId,data,nj)	
	unset sciGUITable(win,$winId,data,last_i)
	unset sciGUITable(win,$winId,data,last_j)
	sciGUIDestroy $winId
	
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVar
# Parameters  : winId filename
# Description : Matrix editor GUI
# ----------------------------------------------------------------------------
proc sciGUIEditVar { {winId -1 } } {
    global sciGUITable	
    set winId2 [sciGUICreate $winId "editvar" ]
    # read the information	
    set sciGUITable(win,$winId2,data,last_i) 0
    set sciGUITable(win,$winId2,data,last_j) 0
    
    set w [sciGUIName $winId2]
    wm title $w "Scilab Edit Var ($winId2)"
    wm protocol $w WM_DELETE_WINDOW "sciGUIEditVarQuit $winId2"
    
    $w configure -background white
    frame $w.top -bd 0 -background white
    label $w.top.logo -image sciGUITable(gif,scilab01) -bg white
    label $w.top.mes01 -text "Edit Var" -font $sciGUITable(font,1) -bg white
    pack $w.top -expand 0
    pack $w.top.logo -side left
    pack $w.top.mes01 -side right
    
    frame $w.buttons -bd 1 -background LightGray
    frame $w.editor -bd 3 -background LightGray -relief groove
    
    button $w.buttons.update -text "Update to Scilab" -width 10
    button $w.buttons.resize -text "Resize" -width 10 -command "sciGUIEditVarChangeMatrixSize $winId2"
    button $w.buttons.quit -text "Quit" -width 10 -command "sciGUIEditVarQuit $winId2"
    label $w.buttons.msg1 -text "Loading variable, please wait ..." -bg LightGray -fg blue
    
    label $w.buttons.msg2 -text "" -bg LightGray -fg blue
    pack $w.buttons.update $w.buttons.resize $w.buttons.quit $w.buttons.msg1 $w.buttons.msg2 -side left -padx 4 -expand 0 -fill x -pady 2
    
    pack $w.buttons -side top -expand 0 -fill x
    pack $w.editor -side top -expand 1 -fill both
    
    return $winId2	
}



### FOR TEST ONLY
set TEST 0
if { $TEST==1 } { 
	source sciGUI.tcl
	sciGUIDoInit "d:/sciGUI"
	set winId [sciGUIEditVar -1]
	set sciGUITable(win,$winId,data,name) "test"
	set sciGUITable(win,$winId,data,type) 1
	set sciGUITable(win,$winId,data,ni) 10
	set sciGUITable(win,$winId,data,nj) 10
	for { set i 1 } { $i<=$sciGUITable(win,$winId,data,ni) } { incr i } {
		for { set j 1 } { $j<=$sciGUITable(win,$winId,data,nj) } { incr j } {
			set sciGUITable(win,$winId,data,$i,$j) [expr $i*$j]
		}
	}
	sciGUIEditVarDrawGrid $winId
}
	


















# ----------------------------------------------------------------------------
# Function    : sciGUIEditVar
# Parameters  : winId filename
# Description : Matrix editor GUI
# ----------------------------------------------------------------------------
proc GEDsciGUIEditVar { {winId -1 } } {
    global sciGUITable	
    set winId2 [sciGUICreate $winId "editvar" ]
    # read the information	
    set sciGUITable(win,$winId2,data,last_i) 0
    set sciGUITable(win,$winId2,data,last_j) 0
    
    set w [sciGUIName $winId2]
    wm title $w "Scilab Edit Var ($winId2)"
    wm protocol $w WM_DELETE_WINDOW "sciGUIEditVarQuit $winId2"
    #F.Leray
    wm geometry $w 420x320
    
    # I will now force this window to be always on top while not closed.
    wm withdraw  $w
    update idletasks
#    wm transient $www $w
    wm deiconify $w
    grab set $w
    
    
    
    $w configure -background white
    frame $w.top -bd 0 -background white
    label $w.top.logo -image sciGUITable(gif,scilab01) -bg white
    label $w.top.mes01 -text "Edit Var" -font $sciGUITable(font,1) -bg white
    pack $w.top -expand 0
    pack $w.top.logo -side left
    pack $w.top.mes01 -side right
    
    frame $w.buttons -bd 1 -background LightGray
    frame $w.editor -bd 3 -background LightGray -relief groove
    
    button $w.buttons.quit -text "Refresh !"  -command "CloseEditorSaveData $winId2"
    label $w.buttons.msg1 -text "Please wait: loading data... " -bg LightGray -fg blue
    label $w.buttons.msg2 -text "" -bg LightGray -fg blue
    pack $w.buttons.quit $w.buttons.msg1 $w.buttons.msg2 -side left -padx 4 -expand 1 -fill x -pady 2
    
    pack $w.buttons -side top -expand 0 -fill x
    pack $w.editor -side top -expand 1 -fill both
    
    return $winId2	
}



# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarDrawGrid
# Parameters  : winId
# Description : 
# ----------------------------------------------------------------------------
#proc GEDsciGUIEditVarDrawGrid { winId winWB} {
proc GEDsciGUIEditVarDrawGrid { winId } {
    global sciGUITable
    
    set w "[sciGUIName $winId].buttons.msg1"
    $w configure -text "loading...."
    set w "[sciGUIName $winId].editor"
    set ni $sciGUITable(win,$winId,data,ni)
    set nj $sciGUITable(win,$winId,data,nj)
    set defw 350
    set defh 350
    set defcc 15

    canvas $w.editzone -width $defw -height $defh -yscrollcommand "$w.sbi set" -xscrollcommand "$w.sbj set"
    set k0 [entry $w.editzone.entry -width $defcc -bd 0 -relief flat -background yellow]
    bind $k0 <Return> "sciGUIEditVarEditCellAdv $winId 1 0"
    bind $k0 <Right> "sciGUIEditVarEditCellAdv $winId 0 1"
    bind $k0 <Left> "sciGUIEditVarEditCellAdv $winId 0 -1"
    bind $k0 <Up> "sciGUIEditVarEditCellAdv $winId -1 0"
    bind $k0 <Down> "sciGUIEditVarEditCellAdv $winId 1 0"
    
    $w.editzone create window -100 -100 -window $w.editzone.entry -anchor nw -tags entryTag
    set bb1 [$w.editzone bbox entryTag]
    set dj [expr [lindex $bb1 2] - [lindex $bb1 0] + 2]
    set dj2 [expr $dj/2]
    set di [expr [lindex $bb1 3] - [lindex $bb1 1] + 2]
    set di2 [expr $di/2]
    set jall [expr $dj*$nj + 1 ]
    set iall [expr $di*$ni + 1 ]
    $w.editzone delete entryTag
    set sciGUITable(win,$winId,data,jall) $jall
    set sciGUITable(win,$winId,data,iall) $iall
    set sciGUITable(win,$winId,data,di) $di

    $w.editzone create rectangle 0 0 $jall $iall -fill white -width 0 -tag bground

    canvas $w.jaxes -width $defw -height $di -xscrollcommand "$w.sbj set"
    set sciGUITable(win,$winId,data,cwp,0) 0
    set sciGUITable(win,$winId,data,cwc,0) 0
    for { set j 1 } { $j<=$nj } { incr j } {
        $w.jaxes create text [expr $j*$dj-3] $di2 -text $j -anchor e -tag coltxt$j
        set sciGUITable(win,$winId,data,cwc,$j) $defcc
        set sciGUITable(win,$winId,data,cwp,$j) [expr $dj*$j]
    }
    $w.jaxes config -scrollregion "0 0 $jall $di"

    canvas $w.iaxes -height $defh -yscrollcommand "$w.sbi set"
    for { set i 1 } { $i<=$ni } { incr i } {
        $w.iaxes create text 5 [expr ($i-0.5)*$di] -text $i -anchor w
    }
    set q [$w.iaxes bbox all]
    set wi [expr [lindex $q 2] - [lindex $q 0] + 10 ]
    $w.iaxes config -width $wi
    $w.iaxes config -scrollregion "0 0 $wi $iall"

    scrollbar $w.sbj -orient horizontal -command "sciGUIEditVarReFreshX $winId"
    scrollbar $w.sbi -orient vertical -command "sciGUIEditVarReFreshY $winId"

    grid config $w.jaxes -column 1 -row 0 -sticky "snew" 
    grid config $w.iaxes -column 0 -row 1 -sticky "snew" 
    grid config $w.editzone -column 1 -row 1 -sticky "snew" 
    grid config $w.sbi -column 2 -row 1 -sticky "snew"
    grid config $w.sbj -column 1 -row 2 -sticky "snew"
    grid columnconfigure $w 1 -weight 1
    grid rowconfigure $w 1 -weight 1

    for { set i 1 } { $i<=$ni } { incr i } {
        for { set j 1 } { $j<=$nj } { incr j } {
            set y [expr ($i-0.5)*$di]
            set x [expr $j*$dj-3]
            sciGUIEditVarPutCell $w.editzone $x $y "$sciGUITable(win,$winId,data,$i,$j)" $dj item($i,$j) $di
        }
    }

    for { set i 0 } { $i<=$ni } { incr i } {
        $w.editzone create line 0 [expr $i*$di] $jall [expr $i*$di] -tag row$i
    }

    for { set j 0 } { $j<=$nj } { incr j } {
        $w.editzone create line [expr $j*$dj] 0 [expr $j*$dj] $iall -tag col$j
        $w.jaxes create line [expr $j*$dj] 0 [expr $j*$dj] $di -tag colj$j
        $w.jaxes bind colj$j <Any-Enter> "$w.jaxes configure -cursor sb_h_double_arrow"
        $w.jaxes bind colj$j <Any-Leave> "$w.jaxes configure -cursor arrow"
        $w.jaxes bind colj$j <1> "sciGUIEditVarChangeColWidth $winId $j"
    }

    set bb2 [$w.editzone bbox all]
    $w.editzone config -scrollregion "0 0 $jall $iall"

    bind $w.editzone <1> "sciGUIEditVarEdit $winId %x %y"	
}


proc CloseEditorSaveData { winId } {
    ScilabEval "CloseEditorSaveData()" "seq"
    ScilabEval "TCL_EvalStr(\"sciGUIEditVarQuit \"+\"$winId\")" "seq"
    
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarChangeMatrixSize
# Parameters  : winId
# Description : Display a dialog box to enter a new size for the matrix
# ----------------------------------------------------------------------------
proc sciGUIEditVarChangeMatrixSize { winId } {
    global sciGUITable
    set winId2 [sciGUICreate -1 "editvar_change_size" ]
    set w [sciGUIName $winId2]
    set ni $sciGUITable(win,$winId,data,ni)
    set nj $sciGUITable(win,$winId,data,nj)

    frame $w.row    -bd 1
    frame $w.column -bd 1
    frame $w.ok     -bd 1

    label $w.row.msg -text "Set number of row :"
    entry $w.row.nwid -textvariable sciGUITable(win,$winId,data,ni) -width 4

    label $w.column.msg -text "Set number of column :"
    entry $w.column.nwid -textvariable sciGUITable(win,$winId,data,nj) -width 4 

    pack $w.row.msg $w.column.msg -side left -padx 4 -expand 0 -fill x -pady 2
    pack $w.row.nwid $w.column.nwid -side right -padx 4 -expand 0 -fill x -pady 2
   
    button $w.apply -text "OK" -command "sciGUIEditVarSetMatrixSize $winId $winId2 $ni $nj"
    pack $w.apply -side bottom -pady 2
    
    pack $w.row $w.column -side top -fill x -pady 2
   #focus $w.row.nwid
}

# ----------------------------------------------------------------------------
# Function    : sciGUIEditVarSetMatrixSize
# Parameters  : winId
# Description : modify the matrix size
# ----------------------------------------------------------------------------
proc sciGUIEditVarSetMatrixSize { winId winId2 oldNbRow oldNbCol} {
    global sciGUITable
    sciGUIDestroy $winId2

    set newNbRow $sciGUITable(win,$winId,data,ni)
    set newNbCol $sciGUITable(win,$winId,data,nj)

    ScilabEval "$sciGUITable(win,$winId,data,name)=resize_matrix($sciGUITable(win,$winId,data,name),$newNbRow,$newNbCol,'$sciGUITable(win,$winId,data,type)');" "seq"

    # Increase number of rows in table if needed
    if { $oldNbRow < $newNbRow } {
        for { set i [expr $oldNbRow+1] } { $i<=$newNbRow } { incr i } {
            for { set j 0 } { $j<=$newNbCol } { incr j } {
                set sciGUITable(win,$winId,data,$i,$j) "0"
            }
        }
    }
    # Increase number of cols in table if needed
    if { $oldNbCol < $newNbCol } {
        for { set j [expr $oldNbCol+1] } { $j<=$newNbCol } { incr j } {
            for { set i 0 } { $i<=$newNbRow } { incr i } {
                set sciGUITable(win,$winId,data,$i,$j) "0"
            }
        }
    }
    
    set w "[sciGUIName $winId].editor"
    destroy $w.editzone
    destroy $w.jaxes
    destroy $w.iaxes
    destroy $w.sbj
    destroy $w.sbi

    sciGUIEditVarDrawGrid $winId
}
