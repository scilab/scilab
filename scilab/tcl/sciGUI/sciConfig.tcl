# sciConfig.tcl
# Configure scilab
# This file is part of sciGUI toolbox
# Copyright (C) 2004 Jaime Urzua Grez
# mailto:jaime_urzua@yahoo.com
# rev. 0.1 


# ----------------------------------------------------------------------------
# Function    : sciGUIConfigQuit
# Parameters  : winId 
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIConfigQuit { winId } {
	global sciGUITable
	set name { ssize gsize ieee formatmode formatmode_wid lineslin linescol itask tcrit h0 hmax hmin jactype mxstep maxordn maxords ixpr ml mu }
	foreach q $name {
		unset sciGUITable(win,$winId,data,$q)
	}
	sciGUIDestroy $winId
	
}


# ----------------------------------------------------------------------------
# Function    : sciGUIConfigDo
# Parameters  : winId 
# Description : 
# ----------------------------------------------------------------------------
proc sciGUIConfigDo { winId } {
	global sciGUITable
	set cmd ""
	append cmd "stacksize($sciGUITable(win,$winId,data,ssize));"
	append cmd "gstacksize($sciGUITable(win,$winId,data,gsize));"
	append cmd "ieee([string range $sciGUITable(win,$winId,data,ieee) 0 0 ]);"
	append cmd "format(\'[string range $sciGUITable(win,$winId,data,formatmode) 0 0 ]\',$sciGUITable(win,$winId,data,formatmode_wid));"
	append cmd "lines($sciGUITable(win,$winId,data,lineslin),$sciGUITable(win,$winId,data,linescol));"
	            
	set defode "1 0 0 %inf 0 2 500 12 5 0 -1 -1"
	set newode ""
	append newode "[string range $sciGUITable(win,$winId,data,itask) 0 0] "
	append newode "$sciGUITable(win,$winId,data,tcrit) "
	append newode "$sciGUITable(win,$winId,data,h0) "
	append newode "$sciGUITable(win,$winId,data,hmax) "
	append newode "$sciGUITable(win,$winId,data,hmin) "
	append newode "[string range $sciGUITable(win,$winId,data,jactype) 0 0] "
	append newode "$sciGUITable(win,$winId,data,mxstep) "
	append newode "$sciGUITable(win,$winId,data,maxordn) "
	append newode "$sciGUITable(win,$winId,data,maxords) "
	append newode "[string range $sciGUITable(win,$winId,data,ixpr) 0 0] "
	append newode "$sciGUITable(win,$winId,data,ml) "
	append newode "$sciGUITable(win,$winId,data,mu)"
	if { "$newode"!="$defode" } {
		append cmd "\%ODEOPTIONS=\[" "$newode" "\];"		
	} else {
		append cmd "clear \%ODEOPTIONS;"
	}
	ScilabEval "$cmd"	
}
# ----------------------------------------------------------------------------
# Function    : sciGUIConfig
# Parameters  : winId parameters
# Description : Configure Scilab...
# ----------------------------------------------------------------------------
proc sciGUIConfig { {winId -1} {parameters "1000000 1240 0 v 10 72 28 1 0 0 %inf 0 2 500 12 5 0 -1 -1"} } {
	global sciGUITable
	set winId2 [sciGUICreate $winId "editvar" ]
	set w [sciGUIName $winId2]
	wm title $w "Scilab Configuration ($winId2)"
	wm protocol $w WM_DELETE_WINDOW "sciGUIConfigQuit $winId2"
	wm geometry $w 450x450
	$w configure -background white
	
	frame $w.top -bd 0 -background white
	label $w.top.logo -image sciGUITable(gif,scilab01) -bg white
	label $w.top.mes01 -text "Scilab Configuration" -font $sciGUITable(font,1) -bg white

	pack $w.top.logo -side left
	pack $w.top.mes01 -side right	
	pack $w.top -side top -fill x -expand 0
	
	frame $w.cen
	Rnotebook:create $w.cen.book -tabs {Display Stack "ODE options"} -borderwidth 2
	pack $w.cen -side top -fill both -expand 1
	pack $w.cen.book -fill both -expand 1 -padx 10 -pady 10
	
	frame $w.bot
	button $w.bot.apply  -text "Apply" -width 10 -command "sciGUIConfigDo $winId2"
	button $w.bot.cancel -text "Cancel" -width 10 -command "sciGUIConfigQuit $winId2"
	pack $w.bot -side top -fill x -expand 0
	pack $w.bot.apply $w.bot.cancel -side left -padx 10 -pady 5 -expand 0 -fill x
	
	###
	set sciGUITable(win,$winId2,data,ssize) [lindex $parameters 0]
	set sciGUITable(win,$winId2,data,gsize) [lindex $parameters 1]
	set ieemodes "{0: floating point exception produce an error} {1: floating point exception produce a warning} {2: floating point exception procudes Inf or Nan}"
	set sciGUITable(win,$winId2,data,ieee) [lindex $ieemodes [lindex $parameters 2]]
	set formatmodes "{v:variable format} {e:e-format}"
	if {[lindex $parameters 3]=="v"} { 
		set sciGUITable(win,$winId2,data,formatmode) [lindex $formatmodes 0]
	} else {
		set sciGUITable(win,$winId2,data,formatmode) [lindex $formatmodes 1]
	}
	set sciGUITable(win,$winId2,data,formatmode_wid) [lindex $parameters 4]
	set sciGUITable(win,$winId2,data,lineslin) [lindex $parameters 5]
	set sciGUITable(win,$winId2,data,linescol) [lindex $parameters 6]
	set itask "{1: normal computation at specified times} {2: computation at mesh points} {3: one step at one internal mesh point and return} {4: normal computation without overshooting tcrit} {5: one step, without passing  tcrit , and return}"
	set sciGUITable(win,$winId2,data,itask) [lindex $itask [expr [lindex $parameters 7] - 1] ]
	set sciGUITable(win,$winId2,data,tcrit) [lindex $parameters 8]
	set sciGUITable(win,$winId2,data,h0) [lindex $parameters 9]
	set sciGUITable(win,$winId2,data,hmax) [lindex $parameters 10]
	set sciGUITable(win,$winId2,data,hmin) [lindex $parameters 11]
	set jactype "{0: functional iterations, no jacobian used (adams or stiff only)} {1: user-supplied full jacobian} {2: internally generated full jacobian} {3: internally generated diagonal jacobian (adams or stiff only)} {4: user-supplied banded jacobian (see ml and mu below)} {5: internally generated banded jacobian (see ml and mu below)}"
	set sciGUITable(win,$winId2,data,jactype) [lindex $jactype [lindex $parameters 12]]
	set sciGUITable(win,$winId2,data,mxstep) [lindex $parameters 13]
	set sciGUITable(win,$winId2,data,maxordn) [lindex $parameters 14]
	set sciGUITable(win,$winId2,data,maxords) [lindex $parameters 15]
	set sciGUITable(win,$winId2,data,ixpr) [lindex $parameters 16]
	set sciGUITable(win,$winId2,data,ml) [lindex $parameters 17]
	set sciGUITable(win,$winId2,data,mu) [lindex $parameters 18]
	
	### DISPLAY
	set wt1 [Rnotebook:frame $w.cen.book 1]
	frame $wt1.f1
	label $wt1.f1.ieetxt -text "Floating point exception :"
	combobox::combobox $wt1.f1.ieee -borderwidth 1 -textvariable sciGUITable(win,$winId2,data,ieee) -editable false -highlightthickness 0 -width 40 	
	eval $wt1.f1.ieee list insert end $ieemodes
	pack $wt1.f1.ieetxt $wt1.f1.ieee -side left -fill x -expand 0
	pack $wt1.f1 -side top -expand 0 -fill both -pady 5

	frame $wt1.f2
	label $wt1.f2.formatmodetxt -text "Printing Format :"
	combobox::combobox $wt1.f2.formatmode -borderwidth 1 -textvariable sciGUITable(win,$winId2,data,formatmode) -editable false -highlightthickness 0 -width 15
	eval $wt1.f2.formatmode list insert end $formatmodes
	label $wt1.f2.formatmodewidthtxt -text "width :"
	entry $wt1.f2.formatmodewidth -width 5 -textvariable sciGUITable(win,$winId2,data,formatmode_wid)
	pack $wt1.f2.formatmodetxt $wt1.f2.formatmode $wt1.f2.formatmodewidthtxt $wt1.f2.formatmodewidth -side left -fill x -expand 0
	pack $wt1.f2 -side top -expand 0 -fill both -pady 5

	frame $wt1.f3
	label $wt1.f3.lineslintxt -text "Lines to display :"
	entry $wt1.f3.lineslin -textvariable sciGUITable(win,$winId2,data,lineslin) -width 5
	label $wt1.f3.linescoltxt -text "Columns to display :"
	entry $wt1.f3.linescol -textvariable sciGUITable(win,$winId2,data,linescol) -width 5
	pack $wt1.f3.lineslintxt $wt1.f3.lineslin $wt1.f3.linescoltxt $wt1.f3.linescol -side left -fill x -expand 0
	pack $wt1.f3 -side top -expand 0 -fill both -pady 5
	
	### STACK
	set wt2 [Rnotebook:frame $w.cen.book 2]	
	label $wt2.ssizetx -text "Stack Size :" 
	label $wt2.gsizetx -text "Global Stack Size :"
	entry $wt2.ssize -width 10 -textvariable sciGUITable(win,$winId2,data,ssize)
	entry $wt2.gsize -width 10 -textvariable sciGUITable(win,$winId2,data,gsize)
	grid config $wt2.ssizetx -column 0 -row 0 -sticky "e"
	grid config $wt2.ssize -column 1 -row 0 -sticky "w"
	grid config $wt2.gsizetx -column 0 -row 1 -sticky "e"
	grid config $wt2.gsize -column 1 -row 1 -sticky "w"

	### ODEOPTIONS
	set wt3 [Rnotebook:frame $w.cen.book 3]	
	frame $wt3.f1
	label $wt3.f1.itasktxt -text "itask :"
	combobox::combobox $wt3.f1.formatmode -borderwidth 1 -textvariable sciGUITable(win,$winId2,data,itask) -editable false -highlightthickness 0 -width 40
	eval $wt3.f1.formatmode list insert end $itask
	pack $wt3.f1.itasktxt $wt3.f1.formatmode -side left -fill x -expand 0
	pack $wt3.f1 -side top -expand 0 -fill both -pady 2

	frame $wt3.f2
	label $wt3.f2.tcrittxt -text "tcrit ( itask==4 | itask==5) :"
	entry $wt3.f2.tcrit -textvariable sciGUITable(win,$winId2,data,tcrit) -width 10
	pack $wt3.f2.tcrittxt $wt3.f2.tcrit -side left -fill x -expand 0
	pack $wt3.f2 -side top -expand 0 -fill both -pady 2

	frame $wt3.f3
	label $wt3.f3.h0txt -text "First step tried (h0):"
	entry $wt3.f3.h0 -textvariable sciGUITable(win,$winId2,data,h0) -width 10
	pack $wt3.f3.h0txt $wt3.f3.h0 -side left -fill x -expand 0
	pack $wt3.f3 -side top -expand 0 -fill both -pady 2

	frame $wt3.f4
	label $wt3.f4.hmaxtxt -text "Max step size (hmax):"
	entry $wt3.f4.hmax -textvariable sciGUITable(win,$winId2,data,hmax) -width 10
	pack $wt3.f4.hmaxtxt $wt3.f4.hmax -side left -fill x -expand 0
	pack $wt3.f4 -side top -expand 0 -fill both -pady 2

	frame $wt3.f5
	label $wt3.f5.hmintxt -text "Min step size (hmin):"
	entry $wt3.f5.hmin -textvariable sciGUITable(win,$winId2,data,hmin) -width 10
	pack $wt3.f5.hmintxt $wt3.f5.hmin -side left -fill x -expand 0
	pack $wt3.f5 -side top -expand 0 -fill both -pady 2

	frame $wt3.f6
	label $wt3.f6.jactypetxt -text "jactype:"
	combobox::combobox $wt3.f6.jactype -borderwidth 1 -textvariable sciGUITable(win,$winId2,data,jactype) -editable false -highlightthickness 0 -width 50
	eval $wt3.f6.jactype list insert end $jactype
	pack $wt3.f6.jactypetxt $wt3.f6.jactype -side left -fill x -expand 0
	pack $wt3.f6 -side top -expand 0 -fill both -pady 2
	
	frame $wt3.f7
	label $wt3.f7.mxsteptxt -text "Max number of steps allowed (mxstep):"
	entry $wt3.f7.mxstep -textvariable sciGUITable(win,$winId2,data,mxstep) -width 10
	pack  $wt3.f7.mxsteptxt $wt3.f7.mxstep -side left -fill x -expand 0
	pack $wt3.f7 -side top -expand 0 -fill both -pady 2

	frame $wt3.f8
	label $wt3.f8.maxordntxt -text "Maximum non-stiff order allowed, at most 12 (maxordn):"
	entry $wt3.f8.maxordn -textvariable sciGUITable(win,$winId2,data,maxordn) -width 10
	pack  $wt3.f8.maxordntxt $wt3.f8.maxordn -side left -fill x -expand 0
	pack $wt3.f8 -side top -expand 0 -fill both -pady 2


	frame $wt3.f9
	label $wt3.f9.maxordstxt -text "Maximum stiff order allowed, at most 5 (maxords):"
	entry $wt3.f9.maxords -textvariable sciGUITable(win,$winId2,data,maxords) -width 10
	pack  $wt3.f9.maxordstxt $wt3.f9.maxords -side left -fill x -expand 0
	pack $wt3.f9 -side top -expand 0 -fill both -pady 2

	frame $wt3.f10
	label $wt3.f10.ixprtxt -text "Print level (ixpr):"
	combobox::combobox $wt3.f10.ixpr -borderwidth 1 -textvariable sciGUITable(win,$winId2,data,ixpr) -editable false -highlightthickness 0 -width 2
	eval $wt3.f10.ixpr list insert end "0 1"
	pack  $wt3.f10.ixprtxt $wt3.f10.ixpr -side left -fill x -expand 0
	pack $wt3.f10 -side top -expand 0 -fill both -pady 2

	frame $wt3.f11
	label $wt3.f11.mltxt -text "ml:"
	entry $wt3.f11.ml -textvariable sciGUITable(win,$winId2,data,ml) -width 10
	pack  $wt3.f11.mltxt $wt3.f11.ml -side left -fill x -expand 0
	pack $wt3.f11 -side top -expand 0 -fill both -pady 2

	frame $wt3.f12
	label $wt3.f12.mutxt -text "mu:"
	entry $wt3.f12.mu -textvariable sciGUITable(win,$winId2,data,mu) -width 10
	pack  $wt3.f12.mutxt $wt3.f12.mu -side left -fill x -expand 0
	pack $wt3.f12 -side top -expand 0 -fill both -pady 2

	
}


