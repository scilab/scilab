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



variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/modules/tclsci/tcl/BWidget"]
namespace inscope :: package require BWidget
package require BWidget


set sourcedir [file join "$env(SCIPATH)" "modules" "tclsci" "tcl" "utils"]
set sourcedir2 [file join "$env(SCIPATH)" "modules" "graphics" "tcl" "ged"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
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
global ncolors RED GREEN BLUE

global curvis
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox
global curlinestyle arcVAL nbcol curcolor
global curfillmode curthick curback
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox ncolors

global Hval Wval Xval Yval Zval A1val A2val
global curlinemode curfillmode
global smallPad mediumPad

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 270
set NBwidth  288

set smallPad  4
set mediumPad 8

set Wheight [expr $NBheight + 110]
set Wwidth  [expr $NBwidth  + 270]

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Arc Object"
wm iconname $ww "AO"
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

Notebook:create $uf.n -pages {Style Data Clipping} -pad 0  -height  $NBheight -width $NBwidth
pack $uf.n -in $uf -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

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
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x -padx $smallPad

#Line mode
frame $w.frame.linelinemode  -borderwidth 0
pack $w.frame.linelinemode  -in $w.frame  -side top  -fill x

label $w.frame.linemodelabel -height 0 -text "Line mode:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.linemode  -text "on" -indicatoron 1 \
    -variable curlinemode -onvalue "on" -offvalue "off" \
    -command "toggleLinemode $w.frame.linemode" -font $gedFont
OnOffForeground $w.frame.linemode $curlinemode

pack $w.frame.linemodelabel  -in $w.frame.linelinemode  -side left 
pack $w.frame.linemode   -in $w.frame.linelinemode   -side left  -fill x -pady 0 -padx $smallPad

#Fill mode
frame $w.frame.linefillmode  -borderwidth 0
pack $w.frame.linefillmode  -in $w.frame  -side top  -fill x

label $w.frame.fillmodelabel -height 0 -text "Fill mode:" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.fillmode  -text "on" -indicatoron 1 \
    -variable curfillmode -onvalue "on" -offvalue "off" \
    -command "toggleFill $w.frame.fillmode"  -font $gedFont
OnOffForeground $w.frame.fillmode $curfillmode

pack $w.frame.fillmodelabel  -in $w.frame.linefillmode  -side left 
pack $w.frame.fillmode   -in $w.frame.linefillmode   -side left  -fill x -pady 0 -padx $smallPad

#Line Style
frame $w.frame.rectst  -borderwidth 0
pack $w.frame.rectst  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Line:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 14 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ] -font $gedFont
eval $w.frame.style list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"]

#Add thickness here
combobox $w.frame.thickness \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curthick \
    -editable true \
    -command [list SelectThickness ] -font $gedFont
eval $w.frame.thickness list insert end [list "0.0" "1.0" "2.0" "3.0" "4.0" "5.0" "6.0" "8.0" "10.0"]

pack $w.frame.stylelabel -in $w.frame.rectst   -side left
pack $w.frame.style   -in $w.frame.rectst   -side left -padx $mediumPad
pack $w.frame.thickness  -in $w.frame.rectst  -expand 1 -fill x -pady 0 -padx 0

#Color scale
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Foreground:" -width 0  -font $gedFont -anchor e -width $largeur
#         -foreground $color
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0  -font $gedFont

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color  -in  $w.frame.clrf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $curcolor


#Background scale (line)
frame $w.frame.backg  -borderwidth 0
pack $w.frame.backg  -in $w.frame -side top  -fill x

label $w.frame.backlabel -height 0 -text "Background:" -font $gedFont -anchor e -width $largeur
#         -foreground $back
scale $w.frame.back -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBack $w.frame.back" -tickinterval 0  -font $gedFont

pack $w.frame.backlabel -in $w.frame.backg -side left
pack $w.frame.back  -in  $w.frame.backg -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.back set $curback


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
button $w.buttons -text Quit -command "destroy $ww"  -font $gedFont
pack $w.buttons  -side bottom



########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n Data]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#x 
frame $w.frame.lb1 -borderwidth 0
pack $w.frame.lb1  -in $w.frame -side top   -fill x
label $w.frame.labelul -text "Upper-left point coordinates:" -font $gedFont
pack $w.frame.labelul -in  $w.frame.lb1 -side left

frame $w.frame.lb2 -borderwidth 0
pack $w.frame.lb2  -in $w.frame -side top   -fill x

frame $w.frame.lb21 -borderwidth 0
pack $w.frame.lb21  -in $w.frame -side top   -fill x

frame $w.frame.lb22 -borderwidth 0
pack $w.frame.lb22  -in $w.frame -side top   -fill x

label $w.frame.labelx -text "X:" -font $gedFont -anchor e -width $largeur
entry $w.frame.datax -relief sunken  -textvariable Xval -width 10 -font $gedFont
label $w.frame.labely -text "Y:" -font $gedFont -anchor e -width $largeur
entry $w.frame.datay -relief sunken  -textvariable Yval -width 10 -font $gedFont
label $w.frame.labelz -text "Z:" -font $gedFont -anchor e -width $largeur
entry $w.frame.dataz -relief sunken  -textvariable Zval -width 10 -font $gedFont

pack $w.frame.labelx  -in  $w.frame.lb2 -side left
pack $w.frame.datax   -in  $w.frame.lb2 -side left  -fill x -pady 0 -padx $mediumPad
pack $w.frame.labely  -in  $w.frame.lb21 -side left
pack $w.frame.datay   -in  $w.frame.lb21 -side left -fill x -pady 0 -padx $mediumPad
pack $w.frame.labelz  -in  $w.frame.lb22 -side left 
pack $w.frame.dataz   -in  $w.frame.lb22 -side left -fill x -pady 0 -padx $mediumPad
bind  $w.frame.datax <Return> {SelectXval} 
bind  $w.frame.datay <Return> {SelectYval} 
bind  $w.frame.dataz <Return> {SelectZval} 
bind  $w.frame.datax <KP_Enter> {SelectXval} 
bind  $w.frame.datay <KP_Enter> {SelectYval} 
bind  $w.frame.dataz <KP_Enter> {SelectZval} 
bind  $w.frame.datax <FocusOut> {SelectXval} 
bind  $w.frame.datay <FocusOut> {SelectYval} 
bind  $w.frame.dataz <FocusOut> {SelectZval} 

#----------------------------#
frame $w.frame.lb3 -borderwidth 0
pack $w.frame.lb3  -in $w.frame -side top   -fill x
label $w.frame.labelwh -text "Width and height:" -font $gedFont
pack $w.frame.labelwh -in  $w.frame.lb3 -side left

frame $w.frame.lb4 -borderwidth 0
pack $w.frame.lb4  -in $w.frame -side top   -fill x

frame $w.frame.lb41 -borderwidth 0
pack $w.frame.lb41  -in $w.frame -side top   -fill x

label $w.frame.labelw -text "W:" -font $gedFont -anchor e -width $largeur
entry $w.frame.dataw -relief sunken  -textvariable Wval  -width 10 -font $gedFont
label $w.frame.labelh -text "H:" -font $gedFont -anchor e -width $largeur
entry $w.frame.datah -relief sunken  -textvariable Hval  -width 10 -font $gedFont

pack $w.frame.labelw  -in  $w.frame.lb4  -side left
pack $w.frame.dataw   -in  $w.frame.lb4  -side left -fill x -pady 0 -padx $mediumPad
pack $w.frame.labelh  -in  $w.frame.lb41 -side left
pack $w.frame.datah   -in  $w.frame.lb41 -side left -fill x -pady 0 -padx $mediumPad
bind  $w.frame.dataw <Return> {SelectWval} 
bind  $w.frame.datah <Return> {SelectHval} 
bind  $w.frame.dataw <KP_Enter> {SelectWval} 
bind  $w.frame.datah <KP_Enter> {SelectHval} 
bind  $w.frame.dataw <FocusOut> {SelectWval} 
bind  $w.frame.datah <FocusOut> {SelectHval} 



#----------------------------#
#a1 and a2 parameters:
frame $w.frame.lb5 -borderwidth 0
pack $w.frame.lb5  -in $w.frame -side top   -fill x
label $w.frame.labela1a2 -text "a1 and a2 values:" -font $gedFont
pack $w.frame.labela1a2 -in  $w.frame.lb5 -side left

frame $w.frame.lb6 -borderwidth 0
pack $w.frame.lb6  -in $w.frame -side top   -fill x

frame $w.frame.lb61 -borderwidth 0
pack $w.frame.lb61  -in $w.frame -side top   -fill x

label $w.frame.labela1 -text "a1:" -font $gedFont -anchor e -width $largeur
entry $w.frame.dataa1 -relief sunken  -textvariable A1val -width 10 -font $gedFont
label $w.frame.labela2 -text "a2:" -font $gedFont -anchor e -width $largeur
entry $w.frame.dataa2 -relief sunken  -textvariable A2val  -width 10 -font $gedFont


pack $w.frame.labela1  -in  $w.frame.lb6  -side left
pack $w.frame.dataa1   -in  $w.frame.lb6  -side left -fill x -pady 0 -padx $mediumPad
pack $w.frame.labela2  -in  $w.frame.lb61 -side left
pack $w.frame.dataa2   -in  $w.frame.lb61 -side left -fill x -pady $smallPad -padx $mediumPad
bind  $w.frame.dataa1 <Return> {SelectA1val} 
bind  $w.frame.dataa2 <Return> {SelectA2val} 
bind  $w.frame.dataa1 <KP_Enter> {SelectA1val} 
bind  $w.frame.dataa2 <KP_Enter> {SelectA2val} 
bind  $w.frame.dataa1 <FocusOut> {SelectA1val} 
bind  $w.frame.dataa2 <FocusOut> {SelectA2val} 



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww" -font $gedFont
pack $w.b -side bottom 


########### Clipping onglet #######################################
###################################################################

set w9 [Notebook:frame $uf.n Clipping]

frame $w9.frame -borderwidth 0
pack $w9.frame -anchor w -fill both

set letext ""

#Clip state
#frame $w9.frame.clpwarning  -borderwidth 0

frame $w9.frame.clpstat  -borderwidth 0
pack $w9.frame.clpstat  -in $w9.frame -side top -fill x -pady 0

label $w9.frame.cliplabel  -height 0 -text "Clip state:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w9.frame.clip \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 8 \
    -textvariable curclipstate\
    -editable false \
    -command [list SelectClipState ] -font $gedFont
eval $w9.frame.clip list insert end [list "on" "off" "clipgrf"]

pack $w9.frame.cliplabel -in $w9.frame.clpstat   -side left
pack $w9.frame.clip -in $w9.frame.clpstat -side left -pady 0 -padx $mediumPad

#clip box
frame $w9.frame.lb1 -borderwidth 0
pack $w9.frame.lb1  -in $w9.frame -side top   -fill x
label $w9.frame.labelul -text "Clip box : upper-left point coordinates" -font $gedFont
pack $w9.frame.labelul -in  $w9.frame.lb1 -side left

frame $w9.frame.lb2 -borderwidth 0
pack $w9.frame.lb2  -in $w9.frame -side top   -fill x

frame $w9.frame.lb21 -borderwidth 0
pack $w9.frame.lb21  -in $w9.frame -side top   -fill x

frame $w9.frame.lb22 -borderwidth 0
pack $w9.frame.lb22  -in $w9.frame -side top   -fill x

label $w9.frame.labelx -text "X:" -font $gedFont  -anchor e -width $largeur
entry $w9.frame.datax -relief sunken  -textvariable Xclipbox -width 10  -font $gedFont
label $w9.frame.labely -text "Y:" -font $gedFont -anchor e -width $largeur
entry $w9.frame.datay -relief sunken  -textvariable Yclipbox -width 10  -font $gedFont

pack $w9.frame.labelx  -in  $w9.frame.lb2 -side left 
pack $w9.frame.datax   -in  $w9.frame.lb2 -side left -pady 0 -padx $mediumPad
pack $w9.frame.labely  -in  $w9.frame.lb21 -side left 
pack $w9.frame.datay   -in  $w9.frame.lb21 -side left -pady 0 -padx $mediumPad
bind  $w9.frame.datax <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datax <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <KP_Enter> "SelectClipBox $w9.frame"

#----------------------------#
frame $w9.frame.lb3 -borderwidth 0
pack $w9.frame.lb3  -in $w9.frame -side top   -fill x
label $w9.frame.labelwh -text "Clip box : width and height" -font $gedFont
pack $w9.frame.labelwh -in  $w9.frame.lb3 -side left

frame $w9.frame.lb4 -borderwidth 0
pack $w9.frame.lb4  -in $w9.frame -side top   -fill x

frame $w9.frame.lb41 -borderwidth 0
pack $w9.frame.lb41  -in $w9.frame -side top   -fill x

label $w9.frame.labelw -text "W:" -font $gedFont -anchor e -width $largeur
entry $w9.frame.dataw -relief sunken  -textvariable Wclipbox -width 10  -font $gedFont
label $w9.frame.labelh -text "H:" -font $gedFont -anchor e -width $largeur
entry $w9.frame.datah -relief sunken  -textvariable Hclipbox -width 10  -font $gedFont

pack $w9.frame.labelw  -in  $w9.frame.lb4 -side left 
pack $w9.frame.dataw   -in  $w9.frame.lb4 -side left -pady 0 -padx $mediumPad
pack $w9.frame.labelh  -in  $w9.frame.lb41 -side left 
pack $w9.frame.datah   -in  $w9.frame.lb41 -side left -pady 0 -padx $mediumPad
bind  $w9.frame.dataw <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.dataw <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <KP_Enter> "SelectClipBox $w9.frame"


frame $w9.frame.warning
label $w9.frame.mesgwarning  -justify left -textvariable letext -font $gedFont
$w9.frame.mesgwarning config -foreground red
pack $w9.frame.mesgwarning -in $w9.frame.warning
pack $w9.frame.warning -in $w9.frame

#sep bar
frame $w9.sep -height 2 -borderwidth 1 -relief sunken
pack $w9.sep -fill both


#exit button
frame $w9.buttons
button $w9.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w9.b -side bottom



pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}

proc toggleLinemode { frame } {
    global curlinemode
    ScilabEval "global ged_handle;ged_handle.line_mode='$curlinemode'"

    OnOffForeground $frame $curlinemode
}

proc toggleFillmode { frame } {
    global curfillmode
    ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"

    OnOffForeground $frame $curfillmode
}

proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
}


proc SelectThickness {w args} {
    global curthick
    ScilabEval "global ged_handle;ged_handle.thickness=$curthick;"
}


proc setColor {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}


proc setBack {w index} {   
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



proc toggleFill { frame } {
    global curfillmode
    ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"

    OnOffForeground $frame $curfillmode
}


proc setThickness {w thick} {
ScilabEval "global ged_handle;if ged_handle.thickness <> $thick then ged_handle.thickness=$thick; end;"
}




proc SelectXval {} {
global Xval
ScilabEval "setXval($Xval)"
}


proc SelectYval {} {
global Yval
ScilabEval "setYval($Yval)"
}


proc SelectZval {} {
global Zval
ScilabEval "setZval($Zval)"
}



proc SelectWval {} {
global Wval
ScilabEval "setWval($Wval)"
}



proc SelectHval {} {
global Hval
ScilabEval "setHval($Hval)"
}



proc SelectA1val {} {
global A1val
ScilabEval "setA1val($A1val)"
}



proc SelectA2val {} {
global A2val
ScilabEval "setA2val($A2val)"
}







#Clipping proc for all entities (clip box and clip state fields)
proc SelectClipBox { w } {
    global Xclipbox Yclipbox Wclipbox Hclipbox curclipstate letext
    if { ($Xclipbox == "") || ($Yclipbox == "") ||
	 ($Wclipbox == "") || ($Hclipbox == "") } {
	if { [info exists  text] } {
	    unset text
	}
	set letext "Note that all clip box fields must be filled."
	
    } else {
	if { [info exists  text] } {
	    unset text
	}
	set letext ""
	
	ScilabEval "global ged_handle;ged_handle.clip_box=\[$Xclipbox $Yclipbox $Wclipbox $Hclipbox\]"
	set curclipstate "on"
    }
}

proc SelectClipState {w args} {
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox
ScilabEval "global ged_handle;ged_handle.clip_state='$curclipstate';"
    if { $curclipstate == "off" } {
	set old_Xclipbox $Xclipbox
	set old_Yclipbox $Yclipbox
	set old_Wclipbox $Wclipbox
	set old_Hclipbox $Hclipbox
	set Xclipbox ""
	set Yclipbox ""
	set Wclipbox ""
	set Hclipbox ""
    } else {
	set Xclipbox $old_Xclipbox
	set Yclipbox $old_Yclipbox
	set Wclipbox $old_Wclipbox
	set Hclipbox $old_Hclipbox
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

