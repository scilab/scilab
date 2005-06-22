#Copyright INRIA
#2004
#Author: F.Leray

set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd



variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/tcl/BWidget-1.7.0"]
namespace inscope :: package require BWidget
package require BWidget


set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]
set sourcedir2 [file join "$env(SCIPATH)" "tcl" "ged"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
source [file join $sourcedir2 ObjectsBrowser.tcl]

package require combobox 2.3
catch {namespace import combobox::*}

#package require lemonTree
catch {namespace import LemonTree::*}

global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject

global curvis
global curforeground ncolors curfontstyle curfontsize curfontangle curtextbox1 curtextbox2
global curtextboxmode curtext
global RED BLUE GREEN
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox


#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 260
set NBwidth  250

set Wheight [expr $NBheight + 110]
set Wwidth  [expr $NBwidth  + 265]

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Text Object"
wm iconname $ww "TO"
wm geometry $ww [expr $Wwidth]x[expr $Wheight]
wm protocol $ww WM_DELETE_WINDOW "DestroyGlobals; destroy $ww "

set topf  [frame $ww.topf]
set titf1 [TitleFrame $topf.titf1 -text "Graphic Editor" -font {Arial 9}]

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
set titf1label [TitleFrame $topflabel.titflabel1 -text "Objects Browser" -font {Arial 9}]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Object Properties" -font {Arial 9}]

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
label $w.frame.vislabel  -text "Visibility:" -font {Arial 9} -anchor e -width $largeur
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib" -font {Arial 9}
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -padx 1m

###############
#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 0m

label $w.frame.fontcolorlabel -height 0 -text "Color:" -width 0  -font {Arial 9} -anchor e -width $largeur
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0  -font {Arial 9}

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 0m -padx 1m
$w.frame.fontcolor set $curforeground


#Fontsize scale
frame $w.frame.fontssz  -borderwidth 0
pack $w.frame.fontssz  -in $w.frame    -side top -fill x -pady 0m

label $w.frame.fontsizelabel -height 0 -text "Font size:" -width 0  -font {Arial 9} -anchor e -width $largeur
scale $w.frame.fontsize -orient horizontal  -from 0 -to 5 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0 -font {Arial 9}
pack $w.frame.fontsizelabel  -in $w.frame.fontssz -side left
pack $w.frame.fontsize -in $w.frame.fontssz   -expand 1 -fill x -pady 0m -padx 1m
$w.frame.fontsize set $curfontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0m

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0  -font {Arial 9} -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curfontstyle \
    -editable false \
    -command [list SelectFontStyle] -font {Arial 9}
eval $w.frame.style list insert end [list "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 0m -padx 2m

#Font Angle
frame $w.frame.fontsang  -borderwidth 0
pack $w.frame.fontsang  -in $w.frame    -side top -fill x -pady 0m

label $w.frame.fontanglelabel -height 0 -text "Font angle:" -width 0  -font {Arial 9} -anchor e -width $largeur
scale $w.frame.fontangle -orient horizontal  -from 0 -to 360 \
	 -resolution -1.0 -command "setFontAngle $w.frame.fontangle " -tickinterval 0 -font {Arial 9}
$w.frame.fontangle set $curfontangle
entry $w.frame.fontangle2 -relief sunken  -textvariable curfontangle2 -font {Arial 9} -width 15

bind  $w.frame.fontangle2 <Return> "setEntryFontAngle $w.frame.fontangle2 $w.frame.fontangle"
bind  $w.frame.fontangle2 <KP_Enter> 	"setEntryFontAngle $w.frame.fontangle2 $w.frame.fontangle"


pack $w.frame.fontanglelabel  -in $w.frame.fontsang -side left
pack $w.frame.fontangle -in $w.frame.fontsang    -side left -padx 1m
pack $w.frame.fontangle2 -in $w.frame.fontsang   -expand 1 -fill x -pady 0m -padx 2m

#############

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0m
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww"  -font {Arial 9}
pack $w.buttons.dismiss  -side bottom -expand 1


########### Data & Mode onglet ####################################
###################################################################
set w [Notebook:frame $uf.n "Data & Mode"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#x label
frame $w.frame.lbx -borderwidth 0
pack $w.frame.lbx  -in $w.frame -side top -fill x -pady 0m

label $w.frame.xlabel -text "Text:" -font {Arial 9} -anchor e -width $largeur 
entry $w.frame.xlabel1 -relief sunken  -textvariable curtext -font {Arial 9} -width 15
pack $w.frame.xlabel -in  $w.frame.lbx -side left
pack $w.frame.xlabel1  -in  $w.frame.lbx  -side left -pady 0m -padx 2m
bind  $w.frame.xlabel1 <Return> {setText} 
bind  $w.frame.xlabel1 <KP_Enter> {setText} 


#Text box mode
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 0m

label $w.frame.stylelabel  -height 0 -text "Text box mode:" -width 0  -font {Arial 9} -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 13 \
    -textvariable curtextboxmode \
    -editable false \
    -command [list SelectTextBoxMode] -font {Arial 9}
eval $w.frame.style list insert end [list "off" "centered" "filled"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -side left -pady 0m -padx 2m


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0m
button $w.buttons.dismiss -text Quit -command "destroy $ww"  -font {Arial 9}
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
pack $w9.frame.clpstat  -in $w9.frame -side top -fill x -pady 0m

label $w9.frame.cliplabel  -height 0 -text "Clip state:" -width 0  -font {Arial 9} -anchor e -width $largeur
combobox $w9.frame.clip \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 8 \
    -textvariable curclipstate\
    -editable false \
    -command [list SelectClipState ] -font {Arial 9}
eval $w9.frame.clip list insert end [list "on" "off" "clipgrf"]

pack $w9.frame.cliplabel -in $w9.frame.clpstat   -side left
pack $w9.frame.clip -in $w9.frame.clpstat  -side left -pady 0m -padx 2m

#clip box
frame $w9.frame.lb1 -borderwidth 0
pack $w9.frame.lb1  -in $w9.frame -side top   -fill x
label $w9.frame.labelul -text "Clip box : upper-left point coordinates" -font {Arial 9}
pack $w9.frame.labelul -in  $w9.frame.lb1 -side left

frame $w9.frame.lb2 -borderwidth 0
pack $w9.frame.lb2  -in $w9.frame -side top   -fill x

frame $w9.frame.lb21 -borderwidth 0
pack $w9.frame.lb21  -in $w9.frame -side top   -fill x

frame $w9.frame.lb22 -borderwidth 0
pack $w9.frame.lb22  -in $w9.frame -side top   -fill x

label $w9.frame.labelx -text "X:" -font {Arial 9}  -anchor e -width $largeur
entry $w9.frame.datax -relief sunken  -textvariable Xclipbox -width 10  -font {Arial 9}
label $w9.frame.labely -text "Y:" -font {Arial 9} -anchor e -width $largeur
entry $w9.frame.datay -relief sunken  -textvariable Yclipbox -width 10  -font {Arial 9}

pack $w9.frame.labelx  -in  $w9.frame.lb2 -side left 
pack $w9.frame.datax   -in  $w9.frame.lb2 -side left -pady 0m -padx 2m
pack $w9.frame.labely  -in  $w9.frame.lb21 -side left 
pack $w9.frame.datay   -in  $w9.frame.lb21 -side left -pady 0m -padx 2m
bind  $w9.frame.datax <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datax <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <KP_Enter> "SelectClipBox $w9.frame"

#----------------------------#
frame $w9.frame.lb3 -borderwidth 0
pack $w9.frame.lb3  -in $w9.frame -side top   -fill x
label $w9.frame.labelwh -text "Clip box : width and height" -font {Arial 9}
pack $w9.frame.labelwh -in  $w9.frame.lb3 -side left

frame $w9.frame.lb4 -borderwidth 0
pack $w9.frame.lb4  -in $w9.frame -side top   -fill x

frame $w9.frame.lb41 -borderwidth 0
pack $w9.frame.lb41  -in $w9.frame -side top   -fill x

label $w9.frame.labelw -text "W:" -font {Arial 9} -anchor e -width $largeur
entry $w9.frame.dataw -relief sunken  -textvariable Wclipbox -width 10  -font {Arial 9}
label $w9.frame.labelh -text "H:" -font {Arial 9} -anchor e -width $largeur
entry $w9.frame.datah -relief sunken  -textvariable Hclipbox -width 10  -font {Arial 9}

pack $w9.frame.labelw  -in  $w9.frame.lb4 -side left 
pack $w9.frame.dataw   -in  $w9.frame.lb4 -side left -pady 0m -padx 2m
pack $w9.frame.labelh  -in  $w9.frame.lb41 -side left 
pack $w9.frame.datah   -in  $w9.frame.lb41 -side left -pady 0m -padx 2m
bind  $w9.frame.dataw <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.dataw <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <KP_Enter> "SelectClipBox $w9.frame"


frame $w9.frame.warning
label $w9.frame.mesgwarning  -justify left -textvariable letext -font {Arial 9}
$w9.frame.mesgwarning config -foreground red
pack $w9.frame.mesgwarning -in $w9.frame.warning
pack $w9.frame.warning -in $w9.frame

#sep bar
frame $w9.sep -height 2 -borderwidth 1 -relief sunken
pack $w9.sep -fill both


#exit button
frame $w9.buttons
button $w9.b -text Quit -command "DestroyGlobals; destroy $ww" -font {Arial 9}
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

proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"
}