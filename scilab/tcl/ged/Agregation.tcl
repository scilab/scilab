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
wm title $ww "Agregation Editor"
wm iconname $ww "AE"
wm geometry $ww 370x250
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


Notebook:create $uf.n -pages {Style} -pad 0 -height 100 -width 100
pack $uf.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame  $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "       Visibility: " -font {Arial 9}
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib"  -font {Arial 9}
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0m
button $w.buttons.dismiss -text Quit -command "DestroyGlobals; destroy $ww"  -font {Arial 9}
pack $w.buttons.dismiss  -side bottom


pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}


proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"
}