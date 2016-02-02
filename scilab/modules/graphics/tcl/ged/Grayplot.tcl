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

global curvis curdatamapping
global nbrowX nbrowY grayplotXVAL grayplotYVAL grayplotZVAL grayplotCOLORVAL
#global nbrowZ nbcolZ nbrowCOLOR 
global nbcolCOLOR ncolors
global scicomint_dataX scicomint_dataY scicomint_dataZ
global curdata_x curdata_y curdata_z

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 200
set NBwidth  230

set Wheight [expr $NBheight + 110]
set Wwidth  [expr $NBwidth  + 270]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Grayplot Object"
wm iconname $ww "GE"
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


set w $uf
set fra [frame $w.frame -borderwidth 0]
pack $fra  -anchor w -fill both

set largeur 12

Notebook:create $uf.n -pages {"Style" "Data" } -pad 0 -height $NBheight -width $NBwidth
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
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x -padx $smallPad

#data_mapping
frame $w.frame.dtmap  -borderwidth 0
pack $w.frame.dtmap  -in $w.frame  -side top  -fill x

label $w.frame.dtmapylelabel  -height 0 -text "Data mapping:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.dtmapyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 6 \
    -textvariable curdatamapping \
    -editable false \
    -command [list SelectDataMapping ] -font $gedFont
eval $w.frame.dtmapyle list insert end [list "direct" "scaled"]
pack $w.frame.dtmapylelabel -in $w.frame.dtmap   -side left
pack $w.frame.dtmapyle   -in $w.frame.dtmap   -expand 1 -pady 0 -anchor w -padx $mediumPad

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

set largeurData 10

frame $w.frame.curdataframeX  -borderwidth 0
pack $w.frame.curdataframeX  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelX  -height 0 -text "X Data:" -width 0   -font $gedFont -anchor e -width $largeurData
combobox $w.frame.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_x \
    -editable false \
    -command [list SelectDataX ]  -font $gedFont
eval $w.frame.polydataX list insert end [list $curdata_x "----" "Edit data..."]
pack $w.frame.polydatalabelX -in $w.frame.curdataframeX  -side left
pack $w.frame.polydataX   -in $w.frame.curdataframeX  -expand 1 -fill x -pady 0 -padx $mediumPad


frame $w.frame.curdataframeY  -borderwidth 0
pack $w.frame.curdataframeY  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelY  -height 0 -text "Y Data:" -width 0   -font $gedFont -anchor e -width $largeurData
combobox $w.frame.polydataY \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_y \
    -editable false \
    -command [list SelectDataY ]  -font $gedFont
eval $w.frame.polydataY list insert end [list $curdata_y "----" "Edit data..."]
pack $w.frame.polydatalabelY -in $w.frame.curdataframeY  -side left
pack $w.frame.polydataY   -in $w.frame.curdataframeY  -expand 1 -fill x -pady 0 -padx $mediumPad

frame $w.frame.curdataframeZ  -borderwidth 0
pack $w.frame.curdataframeZ  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelZ  -height 0 -text "Z Data:" -width 0   -font $gedFont -anchor e -width $largeurData
combobox $w.frame.polydataZ \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata_z \
    -editable false \
    -command [list SelectDataZ ]  -font $gedFont
eval $w.frame.polydataZ list insert end [list $curdata_z "----" "Edit data..."]
pack $w.frame.polydatalabelZ -in $w.frame.curdataframeZ  -side left
pack $w.frame.polydataZ   -in $w.frame.curdataframeZ  -expand 1 -fill x -pady 0 -padx $mediumPad





#######################################################
## DATA edit via Scilab Command Interface sciGUI ######
#######################################################

set largeur 18

frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 0

label $w.scicom1.label1 -text "Scilab Command Interface for data:"  -font $gedFont -anchor w
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicomX
pack $w.scicomX -side top -fill x -pady 0

label $w.scicomX.label1 -text "grayplot_handle.data.x:"  -font $gedFont -anchor e -width $largeur
pack  $w.scicomX.label1 -in $w.scicomX -side left

entry $w.scicomX.text1 -relief sunken -textvariable scicomint_dataX -width 10 -font $gedFont
set_balloon $w.scicomX.text1 "Enter a variable defined in Scilab Console representing\n a real vector or use a macro call\n to initialize the \"X data\" field."
bind  $w.scicomX.text1 <Return> "sciCommandData"
bind  $w.scicomX.text1 <KP_Enter> "sciCommandData"

pack $w.scicomX.text1  -side left  -fill both


frame $w.scicomY
pack $w.scicomY -side top -fill x -pady 0

label $w.scicomY.label1 -text "grayplot_handle.data.y:"  -font $gedFont -anchor e -width $largeur
pack  $w.scicomY.label1 -in $w.scicomY -side left

entry $w.scicomY.text1 -relief sunken -textvariable scicomint_dataY -width 10 -font $gedFont
set_balloon $w.scicomY.text1 "Enter a variable defined in Scilab Console representing\n a real vector or use a macro call\n to initialize the \"Y data\" field."
bind  $w.scicomY.text1 <Return> "sciCommandData"
bind  $w.scicomY.text1 <KP_Enter> "sciCommandData"

pack $w.scicomY.text1  -side left  -fill both


frame $w.scicomZ
pack $w.scicomZ -side top -fill x -pady 0

label $w.scicomZ.label1 -text "grayplot_handle.data.z:"  -font $gedFont -anchor e -width $largeur
pack  $w.scicomZ.label1 -in $w.scicomZ -side left

entry $w.scicomZ.text1 -relief sunken -textvariable scicomint_dataZ -width 10 -font $gedFont
set_balloon $w.scicomZ.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call\n to initialize the \"Z data\" field."
bind  $w.scicomZ.text1 <Return> "sciCommandData"
bind  $w.scicomZ.text1 <KP_Enter> "sciCommandData"

pack $w.scicomZ.text1  -side left  -fill both






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





# ########### X vector onglet #######################################
# ###################################################################
#  set w [Notebook:frame $uf.n "X vector"]

#  frame $w.frame -borderwidth 0
#  pack $w.frame -anchor w -fill both

#  frame $w.frame.fdata -borderwidth 0
#  pack $w.frame.fdata  -in $w.frame -side top   -fill x

#  scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
#  canvas $w.frame.c -width 8i -height 4i  -yscrollcommand {$w.frame.ysbar set}

#  #$w.frame.c create text 160 10 -anchor c -text "X vector"

#  for {set i 1} {$i<=$nbrowX} {incr i} {
#      set bb [expr 10+(25*$i)]
#      $w.frame.c create text 10 $bb -anchor c -text $i
#      entry  $w.frame.c.data$i  -relief sunken  -textvariable grayplotXVAL($i)
#      bind  $w.frame.c.data$i <Return> "setXData $i "
# #location help balloon	
#      set_balloon $w.frame.c.data$i "Row: $i"

#      $w.frame.c create window 160 $bb -anchor c -window $w.frame.c.data$i
#  }

#  $w.frame.c configure -scrollregion [$w.frame.c bbox all] -yscrollincrement 0.1i

#  pack  $w.frame.ysbar -side right -fill y
#  pack  $w.frame.c


# #sep bar
#  frame $w.sep -height 2 -borderwidth 1 -relief sunken
#  pack $w.sep -fill both


#  #exit button
#  frame $w.buttons
#  button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
#  pack $w.b -side bottom 



#  ########### Y vector onglet #######################################
#  ###################################################################
#   set w2 [Notebook:frame $uf.n "Y vector"]

#   frame $w2.frame2 -borderwidth 0
#   pack $w2.frame2 -anchor w -fill both

#   frame $w2.frame2.fdata -borderwidth 0
#   pack $w2.frame2.fdata  -in $w2.frame2 -side top   -fill x


#   canvas $w2.frame2.c1 -width 8i -height 4i  -yscrollcommand {$w2.frame2.ysbar set}
#  scrollbar $w2.frame2.ysbar -orient vertical -command   {$w2.frame2.c1 yview}

#  #$w2.frame2.c1 create text 160 10 -anchor c -text "Y vector"

#   for {set i 1} {$i<=$nbrowY} {incr i} {
#       set bb [expr 10+(25*$i)]
#       $w2.frame2.c1 create text 10 $bb -anchor c -text $i
#       entry  $w2.frame2.c1.data$i  -relief sunken  -textvariable grayplotYVAL($i)
#       bind  $w2.frame2.c1.data$i <Return> "setYData $i "
# #location help balloon	
#       set_balloon $w2.frame2.c1.data$i "Row: $i"
   
#       $w2.frame2.c1 create window 160 $bb -anchor c -window $w2.frame2.c1.data$i
#   }

#   $w2.frame2.c1 configure -scrollregion [$w2.frame2.c1 bbox all] -yscrollincrement 0.1i

#   pack  $w2.frame2.ysbar -side right -fill y
#   pack  $w2.frame2.c1

# #sep bar
#  frame $w2.sep -height 2 -borderwidth 1 -relief sunken
#  pack $w2.sep -fill both


#   #exit button
#   frame $w2.buttons
#   button $w2.b -text Quit -command "DestroyGlobals; destroy $ww"
#   pack $w2.b -side bottom 



#  ########### Z matrix onglet #######################################
#  ###################################################################
#   set w3 [Notebook:frame $uf.n "Z matrix"]

#   frame $w3.frame2 -borderwidth 0
#   pack $w3.frame2 -anchor w -fill both

#   frame $w3.frame2.fdata -borderwidth 0
#   pack $w3.frame2.fdata  -in $w3.frame2 -side top   -fill x


#  canvas $w3.frame2.c1 -width 8i -height 4i  -yscrollcommand {$w3.frame2.ysbar set} -xscrollcommand {$w3.frame2.xsbar set}
#  scrollbar $w3.frame2.ysbar -orient vertical -command   {$w3.frame2.c1 yview}
#  scrollbar $w3.frame2.xsbar -orient horizontal -command   {$w3.frame2.c1 xview}
#  #scrollbar $w3.frame2.ysbar.scrollh -orient horizontal -command {$w.frame2.c1 xview}
#  #pack configure $w3.frame2.ysbar.scrollh -side bottom -fill x -expand 1 
   
#  for {set i 1} {$i<=$nbrowZ} {incr i} {
#  #    puts $i
#      set bb [expr 10+(25*$i)]
#      $w3.frame2.c1 create text 10 $bb -anchor c -text $i
#      for {set j 1} {$j<=$nbcolZ} {incr j} {
#  #	puts $j
#  	set aa [expr 10+($j*150)]
#  	$w3.frame2.c1 create text $aa 10 -anchor c -text $j
#  	set tmp $w3.frame2.c1.data$i
#  	set tmp $tmp+"_"
#  	entry  $tmp$j  -relief sunken  -textvariable grayplotZVAL($i,$j)
#  	bind   $tmp$j <Return> "setZData $i $j"
# #location help balloon	
# 	set_balloon $tmp$j "Row: $i Column: $j"

#  	$w3.frame2.c1 create window $aa $bb -anchor c -window $tmp$j
#      }
#  }

#   $w3.frame2.c1 configure -scrollregion [$w3.frame2.c1 bbox all] -yscrollincrement 0.1i -xscrollincrement 0.1i
#  # $w3.frame2.c1 configure -scrollregion [$w3.frame2.c1 bbox all] -xscrollincrement 0.1i

#  #pack  $w3.frame2.ysbar.scrollh -side bottom -fill x
#   pack  $w3.frame2.ysbar -side right -fill y
#   pack  $w3.frame2.xsbar -side bottom -fill x
#   pack  $w3.frame2.c1

# #sep bar
#  frame $w3.sep -height 2 -borderwidth 1 -relief sunken
#  pack $w3.sep -fill both

#   #exit button
#   frame $w3.buttons
#   button $w3.b -text Quit -command "DestroyGlobals; destroy $ww"
#   pack $w3.b -side bottom 

# ########### Scilab Command Interface ##############################
# ###################################################################
# set w5 [Notebook:frame $uf.n "Scilab Command Interface"]

# frame $w5.frame -borderwidth 0
# pack $w5.frame -anchor w -fill both


# frame $w5.scicom1
# pack $w5.scicom1 -side top -fill x -pady 0

# label $w5.scicom1.label1 -text "Scilab Command Interface for data:"
# pack  $w5.scicom1.label1 -in $w5.scicom1 -side left

# frame $w5.scicomX
# pack $w5.scicomX -side top -fill x -pady 0

# label $w5.scicomX.label1 -text "grayplot_handle.data.x =      "
# pack  $w5.scicomX.label1 -in $w5.scicomX -side left

# entry $w5.scicomX.text1 -relief sunken -textvariable scicomint_dataX
# set_balloon $w5.scicomX.text1 "Enter a variable defined in Scilab Console representing\n a real vector or use a macro call\n to initialize the \"X data\" field."
# bind  $w5.scicomX.text1 <Return> "sciCommandData"

# pack $w5.scicomX.text1  -side left  -fill both -expand yes


# frame $w5.scicomY
# pack $w5.scicomY -side top -fill x -pady 0

# label $w5.scicomY.label1 -text "grayplot_handle.data.y =      "
# pack  $w5.scicomY.label1 -in $w5.scicomY -side left

# entry $w5.scicomY.text1 -relief sunken -textvariable scicomint_dataY
# set_balloon $w5.scicomY.text1 "Enter a variable defined in Scilab Console representing\n a real vector or use a macro call\n to initialize the \"Y data\" field."
# bind  $w5.scicomY.text1 <Return> "sciCommandData"

# pack $w5.scicomY.text1  -side left  -fill both -expand yes


# frame $w5.scicomZ
# pack $w5.scicomZ -side top -fill x -pady 0

# label $w5.scicomZ.label1 -text "grayplot_handle.data.z =       "
# pack  $w5.scicomZ.label1 -in $w5.scicomZ -side left

# entry $w5.scicomZ.text1 -relief sunken -textvariable scicomint_dataZ
# set_balloon $w5.scicomZ.text1 "Enter a variable defined in Scilab Console representing\n a real matrix or use a macro call\n to initialize the \"Z data\" field."
# bind  $w5.scicomZ.text1 <Return> "sciCommandData"

# pack $w5.scicomZ.text1  -side left  -fill both -expand yes


# #sep bar
# frame $w5.sep -height 2 -borderwidth 1 -relief sunken
# pack $w5.sep -fill both


# #exit button
# frame $w5.buttons
# button $w5.b -text Quit -command "DestroyGlobals; destroy $ww"
# pack $w5.b -side bottom 


# pack $sw $pw1 -fill both -expand yes
# pack $titf1 -padx 4 -side left -fill both -expand yes
# pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
    
    OnOffForeground $frame $curvis
}


proc setXData { i } {
global grayplotXVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i+1,1)=$grayplotXVAL($i);\",\'errcatch\',\'n\');"
}


proc setYData { i } {
global grayplotYVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data(1,$i+1)=$grayplotYVAL($i);\",\'errcatch\',\'n\');"
}

proc setZData { i j } {
global grayplotZVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i+1,$j+1)=$grayplotZVAL($i,$j);\",\'errcatch\',\'n\');"
}



proc SelectDataMapping {w args} {
global curdatamapping
ScilabEval "global ged_handle; ged_handle.data_mapping='$curdatamapping'"
}

proc sciCommandData {} {
    global scicomint_dataX scicomint_dataY scicomint_dataZ
    
    if { ($scicomint_dataX == "") || ($scicomint_dataY == "") ||
	 ($scicomint_dataZ == "")  } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must fill in all the fields using variables defined in Scilab Console\n to initialize the \"data\" field."
    } else {
	
	ScilabEval "setGrayplottlist($scicomint_dataX,$scicomint_dataY,$scicomint_dataZ)" "seq"
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

