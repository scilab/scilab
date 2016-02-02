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

global curvis
global curarrowsize curthick curlinestyle
global RED GREEN BLUE ncolors
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox

global curseg curcolor curticsstyle curfontcolor curfontsize 
global nbcolX nbcolY xticscoord yticscoord
global cursubtics curticslabel curticsdir
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
set NBwidth  225

set smallPad  4
set mediumPad 8

set Wheight [expr $NBheight + 120]
set Wwidth  [expr $NBwidth  + 265]

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Axis Object"
wm iconname $ww "SO"
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

Notebook:create $uf.n -pages {Style Xtics Ytics Clipping} -pad 0 -height $NBheight -width $NBwidth
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

#tics segment
frame $w.frame.seg -borderwidth 0
pack $w.frame.seg  -in $w.frame  -side top -fill x
label $w.frame.seglabel  -text "Tics segment:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.segib  -text "on"\
    -variable curseg  -onvalue "on" -offvalue "off" \
    -command "toggleSeg $w.frame.segib" -font $gedFont
OnOffForeground $w.frame.segib $curseg

pack $w.frame.seglabel -in $w.frame.seg  -side left
pack $w.frame.segib  -in $w.frame.seg    -side left -fill x -padx $smallPad

#Color scale
frame $w.frame.ticscol  -borderwidth 0
pack $w.frame.ticscol  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Tics color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0  -font $gedFont

pack $w.frame.colorlabel -in $w.frame.ticscol -side left
pack $w.frame.color  -in  $w.frame.ticscol -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $curcolor

#Tics Style
frame $w.frame.ticsst  -borderwidth 0
pack $w.frame.ticsst  -in $w.frame  -side top  -fill x

label $w.frame.ticsstylelabel  -height 0 -text "Tics style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.ticsstyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curticsstyle \
    -editable false \
    -command [list SelectTicsStyle ] -font $gedFont
eval $w.frame.ticsstyle list insert end [list "v" "r" "i"]
pack $w.frame.ticsstylelabel -in $w.frame.ticsst   -side left
pack $w.frame.ticsstyle   -in $w.frame.ticsst -anchor w  -pady 0 -padx $mediumPad

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontcolorlabel -height 0 -text "Labels font color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0  -font $gedFont

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontcolor set $curfontcolor


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontsizlabel -height 0 -text "Labels font size:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal -from -1 -to 6 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0  -font $gedFont

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz   -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $curfontsize

   
frame $w.frame.cas -borderwidth 0
pack $w.frame.cas -anchor w -fill both

label $w.frame.subticslab -height 0 -text "Subtics:"  -font $gedFont -anchor e -width $largeur
entry $w.frame.subtics  -relief sunken  -textvariable cursubtics  -width 10 -font $gedFont
bind  $w.frame.subtics  <Return> "SelectSubtics  $w.frame"
bind  $w.frame.subtics  <KP_Enter> "SelectSubtics  $w.frame"
bind  $w.frame.subtics  <FocusOut> "SelectSubtics  $w.frame"
pack  $w.frame.subticslab  -in  $w.frame.cas  -side left
pack  $w.frame.subtics     -in  $w.frame.cas  -side left -fill x -pady 0 -padx $mediumPad


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
button $w.buttons -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.buttons -side bottom

#Trimmings here
###############
# proc TrimmingFunction  { stringtotrim value } {
#     #Trimming et epuration des $stringtotrim dans $value
    
#     set i [string last $stringtotrim $value]
#     set i [string last $stringtotrim $value]
#     while { $i != -1 } {
# 	set firstocc [string last $stringtotrim $value]
# 	set tmp [string range $value 0 [expr $firstocc-1]]
# 	append tmp [string range $value [expr $firstocc+1] end] 
# 	set value $tmp
# 	set i [string first $stringtotrim $value]
#     }
#     return $value
# }

# #xticscoord
# set xticscoord [TrimmingFunction \"  $xticscoord]
# set xticscoord [TrimmingFunction \=  $xticscoord ]
# set xticscoord [TrimmingFunction " " $xticscoord ]

# #yticscoord
# set yticscoord [TrimmingFunction \"  $yticscoord ]
# set yticscoord [TrimmingFunction \=  $yticscoord ]
# set yticscoord [TrimmingFunction " " $yticscoord ]


########### Xtics onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Xtics]


frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

if { $nbcolX == 1} {
    frame $w.frame.cas1_0 -borderwidth 0
    pack $w.frame.cas1_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Vertical axis case  --" -width 0  -font $gedFont -anchor w -width $largeur
    pack  $w.frame.warning -in  $w.frame.cas1_0  -expand 1 -fill x -pady 0
    
    frame $w.frame.cas1_1 -borderwidth 0
    pack $w.frame.cas1_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "Xtics coord:" -width 0  -font $gedFont -anchor e -width $largeur
    entry $w.frame.xcoord  -relief sunken  -textvariable xticscoord -width 10 -font $gedFont
    bind  $w.frame.xcoord  <Return> "SelectXticsCoord  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectXticsCoord  $w.frame"
    bind  $w.frame.xcoord  <FocusOut> "SelectXticsCoord  $w.frame"
    pack  $w.frame.xcoordlab   -in  $w.frame.cas1_1   -side left
		pack  $w.frame.xcoord      -in  $w.frame.cas1_1   -side left -fill x -pady 0 -padx $mediumPad
} else {
    frame $w.frame.cas2_0 -borderwidth 0
    pack $w.frame.cas2_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Horizontal axis case  --" -width 0 -font $gedFont -anchor w -width $largeur
    pack  $w.frame.warning -in  $w.frame.cas2_0  -expand 1 -fill x -pady 0
    
    frame $w.frame.cas2_1 -borderwidth 0
    pack $w.frame.cas2_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "Xtics coord:" -width 0  -font $gedFont -anchor e -width $largeur
#    text $w.frame.xcoord  -relief sunken -yscrollcommand "$w.frame.scroll set"
#    scrollbar $w.frame.scroll -command "$w.frame.xcoord yview"
#    $w.frame.xcoord insert end $xticscoord
#    pack $w.frame.scroll  -in  $w.frame.cas2_1 -side right -fill y
    entry $w.frame.xcoord  -relief sunken  -textvariable xticscoord -width 10 -font $gedFont
    bind  $w.frame.xcoord  <Return> "SelectXticsCoord2  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectXticsCoord2  $w.frame"
    bind  $w.frame.xcoord  <FocusOut> "SelectXticsCoord2  $w.frame"
    pack  $w.frame.xcoordlab  -in  $w.frame.cas2_1 -side left
    pack  $w.frame.xcoord  -in  $w.frame.cas2_1 -side left -fill x -pady 0 -padx $mediumPad
    
    frame $w.frame.cas2_2 -borderwidth 0
    pack $w.frame.cas2_2 -anchor w -fill both
    
    label $w.frame.ticslabellab -height 0 -text "Tics Labels:" -width 0  -font $gedFont -anchor e -width $largeur
    entry $w.frame.ticslabel  -relief sunken  -textvariable curticslabel -width 10 -font $gedFont
    bind  $w.frame.ticslabel  <Return> "SelectTicsLabels  $w.frame"
    bind  $w.frame.ticslabel  <KP_Enter> "SelectTicsLabels  $w.frame"
    bind  $w.frame.ticslabel  <FocusOut> "SelectTicsLabels  $w.frame"
    pack  $w.frame.ticslabellab  -in  $w.frame.cas2_2   -side left
    pack  $w.frame.ticslabel  -in  $w.frame.cas2_2 -expand 1 -fill x -pady 0 -padx $mediumPad

    
    #Tics direction
    frame $w.frame.px  -borderwidth 0
    pack $w.frame.px  -in $w.frame -side top   -fill x -pady 0
    
    label $w.frame.xposlabel  -height 0 -text "Tics direction:" -width 0  -font $gedFont -anchor e -width $largeur
    combobox $w.frame.xpos \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curticsdir \
    -editable false \
    -command [list SelectTicsdir ] -font $gedFont
    eval $w.frame.xpos list insert end [list "top" "bottom"]
    
    pack $w.frame.xposlabel -in  $w.frame.px -side left
    pack $w.frame.xpos  -in  $w.frame.px  -expand 1 -fill x -pady 0 -padx $mediumPad
}

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both
     


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w.b -side bottom 


########### Ytics onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Ytics]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

if { $nbcolY == 1} {
    frame $w.frame.cas1_0 -borderwidth 0
    pack $w.frame.cas1_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Horizontal axis case  --" -width 0  -font $gedFont -anchor w -width $largeur
    pack  $w.frame.warning -in  $w.frame.cas1_0  -expand 1 -fill x -pady 0
    
    frame $w.frame.cas1_1 -borderwidth 0
    pack $w.frame.cas1_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "Ytics coord:" -width 0  -font $gedFont -anchor e -width $largeur
    entry $w.frame.xcoord  -relief sunken  -textvariable yticscoord -width 10  -font $gedFont
    bind  $w.frame.xcoord  <Return> "SelectYticsCoord  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectYticsCoord  $w.frame"
    bind  $w.frame.xcoord  <FocusOut> "SelectYticsCoord  $w.frame"
    pack  $w.frame.xcoordlab  -in  $w.frame.cas1_1   -side left
 	  pack  $w.frame.xcoord     -in  $w.frame.cas1_1   -side left -fill x -pady 0 -padx $mediumPad
} else {
    frame $w.frame.cas2_0 -borderwidth 0
    pack $w.frame.cas2_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Vertical axis case  --" -width 0  -font $gedFont -anchor w -width $largeur
    pack  $w.frame.warning -in  $w.frame.cas2_0  -expand 1 -fill x -pady 0
   
    frame $w.frame.cas2_1 -borderwidth 0
    pack $w.frame.cas2_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "Ytics coord:" -width 0  -font $gedFont -anchor e -width $largeur
    entry $w.frame.xcoord  -relief sunken  -textvariable yticscoord -width 10 -font $gedFont
    bind  $w.frame.xcoord  <Return> "SelectYticsCoord2  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectYticsCoord2  $w.frame"
    bind  $w.frame.xcoord  <FocusOut> "SelectYticsCoord2  $w.frame"
    pack  $w.frame.xcoordlab  -in  $w.frame.cas2_1 -side left
    pack  $w.frame.xcoord  -in  $w.frame.cas2_1 -expand 1 -fill x -pady 0 -padx $mediumPad
    
    frame $w.frame.cas2_2 -borderwidth 0
    pack $w.frame.cas2_2 -anchor w -fill both

    label $w.frame.ticslabellab -height 0 -text "Tics Labels:" -width 0  -font $gedFont -anchor e -width $largeur
    entry $w.frame.ticslabel  -relief sunken  -textvariable curticslabel -width 10 -font $gedFont
    bind  $w.frame.ticslabel  <Return> "SelectTicsLabels  $w.frame"
    bind  $w.frame.ticslabel  <KP_Enter> "SelectTicsLabels  $w.frame"
    bind  $w.frame.ticslabel  <FocusOut> "SelectTicsLabels  $w.frame"
    pack  $w.frame.ticslabellab  -in  $w.frame.cas2_2   -side left
    pack  $w.frame.ticslabel  -in  $w.frame.cas2_2 -expand 1 -fill x -pady 0 -padx $mediumPad

    
    #Tics direction
    frame $w.frame.px  -borderwidth 0
    pack $w.frame.px  -in $w.frame -side top   -fill x -pady 0
    
    label $w.frame.xposlabel  -height 0 -text "Tics direction:" -width 0  -font $gedFont -anchor e -width $largeur
    combobox $w.frame.xpos \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curticsdir \
    -editable false \
    -command [list SelectTicsdir ] -font $gedFont
    eval $w.frame.xpos list insert end [list "left" "right"]
    
    pack $w.frame.xposlabel -in  $w.frame.px -side left
    pack $w.frame.xpos  -in  $w.frame.px  -expand 1 -fill x -pady 0 -padx $mediumPad
    
}

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both
    

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w.b -side bottom 


########### Clipping onglet #########################################
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

proc toggleSeg { frame } {
    global curseg
    ScilabEval "global ged_handle;ged_handle.tics_segment='$curseg'"

    OnOffForeground $frame $curseg
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


proc setColor {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.tics_color <> $index then ged_handle.tics_color=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.tics_color <> $index then ged_handle.tics_color=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.tics_color <> $index then ged_handle.tics_color=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.tics_color <> $index then ged_handle.tics_color=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}


proc SelectTicsStyle {w args} {
global curticsstyle
ScilabEval "global ged_handle; ged_handle.tics_style='$curticsstyle'"
}


proc setFontSize {w siz} {
    ScilabEval "global ged_handle;if ged_handle.labels_font_size <> $siz then ged_handle.labels_font_size=$siz; end;"
}


proc setFontColor {w index} {
   global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.labels_font_color <> $index then ged_handle.labels_font_color=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.labels_font_color <> $index then ged_handle.labels_font_color=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.labels_font_color <> $index then ged_handle.labels_font_color=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.labels_font_color <> $index then ged_handle.labels_font_color=$index; end;"
		
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}


#SelectXticsCoord
proc SelectXticsCoord2 { w } {
    global xticscoord
    SelectXticsCoord { w }
    SelectXticsCoord { w }
}
#SelectXticsCoord
proc SelectXticsCoord { w } {
    global xticscoord curticslabel
    ScilabEval "global ged_handle;ged_handle.xtics_coord=$xticscoord"
    ScilabEval "global ged_handle;TCL_SetVar(\"curticslabel\",sci2exp(ged_handle.tics_labels,0))"
}


#SelectYticsCoord
proc SelectYticsCoord2 { w } {
    global yticscoord
    SelectYticsCoord { w }
    SelectYticsCoord { w }
}
#SelectYticsCoord
proc SelectYticsCoord { w } {
    global yticscoord curticslabel
    ScilabEval "global ged_handle;ged_handle.ytics_coord=$yticscoord"
    ScilabEval "global ged_handle;TCL_SetVar(\"curticslabel\",sci2exp(ged_handle.tics_labels,0))"
  
}

proc SelectSubtics { w } {
    global cursubtics
    ScilabEval "global ged_handle;ged_handle.sub_tics=$cursubtics"
}

proc SelectTicsLabels { w } {
    global curticslabel
    ScilabEval "global ged_handle;ged_handle.tics_labels=$curticslabel"
 
}

proc SelectTicsdir { w args } {
    global curticsdir
    ScilabEval "global ged_handle;ged_handle.tics_direction='$curticsdir'"
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

