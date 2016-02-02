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
#curdatamapping
global nbrowX nbrowY grayplotXVAL grayplotYVAL grayplotZVAL grayplotCOLORVAL
#global nbrowZ nbcolZ nbrowCOLOR 
global nbcolCOLOR ncolors
global scicomint_data
global curdata

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 165
set NBwidth  225

set Wheight [expr $NBheight + 110]
set Wwidth  [expr $NBwidth  + 270]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Matplot Object"
wm iconname $ww "ME"
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

set largeur 17

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
pack $w.frame.visib  -in $w.frame.vis    -side left -padx $smallPad

#data_mapping
#frame $w.frame.dtmap  -borderwidth 0
#pack $w.frame.dtmap  -in $w.frame  -side top  -fill x
#
#label $w.frame.dtmapylelabel  -height 0 -text "       Data mapping:   " -width 0 
#combobox $w.frame.dtmapyle \
#    -borderwidth 1 \
#    -highlightthickness 1 \
#    -maxheight 0 \
#    -width 3 \
#    -textvariable curdatamapping \
#    -editable false \
#    -command [list SelectDataMapping ]
#eval $w.frame.dtmapyle list insert end [list "direct" "scaled"]
#pack $w.frame.dtmapylelabel -in $w.frame.dtmap   -side left
#pack $w.frame.dtmapyle   -in $w.frame.dtmap   -expand 1 -fill x -pady 0 -padx $mediumPad
#
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

label $w.frame.polydatalabelX  -height 0 -text "Data:" -width 0   -font $gedFont -anchor e -width $largeurData
combobox $w.frame.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 10 \
    -textvariable curdata \
    -editable false \
    -command [list SelectData ]  -font $gedFont
eval $w.frame.polydataX list insert end [list $curdata "----" "Edit data..."]
pack $w.frame.polydatalabelX -in $w.frame.curdataframeX  -side left
pack $w.frame.polydataX   -in $w.frame.curdataframeX  -expand 1 -fill x -pady 3 -padx $smallPad


#######################################################
## DATA edit via Scilab Command Interface sciGUI ######
#######################################################

#set largeur 18

frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 0

label $w.scicom1.label1 -text "Scilab Command Interface for data:"  -font $gedFont -anchor w
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicomX
pack $w.scicomX -side top -fill x -pady 0

label $w.scicomX.label1 -text "matplot_handle.data:"  -font $gedFont -anchor e -width $largeur
pack  $w.scicomX.label1 -in $w.scicomX -side left

entry $w.scicomX.text1 -relief sunken -textvariable scicomint_data -width 10  -font $gedFont
set_balloon $w.scicomX.text1 "Enter a variable defined in Scilab Console representing\n a real vector or use a macro call\n to initialize the \"data\" field."
bind  $w.scicomX.text1 <Return> "sciCommandData"
bind  $w.scicomX.text1 <KP_Enter> "sciCommandData"

pack $w.scicomX.text1  -side left -padx $mediumPad


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.b -side bottom 



pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes





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



#proc SelectDataMapping {w args} {
#global curdatamapping
#ScilabEval "global ged_handle; ged_handle.data_mapping='$curdatamapping'"
#}

proc sciCommandData {} {
    global scicomint_data
    
    if { ($scicomint_data == "") } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message "You must fill in the field using variables defined in Scilab Console\n to initialize the \"data\" field."
    } else {
	
	ScilabEval "ged_handle.data=$scicomint_data" "seq"
	#Refresh now !
	ScilabEval "tkged();" "seq"
	
    }
}



proc SelectData  {w args} {
    global curdata
    variable mycurdata
    set mycurdata $curdata
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

