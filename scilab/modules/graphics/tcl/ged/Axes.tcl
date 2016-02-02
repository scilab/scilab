# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2004 - INRIA - Fabrice Leray
# Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
# Copyright (C) 2009 - DIGITEO - Pierre Lando
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
#lappend ::auto_path "$env(SCIPATH)/tcl"
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

global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
set ged_listofpref "MAIN_WINDOW_POSITION TICK_WINDOW_POSITION"
set MAIN_WINDOW_POSITION "+0+0"
set TICK_WINDOW_POSITION "+0+0"

set envSCIHOME $SCIHOME
set preffilename $envSCIHOME/.GedPreferences.tcl
catch {source $preffilename}


global fen1 fen2 fen3
global Xaxes_reverseToggle Yaxes_reverseToggle Zaxes_reverseToggle
global axes_boundsL axes_boundsU axes_boundsW axes_boundsH
global xlabel_visibility ylabel_visibility zlabel_visibility titlelabel_visibility
global SubticksEntryX SubticksEntryY SubticksEntryZ
global XautoticksToggle YautoticksToggle ZautoticksToggle
global StepEntryX StepEntryY StepEntryZ
global LOCATIONS_X LABELS_X nbticks_x
global LOCATIONS_Y LABELS_Y nbticks_y
global LOCATIONS_Z LABELS_Z nbticks_z
global LEI_x LEI_y LEI_z
global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject
global ticksNoteBook ;

global  visToggle limToggle boxToggle isoToggle gridToggle
global  curBoxState
global  xToggle yToggle zToggle red green blue color 
global  xlabel ylabel zlabel tlabel
global  curvis cubToggle viewToggle

global xlabel_fontforeground ylabel_fontforeground zlabel_fontforeground titlelabel_fontforg
global xlabel_foreground ylabel_foreground zlabel_foreground titlelabel_foreground
global xlabel_background ylabel_background zlabel_background titlelabel_background
global Xfillmode Yfillmode Zfillmode Titlefillmode
global xlabel_fontsize ylabel_fontsize zlabel_fontsize  titlelabel_fontsize
global curfontangle_x curfontangle_x2 
global curfontangle_y curfontangle_y2
global curfontangle_z curfontangle_z2
global curfontangle_title curfontangle_title2

global ncolors fcolor curthick curvis curfontsize curfontcolor
global curalpharotation curthetarotation old_curalpharotation old_curthetarotation
global curautoclear curautoscale curfillmode
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox
global curlinestyle
global Lmargins Rmargins Tmargins Bmargins

#axes_visible has changed !!!
global Xaxes_visibleToggle Yaxes_visibleToggle Zaxes_visibleToggle

global xGrid yGrid zGrid zGrid_initial

global viewToggle isoToggle cubToggle
global dbxmin dbxmax dbymin dbymax dbzmin dbzmax

global msdos

global Xlabelfontstyle Ylabelfontstyle Zlabelfontstyle TITLEfontstyle fontstyle
global RED GREEN BLUE

global hiddenAxisColor

global xauto_position yauto_position zauto_position titleauto_position
global x_position y_position z_position title_position

global xauto_rotation yauto_rotation zauto_rotation titleauto_rotation

global smallPad mediumPad #used to be 1m and 2m
global gedFont

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

# 1m
set smallPad  4
# 2m
set mediumPad 8

set NBheight 350
set NBwidth  360

set Wheight [expr $NBheight + 210]
set Wwidth  [expr $NBwidth  + 265]

#create the font we will use
set gedFont {Arial -12}
set titFont {Arial -15}

set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Axes Editor"
wm iconname $ww "AE"
wm geometry $ww [expr $Wwidth]x[expr $Wheight]$MAIN_WINDOW_POSITION
wm maxsize  $ww $Wwidth $Wheight
wm protocol $ww WM_DELETE_WINDOW "DestroyGlobals"


set topf  [frame $ww.topf]
set titf1 [TitleFrame $topf.titf1 -text "Graphic Editor" -font $gedFont]

set parent  [$titf1 getframe]
set pw1  [PanedWindow $parent.pw1 -side top]

set pane3  [$pw1 add -weight 2]


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

pack $titf1label -padx $smallPad -side left -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx $smallPad -fill both -expand yes

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
	       -width 20 -height 31 \
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
set largeurRight 12

Notebook:create $uf.n -pages {X Y Z Title Style Aspect Viewpoint} -pad 0  -height $NBheight -width $NBwidth

pack $uf.n -fill both -expand yes  -side top


########### X onglet ##############################################
###################################################################

set w [Notebook:frame $uf.n "X"]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options" -font $titFont]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options" -font $titFont]

set w [$titf1label getframe]

pack $titf1label -padx $smallPad -side top -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx $smallPad -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding


#x label
frame $w.frame.lbx -borderwidth 0
pack $w.frame.lbx  -in $w.frame -side top   -fill x -pady 0

set mycurtext $xlabel

label $w.frame.textlabel  -height 0 -text "Text:" -width 0  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.text \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable xlabel \
    -editable true \
    -command [list SelectXlabel ] -font $gedFont
eval $w.frame.text list insert end [list $mycurtext "----" "Edit data..."]
pack $w.frame.textlabel -in $w.frame.lbx  -side left
pack $w.frame.text   -in $w.frame.lbx  -expand 1 -fill x -pady 0 -padx $mediumPad

#visibility
frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 0
label $w.frame.vislablabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.vislabb  -text "on"\
    -variable xlabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitylabx $w.frame.vislabb"  -font $gedFont
OnOffForeground $w.frame.vislabb $xlabel_visibility

label $w.frame.fillmodelabel -height 0 -text "Fill mode:" -font $gedFont -anchor e -width $largeurRight
checkbutton $w.frame.fillmode  -text "on" -indicatoron 1 \
    -variable Xfillmode -onvalue "on" -offvalue "off" \
    -command "XtoggleFillmode $w.frame.fillmode" -font $gedFont
OnOffForeground $w.frame.fillmode $Xfillmode

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb -in $w.frame.vislab -side left -padx $smallPad
pack $w.frame.fillmodelabel -in $w.frame.vislab -side left
pack $w.frame.fillmode -in $w.frame.vislab -side left -fill x -padx $smallPad

#Auto position & Position
frame $w.frame.poslab -borderwidth 0
pack $w.frame.poslab  -in $w.frame -side top -fill x -pady 0
label $w.frame.poslablabel  -text "Auto position:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.poslabb  -text "on"\
    -variable xauto_position  -onvalue "on" -offvalue "off" \
    -command "toggleAutoPositionx $w.frame.poslabb"  -font $gedFont
OnOffForeground $w.frame.poslabb $xauto_position

label $w.frame.posmodelabel -height 0 -text "Position:" -font $gedFont -anchor e -width $largeurRight
entry $w.frame.posmode -relief sunken  -textvariable x_position -font $gedFont

bind  $w.frame.posmode <Return> "setPosition_x $w.frame.poslabb"
bind  $w.frame.posmode <KP_Enter> "setPosition_x $w.frame.poslabb"
bind  $w.frame.posmode <FocusOut> "setPosition_x $w.frame.poslabb"

pack $w.frame.poslablabel -in $w.frame.poslab -side left
pack $w.frame.poslabb -in $w.frame.poslab -side left -padx $smallPad
pack $w.frame.posmodelabel -in $w.frame.poslab -side left
pack $w.frame.posmode -in $w.frame.poslab -side left -fill x -padx $mediumPad


#Auto Rotation
frame $w.frame.rotlab -borderwidth 0
pack $w.frame.rotlab  -in $w.frame -side top -fill x -pady 0
label $w.frame.rotlablabel  -text "Auto rotation:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rotlabb  -text "on"\
    -variable xauto_rotation  -onvalue "on" -offvalue "off" \
    -command "toggleAutoRotationx $w.frame.rotlabb"  -font $gedFont
OnOffForeground $w.frame.rotlabb $xauto_rotation

pack $w.frame.rotlablabel -in $w.frame.rotlab -side left
pack $w.frame.rotlabb -in $w.frame.rotlab -side left -padx $smallPad

#Font Angle
frame $w.frame.font  -borderwidth 0
pack $w.frame.font  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontanglelabel -height 0 -text "Font angle:"  -font $gedFont -anchor e -width $largeur

radiobutton $w.frame.fontanglechoice0 -text "0°" -variable curfontangle_x -value 0 -command "setFontAngle_x $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice90 -text "90°" -variable curfontangle_x -value 90 -command "setFontAngle_x $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice180 -text "180°" -variable curfontangle_x -value 180 -command "setFontAngle_x $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice270 -text "270°" -variable curfontangle_x -value 270 -command "setFontAngle_x $w.frame.rotlabb" -font $gedFont

entry $w.frame.fontangle2 -relief sunken  -textvariable curfontangle_x2 -font $gedFont
bind  $w.frame.fontangle2 <Return> "setEntryFontAngle_x $w.frame.rotlabb"
bind  $w.frame.fontangle2 <KP_Enter> "setEntryFontAngle_x $w.frame.rotlabb"
bind  $w.frame.fontangle2 <FocusOut> "setEntryFontAngle_x $w.frame.rotlabb"

pack $w.frame.fontanglelabel -in $w.frame.font -side left
pack $w.frame.fontanglechoice0 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice90 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice180 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice270 -in $w.frame.font -side left -padx 0
pack $w.frame.fontangle2  -in $w.frame.font  -side left  -fill x -padx $smallPad


#Font color
frame $w.frame.col  -borderwidth 0
pack $w.frame.col  -in $w.frame -side top   -fill x -pady 0

label $w.frame.colorlabel -height 0 -text "Fore/Back colors:" -width 0 -font $gedFont -anchor e -width $largeur
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setXLabelColor $w.frame.color" -tickinterval 0   -font $gedFont

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setXBackLabelColor $w.frame.bcolor" -tickinterval 0   -font $gedFont

pack $w.frame.colorlabel  -in  $w.frame.col -side left 
pack $w.frame.color -in  $w.frame.col -side left
pack $w.frame.bcolor -in  $w.frame.col -side left -expand 1 -fill x -pady 0 -padx $smallPad

$w.frame.color set $xlabel_foreground
$w.frame.bcolor set $xlabel_background

#Font size/foreground
frame $w.frame.fontsf -borderwidth 0
pack $w.frame.fontsf -in $w.frame -side top -fill x -pady 0

label $w.frame.fontsflabel -height 0 -text "Font size/color:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
    -resolution 1.0 -command "setXFontLabelSize $w.frame.fontsize" -tickinterval 0   -font $gedFont

scale $w.frame.fontforeground -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setXFontForeground $w.frame.fontforeground" -tickinterval 0 -font $gedFont

pack $w.frame.fontsflabel -in $w.frame.fontsf -side left
pack $w.frame.fontsize  -in  $w.frame.fontsf -side left 
pack $w.frame.fontforeground  -in  $w.frame.fontsf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $xlabel_fontsize
$w.frame.fontforeground set $xlabel_fontforeground


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0   -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Xlabelfontstyle \
    -editable false \
    -command [list SelectXFontStyle ]  -font $gedFont
eval $w.frame.style list insert end [list "Monospaced" "Symbol" "Serif" "Serif Italic" "Serif Bold" "Serif Bold Italic"  "SansSerif"  "SansSerif Italic" "SansSerif Bold" "SansSerif Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 0 -padx $mediumPad

#adding 26.05.2005
set w [$titf1axes getframe]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#Xpos
frame $w.frame.px  -borderwidth 0
pack $w.frame.px  -in $w.frame -side top   -fill x -pady 0

label $w.frame.xposlabel  -height 0 -text "Location:" -width 0   -font $gedFont -anchor e -width $largeur
combobox $w.frame.xpos \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Xlabelpos \
    -editable false \
    -command [list SelectXpos ]  -font $gedFont
eval $w.frame.xpos list insert end [list "top" "middle" "bottom" "origin"]

pack $w.frame.xposlabel -in  $w.frame.px -side left
pack $w.frame.xpos  -in  $w.frame.px  -expand 1 -fill x -pady 0 -padx $mediumPad

#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x -pady 0

label $w.frame.gridcolorlabel -height 0 -text "Grid color:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
    -resolution 1.0  -variable xGrid   -tickinterval 0   -font $gedFont
$w.frame.gridcolor set $xGrid
$w.frame.gridcolor configure -command "setXGridColor $w.frame.gridcolor"

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 0 -padx $smallPad
#$w.frame.gridcolor set $xGrid

#X Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 0

label $w.frame.datalabel -text "Data bounds:"  -font $gedFont -anchor e -width $largeur
entry $w.frame.databmin -relief sunken  -textvariable dbxmin -width 10 -font $gedFont
entry $w.frame.databmax -relief sunken  -textvariable dbxmax -width 10 -font $gedFont
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x -padx $mediumPad
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x -padx $mediumPad
bind  $w.frame.databmin <Return> {setXdb}
bind  $w.frame.databmin <KP_Enter> {setXdb}
bind  $w.frame.databmin <FocusOut> {setXdb}

bind  $w.frame.databmax <Return> {setXdb} 
bind  $w.frame.databmax <KP_Enter> {setXdb}
bind  $w.frame.databmax <FocusOut> {setXdb}

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x  -pady 0

label $w.frame.scalesw.label -height 0 -text "Scale:" -width 0  -font $gedFont -anchor e -width $largeur
radiobutton $w.frame.scalesw.radioLIN -text "Lin." -variable xToggle -value "n"    -command "toggleX"  -font $gedFont
radiobutton $w.frame.scalesw.radioLOG -text "Log." -variable xToggle -value "l" 	  -command "toggleX"  -font $gedFont

set numpage 0
button $w.frame.scalesw.buttonticks -text "Ticks..." -command "Reload_and_popup $ww $numpage"  -font $gedFont

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left -padx $smallPad
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left -padx $smallPad
pack $w.frame.scalesw.buttonticks  -in  $w.frame.scalesw    -side left -expand 1 -fill x -padx $mediumPad

#Reverse axis
frame $w.frame.rev  -borderwidth 0
pack $w.frame.rev  -in $w.frame -side top   -fill x  -pady 0

label $w.frame.rev.label -height 0 -text "Reverse:" -width 0 -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rev.revvalue  -text "on"\
    -variable Xaxes_reverseToggle -onvalue "on" -offvalue "off" \
    -command "toggleReverselabx $w.frame.rev.revvalue"  -font $gedFont
OnOffForeground $w.frame.rev.revvalue $Xaxes_reverseToggle

pack $w.frame.rev.label -in $w.frame.rev -side left
pack $w.frame.rev.revvalue -in $w.frame.rev  -side left  -fill x -padx $smallPad


set w $theframe

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom

########### Y onglet ##############################################
###################################################################
set w [Notebook:frame $uf.n Y]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options" -font $titFont]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options" -font $titFont]

set w [$titf1label getframe]

pack $titf1label -padx $smallPad -side top -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx $smallPad -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding


#y label
frame $w.frame.lby -borderwidth 0
pack $w.frame.lby  -in $w.frame -side top   -fill x  -pady 0

set mycurtext $ylabel

label $w.frame.textlabel  -height 0 -text "Text:" -width 0  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.text \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable ylabel \
    -editable true \
    -command [list SelectYlabel ] -font $gedFont
eval $w.frame.text list insert end [list $mycurtext "----" "Edit data..."]
pack $w.frame.textlabel -in $w.frame.lby  -side left
pack $w.frame.text   -in $w.frame.lby  -expand 1 -fill x -pady 0 -padx $mediumPad

#visibility
frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 0
label $w.frame.vislablabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.vislabb  -text "on"\
    -variable ylabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitylaby $w.frame.vislabb"  -font $gedFont
OnOffForeground $w.frame.vislabb $ylabel_visibility

label $w.frame.fillmodelabel -height 0 -text "Fill mode:" -font $gedFont -anchor e -width $largeurRight
checkbutton $w.frame.fillmode  -text "on" -indicatoron 1 \
    -variable Yfillmode -onvalue "on" -offvalue "off" \
    -command "YtoggleFillmode $w.frame.fillmode" -font $gedFont
OnOffForeground $w.frame.fillmode $Yfillmode

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb -in $w.frame.vislab  -side left -padx $smallPad
pack $w.frame.fillmodelabel -in $w.frame.vislab -side left
pack $w.frame.fillmode -in $w.frame.vislab -side left -fill x -padx $smallPad

#Auto position & Position
frame $w.frame.poslab -borderwidth 0
pack $w.frame.poslab  -in $w.frame -side top -fill x -pady 0
label $w.frame.poslablabel  -text "Auto position:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.poslabb  -text "on"\
    -variable yauto_position  -onvalue "on" -offvalue "off" \
    -command "toggleAutoPositiony $w.frame.poslabb"  -font $gedFont
OnOffForeground $w.frame.poslabb $yauto_position

label $w.frame.posmodelabel -height 0 -text "Position:" -font $gedFont -anchor e -width $largeurRight
entry $w.frame.posmode -relief sunken  -textvariable y_position -font $gedFont

bind  $w.frame.posmode <Return> "setPosition_y $w.frame.poslabb"
bind  $w.frame.posmode <KP_Enter> "setPosition_y $w.frame.poslabb"
bind  $w.frame.posmode <FocusOut> "setPosition_y $w.frame.poslabb"

pack $w.frame.poslablabel -in $w.frame.poslab -side left
pack $w.frame.poslabb -in $w.frame.poslab -side left -padx $smallPad
pack $w.frame.posmodelabel -in $w.frame.poslab -side left
pack $w.frame.posmode -in $w.frame.poslab -side left -fill x -padx $mediumPad

#Auto Rotation
frame $w.frame.rotlab -borderwidth 0
pack $w.frame.rotlab  -in $w.frame -side top -fill x -pady 0
label $w.frame.rotlablabel  -text "Auto rotation:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rotlabb  -text "on"\
    -variable yauto_rotation  -onvalue "on" -offvalue "off" \
    -command "toggleAutoRotationy $w.frame.rotlabb"  -font $gedFont
OnOffForeground $w.frame.rotlabb $yauto_rotation

pack $w.frame.rotlablabel -in $w.frame.rotlab -side left
pack $w.frame.rotlabb -in $w.frame.rotlab -side left -padx $smallPad

#Font Angle
frame $w.frame.font  -borderwidth 0
pack $w.frame.font  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontanglelabel -height 0 -text "Font angle:" -width 0  -font $gedFont -anchor e -width $largeur

radiobutton $w.frame.fontanglechoice0 -text "0°" -variable curfontangle_y -value 0 -command "setFontAngle_y $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice90 -text "90°" -variable curfontangle_y -value 90 -command "setFontAngle_y $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice180 -text "180°" -variable curfontangle_y -value 180 -command "setFontAngle_y $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice270 -text "270°" -variable curfontangle_y -value 270 -command "setFontAngle_y $w.frame.rotlabb" -font $gedFont

entry $w.frame.fontangle2 -relief sunken  -textvariable curfontangle_y2 -font $gedFont 
bind  $w.frame.fontangle2 <Return> "setEntryFontAngle_y $w.frame.rotlabb"
bind  $w.frame.fontangle2 <KP_Enter> "setEntryFontAngle_y $w.frame.rotlabb"
bind  $w.frame.fontangle2 <FocusOut> "setEntryFontAngle_y $w.frame.rotlabb"

pack $w.frame.fontanglelabel -in $w.frame.font -side left
pack $w.frame.fontanglechoice0 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice90 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice180 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice270 -in $w.frame.font -side left -padx 0
pack $w.frame.fontangle2  -in $w.frame.font  -side left  -fill x -padx $smallPad

#Font color
frame $w.frame.col  -borderwidth 0
pack $w.frame.col  -in $w.frame -side top   -fill x  -pady 0

label $w.frame.colorlabel -height 0 -text "Fore/Back colors:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setYLabelColor $w.frame.color" -tickinterval 0   -font $gedFont

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setYBackLabelColor $w.frame.bcolor" -tickinterval 0   -font $gedFont

pack $w.frame.colorlabel  -in  $w.frame.col -side left 
pack $w.frame.color -in  $w.frame.col -side left
pack $w.frame.bcolor -in  $w.frame.col -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $ylabel_foreground
$w.frame.bcolor set $ylabel_background


#Font size/foreground
frame $w.frame.fontsf -borderwidth 0
pack $w.frame.fontsf -in $w.frame -side top -fill x -pady 0

label $w.frame.fontsflabel -height 0 -text "Font size/color:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
    -resolution 1.0 -command "setYFontLabelSize $w.frame.fontsize" -tickinterval 0   -font $gedFont

scale $w.frame.fontforeground -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setYFontForeground $w.frame.fontforeground" -tickinterval 0 -font $gedFont

pack $w.frame.fontsflabel -in $w.frame.fontsf -side left
pack $w.frame.fontsize  -in  $w.frame.fontsf -side left 
pack $w.frame.fontforeground  -in  $w.frame.fontsf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $ylabel_fontsize
$w.frame.fontforeground set $ylabel_fontforeground

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Ylabelfontstyle \
    -editable false \
    -command [list SelectYFontStyle ] -font $gedFont
eval $w.frame.style list insert end [list "Monospaced" "Symbol" "Serif" "Serif Italic" "Serif Bold" "Serif Bold Italic"  "SansSerif"  "SansSerif Italic" "SansSerif Bold" "SansSerif Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 0 -padx $mediumPad

#adding 26.05.2005
set w [$titf1axes getframe]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#Ypos
frame $w.frame.py  -borderwidth 0
pack $w.frame.py  -in $w.frame -side top   -fill x -pady 0

label $w.frame.yposlabel  -height 0 -text "Location:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.ypos \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Ylabelpos \
    -editable false \
    -command [list SelectYpos ] -font $gedFont
eval $w.frame.ypos list insert end [list "left" "middle" "right" "origin"]

pack $w.frame.yposlabel -in  $w.frame.py -side left
pack $w.frame.ypos  -in  $w.frame.py  -expand 1 -fill x -pady 0 -padx $mediumPad

#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x -pady 0

label $w.frame.gridcolorlabel -height 0 -text "Grid color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setYGridColor $w.frame.gridcolor" -tickinterval 0  -font $gedFont

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.gridcolor set $yGrid

#Y Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 0

label $w.frame.datalabel -text "Data bounds:" -font $gedFont -anchor e -width $largeur
entry $w.frame.databmin -relief sunken  -textvariable dbymin -width 10 -font $gedFont
entry $w.frame.databmax -relief sunken  -textvariable dbymax -width 10 -font $gedFont
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x -padx $mediumPad
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x -padx $mediumPad
bind  $w.frame.databmin <Return> {setYdb} 
bind  $w.frame.databmin <KP_Enter> {setYdb}
bind  $w.frame.databmin <FocusOut> {setYdb}

bind  $w.frame.databmax <Return> {setYdb} 
bind  $w.frame.databmax <KP_Enter> {setYdb}
bind  $w.frame.databmax <FocusOut> {setYdb}

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x -pady 0

label $w.frame.scalesw.label -height 0 -text "Scale:" -width 0  -font $gedFont -anchor e -width $largeur
radiobutton $w.frame.scalesw.radioLIN -text "Lin." -variable yToggle -value "n"    -command "toggleY"  -font $gedFont
radiobutton $w.frame.scalesw.radioLOG -text "Log." -variable yToggle -value "l" 	  -command "toggleY" 	    -font $gedFont

set numpage 1
button $w.frame.scalesw.buttonticks -text "Ticks..." -command "Reload_and_popup $ww $numpage" -font $gedFont

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left -padx $smallPad
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left -padx $smallPad
pack $w.frame.scalesw.buttonticks  -in  $w.frame.scalesw    -side left -expand 1 -fill x -padx $mediumPad

#Reverse axis
frame $w.frame.rev  -borderwidth 0
pack $w.frame.rev  -in $w.frame -side top   -fill x  -pady 0

label $w.frame.rev.label -height 0 -text "Reverse:" -width 0 -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rev.revvalue  -text "on"\
    -variable Yaxes_reverseToggle -onvalue "on" -offvalue "off" \
    -command "toggleReverselaby $w.frame.rev.revvalue" -font $gedFont
OnOffForeground $w.frame.rev.revvalue $Yaxes_reverseToggle

pack $w.frame.rev.label -in $w.frame.rev -side left
pack $w.frame.rev.revvalue -in $w.frame.rev  -side left  -fill x -padx $smallPad

set w $theframe

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom

########### Z onglet ##############################################
###################################################################
set w [Notebook:frame $uf.n Z]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options" -font $titFont]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options" -font $titFont]

set w [$titf1label getframe]

pack $titf1label -padx $smallPad -side top -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx $smallPad -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#z label
frame $w.frame.lbz -borderwidth 0
pack $w.frame.lbz  -in $w.frame -side top   -fill x -pady 0

set mycurtext $zlabel

label $w.frame.textlabel  -height 0 -text "Text:" -width 0  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.text \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable zlabel \
    -editable true \
    -command [list SelectZlabel ] -font $gedFont
eval $w.frame.text list insert end [list $mycurtext "----" "Edit data..."]
pack $w.frame.textlabel -in $w.frame.lbz  -side left
pack $w.frame.text   -in $w.frame.lbz  -expand 1 -fill x -pady 0 -padx $mediumPad


#visibility
frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 0
label $w.frame.vislablabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.vislabb  -text "on"\
    -variable zlabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitylabz $w.frame.vislabb"  -font $gedFont
OnOffForeground $w.frame.vislabb $zlabel_visibility

label $w.frame.fillmodelabel -height 0 -text "Fill mode:" -font $gedFont -anchor e -width $largeurRight
checkbutton $w.frame.fillmode  -text "on" -indicatoron 1 \
    -variable Zfillmode -onvalue "on" -offvalue "off" \
    -command "ZtoggleFillmode $w.frame.fillmode" -font $gedFont
OnOffForeground $w.frame.fillmode $Zfillmode

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb  -in $w.frame.vislab  -side left -padx $smallPad
pack $w.frame.fillmodelabel -in $w.frame.vislab -side left
pack $w.frame.fillmode -in $w.frame.vislab -side left -fill x -padx $smallPad

#Auto position & Position
frame $w.frame.poslab -borderwidth 0
pack $w.frame.poslab  -in $w.frame -side top -fill x -pady 0
label $w.frame.poslablabel  -text "Auto position:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.poslabb  -text "on"\
    -variable zauto_position  -onvalue "on" -offvalue "off" \
    -command "toggleAutoPositionz $w.frame.poslabb"  -font $gedFont
OnOffForeground $w.frame.poslabb $zauto_position

label $w.frame.posmodelabel -height 0 -text "Position:" -font $gedFont -anchor e -width $largeurRight
entry $w.frame.posmode -relief sunken  -textvariable z_position -font $gedFont

bind  $w.frame.posmode <Return> "setPosition_z $w.frame.poslabb"
bind  $w.frame.posmode <KP_Enter> "setPosition_z $w.frame.poslabb"
bind  $w.frame.posmode <FocusOut> "setPosition_z $w.frame.poslabb"

pack $w.frame.poslablabel -in $w.frame.poslab -side left
pack $w.frame.poslabb -in $w.frame.poslab -side left -padx $smallPad
pack $w.frame.posmodelabel -in $w.frame.poslab -side left
pack $w.frame.posmode -in $w.frame.poslab -side left -fill x -padx $mediumPad

#Auto Rotation
frame $w.frame.rotlab -borderwidth 0
pack $w.frame.rotlab  -in $w.frame -side top -fill x -pady 0
label $w.frame.rotlablabel  -text "Auto rotation:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rotlabb  -text "on"\
    -variable zauto_rotation  -onvalue "on" -offvalue "off" \
    -command "toggleAutoRotationz $w.frame.rotlabb"  -font $gedFont
OnOffForeground $w.frame.rotlabb $zauto_rotation

pack $w.frame.rotlablabel -in $w.frame.rotlab -side left
pack $w.frame.rotlabb -in $w.frame.rotlab -side left -padx $smallPad

#Font Angle
frame $w.frame.font  -borderwidth 0
pack $w.frame.font  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontanglelabel -height 0 -text "Font angle:" -width 0  -font $gedFont -anchor e -width $largeur

radiobutton $w.frame.fontanglechoice0 -text "0°" -variable curfontangle_z -value 0 -command "setFontAngle_z $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice90 -text "90°" -variable curfontangle_z -value 90 -command "setFontAngle_z $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice180 -text "180°" -variable curfontangle_z -value 180 -command "setFontAngle_z $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice270 -text "270°" -variable curfontangle_z -value 270 -command "setFontAngle_z $w.frame.rotlabb" -font $gedFont

entry $w.frame.fontangle2 -relief sunken  -textvariable curfontangle_z2 -font $gedFont
bind  $w.frame.fontangle2 <Return> "setEntryFontAngle_z $w.frame.rotlabb"
bind  $w.frame.fontangle2 <KP_Enter> "setEntryFontAngle_z $w.frame.rotlabb"
bind  $w.frame.fontangle2 <FocusOut> "setEntryFontAngle_z $w.frame.rotlabb"

pack $w.frame.fontanglelabel -in $w.frame.font -side left
pack $w.frame.fontanglechoice0 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice90 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice180 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice270 -in $w.frame.font -side left -padx 0
pack $w.frame.fontangle2  -in $w.frame.font  -side left  -fill x -padx $smallPad

#Font color
frame $w.frame.col  -borderwidth 0
pack $w.frame.col  -in $w.frame -side top   -fill x -pady 0

label $w.frame.colorlabel -height 0 -text "Fore/Back colors:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setZFontLabelColor $w.frame.color" -tickinterval 0   -font $gedFont

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setZBackLabelColor $w.frame.bcolor" -tickinterval 0   -font $gedFont

pack $w.frame.colorlabel  -in  $w.frame.col -side left 
pack $w.frame.color -in  $w.frame.col -side left
pack $w.frame.bcolor -in  $w.frame.col -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $zlabel_foreground
$w.frame.bcolor set $zlabel_background


#Font size/foreground
frame $w.frame.fontsf -borderwidth 0
pack $w.frame.fontsf -in $w.frame -side top -fill x -pady 0

label $w.frame.fontsflabel -height 0 -text "Font size/color:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
    -resolution 1.0 -command "setZFontLabelSize $w.frame.fontsize" -tickinterval 0   -font $gedFont

scale $w.frame.fontforeground -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setZFontForeground $w.frame.fontforeground" -tickinterval 0 -font $gedFont

pack $w.frame.fontsflabel -in $w.frame.fontsf -side left
pack $w.frame.fontsize  -in  $w.frame.fontsf -side left 
pack $w.frame.fontforeground  -in  $w.frame.fontsf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $zlabel_fontsize
$w.frame.fontforeground set $zlabel_fontforeground


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Zlabelfontstyle \
    -editable false \
    -command [list SelectZFontStyle ] -font $gedFont
eval $w.frame.style list insert end [list "Monospaced" "Symbol" "Serif" "Serif Italic" "Serif Bold" "Serif Bold Italic"  "SansSerif"  "SansSerif Italic" "SansSerif Bold" "SansSerif Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 0 -padx $mediumPad

#adding 26.05.2005
set w [$titf1axes getframe]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#Zpos
# frame $w.frame.pz  -borderwidth 0
# pack $w.frame.pz  -in $w.frame -side top   -fill x

# label $w.frame.zposlabel  -height 0 -text "Location:" -width 0 -font $gedFont -anchor e -width $largeur
# combobox $w.frame.zpos \
#     -borderwidth 1 \
#     -highlightthickness 1 \
#     -maxheight 0 \
#     -width 3 \
#     -textvariable Zlabelpos \
#     -editable false \
#     -command [list SelectZpos ]
# eval $w.frame.zpos list insert end [list "left" "middle" "right"]

# pack $w.frame.zposlabel -in  $w.frame.pz -side left
# pack $w.frame.zpos  -in  $w.frame.pz -fill x


#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x -pady 0

label $w.frame.gridcolorlabel -height 0 -text "Grid color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setZGridColor $w.frame.gridcolor" -tickinterval 0  -font $gedFont

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.gridcolor set $zGrid

#Z Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 0

label $w.frame.datalabel -text "Data bounds:" -font $gedFont -anchor e -width $largeur
entry $w.frame.databmin -relief sunken  -textvariable dbzmin -width 10 -font $gedFont
entry $w.frame.databmax -relief sunken  -textvariable dbzmax -width 10 -font $gedFont
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x -padx $mediumPad
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x -padx $mediumPad
bind  $w.frame.databmin <Return> {setZdb} 
bind  $w.frame.databmin <KP_Enter> {setZdb}
bind  $w.frame.databmin <FocusOut> {setZdb}

bind  $w.frame.databmax <Return> {setZdb} 
bind  $w.frame.databmax <KP_Enter> {setZdb}
bind  $w.frame.databmax <FocusOut> {setZdb}

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x -pady 0

label $w.frame.scalesw.label -height 0 -text "Scale:" -width 0  -font $gedFont -anchor e -width $largeur
radiobutton $w.frame.scalesw.radioLIN -text "Lin." -variable zToggle -value "n" -command "toggleZ"  -font $gedFont
radiobutton $w.frame.scalesw.radioLOG -text "Log." -variable zToggle -value "l"	  -command "toggleZ" -font $gedFont

set numpage 2
button $w.frame.scalesw.buttonticks -text "Ticks..." -command "Reload_and_popup $ww $numpage" -font $gedFont


pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left -padx $smallPad
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left -padx $smallPad
pack $w.frame.scalesw.buttonticks  -in  $w.frame.scalesw   -side left -expand 1 -fill x -padx $mediumPad

#Reverse axis
frame $w.frame.rev  -borderwidth 0
pack $w.frame.rev  -in $w.frame -side top   -fill x  -pady 0

label $w.frame.rev.label -height 0 -text "Reverse:" -width 0 -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rev.revvalue  -text "on"\
    -variable Zaxes_reverseToggle -onvalue "on" -offvalue "off" \
    -command "toggleReverselabz $w.frame.rev.revvalue" -font $gedFont
OnOffForeground $w.frame.rev.revvalue $Zaxes_reverseToggle

pack $w.frame.rev.label -in $w.frame.rev -side left
pack $w.frame.rev.revvalue -in $w.frame.rev  -side left  -fill x -padx $smallPad

set w $theframe

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom

########### Title onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Title]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options" -font $titFont]
#set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options" -font $titFont]

set w [$titf1label getframe]

pack $titf1label -padx $smallPad -side top -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx $smallPad -fill both -expand yes


frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#title text
frame $w.frame.lbtitle -borderwidth 0
pack $w.frame.lbtitle  -in $w.frame -side top   -fill x -pady 0

set mycurtext $tlabel

label $w.frame.textlabel  -height 0 -text "Text:" -width 0  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.text \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable tlabel \
    -editable true \
    -command [list SelectTextTitle ] -font $gedFont
eval $w.frame.text list insert end [list $mycurtext "----" "Edit data..."]
pack $w.frame.textlabel -in $w.frame.lbtitle  -side left
pack $w.frame.text   -in $w.frame.lbtitle  -expand 1 -fill x -pady 0 -padx $mediumPad

#visibility for title
frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 0
label $w.frame.vislablabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.vislabb  -text "on"\
    -variable titlelabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitytitle $w.frame.vislabb" -font $gedFont
OnOffForeground $w.frame.vislabb $titlelabel_visibility

label $w.frame.fillmodelabel -height 0 -text "Fill mode:" -font $gedFont -anchor e -width $largeurRight
checkbutton $w.frame.fillmode  -text "on" -indicatoron 1 \
    -variable Titlefillmode -onvalue "on" -offvalue "off" \
    -command "TitletoggleFillmode $w.frame.fillmode" -font $gedFont
OnOffForeground $w.frame.fillmode $Titlefillmode

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb  -in $w.frame.vislab  -side left -padx $smallPad
pack $w.frame.fillmodelabel -in $w.frame.vislab -side left
pack $w.frame.fillmode -in $w.frame.vislab -side left -fill x -padx $smallPad

#Auto position & Position
frame $w.frame.poslab -borderwidth 0
pack $w.frame.poslab  -in $w.frame -side top -fill x -pady 0
label $w.frame.poslablabel  -text "Auto position:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.poslabb  -text "on"\
    -variable titleauto_position  -onvalue "on" -offvalue "off" \
    -command "toggleAutoPositiontitle $w.frame.poslabb"  -font $gedFont
OnOffForeground $w.frame.poslabb $titleauto_position

label $w.frame.posmodelabel -height 0 -text "Position:" -font $gedFont -anchor e -width $largeurRight
entry $w.frame.posmode -relief sunken  -textvariable title_position -font $gedFont

bind  $w.frame.posmode <Return> "setPosition_title $w.frame.poslabb"
bind  $w.frame.posmode <KP_Enter> "setPosition_title $w.frame.poslabb"
bind  $w.frame.posmode <FocusOut> "setPosition_title $w.frame.poslabb"

pack $w.frame.poslablabel -in $w.frame.poslab -side left
pack $w.frame.poslabb -in $w.frame.poslab -side left -padx $smallPad
pack $w.frame.posmodelabel -in $w.frame.poslab -side left
pack $w.frame.posmode -in $w.frame.poslab -side left -fill x -padx $mediumPad


#Auto Rotation
frame $w.frame.rotlab -borderwidth 0
pack $w.frame.rotlab  -in $w.frame -side top -fill x -pady 0
label $w.frame.rotlablabel  -text "Auto rotation:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.rotlabb  -text "on"\
    -variable titleauto_rotation  -onvalue "on" -offvalue "off" \
    -command "toggleAutoRotationtitle $w.frame.rotlabb"  -font $gedFont
OnOffForeground $w.frame.rotlabb $titleauto_rotation

pack $w.frame.rotlablabel -in $w.frame.rotlab -side left
pack $w.frame.rotlabb -in $w.frame.rotlab -side left -padx $smallPad

#Font Angle
frame $w.frame.font  -borderwidth 0
pack $w.frame.font  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontanglelabel -height 0 -text "Font angle:" -width 0  -font $gedFont -anchor e -width $largeur

radiobutton $w.frame.fontanglechoice0 -text "0°" -variable curfontangle_title -value 0 -command "setFontAngle_title $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice90 -text "90°" -variable curfontangle_title -value 90 -command "setFontAngle_title $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice180 -text "180°" -variable curfontangle_title -value 180 -command "setFontAngle_title $w.frame.rotlabb" -font $gedFont
radiobutton $w.frame.fontanglechoice270 -text "270°" -variable curfontangle_title -value 270 -command "setFontAngle_title $w.frame.rotlabb" -font $gedFont

entry $w.frame.fontangle2 -relief sunken  -textvariable curfontangle_title2 -font $gedFont
bind  $w.frame.fontangle2 <Return> "setEntryFontAngle_title $w.frame.rotlabb"
bind  $w.frame.fontangle2 <KP_Enter> "setEntryFontAngle_title $w.frame.rotlabb"
bind  $w.frame.fontangle2 <FocusOut> "setEntryFontAngle_title $w.frame.rotlabb"

pack $w.frame.fontanglelabel -in $w.frame.font -side left
pack $w.frame.fontanglechoice0 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice90 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice180 -in $w.frame.font -side left -padx 0
pack $w.frame.fontanglechoice270 -in $w.frame.font -side left -padx 0
pack $w.frame.fontangle2  -in $w.frame.font  -side left  -fill x -padx $smallPad

#Font color
frame $w.frame.col  -borderwidth 0
pack $w.frame.col  -in $w.frame -side top   -fill x -pady 0

label $w.frame.colorlabel -height 0 -text "Fore/Back colors:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setTitleFontLabelColor $w.frame.color" -tickinterval 0   -font $gedFont

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setTitleBackLabelColor $w.frame.bcolor" -tickinterval 0   -font $gedFont

pack $w.frame.colorlabel  -in  $w.frame.col -side left 
pack $w.frame.color -in  $w.frame.col -side left
pack $w.frame.bcolor -in  $w.frame.col -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $titlelabel_foreground
$w.frame.bcolor set $titlelabel_background

#Font size/foreground
frame $w.frame.fontsf -borderwidth 0
pack $w.frame.fontsf -in $w.frame -side top -fill x -pady 0

label $w.frame.fontsflabel -height 0 -text "Font size/color:" -width 0   -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
    -resolution 1.0 -command "setTitleFontLabelSize $w.frame.fontsize" -tickinterval 0   -font $gedFont

scale $w.frame.fontforeground -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setTitleFontForeground $w.frame.fontforeground" -tickinterval 0 -font $gedFont

pack $w.frame.fontsflabel -in $w.frame.fontsf -side left
pack $w.frame.fontsize  -in  $w.frame.fontsf -side left 
pack $w.frame.fontforeground  -in  $w.frame.fontsf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $titlelabel_fontsize
$w.frame.fontforeground set $titlelabel_fontforg

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable TITLEfontstyle \
    -editable false \
    -command [list SelectTitleFontStyle] -font $gedFont
eval $w.frame.style list insert end [list "Monospaced" "Symbol" "Serif" "Serif Italic" "Serif Bold" "Serif Bold Italic"  "SansSerif"  "SansSerif Italic" "SansSerif Bold" "SansSerif Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -expand 1 -fill x -pady 0 -padx $mediumPad

set w $theframe

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom


########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x -pady 0
label $w.frame.vislabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib" -font $gedFont
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left  -fill x -pady 0 -padx $smallPad

#fill mode
# frame $w.frame.mod -borderwidth 0
# pack $w.frame.mod  -in $w.frame  -side top -fill x -pady $mediumPad
# label $w.frame.modlabel  -text "  Fill mode:  "
# checkbutton $w.frame.modib  -textvariable curfillmode \
#     -variable curfillmode  -onvalue "on" -offvalue "off" \
#     -command "toggleFill" 
# pack $w.frame.modlabel -in $w.frame.mod  -side left
# pack $w.frame.modib  -in $w.frame.mod    -side left  -fill x -pady $mediumPad -padx $mediumPad

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable fontstyle \
    -editable false \
    -command [list SelectFontStyle] -font $gedFont
eval $w.frame.style list insert end [list "Monospaced" "Symbol" "Serif" "Serif Italic" "Serif Bold" "Serif Bold Italic"  "SansSerif"  "SansSerif Italic" "SansSerif Bold" "SansSerif Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 0 -padx $mediumPad


#FontColor scale
frame $w.frame.fontclr  -borderwidth 0
pack $w.frame.fontclr   -in $w.frame -side top  -fill x -pady 0
label $w.frame.fontcolorlabel -height 0 -text "Font color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0  -font $gedFont

pack $w.frame.fontcolorlabel -in $w.frame.fontclr -side left
pack $w.frame.fontcolor -in  $w.frame.fontclr -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontcolor set $curfontcolor

#Fontsize scale
frame $w.frame.fontssz  -borderwidth 0
pack $w.frame.fontssz  -in $w.frame    -side top -fill x -pady 0

label $w.frame.fontsizelabel -height 0 -text "Font size:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal  -from 0 -to 6 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0 -font $gedFont
pack $w.frame.fontsizelabel  -in $w.frame.fontssz -side left
pack $w.frame.fontsize -in $w.frame.fontssz   -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $curfontsize


#Color scale foregound
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x -pady 0
label $w.frame.fcolorlabel -height 0 -text "Fore. color:" -width 0  -font $gedFont -anchor e -width $largeur

scale $w.frame.fcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setForeColor $w.frame.fcolor" -tickinterval 0  -font $gedFont

pack $w.frame.fcolorlabel -in $w.frame.clrf -side left
pack $w.frame.fcolor -in  $w.frame.clrf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fcolor set $fcolor


#Color scale background
frame $w.frame.clrb  -borderwidth 0
pack $w.frame.clrb  -in $w.frame -side top  -fill x -pady 0
label $w.frame.bcolorlabel -height 0 -text "Back. color: " -width 0  -font $gedFont -anchor e -width $largeur

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackColor $w.frame.bcolor" -tickinterval 0  -font $gedFont

pack $w.frame.bcolorlabel -in $w.frame.clrb -side left
pack $w.frame.bcolor -in  $w.frame.clrb -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.bcolor set $bcolor


#Color scale hiddencolor
frame $w.frame.clrh  -borderwidth 0
pack $w.frame.clrh  -in $w.frame -side top  -fill x -pady 0
label $w.frame.hcolorlabel -height 0 -text "Hidden axis color:" -width 0  -font $gedFont -anchor e -width $largeur

scale $w.frame.hcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setHiddenAxisColor $w.frame.hcolor" -tickinterval 0  -font $gedFont

pack $w.frame.hcolorlabel -in $w.frame.clrh -side left
pack $w.frame.hcolor -in  $w.frame.clrh -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.hcolor set $hiddenAxisColor

#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x -pady 0

label $w.frame.scalelabel -height 0 -text "Thickness:"  -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 30 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0 -font $gedFont
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack  $w.frame.thickness  -in $w.frame.thk -expand yes -fill x  -padx $smallPad
$w.frame.thickness set $curthick


#Line Style
frame $w.frame.linest  -borderwidth 0
pack $w.frame.linest  -in $w.frame  -side top  -fill x

label $w.frame.linestylelabel  -height 0 -text "Line style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.linestyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ] -font $gedFont
eval $w.frame.linestyle list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]
pack $w.frame.linestylelabel -in $w.frame.linest   -side left
pack $w.frame.linestyle   -in $w.frame.linest   -expand 1 -fill x -pady 0 -padx $mediumPad


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom

########### Aspect onglet #########################################
###################################################################
set w [Notebook:frame $uf.n Aspect]

set largeurArray 12

frame $w.frame -borderwidth 0
pack $w.frame -side top   -fill x

frame $w.frame.line1 -borderwidth 0
pack $w.frame.line1 -in $w.frame -side top   -fill x

frame $w.frame.line2 -borderwidth 0
pack $w.frame.line2 -in $w.frame -side top   -fill x

frame $w.frame.line3 -borderwidth 0
pack $w.frame.line3 -in $w.frame -side top   -fill x

#auto clear
label $w.frame.clearlabel  -text "Auto clear:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.clearib  -text "on"\
    -variable curautoclear  -onvalue "on" -offvalue "off" \
    -command "toggleClear $w.frame.clearib" -font $gedFont
OnOffForeground $w.frame.clearib $curautoclear

pack $w.frame.clearlabel -in $w.frame.line1   -side left
pack $w.frame.clearib  -in $w.frame.line1    -side left -fill x -pady 0 -padx $smallPad

#Isoview
label $w.frame.isolabel  -text " Isoview:" -font $gedFont -anchor e -width $largeurArray
checkbutton $w.frame.isob  -text "on"\
    -variable isoToggle  -onvalue "on" -offvalue "off" \
    -command "toggleIsoview $w.frame.isob" -font $gedFont
OnOffForeground $w.frame.isob $isoToggle

pack $w.frame.isolabel -in $w.frame.line1 -side left
pack $w.frame.isob  -in $w.frame.line1  -side left -fill x -pady 0 -padx $smallPad


#auto scal
label $w.frame.scallabel  -text "Auto scale:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.scalib  -text "on"\
    -variable curautoscale  -onvalue "on" -offvalue "off" \
    -command "toggleScale $w.frame.scalib" -font $gedFont
OnOffForeground $w.frame.scalib $curautoscale

pack $w.frame.scallabel -in $w.frame.line2  -side left
pack $w.frame.scalib  -in $w.frame.line2    -side left  -fill x -pady 0 -padx $smallPad

#Tight Limits
label $w.frame.limitlabel -text "Tight limits:" -font $gedFont -anchor e -width $largeurArray
checkbutton $w.frame.limit  -text "on"\
    -variable limToggle  -onvalue "on" -offvalue "off" \
    -command "toggleLimits $w.frame.limit" -font $gedFont
OnOffForeground $w.frame.limit $limToggle

pack $w.frame.limitlabel -in $w.frame.line2 -side left
pack $w.frame.limit  -in $w.frame.line2  -side left  -fill x -pady 0 -padx $smallPad

#box
# label $w.frame.boxlabel  -text "Boxed:" -font $gedFont -anchor e -width $largeur
# checkbutton $w.frame.box  -text "on"\
#     -variable boxToggle  -onvalue "on" -offvalue "off" \
#     -command "toggleBox $w.frame.box" -font $gedFont
# OnOffForeground $w.frame.box $boxToggle

# pack $w.frame.boxlabel -in $w.frame.line3  -side left
# pack $w.frame.box  -in $w.frame.line3   -side left -fill x -pady 0 -padx $smallPad

#cubescaling
label $w.frame.cublabel  -text "Cube scaling:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.cubb  -text "on"\
    -variable cubToggle  -onvalue "on" -offvalue "off" \
    -command "toggleCubview $w.frame.cubb" -font $gedFont
OnOffForeground $w.frame.cubb $cubToggle

pack $w.frame.cublabel -in $w.frame.line3 -side left
pack $w.frame.cubb  -in $w.frame.line3  -side left -fill x -pady 0 -padx $smallPad



#box
label $w.frame.boxlabel -text "Boxed:" -font $gedFont -anchor e -width $largeurArray
combobox $w.frame.box \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 10 \
    -textvariable curBoxState\
    -editable false \
    -command [list selectBoxState ] -font $gedFont
eval $w.frame.box list insert end [list "off" "hidden_axes" "back_half" "on"]
pack $w.frame.boxlabel -in $w.frame.line3   -side left
pack $w.frame.box -in $w.frame.line3  -side left -fill x -pady 0 -padx $smallPad


#Clip state
frame $w.frame.clpstat  -borderwidth 0
pack $w.frame.clpstat  -in $w.frame -side top -fill x -pady 0

label $w.frame.cliplabel  -height 0 -text "Clip state:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.clip \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 8 \
    -textvariable curclipstate\
    -editable false \
    -command [list SelectClipState ] -font $gedFont
eval $w.frame.clip list insert end [list "on" "off" "clipgrf"]

pack $w.frame.cliplabel -in $w.frame.clpstat   -side left
pack $w.frame.clip -in $w.frame.clpstat  -side left -pady 0 -padx $mediumPad

set largeur 8

#clip box
frame $w.frame.lb1 -borderwidth 0
pack $w.frame.lb1  -in $w.frame -side top   -fill x
label $w.frame.labelul -text "Clip box : upper-left point coordinates" -font $gedFont
pack $w.frame.labelul -in  $w.frame.lb1 -side left

frame $w.frame.lb2 -borderwidth 0
pack $w.frame.lb2  -in $w.frame -side top   -fill x

frame $w.frame.lb21 -borderwidth 0
pack $w.frame.lb21  -in $w.frame -side top   -fill x

frame $w.frame.lb22 -borderwidth 0
pack $w.frame.lb22  -in $w.frame -side top   -fill x

label $w.frame.labelx -text "X:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.datax -relief sunken  -textvariable Xclipbox -width 10  -font $gedFont
label $w.frame.labely -text "Y:" -font $gedFont -anchor e -width $largeur
entry $w.frame.datay -relief sunken  -textvariable Yclipbox -width 10  -font $gedFont

pack $w.frame.labelx  -in  $w.frame.lb2 -side left 
pack $w.frame.datax   -in  $w.frame.lb2 -side left -pady 0 -padx $mediumPad
pack $w.frame.labely  -in  $w.frame.lb21 -side left 
pack $w.frame.datay   -in  $w.frame.lb21 -side left -pady 0 -padx $mediumPad
bind  $w.frame.datax <Return> "SelectClipBox $w.frame"
bind  $w.frame.datay <Return> "SelectClipBox $w.frame"
bind  $w.frame.datax <KP_Enter> "SelectClipBox $w.frame"
bind  $w.frame.datay <KP_Enter> "SelectClipBox $w.frame"

#----------------------------#
frame $w.frame.lb3 -borderwidth 0
pack $w.frame.lb3  -in $w.frame -side top   -fill x
label $w.frame.labelwh -text "Clip box : width and height" -font $gedFont
pack $w.frame.labelwh -in  $w.frame.lb3 -side left

frame $w.frame.lb4 -borderwidth 0
pack $w.frame.lb4  -in $w.frame -side top   -fill x

frame $w.frame.lb41 -borderwidth 0
pack $w.frame.lb41  -in $w.frame -side top   -fill x

label $w.frame.labelw -text "W:" -font $gedFont -anchor e -width $largeur
entry $w.frame.dataw -relief sunken  -textvariable Wclipbox -width 10  -font $gedFont
label $w.frame.labelh -text "H:" -font $gedFont -anchor e -width $largeur
entry $w.frame.datah -relief sunken  -textvariable Hclipbox -width 10  -font $gedFont

pack $w.frame.labelw  -in  $w.frame.lb4 -side left 
pack $w.frame.dataw   -in  $w.frame.lb4 -side left -pady 0 -padx $mediumPad
pack $w.frame.labelh  -in  $w.frame.lb41 -side left 
pack $w.frame.datah   -in  $w.frame.lb41 -side left -pady 0 -padx $mediumPad
bind  $w.frame.dataw <Return> "SelectClipBox $w.frame"
bind  $w.frame.datah <Return> "SelectClipBox $w.frame"
bind  $w.frame.dataw <KP_Enter> "SelectClipBox $w.frame"
bind  $w.frame.datah <KP_Enter> "SelectClipBox $w.frame"



#margins
frame $w.frame.marg1 -borderwidth 0
pack $w.frame.marg1  -in $w.frame -side top   -fill x
label $w.frame.labelmarg -text "Margins:" -font $gedFont
label $w.frame.labelaxesbounds -text "\t\tAxes bounds:" -font $gedFont
pack $w.frame.labelmarg -in  $w.frame.marg1 -side left
pack $w.frame.labelaxesbounds -in  $w.frame.marg1 -side left -pady 0 -padx $mediumPad -fill x

frame $w.frame.marg2 -borderwidth 0
pack $w.frame.marg2  -in $w.frame -side top   -fill x

frame $w.frame.marg21 -borderwidth 0
pack $w.frame.marg21  -in $w.frame -side top   -fill x

frame $w.frame.marg4 -borderwidth 0
pack $w.frame.marg4  -in $w.frame -side top   -fill x

frame $w.frame.marg41 -borderwidth 0
pack $w.frame.marg41  -in $w.frame -side top   -fill x

label $w.frame.labelleft -text  "Left:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.datamargl -relief sunken  -textvariable Lmargins -width 10 -font $gedFont
label $w.frame.labelleftaxesbounds -text  "Left:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.dataleftaxesbounds -relief sunken  -textvariable axes_boundsL -width 10 -font $gedFont

label $w.frame.labelright -text "Right:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.datamargr -relief sunken  -textvariable Rmargins -width 10 -font $gedFont
label $w.frame.labelupaxesbounds -text  "Up:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.dataupaxesbounds -relief sunken  -textvariable axes_boundsU -width 10 -font $gedFont

pack $w.frame.labelleft  -in  $w.frame.marg2 -side left
pack $w.frame.datamargl  -in  $w.frame.marg2 -side left -padx $mediumPad
pack $w.frame.labelleftaxesbounds -in  $w.frame.marg2 -side left
pack $w.frame.dataleftaxesbounds -in  $w.frame.marg2 -side left  -fill x -pady 0 -padx $mediumPad
pack $w.frame.labelright  -in  $w.frame.marg21 -side left
pack $w.frame.datamargr   -in  $w.frame.marg21 -side left -padx $mediumPad
pack $w.frame.labelupaxesbounds  -in  $w.frame.marg21 -side left
pack $w.frame.dataupaxesbounds -in  $w.frame.marg21 -side left -fill x -pady 0 -padx $mediumPad 
bind  $w.frame.datamargl <Return> {SelectMargins}
bind  $w.frame.datamargr <Return> {SelectMargins}
bind  $w.frame.dataleftaxesbounds <Return> {SelectAxesbounds}
bind  $w.frame.dataupaxesbounds   <Return> {SelectAxesbounds}
bind  $w.frame.datamargl <KP_Enter> {SelectMargins}
bind  $w.frame.datamargr <KP_Enter> {SelectMargins}
bind  $w.frame.dataleftaxesbounds <KP_Enter> {SelectAxesbounds}
bind  $w.frame.dataupaxesbounds   <KP_Enter> {SelectAxesbounds}


label $w.frame.labeltop -text "Top:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.datamargt -relief sunken  -textvariable Tmargins -width 10 -font $gedFont
label $w.frame.labelwidthaxesbounds -text "Width:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.datawidthaxesbounds -relief sunken  -textvariable axes_boundsW -width 10 -font $gedFont

label $w.frame.labelbottom -text "Bottom:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.datamargb -relief sunken  -textvariable Bmargins -width 10 -font $gedFont
label $w.frame.labelheightaxesbounds -text "Height:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.dataheightaxesbounds -relief sunken  -textvariable axes_boundsH -width 10 -font $gedFont

pack $w.frame.labeltop  -in  $w.frame.marg4  -side left
pack $w.frame.datamargt  -in  $w.frame.marg4  -side left -padx $mediumPad
pack $w.frame.labelwidthaxesbounds   -in  $w.frame.marg4  -side left
pack $w.frame.datawidthaxesbounds -in  $w.frame.marg4  -side left -fill x -pady 0 -padx $mediumPad
pack $w.frame.labelbottom   -in  $w.frame.marg41 -side left
pack $w.frame.datamargb  -in  $w.frame.marg41 -side left  -padx $mediumPad
pack $w.frame.labelheightaxesbounds  -in  $w.frame.marg41 -side left
pack $w.frame.dataheightaxesbounds -in  $w.frame.marg41 -side left -fill x -pady 0 -padx $mediumPad
bind  $w.frame.datamargt <Return> {SelectMargins}
bind  $w.frame.datamargb <Return> {SelectMargins}
bind  $w.frame.datawidthaxesbounds   <Return> {SelectAxesbounds}
bind  $w.frame.dataheightaxesbounds  <Return> {SelectAxesbounds}
bind  $w.frame.datamargt <KP_Enter> {SelectMargins}
bind  $w.frame.datamargb <KP_Enter> {SelectMargins}
bind  $w.frame.datawidthaxesbounds   <KP_Enter> {SelectAxesbounds}
bind  $w.frame.dataheightaxesbounds  <KP_Enter> {SelectAxesbounds}




frame $w.frame.warning
label $w.frame.mesgwarning  -justify left -textvariable letext -font $gedFont
$w.frame.mesgwarning config -foreground red
pack $w.frame.mesgwarning -in $w.frame.warning
pack $w.frame.warning -in $w.frame

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom

########### Viewpoint onglet ######################################
###################################################################
set w [Notebook:frame $uf.n Viewpoint]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#update largeur
set largeur 13

#view
frame $w.frame.view -borderwidth 0
pack $w.frame.view  -in $w.frame -side top -fill x -pady 0
label $w.frame.viewlabel  -text "View:" -font $gedFont -anchor e -width $largeur

radiobutton $w.frame.view.radio0 -text "2d" -variable viewToggle -value "2d" -command "toggleView" -font $gedFont
radiobutton $w.frame.view.radio1 -text "3d" -variable viewToggle -value "3d" -command "toggleView" -font $gedFont

pack $w.frame.viewlabel -in $w.frame.view -side left
pack $w.frame.view.radio0 -in  $w.frame.view -side left -padx $smallPad
pack $w.frame.view.radio1 -in  $w.frame.view -side left -padx $smallPad

#rotation_angles
frame $w.frame.rotang -borderwidth 0
pack $w.frame.rotang  -in $w.frame -side top -fill x -pady 0
label $w.frame.rotanglabel  -text "Rotation angles:" -font $gedFont -anchor e -width $largeur
entry $w.frame.rotalpha -relief sunken  -textvariable curalpharotation -width 10 -font $gedFont
entry $w.frame.rottheta -relief sunken  -textvariable curthetarotation -width 10 -font $gedFont
pack $w.frame.rotanglabel  -in $w.frame.rotang -side left  -pady 0
pack $w.frame.rotalpha $w.frame.rottheta -in $w.frame.rotang  -in $w.frame.rotang -side left  -pady 0 -padx $mediumPad
bind  $w.frame.rotalpha <Return> {setAlphaAngle}
bind  $w.frame.rottheta <Return> {setThetaAngle} 
bind  $w.frame.rotalpha <KP_Enter> {setAlphaAngle} 
bind  $w.frame.rottheta <KP_Enter> {setThetaAngle} 
bind  $w.frame.rotalpha <FocusOut> {setAlphaAngle} 
bind  $w.frame.rottheta <FocusOut> {setThetaAngle} 




#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals" -font $gedFont
pack $w.b -side bottom



pack $sw -fill both -expand yes
pack $pw1 -fill both -expand yes
pack $titf1 -padx $smallPad -side left -fill both -expand yes
pack $topf -fill both -pady 0 -expand yes


# les proc associes:

proc SelectAxesbounds {} {
    global axes_boundsL axes_boundsU axes_boundsW axes_boundsH
    
    if { ($axes_boundsL == "") || ($axes_boundsU == "") ||($axes_boundsW == "") ||($axes_boundsH == "") } {
	tk_messageBox -icon error -type ok -title "Axes bounds selection failed" -message "You must fill in the 4 fields"
	return
    }
    
    ScilabEval "global ged_handle;ged_handle.axes_bounds=\[$axes_boundsL $axes_boundsU $axes_boundsW $axes_boundsH \]" "seq"
}

proc SelectMargins {} {
global Lmargins Rmargins Tmargins Bmargins
    
    if { ($Lmargins == "") || ($Rmargins == "") ||($Tmargins == "") ||($Bmargins == "") } {
	tk_messageBox -icon error -type ok -title "Margins selection failed" -message "You must fill in the 4 fields"
	return
    }
ScilabEval "global ged_handle;ged_handle.margins=\[$Lmargins $Rmargins $Tmargins $Bmargins \]"
}
proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
}

proc SelectClipBox {} {
    global Xclipbox Yclipbox Wclipbox Hclipbox curclipstate
    if { ($Xclipbox == "") || ($Yclipbox == "") ||($Wclipbox == "") ||($Hclipbox == "") } {
	tk_messageBox -icon error -type ok -title "Clip box selection failed" -message "You must fill in the 4 fields"
	return
    }
    ScilabEval "global ged_handle;ged_handle.clip_box=\[$Xclipbox $Yclipbox $Wclipbox $Hclipbox\]"
    set curclipstate "on"
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



proc setAlphaAngle {} {
    global curalpharotation curthetarotation viewToggle

    if { ($curalpharotation == "") || ($curthetarotation == "") } {
	tk_messageBox -icon error -type ok -title "Rotation angles selection failed" -message "You must fill in the 2 fields"
	return
    }

    ScilabEval "global ged_handle;ged_handle.rotation_angles=\[$curalpharotation $curthetarotation\]"
   if { ( $curalpharotation == 0) && ( $curthetarotation == 270) } {
	set viewToggle "2d"
    } else {
	set viewToggle "3d"
    }
}

proc setThetaAngle {} {
    global curalpharotation curthetarotation viewToggle
    
    if { ($curalpharotation == "") || ($curthetarotation == "") } {
	tk_messageBox -icon error -type ok -title "Rotation angles selection failed" -message "You must fill in the 2 fields"
	return
    }
    
    ScilabEval "global ged_handle;ged_handle.rotation_angles=\[$curalpharotation $curthetarotation\]"
    if { ( $curalpharotation == 0) && ( $curthetarotation == 270) } {
	set viewToggle "2d"
    } else {
	set viewToggle "3d"
    }
}

proc toggleView {} {
global viewToggle curalpharotation curthetarotation old_curalpharotation old_curthetarotation
ScilabEval "global ged_handle;ged_handle.view='$viewToggle'"
    if { $viewToggle == "2d" } {
	set old_curalpharotation $curalpharotation
	set old_curthetarotation $curthetarotation
	set curalpharotation 0
	set curthetarotation 270
    } else {
	set curalpharotation $old_curalpharotation
	set curthetarotation $old_curthetarotation
    }
}

proc setXdb {} {
    global dbxmin dbxmax

    if { $dbxmin=="" | $dbxmax=="" } {
	return
    }

    if { $dbxmin > $dbxmax } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message " Xmin > Xmax !"
    } else {
	ScilabEval "setXdb($dbxmin, $dbxmax);"
    }
}

proc setYdb {} {
    global dbymin dbymax

    if { $dbymin=="" | $dbymax=="" } {
	return
    }

    if { $dbymin > $dbymax } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message " Ymin > Ymax !"
    } else {
	ScilabEval "setYdb($dbymin, $dbymax);"
    }
}


proc setZdb {} {
    global dbzmin dbzmax

    if { $dbzmin=="" | $dbzmax=="" } {
	return
    }

    if { $dbzmin > $dbzmax } {
	 tk_messageBox -icon error -type ok -title "Incorrect input" -message " Zmin > Zmax !"
    } else {
	ScilabEval "setZdb($dbzmin, $dbzmax);"
    }
}



proc setForeColor {w index} {  
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.foreground <> $index then ged_handle.foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
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

proc setHiddenAxisColor {w index} {  
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.hidden_axis_color <> $index then ged_handle.hidden_axis_color=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color

    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.hidden_axis_color <> $index then ged_handle.hidden_axis_color=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.hidden_axis_color <> $index then ged_handle.hidden_axis_color=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.hidden_axis_color <> $index then ged_handle.hidden_axis_color=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color

    }
}

proc setFontColor {w index} {    
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.font_color <> $index then ged_handle.font_color=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;  if ged_handle.font_color <> $index ged_handle.font_color=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.font_color <> $index  ged_handle.font_color=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.font_color <> $index  ged_handle.font_color=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}
proc setThickness {w thick} {
ScilabEval "global ged_handle;if ged_handle.thickness <> $thick then ged_handle.thickness=$thick; end;"
}
proc setFontSize {w fs} {
ScilabEval "global ged_handle;if ged_handle.labels_font_size <> $fs then ged_handle.labels_font_size=$fs; end;"
}


proc toggleVisibilityX { frame } {
    global Xaxes_visibleToggle
    ScilabEval "global ged_handle;ged_handle.axes_visible(1)='$Xaxes_visibleToggle'"

    OnOffForeground $frame $Xaxes_visibleToggle
}

proc toggleVisibilityY { frame } {
    global Yaxes_visibleToggle
    ScilabEval "global ged_handle;ged_handle.axes_visible(2)='$Yaxes_visibleToggle'"
    
    OnOffForeground $frame $Yaxes_visibleToggle
}

proc toggleVisibilityZ { frame } {
    global Zaxes_visibleToggle
    ScilabEval "global ged_handle;ged_handle.axes_visible(3)='$Zaxes_visibleToggle'"
    
    OnOffForeground $frame $Zaxes_visibleToggle
}


proc toggleVisibilitylabx { frame } {
    global xlabel_visibility
    ScilabEval "global ged_handle;ged_handle.x_label.visible='$xlabel_visibility'"
    
    OnOffForeground $frame $xlabel_visibility
}

proc toggleVisibilitylaby { frame } {
    global ylabel_visibility
    ScilabEval "global ged_handle;ged_handle.y_label.visible='$ylabel_visibility'"
    
    OnOffForeground $frame $ylabel_visibility
}

proc toggleVisibilitylabz { frame } {
    global zlabel_visibility
    ScilabEval "global ged_handle;ged_handle.z_label.visible='$zlabel_visibility'"
    
    OnOffForeground $frame $zlabel_visibility
}


proc toggleVisibilitytitle { frame } {
    global titlelabel_visibility
    ScilabEval "global ged_handle;ged_handle.title.visible='$titlelabel_visibility'"

    OnOffForeground $frame $titlelabel_visibility
}



proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}

proc toggleIsoview { frame } {
    global isoToggle
    ScilabEval "global ged_handle;ged_handle.isoview='$isoToggle'"

    OnOffForeground $frame $isoToggle
}

proc toggleCubview { frame } {
    global cubToggle
    ScilabEval "global ged_handle;ged_handle.cube_scaling='$cubToggle'"

    OnOffForeground $frame $cubToggle
}

proc selectBoxState {w args} {
    global curBoxState
    ScilabEval "global ged_handle;ged_handle.box='$curBoxState'"
}

proc toggleBox { frame } {
    global boxToggle
    ScilabEval "global ged_handle;ged_handle.box='$boxToggle'"

    OnOffForeground $frame $boxToggle
}
proc toggleX {} {
    global xToggle
    ScilabEval "LogtoggleX('$xToggle')"
}
proc toggleY {} {
global yToggle
ScilabEval "LogtoggleY('$yToggle')"
}
proc toggleZ {} {
global zToggle
ScilabEval "LogtoggleZ('$zToggle')"
}
proc toggleLimits { frame } {
    global limToggle
    ScilabEval "global ged_handle;ged_handle.tight_limits='$limToggle'"

    OnOffForeground $frame $limToggle
}


# X LABEL
proc setXGridColor {w index} {
    global RED BLUE GREEN msdos
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.grid(1) <> $index then ged_handle.grid(1)=$index; end;"
	#nothing to draw: grey color used
	if { $msdos == "F" } {
	    set color [format \#e6e7e6]
	} else {
	    set color [format \#d6d3ce]
	}
	$w config  -activebackground $color -troughcolor $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.grid(1) <> $index then  ged_handle.grid(1)=$index; end;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; if ged_handle.grid(1) <> $index then  ged_handle.grid(1)=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
}


proc setXlabel {} {
global xlabel
ScilabEval "global ged_handle;ged_handle.x_label.text='$xlabel'"
}

proc setXFontLabelSize {w siz} {
ScilabEval "global ged_handle; if ged_handle.x_label.font_size <> $siz then ged_handle.x_label.font_size=$siz; end;"
}

proc setXLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.foreground <> $index then ged_handle.x_label.foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.foreground <> $index then ged_handle.x_label.foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.foreground <> $index then ged_handle.x_label.foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.x_label.foreground <> $index then ged_handle.x_label.foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setXBackLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.background <> $index then ged_handle.x_label.background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.background <> $index then ged_handle.x_label.background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.background <> $index then ged_handle.x_label.background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.x_label.background <> $index then ged_handle.x_label.background=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setXFontForeground {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}

# Y LABEL
proc setYGridColor {w index} {
    global RED BLUE GREEN msdos
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    if { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.grid(2) <> $index then ged_handle.grid(2)=$index; end;"
	#nothing to draw: grey color used
	if { $msdos == "F" } {
	    set color [format \#e6e7e6]
	} else {
	    set color [format \#d6d3ce]
	}
	$w config  -activebackground $color -troughcolor $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.grid(2) <> $index then ged_handle.grid(2)=$index; end;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; if ged_handle.grid(2) <> $index then ged_handle.grid(2)=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}









proc setYlabel {} {
global ylabel
ScilabEval "global ged_handle;ged_handle.y_label.text='$ylabel'"
}

proc setYFontLabelSize {w siz} {
    ScilabEval "global ged_handle;if ged_handle.y_label.font_size <> $siz then ged_handle.y_label.font_size=$siz; end;"
}


proc setYLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.foreground <> $index then ged_handle.y_label.foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color

    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.foreground <> $index then  ged_handle.y_label.foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.foreground <> $index then  ged_handle.y_label.foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.y_label.foreground <> $index then  ged_handle.y_label.foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setYBackLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.background <> $index then ged_handle.y_label.background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color

    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.background <> $index then  ged_handle.y_label.background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.background <> $index then  ged_handle.y_label.background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.y_label.background <> $index then  ged_handle.y_label.background=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setYFontForeground {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then ged_handle.y_label.font_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color

    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then  ged_handle.y_label.font_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then  ged_handle.y_label.font_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then  ged_handle.y_label.font_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}

# Z LABEL
proc setZGridColor {w index} {
    global RED BLUE GREEN msdos zGrid_initial
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -1 } {
	# Here and in order to avoid intempestive redraw due to 3d mode switching (to know the value of ged_handlde.grid(3)),
	# we directly take zGrid_initial as default value
	ScilabEval "global ged_handle;if $zGrid_initial <> $index then  ged_handle.grid(3)=$index; end;"
	#nothing to draw: grey color used
	if { $msdos == "F" } {
	    set color [format \#e6e7e6]
	} else {
	    set color [format \#d6d3ce]
	}
	$w config  -activebackground $color -troughcolor $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle;if $zGrid_initial <> $index then  ged_handle.grid(3)=$index; end;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle;if $zGrid_initial <> $index then  ged_handle.grid(3)=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setZlabel {} {
global zlabel
ScilabEval "global ged_handle;ged_handle.z_label.text='$zlabel'"
}

proc setZFontLabelSize {w siz} {
    ScilabEval "global ged_handle;if ged_handle.z_label.font_size <> $siz then ged_handle.z_label.font_size=$siz; end;"
}



proc setZFontLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.foreground <> $index then ged_handle.z_label.foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.foreground <> $index then  ged_handle.z_label.foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.foreground <> $index then  ged_handle.z_label.foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.z_label.foreground <> $index then  ged_handle.z_label.foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}


proc setZBackLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.background <> $index then ged_handle.z_label.background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.background <> $index then  ged_handle.z_label.background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.background <> $index then  ged_handle.z_label.background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.z_label.background <> $index then  ged_handle.z_label.background=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setZFontForeground {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then ged_handle.z_label.font_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then  ged_handle.z_label.font_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then  ged_handle.z_label.font_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then  ged_handle.z_label.font_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}

# Title
proc setTitleLabel {} {
global tlabel
ScilabEval "global ged_handle;ged_handle.title.text='$tlabel';"
}

proc setTitleFontLabelSize {w siz} {
    ScilabEval "global ged_handle;if ged_handle.title.font_size <> $siz then ged_handle.title.font_size=$siz; end;"
}


proc setTitleFontLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.title.foreground <> $index then  ged_handle.title.foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.title.foreground <> $index then  ged_handle.title.foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.title.foreground <> $index then  ged_handle.title.foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.title.foreground <> $index then  ged_handle.title.foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}


proc setTitleBackLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.title.background <> $index then  ged_handle.title.background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.title.background <> $index then  ged_handle.title.background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.title.background <> $index then  ged_handle.title.background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.title.background <> $index then  ged_handle.title.background=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setTitleFontForeground {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setlabels {} {
global ylabel
global xlabel
ScilabEval "global ged_handle;ged_handle.x_label.text='$xlabel';ged_handle.y_label.text='$ylabel'"
}

proc SelectXpos {w args} {
global Xlabelpos
ScilabEval "global ged_handle;ged_handle.x_location='$Xlabelpos'"
}
proc SelectYpos {w args} {
global Ylabelpos
ScilabEval "global ged_handle;ged_handle.y_location='$Ylabelpos'"
}








proc SelectXFontStyle {w args} {
global Xlabelfontstyle
ScilabEval "setLabelsFontStyle('x','$Xlabelfontstyle')"
}

proc SelectYFontStyle {w args} {
global Ylabelfontstyle
ScilabEval "setLabelsFontStyle('y','$Ylabelfontstyle')"
}


proc SelectZFontStyle {w args} {
global Zlabelfontstyle
ScilabEval "setLabelsFontStyle('z','$Zlabelfontstyle')"
}

proc SelectTitleFontStyle {w args} {
global TITLEfontstyle
ScilabEval "setLabelsFontStyle('t','$TITLEfontstyle')"
}


proc SelectFontStyle {w args} {
global fontstyle
ScilabEval "setFontStyle('$fontstyle')"
}



proc toggleClear { frame } {
    global curautoclear
    ScilabEval "global ged_handle;ged_handle.auto_clear='$curautoclear'"
    
    OnOffForeground $frame $curautoclear
}


proc toggleScale { frame } {
    global curautoscale
    ScilabEval "global ged_handle;ged_handle.auto_scale='$curautoscale'"

     OnOffForeground $frame $curautoscale
}


# proc toggleFill {} {
# global curfillmode
# ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"
# }


proc PopUp { w numpage} {
    variable www www2
    global LEI_x LEI_y LEI_z
    global LOCATIONS_X LABELS_X nbticks_x
    global LOCATIONS_Y LABELS_Y nbticks_y
    global LOCATIONS_Z LABELS_Z nbticks_z
    global fen1 fen2 fen3
    global SubticksEntryX SubticksEntryY SubticksEntryZ
    global XautoticksToggle YautoticksToggle ZautoticksToggle
    global StepEntryX StepEntryY StepEntryZ
    global Xaxes_visibleToggle Yaxes_visibleToggle Zaxes_visibleToggle
    global largeur
    global ticksNoteBook
    global smallPad mediumPad
    global gedFont

    global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
#     set TICK_WINDOW_POSITION "+0+0"
    set envSCIHOME $envSCIHOME
    set preffilename $envSCIHOME/.GedPreferences.tcl
    catch {source $preffilename}

    set frameaxes $w

    set www .ticks
        
    catch {destroy $www}

    toplevel $www
    wm withdraw  $www
    update idletasks
    wm transient $www $w
    wm deiconify $www
    
    wm title $www  "Edit Axes Ticks"
    wm geometry $www 310x520$TICK_WINDOW_POSITION
    wm maxsize  $www 310 520
    wm iconname $www "TE"
    wm protocol $www WM_DELETE_WINDOW "closeTicksWindow $www"
    grab set $www
#    bell -displayof $w
    
    set topf  [frame $www.topf]
    set titf1 [TitleFrame $topf.titf1 -text "Ticks Editor" -font $gedFont]
    
    set parent  [$titf1 getframe]
    set pw1  [PanedWindow $parent.pw -side top]
    set pane3  $pw1  
    
    #sauv www
    set old_www $www

    # Make a frame scrollable

    set sw [ScrolledWindow $pane3.sw -relief sunken -borderwidth 2]
    # pack $sw -in .sw -fill both -expand true 
    set sf [ScrollableFrame $sw.f]
    $sw setwidget $sf
    set uf [$sf getframe]

    set w $uf
    set fra [frame $w.frame -borderwidth 0]
    pack $fra  -anchor w -fill both

    
    Notebook:create $uf.n -pages {X Y Z} -pad 20   -height 400 -width 230
    pack $uf.n -in $uf -fill both -expand yes
    set ticksNoteBook $uf.n

    Notebook:raise.page $uf.n $numpage

    set w2 $www
    
    ########### X onglet ##############################################
    ###################################################################
    set fen1 [Notebook:frame $uf.n X]

#meme choix que Mtlb
    set LEI_x $nbticks_x
    set numpage 0
    
    frame $fen1.frame -borderwidth 0
    pack $fen1.frame -anchor w -fill both


    #visibility of X axis
    frame $fen1.frame.vis -borderwidth 0
    pack $fen1.frame.vis  -in $fen1.frame -side top -fill x -pady 0
    label $fen1.frame.vislabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
    checkbutton $fen1.frame.visb  -text "on"\
        -variable Xaxes_visibleToggle  -onvalue "on" -offvalue "off" \
        -command "toggleVisibilityX $fen1.frame.visb" -font $gedFont
    OnOffForeground $fen1.frame.visb $Xaxes_visibleToggle

    pack $fen1.frame.vislabel -in $fen1.frame.vis -side left
    pack $fen1.frame.visb  -in $fen1.frame.vis  -side left  -fill x -padx $smallPad

    frame $fen1.frame.xautoticks -borderwidth 0
    pack $fen1.frame.xautoticks  -in $fen1.frame -side top -fill x -pady 0
    label $fen1.frame.xautotickslabel  -text "Auto ticks:" -font $gedFont -anchor e -width $largeur
    checkbutton $fen1.frame.xautoticksb  -text "on"\
	-variable XautoticksToggle  -onvalue "on" -offvalue "off" \
	-command "toggleXautoticks $frameaxes $numpage $fen1.frame.xautoticksb" -font $gedFont
    OnOffForeground $fen1.frame.xautoticksb $XautoticksToggle

    pack $fen1.frame.xautotickslabel -in $fen1.frame.xautoticks -side left
    pack $fen1.frame.xautoticksb  -in $fen1.frame.xautoticks  -side left  -fill x -pady 0 -padx 1.m

    frame $fen1.frame.step -borderwidth 0
    pack $fen1.frame.step  -in $fen1.frame -side top -fill x -pady 0
    label $fen1.frame.steplabel  -text "Step by:" -font $gedFont -anchor e -width $largeur
    entry $fen1.frame.stepe  -relief sunken  -justify right \
	-background white -textvariable StepEntryX -width 10 -font $gedFont
    bind  $fen1.frame.stepe <Return> "SetStep $frameaxes $numpage 1"
    bind  $fen1.frame.stepe <KP_Enter> "SetStep $frameaxes $numpage 1"
    bind  $fen1.frame.stepe <FocusOut> "SetStep $frameaxes $numpage 1"
    pack $fen1.frame.steplabel -in $fen1.frame.step -side left
    pack $fen1.frame.stepe -in $fen1.frame.step -side left -pady 0 -padx $mediumPad

    frame $fen1.frame.subticks -borderwidth 0
    pack $fen1.frame.subticks  -in $fen1.frame -side top -fill x -pady 0
    label $fen1.frame.subtickslabel  -text "Sub ticks:" -font $gedFont -anchor e -width $largeur

#    puts "SubticksEntryX vaut: $SubticksEntryX"

    entry $fen1.frame.subtickse  -relief sunken  -justify right \
	-background white -textvariable SubticksEntryX -width 10 -font $gedFont
    bind  $fen1.frame.subtickse <Return> "SetSubticksX"
    bind  $fen1.frame.subtickse <KP_Enter> "SetSubticksX"
    bind  $fen1.frame.subtickse <FocusOut> "SetSubticksX"
    pack $fen1.frame.subtickslabel -in $fen1.frame.subticks -side left
    pack $fen1.frame.subtickse -in $fen1.frame.subticks -side left -pady 0 -padx $mediumPad
    
    frame $fen1.frame.fdata -borderwidth 0
    pack $fen1.frame.fdata  -in $fen1.frame -side top   -fill x
    
    scrollbar $fen1.frame.ysbar -orient vertical -command   {$fen1.frame.c yview}
    #canvas $fen1.frame.c -width 8i -height 2.6i  -yscrollcommand {$fen1.frame.ysbar set}
    canvas $fen1.frame.c -width 232  -height 247  -yscrollcommand {$fen1.frame.ysbar set}
    
    $fen1.frame.c create text 70 10 -anchor c -text "Locations" -font $gedFont
    $fen1.frame.c create text 150 10 -anchor c -text "Labels" -font $gedFont
      
    for {set i 1} {$i<=$nbticks_x} {incr i} {
	set bb [expr 10+(25*$i)]
	$fen1.frame.c create text 10 $bb -anchor c -text $i
	#Locations
	set aa [expr 70]
	entry  $fen1.frame.c.locationsdata$i  -relief sunken  -justify right -width 10\
	    -background white -textvariable LOCATIONS_X($i)  -font $gedFont
	#	bind  $w.frame.c.locationsdata$i <Return> "setTicksLocations $w $i "
	bind  $fen1.frame.c.locationsdata$i <ButtonPress-1> "setLEI_x $i $fen1.frame.c.locationsdata$i;"
	bind  $fen1.frame.c.locationsdata$i <Return> "TicksApplyXRepalce $fen1 $fen1.frame.c.locationsdata$i right"
	bind  $fen1.frame.c.locationsdata$i <KP_Enter> "TicksApplyXReplace $fen1 $fen1.frame.c.locationsdata$i right"
	bind  $fen1.frame.c.locationsdata$i <FocusOut> "TicksApplyXReplace $fen1 $fen1.frame.c.locationsdata$i right"
	$fen1.frame.c create window $aa $bb -anchor c -window $fen1.frame.c.locationsdata$i
	
	
	#Labels
	set aa [expr 150]
	entry  $fen1.frame.c.labelsdata$i  -relief sunken   -justify left -width 10\
	    -background white -textvariable LABELS_X($i) -font $gedFont
	#	bind  $fen1.frame.c.labelsdata$i <Return> "setTicksLabels $w $i "
	bind  $fen1.frame.c.labelsdata$i <ButtonPress-1> "setLEI_x $i $fen1.frame.c.labelsdata$i;"
	bind  $fen1.frame.c.labelsdata$i <Return> "TicksApplyXReplace $fen1 $fen1.frame.c.labelsdata$i left"
	bind  $fen1.frame.c.labelsdata$i <KP_Enter> "TicksApplyXReplace $fen1 $fen1.frame.c.labelsdata$i left"
	bind  $fen1.frame.c.labelsdata$i <FocusOut> "TicksApplyXReplace $fen1 $fen1.frame.c.labelsdata$i left"
	$fen1.frame.c create window $aa $bb -anchor c -window $fen1.frame.c.labelsdata$i
    }

    $fen1.frame.c configure -scrollregion [$fen1.frame.c bbox all] -yscrollincrement 9
    
    pack  $fen1.frame.ysbar -side right -fill y
    pack  $fen1.frame.c
    
    
    #sep bar
    frame $fen1.sep -height 2 -borderwidth 1 -relief sunken
    pack $fen1.sep -fill both  
    
     #exit button
    frame $fen1.buttons -borderwidth 0
    pack  $fen1.buttons -anchor w -fill both  -side bottom   -fill x
    
    #apply button
    button $fen1.buttons.apply -text Apply -command "TicksApplyX $fen1" -font $gedFont -width 10
    # quit button
    button $fen1.buttons.b -text Quit -command "closeTicksWindow $old_www" -font $gedFont -width 10
    
    pack $fen1.buttons.apply  $fen1.buttons.b -in  $fen1.buttons \
	-side left   -fill x  -expand 1 -pady 0

    frame $fen1.boutons -borderwidth 0
    pack $fen1.boutons -anchor w -fill both  -side bottom   -fill x
    
    #Insert/ Delete buttons
    button $fen1.boutons.buttoninsert -text Insert -command "TicksInsertX $fen1 $frameaxes " -font $gedFont -width 10
    button $fen1.boutons.buttondelete -text Delete -command "TicksDeleteX $fen1 $frameaxes " -font $gedFont -width 10
    pack $fen1.boutons.buttoninsert $fen1.boutons.buttondelete -in  $fen1.boutons \
	-side left   -fill x  -expand 1 -pady 0
    
    

    ########### Y onglet ##############################################
    ###################################################################
    set fen2 [Notebook:frame $uf.n Y]
    
#    puts "1. fen2 vaut: $fen2"
    #meme choix que Mtlb
    set LEI_y $nbticks_y
    set numpage 1


    frame $fen2.frame -borderwidth 0
    pack $fen2.frame -anchor w -fill both

    #visibility of Y axis
    frame $fen2.frame.vis -borderwidth 0
    pack $fen2.frame.vis  -in $fen2.frame -side top -fill x -pady 0
    label $fen2.frame.vislabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
    checkbutton $fen2.frame.visb  -text "on"\
        -variable Yaxes_visibleToggle  -onvalue "on" -offvalue "off" \
        -command "toggleVisibilityY $fen2.frame.visb"  -font $gedFont
    OnOffForeground $fen2.frame.visb $Yaxes_visibleToggle

    pack $fen2.frame.vislabel -in $fen2.frame.vis -side left
    pack $fen2.frame.visb  -in $fen2.frame.vis  -side left  -fill x -padx $smallPad

    frame $fen2.frame.yautoticks -borderwidth 0
    pack $fen2.frame.yautoticks  -in $fen2.frame -side top -fill x -pady 0
    label $fen2.frame.yautotickslabel  -text "Auto ticks:" -font $gedFont -anchor e -width $largeur
    checkbutton $fen2.frame.yautoticksb  -text "on"\
	-variable YautoticksToggle  -onvalue "on" -offvalue "off" \
	-command "toggleYautoticks $frameaxes $numpage $fen2.frame.yautoticksb" -font $gedFont
    OnOffForeground $fen2.frame.yautoticksb $YautoticksToggle

    pack $fen2.frame.yautotickslabel -in $fen2.frame.yautoticks -side left
    pack $fen2.frame.yautoticksb  -in $fen2.frame.yautoticks  -side left  -fill x -pady 0 -padx $smallPad

    frame $fen2.frame.step -borderwidth 0
    pack $fen2.frame.step  -in $fen2.frame -side top -fill x -pady 0
    label $fen2.frame.steplabel  -text "Step by:" -font $gedFont -anchor e -width $largeur
    entry $fen2.frame.stepe  -relief sunken  -justify right \
	-background white -textvariable StepEntryY -width 10 -font $gedFont
    bind  $fen2.frame.stepe <Return> "SetStep $frameaxes $numpage 2"
    bind  $fen2.frame.stepe <KP_Enter> "SetStep $frameaxes $numpage 2"
    bind  $fen2.frame.stepe <FocusOut> "SetStep $frameaxes $numpage 2"
    pack $fen2.frame.steplabel -in $fen2.frame.step -side left
		pack $fen2.frame.stepe -in $fen2.frame.step -side left -pady 0 -padx $mediumPad

    frame $fen2.frame.subticks -borderwidth 0
    pack $fen2.frame.subticks  -in $fen2.frame -side top -fill x -pady 0
    label $fen2.frame.subtickslabel  -text "Sub ticks:" -font $gedFont -anchor e -width $largeur

    entry $fen2.frame.subtickse  -relief sunken  -justify right \
	-background white -textvariable SubticksEntryY -width 10 -font $gedFont
    bind  $fen2.frame.subtickse <Return> "SetSubticksY"
    bind  $fen2.frame.subtickse <KP_Enter> "SetSubticksY"
    bind  $fen2.frame.subtickse <FocusOut> "SetSubticksY"
    pack $fen2.frame.subtickslabel -in $fen2.frame.subticks -side left
		pack $fen2.frame.subtickse -in $fen2.frame.subticks -side left -pady 0 -padx $mediumPad

    frame $fen2.frame.fdata -borderwidth 0
    pack $fen2.frame.fdata  -in $fen2.frame -side top   -fill x
  
    canvas $fen2.frame.c -width 232 -height 247  -yscrollcommand {$fen2.frame.ysbar set}
    scrollbar $fen2.frame.ysbar -orient vertical -command   {$fen2.frame.c yview}
    
    $fen2.frame.c create text 70 10 -anchor c -text "Locations" -font $gedFont
    $fen2.frame.c create text 150 10 -anchor c -text "Labels" -font $gedFont
      
    for {set i 1} {$i<=$nbticks_y} {incr i} {
	set bb [expr 10+(25*$i)]
	$fen2.frame.c create text 10 $bb -anchor c -text $i
	#Locations
	set aa [expr 70]
	entry  $fen2.frame.c.locationsdata$i  -relief sunken  -justify right  -width 10\
	    -background white -textvariable LOCATIONS_Y($i) -font $gedFont
	#	bind  $w.frame.c.locationsdata$i <Return> "setTicksLocations $w $i "
	bind  $fen2.frame.c.locationsdata$i <ButtonPress-1> "setLEI_y $i $fen2.frame.c.locationsdata$i "
	bind  $fen2.frame.c.locationsdata$i <Return> "TicksApplyYReplace $fen2 $fen2.frame.c.locationsdata$i right"
	bind  $fen2.frame.c.locationsdata$i <KP_Enter> "TicksApplyYReplace $fen2 $fen2.frame.c.locationsdata$i right"
	bind  $fen2.frame.c.locationsdata$i <FocusOut> "TicksApplyYReplace $fen2 $fen2.frame.c.locationsdata$i right"
	$fen2.frame.c create window $aa $bb -anchor c -window $fen2.frame.c.locationsdata$i
	
	
	#Labels
	set aa [expr 150]
	entry  $fen2.frame.c.labelsdata$i  -relief sunken   -justify left  -width 10\
	    -background white -textvariable LABELS_Y($i) -font $gedFont
	#	bind  $fen2.frame.c.labelsdata$i <Return> "setTicksLabels $w $i "
	bind  $fen2.frame.c.labelsdata$i <ButtonPress-1> "setLEI_y $i $fen2.frame.c.labelsdata$i"
	bind  $fen2.frame.c.labelsdata$i <Return> "TicksApplyYReplace $fen2 $fen2.frame.c.labelsdata$i left"
	bind  $fen2.frame.c.labelsdata$i <KP_Enter> "TicksApplyYReplace $fen2 $fen2.frame.c.labelsdata$i left"
	bind  $fen2.frame.c.labelsdata$i <FocusOut> "TicksApplyYReplace $fen2 $fen2.frame.c.labelsdata$i left"
	$fen2.frame.c create window $aa $bb -anchor c -window $fen2.frame.c.labelsdata$i
    }

    $fen2.frame.c configure -scrollregion [$fen2.frame.c bbox all] -yscrollincrement 9
    
    pack  $fen2.frame.ysbar -side right -fill y
    pack  $fen2.frame.c
    
    #sep bar
    frame $fen2.sep -height 2 -borderwidth 1 -relief sunken
    pack $fen2.sep -fill both

    
     #exit button
    frame $fen2.buttons -borderwidth 0
    pack  $fen2.buttons -anchor w -fill both  -side bottom   -fill x
    
    button $fen2.buttons.apply -text Apply -command "TicksApplyY $fen2" -font $gedFont -width 10
    # button $fen2.buttons.b -text Quit -command "SavePreferences2 $old_www; destroy $old_www" -font $gedFont -width 10
    button $fen2.buttons.b -text Quit -command "closeTicksWindow $old_www" -font $gedFont -width 10
    pack $fen2.buttons.apply  $fen2.buttons.b -in  $fen2.buttons \
	-side left   -fill x  -expand 1 -pady 0
    
    frame $fen2.boutons -borderwidth 0
    pack $fen2.boutons -anchor w -fill both  -side bottom   -fill x
    
    #Insert/ Delete buttons
    button $fen2.boutons.buttoninsert -text Insert -command "TicksInsertY $fen2 $frameaxes " -font $gedFont -width 10
    button $fen2.boutons.buttondelete -text Delete -command "TicksDeleteY $fen2 $frameaxes " -font $gedFont -width 10
    pack $fen2.boutons.buttoninsert $fen2.boutons.buttondelete -in  $fen2.boutons \
	-side left   -fill x  -expand 1 -pady 0
    
    


     ########### Z onglet ##############################################
     ###################################################################
     set fen3 [Notebook:frame $uf.n Z]
  
    #meme choix que Mtlb
    set LEI_z $nbticks_z
    set numpage 2


    frame $fen3.frame -borderwidth 0
    pack $fen3.frame -anchor w -fill both

    #visibility of Z axis
    frame $fen3.frame.vis -borderwidth 0
    pack $fen3.frame.vis  -in $fen3.frame -side top -fill x -pady 0
    label $fen3.frame.vislabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
    checkbutton $fen3.frame.visb  -text "on"\
        -variable Zaxes_visibleToggle  -onvalue "on" -offvalue "off" \
        -command "toggleVisibilityZ $fen3.frame.visb" -font $gedFont
    OnOffForeground $fen3.frame.visb $Zaxes_visibleToggle

    pack $fen3.frame.vislabel -in $fen3.frame.vis -side left
    pack $fen3.frame.visb  -in $fen3.frame.vis  -side left  -fill x -padx $smallPad

    frame $fen3.frame.zautoticks -borderwidth 0
    pack $fen3.frame.zautoticks  -in $fen3.frame -side top -fill x -pady 0
    label $fen3.frame.zautotickslabel  -text "Auto ticks:" -font $gedFont -anchor e -width $largeur
    checkbutton $fen3.frame.zautoticksb  -text "on"\
	-variable ZautoticksToggle  -onvalue "on" -offvalue "off" \
	-command "toggleZautoticks $frameaxes $numpage $fen3.frame.zautoticksb" -font $gedFont
    OnOffForeground $fen3.frame.zautoticksb $ZautoticksToggle

    pack $fen3.frame.zautotickslabel -in $fen3.frame.zautoticks -side left
    pack $fen3.frame.zautoticksb  -in $fen3.frame.zautoticks  -side left  -fill x -pady 0 -padx $smallPad

    frame $fen3.frame.step -borderwidth 0
    pack $fen3.frame.step  -in $fen3.frame -side top -fill x -pady 0
    label $fen3.frame.steplabel  -text "Step by:" -font $gedFont -anchor e -width $largeur
    entry $fen3.frame.stepe  -relief sunken  -justify right \
	-background white -textvariable StepEntryZ -width 10 -font $gedFont
    bind  $fen3.frame.stepe <Return> "SetStep $frameaxes $numpage 3"
    bind  $fen3.frame.stepe <KP_Enter> "SetStep $frameaxes $numpage 3"
    bind  $fen3.frame.stepe <FocusOut> "SetStep $frameaxes $numpage 3"
    pack $fen3.frame.steplabel -in $fen3.frame.step -side left
    pack $fen3.frame.stepe -in $fen3.frame.step -side left -pady 0 -padx $mediumPad

    frame $fen3.frame.subticks -borderwidth 0
    pack $fen3.frame.subticks  -in $fen3.frame -side top -fill x -pady 0
    label $fen3.frame.subtickslabel  -text "Sub ticks:" -font $gedFont -anchor e -width $largeur

    entry $fen3.frame.subtickse  -relief sunken  -justify right \
	-background white -textvariable SubticksEntryZ -width 10 -font $gedFont
    bind  $fen3.frame.subtickse <Return> "SetSubticksZ"
    bind  $fen3.frame.subtickse <KP_Enter> "SetSubticksZ"
    bind  $fen3.frame.subtickse <FocusOut> "SetSubticksZ"
    pack $fen3.frame.subtickslabel -in $fen3.frame.subticks -side left
		pack $fen3.frame.subtickse -in $fen3.frame.subticks -side left -pady 0 -padx $mediumPad

    frame $fen3.frame.fdata -borderwidth 0
    pack $fen3.frame.fdata  -in $fen3.frame -side top   -fill x
  
    canvas $fen3.frame.c -width 232 -height 247 -yscrollcommand {$fen3.frame.ysbar set}
    scrollbar $fen3.frame.ysbar -orient vertical -command   {$fen3.frame.c yview}
    
    $fen3.frame.c create text 70 10 -anchor c -text "Locations" -font $gedFont
    $fen3.frame.c create text 150 10 -anchor c -text "Labels" -font $gedFont
      
    for {set i 1} {$i<=$nbticks_z} {incr i} {
	set bb [expr 10+(25*$i)]
	$fen3.frame.c create text 10 $bb -anchor c -text $i
	#Locations
	set aa [expr 70]
	entry  $fen3.frame.c.locationsdata$i  -relief sunken  -justify right  -width 10\
	    -background white -textvariable LOCATIONS_Z($i) -font $gedFont
	#	bind  $w.frame.c.locationsdata$i <Return> "setTicksLocations $w $i "
	bind  $fen3.frame.c.locationsdata$i <ButtonPress-1> "setLEI_z $i $fen3.frame.c.locationsdata$i;"
	bind  $fen3.frame.c.locationsdata$i <Return> "TicksApplyZReplace $fen3 $fen3.frame.c.locationsdata$i right"
	bind  $fen3.frame.c.locationsdata$i <KP_Enter> "TicksApplyZReplace $fen3 $fen3.frame.c.locationsdata$i right"
	bind  $fen3.frame.c.locationsdata$i <FocusOut> "TicksApplyZReplace $fen3 $fen3.frame.c.locationsdata$i right"
	$fen3.frame.c create window $aa $bb -anchor c -window $fen3.frame.c.locationsdata$i
	
	
	#Labels
	set aa [expr 150]
	entry  $fen3.frame.c.labelsdata$i  -relief sunken   -justify left  -width 10\
	    -background white -textvariable LABELS_Z($i) -font $gedFont
	#	bind  $fen3.frame.c.labelsdata$i <Return> "setTicksLabels $w $i "
	bind  $fen3.frame.c.labelsdata$i <ButtonPress-1> "setLEI_z $i $fen3.frame.c.labelsdata$i "
	bind  $fen3.frame.c.labelsdata$i <Return> "TicksApplyZReplace $fen3 $fen3.frame.c.labelsdata$i left"
	bind  $fen3.frame.c.labelsdata$i <KP_Enter> "TicksApplyZReplace $fen3 $fen3.frame.c.labelsdata$i left"
	bind  $fen3.frame.c.labelsdata$i <FocusOut> "TicksApplyZReplace $fen3 $fen3.frame.c.labelsdata$i left"
	$fen3.frame.c create window $aa $bb -anchor c -window $fen3.frame.c.labelsdata$i
    }

    $fen3.frame.c configure -scrollregion [$fen3.frame.c bbox all] -yscrollincrement 9
    
    pack  $fen3.frame.ysbar -side right -fill y
    pack  $fen3.frame.c
    
    
    #sep bar
    frame $fen3.sep -height 2 -borderwidth 1 -relief sunken
    pack $fen3.sep -fill both

    
     #exit button
    frame $fen3.buttons -borderwidth 0
    pack  $fen3.buttons -anchor w -fill both  -side bottom   -fill x
    
    button $fen3.buttons.apply -text Apply -command "TicksApplyZ $fen3" -font $gedFont -width 10
    button $fen3.buttons.b -text Quit -command "closeTicksWindow $old_www" -font $gedFont -width 10
    
    pack $fen3.buttons.apply  $fen3.buttons.b -in  $fen3.buttons \
	-side left   -fill x  -expand 1 -pady 0
    
    frame $fen3.boutons -borderwidth 0
    pack $fen3.boutons -anchor w -fill both  -side bottom   -fill x
    
    #Insert/ Delete buttons
    button $fen3.boutons.buttoninsert -text Insert -command "TicksInsertZ $fen3 $frameaxes " -font $gedFont -width 10
    button $fen3.boutons.buttondelete -text Delete -command "TicksDeleteZ $fen3 $frameaxes " -font $gedFont -width 10
    pack $fen3.boutons.buttoninsert $fen3.boutons.buttondelete -in  $fen3.boutons \
	-side left   -fill x  -expand 1 -pady 0
    


    pack $sw $pw1 -fill both -expand yes
    pack $titf1 -padx $smallPad -side left -fill both -expand yes
    pack $topf -fill both -pady 0 -expand yes




}


############## TICKS ##############

### X

proc TicksInsertX { ww w } {
    global tmploc tmplab
    global LOCATIONS_X LABELS_X
    global LEI_x
    global nbticks_x

    set i $LEI_x

    if {$i == -1} {
	tk_messageBox -icon error -type ok -title "Insertion failed" -message "Select a cell first to insert after"
	return
    }

    for {set j 1} {$j<=$nbticks_x} {incr j} {
	set tmploc($j) $LOCATIONS_X($j)
	set tmplab($j) $LABELS_X($j)

	if {$j == $i} {
	    set tmploc($j) $LOCATIONS_X($j)
	    set tmplab($j) $LABELS_X($j)
	    break
	}
    }
    set fin [expr $nbticks_x+1]
    set index [expr $i+1]
    for {set j $index} {$j<=$fin} {incr j} {
	set ind [expr $j-1]
	set tmploc($j) $LOCATIONS_X($ind)
	set tmplab($j) $LABELS_X($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_X($j) $tmploc($j)
	set LABELS_X($j) $tmplab($j)
    }

    set nbticks_x $fin

    set numpage 0
    PopUp $w $numpage

}





proc TicksDeleteX { ww w } {
    global tmploc tmplab
    global LOCATIONS_X LABELS_X
    global LEI_x
    global nbticks_x


    set i $LEI_x

    if {$nbticks_x <= 2} {
	tk_messageBox -icon error -type ok -title "Deletion forbidden" -message "You must have at least 2 ticks"
	return
    }

    for {set j 1} {$j<=$nbticks_x} {incr j} {
	set tmploc($j) $LOCATIONS_X($j)
	set tmplab($j) $LABELS_X($j)

	if {$j == $i} {
	    break
	}
    }
    set fin [expr $nbticks_x-1]
    set index [expr $i+1]
    for {set j $i} {$j<=$fin} {incr j} {
	set ind [expr $j+1]
	set tmploc($j) $LOCATIONS_X($ind)
	set tmplab($j) $LABELS_X($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_X($j) $tmploc($j)
	set LABELS_X($j) $tmplab($j)
    }

    set nbticks_x $fin

    set numpage 0
    PopUp $w $numpage
    
}

# jb Silvy
# replace the entry wid and call TickApply
proc TicksApplyXReplace { w wid side } {
    $wid configure -justify $side
    TicksApplyX $w ;
}

proc TicksApplyX { w } {
    global LOCATIONS_X LABELS_X
    global nbticks_x 
    global XautoticksToggle
    
    if { $nbticks_x == 0 } {
        # in this case ged_tmp_* won't be initialized
        return
    }

    #First, we pass the TLIST in 2 arrays to scilab
    #LOCATIONS_X
    for {set i 1} {$i<=$nbticks_x} {incr i} {
	set index $i
	if { $i == 1 } {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab($LOCATIONS_X($index),$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab2($LOCATIONS_X($index),$index,ged_tmp_LOCATIONS)"  "seq"
	}
    }
     #LABELS_X
    for {set i 1} {$i<=$nbticks_x} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LABELS=GetTab('$LABELS_X($index)',$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LABELS=GetTab2('$LABELS_X($index)',$index,ged_tmp_LABELS)"  "seq"
	}
    }
    
    ScilabEval "setTicksTList('X',ged_tmp_LOCATIONS,ged_tmp_LABELS)" "seq"
    
    set XautoticksToggle "off"
    OnOffForeground $w.frame.xautoticksb $XautoticksToggle

    #$wi configure -justify right
}


### Y

proc TicksInsertY { ww w } {
    global tmploc tmplab
    global LOCATIONS_Y LABELS_Y
    global LEI_y
    global nbticks_y
 
    set i $LEI_y

    if {$i == -1} {
	tk_messageBox -icon error -type ok -title "Insertion failed" -message "Select a cell first to insert after"
	return
    }

    for {set j 1} {$j<=$nbticks_y} {incr j} {
	set tmploc($j) $LOCATIONS_Y($j)
	set tmplab($j) $LABELS_Y($j)

	if {$j == $i} {
	    set tmploc($j) $LOCATIONS_Y($j)
	    set tmplab($j) $LABELS_Y($j)
	    break
	}
    }
    set fin [expr $nbticks_y+1]
    set index [expr $i+1]
    for {set j $index} {$j<=$fin} {incr j} {
	set ind [expr $j-1]
	set tmploc($j) $LOCATIONS_Y($ind)
	set tmplab($j) $LABELS_Y($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Y($j) $tmploc($j)
	set LABELS_Y($j) $tmplab($j)
    }

    set nbticks_y $fin

    set numpage 1
    PopUp $w $numpage

}





proc TicksDeleteY { ww w } {
    global tmploc tmplab
    global LOCATIONS_Y LABELS_Y
    global LEI_y
    global nbticks_y


    set i $LEI_y

    if {$nbticks_y <= 2} {
	tk_messageBox -icon error -type ok -title "Deletion forbidden" -message "You must have at least 2 ticks"
	return
    }

    for {set j 1} {$j<=$nbticks_y} {incr j} {
	set tmploc($j) $LOCATIONS_Y($j)
	set tmplab($j) $LABELS_Y($j)

	if {$j == $i} {
	    break
	}
    }
    set fin [expr $nbticks_y-1]
    set index [expr $i+1]
    for {set j $i} {$j<=$fin} {incr j} {
	set ind [expr $j+1]
	set tmploc($j) $LOCATIONS_Y($ind)
	set tmplab($j) $LABELS_Y($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Y($j) $tmploc($j)
	set LABELS_Y($j) $tmplab($j)
    }

    set nbticks_y $fin

    set numpage 1
    PopUp $w $numpage
    
}

# jb Silvy
# replace the entry wid and call TickApply
proc TicksApplyYReplace { w wid side } {
    $wid configure -justify $side
    TicksApplyY $w ;
}

proc TicksApplyY { w } {
    global LOCATIONS_Y LABELS_Y
    global nbticks_y
    global YautoticksToggle

    if { $nbticks_y == 0 } {
        return
    }

    #First, we pass the TLIST in 2 arrays to scilab
    #LOCATIONS_Y
    for {set i 1} {$i<=$nbticks_y} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab($LOCATIONS_Y($index),$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab2($LOCATIONS_Y($index),$index,ged_tmp_LOCATIONS)"  "seq"
	}
    }
     #LABELS_Y
    for {set i 1} {$i<=$nbticks_y} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LABELS=GetTab('$LABELS_Y($index)',$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LABELS=GetTab2('$LABELS_Y($index)',$index,ged_tmp_LABELS)"  "seq"
	}
    }
    
    ScilabEval "setTicksTList('Y',ged_tmp_LOCATIONS,ged_tmp_LABELS)" "seq"
    
    set YautoticksToggle "off"
    OnOffForeground $w.frame.yautoticksb $YautoticksToggle
}



### Z

proc TicksInsertZ { ww w } {
    global tmploc tmplab
    global LOCATIONS_Z LABELS_Z
    global LEI_z
    global nbticks_z

    set i $LEI_z

    if {$i == -1} {
	tk_messageBox -icon error -type ok -title "Insertion failed" -message "Select a cell first to insert after"
	return
    }

    for {set j 1} {$j<=$nbticks_z} {incr j} {
	set tmploc($j) $LOCATIONS_Z($j)
	set tmplab($j) $LABELS_Z($j)

	if {$j == $i} {
	    set tmploc($j) $LOCATIONS_Z($j)
	    set tmplab($j) $LABELS_Z($j)
	    break
	}
    }
    set fin [expr $nbticks_z+1]
    set index [expr $i+1]
    for {set j $index} {$j<=$fin} {incr j} {
	set ind [expr $j-1]
	set tmploc($j) $LOCATIONS_Z($ind)
	set tmplab($j) $LABELS_Z($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Z($j) $tmploc($j)
	set LABELS_Z($j) $tmplab($j)
    }

    set nbticks_z $fin

    set numpage 2
    PopUp $w $numpage

}





proc TicksDeleteZ { ww w } {
    global tmploc tmplab
    global LOCATIONS_Z LABELS_Z
    global LEI_z
    global nbticks_z


    set i $LEI_z

    if {$nbticks_z <= 2} {
	tk_messageBox -icon error -type ok -title "Deletion forbidden" -message "You must have at least 2 ticks"
	return
    }

    for {set j 1} {$j<=$nbticks_z} {incr j} {
	set tmploc($j) $LOCATIONS_Z($j)
	set tmplab($j) $LABELS_Z($j)

	if {$j == $i} {
	    break
	}
    }
    set fin [expr $nbticks_z-1]
    set index [expr $i+1]
    for {set j $i} {$j<=$fin} {incr j} {
	set ind [expr $j+1]
	set tmploc($j) $LOCATIONS_Z($ind)
	set tmplab($j) $LABELS_Z($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Z($j) $tmploc($j)
	set LABELS_Z($j) $tmplab($j)
    }

    set nbticks_z $fin

    set numpage 2
    PopUp $w $numpage
    
}

# jb Silvy
# replace the entry wid and call TickApply
proc TicksApplyZReplace { w wid side } {
    $wid configure -justify $side
    TicksApplyZ $w ;
}

proc TicksApplyZ { w } {
    global LOCATIONS_Z LABELS_Z
    global nbticks_z
    global ZautoticksToggle

    #First, we pass the TLIST in 2 arrays to scilab
    #LOCATIONS_Z
    if { $nbticks_z == 0 } {
        return
    }
    for {set i 1} {$i<=$nbticks_z} {incr i} {
	set index $i
	if { $i == 1 } {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab($LOCATIONS_Z($index),$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab2($LOCATIONS_Z($index),$index,ged_tmp_LOCATIONS)"  "seq"
	}
    }
     #LABELS_Z
    for {set i 1} {$i<=$nbticks_z} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LABELS=GetTab('$LABELS_Z($index)',$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LABELS=GetTab2('$LABELS_Z($index)',$index,ged_tmp_LABELS)"  "seq"
	}
    }
    
    ScilabEval "setTicksTList('Z',ged_tmp_LOCATIONS,ged_tmp_LABELS)" "seq"
    
    set ZautoticksToggle "off"
    OnOffForeground $w.frame.zautoticksb $ZautoticksToggle
}




proc setLEI_x { i wi } {
    global LEI_x

    set LEI_x $i
    $wi configure -justify left
#    return $LEI
}

proc setLEI_y { i wid } {
    global LEI_y

    set LEI_y $i
    $wid configure -justify left
#    return $LEI
}

proc setLEI_z { i wid } {
    global LEI_z

    set LEI_z $i
    $wid configure -justify left
#    return $LEI
}

proc toggleXautoticks { win numpage frame } {
    global XautoticksToggle
    
    ScilabEval "global ged_handle;ged_handle.auto_ticks(1)='$XautoticksToggle'" "seq"
    ScilabEval "TCL_EvalStr(\"Reload_and_popup \"+\"$win $numpage\")" "seq"

    OnOffForeground $frame $XautoticksToggle
}

proc toggleYautoticks { win numpage frame } {
    global YautoticksToggle
    
    ScilabEval "global ged_handle;ged_handle.auto_ticks(2)='$YautoticksToggle'" "seq"
    ScilabEval "TCL_EvalStr(\"Reload_and_popup \"+\"$win $numpage\")" "seq"

    OnOffForeground $frame $YautoticksToggle
}

proc toggleZautoticks { win numpage frame } {
    global ZautoticksToggle
    
    ScilabEval "global ged_handle;ged_handle.auto_ticks(3)='$ZautoticksToggle'" "seq"
    ScilabEval "TCL_EvalStr(\"Reload_and_popup \"+\"$win $numpage\")" "seq"

    OnOffForeground $frame $ZautoticksToggle
}


proc SetStep { w numpage xyz} {
    # I rely on a.data_bounds data
    variable min max
    variable step
    global StepEntryX StepEntryY StepEntryZ
    global dbxmin dbxmax dbymin dbymax dbzmin dbzmax
    global LOCATIONS_X LABELS_X
    global LOCATIONS_Y LABELS_Y
    global LOCATIONS_Z LABELS_Z
    global nbticks_x nbticks_y nbticks_z
    global ticksNoteBook
    global fen1 fen2 fen3
    

    if { $xyz == 1 } {
        set step $StepEntryX
        if { $step=="" } {
            return
        }
	set min $dbxmin
	set max $dbxmax
	set nbticks_x 0
    } else {
	if { $xyz == 2 } {
            set step $StepEntryY
            if { $step=="" } {
                return
            }
	    set min $dbymin
	    set max $dbymax
	    set nbticks_y 0
	    
	} else {
	    if { $xyz == 3 } {
                set step $StepEntryZ
                if { $step=="" } {
                    return
                }
		set min $dbzmin
		set max $dbzmax
		set nbticks_z 0
	    }
	}
    }
    
    set min [expr floor($min)]
    set max [expr ceil ($max)]
    
    set x 0
    set i 0
    
#     puts "tout debut"
#     puts "min = $min"
#     puts "max = $max"
    
    if { $step <= 0 } {
	tk_messageBox -icon error -type ok -title "Wrong Step" -message "The step must be positive !"
        return
    }
    
    
    set x $min
    
    while {$x<$max} {

	set ii [expr $i+1]
	if { $xyz == 1 } {
	    set LOCATIONS_X($ii) $x
	    set LABELS_X($ii) $x
	    set nbticks_x [expr $nbticks_x +1]
	} else {
	    if { $xyz == 2 } {
		set LOCATIONS_Y($ii) $x
		set LABELS_Y($ii) $x
		set nbticks_y [expr $nbticks_y +1]
	    } else {
		if { $xyz == 3 } {
		    set LOCATIONS_Z($ii) $x
		    set LABELS_Z($ii) $x
		    set nbticks_z [expr $nbticks_z +1]
		}
	    }
	}
	incr i
	set x [expr $min + $i*$step]
    }

#     puts "AVANT avant one more to finish"
  
#     puts "i= $i"
    # one more to finish
#    incr i
    set ii [expr $i+1]
#     puts "avant one more to finish"
#     puts "i= $i"
#     puts "step = $step"
#     puts "min = $min"
    set x $max
    if { $xyz == 1 } {
	set LOCATIONS_X($ii) $x
	set LABELS_X($ii) $x
	set nbticks_x [expr $nbticks_x +1]
        TicksApplyX $fen1
        
    } else {
	if { $xyz == 2 } {
	    set LOCATIONS_Y($ii) $x
	    set LABELS_Y($ii) $x
	    set nbticks_y [expr $nbticks_y +1]
            TicksApplyY $fen2
            
	} else {
	    if { $xyz == 3 } {
		set LOCATIONS_Z($ii) $x
		set LABELS_Z($ii) $x
		set nbticks_z [expr $nbticks_z +1]
                TicksApplyZ $fen3
            }
	}
    }
    
    set curPage [Notebook:getCurrentPage $ticksNoteBook]
    
    # save the current position
    SavePreferences2 .ticks
    # redraw the modified page
    PopUp $w $numpage
    # and popup the current
    #PopUp $w $curPage
    Notebook:raise.page $ticksNoteBook $curPage
}



proc SetSubticksX { } {
    global SubticksEntryX
    
    if {$SubticksEntryX==""} {
        return
    }
    if {$SubticksEntryX < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect sub ticks value" -message "Select a positive integer value"
	return
    }
    ScilabEval "Subtickstoggle($SubticksEntryX,1)" "seq"
}


proc SetSubticksY { } {
    global SubticksEntryY
    
    if {$SubticksEntryY==""} {
        return
    }
    if {$SubticksEntryY < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect sub ticks value" -message "Select a positive integer value"
	return
    }
    ScilabEval "Subtickstoggle($SubticksEntryY,2)" "seq"
}

proc SetSubticksZ { } {
    global SubticksEntryZ
    
    if {$SubticksEntryZ==""} {
        return
    }
    if {$SubticksEntryZ < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect sub ticks value" -message "Select a positive integer value"
	return
    }
    ScilabEval "Subtickstoggle($SubticksEntryZ,3)" "seq"
}

proc SavePreferences { } {
    global ged_listofpref
    global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
    global ww msdos
    
#    ScilabEval "DestroyGlobals()" "seq"
        
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

proc SavePreferences2 { w } {
    global ged_listofpref
    global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
    global ww msdos
    
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

    set x [eval {winfo x $w}]
    set y [eval {winfo y $w}]
    
    # here is the difference with SavePreference
    set TICK_WINDOW_POSITION "+[expr $x+$xoffset]+[expr $y+$yoffset]"
    
    
    #save preferences (position...)
    set preffilename [file join $envSCIHOME .GedPreferences.tcl]
    catch {
 	set preffile [open $preffilename w]
 	foreach opt $ged_listofpref {
 	    global $opt
  	    puts $preffile [concat "set $opt" [set $opt]]
# 	    puts [concat "set $opt" [set $opt]]
 	}
	close $preffile
    }
    
}


# to be called when the window is closed
proc closeTicksWindow { wind } {
    global StepEntryX StepEntryY StepEntryZ
    
    # remove the step by entry texts
    set StepEntryX  ""
    set StepEntryY  ""
    set StepEntryZ  ""
    SavePreferences2 $wind
    destroy $wind
}

proc DestroyGlobals { } {
    global ww
    ScilabEval "DestroyGlobals()" "seq"

    SavePreferences
    
    ScilabEval "TCL_EvalStr(\"destroy $ww\")" "seq"
}


proc Reload_and_popup { win numpage } {
    global LOCATIONS_X LABELS_X
    global LOCATIONS_Y LABELS_Y
    global LOCATIONS_Z LABELS_Z
    global nbticks_x nbticks_y nbticks_z
    
    ScilabEval "global ged_handle; ReLoadTicks2TCL(ged_handle); " "seq"
    ScilabEval "TCL_EvalStr(\"PopUp \"+\"$win $numpage\")" "seq"

}

# unpush a button and puts the on label in grey
proc uncheckButton { button } {
    OnOffForeground $button "off"
    $button deselect
}

proc toggleReverselabx { frame } {
    global Xaxes_reverseToggle
    ScilabEval "global ged_handle;ged_handle.axes_reverse(1)='$Xaxes_reverseToggle'"

    OnOffForeground $frame $Xaxes_reverseToggle

}

proc toggleReverselaby { frame } {
    global Yaxes_reverseToggle
    ScilabEval "global ged_handle;ged_handle.axes_reverse(2)='$Yaxes_reverseToggle'"

    OnOffForeground $frame $Yaxes_reverseToggle
}

proc toggleReverselabz { frame } {
    global Zaxes_reverseToggle
    ScilabEval "global ged_handle;ged_handle.axes_reverse(3)='$Zaxes_reverseToggle'"

    OnOffForeground $frame $Zaxes_reverseToggle
}


proc setFontAngle_x { button } {
    global curfontangle_x2
    global curfontangle_x
    global xauto_rotation

    set fa $curfontangle_x
    ScilabEval "global ged_handle;if ged_handle.x_label.font_angle <> $fa then ged_handle.x_label.font_angle=$fa; end;"
    set curfontangle_x2 $fa
    
    # uncheck auto rotation
    set xauto_rotation "off"
    uncheckButton $button
}

proc setEntryFontAngle_x { button } {
    global curfontangle_x
    global curfontangle_x2
    global xauto_rotation
    if { $curfontangle_x2 == "" } {
        return
    }
    if { $curfontangle_x2 < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect angle" -message "Set a correct angle value in degree from 0 to 360"
	return
    }
    
    ScilabEval "global ged_handle;if ged_handle.x_label.font_angle <> $curfontangle_x2 then ged_handle.x_label.font_angle=$curfontangle_x2; end;"
    set curfontangle_x $curfontangle_x2
    
    # uncheck auto rotation
    set xauto_rotation "off"
    uncheckButton $button
}


proc setFontAngle_y { button } {
    global curfontangle_y
    global curfontangle_y2
    global yauto_rotation
    
    set fa $curfontangle_y
    ScilabEval "global ged_handle;if ged_handle.y_label.font_angle <> $fa then ged_handle.y_label.font_angle=$fa; end;"
    set curfontangle_y2 $fa
    
    # uncheck auto rotation
    set yauto_rotation "off"
    uncheckButton $button
}

proc setEntryFontAngle_y { button } {
    global curfontangle_y
    global curfontangle_y2
    global yauto_rotation

    if { $curfontangle_y2 == "" } {
        return
    }
    if { $curfontangle_y2 < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect angle" -message "Set a correct angle value in degree from 0 to 360"
	return
    }
    ScilabEval "global ged_handle;if ged_handle.y_label.font_angle <> $curfontangle_y2 then ged_handle.y_label.font_angle=$curfontangle_y2; end;"
    set curfontangle_y $curfontangle_y2

    # uncheck auto rotation
    set yauto_rotation "off"
    uncheckButton $button
}


proc setFontAngle_z { button } {
    global curfontangle_z
    global curfontangle_z2
    global zauto_rotation
    
    set fa $curfontangle_z
    ScilabEval "global ged_handle;if ged_handle.z_label.font_angle <> $fa then ged_handle.z_label.font_angle=$fa; end;"
    set curfontangle_z2 $fa
    
    # uncheck auto rotation
    set zauto_rotation "off"
    uncheckButton $button

}

proc setEntryFontAngle_z { button } {
    global curfontangle_z
    global curfontangle_z2
    global zauto_rotation

    if { $curfontangle_z2 == "" } {
        return
    }
    if { $curfontangle_z2 < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect angle" -message "Set a correct angle value in degree from 0 to 360"
	return
    }
    ScilabEval "global ged_handle;if ged_handle.z_label.font_angle <> $curfontangle_z2 then ged_handle.z_label.font_angle=$curfontangle_z2; end;"
    set curfontangle_z $curfontangle_z2

    # uncheck auto rotation
    set zauto_rotation "off"
    uncheckButton $button

}


proc setFontAngle_title { button } {
    global curfontangle_title
    global curfontangle_title2
    global titleauto_rotation

    set fa $curfontangle_title
    ScilabEval "global ged_handle;if ged_handle.title.font_angle <> $fa then ged_handle.title.font_angle=$fa; end;"
    set curfontangle_title2 $fa

    # uncheck auto rotation
    set titleauto_rotation "off"
    uncheckButton $button

}

proc setEntryFontAngle_title { button } {
    global curfontangle_title
    global curfontangle_title2
    global titleauto_rotation

    if { $curfontangle_title2 == "" } {
        return
    }
    if { $curfontangle_title2 < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect angle" -message "Set a correct angle value in degree from 0 to 360"
	return
    }
    ScilabEval "global ged_handle;if ged_handle.title.font_angle <> $curfontangle_title2 then ged_handle.title.font_angle=$curfontangle_title2; end;"
    set curfontangle_title $curfontangle_title2

    # uncheck auto rotation
    set titleauto_rotation "off"
    uncheckButton $button

}


proc XtoggleFillmode { frame } {
    global Xfillmode
    ScilabEval "global ged_handle;ged_handle.x_label.fill_mode='$Xfillmode'"

    OnOffForeground $frame $Xfillmode
}


proc YtoggleFillmode { frame } {
    global Yfillmode
    ScilabEval "global ged_handle;ged_handle.y_label.fill_mode='$Yfillmode'"

    OnOffForeground $frame $Yfillmode
}


proc ZtoggleFillmode { frame } {
    global Zfillmode
    ScilabEval "global ged_handle;ged_handle.z_label.fill_mode='$Zfillmode'"

    OnOffForeground $frame $Zfillmode
}


proc TitletoggleFillmode { frame } {
    global Titlefillmode
    ScilabEval "global ged_handle;ged_handle.title.fill_mode='$Titlefillmode'"

    OnOffForeground $frame $Titlefillmode
}


proc toggleAutoPositionx { frame } {
    global xauto_position 
    ScilabEval "global ged_handle;ged_handle.x_label.auto_position='$xauto_position'"

    OnOffForeground $frame $xauto_position
}

proc toggleAutoPositiony { frame } {
    global yauto_position
    ScilabEval "global ged_handle;ged_handle.y_label.auto_position='$yauto_position'"

    OnOffForeground $frame $yauto_position
}

proc toggleAutoPositionz { frame } {
    global zauto_position
    ScilabEval "global ged_handle;ged_handle.z_label.auto_position='$zauto_position'"

    OnOffForeground $frame $zauto_position
}

proc toggleAutoPositiontitle { frame } {
    global titleauto_position
    ScilabEval "global ged_handle;ged_handle.title.auto_position='$titleauto_position'"

    OnOffForeground $frame $titleauto_position
}


proc setPosition_x { button } {
    global x_position
    global xauto_position
    if { $x_position == ""} {
        return
    }
    ScilabEval "global ged_handle;ged_handle.x_label.position = $x_position"
    
    # uncheck auto_position button
    set xauto_position "off"
    uncheckButton $button
}


proc setPosition_y { button } {
    global y_position
    global yauto_position
    if { $y_position == ""} {
	tk_messageBox -icon error -type ok -title "Incorrect entry" -message "Set a correct entry"
	return
    } 
    
    ScilabEval "global ged_handle;ged_handle.y_label.position = $y_position"
    
    # uncheck auto_position button
    set yauto_position "off"
    uncheckButton $button
}


proc setPosition_z { button } {
    global z_position
    global zauto_position
    if { $z_position == ""} {
	tk_messageBox -icon error -type ok -title "Incorrect entry" -message "Set a correct entry"
	return
    }
    ScilabEval "global ged_handle;ged_handle.z_label.position = $z_position"
    
    # uncheck auto_position button
    set zauto_position "off"
    uncheckButton $button
}


proc setPosition_title { button } {
    global title_position
    global titleauto_position
    if { $title_position == ""} {
	tk_messageBox -icon error -type ok -title "Incorrect entry" -message "Set a correct entry"
	return
    }
    ScilabEval "global ged_handle;ged_handle.title.position = $title_position"

     # uncheck auto_position button
    set titleauto_position "off"
    uncheckButton $button
}


proc toggleAutoRotationx { frame } {
    global xauto_rotation
    ScilabEval "global ged_handle;ged_handle.x_label.auto_rotation='$xauto_rotation'"
    OnOffForeground $frame $xauto_rotation
}

proc toggleAutoRotationy { frame } {
    global yauto_rotation
    ScilabEval "global ged_handle;ged_handle.y_label.auto_rotation='$yauto_rotation'"

    OnOffForeground $frame $yauto_rotation
}

proc toggleAutoRotationz { frame } {
    global zauto_rotation
    ScilabEval "global ged_handle;ged_handle.z_label.auto_rotation='$zauto_rotation'"

    OnOffForeground $frame $zauto_rotation
}

proc toggleAutoRotationtitle { frame } {
    global titleauto_rotation
    ScilabEval "global ged_handle;ged_handle.title.auto_rotation='$titleauto_rotation'"

    OnOffForeground $frame $titleauto_rotation
}

proc sciCommandXlabel {} {
    global scicomint_Xlabel

    set longueur [expr [string length $scicomint_Xlabel]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a string matrix\n(or use a macro call).\n to initialize the \"text\" field."
    } else {
        ScilabEval "global ged_handle ; if ( ged_handle.x_label.text <> $scicomint_Xlabel ) then, ged_handle.x_label.text = $scicomint_Xlabel ; end;" "seq"
    }
}

proc GUIEditXlabel  {} {
    ScilabEval "global ged_handle;editvar(\"ged_handle.x_label.text\")" "seq"
}

proc SelectXlabel  {w args} {
    global xlabel scicomint_Xlabel
    set finddbarray -1
    set dbarray "string array"
    set finddbarray [expr [string first $dbarray $xlabel]]

    if { ($xlabel == "----") || ($finddbarray != -1) } {
#	puts "nothing to do"
    } else {
	if { $xlabel ==  "Edit data..." } {
            GUIEditXlabel
	} else {
	    #enter a variable
	    set scicomint_Xlabel $xlabel
	    sciCommandXlabel
	}
    }
}


proc sciCommandYlabel {} {
    global scicomint_Ylabel

    set longueur [expr [string length $scicomint_Ylabel]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a string matrix\n(or use a macro call).\n to initialize the \"text\" field."
    } else {
        ScilabEval "global ged_handle ; if ( ged_handle.y_label.text <> $scicomint_Ylabel ) then, ged_handle.y_label.text = $scicomint_Ylabel ; end;" "seq"
    }
}

proc GUIEditYlabel  {} {
    ScilabEval "global ged_handle;editvar(\"ged_handle.y_label.text\")" "seq"
}

proc SelectYlabel  {w args} {
    global ylabel scicomint_Ylabel
    set finddbarray -1
    set dbarray "string array"
    set finddbarray [expr [string first $dbarray $ylabel]]

    if { ($ylabel == "----") || ($finddbarray != -1) } {
#	puts "nothing to do"
    } else {
	if { $ylabel ==  "Edit data..." } {
            GUIEditYlabel
	} else {
	    #enter a variable
	    set scicomint_Ylabel $ylabel
	    sciCommandYlabel
	}
    }
}


proc sciCommandZlabel {} {
    global scicomint_Zlabel

    set longueur [expr [string length $scicomint_Zlabel]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a string matrix\n(or use a macro call).\n to initialize the \"text\" field."
    } else {
        # jb Silvy I removed tkged
        ScilabEval "global ged_handle ; if ( ged_handle.z_label.text <> $scicomint_Zlabel ) then, ged_handle.z_label.text = $scicomint_Zlabel ; end;" "seq"
    }
}

proc GUIEditZlabel  {} {
    ScilabEval "global ged_handle;editvar(\"ged_handle.z_label.text\")" "seq"
}

proc SelectZlabel  {w args} {
    global zlabel scicomint_Zlabel
    set finddbarray -1
    set dbarray "string array"
    set finddbarray [expr [string first $dbarray $zlabel]]

    if { ($zlabel == "----") || ($finddbarray != -1) } {
#	puts "nothing to do"
    } else {
	if { $zlabel ==  "Edit data..." } {
            GUIEditZlabel
	} else {
	    #enter a variable
	    set scicomint_Zlabel $zlabel
	    sciCommandZlabel
	}
    }
}


proc sciCommandTextTitle {} {
    global scicomint_textTitle

    set longueur [expr [string length $scicomint_textTitle]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a string matrix\n(or use a macro call).\n to initialize the \"text\" field."
    } else {
        ScilabEval "global ged_handle ; if ( ged_handle.title.text <> $scicomint_textTitle ) then, ged_handle.title.text = $scicomint_textTitle ; end;" "seq"
    }
}

proc GUIEditTextTitle  {} {
    ScilabEval "global ged_handle;editvar(\"ged_handle.title.text\")" "seq"
}

proc SelectTextTitle  {w args} {
    global tlabel scicomint_textTitle
    set finddbarray -1
    set dbarray "string array"
    set finddbarray [expr [string first $dbarray $tlabel]]

    if { ($tlabel == "----") || ($finddbarray != -1) } {
#	puts "nothing to do"
    } else {
	if { $tlabel ==  "Edit data..." } {
            GUIEditTextTitle
	} else {
	    #enter a variable
	    set scicomint_textTitle $tlabel
	    sciCommandTextTitle
	}
    }
}


# F.Leray Other proc. maybe for futur update...
# NOT TAKEN INTO ACCOUNT (no distinction made between font_foregroun and foreground for now (08.08.05))


# proc setXFontLabelColor {w  index} {
#     global RED BLUE GREEN
#     variable REDCOL 
#     variable GRECOL 
#     variable BLUCOL
    
#     #ScilabEval "global ged_handle;"
#     if { $index == -2 } {
# 	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
# 	#like $index==-2: display white color
# 	set color [format \#%02x%02x%02x 255 255 255]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == -1 } {
# 	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
# 	#like $index==-1: display black color
# 	set color [format \#%02x%02x%02x 0 0 0]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == 0 } {
# 	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
# 	#like $index==1: display first color
# 	set REDCOL $RED(1) 
# 	set GRECOL $GREEN(1) 
# 	set BLUCOL $BLUE(1) 

# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

# 	$w config  -activebackground $color -troughcolor $color
#     } else { 
# 	ScilabEval "global ged_handle; if ged_handle.x_label.font_foreground <> $index then ged_handle.x_label.font_foreground=$index; end;"
	
# 	set REDCOL $RED($index) 
# 	set GRECOL $GREEN($index) 
# 	set BLUCOL $BLUE($index) 

# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
# 	$w config  -activebackground $color -troughcolor $color
	
#     }
# }


# proc setYFontLabelColor {w  index} {
#     global RED BLUE GREEN
#     variable REDCOL 
#     variable GRECOL 
#     variable BLUCOL
    
#     #ScilabEval "global ged_handle;"
#     if { $index == -2 } {
# 	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then ged_handle.y_label.font_foreground=$index; end;"
# 	#like $index==-2: display white color
# 	set color [format \#%02x%02x%02x 255 255 255]
# 	$w config  -activebackground $color -troughcolor $color

#     } elseif { $index == -1 } {
# 	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then  ged_handle.y_label.font_foreground=$index; end;"
# 	#like $index==-1: display black color
# 	set color [format \#%02x%02x%02x 0 0 0]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == 0 } {
# 	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then  ged_handle.y_label.font_foreground=$index; end;"
# 	#like $index==1: display first color
# 	set REDCOL $RED(1) 
# 	set GRECOL $GREEN(1) 
# 	set BLUCOL $BLUE(1) 

# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

# 	$w config  -activebackground $color -troughcolor $color
#     } else { 
# 	ScilabEval "global ged_handle;if ged_handle.y_label.font_foreground <> $index then  ged_handle.y_label.font_foreground=$index; end;"
	
# 	set REDCOL $RED($index) 
# 	set GRECOL $GREEN($index) 
# 	set BLUCOL $BLUE($index) 

# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

# 	$w config  -activebackground $color -troughcolor $color
	
#     }
# }

# proc setZFontLabelColor {w  index} {
#     global RED BLUE GREEN
#     variable REDCOL 
#     variable GRECOL 
#     variable BLUCOL
    
#     #ScilabEval "global ged_handle;"
#     if { $index == -2 } {
# 	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then ged_handle.z_label.font_foreground=$index; end;"
# 	#like $index==-2: display white color
# 	set color [format \#%02x%02x%02x 255 255 255]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == -1 } {
# 	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then  ged_handle.z_label.font_foreground=$index; end;"
# 	#like $index==-1: display black color
# 	set color [format \#%02x%02x%02x 0 0 0]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == 0 } {
# 	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then  ged_handle.z_label.font_foreground=$index; end;"
# 	#like $index==1: display first color
# 	set REDCOL $RED(1) 
# 	set GRECOL $GREEN(1) 
# 	set BLUCOL $BLUE(1) 

# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

# 	$w config  -activebackground $color -troughcolor $color
#     } else { 
# 	ScilabEval "global ged_handle;if ged_handle.z_label.font_foreground <> $index then  ged_handle.z_label.font_foreground=$index; end;"
	
# 	set REDCOL $RED($index) 
# 	set GRECOL $GREEN($index) 
# 	set BLUCOL $BLUE($index) 

# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

# 	$w config  -activebackground $color -troughcolor $color
	
#     }
# }


# proc setTitleFontLabelColor {w  index} {
#     global RED BLUE GREEN
#     variable REDCOL 
#     variable GRECOL 
#     variable BLUCOL
    
#     #ScilabEval "global ged_handle;"
#     if { $index == -2 } {
# 	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
# 	#like $index==-2: display white color
# 	set color [format \#%02x%02x%02x 255 255 255]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == -1 } {
# 	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
# 	#like $index==-1: display black color
# 	set color [format \#%02x%02x%02x 0 0 0]
# 	$w config  -activebackground $color -troughcolor $color
#     } elseif { $index == 0 } {
# 	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
# 	#like $index==1: display first color
# 	set REDCOL $RED(1) 
# 	set GRECOL $GREEN(1) 
# 	set BLUCOL $BLUE(1) 
	
# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

# 	$w config  -activebackground $color -troughcolor $color
#     } else { 
# 	ScilabEval "global ged_handle;if ged_handle.title.font_foreground <> $index then  ged_handle.title.font_foreground=$index; end;"
	
# 	set REDCOL $RED($index) 
# 	set GRECOL $GREEN($index) 
# 	set BLUCOL $BLUE($index) 
# 	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
# 	$w config  -activebackground $color -troughcolor $color
	
#     }
# }
