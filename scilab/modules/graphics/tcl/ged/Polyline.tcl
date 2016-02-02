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


global curvis curthick curpolylinestyle curlinestyle RED GREEN BLUE
global curmarkmode curlinemode curclosedmode curmarksize curmarksizeunit curmarkforeground curmarkbackground
global curfillmode curinterpcolormode curinterpcolorvector curarrowsizefactor
#global polyVAL nbcol nbrow

#puts "curmarkmode = $curmarkmode"

global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox ncolors
global curdata curcolor curback

# set ncolors 32
# set nbcol 2
# set nbrow 6
# set curcolor 1
# set curthick 2

# set polyVAL(1,1) 1
# set polyVAL(1,2) 2
# set polyVAL(1,3) 3
# set polyVAL(1,4) 4
# set polyVAL(1,5) 5
# set polyVAL(1,6) 6


# set polyVAL(2,1) 1.3
# set polyVAL(2,2) 2.6
# set polyVAL(2,3) 3.5
# set polyVAL(2,4) 4.5
# set polyVAL(2,5) 5.8
# set polyVAL(2,6) 6.8

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 390
set NBwidth  287

set Wheight [expr $NBheight + 160]
set Wwidth  [expr $NBwidth  + 275]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Polyline Editor"
wm iconname $ww "PE"
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
	       -width 20 -height 30 \
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

Notebook:create $uf.n -pages {Style Data Clipping} -pad 0 -height $NBheight -width $NBwidth
pack $uf.n -fill both -expand yes

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#Visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x

label $w.frame.visiblelabel -height 0 -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.visible  -text "on" -indicatoron 1 \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility $w.frame.visible" -font $gedFont
OnOffForeground $w.frame.visible $curvis

pack $w.frame.visiblelabel  -in $w.frame.vis  -side left
pack $w.frame.visible  -in $w.frame.vis  -side left  -fill x -pady 0 -padx $smallPad

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

#Closed
label $w.frame.closedmodelabel -height 0 -text "Closed:" -font $gedFont -anchor e -width 10
checkbutton $w.frame.closedmode  -text "on" -indicatoron 1 \
    -variable curclosedmode -onvalue "on" -offvalue "off" \
    -command "toggleClosedmode $w.frame.closedmode" -font $gedFont -anchor w
OnOffForeground $w.frame.closedmode $curclosedmode

pack $w.frame.closedmodelabel  -in $w.frame.linelinemode  -side left 
pack $w.frame.closedmode   -in $w.frame.linelinemode  -side left  -pady 0

#Fill mode
frame $w.frame.fillfillmode  -borderwidth 0
pack $w.frame.fillfillmode  -in $w.frame  -side top  -fill x

label $w.frame.fillmodelabel -height 0 -text "Fill mode:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.fillmode  -text "on" -indicatoron 1 \
    -variable curfillmode -onvalue "on" -offvalue "off" \
    -command "toggleFillmode $w.frame.fillmode" -font $gedFont
OnOffForeground $w.frame.fillmode $curfillmode

pack $w.frame.fillmodelabel  -in $w.frame.fillfillmode  -side left 
pack $w.frame.fillmode   -in $w.frame.fillfillmode   -side left  -fill x -pady 0 -padx $smallPad

#Interp. color mode
label $w.frame.interpcolormodelabel -height 0 -text "Interp. mode:" -font $gedFont -anchor w -width 10
checkbutton $w.frame.interpcolormode  -text "on" -indicatoron 1 \
    -variable curinterpcolormode -onvalue "on" -offvalue "off" \
    -command "toggleInterpColormode $w.frame.interpcolormode" -font $gedFont -anchor w
OnOffForeground $w.frame.interpcolormode $curinterpcolormode

pack $w.frame.interpcolormodelabel  -in $w.frame.fillfillmode  -side left 
pack $w.frame.interpcolormode   -in $w.frame.fillfillmode   -side left  -pady 0

#Polyline Style
frame $w.frame.curvst  -borderwidth 0
pack $w.frame.curvst  -in $w.frame  -side top  -fill x

label $w.frame.polystylelabel  -height 0 -text "Polyline style:"  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.polystyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curpolylinestyle \
    -editable false \
    -command [list SelectPolylineStyle ] -font $gedFont
eval $w.frame.polystyle list insert end [list "interpolated" "staircase" "barplot" "arrowed" "filled" "bar"]
pack $w.frame.polystylelabel -in $w.frame.curvst   -side left
pack $w.frame.polystyle   -in $w.frame.curvst   -expand 1 -fill x -pady 0 -padx $mediumPad

#Line Style
frame $w.frame.linest  -borderwidth 0
pack $w.frame.linest  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Line:"  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 14 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ] -font $gedFont
eval $w.frame.style list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"]

#Arrow size factor
frame $w.frame.arrowsize  -borderwidth 0
pack $w.frame.arrowsize  -in $w.frame -side top  -fill x

label $w.frame.arrowsizelabel -height 0 -text "Arrow size:" -font $gedFont -anchor e -width $largeur

entry $w.frame.arrowsize1 -relief sunken  -textvariable curarrowsizefactor -font $gedFont -width 6
pack $w.frame.arrowsizelabel -in  $w.frame.arrowsize -side left
pack $w.frame.arrowsize1  -in  $w.frame.arrowsize  -side left -pady 0 -padx $mediumPad
bind  $w.frame.arrowsize1 <Return> {setArrowSize} 
bind  $w.frame.arrowsize1 <KP_Enter> {setArrowSize} 
bind  $w.frame.arrowsize1 <FocusOut> {setArrowSize} 

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


pack $w.frame.stylelabel -in $w.frame.linest   -side left
pack $w.frame.style   -in $w.frame.linest   -side left -padx {8 4}
pack $w.frame.thickness  -in $w.frame.linest  -expand 1 -fill x -pady 0 -padx {0 4}

#Color scale (line)
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Foreground:" -font $gedFont -anchor e -width $largeur
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

#Interp. vector
frame $w.frame.interpvec  -borderwidth 0
pack $w.frame.interpvec  -in $w.frame -side top  -fill x

label $w.frame.interpveclabel -height 0 -text "Interp. vector:" -font $gedFont -anchor e -width $largeur

entry $w.frame.interpvec1 -relief sunken  -textvariable curinterpcolorvector -font $gedFont -width 15
pack $w.frame.interpveclabel -in  $w.frame.interpvec -side left
pack $w.frame.interpvec1  -in  $w.frame.interpvec  -side left -pady 0 -padx $mediumPad
bind  $w.frame.interpvec1 <Return> {setInterpColor} 
bind  $w.frame.interpvec1 <KP_Enter> {setInterpColor} 
bind  $w.frame.interpvec1 <FocusOut> {setInterpColor} 

#Mark mode
frame $w.frame.linemarkmode  -borderwidth 0
pack $w.frame.linemarkmode  -in $w.frame  -side top  -fill x

label $w.frame.markmodelabel -height 0 -text "Mark mode:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.markmode  -text "on" -indicatoron 1 \
    -variable curmarkmode -onvalue "on" -offvalue "off" \
    -command "toggleMarkmode $w.frame.markmode" -font $gedFont
OnOffForeground $w.frame.markmode $curmarkmode

pack $w.frame.markmodelabel  -in $w.frame.linemarkmode  -side left 
pack $w.frame.markmode   -in $w.frame.linemarkmode   -side left  -fill x -pady 0 -padx $smallPad


#Mark style
frame $w.frame.linemarkst  -borderwidth 0
pack $w.frame.linemarkst  -in $w.frame  -side top  -fill x

label $w.frame.markstylelabel  -height 0 -text "Mark style:"  -font $gedFont -anchor e -width $largeur
combobox $w.frame.markstyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curmarkstyle \
    -editable false \
    -command [list SelectMarkStyle ] -font $gedFont
eval $w.frame.markstyle list insert end [list "dot" "plus" "cross" "star" "filled diamond" "diamond" "triangle up" "triangle down" "diamond plus" "circle" "asterisk" "square" "triangle right" "triangle left" "pentagram"]

pack $w.frame.markstylelabel  -in $w.frame.linemarkst   -side left
pack $w.frame.markstyle   -in $w.frame.linemarkst   -expand 1 -fill x -pady 0 -padx $mediumPad


#Mark size
frame $w.frame.mksize  -borderwidth 0
pack $w.frame.mksize  -side top -fill x

label $w.frame.marksizelabel -height 0 -text "Mark size:"  -font $gedFont -anchor e -width $largeur
combobox $w.frame.marksize \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curmarksize \
    -editable true \
    -command [list SelectMarkSize ] -font $gedFont
eval $w.frame.marksize list insert end [list "0.5" "1.0" "2.0" "3.0" "4.0" "6.0" "8.0" "10.0" "15.0" "20.0" "25.0" "30.0"]


#Add Mark size unit
label $w.frame.marksizeunitlabel  -height 0 -text "Mark size:"  -font $gedFont -anchor e -width $largeur
combobox $w.frame.marksizeunit \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 8 \
    -textvariable curmarksizeunit \
    -editable false \
    -command [list SelectMarkSizeUnit ] -font $gedFont
eval $w.frame.marksizeunit list insert end [list "point" "tabulated"]

pack  $w.frame.marksizeunitlabel -in $w.frame.mksize -side left
pack  $w.frame.marksizeunit -in $w.frame.mksize -side left -padx $mediumPad
pack  $w.frame.marksize -in $w.frame.mksize -side left  -fill x  -expand 1 -pady 0 -padx $mediumPad


#Mark foreground
frame $w.frame.markf  -borderwidth 0
pack $w.frame.markf  -in $w.frame -side top  -fill x

label $w.frame.markflabel -height 0 -text "Mark foreground:"  -font $gedFont -anchor e -width $largeur
scale $w.frame.markforeground -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setMarkForeground $w.frame.markforeground" -tickinterval 0  -font $gedFont

pack $w.frame.markflabel -in $w.frame.markf -side left
pack $w.frame.markforeground  -in  $w.frame.markf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.markforeground set $curmarkforeground


#Mark background
frame $w.frame.markb  -borderwidth 0
pack $w.frame.markb  -in $w.frame -side top  -fill x

label $w.frame.markblabel -height 0 -text "Mark background:" -font $gedFont -anchor e -width $largeur
scale $w.frame.markbackground -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setMarkBackground $w.frame.markbackground" -tickinterval 0  -font $gedFont

pack $w.frame.markblabel -in $w.frame.markb -side left
pack $w.frame.markbackground  -in  $w.frame.markb -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.markbackground set $curmarkbackground

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w.b -side bottom



########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n Data]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

set mycurdata $curdata

frame $w.frame.curdataframe  -borderwidth 0
pack $w.frame.curdataframe  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabel  -height 0 -text "Data field:" -width 0  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.polydata \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata \
    -editable true \
    -command [list SelectData ] -font $gedFont
eval $w.frame.polydata list insert end [list $mycurdata "----" "Edit data..."]
pack $w.frame.polydatalabel -in $w.frame.curdataframe  -side left
pack $w.frame.polydata   -in $w.frame.curdataframe  -expand 1 -fill x -pady 0 -padx $mediumPad



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
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
pack $w9.frame.clip -in $w9.frame.clpstat  -side left -pady 0 -padx $mediumPad

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

proc setData { i j } {
global polyVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i,$j)=$polyVAL($i,$j);\",\'errcatch\',\'n\');"
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

proc setInterpColor {} {
global curinterpcolorvector
ScilabEval "global ged_handle;ged_handle.interp_color_vector=$curinterpcolorvector"
}

proc setArrowSize {} {
global curarrowsizefactor
ScilabEval "global ged_handle;ged_handle.arrow_size_factor=$curarrowsizefactor"
}


proc setThickness {w thick} {
ScilabEval "global ged_handle;ged_handle.thickness=$thick;"
}

proc toggleVisibility { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}

proc SelectPolylineStyle {w args} {
global curpolylinestyle
ScilabEval "setStyle('$curpolylinestyle')"
}

proc SelectLineStyle {w args} {
global curlinestyle
global curlinemode
ScilabEval "setLineStyle('$curlinestyle')"
set curlinemode "on"
}


proc SelectMarkStyle {w args} {
global curmarkstyle
global curmarkmode
ScilabEval "setMarkStyle('$curmarkstyle')"

set curmarkmode "on"
#ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"

}

proc toggleMarkmode { frame } {
    global curmarkmode
    ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"

    OnOffForeground $frame $curmarkmode
}

#Added on the 21.01.05
proc toggleLinemode { frame } {
    global curlinemode
    ScilabEval "global ged_handle;ged_handle.line_mode='$curlinemode'"

    OnOffForeground $frame $curlinemode
}

#Added on the 15.09.05
proc toggleClosedmode { frame } {
    global curclosedmode
    ScilabEval "global ged_handle;ged_handle.closed='$curclosedmode'"

    OnOffForeground $frame $curclosedmode
}

proc toggleFillmode { frame } {
    global curfillmode
    ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"

    OnOffForeground $frame $curfillmode
}

proc toggleInterpColormode { frame } {
    global curinterpcolormode
    ScilabEval "global ged_handle;ged_handle.interp_color_mode='$curinterpcolormode'"

    OnOffForeground $frame $curinterpcolormode
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


proc sciCommandData {} {
    global scicomint_data

    set longueur [expr [string length $scicomint_data]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a real matrix\n or use a macro call (defining a Nx2 or Nx3 matrix)\n to initialize the \"data\" field."
    } else {
	
	ScilabEval "global ged_handle;ged_handle.data=$scicomint_data;" "seq"
	#Refresh now !
	ScilabEval "tkged();" "seq"
    }
}

proc GUIEditData  {} {
    ScilabEval "global ged_handle;EditData(ged_handle.data,\"ged_handle.data\")" "seq"
}

proc SelectData  {w args} {
    global curdata scicomint_data
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $curdata]]
#    puts "finddbarray = $finddbarray"


    if { ($curdata == "----") || ($finddbarray != -1) } {
#	puts "nothing to do"
    } else {
	if { $curdata ==  "Edit data..." } {
	    GUIEditData
	} else {
	    #enter a variable
	    set scicomint_data $curdata
	    sciCommandData
	}
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


proc setMarkForeground {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setMarkBackground {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc SelectMarkSizeUnit {w args} {
    global curmarksizeunit
    ScilabEval "global ged_handle;ged_handle.mark_size_unit='$curmarksizeunit'"
}



proc SelectMarkSize {w args} {
    global curmarksize
    ScilabEval "global ged_handle;ged_handle.mark_size=$curmarksize"
}


proc SelectThickness {w args} {
    global curthick
    ScilabEval "global ged_handle;ged_handle.thickness=$curthick;"
}
