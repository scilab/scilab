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

global curvis curthick curpolylinestyle curlinestyle RED GREEN BLUE
global polyVAL nbcol nbrow

global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject


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



set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Polyline Editor"
wm iconname $ww "PE"
wm geometry $ww 690x650

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

set comb [ combobox $w.frame.selgedobject \
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


#Notebook:create $uf.n -pages {Style Aspect Data} -pad 20     -height 520 -width 600
Notebook:create $uf.n -pages {Style Data} -pad 20 -height 520 -width 600
pack $uf.n -fill both -expand yes

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#Visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x

label $w.frame.visiblelabel -height 0 -text "       Visibility:    " -width 0 
checkbutton $w.frame.visible  -textvariable curvis -indicatoron 1 \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel  -in $w.frame.vis  -side left
pack $w.frame.visible  -in $w.frame.vis  -side left  -fill x -pady 2m -padx 2m


#Polyline Style
frame $w.frame.curvst  -borderwidth 0
pack $w.frame.curvst  -in $w.frame  -side top  -fill x

label $w.frame.polystylelabel  -height 0 -text "Polyline style:    " -width 0 
combobox $w.frame.polystyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curpolylinestyle \
    -editable false \
    -command [list SelectPolylineStyle ]
eval $w.frame.polystyle list insert end [list "interpolated" "staircase" "barplot" "arrowed" "filled"]
pack $w.frame.polystylelabel -in $w.frame.curvst   -side left
pack $w.frame.polystyle   -in $w.frame.curvst   -expand 1 -fill x -pady 2m -padx 2m

#Line Style
frame $w.frame.linest  -borderwidth 0
pack $w.frame.linest  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "     Line style:     " -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ]
eval $w.frame.style list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]
pack $w.frame.stylelabel -in $w.frame.linest   -side left
pack $w.frame.style   -in $w.frame.linest   -expand 1 -fill x -pady 2m -padx 2m

#Mark style
frame $w.frame.linemarkst  -borderwidth 0
pack $w.frame.linemarkst  -in $w.frame  -side top  -fill x

label $w.frame.markstylelabel  -height 0 -text "    Mark style:    " -width 0 
combobox $w.frame.markstyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curmarkstyle \
    -editable false \
    -command [list SelectMarkStyle ]
eval $w.frame.markstyle list insert end [list "dot" "plus" "cross" "star" "diamond fill" "diamond" "triangle up" "triangle down" "trefle" "circle"]


pack $w.frame.markstylelabel  -in $w.frame.linemarkst   -side left
pack $w.frame.markstyle   -in $w.frame.linemarkst   -expand 1 -fill x -pady 2m -padx 2m

#Mark mode
frame $w.frame.linemarkmode  -borderwidth 0
pack $w.frame.linemarkmode  -in $w.frame  -side top  -fill x

label $w.frame.markmodelabel -height 0 -text "    Mark mode:   " -width 0 
checkbutton $w.frame.markmode  -textvariable curmarkmode -indicatoron 1 \
    -variable curmarkmode -onvalue "on" -offvalue "off" \
    -command "toggleMarkmode" 

pack $w.frame.markmodelabel  -in $w.frame.linemarkmode  -side left 
pack $w.frame.markmode   -in $w.frame.linemarkmode   -side left  -fill x -pady 2m -padx 2m


#Color scale
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "             Color:   " -width 0 
#         -foreground $color
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color  -in  $w.frame.clrf -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.color set $curcolor


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x

label $w.frame.scalelabel -height 0 -text "      Thickness:   " -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk  -expand 1 -fill x -pady 2m -padx 2m
$w.frame.thickness set $curthick


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 30m  

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 


########### Aspect onglet #########################################
###################################################################
# set w [Notebook:frame $uf.n Aspect]

# frame $w.frame -borderwidth 0
# pack $w.frame  -anchor w -fill both


# #Clip state
# frame $w.frame.clpstat  -borderwidth 0
# pack $w.frame.clpstat  -in $w.frame -side top -fill x -pady 1.m

# label $w.frame.cliplabel  -height 0 -text "   Clip state:  " -width 0 
# combobox $w.frame.clip \
#     -borderwidth 1 \
#     -highlightthickness 1 \
#     -maxheight 0 \
#     -width 3 \
#     -textvariable curclipstate\
#     -editable false \
#     -command [list SelectClipState ]
# eval $w.frame.clip list insert end [list "on" "off" "clipgrf"]

# pack $w.frame.cliplabel -in $w.frame.clpstat   -side left
# pack $w.frame.clip -in $w.frame.clpstat   -expand 1 -fill x -pady 1.m -padx 1.m

# #clip box
# frame $w.frame.lb1 -borderwidth 0
# pack $w.frame.lb1  -in $w.frame -side top   -fill x
# label $w.frame.labelul -text "  Clip box : upper-left point coordinates "
# pack $w.frame.labelul -in  $w.frame.lb1 -side left

# frame $w.frame.lb2 -borderwidth 0
# pack $w.frame.lb2  -in $w.frame -side top   -fill x

# frame $w.frame.lb21 -borderwidth 0
# pack $w.frame.lb21  -in $w.frame -side top   -fill x

# frame $w.frame.lb22 -borderwidth 0
# pack $w.frame.lb22  -in $w.frame -side top   -fill x

# label $w.frame.labelx -text "             X: "
# entry $w.frame.datax -relief sunken  -textvariable Xclipbox
# label $w.frame.labely -text "             Y: "
# entry $w.frame.datay -relief sunken  -textvariable Yclipbox

# pack $w.frame.labelx  $w.frame.datax  -in  $w.frame.lb2 -side left  -fill x -pady 1.m -padx 1.m
# pack $w.frame.labely  $w.frame.datay  -in  $w.frame.lb21 -side left -fill x -pady 1.m -padx 1.m 
# bind  $w.frame.datax <Return> {SelectClipBox}
# bind  $w.frame.datay <Return> {SelectClipBox}

# #----------------------------#
# frame $w.frame.lb3 -borderwidth 0
# pack $w.frame.lb3  -in $w.frame -side top   -fill x
# label $w.frame.labelwh -text "   Clip box : width and height  "
# pack $w.frame.labelwh -in  $w.frame.lb3 -side left

# frame $w.frame.lb4 -borderwidth 0
# pack $w.frame.lb4  -in $w.frame -side top   -fill x

# frame $w.frame.lb41 -borderwidth 0
# pack $w.frame.lb41  -in $w.frame -side top   -fill x

# label $w.frame.labelw -text "             W: "
# entry $w.frame.dataw -relief sunken  -textvariable Wclipbox
# label $w.frame.labelh -text "             H: "
# entry $w.frame.datah -relief sunken  -textvariable Hclipbox

# pack $w.frame.labelw  $w.frame.dataw -in  $w.frame.lb4  -side left -fill x -pady 1.m -padx 1.m
# pack $w.frame.labelh  $w.frame.datah -in  $w.frame.lb41 -side left -fill x -pady 1.m -padx 1.m
# bind  $w.frame.dataw <Return> {SelectClipBox}
# bind  $w.frame.datah <Return> {SelectClipBox}


# #sep bar
# frame $w.sep -height 2 -borderwidth 1 -relief sunken
# pack $w.sep -fill both -pady 30m  

# #exit button
# frame $w.buttons
# button $w.b -text Quit -command "destroy $ww"
# pack $w.b -side bottom 



########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n Data]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

frame $w.frame.fdata -borderwidth 0
pack $w.frame.fdata  -in $w.frame -side top   -fill x

scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
canvas $w.frame.c -width 8i -height 6i  -yscrollcommand {$w.frame.ysbar set}

$w.frame.c create text 160 10 -anchor c -text "X"
$w.frame.c create text 310 10 -anchor c -text "Y"

if { $nbcol == 3 } {
    $w.frame.c create text 460 10 -anchor c -text "Z"
}

for {set i 1} {$i<=$nbrow} {incr i} {
    set bb [expr 10+(25*$i)]
    $w.frame.c create text 10 $bb -anchor c -text $i
    for {set j 1} {$j<=$nbcol} {incr j} {
	set aa [expr 10+($j*150)]
	set tmp $w.frame.c.data$i
	set tmp $tmp+"_"
	entry  $tmp$j  -relief sunken  -textvariable polyVAL($i,$j)
	bind  $tmp$j <Return> "setData $i $j"

	$w.frame.c create window $aa $bb -anchor c -window $tmp$j
    }
}

$w.frame.c configure -scrollregion [$w.frame.c bbox all] -yscrollincrement 0.1i

pack  $w.frame.ysbar -side right -fill y
pack  $w.frame.c


#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 



pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#les proc asssocies
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

#ScilabEval "disp($thick)"
}

proc toggleVisibility {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
#ScilabEval "disp('$visToggle')"
}

proc SelectPolylineStyle {w args} {
global curpolylinestyle
ScilabEval "setStyle('$curpolylinestyle')"
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


proc SelectClipBox {} {
global Xclipbox Yclipbox Wclipbox Hclipbox curclipstate
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
