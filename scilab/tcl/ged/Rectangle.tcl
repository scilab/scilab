set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd



variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/tcl/BWidget-1.7.0"]
namespace inscope :: package require BWidget
package require BWidget

set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
package require combobox 2.3
catch {namespace import combobox::*}

global  curvis red green blue color filToggle curlinestyle curmarkstyle curmarkmode

global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox
global ncolors RED GREEN BLUE

global Hval Wval Xval Yval Zval

set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Rectangle Editor"
wm iconname $ww "RE"
wm geometry $ww 530x550

set topf  [frame $ww.topf]
set titf1 [TitleFrame $topf.titf1 -text "Graphic Editor"]

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

#set w $ww
#frame $w.frame -borderwidth 0
#pack $w.frame -anchor w -fill both

#Hierarchical selection
set lalist ""
for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
append lalist "\""
append lalist "$SELOBJECT($i)" 
append lalist "\" "
}

set curgedobject $SELOBJECT($curgedindex)


#Hiereachical viewer
set fra [frame $w.frame.view  -borderwidth 0]
pack $fra -in $w.frame  -side top  -fill x
#frame $w.frame.view  -borderwidth 0
#pack $w.frame.view  -in $w.frame  -side top  -fill x

#label $w.frame.selgedobjectlabel  -height 0 -text "Edit properties for:    " -width 0 
set lab [label $w.frame.selgedobjectlabel  -height 0 -text "Edit properties for:    " -width 0 ]
pack $lab -in $w.frame.view   -side left

set comb [combobox $w.frame.selgedobject \
	      -borderwidth 2 \
	      -highlightthickness 3 \
	      -maxheight 0 \
	      -width 3 \
	      -textvariable curgedobject \
	      -editable false \
	      -background white \
	      -command [list SelectObject ]]
pack $comb  -in $w.frame.view  -fill x
eval $w.frame.selgedobject list insert end $lalist
#pack $w.frame.selgedobjectlabel -in $w.frame.view   -side left
#pack $w.frame.selgedobject   -in $w.frame.view   -fill x


Notebook:create $uf.n -pages {Style Data Clipping} -pad 20  -height 400
pack $uf.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 10
pack $w.frame  -anchor w -fill both

#Visibility
frame $w.frame.vis  -borderwidth 0
pack $w.frame.vis  -in $w.frame -side top -fill x

label $w.frame.visiblelabel -height 0 -text "   Visibility:   " -width 0 
checkbutton $w.frame.visible  -textvariable curvis -indicatoron 1 \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel   -in $w.frame.vis  -side left
pack $w.frame.visible  -in $w.frame.vis  -side left -fill x -pady 2m -padx 2m


#Line Style
frame $w.frame.rectst  -borderwidth 0
pack $w.frame.rectst  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text " Line style:    " -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ]
eval $w.frame.style list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]
pack $w.frame.stylelabel -in $w.frame.rectst   -side left
pack $w.frame.style   -in $w.frame.rectst   -expand 1 -fill x -pady 2m -padx 2m


#Mark style
frame $w.frame.rectmarkst  -borderwidth 0
pack $w.frame.rectmarkst  -in $w.frame  -side top  -fill x

label $w.frame.markstylelabel  -height 0 -text "Mark style:   " -width 0 
combobox $w.frame.markstyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curmarkstyle \
    -editable false \
    -command [list SelectMarkStyle ]
eval $w.frame.markstyle list insert end [list "dot" "plus" "cross" "star" "diamond fill" "diamond" "triangle up" "triangle down" "trefle" "circle"]


pack $w.frame.markstylelabel  -in $w.frame.rectmarkst   -side left
pack $w.frame.markstyle   -in $w.frame.rectmarkst  -expand 1 -fill x -pady 2m -padx 2m

#Mark mode
frame $w.frame.rectmarkmode  -borderwidth 0
pack $w.frame.rectmarkmode  -in $w.frame  -side top  -fill x

label $w.frame.markmodelabel -height 0 -text "Mark mode: " -width 0 
checkbutton $w.frame.markmode  -textvariable curmarkmode -indicatoron 1 \
    -variable curmarkmode -onvalue "on" -offvalue "off" \
    -command "toggleMarkmode" 

pack $w.frame.markmodelabel  -in $w.frame.rectmarkmode  -side left
pack $w.frame.markmode   -in $w.frame.rectmarkmode  -side left  -fill x -pady 2m -padx 2m


#filled
frame $w.frame.fil  -borderwidth 0
pack $w.frame.fil  -in $w.frame  -side top  -fill x

label $w.frame.filledlabel -height 0 -text "         Filled: " -width 0 
checkbutton $w.frame.filled  -textvariable filToggle -indicatoron 1 \
    -variable filToggle  -onvalue "on" -offvalue "off" \
    -command "toggleFilled" 
pack $w.frame.filledlabel  -in $w.frame.fil  -side left
pack $w.frame.filled  -in $w.frame.fil   -side left -fill x -pady 2m -padx 2m


#Color scale
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "         Color: " -width 0 
#         -foreground $color
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color -in  $w.frame.clrf -side left  -expand 1 -fill x -pady 2m -padx 2m
$w.frame.color set $curcolor


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x
label $w.frame.scalelabel -height 0 -text "  Thickness: " -width 0 
scale $w.frame.thickness -orient horizontal -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk  -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.thickness set $curthick

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 10m  

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 

########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n Data]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#x 
frame $w.frame.lb1 -borderwidth 0
pack $w.frame.lb1  -in $w.frame -side top   -fill x
label $w.frame.labelul -text "  Upper-left point coordinates "
pack $w.frame.labelul -in  $w.frame.lb1 -side left

frame $w.frame.lb2 -borderwidth 0
pack $w.frame.lb2  -in $w.frame -side top   -fill x

frame $w.frame.lb21 -borderwidth 0
pack $w.frame.lb21  -in $w.frame -side top   -fill x

frame $w.frame.lb22 -borderwidth 0
pack $w.frame.lb22  -in $w.frame -side top   -fill x

label $w.frame.labelx -text "     X: "
entry $w.frame.datax -relief sunken  -textvariable Xval 
label $w.frame.labely -text "     Y: "
entry $w.frame.datay -relief sunken  -textvariable Yval 
label $w.frame.labelz -text "     Z: "
entry $w.frame.dataz -relief sunken  -textvariable Zval 

pack $w.frame.labelx  $w.frame.datax  -in  $w.frame.lb2 -side left  -fill x -pady 2m -padx 2m
pack $w.frame.labely  $w.frame.datay  -in  $w.frame.lb21 -side left -fill x -pady 2m -padx 2m 
pack $w.frame.labelz  $w.frame.dataz  -in  $w.frame.lb22 -side left -fill x -pady 2m -padx 2m 
bind  $w.frame.datax <Return> {SelectXval} 
bind  $w.frame.datay <Return> {SelectYval} 
bind  $w.frame.dataz <Return> {SelectZval} 

#----------------------------#
frame $w.frame.lb3 -borderwidth 0
pack $w.frame.lb3  -in $w.frame -side top   -fill x
label $w.frame.labelwh -text "  Width and height  "
pack $w.frame.labelwh -in  $w.frame.lb3 -side left

frame $w.frame.lb4 -borderwidth 0
pack $w.frame.lb4  -in $w.frame -side top   -fill x

frame $w.frame.lb41 -borderwidth 0
pack $w.frame.lb41  -in $w.frame -side top   -fill x

label $w.frame.labelw -text "     W: "
entry $w.frame.dataw -relief sunken  -textvariable Wval 
label $w.frame.labelh -text "     H: "
entry $w.frame.datah -relief sunken  -textvariable Hval 

pack $w.frame.labelw  $w.frame.dataw -in  $w.frame.lb4  -side left -fill x -pady 2m -padx 2m
pack $w.frame.labelh  $w.frame.datah -in  $w.frame.lb41 -side left -fill x -pady 2m -padx 2m
bind  $w.frame.dataw <Return> {SelectWval} 
bind  $w.frame.datah <Return> {SelectHval} 

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 23m  

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
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
pack $w9.frame.clpstat  -in $w9.frame -side top -fill x -pady 1.m

label $w9.frame.cliplabel  -height 0 -text "   Clip state:  " -width 0 
combobox $w9.frame.clip \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curclipstate\
    -editable false \
    -command [list SelectClipState ]
eval $w9.frame.clip list insert end [list "on" "off" "clipgrf"]

pack $w9.frame.cliplabel -in $w9.frame.clpstat   -side left
pack $w9.frame.clip -in $w9.frame.clpstat   -expand 1 -fill x -pady 1.m -padx 1.m

#clip box
frame $w9.frame.lb1 -borderwidth 0
pack $w9.frame.lb1  -in $w9.frame -side top   -fill x
label $w9.frame.labelul -text "  Clip box : upper-left point coordinates "
pack $w9.frame.labelul -in  $w9.frame.lb1 -side left

frame $w9.frame.lb2 -borderwidth 0
pack $w9.frame.lb2  -in $w9.frame -side top   -fill x

frame $w9.frame.lb21 -borderwidth 0
pack $w9.frame.lb21  -in $w9.frame -side top   -fill x

frame $w9.frame.lb22 -borderwidth 0
pack $w9.frame.lb22  -in $w9.frame -side top   -fill x

label $w9.frame.labelx -text "             X: "
entry $w9.frame.datax -relief sunken  -textvariable Xclipbox
label $w9.frame.labely -text "             Y: "
entry $w9.frame.datay -relief sunken  -textvariable Yclipbox

pack $w9.frame.labelx  $w9.frame.datax  -in  $w9.frame.lb2 -side left  -fill x -pady 1.m -padx 1.m
pack $w9.frame.labely  $w9.frame.datay  -in  $w9.frame.lb21 -side left -fill x -pady 1.m -padx 1.m 
bind  $w9.frame.datax <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <Return> "SelectClipBox $w9.frame"

#----------------------------#
frame $w9.frame.lb3 -borderwidth 0
pack $w9.frame.lb3  -in $w9.frame -side top   -fill x
label $w9.frame.labelwh -text "   Clip box : width and height  "
pack $w9.frame.labelwh -in  $w9.frame.lb3 -side left

frame $w9.frame.lb4 -borderwidth 0
pack $w9.frame.lb4  -in $w9.frame -side top   -fill x

frame $w9.frame.lb41 -borderwidth 0
pack $w9.frame.lb41  -in $w9.frame -side top   -fill x

label $w9.frame.labelw -text "             W: "
entry $w9.frame.dataw -relief sunken  -textvariable Wclipbox
label $w9.frame.labelh -text "             H: "
entry $w9.frame.datah -relief sunken  -textvariable Hclipbox

pack $w9.frame.labelw  $w9.frame.dataw -in  $w9.frame.lb4  -side left -fill x -pady 1.m -padx 1.m
pack $w9.frame.labelh  $w9.frame.datah -in  $w9.frame.lb41 -side left -fill x -pady 1.m -padx 1.m
bind  $w9.frame.dataw <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <Return> "SelectClipBox $w9.frame"


frame $w9.frame.warning
label $w9.frame.mesgwarning  -justify left -textvariable letext
$w9.frame.mesgwarning config -foreground red
pack $w9.frame.mesgwarning -in $w9.frame.warning
pack $w9.frame.warning -in $w9.frame

#sep bar
frame $w9.sep -height 2 -borderwidth 1 -relief sunken
pack $w9.sep -fill both  -pady 30m


#exit button
frame $w9.buttons
button $w9.b -text Quit -command "destroy $ww"
pack $w9.b -side bottom 


pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes



#proc associes
proc SelectObject {w args} {
    global curgedobject;
    global ged_handle_list_size;
    global SELOBJECT
    
    set index 1

    for {set i 1} {$i<=$ged_handle_list_size} {incr i} {
	if {$curgedobject==$SELOBJECT($i)} {
	    set index $i
	}
    }
    ScilabEval "Get_handle_from_index($index);"
}

proc setColor {w index} {    
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    }
}

proc setThickness {w thick} {
ScilabEval "global ged_handle;ged_handle.thickness=$thick;"
}

proc toggleVisibility {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
}
proc toggleFilled {} {
global filToggle
ScilabEval "global ged_handle;ged_handle.fill_mode='$filToggle'"
}

proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
}

proc SelectMarkStyle {w args} {
global curmarkstyle
global curmarkmode
ScilabEval "setMarkStyle('$curmarkstyle')"
set curmarkmode "on"
#ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"

}

proc toggleMarkmode {} {
global curmarkmode
ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"
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



