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

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
package require combobox 2.3
catch {namespace import combobox::*}

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

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Axis Object"
wm iconname $ww "SO"
wm geometry $ww 435x600
#wm geometry $ww 545x600
#wm geometry $ww 650x700
wm protocol $ww WM_DELETE_WINDOW "DestroyGlobals; destroy $ww "

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


Notebook:create $uf.n -pages {Style Xtics Ytics Clipping} -pad 40 -height 450 -width 350
pack $uf.n -in $uf -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "            Visibility:    "
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib"
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x

#tics segment
frame $w.frame.seg -borderwidth 0
pack $w.frame.seg  -in $w.frame  -side top -fill x
label $w.frame.seglabel  -text "    Tics segment:    "
checkbutton $w.frame.segib  -text "on"\
    -variable curseg  -onvalue "on" -offvalue "off" \
    -command "toggleSeg $w.frame.segib"
OnOffForeground $w.frame.segib $curseg

pack $w.frame.seglabel -in $w.frame.seg  -side left
pack $w.frame.segib  -in $w.frame.seg    -side left -fill x

#Color scale
frame $w.frame.ticscol  -borderwidth 0
pack $w.frame.ticscol  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "         Tics color:   " -width 0 
#         -foreground $color
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 

pack $w.frame.colorlabel -in $w.frame.ticscol -side left
pack $w.frame.color  -in  $w.frame.ticscol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.color set $curcolor

#Tics Style
frame $w.frame.ticsst  -borderwidth 0
pack $w.frame.ticsst  -in $w.frame  -side top  -fill x

label $w.frame.ticsstylelabel  -height 0 -text "         Tics style:   " -width 0 
combobox $w.frame.ticsstyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curticsstyle \
    -editable false \
    -command [list SelectTicsStyle ]
eval $w.frame.ticsstyle list insert end [list "v" "r" "i"]
pack $w.frame.ticsstylelabel -in $w.frame.ticsst   -side left
pack $w.frame.ticsstyle   -in $w.frame.ticsst   -expand 1 -fill x -pady 2m -padx 2m

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontcolorlabel -height 0 -text "Labels font color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0 

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $curfontcolor


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontsizlabel -height 0 -text " Labels font size:  " -width 0 
scale $w.frame.fontsize -orient horizontal -from -1 -to 6 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontsize set $curfontsize

   
frame $w.frame.cas -borderwidth 0
pack $w.frame.cas -anchor w -fill both

label $w.frame.subticslab -height 0 -text "            Subtics: " -width 0 
entry $w.frame.subtics  -relief sunken  -textvariable cursubtics
bind  $w.frame.subtics  <Return> "SelectSubtics  $w.frame"
bind  $w.frame.subtics  <KP_Enter> "SelectSubtics  $w.frame"
pack  $w.frame.subticslab  $w.frame.subtics  -in  $w.frame.cas   -side left -fill x -pady 2m -padx 2m



#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both  -pady 5m

#exit button
button $w.buttons -text Quit -command "DestroyGlobals; destroy $ww" 
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

    label $w.frame.warning -height 0 -text "  --  Vertical axis case  --" -width 0 
    pack  $w.frame.warning -in  $w.frame.cas1_0  -expand 1 -fill x -pady 1.m -padx 1.m
    
    frame $w.frame.cas1_1 -borderwidth 0
    pack $w.frame.cas1_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "    Xtics coord:   " -width 0 
    entry $w.frame.xcoord  -relief sunken  -textvariable xticscoord
    bind  $w.frame.xcoord  <Return> "SelectXticsCoord  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectXticsCoord  $w.frame"
    pack  $w.frame.xcoordlab  $w.frame.xcoord  -in  $w.frame.cas1_1   -side left -fill x -pady 1.m -padx 1.m

    #sep bar
    frame $w.sep -height 2 -borderwidth 1 -relief sunken
    pack $w.sep -fill both  -pady 30m
    

} else {
    frame $w.frame.cas2_0 -borderwidth 0
    pack $w.frame.cas2_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Horizontal axis case  --" -width 0 
    pack  $w.frame.warning -in  $w.frame.cas2_0  -expand 1 -fill x -pady 1.m -padx 1.m
    
    frame $w.frame.cas2_1 -borderwidth 0
    pack $w.frame.cas2_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "    Xtics coord:   " -width 0 
#    text $w.frame.xcoord  -relief sunken -yscrollcommand "$w.frame.scroll set"
#    scrollbar $w.frame.scroll -command "$w.frame.xcoord yview"
#    $w.frame.xcoord insert end $xticscoord
#    pack $w.frame.scroll  -in  $w.frame.cas2_1 -side right -fill y
    entry $w.frame.xcoord  -relief sunken  -textvariable xticscoord
    bind  $w.frame.xcoord  <Return> "SelectXticsCoord2  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectXticsCoord2  $w.frame"
    pack  $w.frame.xcoordlab  -in  $w.frame.cas2_1 -side left
    pack  $w.frame.xcoord  -in  $w.frame.cas2_1 -expand 1 -fill x -pady 1.m -padx 1.m
    
    frame $w.frame.cas2_2 -borderwidth 0
    pack $w.frame.cas2_2 -anchor w -fill both
    
    label $w.frame.ticslabellab -height 0 -text "    Tics Labels:   " -width 0 
    entry $w.frame.ticslabel  -relief sunken  -textvariable curticslabel
    bind  $w.frame.ticslabel  <Return> "SelectTicsLabels  $w.frame"
    bind  $w.frame.ticslabel  <KP_Enter> "SelectTicsLabels  $w.frame"
    pack  $w.frame.ticslabellab  -in  $w.frame.cas2_2   -side left
    pack  $w.frame.ticslabel  -in  $w.frame.cas2_2 -expand 1 -fill x -pady 1.m -padx 1.m

    
    #Tics direction
    frame $w.frame.px  -borderwidth 0
    pack $w.frame.px  -in $w.frame -side top   -fill x -pady 2m
    
    label $w.frame.xposlabel  -height 0 -text "  Tics direction: " -width 0 
    combobox $w.frame.xpos \
	-borderwidth 1 \
	-highlightthickness 1 \
	-maxheight 0 \
	-width 3 \
	-textvariable curticsdir \
	-editable false \
	-command [list SelectTicsdir ]
    eval $w.frame.xpos list insert end [list "top" "bottom"]
    
    pack $w.frame.xposlabel -in  $w.frame.px -side left
    pack $w.frame.xpos  -in  $w.frame.px  -expand 1 -fill x -pady 2m -padx 2m
    
    #sep bar
    frame $w.sep -height 2 -borderwidth 1 -relief sunken
    pack $w.sep -fill both  -pady 30m
    
}
    


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom 


########### Ytics onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Ytics]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

if { $nbcolY == 1} {
    frame $w.frame.cas1_0 -borderwidth 0
    pack $w.frame.cas1_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Horizontal axis case  --" -width 0 
    pack  $w.frame.warning -in  $w.frame.cas1_0  -expand 1 -fill x -pady 1.m -padx 1.m
    
    frame $w.frame.cas1_1 -borderwidth 0
    pack $w.frame.cas1_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "    Ytics coord:   " -width 0 
    entry $w.frame.xcoord  -relief sunken  -textvariable yticscoord
    bind  $w.frame.xcoord  <Return> "SelectYticsCoord  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectYticsCoord  $w.frame"
    pack  $w.frame.xcoordlab  $w.frame.xcoord  -in  $w.frame.cas1_1   -side left -fill x -pady 1.m -padx 1.m

    #sep bar
    frame $w.sep -height 2 -borderwidth 1 -relief sunken
    pack $w.sep -fill both  -pady 30m
    
} else {
    frame $w.frame.cas2_0 -borderwidth 0
    pack $w.frame.cas2_0 -anchor w -fill both

    label $w.frame.warning -height 0 -text "  --  Vertical axis case  --" -width 0 
    pack  $w.frame.warning -in  $w.frame.cas2_0  -expand 1 -fill x -pady 1.m -padx 1.m
    
    frame $w.frame.cas2_1 -borderwidth 0
    pack $w.frame.cas2_1 -anchor w -fill both

    label $w.frame.xcoordlab -height 0 -text "    Ytics coord:   " -width 0 
    entry $w.frame.xcoord  -relief sunken  -textvariable yticscoord
    bind  $w.frame.xcoord  <Return> "SelectYticsCoord2  $w.frame"
    bind  $w.frame.xcoord  <KP_Enter> "SelectYticsCoord2  $w.frame"
    pack  $w.frame.xcoordlab  -in  $w.frame.cas2_1 -side left
    pack  $w.frame.xcoord  -in  $w.frame.cas2_1 -expand 1 -fill x -pady 1.m -padx 1.m
    
    frame $w.frame.cas2_2 -borderwidth 0
    pack $w.frame.cas2_2 -anchor w -fill both

    label $w.frame.ticslabellab -height 0 -text "    Tics Labels:   " -width 0 
    entry $w.frame.ticslabel  -relief sunken  -textvariable curticslabel
    bind  $w.frame.ticslabel  <Return> "SelectTicsLabels  $w.frame"
    bind  $w.frame.ticslabel  <KP_Enter> "SelectTicsLabels  $w.frame"
    pack  $w.frame.ticslabellab  -in  $w.frame.cas2_2   -side left
    pack  $w.frame.ticslabel  -in  $w.frame.cas2_2 -expand 1 -fill x -pady 1.m -padx 1.m

    
    #Tics direction
    frame $w.frame.px  -borderwidth 0
    pack $w.frame.px  -in $w.frame -side top   -fill x -pady 2m
    
    label $w.frame.xposlabel  -height 0 -text "  Tics direction: " -width 0 
    combobox $w.frame.xpos \
	-borderwidth 1 \
	-highlightthickness 1 \
	-maxheight 0 \
	-width 3 \
	-textvariable curticsdir \
	-editable false \
	-command [list SelectTicsdir ]
    eval $w.frame.xpos list insert end [list "left" "right"]
    
    pack $w.frame.xposlabel -in  $w.frame.px -side left
    pack $w.frame.xpos  -in  $w.frame.px  -expand 1 -fill x -pady 2m -padx 2m
    

    #sep bar
    frame $w.sep -height 2 -borderwidth 1 -relief sunken
    pack $w.sep -fill both  -pady 30m
    
}
    

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
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
bind  $w9.frame.datax <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <KP_Enter> "SelectClipBox $w9.frame"

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
bind  $w9.frame.dataw <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <KP_Enter> "SelectClipBox $w9.frame"


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
button $w9.b -text Quit -command "DestroyGlobals; destroy $ww"
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
	ScilabEval "global ged_handle; ged_handle.tics_color=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.tics_color=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.tics_color=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.tics_color=$index;"
	
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
ScilabEval "global ged_handle;ged_handle.labels_font_size=$siz"
}


proc setFontColor {w index} {
   global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle;ged_handle.labels_font_color=$index"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle;ged_handle.labels_font_color=$index"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle;ged_handle.labels_font_color=$index"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle;ged_handle.labels_font_color=$index"
		
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


proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"
}