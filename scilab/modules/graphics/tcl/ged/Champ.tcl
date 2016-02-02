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


global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject

global curvis
global curarrowsize curthick curlinestyle
global RED GREEN BLUE ncolors
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox

global curcolored 
#global nbrow nbcol champVAL
global scicomint_data
global curdata_x curdata_y curdata_fx curdata_fy
global scicomint_dataFX scicomint_dataFY scicomint_dataFX scicomint_dataFY
global smallPad mediumPad

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 250
set NBwidth  250

set smallPad  4
set mediumPad 8

set Wheight [expr $NBheight + 125]
set Wwidth  [expr $NBwidth  + 275]

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Champ Object"
wm iconname $ww "CO"

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
	       -width 20 -height 11 \
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

set largeur 12

Notebook:create $uf.n -pages {Style Data Clipping} -pad 0 -height $NBheight -width $NBwidth
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


#Line Style
frame $w.frame.linest  -borderwidth 0
pack $w.frame.linest  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Line style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ] -font $gedFont
eval $w.frame.style list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"]
pack $w.frame.stylelabel -in $w.frame.linest   -side left
pack $w.frame.style   -in $w.frame.linest   -expand 1 -fill x -pady 0 -padx $mediumPad


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x

label $w.frame.scalelabel -height 0 -text "Thickness:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0 -font $gedFont
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk  -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.thickness set $curthick

#Arrow size
frame $w.frame.ar -borderwidth 0
pack $w.frame.ar  -in $w.frame -side top   -fill x

label $w.frame.arrowlab -text "Arrow size:" -font $gedFont -anchor e -width $largeur
entry $w.frame.arrow -relief sunken  -textvariable curarrowsize -width 10
pack $w.frame.arrowlab -in $w.frame.ar -side left
pack $w.frame.arrow  -in $w.frame.ar -side left -pady 0 -padx $mediumPad 
bind  $w.frame.arrow <Return> {SelectArrowSize}
bind  $w.frame.arrow <KP_Enter> {SelectArrowSize}
bind  $w.frame.arrow <FocusOut> {SelectArrowSize}

#colored
frame $w.frame.col -borderwidth 0
pack $w.frame.col  -in $w.frame  -side top -fill x
label $w.frame.collabel  -text "Colored:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.colored  -text "on"\
    -variable curcolored  -onvalue "on" -offvalue "off" \
    -command "toggleCol $w.frame.colored" -font $gedFont
OnOffForeground $w.frame.colored $curcolored

pack $w.frame.collabel -in $w.frame.col  -side left
pack $w.frame.colored  -in $w.frame.col    -side left -fill x -padx $smallPad


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
button $w.b -text Quit -command "destroy $ww"  -font $gedFont
pack $w.b -side bottom 


########### Data onglet ###########################################
###################################################################
set w3 [Notebook:frame $uf.n Data]

frame $w3.frame -borderwidth 0
pack $w3.frame -anchor w -fill both

# set mycurdata $curdata

# frame $w3.frame.curdataframe  -borderwidth 0
# pack $w3.frame.curdataframe  -in $w3.frame  -side top  -fill x

# label $w3.frame.polydatalabel  -height 0 -text "Data field:    " -width 0 
# combobox $w3.frame.polydata \
#     -borderwidth 1 \
#     -highlightthickness 1 \
#     -maxheight 0 \
#     -width 3 \
#     -textvariable curdata \
#     -editable true \
#     -command [list SelectData ]
# eval $w3.frame.polydata list insert end [list $mycurdata "----" "Edit data..."]
# pack $w3.frame.polydatalabel -in $w3.frame.curdataframe  -side left
# pack $w3.frame.polydata   -in $w3.frame.curdataframe  -expand 1 -fill x -pady 0 -padx $mediumPad


##############################
## DATA edit via sciGUI ######
##############################

frame $w3.frame.curdataframeX  -borderwidth 0
pack $w3.frame.curdataframeX  -in $w3.frame  -side top  -fill x

label $w3.frame.polydatalabelX  -height 0 -text "X Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w3.frame.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_x \
    -editable false \
    -command [list SelectDataX ] -font $gedFont
eval $w3.frame.polydataX list insert end [list $curdata_x "----" "Edit data..."]
pack $w3.frame.polydatalabelX -in $w3.frame.curdataframeX  -side left
pack $w3.frame.polydataX   -in $w3.frame.curdataframeX  -expand 1 -fill x -pady 0 -padx $mediumPad


frame $w3.frame.curdataframeY  -borderwidth 0
pack $w3.frame.curdataframeY  -in $w3.frame  -side top  -fill x

label $w3.frame.polydatalabelY  -height 0 -text "Y Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w3.frame.polydataY \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_y \
    -editable false \
    -command [list SelectDataY ] -font $gedFont
eval $w3.frame.polydataY list insert end [list $curdata_y "----" "Edit data..."]
pack $w3.frame.polydatalabelY -in $w3.frame.curdataframeY  -side left
pack $w3.frame.polydataY   -in $w3.frame.curdataframeY  -expand 1 -fill x -pady 0 -padx $mediumPad

frame $w3.frame.curdataframeFX  -borderwidth 0
pack $w3.frame.curdataframeFX  -in $w3.frame  -side top  -fill x

label $w3.frame.polydatalabelFX  -height 0 -text "FX Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w3.frame.polydataFX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_fx \
    -editable false \
    -command [list SelectDataFX ] -font $gedFont
eval $w3.frame.polydataFX list insert end [list $curdata_fx "----" "Edit data..."]
pack $w3.frame.polydatalabelFX -in $w3.frame.curdataframeFX  -side left
pack $w3.frame.polydataFX   -in $w3.frame.curdataframeFX  -expand 1 -fill x -pady 0 -padx $mediumPad


frame $w3.frame.curdataframeFY  -borderwidth 0
pack $w3.frame.curdataframeFY  -in $w3.frame  -side top  -fill x

label $w3.frame.polydatalabelFY  -height 0 -text "FY Data:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w3.frame.polydataFY \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_fy \
    -editable false \
    -command [list SelectDataFY ] -font $gedFont
eval $w3.frame.polydataFY list insert end [list $curdata_fy "----" "Edit data..."]
pack $w3.frame.polydatalabelFY -in $w3.frame.curdataframeFY  -side left
pack $w3.frame.polydataFY   -in $w3.frame.curdataframeFY  -expand 1 -fill x -pady 0 -padx $mediumPad



#######################################################
## DATA edit via Scilab Command Interface sciGUI ######
#######################################################

frame $w3.scicom1
pack $w3.scicom1 -side top -fill x -pady 0

label $w3.scicom1.label1 -text "Scilab Command Interface for data:" -font $gedFont -anchor w
pack  $w3.scicom1.label1 -in $w3.scicom1 -side left

frame $w3.scicomX
pack $w3.scicomX -side top -fill x -pady 0

label $w3.scicomX.label1 -text "X Data:" -font $gedFont -anchor e -width $largeur
pack  $w3.scicomX.label1 -in $w3.scicomX -side left

entry $w3.scicomX.text1 -relief sunken -textvariable scicomint_dataX -width 10 -font $gedFont
set_balloon $w3.scicomX.text1 "Enter a variable defined in Scilab Console representing\n a real vector or matrix or use a macro call (defining a vector)\n to initialize the \"X data\" field."
bind  $w3.scicomX.text1 <Return> "sciCommandData"
bind  $w3.scicomX.text1 <KP_Enter> "sciCommandData"

pack $w3.scicomX.text1  -side left  -fill both -expand yes


frame $w3.scicomY
pack $w3.scicomY -side top -fill x -pady 0

label $w3.scicomY.label1 -text "Y Data:" -font $gedFont -anchor e -width $largeur
pack  $w3.scicomY.label1 -in $w3.scicomY -side left

entry $w3.scicomY.text1 -relief sunken -textvariable scicomint_dataY -width 10 -font $gedFont
set_balloon $w3.scicomY.text1 "Enter a variable defined in Scilab Console representing\n a real vector or matrix or use a macro call (defining a vector)\n to initialize the \"Y data\" field."
bind  $w3.scicomY.text1 <Return> "sciCommandData"
bind  $w3.scicomY.text1 <KP_Enter> "sciCommandData"

pack $w3.scicomY.text1  -side left  -fill both -expand yes


frame $w3.scicomFX
pack $w3.scicomFX -side top -fill x -pady 0

label $w3.scicomFX.label1 -text "FX Data:" -font $gedFont -anchor e -width $largeur
pack  $w3.scicomFX.label1 -in $w3.scicomFX -side left

entry $w3.scicomFX.text1 -relief sunken -textvariable scicomint_dataFX -width 10 -font $gedFont
set_balloon $w3.scicomFX.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call (defining a matrix)\n to initialize the \"FX data\" field."
bind  $w3.scicomFX.text1 <Return> "sciCommandData"
bind  $w3.scicomFX.text1 <KP_Enter> "sciCommandData"

pack $w3.scicomFX.text1  -side left  -fill both -expand yes



frame $w3.scicomFY
pack $w3.scicomFY -side top -fill x -pady 0

label $w3.scicomFY.label1 -text "FY Data:" -font $gedFont -anchor e -width $largeur
pack  $w3.scicomFY.label1 -in $w3.scicomFY -side left 

entry $w3.scicomFY.text1 -relief sunken -textvariable scicomint_dataFY -width 10 -font $gedFont
set_balloon $w3.scicomFY.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call (defining a matrix)\n to initialize the \"FY data\" field."
bind  $w3.scicomFY.text1 <Return> "sciCommandData"
bind  $w3.scicomFY.text1 <KP_Enter> "sciCommandData"

pack $w3.scicomFY.text1  -side left  -fill both -expand yes -pady "0 $smallPad"







# frame $w3.frame2.fdata -borderwidth 0
# pack $w3.frame2.fdata  -in $w3.frame2 -side top   -fill x


# canvas $w3.frame2.c1 -width 8i -height 4i  -yscrollcommand {$w3.frame2.ysbar set} -xscrollcommand {$w3.frame2.xsbar set}
# scrollbar $w3.frame2.ysbar -orient vertical -command   {$w3.frame2.c1 yview}
# scrollbar $w3.frame2.xsbar -orient horizontal -command   {$w3.frame2.c1 xview}
# #scrollbar $w3.frame2.ysbar.scrollh -orient horizontal -command {$w.frame2.c1 xview}
# #pack configure $w3.frame2.ysbar.scrollh -side bottom -fill x -expand 1 
   
# for {set i 1} {$i<=$nbrow} {incr i} {
# #    puts $i
#     set bb [expr 10+(25*$i)]
#     $w3.frame2.c1 create text 10 $bb -anchor c -text $i
#     for {set j 1} {$j<=$nbcol} {incr j} {
# #	puts $j
# 	set aa [expr 10+($j*150)]
# 	$w3.frame2.c1 create text $aa 10 -anchor c -text $j
# 	set tmp $w3.frame2.c1.data$i
# 	set tmp $tmp+"_"
# 	entry  $tmp$j  -relief sunken  -textvariable champVAL($i,$j)
# 	bind   $tmp$j <Return> "setData $i $j"
# #location help balloon	
# 	set_balloon $tmp$j "Row: $i Column: $j"

# 	$w3.frame2.c1 create window $aa $bb -anchor c -window $tmp$j
#     }
# }

# $w3.frame2.c1 configure -scrollregion [$w3.frame2.c1 bbox all] -yscrollincrement 0.1i -xscrollincrement 0.1i
# # $w3.frame2.c1 configure -scrollregion [$w3.frame2.c1 bbox all] -xscrollincrement 0.1i

# #pack  $w3.frame2.ysbar.scrollh -side bottom -fill x
# pack  $w3.frame2.ysbar -side right -fill y
# pack  $w3.frame2.xsbar -side bottom -fill x
# pack  $w3.frame2.c1

# frame $w3.scicom1
# pack $w3.scicom1 -side top -fill x -pady 0

# label $w3.scicom1.label1 -text "Scilab Command Interface for data:"
# pack  $w3.scicom1.label1 -in $w3.scicom1 -side left

# frame $w3.scicom
# pack $w3.scicom -side top -fill x -pady 0

# label $w3.scicom.label1 -text "champ_handle.data = "
# pack  $w3.scicom.label1 -in $w3.scicom -side left

# entry $w3.scicom.text1 -relief sunken -textvariable scicomint_data
# set_balloon $w3.scicom.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call (defining a matrix)\n to initialize the \"data\" field."
# bind  $w3.scicom.text1 <Return> "sciCommandData"

# pack $w3.scicom.text1  -side left  -fill both -expand yes

#sep bar
frame $w3.sep -height 2 -borderwidth 1 -relief sunken
pack $w3.sep -fill both


#exit button
frame $w3.buttons
button $w3.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w3.b -side bottom 


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

proc toggleCol { frame } {
    global curcolored
    ScilabEval "global ged_handle;ged_handle.colored='$curcolored'"

    OnOffForeground $frame $curcolored
}

proc SelectArrowSize {} {
global curarrowsize
ScilabEval "global ged_handle;ged_handle.arrow_size=$curarrowsize"
}

proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
}



proc setThickness {w thick} {
ScilabEval "global ged_handle;if ged_handle.thickness <> $thick then ged_handle.thickness=$thick; end;"
}


proc setData { i j } {
global segsVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i,$j)=$segsVAL($i,$j);\",\'errcatch\',\'n\');"
}



proc setSegsColorData { i } {
    global segscolorVAL
    ScilabEval "execstr(\"global ged_handle; tmp = ged_handle.segs_color; tmp($i)=$segscolorVAL($i);ged_handle.segs_color = tmp;\",\'errcatch\',\'n\');"
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


proc setData { i j } {
global champVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i,$j)=$champVAL($i,$j);\",\'errcatch\',\'n\');"
}



proc sciCommandData {} {
    global scicomint_data
    
    if { $scicomint_data == "" } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a real matrix\n or use a macro call (defining a matrix)\n to initialize the \"data\" field."
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

proc sciCommandData {} {
    global scicomint_dataX scicomint_dataY scicomint_dataFX scicomint_dataFY
    
    if { ($scicomint_dataX == "") || ($scicomint_dataY == "") ||
	 ($scicomint_dataFX == "") || ($scicomint_dataFY == "") } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must fill in all the fields (only color field is optional) using variables defined in Scilab Console\n to initialize the \"data\" field."
    } else {
	ScilabEval "setchamptlistXYFXFY($scicomint_dataX,$scicomint_dataY,$scicomint_dataFX,$scicomint_dataFY)" "seq"
	#Refresh now !
	ScilabEval "tkged();" "seq"
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

    puts "mycurdata = $mycurdata"

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

proc SelectDataFX  {w args} {
    global curdata_fx
    variable mycurdata
    set mycurdata $curdata_fx
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
    #    puts "finddbarray = $finddbarray"
    
    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data.fx,\"ged_handle.data.fx\")" "seq"
	}
    }
}


proc SelectDataFY  {w args} {
    global curdata_fy
    variable mycurdata
    set mycurdata $curdata_fy
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
    #    puts "finddbarray = $finddbarray"
    
    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data.fy,\"ged_handle.data.fy\")" "seq"
	}
    }
}
