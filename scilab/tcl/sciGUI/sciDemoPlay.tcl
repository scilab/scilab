# sciDemoPlay.tcl
# Scilab Demo Play GUI
# This routine is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.1 - 2004/06/13

# ----------------------------------------------------------------------------
# Function    : sciGUIDemoPlayDoAction
# Parameters  : winId action
# Description : Do the action in the player
# ----------------------------------------------------------------------------
proc sciGUIDemoPlayDoAction { winId {action ""} } {
	global sciGUITable
	if { $sciGUITable(win,$winId,data) == 0 } {
		set ima 0
		set sciGUITable(win,$winId,data) 1
		set w [sciGUIName $winId]
		set curSlide $sciGUITable(win,$winId,data,cSlide)
		switch $action {
			n { incr curSlide }
			p { incr curSlide -1 }
			h { set curSlide 1 }
		}
		if { $curSlide < 1 } { set curSlide 1 }
		if { $curSlide >= $sciGUITable(win,$winId,data,Slide) } {
			set curSlide [expr $sciGUITable(win,$winId,data,Slide) ]
		}
		set sciGUITable(win,$winId,data,cSlide) $curSlide
		$w.top2.slide configure -text "Slide $curSlide / $sciGUITable(win,$winId,data,Slide)"
		$w.left.txt delete 1.0 end
		$w.left.txt tag configure SLIDE -font {-*-helvetica-bold-r-*-*-24} -foreground blue
		$w.left.txt tag configure CMD -foreground blue
		set fid2 [open $sciGUITable(win,$winId,data,filetmp) w]
		set runmode 3
		set runfile 0
		foreach dt $sciGUITable(win,$winId,data,Slide,$curSlide) {
			set dta $sciGUITable(win,$winId,data,Entry,$dt,dta)
			set typ $sciGUITable(win,$winId,data,Entry,$dt,typ)
			switch -regexp $typ {
				"cm|cms" {
					set runfile 1
					puts $fid2 "$dta"
					if { $typ=="cm" } {
						$w.left.txt insert end "-->$dta\n" CMD
					}						
				}

				"slide|slide silent" {
					if { $dta!="" } { $w.left.txt insert end "$dta\n" SLIDE }
					if { $typ == "slide silent" } { set runmode -1 }
				}
				
				"tx" {
					$w.left.txt insert end "$dta\n"
				}

				"img" {
					set imgName [file join [file dirname "$sciGUITable(win,$winId,data,filerun)"] "$dta"]
					image create photo imageinternal$ima -file $imgName
					$w.left.txt image create end -image imageinternal$ima
					incr ima
					$w.left.txt insert end "\n"
				}
			}	
		}			
		flush $fid2
		close $fid2
		if { $runfile == 1 } {
			set filerun $sciGUITable(win,$winId,data,filerun)
			set filetmp $sciGUITable(win,$winId,data,filetmp)
			ScilabEval "exec('$filetmp',$runmode);TK_SetVar('sciGUITable(win,$winId,data)','0');"
		} else {
			set sciGUITable(win,$winId,data) 0
		}		
	}
}



# ----------------------------------------------------------------------------
# Function    : sciGUIDemoPlayProcFile
# Parameters  : winId
# Description : Read demo information and process
# ----------------------------------------------------------------------------
proc sciGUIDemoPlayProcFile { winId } {
	global sciGUITable	
	set h [sciGUIBarProgression -1 "Reading $sciGUITable(win,$winId,data,filerun)"]
	set fid1 [open $sciGUITable(win,$winId,data,filerun) r]
	set fid2 [open $sciGUITable(win,$winId,data,filetmp) w]
	puts $fid2 "mode(-1);"
	set listOfCmd ""
	set runfile 0
	while { [eof $fid1]==0 } {
		sciGUIBarProgression $h
		gets $fid1 lineRead
		set lineRead [string trimleft "$lineRead"]
		set pgt [expr [string first ">" "$lineRead"] + 1]
		set opt [string range "$lineRead" $pgt end]		
		if { [regexp "^<slide>*|^<slide silent>*|^<cm>*|^<cms>*|^<img>*|^<tx>*" "$lineRead"] } {
			if  { [regexp "^<slide>*|^<slide silent>*" "$lineRead"] } {
				incr sciGUITable(win,$winId,data,Slide)
				set curSlide $sciGUITable(win,$winId,data,Slide)				
				set sciGUITable(win,$winId,data,Slide,$curSlide) ""
			}
			set curEntry $sciGUITable(win,$winId,data,Entry)
			set curSlide $sciGUITable(win,$winId,data,Slide)
			incr sciGUITable(win,$winId,data,Entry)
			set sciGUITable(win,$winId,data,Entry,$curEntry,dta) "$opt"
			set sciGUITable(win,$winId,data,Entry,$curEntry,typ) [string range "$lineRead" 1 [expr $pgt-2]]
			lappend sciGUITable(win,$winId,data,Slide,$curSlide) $curEntry			
		}		
		if { [regexp "^<ic>*" "$lineRead"] } {
			set runfile 1
			puts $fid2 "$opt"
		}
	}
	close $fid1
	flush $fid2
	close $fid2
	sciGUIDestroy $h
	if { $runfile==1 } {
		set sciGUITable(win,$winId,data) 1
		set filerun $sciGUITable(win,$winId,data,filerun)
		set filetmp $sciGUITable(win,$winId,data,filetmp)
		ScilabEval "exec('$filetmp',-1);TK_SetVar('sciGUITable(win,$winId,data)','0');TK_EvalStr('sciGUIDemoPlayDoAction $winId h');"
	} else {
		set sciGUITable(win,$winId,data) 0
		sciGUIDemoPlayDoAction $winId h
	}
}


# ----------------------------------------------------------------------------
# Function    : sciGUIDemoPlayQuit
# Parameters  : winId
# Description : Destroy demo play widget
# ----------------------------------------------------------------------------
proc sciGUIDemoPlayQuit { winId } {
	global sciGUITable

	for {set dt 1 } { $dt <$sciGUITable(win,$winId,data,Entry) } { incr dt } {
		unset sciGUITable(win,$winId,data,Entry,$dt,dta)
		unset sciGUITable(win,$winId,data,Entry,$dt,typ)
	}

	for {set curSlide 1 } { $curSlide<$sciGUITable(win,$winId,data,Slide) } { incr curSlide } {
		unset sciGUITable(win,$winId,data,Slide,$curSlide)
	}
		
	unset sciGUITable(win,$winId,data,filerun)
	unset sciGUITable(win,$winId,data,filetmp)
	unset sciGUITable(win,$winId,data,cSlide)
	unset sciGUITable(win,$winId,data,Entry)
	unset sciGUITable(win,$winId,data,Slide)
	unset sciGUITable(win,$winId,data,Slide,0)
	sciGUIDestroy $winId
}


# ----------------------------------------------------------------------------
# Function    : sciGUIDemoPlay
# Parameters  : winID filerun filetmp
# Description : Interactive demo player GUI
# ----------------------------------------------------------------------------
proc sciGUIDemoPlay { {winId -1} filerun filetmp } {
	global sciGUITable
	set winId2 [sciGUICreate $winId "demoplay" ]
	set w [sciGUIName $winId2]
	wm title $w "Scilab DemoPlay ($winId2)"
	wm protocol $w WM_DELETE_WINDOW "sciGUIDemoPlayQuit $winId2"
	#wm resizable $w 0 0
	$w configure -background white
	frame $w.top -bd 0 -background white
	label $w.top.logo -image sciGUITable(gif,scilab01) -bg white
	label $w.top.mes01 -text "DemoPlay" -font $sciGUITable(font,1) -bg white
	pack $w.top -fill x -expand 0
	pack $w.top.logo -side left
	pack $w.top.mes01 -side right
	frame $w.top2 -background LightGray -bd 2 -relief sunken
	button $w.top2.previous -width 25 -height 25 -image sciGUITable(icon,iconPrevious) -command "sciGUIDemoPlayDoAction $winId2 p"
	balloonhelp $w.top2.previous "Previous"
	button $w.top2.home -width 25 -height 25 -image sciGUITable(icon,iconHome) -command "sciGUIDemoPlayDoAction $winId2 h"
	balloonhelp $w.top2.home "Home"
	button $w.top2.next -width 25 -height 25 -image sciGUITable(icon,iconNext) -command "sciGUIDemoPlayDoAction $winId2 n"
	balloonhelp $w.top2.next "Next"
	button $w.top2.quit -width 25 -height 25 -image sciGUITable(icon,iconQuit) -command "sciGUIDemoPlayQuit $winId2"
	balloonhelp $w.top2.quit "Quit"
	label $w.top2.slide -text "Slide 0 / 0" -background LightGray
	pack $w.top2.previous $w.top2.home $w.top2.next $w.top2.quit $w.top2.slide -side left -padx 3 -pady 3 -expand 0
	pack $w.top2 -fill x -side top -pady 2 -expand 0
	frame $w.left -relief raised -background LightGray -bd 0
	text $w.left.txt -relief sunken -bd 1 -width 55 -height 20 -background white -wrap word -yscrollcommand "$w.left.sbr set" 
	scrollbar $w.left.sbr -command "$w.left.txt yview"
	pack $w.left.txt -fill both -side left -expand 1
	pack $w.left.sbr -padx 2 -pady 5 -fill y -side left -expand 0
	pack $w.left -side left -padx 5 -pady 5 -expand 1 -fill both

	set sciGUITable(win,$winId2,data) ""
	set sciGUITable(win,$winId2,data,filerun) $filerun
	set sciGUITable(win,$winId2,data,filetmp) $filetmp
	set sciGUITable(win,$winId2,data,cSlide) 1
	set sciGUITable(win,$winId2,data,Entry) 1
	set sciGUITable(win,$winId2,data,Slide) 0
	set sciGUITable(win,$winId2,data,Slide,0) ""
	sciGUIDemoPlayProcFile $winId2
	return $winId2
}

