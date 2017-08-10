# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2004 - INRIA - Fabrice Leray
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
global curfontforeground ncolors curfontstyle curfontsize curfontangle
global curtextboxmode curtext
global RED BLUE GREEN
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox
global curboxmode curlinemode curlfillmode curforeground curbackground
global curtext
global textBoxWidth textBoxHeight
global curAlignment

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 310
set NBwidth  250

set Wheight [expr $NBheight + 185]
set Wwidth  [expr $NBwidth  + 270]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Text Object"
wm iconname $ww "TO"
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

set largeur 12

Notebook:create $uf.n -pages {Style "Data & Mode" "Clipping"} -pad 0 -height $NBheight -width $NBwidth
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
pack $w.frame.visib  -in $w.frame.vis    -side left -padx $smallPad

#Box mode
frame $w.frame.boxmode  -borderwidth 0
pack $w.frame.boxmode  -in $w.frame -side top   -fill x -pady 0

label $w.frame.boxmodelabel -height 0 -text "Box :" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.boxmodevalue  -text "on"\
    -variable curboxmode  -onvalue "on" -offvalue "off" \
    -command "toggleBoxmode $w.frame.boxmodevalue" -font $gedFont
OnOffForeground $w.frame.boxmodevalue $curboxmode

pack $w.frame.boxmodelabel -in $w.frame.boxmode  -side left
pack $w.frame.boxmodevalue  -in $w.frame.boxmode    -side left -padx $smallPad

#line mode
frame $w.frame.linemode  -borderwidth 0
pack $w.frame.linemode  -in $w.frame -side top   -fill x -pady 0
label $w.frame.linemodelabel -height 0 -text "Line mode :" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.linemodevalue  -text "on"\
    -variable curlinemode  -onvalue "on" -offvalue "off" \
    -command "toggleLineMode $w.frame.linemodevalue" -font $gedFont
OnOffForeground $w.frame.linemodevalue $curlinemode

pack $w.frame.linemodelabel -in $w.frame.linemode  -side left
pack $w.frame.linemodevalue  -in $w.frame.linemode    -side left -padx $smallPad

#fill mode
frame $w.frame.fillmode  -borderwidth 0
pack $w.frame.fillmode  -in $w.frame -side top   -fill x -pady 0
label $w.frame.fillmodelabel -height 0 -text "Fill mode :" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.fillmodevalue  -text "on"\
    -variable curfillmode  -onvalue "on" -offvalue "off" \
    -command "toggleFillMode $w.frame.fillmodevalue" -font $gedFont
OnOffForeground $w.frame.fillmodevalue $curfillmode

pack $w.frame.fillmodelabel -in $w.frame.fillmode  -side left
pack $w.frame.fillmodevalue  -in $w.frame.fillmode    -side left -padx $smallPad

###############
#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 0

label $w.frame.fontcolorlabel -height 0 -text "Font color:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0  -font $gedFont

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontcolor set $curfontforeground

#Fontsize scale
frame $w.frame.fontssz  -borderwidth 0
pack $w.frame.fontssz  -in $w.frame    -side top -fill x -pady 0

label $w.frame.fontsizelabel -height 0 -text "Font size:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal  -from 0 -to 5 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0 -font $gedFont
pack $w.frame.fontsizelabel  -in $w.frame.fontssz -side left
pack $w.frame.fontsize -in $w.frame.fontssz   -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.fontsize set $curfontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curfontstyle \
    -editable false \
    -command [list SelectFontStyle] -font $gedFont
eval $w.frame.style list insert end [list "Monospaced" "Symbol" "Serif" "Serif Italic" "Serif Bold" "Serif Bold Italic"  "SansSerif"  "SansSerif Italic" "SansSerif Bold" "SansSerif Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 0 -padx $mediumPad

#Font Angle
frame $w.frame.fontsang  -borderwidth 0
pack $w.frame.fontsang  -in $w.frame    -side top -fill x -pady 0

label $w.frame.fontanglelabel -height 0 -text "Font angle:" -width 0  -font $gedFont -anchor e -width $largeur
scale $w.frame.fontangle -orient horizontal  -from 0 -to 360 \
	 -resolution 1.0 -command "setFontAngle $w.frame.fontangle " -tickinterval 0 -font $gedFont
$w.frame.fontangle set $curfontangle
entry $w.frame.fontangle2 -relief sunken  -textvariable curfontangle2 -font $gedFont -width 15

bind  $w.frame.fontangle2 <Return> "setEntryFontAngle $w.frame.fontangle2 $w.frame.fontangle"
bind  $w.frame.fontangle2 <KP_Enter> 	"setEntryFontAngle $w.frame.fontangle2 $w.frame.fontangle"
bind  $w.frame.fontangle2 <FocusOut> 	"setEntryFontAngle $w.frame.fontangle2 $w.frame.fontangle"

#Foreground scale
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Foreground:" -width 0  -font $gedFont -anchor e -width $largeur
#         -foreground $color
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setForeground $w.frame.color" -tickinterval 0  -font $gedFont

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color -in  $w.frame.clrf -side left  -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.color set $curforeground

#Background scale (line)
frame $w.frame.backg  -borderwidth 0
pack $w.frame.backg  -in $w.frame -side top  -fill x

label $w.frame.backlabel -height 0 -text "Background:" -font $gedFont -anchor e -width $largeur
#         -foreground $back
scale $w.frame.back -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackground $w.frame.back" -tickinterval 0  -font $gedFont

pack $w.frame.backlabel -in $w.frame.backg -side left
pack $w.frame.back  -in  $w.frame.backg -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.back set $curbackground


pack $w.frame.fontanglelabel  -in $w.frame.fontsang -side left
pack $w.frame.fontangle -in $w.frame.fontsang    -side left -padx $smallPad
pack $w.frame.fontangle2 -in $w.frame.fontsang   -expand 1 -fill x -pady 0 -padx $mediumPad

#############

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.buttons.dismiss  -side bottom -expand 1


########### Data & Mode onglet ####################################
###################################################################
set w [Notebook:frame $uf.n "Data & Mode"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


### Text data ###
set mycurtext $curtext

frame $w.frame.textframe  -borderwidth 0
pack $w.frame.textframe  -in $w.frame  -side top  -fill x

label $w.frame.textlabel  -height 0 -text "Text:" -width 0  -font $gedFont  -anchor e -width $largeur
combobox $w.frame.text \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curtext \
    -editable true \
    -command [list SelectData ] -font $gedFont
eval $w.frame.text list insert end [list $mycurtext "----" "Edit data..."]
pack $w.frame.textlabel -in $w.frame.textframe  -side left
pack $w.frame.text   -in $w.frame.textframe  -expand 1 -fill x -pady $smallPad -padx $mediumPad




### Text box mode ###
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0

label $w.frame.stylelabel  -height 0 -text "Text box mode:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 13 \
    -textvariable curtextboxmode \
    -editable false \
    -command [list SelectTextBoxMode] -font $gedFont
eval $w.frame.style list insert end [list "off" "centered" "filled"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -side left -pady $smallPad -padx $mediumPad

#### Text box ###
frame $w.frame.textBoxFrame  -borderwidth 0
pack $w.frame.textBoxFrame  -in $w.frame -side top -fill x -pady 0

#display label
label $w.frame.labelTextBox -text "Text Box: Width and Height:" -font $gedFont
pack $w.frame.labelTextBox -in $w.frame.textBoxFrame -side left

#create the 2 entity boxes
frame $w.frame.textBoxWidth -borderwidth 0
pack $w.frame.textBoxWidth -in $w.frame -side top -fill x

label $w.frame.labelTbWidth -text  "Width:" -font $gedFont  -anchor e -width $largeur
entry $w.frame.tbWidthEntry -relief sunken -textvariable textBoxWidth -width 10 -font $gedFont

pack $w.frame.labelTbWidth -in $w.frame.textBoxWidth -side left
pack $w.frame.tbWidthEntry -in $w.frame.textBoxWidth -side left -fill x -pady $smallPad -padx $mediumPad

bind $w.frame.tbWidthEntry <Return>   { setTextBox }
bind $w.frame.tbWidthEntry <KP_Enter> { setTextBox }

frame $w.frame.textBoxHeight -borderwidth 0
pack $w.frame.textBoxHeight -in $w.frame -side top -fill x

label $w.frame.labelTbHeight -text  "Height:" -font $gedFont -anchor e -width $largeur
entry $w.frame.tbHeightEntry -relief sunken -textvariable textBoxHeight -width 10 -font $gedFont

pack $w.frame.labelTbHeight -in $w.frame.textBoxHeight -side left
pack $w.frame.tbHeightEntry -in $w.frame.textBoxHeight -side left -padx $mediumPad

bind $w.frame.tbHeightEntry <Return>   {setTextBox}
bind $w.frame.tbHeightEntry <KP_Enter> {setTextBox}


### alignment ###
frame $w.frame.alignment  -borderwidth 0
pack $w.frame.alignment  -in $w.frame -side top -fill x -pady 0

label $w.frame.alignLabel  -height 0 -text "Alignment:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.alignMenu \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 13 \
    -textvariable curAlignment \
    -editable false \
    -command [list setTextAlignment] -font $gedFont
eval $w.frame.alignMenu list insert end [list "left" "center" "right"]

pack $w.frame.alignLabel -in $w.frame.alignment   -side left
pack $w.frame.alignMenu -in $w.frame.alignment  -side left -pady $smallPad -padx $mediumPad



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0
button $w.buttons.dismiss -text Quit -command "destroy $ww"  -font $gedFont
pack $w.buttons.dismiss  -side bottom -expand 1


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


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}


proc setFontColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.font_foreground <> $index then ged_handle.font_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.font_foreground <> $index then ged_handle.font_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.font_foreground <> $index then ged_handle.font_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.font_foreground <> $index then ged_handle.font_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}

proc setFontSize {w fs} {
    ScilabEval "global ged_handle; if ged_handle.font_size <> $fs then ged_handle.font_size=$fs; end;"
}


proc SelectFontStyle {w args} {
global curfontstyle
ScilabEval "setFontStyle('$curfontstyle')"
}

proc setFontAngle {w fa} {
    global curfontangle2
    ScilabEval "global ged_handle;if ged_handle.font_angle <> $fa then ged_handle.font_angle=$fa; end;"
    set curfontangle2 $fa
}

proc setEntryFontAngle {w w2 args} {
    global curfontangle
    global curfontangle2
    if { $curfontangle2 == "" || $curfontangle2 < 0} {
	tk_messageBox -icon error -type ok -title "Incorrect angle" -message "Set a correct angle value in degree from 0 to 360"
	return
    }
    ScilabEval "global ged_handle;if ged_handle.font_angle <> $curfontangle2 then ged_handle.font_angle=$curfontangle2; end;"
    set curfontangle $curfontangle2
    $w2 set $curfontangle
}

proc setText {} {
global curtext
ScilabEval "global ged_handle;ged_handle.text='$curtext'"
}


proc SelectTextBoxMode {w args} {
global curtextboxmode
ScilabEval "global ged_handle;ged_handle.text_box_mode='$curtextboxmode'"
}

proc toggleBoxmode { frame } { #fill mode for text object
    global curboxmode
    ScilabEval "global ged_handle;ged_handle.box='$curboxmode'"
    
    OnOffForeground $frame $curboxmode
}

proc toggleLineMode { frame } { #line mode for text object
    global curlinemode
    ScilabEval "global ged_handle;ged_handle.line_mode='$curlinemode'"
    
    OnOffForeground $frame $curlinemode
}

proc toggleFillMode { frame } { #line mode for text object
    global curfillmode
    ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"
    
    OnOffForeground $frame $curfillmode
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

proc setBackground {w index} {   
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


proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"

    SavePreferences
}


proc sciCommandData {} {
    global scicomint_text

    set longueur [expr [string length $scicomint_text]]
    
    if { $longueur == 0 } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must specify a variable defined in Scilab Console representing a string matrix\n(or use a macro call).\n to initialize the \"text\" field."
    } else {
	# check if it is needed to reload the page
        # jb Silvy 06/2006: I removed the call to tkged
        ScilabEval "global ged_handle ; if ( ged_handle.text <> $scicomint_text ) then, ged_handle.text = $scicomint_text ; end;" "seq"
    }
}

proc GUIEditData  {} {
    ScilabEval "global ged_handle;editvar(\"ged_handle.text\")" "seq"
}

proc SelectData  {w args} {
    global curtext scicomint_text
    set finddbarray -1
    set dbarray "string array"
    set finddbarray [expr [string first $dbarray $curtext]]

    if { ($curtext == "----") || ($finddbarray != -1) } {
#	puts "nothing to do"
    } else {
	if { $curtext ==  "Edit data..." } {
	    GUIEditData
	} else {
	    #enter a variable
	    set scicomint_text $curtext
	    sciCommandData
	}
    }
}

proc setTextBox {} {
    global textBoxWidth
    global textBoxHeight
        
    if { ($textBoxWidth == "") || ($textBoxHeight == "") } {
	tk_messageBox -icon error -type ok -title "Text Box error" -message "You must fill in the 2 fields."
	return
    }
    ScilabEval "global ged_handle;ged_handle.text_box=\[$textBoxWidth $textBoxHeight \]"
}

proc setTextAlignment {w args} {
    global curAlignment
    if { ($curAlignment == "" ) } {
        tk_messageBox -icon error -type ok -title "Alignment error" -message "You must fill in the field."
	return
    }
    ScilabEval "global ged_handle;ged_handle.alignment = \"$curAlignment\" "  
}
