# sciBrowseHelp.tcl
# Browse help GUI
# This file is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.2 - 2004/06/23

# ----------------------------------------------------------------------------
# Function    : sciGUIBroeseHelpINI
# Parameters  : 
# Description : Initialice
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelpINI { } {
	global sciGUITable	
	set sciGUITable(browsehelp,nchap) 0
	set sciGUITable(browsehelp,curid) 0
	set sciGUITable(browsehelp,nitems) 0
	set sciGUITable(browsehelp,filelist) ""	
	set sciGUITable(browsehelp,mode) 0
	set sciGUITable(browsehelp,last_0) 0
	set sciGUITable(browsehelp,last_1) 0
	set sciGUITable(browsehelp,last_2) 0
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseHelpParseFile
# Parameters  : 
# Description : Initialice
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelpParseFile { } {
	global sciGUITable
	set sciGUITable(browsehelp,curid) 0
	set sciGUITable(browsehelp,nitems) 0
	set fid [open $sciGUITable(browsehelp,filelist) r]
	set p 1
	set var1 ""; set var2 ""; set var3 ""; set var4 ""
	while { [eof $fid]==0 } {
		gets $fid lineRead
		set lineRead [string trimleft "$lineRead"]
		set var$p $lineRead
		incr p
		if { $p==5 } {
			set posInTable [expr $sciGUITable(browsehelp,nitems)+1]
			set sciGUITable(browsehelp,$posInTable,Deep) $var1
			set sciGUITable(browsehelp,$posInTable,Name) $var2
			set sciGUITable(browsehelp,$posInTable,URL) $var3
			set sciGUITable(browsehelp,$posInTable,Extra) $var4
			set sciGUITable(browsehelp,$posInTable,Status) 0
			set sciGUITable(browsehelp,nitems) $posInTable
			set var1 ""; set var2 ""; set var3 ""; set var4 ""; set p 1
		}
	}
	close $fid	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseHelpChange
# Parameters  : winId
# Description : Change the state in the tree 
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelpChange { winId } {
	global sciGUITable
	set id $sciGUITable(browsehelp,curid)
	set tmp $sciGUITable(browsehelp,$id,Status)
	set sciGUITable(browsehelp,$id,Status) 0
	if { $tmp == 0 } { set sciGUITable(browsehelp,$id,Status) 1 }		
	sciGUIBrowseHelpShowTree $winId
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseHelpShowTree
# Parameters  : winId
# Description : Draw tree 
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelpShowTree { winId } {
	global sciGUITable
	set w "[sciGUIName $winId].l.b.tree"
	$w delete all
	set yLocal 0
	set lastDeep -1
	set lastStatus 1
	set gethtml 0
	for {set item 1} { $item<=$sciGUITable(browsehelp,nitems) } { incr item } {
		set curIcon "File" 
		set itemDeep $sciGUITable(browsehelp,$item,Deep)
		set isFolder 0
		set itemStatus $sciGUITable(browsehelp,$item,Status)
		if { $item < $sciGUITable(browsehelp,nitems) } {
			set nextDeep $sciGUITable(browsehelp,[expr $item+1],Deep)
			set IsOpen $sciGUITable(browsehelp,$item,Status)
			if  { $nextDeep > $itemDeep } {
				set isFolder 1
				set curIcon "OpenBook"
				if { $itemStatus == 0 } { set curIcon "ClosedBook" }
			}
		}
		
		set disp $lastStatus
		if { $itemDeep <= $lastDeep } {
			set lastDeep $itemDeep
			set lastStatus 1
			set disp 1
		}
		
		if { [expr $lastStatus*$isFolder] == 1 } {
			set lastStatus $IsOpen
			set lastDeep $itemDeep
			set disp 1
		}

		set extra ""

		if { $sciGUITable(browsehelp,mode)==1 } {
			set disp 0
			set itemDeep 0
			set toFind $sciGUITable(win,$winId,data,labFind)
			set st "$sciGUITable(browsehelp,$item,Name) $sciGUITable(browsehelp,$item,Extra)"
			if { $toFind!="" } {
				if { [regexp "$toFind" "$st"] } {
					if { $isFolder==1 } { 
						set curIcon "ClosedBook"
					} else {
						set curIcon "File" 
					}
					set disp 1
					set extra $sciGUITable(browsehelp,$item,Extra)
					if { $toFind==$sciGUITable(browsehelp,$item,Name) & $sciGUITable(browsehelp,last_2)} {
						set sciGUITable(browsehelp,curid) $item
						set sciGUITable(browsehelp,last_2) 0
					}
					
				}
			}				
		}
		
		if { $disp == 1 } {
			set y [expr 15+18*$yLocal]
			incr yLocal
			set x [expr 5+$itemDeep*20]
			set k0 [$w create image $x $y -image sciGUITable(icon,$curIcon) -anchor w]
			set fc black
			if { $sciGUITable(browsehelp,curid) == $item } {
				set fc red
				set gethtml $item
			}
			
			set txt $sciGUITable(browsehelp,$item,Name)
			set k1 [$w create text [expr $x+20] $y -text "$txt $extra" -anchor w -fill $fc]
			$w bind $k0 <1> "set sciGUITable(browsehelp,curid) $item; sciGUIBrowseHelpChange $winId"
			$w bind $k1 <1> "set sciGUITable(browsehelp,curid) $item; sciGUIBrowseHelpChange $winId"
		}
	}
	$w config -scrollregion [$w bbox all]
	if { $gethtml > 0 } {
		help::init $sciGUITable(browsehelp,$gethtml,URL)
	}
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseHelpFind
# Parameters  : winId
# Description : Find
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelpChangeMode { winId {force ""} } {
	global sciGUITable
	set btname "[sciGUIName $winId].l.t.butFind"
	set laname "[sciGUIName $winId].l.t.labFind"
	set go 1
	while { $go==1 } {
		if  { $sciGUITable(browsehelp,mode)==0 } {
			$btname configure -image sciGUITable(icon,iconViewTree)
			set sciGUITable(browsehelp,last_0) $sciGUITable(browsehelp,curid)
			set sciGUITable(browsehelp,mode) 1
			set sciGUITable(browsehelp,curid) $sciGUITable(browsehelp,last_1)
			set g0 [entry $laname -textvariable sciGUITable(win,$winId,data,labFind)]
			bind $g0 <Return> "sciGUIBrowseHelpShowTree $winId"
			pack $laname -side right -fill x -expand 1 -padx 5
		
		} else {
			$btname configure -image sciGUITable(icon,iconFind)
			set sciGUITable(browsehelp,last_1) $sciGUITable(browsehelp,curid)
			set sciGUITable(browsehelp,mode) 0
			set sciGUITable(browsehelp,curid) $sciGUITable(browsehelp,last_0)
			catch {destroy $laname}			
		}
		
		if { $force!="" } {
			if {$sciGUITable(browsehelp,mode)==$force} {
				set go 0
			}
		} else {
			set go 0
		}
	}	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseHelpQuit
# Parameters  : winId
# Description : Destroy help widget but keep the help information
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelpQuit { winId } {
	global sciGUITable
	catch {unset sciGUITable(win,$winId,data,labFind)}
	::help::destroy 
	sciGUIDestroy $winId
}


# ----------------------------------------------------------------------------
# Function    : sciGUIBrowseHelp
# Parameters  : winId update filelist initialfile
# Description : BrowseHelp widget
# ----------------------------------------------------------------------------
proc sciGUIBrowseHelp { {winId -1} update filelist {toFind ""} } {
	global sciGUITable
	set create 1
	foreach winId2 $sciGUITable(win,id) {
		if { [sciGUIGetType $winId2]=="browsehelp" } { set create 0; break; }
	}
	if { $create==1 } {
		set winId2 [sciGUICreate $winId "browsehelp" ]
		set w [sciGUIName $winId2]
		wm title $w "Scilab Browse Help ($winId2)"
		wm protocol $w WM_DELETE_WINDOW "sciGUIBrowseHelpQuit $winId2"
		$w configure -background white
		frame $w.top -bd 0 -background white
		label $w.top.logo -image sciGUITable(gif,scilab01) -bg white
		label $w.top.mes01 -text "Browse Help" -font $sciGUITable(font,1) -bg white
		pack $w.top -expand 0
		pack $w.top.logo -side left
		pack $w.top.mes01 -side right
		frame $w.l -bd 0 -background white
		frame $w.r -bd 0 -background white
		frame $w.l.t -bd 0 -background white
		frame $w.l.b -bd 0 -background white
		set sciGUITable(win,$winId,data,labFind) ""
		#set g0 [entry $w.l.t.labFind -textvariable sciGUITable(win,$winId2,data,labFind)]
		#bind $g0 <Return> "sciGUIBrowseHelpShowTree $winId2"
		
		button $w.l.t.butFind -width 20 -height 20 -image sciGUITable(icon,iconFind) -command "sciGUIBrowseHelpChangeMode $winId2; sciGUIBrowseHelpShowTree $winId2"
		#pack $w.l.t.labFind -side right -fill x -expand 1 -padx 5
		pack $w.l.t.butFind -side left -expand 0
		
		canvas $w.l.b.tree -width 250 -height 350 -bd 1 -background LightGray -relief sunken -yscrollcommand "$w.l.b.sb set"
		scrollbar $w.l.b.sb -command "$w.l.b.tree yview"
		pack $w.l.b.tree $w.l.b.sb -side left -fill both -expand 0
		pack $w.l.t -side top -fill x -expand 0 
		pack $w.l.b -side top -fill both -expand 1 -pady 3

		pack $w.l -side left -padx 5 -fill both -expand 0
		pack $w.r -side left -padx 5 -fill both -expand 1
		set sciGUITable(browsehelp,filelist) $filelist
		set myIni [file join $sciGUITable(internal,path) "tcl" "sciGUI" "data" "initial.help"]
		help::init $myIni initial $w.r 350 350		
	} 
		
	if { $update==1 } { sciGUIBrowseHelpParseFile }
	if { $toFind!="" } {
		set sciGUITable(win,$winId2,data,labFind) $toFind
		set sciGUITable(browsehelp,last_2) 1
		sciGUIBrowseHelpChangeMode $winId2 1
	}
	sciGUIBrowseHelpShowTree $winId2
}

