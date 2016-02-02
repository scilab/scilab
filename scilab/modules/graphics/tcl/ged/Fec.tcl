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
global curarrowsize curthick curlinestyle datafecVAL trianglesfecVAL
global RED GREEN BLUE ncolors
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox

#global nbrow nbcol nbrowTri nbcolTri
global zbmin zbmax
global scicomint_data scicomint_triangles
global curdata_data curdata_triangles

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 180
set NBwidth  215

set Wheight [expr $NBheight + 110]
set Wwidth  [expr $NBwidth  + 270]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Fec Object"
wm iconname $ww "FO"
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

set largeur 10

Notebook:create $uf.n -pages {Style Data } -pad 0 -height $NBheight -width $NBwidth
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

#z bounds
frame $w.frame.zb  -borderwidth 0
pack $w.frame.zb  -in $w.frame -side top   -fill x 

label $w.frame.datalabel -text "Z bounds:" -font $gedFont -anchor e -width $largeur
entry $w.frame.zbmin -relief sunken  -textvariable zbmin -width 6 -font $gedFont
entry $w.frame.zbmax -relief sunken  -textvariable zbmax -width 6 -font $gedFont
pack $w.frame.datalabel -in  $w.frame.zb -side left
pack $w.frame.zbmin -in  $w.frame.zb  -side left  -pady 0 -padx $mediumPad
pack $w.frame.zbmax -in  $w.frame.zb  -side left  -pady 0 -padx $mediumPad
bind  $w.frame.zbmin <Return> {setZb} 
bind  $w.frame.zbmax <Return> {setZb} 
bind  $w.frame.zbmin <KP_Enter> {setZb} 
bind  $w.frame.zbmax <KP_Enter> {setZb} 
bind  $w.frame.zbmin <FocusOut> {setZb} 
bind  $w.frame.zbmax <FocusOut> {setZb} 



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
button $w.buttons -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.buttons -side bottom


########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n Data]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


##############################
## DATA edit via sciGUI ######
##############################

frame $w.frame.curdataframeX  -borderwidth 0
pack $w.frame.curdataframeX  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelX  -height 0 -text "Data:" -width 10  -font $gedFont -anchor e -width $largeur
combobox $w.frame.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_data \
    -editable false \
    -command [list SelectDataData ] -font $gedFont
eval $w.frame.polydataX list insert end [list $curdata_data "----" "Edit data..."]
pack $w.frame.polydatalabelX -in $w.frame.curdataframeX  -side left
pack $w.frame.polydataX   -in $w.frame.curdataframeX  -pady 0 -padx 0 -fill x


frame $w.frame.curdataframeY  -borderwidth 0
pack $w.frame.curdataframeY  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelY  -height 0 -text "Triangles:" -width 10  -font $gedFont -anchor e -width $largeur
combobox $w.frame.polydataY \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_triangles \
    -editable false \
    -command [list SelectDataTriangles ] -font $gedFont
eval $w.frame.polydataY list insert end [list $curdata_triangles "----" "Edit data..."]
pack $w.frame.polydatalabelY -in $w.frame.curdataframeY  -side left
pack $w.frame.polydataY   -in $w.frame.curdataframeY  -pady 0 -padx 0 -fill x


#######################################################
## DATA edit via Scilab Command Interface sciGUI ######
#######################################################

frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 0

label $w.scicom1.label1 -text "Scilab Command Interface for" -font $gedFont -anchor w
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicom2
pack $w.scicom2 -side top -fill x -pady 0

label $w.scicom2.label2 -text "data and triangles:" -font $gedFont -anchor w
pack  $w.scicom2.label2 -in $w.scicom2 -side left

frame $w.scicom
pack $w.scicom -side top -fill x -pady 0

label $w.scicom.label1 -text "Data:" -font $gedFont -anchor e -width $largeur
pack  $w.scicom.label1 -in $w.scicom -side left

entry $w.scicom.text1 -relief sunken -textvariable scicomint_data -width 10 -font $gedFont
set_balloon $w.scicom.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call (defining a matrix)\n to initialize the \"data\" field."
bind  $w.scicom.text1 <Return> "sciCommandData"
bind  $w.scicom.text1 <KP_Enter> "sciCommandData"

pack $w.scicom.text1  -side left  -fill both


frame $w.scicomT
pack $w.scicomT -side top -fill x -pady 0

label $w.scicomT.label1 -text "Triangles:" -font $gedFont -anchor e -width $largeur
pack  $w.scicomT.label1 -in $w.scicomT -side left

entry $w.scicomT.text1 -relief sunken -textvariable scicomint_triangles -width 10 -font $gedFont
set_balloon $w.scicomT.text1 "Enter an integer matrix defined in Scilab Console representing\n the indexes of the nodes which constitute\n each triangle to initialize the \"triangles\" field."
bind  $w.scicomT.text1 <Return> "sciCommandTriangles"
bind  $w.scicomT.text1 <KP_Enter> "sciCommandTriangles"

pack $w.scicomT.text1  -side left  -fill both



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w.b -side bottom 


# ########### Triangles onglet ######################################
# ###################################################################
# set w4 [Notebook:frame $uf.n Triangles]

# frame $w4.frame2 -borderwidth 0
# pack $w4.frame2 -anchor w -fill both

# frame $w4.frame2.fdata -borderwidth 0
# pack $w4.frame2.fdata  -in $w4.frame2 -side top   -fill x

# canvas $w4.frame2.c1 -width 8i -height 4i  -yscrollcommand {$w4.frame2.ysbar set} -xscrollcommand {$w4.frame2.xsbar set}
# scrollbar $w4.frame2.ysbar -orient vertical -command   {$w4.frame2.c1 yview}
# scrollbar $w4.frame2.xsbar -orient horizontal -command   {$w4.frame2.c1 xview}
# #scrollbar $w4.frame2.ysbar.scrollh -orient horizontal -command {$w.frame2.c1 xview}
# #pack configure $w4.frame2.ysbar.scrollh -side bottom -fill x -expand 1 
   
# for {set i 1} {$i<=$nbrowTri} {incr i} {
# #    puts $i
#     set bb [expr 10+(25*$i)]
#     $w4.frame2.c1 create text 10 $bb -anchor c -text $i
#     for {set j 1} {$j<=$nbcolTri} {incr j} {
# #	puts $j
# 	set aa [expr 10+($j*150)]
# 	$w4.frame2.c1 create text $aa 10 -anchor c -text $j
# 	set tmp $w4.frame2.c1.data$i
# 	set tmp $tmp+"_"
# 	entry  $tmp$j  -relief sunken  -textvariable trianglesfecVAL($i,$j)
# 	bind   $tmp$j <Return> "setTriangles $i $j"
# #location help balloon	
# 	set_balloon $tmp$j "Row: $i Column: $j"
	
# 	$w4.frame2.c1 create window $aa $bb -anchor c -window $tmp$j
#     }
# }

#  $w4.frame2.c1 configure -scrollregion [$w4.frame2.c1 bbox all] -yscrollincrement 0.1i -xscrollincrement 0.1i
# # $w4.frame2.c1 configure -scrollregion [$w4.frame2.c1 bbox all] -xscrollincrement 0.1i

# #pack  $w4.frame2.ysbar.scrollh -side bottom -fill x
#  pack  $w4.frame2.ysbar -side right -fill y
#  pack  $w4.frame2.xsbar -side bottom -fill x
#  pack  $w4.frame2.c1

# #sep bar
# frame $w4.sep -height 2 -borderwidth 1 -relief sunken
# pack $w4.sep -fill both


# #exit button
# frame $w4.buttons
# button $w4.b -text Quit -command "destroy $ww"
# pack $w4.b -side bottom 


########### Clipping onglet #######################################
###################################################################

#set w9 [Notebook:frame $uf.n Clipping]
#
#frame $w9.frame -borderwidth 0
#pack $w9.frame -anchor w -fill both
#
#set letext ""
#
##Clip state
##frame $w9.frame.clpwarning  -borderwidth 0
#
#frame $w9.frame.clpstat  -borderwidth 0
#pack $w9.frame.clpstat  -in $w9.frame -side top -fill x -pady 0
#
#label $w9.frame.cliplabel  -height 0 -text "Clip state:" -width 0  -font $gedFont -anchor e -width $largeur
#combobox $w9.frame.clip \
#    -borderwidth 1 \
#    -highlightthickness 1 \
#    -maxheight 0 \
#    -width 8 \
#    -textvariable curclipstate\
#    -editable false \
#    -command [list SelectClipState ] -font $gedFont
#eval $w9.frame.clip list insert end [list "on" "off" "clipgrf"]
#
#pack $w9.frame.cliplabel -in $w9.frame.clpstat   -side left
#pack $w9.frame.clip -in $w9.frame.clpstat -side left -pady 0 -padx $mediumPad
#
##clip box
#frame $w9.frame.lb1 -borderwidth 0
##pack $w9.frame.lb1  -in $w9.frame -side top   -fill x
#label $w9.frame.labelul -text "Clip box : upper-left point coordinates" -font $gedFont
#pack $w9.frame.labelul -in  $w9.frame.lb1 -side left
#
#frame $w9.frame.lb2 -borderwidth 0
#pack $w9.frame.lb2  -in $w9.frame -side top   -fill x
#
#frame $w9.frame.lb21 -borderwidth 0
#pack $w9.frame.lb21  -in $w9.frame -side top   -fill x
#
#frame $w9.frame.lb22 -borderwidth 0
#pack $w9.frame.lb22  -in $w9.frame -side top   -fill x
#
#label $w9.frame.labelx -text "X:" -font $gedFont  -anchor e -width $largeur
#entry $w9.frame.datax -relief sunken  -textvariable Xclipbox -width 10  -font $gedFont
#label $w9.frame.labely -text "Y:" -font $gedFont -anchor e -width $largeur
#entry $w9.frame.datay -relief sunken  -textvariable Yclipbox -width 10  -font $gedFont
#
#pack $w9.frame.labelx  -in  $w9.frame.lb2 -side left 
#pack $w9.frame.datax   -in  $w9.frame.lb2 -side left -pady 0 -padx $mediumPad
#pack $w9.frame.labely  -in  $w9.frame.lb21 -side left 
#pack $w9.frame.datay   -in  $w9.frame.lb21 -side left -pady 0 -padx $mediumPad
#bind  $w9.frame.datax <Return> "SelectClipBox $w9.frame"
#bind  $w9.frame.datay <Return> "SelectClipBox $w9.frame"
#bind  $w9.frame.datax <KP_Enter> "SelectClipBox $w9.frame"
#bind  $w9.frame.datay <KP_Enter> "SelectClipBox $w9.frame"
#
##----------------------------#
#frame $w9.frame.lb3 -borderwidth 0
#pack $w9.frame.lb3  -in $w9.frame -side top   -fill x
#label $w9.frame.labelwh -text "Clip box : width and height" -font $gedFont
#pack $w9.frame.labelwh -in  $w9.frame.lb3 -side left
#
#frame $w9.frame.lb4 -borderwidth 0
#pack $w9.frame.lb4  -in $w9.frame -side top   -fill x
#
#frame $w9.frame.lb41 -borderwidth 0
#pack $w9.frame.lb41  -in $w9.frame -side top   -fill x
#
#label $w9.frame.labelw -text "W:" -font $gedFont -anchor e -width $largeur
#entry $w9.frame.dataw -relief sunken  -textvariable Wclipbox -width 10  -font $gedFont
#label $w9.frame.labelh -text "H:" -font $gedFont -anchor e -width $largeur
#entry $w9.frame.datah -relief sunken  -textvariable Hclipbox -width 10  -font $gedFont
#
#pack $w9.frame.labelw  -in  $w9.frame.lb4 -side left 
#pack $w9.frame.dataw   -in  $w9.frame.lb4 -side left -pady 0 -padx $mediumPad
#pack $w9.frame.labelh  -in  $w9.frame.lb41 -side left 
#pack $w9.frame.datah   -in  $w9.frame.lb41 -side left -pady 0 -padx $mediumPad
#bind  $w9.frame.dataw <Return> "SelectClipBox $w9.frame"
#bind  $w9.frame.datah <Return> "SelectClipBox $w9.frame"
#bind  $w9.frame.dataw <KP_Enter> "SelectClipBox $w9.frame"
#bind  $w9.frame.datah <KP_Enter> "SelectClipBox $w9.frame"
#
#
#frame $w9.frame.warning
#label $w9.frame.mesgwarning  -justify left -textvariable letext -font $gedFont
#$w9.frame.mesgwarning config -foreground red
#pack $w9.frame.mesgwarning -in $w9.frame.warning
#pack $w9.frame.warning -in $w9.frame
#
##sep bar
#frame $w9.sep -height 2 -borderwidth 1 -relief sunken
#pack $w9.sep -fill both
#
#
##exit button
#frame $w9.buttons
#button $w9.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
#pack $w9.b -side bottom







# ########### Scilab Command Interface ##############################
# ###################################################################
# set w5 [Notebook:frame $uf.n "Scilab Command Interface"]

# frame $w5.frame -borderwidth 0
# pack $w5.frame -anchor w -fill both


# frame $w5.scicom1
# pack $w5.scicom1 -side top -fill x -pady 0

# label $w5.scicom1.label1 -text "Scilab Command Interface for data and triangles:"
# pack  $w5.scicom1.label1 -in $w5.scicom1 -side left

# frame $w5.scicom
# pack $w5.scicom -side top -fill x -pady 0

# label $w5.scicom.label1 -text "fec_handle.data =         "
# pack  $w5.scicom.label1 -in $w5.scicom -side left

# entry $w5.scicom.text1 -relief sunken -textvariable scicomint_data
# set_balloon $w5.scicom.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call (defining a matrix)\n to initialize the \"data\" field."
# bind  $w5.scicom.text1 <Return> "sciCommandData"

# pack $w5.scicom.text1  -side left  -fill both -expand yes


# frame $w5.scicomT
# pack $w5.scicomT -side top -fill x -pady 0

# label $w5.scicomT.label1 -text "fec_handle.triangles =   "
# pack  $w5.scicomT.label1 -in $w5.scicomT -side left

# entry $w5.scicomT.text1 -relief sunken -textvariable scicomint_triangles
# set_balloon $w5.scicomT.text1 "Enter an integer matrix defined in Scilab Console representing\n the indexes of the nodes which constitute\n each triangle to initialize the \"triangles\" field."
# bind  $w5.scicomT.text1 <Return> "sciCommandTriangles"

# pack $w5.scicomT.text1  -side left  -fill both -expand yes



# #sep bar
# frame $w5.sep -height 2 -borderwidth 1 -relief sunken
# pack $w5.sep -fill both


# #exit button
# frame $w5.buttons
# button $w5.b -text Quit -command "destroy $ww"
# pack $w5.b -side bottom 

pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
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
ScilabEval "global ged_handle;ged_handle.thickness=$thick;"
}


proc setData { i j } {
global datafecVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i,$j)=$datafecVAL($i,$j);\",\'errcatch\',\'n\');"
}

proc setTriangles { i j } {
global trianglesfecVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.triangles($i,$j)=$trianglesfecVAL($i,$j);\",\'errcatch\',\'n\');"
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


proc setZb {} {
global zbmin zbmax
ScilabEval "setZb($zbmin, $zbmax);"
}

proc sciCommandData {} {
    global scicomint_data
    
    if { $scicomint_data == "" } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable  defined in Scilab Console representing a real matrix\n or use a macro call (defining a matrix)\n to initialize the \"data\" field."
    } else {
	
	ScilabEval "global ged_handle;ged_handle.data=$scicomint_data;" "seq"
	#Refresh now !
	ScilabEval "tkged();" "seq"
    }
}
    
    
proc sciCommandTriangles {} {
    global scicomint_triangles
    
    if { $scicomint_triangles == "" } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing the indexes of the nodes\n which constitute each triangle to initialize the \"triangles\" field."
    } else {
	
	ScilabEval "global ged_handle;ged_handle.triangles=$scicomint_triangles;" "seq"
	#Refresh now !
	ScilabEval "tkged();" "seq"
    }
}


proc SelectDataData  {w args} {
    global curdata_data
    variable mycurdata
    set mycurdata $curdata_data
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data,\"ged_handle.data\")" "seq"
	}
    }
}


proc SelectDataTriangles  {w args} {
    global curdata_triangles
    variable mycurdata
    set mycurdata $curdata_triangles
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.triangles,\"ged_handle.triangles\")" "seq"
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


