#Copyright INRIA
#2004
#Author: F.Leray

set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd

global MySciPath
set MySciPath [file join  "$env(SCIPATH)"]


variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/tcl/BWidget-1.7.0"]
namespace inscope :: package require BWidget
package require BWidget


set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
source [file join $sourcedir Balloon.tcl]

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

global scicomint_colormap


#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}


set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Figure Editor"
wm iconname $ww "FE"
wm geometry $ww 690x650
wm protocol $ww WM_DELETE_WINDOW "DestroyGlobals; destroy $ww "


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

#set w $ww
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

set comb [combobox $w.frame.selgedobject \
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


Notebook:create  $uf.n -pages {Style Mode Colormap} -pad 20   -height 520 -width 600
pack  $uf.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame  $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "       Visibility: "
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib"
OnOffForeground $w.frame.visib $curvis

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
bind  $w.frame.figlabel1 <KP_Enter> {setFiglabel} 

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
pack $w.frame.xfigposlabel1  -in  $w.frame.xfigpos   -pady 2m -padx 2m -side left 
bind  $w.frame.xfigposlabel1 <Return> {setFigPos} 
bind  $w.frame.xfigposlabel1 <KP_Enter> {setFigPos} 

#figure position y
frame $w.frame.yfigpos -borderwidth 0
pack $w.frame.yfigpos  -in $w.frame -side top   -fill x

label $w.frame.yfigposlabel -text "   Y position : "
entry $w.frame.yfigposlabel1 -relief sunken  -textvariable figure_yposition
pack $w.frame.yfigposlabel -in  $w.frame.yfigpos -side left
pack $w.frame.yfigposlabel1  -in  $w.frame.yfigpos  -pady 2m -padx 2m -side left 
bind  $w.frame.yfigposlabel1 <Return> {setFigPos} 
bind  $w.frame.yfigposlabel1 <KP_Enter> {setFigPos} 

#figure size x
frame $w.frame.xfigsiz -borderwidth 0
pack $w.frame.xfigsiz  -in $w.frame -side top   -fill x

label $w.frame.xfigsizlabel -text "         X size : "
entry $w.frame.xfigsizlabel1 -relief sunken  -textvariable figure_xsiz
pack $w.frame.xfigsizlabel -in  $w.frame.xfigsiz -side left
pack $w.frame.xfigsizlabel1  -in  $w.frame.xfigsiz  -pady 2m -padx 2m -side left 
bind  $w.frame.xfigsizlabel1 <Return> {setFigSiz} 
bind  $w.frame.xfigsizlabel1 <KP_Enter> {setFigSiz} 

#figure size y
frame $w.frame.yfigsiz -borderwidth 0
pack $w.frame.yfigsiz  -in $w.frame -side top   -fill x

label $w.frame.yfigsizlabel -text "         Y size : "
entry $w.frame.yfigsizlabel1 -relief sunken  -textvariable figure_ysiz
pack $w.frame.yfigsizlabel -in  $w.frame.yfigsiz -side left
pack $w.frame.yfigsizlabel1  -in  $w.frame.yfigsiz  -pady 2m -padx 2m -side left 
bind  $w.frame.yfigsizlabel1 <Return> {setFigSiz} 
bind  $w.frame.yfigsizlabel1 <KP_Enter> {setFigSiz} 

#axes size x
frame $w.frame.xaxesiz -borderwidth 0
pack $w.frame.xaxesiz  -in $w.frame -side top   -fill x

label $w.frame.xaxesizlabel -text "  X axis size : "
entry $w.frame.xaxesizlabel1 -relief sunken  -textvariable figure_xaxesiz
pack $w.frame.xaxesizlabel -in  $w.frame.xaxesiz -side left
pack $w.frame.xaxesizlabel1  -in  $w.frame.xaxesiz  -pady 2m -padx 2m -side left 
bind  $w.frame.xaxesizlabel1 <Return> {setAxeSiz} 
bind  $w.frame.xaxesizlabel1 <KP_Enter> {setAxeSiz} 

#axes size y
frame $w.frame.yaxesiz -borderwidth 0
pack $w.frame.yaxesiz  -in $w.frame -side top   -fill x

label $w.frame.yaxesizlabel -text "  Y axis size : "
entry $w.frame.yaxesizlabel1 -relief sunken  -textvariable figure_yaxesiz
pack $w.frame.yaxesizlabel -in  $w.frame.yaxesiz -side left
pack $w.frame.yaxesizlabel1  -in  $w.frame.yaxesiz  -pady 2m -padx 2m -side left 
bind  $w.frame.yaxesizlabel1 <Return> {setAxeSiz} 
bind  $w.frame.yaxesizlabel1 <KP_Enter> {setAxeSiz} 

#Color scale background
frame $w.frame.clrb  -borderwidth 0
pack $w.frame.clrb  -in $w.frame -side top  -fill x
label $w.frame.bcolorlabel -height 0 -text "Background\n   color:         " -width 0 

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackColor $w.frame.bcolor" -tickinterval 0

pack $w.frame.bcolorlabel -in $w.frame.clrb -side left
pack $w.frame.bcolor -in  $w.frame.clrb -side left  -expand 1 -fill x -pady 2m -padx 2m
$w.frame.bcolor set $bcolor

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 12m

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1


########### Mode onglet ###########################################
###################################################################
set w [Notebook:frame  $uf.n Mode]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#auto resize
frame $w.frame.autorsz -borderwidth 0
pack $w.frame.autorsz  -in $w.frame  -side top -fill x
label $w.frame.autorszlabel  -text "            Auto resize: "
checkbutton $w.frame.autorszib  -text "on"\
    -variable curautoresize -onvalue "on" -offvalue "off" \
    -command "toggleResize $w.frame.autorszib"
OnOffForeground $w.frame.autorszib $curautoresize

pack $w.frame.autorszlabel -in $w.frame.autorsz  -side left
pack $w.frame.autorszib  -in $w.frame.autorsz    -side left -fill x -pady 2m -padx 2m


#pixmap
frame $w.frame.pix -borderwidth 0
pack $w.frame.pix  -in $w.frame  -side top -fill x
label $w.frame.pixlabel  -text "                  Pixmap: "
checkbutton $w.frame.pixib  -text "on"\
    -variable curpix  -onvalue "on" -offvalue "off" \
    -command "togglePix $w.frame.pixib"
OnOffForeground $w.frame.pixib $curpix

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

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 40m

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1


########### Colormap onglet #######################################
###################################################################
set w [Notebook:frame  $uf.n Colormap]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


frame $w.frame.fdata -borderwidth 0
pack $w.frame.fdata  -in $w.frame -side top   -fill x

scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
canvas $w.frame.c -width 8i -height 3.5i  -yscrollcommand {$w.frame.ysbar set}

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
    bind  $w.frame.c.reddata$i <KP_Enter> "setRedColor $w $i "
#location help balloon	
    set_balloon $w.frame.c.reddata$i "Row: $i Column: 1"
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.reddata$i

#GREEN
    set aa [expr 10+(2*150)]
    entry  $w.frame.c.gredata$i  -relief sunken  -textvariable GREEN($i)
    bind  $w.frame.c.gredata$i <Return> "setGreenColor $w $i "
    bind  $w.frame.c.gredata$i <KP_Enter> "setGreenColor $w $i "
#location help balloon	
#location help balloon	
    set_balloon $w.frame.c.gredata$i "Row: $i Column: 2"
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.gredata$i

#BLUE
    set aa [expr 10+(3*150)]
    entry  $w.frame.c.bludata$i  -relief sunken  -textvariable BLUE($i)
    bind  $w.frame.c.bludata$i <Return> "setBlueColor  $w $i "
    bind  $w.frame.c.bludata$i <KP_Enter> "setBlueColor  $w $i "
#location help balloon	
    set_balloon $w.frame.c.bludata$i "Row: $i Column: 3"
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


frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 2m

label $w.scicom1.label1 -text "Scilab Command Interface for colormap:"
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicom
pack $w.scicom -side top -fill x -pady 2m


label $w.scicom.label1 -text "Colormap (Nx3 double array) = "
pack  $w.scicom.label1 -in $w.scicom -side left

#text $w.scicom.text1 -height 3m -width 50 -relief sunken -yscrollcommand "$w.scicom.scroll set"
entry $w.scicom.text1 -relief sunken -textvariable scicomint_colormap
set_balloon $w.scicom.text1 "Enter a graycolormap(COLOR_NUMBER), hotcolormap(COLOR_NUMBER)\n or jetcolormap(COLOR_NUMBER) call to initialize the \"colormap\" field."
bind  $w.scicom.text1 <Return> "sciCommandColormap"
bind  $w.scicom.text1 <KP_Enter> "sciCommandColormap"

pack $w.scicom.text1  -side left -fill both -expand yes

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 10m

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1


pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes



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


proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
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



proc togglePix { frame } {
    global curpix
    ScilabEval "global ged_handle;ged_handle.pixmap='$curpix'"

    OnOffForeground $frame $curpix
}

proc toggleResize { frame } {
    global curautoresize
    ScilabEval "global ged_handle;ged_handle.auto_resize='$curautoresize'"

    OnOffForeground $frame $curautoresize
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


proc sciCommandColormap {} {
    global scicomint_colormap

    set longueur [expr [string length $scicomint_colormap]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify an entry (such as graycolormap(COLOR_NUMBER), hotcolormap(COLOR_NUMBER) or jetcolormap(COLOR_NUMBER) or...) to initialize the \"colormap\" field."
    } else {
	
	ScilabEval "global ged_handle;ged_handle.color_map=$scicomint_colormap;" "seq"
	#Refresh now !
	ScilabEval "tkged();" "seq"
    }
}


proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"
}