
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

set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Figure Editor"
wm iconname $ww "FE"
wm geometry $ww 400x300

set w $ww
frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#Hierarchical selection
set lalist ""
for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
append lalist "\""
append lalist "$SELOBJECT($i)" 
append lalist "\" "
}

set curgedobject $SELOBJECT($curgedindex)


#Hiereachical viewer
frame $w.frame.view  -borderwidth 0
pack $w.frame.view  -in $w.frame  -side top  -fill x

label $w.frame.selgedobjectlabel  -height 0 -text "Edit Properties for:    " -width 0 
combobox $w.frame.selgedobject \
    -borderwidth 2 \
    -highlightthickness 3 \
    -maxheight 0 \
    -width 3 \
    -textvariable curgedobject \
    -editable false \
    -background white \
    -command [list SelectObject ]
eval $w.frame.selgedobject list insert end $lalist
pack $w.frame.selgedobjectlabel -in $w.frame.view   -side left
pack $w.frame.selgedobject   -in $w.frame.view   -fill x

Notebook:create .axes.n -pages {Style Info} -pad 20 
pack .axes.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame .axes.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1


########### Info onglet ###########################################
###################################################################
set w [Notebook:frame .axes.n Info]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $ww" 
pack $w.buttons.dismiss  -side bottom -expand 1





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