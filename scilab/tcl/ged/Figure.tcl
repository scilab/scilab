
set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
package require combobox 2.3
catch {namespace import combobox::*}

global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject


global RED BLUE GREEN
global figure_name figure_id background figure_xposition figure_yposition
global figure_xsiz figure_ysiz figure_xaxesiz figure_yaxesiz
global curvis
global ncolors
global curpix curpdm currotation_style

set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Figure Editor"
wm iconname $ww "FE"
wm geometry $ww 650x700

set w $ww
frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#Hierarchical selection
set lalist ""
for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
append lalist "\""
append lalist "$SELOBJECT($i)" 
append lalist "\" "
}

set curgedobject $SELOBJECT($curgedindex)


#Hiereachical viewer
frame $w.frame.view  -borderwidth 0
pack $w.frame.view  -in $w.frame  -side top  -fill x

label $w.frame.selgedobjectlabel  -height 0 -text "Edit properties for:    " -width 0 
combobox $w.frame.selgedobject \
    -borderwidth 2 \
    -highlightthickness 3 \
    -maxheight 0 \
    -width 3 \
    -textvariable curgedobject \
    -editable false \
    -background white \
    -command [list SelectObject ]
eval $w.frame.selgedobject list insert end $lalist
pack $w.frame.selgedobjectlabel -in $w.frame.view   -side left
pack $w.frame.selgedobject   -in $w.frame.view   -fill x

Notebook:create .axes.n -pages {Style Mode Colormap} -pad 20 
pack .axes.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame .axes.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "       Visibility: "
checkbutton $w.frame.visib  -textvariable curvis  \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis" 
pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x -pady 2m -padx 2m


#figure label
frame $w.frame.lbfig -borderwidth 0
pack $w.frame.lbfig  -in $w.frame -side top   -fill x

label $w.frame.figlabel -text " Figure name: "
entry $w.frame.figlabel1 -relief sunken  -textvariable figure_name
pack $w.frame.figlabel -in  $w.frame.lbfig -side left
pack $w.frame.figlabel1  -in  $w.frame.lbfig  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.figlabel1 <Return> {setFiglabel} 

#figure id
#label tout court non modifiable
frame $w.frame.lbfigid -borderwidth 0
pack $w.frame.lbfigid  -in $w.frame -side top   -fill x

label $w.frame.figidlabel -text "     Figure id:"
label $w.frame.figidlabel1 -textvariable figure_id
pack $w.frame.figidlabel $w.frame.figidlabel1 -in  $w.frame.lbfigid -side left  -fill x -pady 2m -padx 2m


#figure position x
frame $w.frame.xfigpos -borderwidth 0
pack $w.frame.xfigpos  -in $w.frame -side top   -fill x

label $w.frame.xfigposlabel -text "   X position : "
entry $w.frame.xfigposlabel1 -relief sunken  -textvariable figure_xposition
pack $w.frame.xfigposlabel -in  $w.frame.xfigpos -side left
pack $w.frame.xfigposlabel1  -in  $w.frame.xfigpos  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.xfigposlabel1 <Return> {setFigPos} 


#figure position y
frame $w.frame.yfigpos -borderwidth 0
pack $w.frame.yfigpos  -in $w.frame -side top   -fill x

label $w.frame.yfigposlabel -text "   Y position : "
entry $w.frame.yfigposlabel1 -relief sunken  -textvariable figure_yposition
pack $w.frame.yfigposlabel -in  $w.frame.yfigpos -side left
pack $w.frame.yfigposlabel1  -in  $w.frame.yfigpos  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.yfigposlabel1 <Return> {setFigPos} 

#figure size x
frame $w.frame.xfigsiz -borderwidth 0
pack $w.frame.xfigsiz  -in $w.frame -side top   -fill x

label $w.frame.xfigsizlabel -text "         X size : "
entry $w.frame.xfigsizlabel1 -relief sunken  -textvariable figure_xsiz
pack $w.frame.xfigsizlabel -in  $w.frame.xfigsiz -side left
pack $w.frame.xfigsizlabel1  -in  $w.frame.xfigsiz  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.xfigsizlabel1 <Return> {setFigSiz} 


#figure size y
frame $w.frame.yfigsiz -borderwidth 0
pack $w.frame.yfigsiz  -in $w.frame -side top   -fill x

label $w.frame.yfigsizlabel -text "         Y size : "
entry $w.frame.yfigsizlabel1 -relief sunken  -textvariable figure_ysiz
pack $w.frame.yfigsizlabel -in  $w.frame.yfigsiz -side left
pack $w.frame.yfigsizlabel1  -in  $w.frame.yfigsiz  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.yfigsizlabel1 <Return> {setFigSiz} 


#axes size x
frame $w.frame.xaxesiz -borderwidth 0
pack $w.frame.xaxesiz  -in $w.frame -side top   -fill x

label $w.frame.xaxesizlabel -text "  X axis size : "
entry $w.frame.xaxesizlabel1 -relief sunken  -textvariable figure_xaxesiz
pack $w.frame.xaxesizlabel -in  $w.frame.xaxesiz -side left
pack $w.frame.xaxesizlabel1  -in  $w.frame.xaxesiz  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.xaxesizlabel1 <Return> {setAxeSiz} 


#axes size y
frame $w.frame.yaxesiz -borderwidth 0
pack $w.frame.yaxesiz  -in $w.frame -side top   -fill x

label $w.frame.yaxesizlabel -text "  Y axis size : "
entry $w.frame.yaxesizlabel1 -relief sunken  -textvariable figure_yaxesiz
pack $w.frame.yaxesizlabel -in  $w.frame.yaxesiz -side left
pack $w.frame.yaxesizlabel1  -in  $w.frame.yaxesiz  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.yaxesizlabel1 <Return> {setAxeSiz} 


#Color scale background
frame $w.frame.clrb  -borderwidth 0
pack $w.frame.clrb  -in $w.frame -side top  -fill x
label $w.frame.bcolorlabel -height 0 -text "Background\n   color:         " -width 0 

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackColor $w.frame.bcolor" -tickinterval 0

pack $w.frame.bcolorlabel -in $w.frame.clrb -side left
pack $w.frame.bcolor -in  $w.frame.clrb -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.bcolor set $bcolor

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1


########### Mode onglet ###########################################
###################################################################
set w [Notebook:frame .axes.n Mode]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#auto resize
frame $w.frame.autorsz -borderwidth 0
pack $w.frame.autorsz  -in $w.frame  -side top -fill x
label $w.frame.autorszlabel  -text "            Auto resize: "
checkbutton $w.frame.autorszib  -textvariable curautoresize  \
    -variable curautoresize -onvalue "on" -offvalue "off" \
    -command "toggleResize" 
pack $w.frame.autorszlabel -in $w.frame.autorsz  -side left
pack $w.frame.autorszib  -in $w.frame.autorsz    -side left -fill x -pady 2m -padx 2m


#pixmap
frame $w.frame.pix -borderwidth 0
pack $w.frame.pix  -in $w.frame  -side top -fill x
label $w.frame.pixlabel  -text "                  Pixmap: "
checkbutton $w.frame.pixib  -textvariable curpix  \
    -variable curpix  -onvalue "on" -offvalue "off" \
    -command "togglePix" 
pack $w.frame.pixlabel -in $w.frame.pix  -side left
pack $w.frame.pixib  -in $w.frame.pix    -side left -fill x -pady 2m -padx 2m


#pixel drawing mode
frame $w.frame.pixel  -borderwidth 0
pack $w.frame.pixel  -in $w.frame -side top -fill x

label $w.frame.pixdmodelbl  -height 0 -text "Pixel drawing mode:  " -width 0 
combobox $w.frame.pixdmode \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curpdm \
    -editable false \
    -command [list SelectPixelDrawingMode]
eval $w.frame.pixdmode list insert end [list "clear" "and" "andReverse"  "copy" "andInverted"  "noop"  "xor"  "or"  "nor"  "equiv"  "invert"  "orReverse"  "copyInverted"  "orInverted"  "nand"  "set" ]

pack $w.frame.pixdmodelbl -in $w.frame.pixel   -side left
pack $w.frame.pixdmode -in $w.frame.pixel  -expand 1 -fill x -pady 2m -padx 2m


#rotation style
frame $w.frame.rotstyle  -borderwidth 0
pack $w.frame.rotstyle  -in $w.frame -side top -fill x

label $w.frame.rotstylelbl  -height 0 -text "        Rotation style:  " -width 0 
combobox $w.frame.rotstyl \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable currotation_style \
    -editable false \
    -command [list SelectRotationStyle]
eval $w.frame.rotstyl list insert end [list "unary" "multiple"]

pack $w.frame.rotstylelbl -in $w.frame.rotstyle   -side left
pack $w.frame.rotstyl -in $w.frame.rotstyle  -expand 1 -fill x -pady 2m -padx 2m

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1


########### Colormap onglet ###########################################
###################################################################
set w [Notebook:frame .axes.n Colormap]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


frame $w.frame.fdata -borderwidth 0
pack $w.frame.fdata  -in $w.frame -side top   -fill x

scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
canvas $w.frame.c -width 8i -height 6i  -yscrollcommand {$w.frame.ysbar set}

$w.frame.c create text 160 10 -anchor c -text "RED"
$w.frame.c create text 310 10 -anchor c -text "GREEN"
$w.frame.c create text 460 10 -anchor c -text "BLUE"

for {set i 1} {$i<=$ncolors} {incr i} {
    set bb [expr 10+(25*$i)]
    $w.frame.c create text 10 $bb -anchor c -text $i
#RED
    set aa [expr 10+(1*150)]
    entry  $w.frame.c.reddata$i  -relief sunken  -textvariable RED($i)
    bind  $w.frame.c.reddata$i <Return> "setRedColor $w $i "
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.reddata$i

#GREEN
    set aa [expr 10+(2*150)]
    entry  $w.frame.c.gredata$i  -relief sunken  -textvariable GREEN($i)
    bind  $w.frame.c.gredata$i <Return> "setGreenColor $w $i "
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.gredata$i

#BLUE
    set aa [expr 10+(3*150)]
    entry  $w.frame.c.bludata$i  -relief sunken  -textvariable BLUE($i)
    bind  $w.frame.c.bludata$i <Return> "setBlueColor  $w $i "
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.bludata$i
    
#sample
    set REDC $RED($i) 
    set GREC $GREEN($i) 
    set BLUC $BLUE($i) 
    
    set color [format \#%02x%02x%02x [expr int($REDC*255)]  [expr int($GREC*255)]  [expr int($BLUC*255)]]
    set aa [expr 10+(4*150) - 60]
    $w.frame.c create rectangle $aa [expr $bb-10] [expr $aa+20] [expr $bb+10] -fill $color

}

$w.frame.c configure -scrollregion [$w.frame.c bbox all] -yscrollincrement 0.1i

pack  $w.frame.ysbar -side right -fill y
pack  $w.frame.c


#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1



#proc associes
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



proc setFiglabel {} {
global figure_name
ScilabEval "global ged_handle;ged_handle.figure_name='$figure_name'"
}


proc toggleVis {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
}


proc setFigPos {} {
global figure_yposition
global figure_xposition
ScilabEval "global ged_handle;ged_handle.figure_position=\[$figure_xposition , $figure_yposition\]"
}

proc setFigSiz {} {
global figure_xsiz
global figure_ysiz
ScilabEval "global ged_handle;ged_handle.figure_size=\[$figure_xsiz , $figure_ysiz\]"
}


proc setAxeSiz {} {
global figure_xaxesiz
global figure_yaxesiz
ScilabEval "global ged_handle;ged_handle.axes_size=\[$figure_xaxesiz , $figure_yaxesiz\]"
}

proc setBackColor {w index} {  
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc togglePix {} {
global curpix
ScilabEval "global ged_handle;ged_handle.pixmap='$curpix'"
}

proc toggleResize {} {
global curautoresize
ScilabEval "global ged_handle;ged_handle.auto_resize='$curautoresize'"
}


proc SelectPixelDrawingMode {w args} {
global curpdm
ScilabEval "global ged_handle; ged_handle.pixel_drawing_mode ='$curpdm'"
}


proc SelectRotationStyle {w args} {
global currotation_style
ScilabEval "global ged_handle; ged_handle.rotation_style ='$currotation_style'"
}


proc setRedColor { w i } {
    global RED GREEN BLUE
    variable REDC GREC BLUC color aa bb
    ScilabEval "execstr(\"global ged_handle; ged_handle.color_map($i,1)=$RED($i);\",\'errcatch\',\'n\');"
    set REDC $RED($i) 
    set GREC $GREEN($i) 
    set BLUC $BLUE($i) 
    set bb [expr 10+(25*$i)]
    set color [format \#%02x%02x%02x [expr int($REDC*255)]  [expr int($GREC*255)]  [expr int($BLUC*255)]]
    set aa [expr 10+(4*150) - 60]
    $w.frame.c create rectangle $aa [expr $bb-10] [expr $aa+20] [expr $bb+10] -fill $color

}


proc setGreenColor { w i } {
    global RED GREEN BLUE
    variable REDC GREC BLUC color aa bb
    ScilabEval "execstr(\"global ged_handle; ged_handle.color_map($i,2)=$GREEN($i);\",\'errcatch\',\'n\');"
    set REDC $RED($i) 
    set GREC $GREEN($i) 
    set BLUC $BLUE($i) 
    set bb [expr 10+(25*$i)]
    set color [format \#%02x%02x%02x [expr int($REDC*255)]  [expr int($GREC*255)]  [expr int($BLUC*255)]]
    set aa [expr 10+(4*150) - 60]
    $w.frame.c create rectangle $aa [expr $bb-10] [expr $aa+20] [expr $bb+10] -fill $color
}


proc setBlueColor { w i } {
    global RED GREEN BLUE
    variable REDC GREC BLUC color aa bb
    ScilabEval "execstr(\"global ged_handle; ged_handle.color_map($i,3)=$BLUE($i);\",\'errcatch\',\'n\');"
    set REDC $RED($i) 
    set GREC $GREEN($i) 
    set BLUC $BLUE($i) 
    set bb [expr 10+(25*$i)]
    set color [format \#%02x%02x%02x [expr int($REDC*255)]  [expr int($GREC*255)]  [expr int($BLUC*255)]]
    set aa [expr 10+(4*150) - 60]
    $w.frame.c create rectangle $aa [expr $bb-10] [expr $aa+20] [expr $bb+10] -fill $color
}

