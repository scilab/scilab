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

global MySciPath
set MySciPath [file join  "$env(SCIPATH)"]


variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/modules/tclsci/tcl/BWidget"]
namespace inscope :: package require BWidget
package require BWidget

global sourcedir
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
global RED GREEN BLUE

global curvis
global curcolormode curthick colorflagToggle curforeground curhiddencolor
global curmarkmode curlinemode curmarksize curmarksizeunit curmarkforeground curmarkbackground
#global nbrowX nbrowY plot3dXVAL plot3dYVAL plot3dZVAL plot3dCOLORVAL
#global nbrowZ nbcolZ nbrowCOLOR nbcolCOLOR 
global ncolors flagCOLOR

global scicomint_dataX scicomint_dataY scicomint_dataZ scicomint_dataCOLOR
global curdata_x curdata_y curdata_z curdata_color

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 450
set NBwidth  300

set Wheight [expr $NBheight + 110]
set Wwidth  [expr $NBwidth  + 270]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Plot3d Object"
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
	       -width 20 -height 26 \
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

Notebook:create $uf.n -pages {"Style" "Data"} -pad 0 -height $NBheight -width $NBwidth

pack $uf.n -in $uf -fill both -expand yes

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n "Style"]

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
pack $w.frame.visib  -in $w.frame.vis    -side left -padx $smallPad

#Surface mode
frame $w.frame.linelinemode  -borderwidth 0
pack $w.frame.linelinemode  -in $w.frame  -side top  -fill x

label $w.frame.linemodelabel -height 0 -text "Surface mode:" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.linemode  -text "on" -indicatoron 1 \
    -variable curlinemode -onvalue "on" -offvalue "off" \
    -command "toggleLinemode $w.frame.linemode" -font $gedFont
OnOffForeground $w.frame.linemode $curlinemode

pack $w.frame.linemodelabel  -in $w.frame.linelinemode  -side left 
pack $w.frame.linemode   -in $w.frame.linelinemode   -side left  -fill x -pady 0 -padx $smallPad

#Color Mode
frame $w.frame.clrm  -borderwidth 0
pack $w.frame.clrm  -in $w.frame -side top  -fill x

label $w.frame.colorlabelm -height 0 -text "Color mode:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.colorm -orient horizontal -from -$ncolors -to $ncolors \
	 -resolution 1.0 -command "setColorMode $w.frame.colorm" -tickinterval 0  -font $gedFont

pack $w.frame.colorlabelm -in $w.frame.clrm -side left
pack $w.frame.colorm  -in  $w.frame.clrm -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.colorm set $curcolormode

#Foreground
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Foreground:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setForeground $w.frame.color" -tickinterval 0  -font $gedFont

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color  -in  $w.frame.clrf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $curforeground

#Hidden Color
frame $w.frame.clrh  -borderwidth 0
pack $w.frame.clrh  -in $w.frame -side top  -fill x

label $w.frame.colorlabelh -height 0 -text "Hidden color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.colorh -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setHiddenColor $w.frame.colorh" -tickinterval 0  -font $gedFont

pack $w.frame.colorlabelh -in $w.frame.clrh -side left
pack $w.frame.colorh  -in  $w.frame.clrh -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.colorh set $curhiddencolor


#Thickness
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x

label $w.frame.thicklabel -height 0 -text "Thickness:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.thickness \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curthick \
    -editable true \
    -command [list SelectThickness ] -font $gedFont
eval $w.frame.thickness list insert end [list "0.0" "1.0" "2.0" "3.0" "4.0" "5.0" "6.0" "8.0" "10.0"]

pack $w.frame.thicklabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk  -expand 1 -fill x -pady 0 -padx $mediumPad


#Color Flag
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x  -pady 0

label $w.frame.scalesw.label -height 0 -text "Color flag:" -width 0  -font $gedFont -anchor e -width $largeur
radiobutton $w.frame.scalesw.radio0 -text "0" -variable colorflagToggle -value 0    -command "toggleColorFlag" -font $gedFont
radiobutton $w.frame.scalesw.radio1 -text "1" -variable colorflagToggle -value 1    -command "toggleColorFlag" -font $gedFont

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left
pack $w.frame.scalesw.radio0 -in  $w.frame.scalesw -side left  -padx $smallPad
pack $w.frame.scalesw.radio1  -in  $w.frame.scalesw    -side left -padx $smallPad



#Mark mode
frame $w.frame.linemarkmode  -borderwidth 0
pack $w.frame.linemarkmode  -in $w.frame  -side top  -fill x

label $w.frame.markmodelabel -height 0 -text "Mark mode:" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.markmode  -text "on" -indicatoron 1 \
    -variable curmarkmode -onvalue "on" -offvalue "off" \
    -command "toggleMarkmode $w.frame.markmode" -font $gedFont
OnOffForeground $w.frame.markmode $curmarkmode

pack $w.frame.markmodelabel  -in $w.frame.linemarkmode  -side left 
pack $w.frame.markmode   -in $w.frame.linemarkmode   -side left  -fill x -pady 0 -padx $smallPad


#Mark style
frame $w.frame.linemarkst  -borderwidth 0
pack $w.frame.linemarkst  -in $w.frame  -side top  -fill x

label $w.frame.markstylelabel  -height 0 -text "Mark style:" -width 0  -font $gedFont -anchor e -width $largeur
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

label $w.frame.marksizelabel -height 0 -text "Mark size:" -width 0  -font $gedFont -anchor e -width $largeur
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
label $w.frame.marksizeunitlabel  -height 0 -text "Mark size:" -width 0 -font $gedFont -anchor e -width $largeur
combobox $w.frame.marksizeunit \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 13 \
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

label $w.frame.markflabel -height 0 -text "Mark foreground:" -width 0 -font $gedFont -anchor e -width $largeur
scale $w.frame.markforeground -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setMarkForeground $w.frame.markforeground" -tickinterval 0  -font $gedFont

pack $w.frame.markflabel -in $w.frame.markf -side left
pack $w.frame.markforeground  -in  $w.frame.markf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.markforeground set $curmarkforeground


#Mark background
frame $w.frame.markb  -borderwidth 0
pack $w.frame.markb  -in $w.frame -side top  -fill x

label $w.frame.markblabel -height 0 -text "Mark background:" -width 0 -font $gedFont -anchor e -width $largeur
scale $w.frame.markbackground -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setMarkBackground $w.frame.markbackground" -tickinterval 0  -font $gedFont

pack $w.frame.markblabel -in $w.frame.markb -side left
pack $w.frame.markbackground  -in  $w.frame.markb -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.markbackground set $curmarkbackground


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
button $w.buttons -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.buttons -side bottom




########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n "Data"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

##############################
## DATA edit via sciGUI ######
##############################

frame $w.frame.curdataframeX  -borderwidth 0
pack $w.frame.curdataframeX  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelX  -height 0 -text "X Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_x \
    -editable false \
    -command [list SelectDataX ] -font $gedFont
eval $w.frame.polydataX list insert end [list $curdata_x "----" "Edit data..."]
pack $w.frame.polydatalabelX -in $w.frame.curdataframeX  -side left
pack $w.frame.polydataX   -in $w.frame.curdataframeX  -expand 1 -fill x -pady 0 -padx $mediumPad


frame $w.frame.curdataframeY  -borderwidth 0
pack $w.frame.curdataframeY  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelY  -height 0 -text "Y Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.polydataY \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_y \
    -editable false \
    -command [list SelectDataY ] -font $gedFont
eval $w.frame.polydataY list insert end [list $curdata_y "----" "Edit data..."]
pack $w.frame.polydatalabelY -in $w.frame.curdataframeY  -side left
pack $w.frame.polydataY   -in $w.frame.curdataframeY  -expand 1 -fill x -pady 0 -padx $mediumPad

frame $w.frame.curdataframeZ  -borderwidth 0
pack $w.frame.curdataframeZ  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelZ  -height 0 -text "Z Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.polydataZ \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_z \
    -editable false \
    -command [list SelectDataZ ] -font $gedFont
eval $w.frame.polydataZ list insert end [list $curdata_z "----" "Edit data..."]
pack $w.frame.polydatalabelZ -in $w.frame.curdataframeZ  -side left
pack $w.frame.polydataZ   -in $w.frame.curdataframeZ  -expand 1 -fill x -pady 0 -padx $mediumPad

if {$flagCOLOR == 1} {
    frame $w.frame.curdataframeColor  -borderwidth 0
    pack $w.frame.curdataframeColor  -in $w.frame  -side top  -fill x
    
    label $w.frame.polydatalabelColor  -height 0 -text "Color Data:" -width 0  -font $gedFont -anchor e -width $largeur
    combobox $w.frame.polydataColor \
	-borderwidth 1 \
	-highlightthickness 1 \
	-maxheight 0 \
	-width 3 \
	-textvariable curdata_color \
	-editable false \
	-command [list SelectDataColor ] -font $gedFont
    eval $w.frame.polydataColor list insert end [list $curdata_color "----" "Edit data..."]
    pack $w.frame.polydatalabelColor -in $w.frame.curdataframeColor  -side left
    pack $w.frame.polydataColor   -in $w.frame.curdataframeColor  -expand 1 -fill x -pady 0 -padx $mediumPad
    
    
    label  $w.frame.warning  -text "Color field is not yet taken into account with Plot3d entity. \nCreate a Fac3d entity to use color data." -font $gedFont
    $w.frame.warning config -foreground red
    pack $w.frame.warning  -in $w.frame -side top -fill x
}
    


#######################################################
## DATA edit via Scilab Command Interface sciGUI ######
#######################################################

frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 0

label $w.scicom1.label1 -text "Scilab Command Interface for data:" -font $gedFont
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicomX
pack $w.scicomX -side top -fill x -pady 0

label $w.scicomX.label1 -text "X Data:" -font $gedFont -anchor e -width $largeur
pack  $w.scicomX.label1 -in $w.scicomX -side left

entry $w.scicomX.text1 -relief sunken -textvariable scicomint_dataX -width 10 -font $gedFont
set_balloon $w.scicomX.text1 "Enter a variable defined in Scilab Console representing\n a real vector or matrix or use a macro call (defining a vector or matrix)\n to initialize the \"X data\" field."
bind  $w.scicomX.text1 <Return> "sciCommandData"
bind  $w.scicomX.text1 <KP_Enter> "sciCommandData"

pack $w.scicomX.text1  -side left  -fill both -padx $mediumPad


frame $w.scicomY
pack $w.scicomY -side top -fill x -pady 0

label $w.scicomY.label1 -text "Y Data:" -font $gedFont -anchor e -width $largeur
pack  $w.scicomY.label1 -in $w.scicomY -side left

entry $w.scicomY.text1 -relief sunken -textvariable scicomint_dataY -width 10 -font $gedFont
set_balloon $w.scicomY.text1 "Enter a variable defined in Scilab Console representing\n a real vector or matrix or use a macro call (defining a vector or matrix)\n to initialize the \"Y data\" field."
bind  $w.scicomY.text1 <Return> "sciCommandData"
bind  $w.scicomY.text1 <KP_Enter> "sciCommandData"

pack $w.scicomY.text1  -side left  -fill both -padx $mediumPad


frame $w.scicomZ
pack $w.scicomZ -side top -fill x -pady 0

label $w.scicomZ.label1 -text "Z Data:" -font $gedFont -anchor e -width $largeur
pack  $w.scicomZ.label1 -in $w.scicomZ -side left

entry $w.scicomZ.text1 -relief sunken -textvariable scicomint_dataZ -width 10 -font $gedFont
set_balloon $w.scicomZ.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call (defining a matrix)\n to initialize the \"Z data\" field."
bind  $w.scicomZ.text1 <Return> "sciCommandData"
bind  $w.scicomZ.text1 <KP_Enter> "sciCommandData"

pack $w.scicomZ.text1  -side left  -fill both -padx $mediumPad


frame $w.scicomCOLOR
pack $w.scicomCOLOR -side top -fill x -pady 0

label $w.scicomCOLOR.label1 -text "Color Data:" -font $gedFont -anchor e -width $largeur
pack  $w.scicomCOLOR.label1 -in $w.scicomCOLOR -side left

entry $w.scicomCOLOR.text1 -relief sunken -textvariable scicomint_dataCOLOR -width 10 -font $gedFont
set_balloon $w.scicomCOLOR.text1 "Enter a variable defined in Scilab Console representing\n an integer matrix or use a macro call (defining a matrix)\n to initialize the \"Color data\" field."
bind  $w.scicomCOLOR.text1 <Return> "sciCommandData"
bind  $w.scicomCOLOR.text1 <KP_Enter> "sciCommandData"

pack $w.scicomCOLOR.text1  -side left  -fill both -padx $mediumPad


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w.b -side bottom 


pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
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
	ScilabEval "global ged_handle; if ged_handle.color_mode <> $index then ged_handle.color_mode=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.color_mode <> $index then ged_handle.color_mode=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.color_mode <> $index then ged_handle.color_mode=$index; end;"
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



proc setHiddenColor {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index < 0 } {
	ScilabEval "global ged_handle; if ged_handle.hiddencolor <> $index then ged_handle.hiddencolor=$index; end;"
	
	set index [expr -$index]
	
	set REDCOL $RED($index)
	set GRECOL $GREEN($index)
	set BLUCOL $BLUE($index)
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.hiddencolor <> $index then ged_handle.hiddencolor=$index; end;"
	#like $index==1: display first color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.hiddencolor <> $index then ged_handle.hiddencolor=$index; end;"
	
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

proc sciCommandData {} {
    global scicomint_dataX scicomint_dataY scicomint_dataZ scicomint_dataCOLOR
    
    if { ($scicomint_dataX == "") || ($scicomint_dataY == "") ||
	 ($scicomint_dataZ == "")  } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must fill in all the fields (only color field is optional) using variables defined in Scilab Console\n to initialize the \"data\" field."
    } else {
	if { $scicomint_dataCOLOR == ""} {
	    ScilabEval "set3dtlistXYZ($scicomint_dataX,$scicomint_dataY,$scicomint_dataZ)" "seq"
	    #Refresh now !
	    ScilabEval "tkged();" "seq"
	} else {
	    ScilabEval "set3dtlistXYZC($scicomint_dataX,$scicomint_dataY,$scicomint_dataZ,$scicomint_dataCOLOR)" "seq"
	    #Refresh now !
	    ScilabEval "tkged();" "seq"
	}
    }   
}

proc SelectDataX  {w args} {
    global curdata_x
    variable mycurdata
    set mycurdata $curdata_x
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data.x,\"ged_handle.data.x\")" "seq"
	}
    }
}


proc SelectDataY  {w args} {
    global curdata_y
    variable mycurdata
    set mycurdata $curdata_y
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data.y,\"ged_handle.data.y\")" "seq"
	}
    }
}

proc SelectDataZ  {w args} {
    global curdata_z
    variable mycurdata
    set mycurdata $curdata_z
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data.z,\"ged_handle.data.z\")" "seq"
	}
    }
}

proc SelectDataColor  {w args} {
    global curdata_color
    variable mycurdata
    set mycurdata $curdata_color
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data.color,\"ged_handle.data.color\")" "seq"
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


proc toggleMarkmode { frame } {
    global curmarkmode
    ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"
    
    OnOffForeground $frame $curmarkmode
}

proc toggleLinemode { frame } {
    global curlinemode
    ScilabEval "global ged_handle;ged_handle.surface_mode='$curlinemode'"
    
    OnOffForeground $frame $curlinemode
}


proc SelectMarkStyle {w args} {
global curmarkstyle
global curmarkmode
ScilabEval "setMarkStyle('$curmarkstyle')"

set curmarkmode "on"
#ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"

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
