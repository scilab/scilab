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
source [file join $sourcedir Balloon.tcl]

package require combobox 2.3
catch {namespace import combobox::*}

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

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Matplot Object"
wm iconname $ww "ME"
wm geometry $ww 435x520
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



Notebook:create $uf.n -pages {"Style" "Data" } -pad 20 -height 400 -width 350
pack $uf.n -in $uf -fill both -expand yes

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n "Style"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "               Visibility:    "
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib"
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x

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
#pack $w.frame.dtmapyle   -in $w.frame.dtmap   -expand 1 -fill x -pady 2m -padx 2m
#
#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both  -pady 5m

#exit button
button $w.buttons -text Quit -command "DestroyGlobals; destroy $ww" 
pack $w.buttons -side bottom





########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n "Data"]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both



##############################
## DATA edit via sciGUI ######
##############################

frame $w.frame.curdataframeX  -borderwidth 0
pack $w.frame.curdataframeX  -in $w.frame  -side top  -fill x

label $w.frame.polydatalabelX  -height 0 -text "       Data :   " -width 0 
combobox $w.frame.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata \
    -editable false \
    -command [list SelectData ]
eval $w.frame.polydataX list insert end [list $curdata "----" "Edit data..."]
pack $w.frame.polydatalabelX -in $w.frame.curdataframeX  -side left
pack $w.frame.polydataX   -in $w.frame.curdataframeX  -expand 1 -fill x -pady 2m -padx 2m


#######################################################
## DATA edit via Scilab Command Interface sciGUI ######
#######################################################


frame $w.scicom1
pack $w.scicom1 -side top -fill x -pady 2m

label $w.scicom1.label1 -text "Scilab Command Interface for data:"
pack  $w.scicom1.label1 -in $w.scicom1 -side left

frame $w.scicomX
pack $w.scicomX -side top -fill x -pady 2m

label $w.scicomX.label1 -text "matplot_handle.data =      "
pack  $w.scicomX.label1 -in $w.scicomX -side left

entry $w.scicomX.text1 -relief sunken -textvariable scicomint_data
set_balloon $w.scicomX.text1 "Enter a variable defined in Scilab Console representing\n a real vector or use a macro call\n to initialize the \"data\" field."
bind  $w.scicomX.text1 <Return> "sciCommandData"
bind  $w.scicomX.text1 <KP_Enter> "sciCommandData"

pack $w.scicomX.text1  -side left  -fill both -expand yes


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both  -pady 5m


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
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


proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"
}