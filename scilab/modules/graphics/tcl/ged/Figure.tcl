# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2004 - INRIA - Fabrice Leray
# Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.


set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd

variable DEMODIR2i

lappend ::auto_path [file dirname  "$env(SCIPATH)/modules/tclsci/tcl/BWidget"]
namespace inscope :: package require BWidget
package require BWidget

set sourcedir [file join "$env(SCIPATH)" "modules" "tclsci" "tcl" "utils"]
set sourcedir2 [file join "$env(SCIPATH)" "modules" "graphics" "tcl" "ged"]


source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
source [file join $sourcedir Balloon.tcl]
source [file join $sourcedir2 ObjectsBrowser.tcl]




package require combobox 2.3
catch {namespace import combobox::*}

#package require lemonTree
catch {namespace import LemonTree::*}



global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION msdos
set ged_listofpref "MAIN_WINDOW_POSITION TICK_WINDOW_POSITION"
set MAIN_WINDOW_POSITION "+0+0"
set TICK_WINDOW_POSITION "+0+0"

set envSCIHOME $SCIHOME
set preffilename $envSCIHOME/.GedPreferences.tcl
catch {source $preffilename}

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
global curpdm currotation_style

global scicomint_colormap
global smallPad mediumPad


#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {

    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 395
set NBwidth  330

set Wheight [expr $NBheight + 120]
set Wwidth  [expr $NBwidth  + 265]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Figure Editor"
wm iconname $ww "FE"
wm geometry $ww [expr $Wwidth]x[expr $Wheight]$MAIN_WINDOW_POSITION
wm maxsize  $ww $Wwidth $Wheight
wm protocol $ww WM_DELETE_WINDOW "DestroyGlobals; destroy $ww "


set topf  [frame $ww.topf]
set titf1 [TitleFrame $topf.titf1 -text "Graphic Editor" -font $gedFont]

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


#------------------------------------------------

set theframe $fra

#adding 15.06.2005
set topflabel  [frame $theframe.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Objects Browser" -font $gedFont]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Object Properties" -font $gedFont]

set w [$titf1label getframe]

pack $titf1label -padx 4 -side left -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx 4 -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

# The variable wfortree create an error in the combobox
# : "Error: can't read "wfortree": no such variable"
# The reason of the bug has nor been found but it is certainly in
# Combobox.tcl (with no combobox in the note book, no error)
# This bug only occurs under windows. However the result is OK.
set wfortree $w

for {set i 1} {$i<=$ged_handle_list_size} {incr i} {
    set OBJECTSARRAY($i) $SELOBJECT($i)
}

set curgedobject $SELOBJECT($curgedindex)

set tree  [Tree $wfortree.tree \
	       -yscrollcommand {$wfortree.y set} -xscrollcommand {$wfortree.x set} \
	       -width 20 -height 10 \
	       -background white -opencmd {LemonTree::open $wfortree.tree} \
	       -selectbackground blue -selectforeground white ]

pack [scrollbar $wfortree.x -orient horiz -command {$wfortree.tree xview}] -side bottom -fill x
pack [scrollbar $wfortree.y -command {$wfortree.tree yview}] -side right -fill y
pack $tree -fill both -expand 1 -side left

$tree bindText  <1> {LemonTree::Info $tree}
$tree bindImage <1> {LemonTree::Info $tree}

LemonTree::add $tree root FIGURE    currentfigure  "Figure(1)"

# I open the tree to browse all the nodes (to know what nodes I have and what their names are)
$tree opentree n1

set allnodes [$tree selection get]

#I close quickly the tree because openreeatnode expanded the tree...
$tree closetree n1


# I directly point onto the current curgedobject (current Axes or Figure or picked entity)
LemonTree::finddata $tree $allnodes $curgedobject


#adding 15.06.2005
set w [$titf1axes getframe]

set uf $w
#------------------------------------------------

set largeur 14

Notebook:create  $uf.n -pages {Style Mode Colormap} -pad 0   -height $NBheight -width $NBwidth
pack  $uf.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame  $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib" -font $gedFont
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x -pady 0 -padx $smallPad


#figure label
frame $w.frame.lbfig -borderwidth 0
pack $w.frame.lbfig  -in $w.frame -side top   -fill x

label $w.frame.figlabel -text " Figure name:" -font $gedFont -anchor e -width $largeur
entry $w.frame.figlabel1 -relief sunken  -textvariable figure_name -font $gedFont -width 15
pack $w.frame.figlabel -in  $w.frame.lbfig -side left
pack $w.frame.figlabel1  -in  $w.frame.lbfig  -expand 1 -fill x -pady 0 -padx $mediumPad
bind  $w.frame.figlabel1 <Return> {setFiglabel}
bind  $w.frame.figlabel1 <KP_Enter> {setFiglabel}
bind  $w.frame.figlabel1 <FocusOut> {setFiglabel}

#figure id
#label tout court non modifiable
frame $w.frame.lbfigid -borderwidth 0
pack $w.frame.lbfigid  -in $w.frame -side top   -fill x

label $w.frame.figidlabel -text "Figure id:" -font $gedFont -anchor e -width $largeur
label $w.frame.figidlabel1 -textvariable figure_id -font $gedFont -anchor e
pack $w.frame.figidlabel  -in  $w.frame.lbfigid -side left
pack $w.frame.figidlabel1 -in  $w.frame.lbfigid -side left -pady 0 -padx $smallPad

#figure position x
frame $w.frame.xfigpos -borderwidth 0
pack $w.frame.xfigpos  -in $w.frame -side top   -fill x

label $w.frame.xfigposlabel -text " X position:" -font $gedFont -anchor e -width $largeur
entry $w.frame.xfigposlabel1 -relief sunken  -textvariable figure_xposition -font $gedFont -width 10
pack $w.frame.xfigposlabel -in  $w.frame.xfigpos -side left
pack $w.frame.xfigposlabel1  -in  $w.frame.xfigpos   -pady 0 -padx $mediumPad -side left
bind  $w.frame.xfigposlabel1 <Return> {setFigPos}
bind  $w.frame.xfigposlabel1 <KP_Enter> {setFigPos}
bind  $w.frame.xfigposlabel1 <FocusOut> {setFigPos}

#figure position y
frame $w.frame.yfigpos -borderwidth 0
pack $w.frame.yfigpos  -in $w.frame -side top   -fill x

label $w.frame.yfigposlabel -text "Y position:" -font $gedFont -anchor e -width $largeur
entry $w.frame.yfigposlabel1 -relief sunken  -textvariable figure_yposition -font $gedFont -width 10
pack $w.frame.yfigposlabel -in  $w.frame.yfigpos -side left
pack $w.frame.yfigposlabel1  -in  $w.frame.yfigpos  -pady 0 -padx $mediumPad -side left
bind  $w.frame.yfigposlabel1 <Return> {setFigPos}
bind  $w.frame.yfigposlabel1 <KP_Enter> {setFigPos}
bind  $w.frame.yfigposlabel1 <FocusOut> {setFigPos}

#figure size x
frame $w.frame.xfigsiz -borderwidth 0
pack $w.frame.xfigsiz  -in $w.frame -side top   -fill x

label $w.frame.xfigsizlabel -text "X size:" -font $gedFont -anchor e -width $largeur
entry $w.frame.xfigsizlabel1 -relief sunken  -textvariable figure_xsiz -font $gedFont -width 10
pack $w.frame.xfigsizlabel -in  $w.frame.xfigsiz -side left
pack $w.frame.xfigsizlabel1  -in  $w.frame.xfigsiz  -pady 0 -padx $mediumPad -side left
bind  $w.frame.xfigsizlabel1 <Return> {setFigSiz}
bind  $w.frame.xfigsizlabel1 <KP_Enter> {setFigSiz}
bind  $w.frame.xfigsizlabel1 <FocusOut> {setFigSiz}

#figure size y
frame $w.frame.yfigsiz -borderwidth 0
pack $w.frame.yfigsiz  -in $w.frame -side top   -fill x

label $w.frame.yfigsizlabel -text "Y size:" -font $gedFont -anchor e -width $largeur
entry $w.frame.yfigsizlabel1 -relief sunken  -textvariable figure_ysiz -font $gedFont -width 10
pack $w.frame.yfigsizlabel -in  $w.frame.yfigsiz -side left
pack $w.frame.yfigsizlabel1  -in  $w.frame.yfigsiz  -pady 0 -padx $mediumPad -side left
bind  $w.frame.yfigsizlabel1 <Return> {setFigSiz}
bind  $w.frame.yfigsizlabel1 <KP_Enter> {setFigSiz}
bind  $w.frame.yfigsizlabel1 <FocusOut> {setFigSiz}

#axes size x
frame $w.frame.xaxesiz -borderwidth 0
pack $w.frame.xaxesiz  -in $w.frame -side top   -fill x

label $w.frame.xaxesizlabel -text "X axis size:" -font $gedFont -anchor e -width $largeur
entry $w.frame.xaxesizlabel1 -relief sunken  -textvariable figure_xaxesiz -font $gedFont -width 10
pack $w.frame.xaxesizlabel -in  $w.frame.xaxesiz -side left
pack $w.frame.xaxesizlabel1  -in  $w.frame.xaxesiz  -pady 0 -padx $mediumPad -side left
bind  $w.frame.xaxesizlabel1 <Return> {setAxeSiz}
bind  $w.frame.xaxesizlabel1 <KP_Enter> {setAxeSiz}
bind  $w.frame.xaxesizlabel1 <FocusOut> {setAxeSiz}

#axes size y
frame $w.frame.yaxesiz -borderwidth 0
pack $w.frame.yaxesiz  -in $w.frame -side top   -fill x

label $w.frame.yaxesizlabel -text "Y axis size:" -font $gedFont -anchor e -width $largeur
entry $w.frame.yaxesizlabel1 -relief sunken  -textvariable figure_yaxesiz -font $gedFont -width 10
pack $w.frame.yaxesizlabel -in  $w.frame.yaxesiz -side left
pack $w.frame.yaxesizlabel1  -in  $w.frame.yaxesiz  -pady 0 -padx $mediumPad -side left
bind  $w.frame.yaxesizlabel1 <Return> {setAxeSiz}
bind  $w.frame.yaxesizlabel1 <KP_Enter> {setAxeSiz}
bind  $w.frame.yaxesizlabel1 <FocusOut> {setAxeSiz}

#Color scale background
frame $w.frame.clrb  -borderwidth 0
pack $w.frame.clrb  -in $w.frame -side top  -fill x
label $w.frame.bcolorlabel -height 0 -text "Back. color:" -width 0  -font $gedFont -anchor e -width $largeur

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackColor $w.frame.bcolor" -tickinterval 0 -font $gedFont

pack $w.frame.bcolorlabel -in $w.frame.clrb -side left
pack $w.frame.bcolor -in  $w.frame.clrb -side left  -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.bcolor set $bcolor

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.buttons.dismiss  -side top


########### Mode onglet ###########################################
###################################################################
set w [Notebook:frame  $uf.n Mode]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#update largeur
set largeur 18

#auto resize
frame $w.frame.autorsz -borderwidth 0
pack $w.frame.autorsz  -in $w.frame  -side top -fill x
label $w.frame.autorszlabel  -text "Auto resize:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.autorszib  -text "on"\
    -variable curautoresize -onvalue "on" -offvalue "off" \
    -command "toggleResize $w.frame.autorszib" -font $gedFont
OnOffForeground $w.frame.autorszib $curautoresize

pack $w.frame.autorszlabel -in $w.frame.autorsz  -side left
pack $w.frame.autorszib  -in $w.frame.autorsz    -side left -fill x -pady 0 -padx $smallPad


#pixel drawing mode
frame $w.frame.pixel  -borderwidth 0
pack $w.frame.pixel  -in $w.frame -side top -fill x

label $w.frame.pixdmodelbl  -height 0 -text "Pixel drawing mode:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.pixdmode \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curpdm \
    -editable false \
    -command [list SelectPixelDrawingMode] -font $gedFont
eval $w.frame.pixdmode list insert end [list "clear" "and" "andReverse"  "copy" "andInverted"  "noop"  "xor"  "or"  "nor"  "equiv"  "invert"  "orReverse"  "copyInverted"  "orInverted"  "nand"  "set" ]

pack $w.frame.pixdmodelbl -in $w.frame.pixel   -side left
pack $w.frame.pixdmode -in $w.frame.pixel  -expand 1 -fill x -pady 0 -padx $mediumPad


#rotation style
frame $w.frame.rotstyle  -borderwidth 0
pack $w.frame.rotstyle  -in $w.frame -side top -fill x

label $w.frame.rotstylelbl  -height 0 -text "Rotation style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.rotstyl \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable currotation_style \
    -editable false \
    -command [list SelectRotationStyle] -font $gedFont
eval $w.frame.rotstyl list insert end [list "unary" "multiple"]

pack $w.frame.rotstylelbl -in $w.frame.rotstyle   -side left
pack $w.frame.rotstyl -in $w.frame.rotstyle  -expand 1 -fill x -pady 0 -padx $mediumPad

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.buttons.dismiss  -side bottom


########### Colormap onglet #######################################
###################################################################
set w [Notebook:frame  $uf.n Colormap]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


frame $w.frame.fdata -borderwidth 0
pack $w.frame.fdata  -in $w.frame -side top   -fill x

scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
#canvas $w.frame.c -width 8i -height 3i  -yscrollcommand {$w.frame.ysbar set}
canvas $w.frame.c -width 310 -height 265 -yscrollcommand {$w.frame.ysbar set}

$w.frame.c create text 70 10 -anchor c -text "RED" -font $gedFont
$w.frame.c create text 150 10 -anchor c -text "GREEN" -font $gedFont
$w.frame.c create text 230 10 -anchor c -text "BLUE" -font $gedFont

for {set i 1} {$i<=$ncolors} {incr i} {
    set bb [expr 10+(25*$i)]
    $w.frame.c create text 10 $bb -anchor c -text $i -font $gedFont
#RED
    set aa 70
    entry  $w.frame.c.reddata$i  -relief sunken  -textvariable RED($i) -width 10 -font $gedFont
    bind  $w.frame.c.reddata$i <Return> "setRedColor $w $i "
    bind  $w.frame.c.reddata$i <KP_Enter> "setRedColor $w $i "
    bind  $w.frame.c.reddata$i <FocusOut> "setRedColor $w $i "
#location help balloon
    set_balloon $w.frame.c.reddata$i "Row: $i Column: 1"
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.reddata$i

#GREEN
    set aa 150
    entry  $w.frame.c.gredata$i  -relief sunken  -textvariable GREEN($i) -width 10 -font $gedFont
    bind  $w.frame.c.gredata$i <Return> "setGreenColor $w $i "
    bind  $w.frame.c.gredata$i <KP_Enter> "setGreenColor $w $i "
    bind  $w.frame.c.gredata$i <FocusOut> "setGreenColor $w $i "
#location help balloon
#location help balloon
    set_balloon $w.frame.c.gredata$i "Row: $i Column: 2"
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.gredata$i

#BLUE
    set aa 230
    entry  $w.frame.c.bludata$i  -relief sunken  -textvariable BLUE($i) -width 10 -font $gedFont
    bind  $w.frame.c.bludata$i <Return> "setBlueColor  $w $i "
    bind  $w.frame.c.bludata$i <KP_Enter> "setBlueColor  $w $i "
    bind  $w.frame.c.bludata$i <FocusOut> "setBlueColor  $w $i "
#location help balloon
    set_balloon $w.frame.c.bludata$i "Row: $i Column: 3"
    $w.frame.c create window $aa $bb -anchor c -window $w.frame.c.bludata$i

#sample
    set REDC $RED($i)
    set GREC $GREEN($i)
    set BLUC $BLUE($i)

    set color [format \#%02x%02x%02x [expr int($REDC*255)]  [expr int($GREC*255)]  [expr int($BLUC*255)]]
    set aa 280
    $w.frame.c create rectangle $aa [expr $bb-10] [expr $aa+20] [expr $bb+10] -fill $color

}

$w.frame.c configure -scrollregion [$w.frame.c bbox all] -yscrollincrement 0.1i

pack  $w.frame.ysbar -side right -fill y
pack  $w.frame.c


frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 0

label $w.scicom1.label1 -text "Scilab Command Interface for colormap:" -font $gedFont
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicom
pack $w.scicom -side top -fill x -pady 0


label $w.scicom.label1 -text "Colormap (Nx3 double array) = " -font $gedFont
pack  $w.scicom.label1 -in $w.scicom -side left

#text $w.scicom.text1 -height 3m -width 50 -relief sunken -yscrollcommand "$w.scicom.scroll set"
entry $w.scicom.text1 -relief sunken -textvariable scicomint_colormap -width 10 -font $gedFont
set_balloon $w.scicom.text1 "Enter a graycolormap(COLOR_NUMBER), hotcolormap(COLOR_NUMBER)\n or jetcolormap(COLOR_NUMBER) call to initialize the \"colormap\" field."
bind  $w.scicom.text1 <Return> "sciCommandColormap"
bind  $w.scicom.text1 <KP_Enter> "sciCommandColormap"

pack $w.scicom.text1  -side left -fill both -expand yes

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
#pack $w.buttons -side bottom -fill x -pady $mediumPad
button $w.dismiss -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.dismiss  -side bottom


pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes



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
	ScilabEval "global ged_handle; if ged_handle.background <> $index then ged_handle.background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.background <> $index then ged_handle.background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.background <> $index then ged_handle.background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1)
	set GRECOL $GREEN(1)
	set BLUCOL $BLUE(1)

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	$w config  -activebackground $color -troughcolor $color
    } else {
	ScilabEval "global ged_handle; if ged_handle.background <> $index then ged_handle.background=$index; end;"

	set REDCOL $RED($index)
	set GRECOL $GREEN($index)
	set BLUCOL $BLUE($index)

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color

    }
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


proc SavePreferences { } {
    global ged_listofpref
    global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
    global ww msdos

    ScilabEval "DestroyGlobals()" "seq"

    if { $msdos == "F" } {
#unix mandrake (at least) needs this offset
#test other unix distribution to see (red hat, suse...)
	set xoffset -5
	set yoffset -26
    } else {
	set xoffset 0
	set yoffset 0
    }

    set x [eval {winfo x $ww}]
    set y [eval {winfo y $ww}]
    set MAIN_WINDOW_POSITION "+[expr $x+$xoffset]+[expr $y+$yoffset]"

    #save preferences (position...)
    set preffilename [file join $envSCIHOME .GedPreferences.tcl]
    catch {
 	set preffile [open $preffilename w]
	foreach opt $ged_listofpref {
	    global $opt
	    puts $preffile [concat "set $opt " [set $opt]]
	    # 	    puts [concat "set $opt" [set $opt]]
	}
	close $preffile
    }
}

proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"

    SavePreferences
}
