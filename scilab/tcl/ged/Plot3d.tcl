source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
package require combobox 2.3
catch {namespace import combobox::*}

global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject

global curvis
global curcolormode curthick colorflagToggle curforeground curhiddencolor
global nbrowX nbrowY plot3dXVAL plot3dYVAL plot3dZVAL plot3dCOLORVAL flagCOLOR
global nbrowZ nbcolZ nbrowCOLOR nbcolCOLOR

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Plot3d Object"
wm iconname $ww "PE"
wm geometry $ww 690x690

set topf  [frame $ww.topf]
set titf1 [TitleFrame $topf.titf1 -text "Graphic Editor"]

set parent  [$titf1 getframe]
set pw1  [PanedWindow $parent.pw -side top]
set pane3  $pw1  



# Make a frame scrollable

set sw [ScrolledWindow $pane3.sw -relief sunken -borderwidth 2]
# pack $sw -in .sw -fill both -expand true 
set sf [ScrollableFrame $sw.f]
$sw setwidget $sf
set uf [$sf getframe]



set w $uf
set fra [frame $w.frame -borderwidth 0]
pack $fra  -anchor w -fill both
#frame $w.frame -borderwidth 0
#pack $w.frame -anchor w -fill both

#Hierarchical selection
set lalist ""
for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
append lalist "\""
append lalist "$SELOBJECT($i)" 
append lalist "\" "
}

set curgedobject $SELOBJECT($curgedindex)


#Hiereachical viewer
set fra [frame $w.frame.view  -borderwidth 0]
pack $fra -in $w.frame  -side top  -fill x
#frame $w.frame.view  -borderwidth 0
#pack $w.frame.view  -in $w.frame  -side top  -fill x

#label $w.frame.selgedobjectlabel  -height 0 -text "Edit properties for:    " -width 0 
set lab [label $w.frame.selgedobjectlabel  -height 0 -text "Edit properties for:    " -width 0 ]
pack $lab -in $w.frame.view   -side left

set comb [ combobox $w.frame.selgedobject \
	       -borderwidth 2 \
	       -highlightthickness 3 \
	       -maxheight 0 \
	       -width 3 \
	       -textvariable curgedobject \
	       -editable false \
	       -background white \
	       -command [list SelectObject ]]
pack $comb  -in $w.frame.view  -fill x
eval $w.frame.selgedobject list insert end $lalist
#pack $w.frame.selgedobjectlabel -in $w.frame.view   -side left
#pack $w.frame.selgedobject   -in $w.frame.view   -fill x


if {$flagCOLOR == 1} {
Notebook:create $uf.n -pages {"Style" "X vector" "Y vector" "Z matrix" "Color data"} -pad 20 -height 540 -width 600
} else {
Notebook:create $uf.n -pages {"Style" "X vector" "Y vector" "Z matrix" } -pad 20 -height 540 -width 600
}
pack $uf.n -in $uf -fill both -expand yes

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n "Style"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "       Visibility:   "
checkbutton $w.frame.visib  -textvariable curvis  \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis" 
pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x

#Color Mode
frame $w.frame.clrm  -borderwidth 0
pack $w.frame.clrm  -in $w.frame -side top  -fill x

label $w.frame.colorlabelm -height 0 -text "   Color mode: " -width 0 
scale $w.frame.colorm -orient horizontal -from -$ncolors -to $ncolors \
	 -resolution 1.0 -command "setColorMode $w.frame.colorm" -tickinterval 0 

pack $w.frame.colorlabelm -in $w.frame.clrm -side left
pack $w.frame.colorm  -in  $w.frame.clrm -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.colorm set $curcolormode

#Foreground
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "   Foreground: " -width 0 
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setForeground $w.frame.color" -tickinterval 0 

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color  -in  $w.frame.clrf -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.color set $curforeground

#Hidden Color
frame $w.frame.clrh  -borderwidth 0
pack $w.frame.clrh  -in $w.frame -side top  -fill x

label $w.frame.colorlabelh -height 0 -text " Hidden color: " -width 0 
scale $w.frame.colorh -orient horizontal -from 0 -to $ncolors \
	 -resolution 1.0 -command "setHiddenColor $w.frame.colorh" -tickinterval 0 

pack $w.frame.colorlabelh -in $w.frame.clrh -side left
pack $w.frame.colorh  -in  $w.frame.clrh -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.colorh set $curhiddencolor


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x

label $w.frame.scalelabel -height 0 -text "    Thickness: " -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk  -expand 1 -fill x -pady 2m -padx 2m
$w.frame.thickness set $curthick


#Color Flag
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.scalesw.label -height 0 -text "      Color flag: " -width 0 
radiobutton $w.frame.scalesw.radio0 -text "0" -variable colorflagToggle -value 0    -command "toggleColorFlag" 
radiobutton $w.frame.scalesw.radio1 -text "1" -variable colorflagToggle -value 1    -command "toggleColorFlag"

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left
pack $w.frame.scalesw.radio0 -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radio1  -in  $w.frame.scalesw    -side left 



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both  -pady 30m

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1




########### X vector onglet #######################################
###################################################################
set w [Notebook:frame $uf.n "X vector"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

frame $w.frame.fdata -borderwidth 0
pack $w.frame.fdata  -in $w.frame -side top   -fill x

scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
canvas $w.frame.c -width 8i -height 6i  -yscrollcommand {$w.frame.ysbar set}

#$w.frame.c create text 160 10 -anchor c -text "X vector"

for {set i 1} {$i<=$nbrowX} {incr i} {
    set bb [expr 10+(25*$i)]
    $w.frame.c create text 10 $bb -anchor c -text $i
    entry  $w.frame.c.data$i  -relief sunken  -textvariable plot3dXVAL($i)
    bind  $w.frame.c.data$i <Return> "setXData $i "
    
    $w.frame.c create window 160 $bb -anchor c -window $w.frame.c.data$i
}

$w.frame.c configure -scrollregion [$w.frame.c bbox all] -yscrollincrement 0.1i

pack  $w.frame.ysbar -side right -fill y
pack  $w.frame.c



#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 



########### Y vector onglet #######################################
###################################################################
 set w2 [Notebook:frame $uf.n "Y vector"]

 frame $w2.frame2 -borderwidth 0
 pack $w2.frame2 -anchor w -fill both

 frame $w2.frame2.fdata -borderwidth 0
 pack $w2.frame2.fdata  -in $w2.frame2 -side top   -fill x


 canvas $w2.frame2.c1 -width 8i -height 6i  -yscrollcommand {$w2.frame2.ysbar set}
scrollbar $w2.frame2.ysbar -orient vertical -command   {$w2.frame2.c1 yview}

#$w2.frame2.c1 create text 160 10 -anchor c -text "Y vector"

 for {set i 1} {$i<=$nbrowY} {incr i} {
     set bb [expr 10+(25*$i)]
     $w2.frame2.c1 create text 10 $bb -anchor c -text $i
     entry  $w2.frame2.c1.data$i  -relief sunken  -textvariable plot3dYVAL($i)
     bind  $w2.frame2.c1.data$i <Return> "setYData $i "
    
     $w2.frame2.c1 create window 160 $bb -anchor c -window $w2.frame2.c1.data$i
 }

 $w2.frame2.c1 configure -scrollregion [$w2.frame2.c1 bbox all] -yscrollincrement 0.1i

 pack  $w2.frame2.ysbar -side right -fill y
 pack  $w2.frame2.c1


 #exit button
 frame $w2.buttons
 button $w2.b -text Quit -command "destroy $ww"
 pack $w2.b -side bottom 



########### Z matrix onglet #######################################
###################################################################
 set w3 [Notebook:frame $uf.n "Z matrix"]

 frame $w3.frame2 -borderwidth 0
 pack $w3.frame2 -anchor w -fill both

 frame $w3.frame2.fdata -borderwidth 0
 pack $w3.frame2.fdata  -in $w3.frame2 -side top   -fill x


canvas $w3.frame2.c1 -width 8i -height 6i  -yscrollcommand {$w3.frame2.ysbar set} -xscrollcommand {$w3.frame2.xsbar set}
scrollbar $w3.frame2.ysbar -orient vertical -command   {$w3.frame2.c1 yview}
scrollbar $w3.frame2.xsbar -orient horizontal -command   {$w3.frame2.c1 xview}
#scrollbar $w3.frame2.ysbar.scrollh -orient horizontal -command {$w.frame2.c1 xview}
#pack configure $w3.frame2.ysbar.scrollh -side bottom -fill x -expand 1 
   
for {set i 1} {$i<=$nbrowZ} {incr i} {
#    puts $i
    set bb [expr 10+(25*$i)]
    $w3.frame2.c1 create text 10 $bb -anchor c -text $i
    for {set j 1} {$j<=$nbcolZ} {incr j} {
#	puts $j
	set aa [expr 10+($j*150)]
	$w3.frame2.c1 create text $aa 10 -anchor c -text $j
	set tmp $w3.frame2.c1.data$i
	set tmp $tmp+"_"
	entry  $tmp$j  -relief sunken  -textvariable plot3dZVAL($i,$j)
	bind   $tmp$j <Return> "setZData $i $j"
	
	$w3.frame2.c1 create window $aa $bb -anchor c -window $tmp$j
    }
}

 $w3.frame2.c1 configure -scrollregion [$w3.frame2.c1 bbox all] -yscrollincrement 0.1i -xscrollincrement 0.1i
# $w3.frame2.c1 configure -scrollregion [$w3.frame2.c1 bbox all] -xscrollincrement 0.1i

#pack  $w3.frame2.ysbar.scrollh -side bottom -fill x
 pack  $w3.frame2.ysbar -side right -fill y
 pack  $w3.frame2.xsbar -side bottom -fill x
 pack  $w3.frame2.c1


 #exit button
 frame $w3.buttons
 button $w3.b -text Quit -command "destroy $ww"
 pack $w3.b -side bottom 


if {$flagCOLOR == 1} {
########### Color matrix onglet ###################################
###################################################################
set w4 [Notebook:frame $uf.n "Color data"]

frame $w4.frame2 -borderwidth 0
pack $w4.frame2 -anchor w -fill both

message  $w4.frame2.warning  -text "This field is not yet taken into account with Plot3d entity. Create a Fac3d entity to use color data."
$w4.frame2.warning config -foreground red
pack $w4.frame2.warning  -in $w4.frame2 -side top -fill x

 frame $w4.frame2.fdata -borderwidth 0
 pack $w4.frame2.fdata  -in $w4.frame2 -side top   -fill x

canvas $w4.frame2.c1 -width 8i -height 6i  -yscrollcommand {$w4.frame2.ysbar set} -xscrollcommand {$w4.frame2.xsbar set}
scrollbar $w4.frame2.ysbar -orient vertical -command   {$w4.frame2.c1 yview}
scrollbar $w4.frame2.xsbar -orient horizontal -command   {$w4.frame2.c1 xview}
#scrollbar $w4.frame2.ysbar.scrollh -orient horizontal -command {$w.frame2.c1 xview}
#pack configure $w4.frame2.ysbar.scrollh -side bottom -fill x -expand 1 
   
for {set i 1} {$i<=$nbrowCOLOR} {incr i} {
#    puts $i
    set bb [expr 10+(25*$i)]
    $w4.frame2.c1 create text 10 $bb -anchor c -text $i
    for {set j 1} {$j<=$nbcolCOLOR} {incr j} {
#	puts $j
	set aa [expr 10+($j*150)]
	$w4.frame2.c1 create text $aa 10 -anchor c -text $j
	set tmp $w4.frame2.c1.data$i
	set tmp $tmp+"_"
	entry  $tmp$j  -relief sunken  -textvariable plot3dCOLORVAL($i,$j)
	bind   $tmp$j <Return> "setColorData $i $j"
	
	$w4.frame2.c1 create window $aa $bb -anchor c -window $tmp$j
    }
}

 $w4.frame2.c1 configure -scrollregion [$w4.frame2.c1 bbox all] -yscrollincrement 0.1i -xscrollincrement 0.1i
# $w4.frame2.c1 configure -scrollregion [$w4.frame2.c1 bbox all] -xscrollincrement 0.1i

#pack  $w4.frame2.ysbar.scrollh -side bottom -fill x
 pack  $w4.frame2.ysbar -side right -fill y
 pack  $w4.frame2.xsbar -side bottom -fill x
 pack  $w4.frame2.c1


 #exit button
 frame $w4.buttons
 button $w4.b -text Quit -command "destroy $ww"
 pack $w4.b -side bottom 
}
pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
}


proc SelectObject {w args} {
    global curgedobject;
    global ged_handle_list_size;
    global SELOBJECT
    
    set index 1

    for {set i 1} {$i<=$ged_handle_list_size} {incr i} {
	if {$curgedobject==$SELOBJECT($i)} {
	    set index $i
	}
    }
    ScilabEval "Get_handle_from_index($index);"
}



proc setColorMode {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
#     if { $index == -2 } {
# 	ScilabEval "global ged_handle; ged_handle.color_mode=$index;"
# 	#like $index==-2: display white color
# 	set color [format \#%02x%02x%02x 255 255 255]
# 	$w config  -activebackground $color -troughcolor $color
#     } else
    if { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.color_mode=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.color_mode=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.color_mode=$index;"
	if { $index < 0} {
	    set index [expr -$index]
	}
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setForeground {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setHiddenColor {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
   
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.hiddencolor=$index;"
	#like $index==1: display first color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.hiddencolor=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc toggleColorFlag {} {
    global colorflagToggle
    global ged_handle
    ScilabEval "global ged_handle;ged_handle.color_flag=$colorflagToggle"
}


proc setThickness {w thick} {
    ScilabEval "global ged_handle;ged_handle.thickness=$thick;"
}



proc setXData { i } {
global plot3dXVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data.x($i)=$plot3dXVAL($i);\",\'errcatch\',\'n\');"
}


proc setYData { i } {
global plot3dYVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data.y($i)=$plot3dYVAL($i);\",\'errcatch\',\'n\');"
}

proc setZData { i j } {
global plot3dZVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data.z($i,$j)=$plot3dZVAL($i,$j);\",\'errcatch\',\'n\');"
}


proc setColorData { i j } {
global plot3dCOLORVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data.color($i,$j)=$plot3dCOLORVAL($i,$j);\",\'errcatch\',\'n\');"
}

