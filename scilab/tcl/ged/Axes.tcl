#Copyright INRIA
#2004
#Author: F.Leray

set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd



variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/tcl/BWidget-1.7.0"]
#lappend ::auto_path "$env(SCIPATH)/tcl"
namespace inscope :: package require BWidget
package require BWidget

set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
package require combobox 2.3
catch {namespace import combobox::*}


global fen1 fen2 fen3
global Xaxes_reverseToggle Yaxes_reverseToggle Zaxes_reverseToggle
global axes_boundsL axes_boundsU axes_boundsW axes_boundsH
global xlabel_visibility ylabel_visibility zlabel_visibility titlelabel_visibility
global SubticksEntryX SubticksEntryY SubticksEntryZ
global XautoticksToggle YautoticksToggle ZautoticksToggle
global StepEntryX StepEntryY StepEntryZ
global LOCATIONS_X LABELS_X nbticks_x
global LOCATIONS_Y LABELS_Y nbticks_y
global LOCATIONS_Z LABELS_Z nbticks_z
global LEI_x LEI_y LEI_z
global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject

global  visToggle limToggle boxToggle isoToggle gridToggle 
global  xToggle yToggle zToggle red green blue color 
global  xlabel ylabel zlabel tlabel
global  curvis cubToggle viewToggle

global xlabel_foreground ylabel_foreground zlabel_foreground titlelabel_foreground
global xlabel_fontsize ylabel_fontsize zlabel_fontsize  titlelabel_fontsize

global ncolors fcolor curthick curvis curfontsize curfontcolor
global curalpharotation curthetarotation old_curalpharotation old_curthetarotation
global curautoclear curautoscale curfillmode
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox
global curlinestyle
global Lmargins Rmargins Tmargins Bmargins

#axes_visible has changed !!!
global Xaxes_visibleToggle Yaxes_visibleToggle Zaxes_visibleToggle

global xGrif yGrid zGrid

global viewToggle isoToggle cubToggle
global dbxmin dbxmax dbymin dbymax dbzmin dbzmax

global msdos

global Xlabelfontstyle Ylabelfontstyle Zlabelfontstyle TITLEfontstyle fontstyle
global RED GREEN BLUE

# add for XF init only : to remove after...

    #test debug
  #   set nbticks_x 2

#     set LOCATIONS_X(1) 0.001
#     set LOCATIONS_X(2) 1.7

#     set LABELS_X(1) "zero virgule 00 et un"
#     set LABELS_X(2) "un,7"
    #


#      set ncolors 32
#      set curcolor 4
#      set curfontsize 1
#      set curfontcolor 7
#      set curthick 1

#      set xToggle "n"
#      set yToggle "n"

#      set xlabel_foreground 1
#      set ylabel_foreground 1
#      set zlabel_foreground 1
#      set titlelabel_foreground 1

#      set xlabel_fontsize 1
#      set ylabel_fontsize 1
#      set zlabel_fontsize 1
#      set titlelabel_fontsize 1


#      set xGrid -1
#      set yGrid -1
#      set zGrid -1

#      set boxToggle "on"

#      set fcolor -1
#      set bcolor -2

#      set viewToggle "2D"
#      set isoToggle "off"
#      set limToggle "off"
#      set cubToggle "off"
#      set visToggle "on"
#      set curvis "on"

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
wm title $ww "Axes Editor"
wm iconname $ww "AE"
wm geometry $ww 500x750
#wm geometry $ww 650x700
#wm maxsize  $ww 450 560
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

#set w $ww
#frame $w.frame -borderwidth 0
#pack $w.frame -anchor w -fill both


#Hierarchical selection
set lalist ""
# puts "ged_handle_list_size vaut: $ged_handle_list_size"
# puts "SELOBJECT(1) = $SELOBJECT(1) "
# puts "SELOBJECT(2) = $SELOBJECT(2) "
# puts "SELOBJECT(3) = $SELOBJECT(3) "
# puts "SELOBJECT(4) = $SELOBJECT(4) "

for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
append lalist "\""
append lalist "$SELOBJECT($i)" 
append lalist "\" "
}

set curgedobject $SELOBJECT($curgedindex)

#bind $ww <destroy> {destroy $ww}
#bind $ww <Maximize> "DestroyGlobals; destroy $ww"

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

Notebook:create $uf.n -pages {"X Axis" "Y Axis" "Z Axis" Title Style Aspect Viewpoint} -pad 20   -height 600 -width 420
pack $uf.n -fill both -expand yes


########### X onglet ##############################################
###################################################################
set w [Notebook:frame $uf.n "X Axis"]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options"]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options"]

set w [$titf1label getframe]

pack $titf1label -padx 4 -side top -fill both -expand yes
pack $topflabel -fill x -pady 2
pack $titf1axes  -pady 2 -padx 4 -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding


#x label
frame $w.frame.lbx -borderwidth 0
pack $w.frame.lbx  -in $w.frame -side top   -fill x -pady 2m


label $w.frame.xlabel -text "     Label:  "
entry $w.frame.xlabel1 -relief sunken  -textvariable xlabel 
pack $w.frame.xlabel -in  $w.frame.lbx -side left
pack $w.frame.xlabel1  -in  $w.frame.lbx  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.xlabel1 <Return> {setXlabel} 
bind  $w.frame.xlabel1 <KP_Enter> {setXlabel}
bind  $w.frame.xlabel1 <FocusOut> {setXlabel}

frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.vislablabel  -text "  Visibility:   "
checkbutton $w.frame.vislabb \
    -variable xlabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitylabx $w.frame.vislabb" -text "on"
OnOffForeground $w.frame.vislabb $xlabel_visibility

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb  -in $w.frame.vislab  -side left  -fill x


#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
    -resolution 1.0 -command "setXFontLabelColor $w.frame.fontcolor" -tickinterval 0 

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $xlabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontsizlabel -height 0 -text "      Size:  " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
    -resolution 1.0 -command "setXFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontsize set $xlabel_fontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 2m

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Xlabelfontstyle \
    -editable false \
    -command [list SelectXFontStyle ]
eval $w.frame.style list insert end [list "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 2m -padx 2m

#adding 26.05.2005
set w [$titf1axes getframe]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#Xpos
frame $w.frame.px  -borderwidth 0
pack $w.frame.px  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.xposlabel  -height 0 -text "   Location:" -width 0 
combobox $w.frame.xpos \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Xlabelpos \
    -editable false \
    -command [list SelectXpos ]
eval $w.frame.xpos list insert end [list "top" "middle" "bottom"]

pack $w.frame.xposlabel -in  $w.frame.px -side left
pack $w.frame.xpos  -in  $w.frame.px  -expand 1 -fill x -pady 2m -padx 2m

#Grid
frame $w.frame.gridcol  -borderwidth 0
 pack $w.frame.gridcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.gridcolorlabel -height 0 -text "Grid color:" -width 0 
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
    -resolution 1.0 -command "setXGridColor $w.frame.gridcolor" -tickinterval 0 

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $xGrid

#X Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.datalabel -text "Data bounds:"
entry $w.frame.databmin -relief sunken  -textvariable dbxmin 
entry $w.frame.databmax -relief sunken  -textvariable dbxmax
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x 
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x 
bind  $w.frame.databmin <Return> {setXdb}
bind  $w.frame.databmin <KP_Enter> {setXdb}
bind  $w.frame.databmin <FocusOut> {setXdb}

bind  $w.frame.databmax <Return> {setXdb} 
bind  $w.frame.databmax <KP_Enter> {setXdb}
bind  $w.frame.databmax <FocusOut> {setXdb}

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.scalesw.label -height 0 -text "       Scale:  " -width 0 
radiobutton $w.frame.scalesw.radioLIN -text "Linear" -variable xToggle -value "n"    -command "toggleX" 
 radiobutton $w.frame.scalesw.radioLOG -text "Logarithmic" -variable xToggle -value "l" 	  -command "toggleX" 	   

set numpage 0
button $w.frame.scalesw.buttonticks -text "Ticks..." -command "Reload_and_popup $ww $numpage" 

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left 
pack $w.frame.scalesw.buttonticks  -in  $w.frame.scalesw    -side left -expand 1 -fill x

#Reverse axis
frame $w.frame.rev  -borderwidth 0
pack $w.frame.rev  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.rev.label -height 0 -text "      Reverse:  " -width 0
checkbutton $w.frame.rev.revvalue  -text "on"\
    -variable Xaxes_reverseToggle -onvalue "on" -offvalue "off" \
    -command "toggleReverselabx $w.frame.rev.revvalue" 
OnOffForeground $w.frame.rev.revvalue $Xaxes_reverseToggle

pack $w.frame.rev.label -in $w.frame.rev -side left
pack $w.frame.rev.revvalue -in $w.frame.rev  -side left  -fill x



#sep bar
#frame $w.sep -height 2 -borderwidth 1 -relief sunken
#pack $w.sep -fill both -pady 10m  

set w $theframe

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww "
pack $w.b -side bottom

########### Y onglet ##############################################
###################################################################
set w [Notebook:frame $uf.n "Y Axis"]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options"]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options"]

set w [$titf1label getframe]

pack $titf1label -padx 4 -side top -fill both -expand yes
pack $topflabel -fill x -pady 2
pack $titf1axes  -pady 2 -padx 4 -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding


#y label
frame $w.frame.lby -borderwidth 0
pack $w.frame.lby  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.ylabel -text "     Label:  "
entry $w.frame.ylabel1 -relief sunken  -textvariable ylabel 
pack $w.frame.ylabel -in  $w.frame.lby -side left
pack $w.frame.ylabel1  -in  $w.frame.lby  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.ylabel1 <Return> {setYlabel} 
bind  $w.frame.ylabel1 <KP_Enter> {setYlabel}
bind  $w.frame.ylabel1 <FocusOut> {setYlabel}

frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.vislablabel  -text "  Visibility:   "
checkbutton $w.frame.vislabb  -text "on"\
    -variable ylabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitylaby $w.frame.vislabb" 
OnOffForeground $w.frame.vislabb $ylabel_visibility

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb  -in $w.frame.vislab  -side left  -fill x

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setYFontLabelColor $w.frame.fontcolor" -tickinterval 0 

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $ylabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontsizlabel -height 0 -text "      Size:  " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setYFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz   -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontsize set $ylabel_fontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 2m

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Ylabelfontstyle \
    -editable false \
    -command [list SelectYFontStyle ]
eval $w.frame.style list insert end [list "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 2m -padx 2m

#adding 26.05.2005
set w [$titf1axes getframe]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#Ypos
frame $w.frame.py  -borderwidth 0
pack $w.frame.py  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.yposlabel  -height 0 -text "   Location:" -width 0 
combobox $w.frame.ypos \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Ylabelpos \
    -editable false \
    -command [list SelectYpos ]
eval $w.frame.ypos list insert end [list "left" "middle" "right"]

pack $w.frame.yposlabel -in  $w.frame.py -side left
pack $w.frame.ypos  -in  $w.frame.py  -expand 1 -fill x -pady 2m -padx 2m

#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.gridcolorlabel -height 0 -text "Grid color:" -width 0 
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setYGridColor $w.frame.gridcolor" -tickinterval 0 

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $yGrid

#Y Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.datalabel -text "Data bounds:"
entry $w.frame.databmin -relief sunken  -textvariable dbymin 
entry $w.frame.databmax -relief sunken  -textvariable dbymax
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x 
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x 
bind  $w.frame.databmin <Return> {setYdb} 
bind  $w.frame.databmin <KP_Enter> {setYdb}
bind  $w.frame.databmin <FocusOut> {setYdb}

bind  $w.frame.databmax <Return> {setYdb} 
bind  $w.frame.databmax <KP_Enter> {setYdb}
bind  $w.frame.databmax <FocusOut> {setYdb}

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.scalesw.label -height 0 -text "       Scale:  " -width 0 
radiobutton $w.frame.scalesw.radioLIN -text "Linear" -variable yToggle -value "n"    -command "toggleY" 
radiobutton $w.frame.scalesw.radioLOG -text "Logarithmic" -variable yToggle -value "l" 	  -command "toggleY" 	   

set numpage 1
button $w.frame.scalesw.buttonticks -text "Ticks..." -command "Reload_and_popup $ww $numpage"

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left 
pack $w.frame.scalesw.buttonticks  -in  $w.frame.scalesw    -side left -expand 1 -fill x

#Reverse axis
frame $w.frame.rev  -borderwidth 0
pack $w.frame.rev  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.rev.label -height 0 -text "      Reverse:  " -width 0
checkbutton $w.frame.rev.revvalue  -text "on"\
    -variable Yaxes_reverseToggle -onvalue "on" -offvalue "off" \
    -command "toggleReverselaby $w.frame.rev.revvalue"
OnOffForeground $w.frame.rev.revvalue $Yaxes_reverseToggle

pack $w.frame.rev.label -in $w.frame.rev -side left
pack $w.frame.rev.revvalue -in $w.frame.rev  -side left  -fill x


# #sep bar
# frame $w.sep -height 2 -borderwidth 1 -relief sunken
# pack $w.sep -fill both -pady 10m  

set w $theframe

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom 

########### Z onglet ##############################################
###################################################################
set w [Notebook:frame $uf.n "Z Axis"]

set theframe $w

frame $theframe.frame -borderwidth 0
pack $theframe.frame -anchor w -fill both

#adding 25.05.2005
set topflabel  [frame $theframe.frame.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Label Options"]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Axis Options"]

set w [$titf1label getframe]

pack $titf1label -padx 4 -side top -fill both -expand yes
pack $topflabel -fill x -pady 2
pack $titf1axes  -pady 2 -padx 4 -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#z label
frame $w.frame.lbz -borderwidth 0
pack $w.frame.lbz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.zlabel -text "     Label:  "
entry $w.frame.zlabel1 -relief sunken  -textvariable zlabel 
pack $w.frame.zlabel -in  $w.frame.lbz -side left
pack $w.frame.zlabel1  -in  $w.frame.lbz  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.zlabel1 <Return> {setZlabel} 
bind  $w.frame.zlabel1 <KP_Enter> {setZlabel}
bind  $w.frame.zlabel1 <FocusOut> {setZlabel}

frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.vislablabel  -text "  Visibility:   "
checkbutton $w.frame.vislabb  -text "on"\
    -variable zlabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitylabz $w.frame.vislabb" 
OnOffForeground $w.frame.vislabb $zlabel_visibility

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb  -in $w.frame.vislab  -side left  -fill x

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setZFontLabelColor $w.frame.fontcolor" -tickinterval 0 

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $zlabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontsizlabel -height 0 -text "      Size:  " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setZFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz   -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontsize set $zlabel_fontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 2m

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable Zlabelfontstyle \
    -editable false \
    -command [list SelectZFontStyle ]
eval $w.frame.style list insert end [list "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 2m -padx 2m

#adding 26.05.2005
set w [$titf1axes getframe]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding

#Zpos
# frame $w.frame.pz  -borderwidth 0
# pack $w.frame.pz  -in $w.frame -side top   -fill x

# label $w.frame.zposlabel  -height 0 -text "   Location:" -width 0 
# combobox $w.frame.zpos \
#     -borderwidth 1 \
#     -highlightthickness 1 \
#     -maxheight 0 \
#     -width 3 \
#     -textvariable Zlabelpos \
#     -editable false \
#     -command [list SelectZpos ]
# eval $w.frame.zpos list insert end [list "left" "middle" "right"]

# pack $w.frame.zposlabel -in  $w.frame.pz -side left
# pack $w.frame.zpos  -in  $w.frame.pz -fill x


#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.gridcolorlabel -height 0 -text "Grid color:" -width 0 
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setZGridColor $w.frame.gridcolor" -tickinterval 0 

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $zGrid

#Z Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.datalabel -text "Data bounds:"
entry $w.frame.databmin -relief sunken  -textvariable dbzmin 
entry $w.frame.databmax -relief sunken  -textvariable dbzmax
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x 
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x 
bind  $w.frame.databmin <Return> {setZdb} 
bind  $w.frame.databmin <KP_Enter> {setZdb}
bind  $w.frame.databmin <FocusOut> {setZdb}

bind  $w.frame.databmax <Return> {setZdb} 
bind  $w.frame.databmax <KP_Enter> {setZdb}
bind  $w.frame.databmax <FocusOut> {setZdb}

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.scalesw.label -height 0 -text "       Scale:  " -width 0 
radiobutton $w.frame.scalesw.radioLIN -text "Linear" -variable zToggle -value "n"    -command "toggleZ" 
radiobutton $w.frame.scalesw.radioLOG -text "Logarithmic" -variable zToggle -value "l" 	  -command "toggleZ" 	   

set numpage 2
button $w.frame.scalesw.buttonticks -text "Ticks..." -command "Reload_and_popup $ww $numpage"


pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left 
pack $w.frame.scalesw.buttonticks  -in  $w.frame.scalesw    -side left -expand 1 -fill x

#Reverse axis
frame $w.frame.rev  -borderwidth 0
pack $w.frame.rev  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.rev.label -height 0 -text "      Reverse:  " -width 0
checkbutton $w.frame.rev.revvalue  -text "on"\
    -variable Zaxes_reverseToggle -onvalue "on" -offvalue "off" \
    -command "toggleReverselabz $w.frame.rev.revvalue"
OnOffForeground $w.frame.rev.revvalue $Zaxes_reverseToggle

pack $w.frame.rev.label -in $w.frame.rev -side left
pack $w.frame.rev.revvalue -in $w.frame.rev  -side left  -fill x

# #sep bar
# frame $w.sep -height 2 -borderwidth 1 -relief sunken
# pack $w.sep -fill both -pady 20m

set w $theframe

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom 

########### Title onglet ##########################################
###################################################################
 set w [Notebook:frame $uf.n Title]


frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#title label
frame $w.frame.lbtitle -borderwidth 0
pack $w.frame.lbtitle  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.titlelabel -text "     Label:  "
entry $w.frame.titlelabel1 -relief sunken  -textvariable tlabel 
pack $w.frame.titlelabel -in  $w.frame.lbtitle -side left
pack $w.frame.titlelabel1  -in  $w.frame.lbtitle  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.titlelabel1 <Return> {setTitleLabel} 
bind  $w.frame.titlelabel1 <KP_Enter> {setTitleLabel} 
bind  $w.frame.titlelabel1 <FocusOut> {setTitleLabel} 

#visibility for title
frame $w.frame.vislab -borderwidth 0
pack $w.frame.vislab  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.vislablabel  -text "  Visibility:   "
checkbutton $w.frame.vislabb  -text "on"\
    -variable titlelabel_visibility  -onvalue "on" -offvalue "off" \
    -command "toggleVisibilitytitle $w.frame.vislabb"
OnOffForeground $w.frame.vislabb $titlelabel_visibility

pack $w.frame.vislablabel -in $w.frame.vislab -side left
pack $w.frame.vislabb  -in $w.frame.vislab  -side left  -fill x

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setTitleFontLabelColor $w.frame.fontcolor" -tickinterval 0 

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $titlelabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.fontsizlabel -height 0 -text "      Size:  " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setTitleFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz  -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontsize set $titlelabel_fontsize

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 2m

label $w.frame.stylelabel  -height 0 -text "Font style:" -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable TITLEfontstyle \
    -editable false \
    -command [list SelectTitleFontStyle]
eval $w.frame.style list insert end [list "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -expand 1 -fill x -pady 2m -padx 2m


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 35m  

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom 


########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x -pady 2m
label $w.frame.vislabel  -text "   Visibility:  "
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib"
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left  -fill x -pady 2m -padx 2m

#fill mode
# frame $w.frame.mod -borderwidth 0
# pack $w.frame.mod  -in $w.frame  -side top -fill x -pady 2m
# label $w.frame.modlabel  -text "  Fill mode:  "
# checkbutton $w.frame.modib  -textvariable curfillmode \
#     -variable curfillmode  -onvalue "on" -offvalue "off" \
#     -command "toggleFill" 
# pack $w.frame.modlabel -in $w.frame.mod  -side left
# pack $w.frame.modib  -in $w.frame.mod    -side left  -fill x -pady 2m -padx 2m

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x -pady 2m

label $w.frame.stylelabel  -height 0 -text "Font style:  " -width 0 
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable fontstyle \
    -editable false \
    -command [list SelectFontStyle]
eval $w.frame.style list insert end [list "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"]

pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -expand 1 -fill x -pady 2m -padx 2m


#FontColor scale
frame $w.frame.fontclr  -borderwidth 0
pack $w.frame.fontclr   -in $w.frame -side top  -fill x -pady 2m
label $w.frame.fontcolorlabel -height 0 -text "Font color:  " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0 

pack $w.frame.fontcolorlabel -in $w.frame.fontclr -side left
pack $w.frame.fontcolor -in  $w.frame.fontclr -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $curfontcolor

#Fontsize scale
frame $w.frame.fontssz  -borderwidth 0
pack $w.frame.fontssz  -in $w.frame    -side top -fill x -pady 2m

label $w.frame.fontsizelabel -height 0 -text "Font size:   " -width 0 
scale $w.frame.fontsize -orient horizontal  -from 0 -to 6 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0
pack $w.frame.fontsizelabel  -in $w.frame.fontssz -side left
pack $w.frame.fontsize -in $w.frame.fontssz   -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontsize set $curfontsize


#Color scale foregound
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x -pady 2m
label $w.frame.fcolorlabel -height 0 -text "Foreground\n   color:      " -width 0 

scale $w.frame.fcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setForeColor $w.frame.fcolor" -tickinterval 0 

pack $w.frame.fcolorlabel -in $w.frame.clrf -side left
pack $w.frame.fcolor -in  $w.frame.clrf -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fcolor set $fcolor


#Color scale background
frame $w.frame.clrb  -borderwidth 0
pack $w.frame.clrb  -in $w.frame -side top  -fill x -pady 2m
label $w.frame.bcolorlabel -height 0 -text "Background\n   color: " -width 0 

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackColor $w.frame.bcolor" -tickinterval 0 

pack $w.frame.bcolorlabel -in $w.frame.clrb -side left
pack $w.frame.bcolor -in  $w.frame.clrb -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.bcolor set $bcolor


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x -pady 2m

label $w.frame.scalelabel -height 0 -text "Thickness:   "  -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 30 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack  $w.frame.thickness  -in $w.frame.thk -expand yes -fill x
$w.frame.thickness set $curthick


#Line Style
frame $w.frame.linest  -borderwidth 0
pack $w.frame.linest  -in $w.frame  -side top  -fill x

label $w.frame.linestylelabel  -height 0 -text " Line style:  " -width 0 
combobox $w.frame.linestyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ]
eval $w.frame.linestyle list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]
pack $w.frame.linestylelabel -in $w.frame.linest   -side left
pack $w.frame.linestyle   -in $w.frame.linest   -expand 1 -fill x -pady 2m -padx 2m


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both -pady 5m  

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom

########### Aspect onglet #########################################
###################################################################
set w [Notebook:frame $uf.n Aspect]

frame $w.frame -borderwidth 0
pack $w.frame  -anchor w -fill both


#auto clear
frame $w.frame.clear -borderwidth 0
pack $w.frame.clear  -in $w.frame  -side top  -fill x -pady 1.m
label $w.frame.clearlabel  -text "   Auto clear:  "
checkbutton $w.frame.clearib  -text "on"\
    -variable curautoclear  -onvalue "on" -offvalue "off" \
    -command "toggleClear $w.frame.clearib"
OnOffForeground $w.frame.clearib $curautoclear

pack $w.frame.clearlabel -in $w.frame.clear   -side left
pack $w.frame.clearib  -in $w.frame.clear    -side left -fill x -pady 1.m -padx 1.m

#auto scal
frame $w.frame.scal -borderwidth 0
pack $w.frame.scal  -in $w.frame  -side top -fill x -pady 1.m
label $w.frame.scallabel  -text "   Auto scale:  "
checkbutton $w.frame.scalib  -text "on"\
    -variable curautoscale  -onvalue "on" -offvalue "off" \
    -command "toggleScale $w.frame.scalib"
OnOffForeground $w.frame.scalib $curautoscale

pack $w.frame.scallabel -in $w.frame.scal  -side left
pack $w.frame.scalib  -in $w.frame.scal    -side left  -fill x -pady 1.m -padx 1.m

#box
frame $w.frame.bxd -borderwidth 0
pack $w.frame.bxd  -in $w.frame  -side top -fill x -pady 1.m
label $w.frame.boxlabel  -text "         Boxed:  "
checkbutton $w.frame.box  -text "on"\
    -variable boxToggle  -onvalue "on" -offvalue "off" \
    -command "toggleBox $w.frame.box"
OnOffForeground $w.frame.box $boxToggle

pack $w.frame.boxlabel -in $w.frame.bxd  -side left
pack $w.frame.box  -in $w.frame.bxd    -side left -fill x -pady 1.m -padx 1.m

#isoview
frame $w.frame.iso -borderwidth 0
pack $w.frame.iso  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.isolabel  -text "        Isoview: "
checkbutton $w.frame.isob  -text "on"\
    -variable isoToggle  -onvalue "on" -offvalue "off" \
    -command "toggleIsoview $w.frame.isob"
OnOffForeground $w.frame.isob $isoToggle

pack $w.frame.isolabel -in $w.frame.iso -side left
pack $w.frame.isob  -in $w.frame.iso  -side left -fill x -pady 1.m -padx 1.m

#Tight Limits
frame $w.frame.lim  -borderwidth 0
pack $w.frame.lim  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.limitlabel -text "   Tight limits: "
checkbutton $w.frame.limit  -text "on"\
    -variable limToggle  -onvalue "on" -offvalue "off" \
    -command "toggleLimits $w.frame.limit"
OnOffForeground $w.frame.limit $limToggle

pack $w.frame.limitlabel -in $w.frame.lim -side left
pack $w.frame.limit  -in $w.frame.lim  -side left  -fill x -pady 1.m -padx 1.m


#cubescaling
frame $w.frame.cub -borderwidth 0
pack $w.frame.cub  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.cublabel  -text "Cube scaling: "
checkbutton $w.frame.cubb  -text "on"\
    -variable cubToggle  -onvalue "on" -offvalue "off" \
    -command "toggleCubview $w.frame.cubb"
OnOffForeground $w.frame.cubb $cubToggle

pack $w.frame.cublabel -in $w.frame.cub -side left
pack $w.frame.cubb  -in $w.frame.cub  -side left -fill x -pady 1.m -padx 1.m


#Clip state
frame $w.frame.clpstat  -borderwidth 0
pack $w.frame.clpstat  -in $w.frame -side top -fill x -pady 1.m

label $w.frame.cliplabel  -height 0 -text "   Clip state:  " -width 0 
combobox $w.frame.clip \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curclipstate\
    -editable false \
    -command [list SelectClipState ]
eval $w.frame.clip list insert end [list "on" "off" "clipgrf"]

pack $w.frame.cliplabel -in $w.frame.clpstat   -side left
pack $w.frame.clip -in $w.frame.clpstat   -expand 1 -fill x -pady 1.m -padx 1.m

#clip box
frame $w.frame.lb1 -borderwidth 0
pack $w.frame.lb1  -in $w.frame -side top   -fill x
label $w.frame.labelul -text "  Clip box : upper-left point coordinates "
pack $w.frame.labelul -in  $w.frame.lb1 -side left

frame $w.frame.lb2 -borderwidth 0
pack $w.frame.lb2  -in $w.frame -side top   -fill x

frame $w.frame.lb21 -borderwidth 0
pack $w.frame.lb21  -in $w.frame -side top   -fill x

frame $w.frame.lb22 -borderwidth 0
pack $w.frame.lb22  -in $w.frame -side top   -fill x

label $w.frame.labelx -text "             X: "
entry $w.frame.datax -relief sunken  -textvariable Xclipbox
label $w.frame.labely -text "             Y: "
entry $w.frame.datay -relief sunken  -textvariable Yclipbox

pack $w.frame.labelx  $w.frame.datax  -in  $w.frame.lb2 -side left  -fill x -pady 1.m -padx 1.m
pack $w.frame.labely  $w.frame.datay  -in  $w.frame.lb21 -side left -fill x -pady 1.m -padx 1.m 
bind  $w.frame.datax <Return> {SelectClipBox}
bind  $w.frame.datax <KP_Enter> {SelectClipBox}
bind  $w.frame.datay <Return> {SelectClipBox}
bind  $w.frame.datay <KP_Enter> {SelectClipBox}
#----------------------------#
frame $w.frame.lb3 -borderwidth 0
pack $w.frame.lb3  -in $w.frame -side top   -fill x
label $w.frame.labelwh -text "   Clip box : width and height  "
pack $w.frame.labelwh -in  $w.frame.lb3 -side left

frame $w.frame.lb4 -borderwidth 0
pack $w.frame.lb4  -in $w.frame -side top   -fill x

frame $w.frame.lb41 -borderwidth 0
pack $w.frame.lb41  -in $w.frame -side top   -fill x

label $w.frame.labelw -text "             W: "
entry $w.frame.dataw -relief sunken  -textvariable Wclipbox
label $w.frame.labelh -text "             H: "
entry $w.frame.datah -relief sunken  -textvariable Hclipbox

pack $w.frame.labelw  $w.frame.dataw -in  $w.frame.lb4  -side left -fill x -pady 1.m -padx 1.m
pack $w.frame.labelh  $w.frame.datah -in  $w.frame.lb41 -side left -fill x -pady 1.m -padx 1.m
bind  $w.frame.dataw <Return> {SelectClipBox}
bind  $w.frame.dataw <KP_Enter> {SelectClipBox}
bind  $w.frame.datah <Return> {SelectClipBox}
bind  $w.frame.datah <KP_Enter> {SelectClipBox}

#margins
frame $w.frame.marg1 -borderwidth 0
pack $w.frame.marg1  -in $w.frame -side top   -fill x
label $w.frame.labelmarg -text "  Margins:"
label $w.frame.labelaxesbounds -text "\t\t\t  Axes bounds:"
pack $w.frame.labelmarg $w.frame.labelaxesbounds -in  $w.frame.marg1 -side left

frame $w.frame.marg2 -borderwidth 0
pack $w.frame.marg2  -in $w.frame -side top   -fill x

frame $w.frame.marg21 -borderwidth 0
pack $w.frame.marg21  -in $w.frame -side top   -fill x

label $w.frame.labelleft -text  "           Left: "
entry $w.frame.datamargl -relief sunken  -textvariable Lmargins
label $w.frame.labelleftaxesbounds -text  "   Left: "
entry $w.frame.dataleftaxesbounds -relief sunken  -textvariable axes_boundsL

label $w.frame.labelright -text "         Right:"
entry $w.frame.datamargr -relief sunken  -textvariable Rmargins
label $w.frame.labelupaxesbounds -text  "      Up: "
entry $w.frame.dataupaxesbounds -relief sunken  -textvariable axes_boundsU

pack $w.frame.labelleft  $w.frame.datamargl  $w.frame.labelleftaxesbounds $w.frame.dataleftaxesbounds -in  $w.frame.marg2 -side left  -fill x -pady 1.m -padx 1.m
pack $w.frame.labelright $w.frame.datamargr  $w.frame.labelupaxesbounds $w.frame.dataupaxesbounds -in  $w.frame.marg21 -side left -fill x -pady 1.m -padx 1.m 
bind  $w.frame.datamargl <Return> {SelectMargins}
bind  $w.frame.datamargr <Return> {SelectMargins}
bind  $w.frame.datamargl <KP_Enter> {SelectMargins}
bind  $w.frame.datamargr <KP_Enter> {SelectMargins}
bind  $w.frame.dataleftaxesbounds <Return> {SelectAxesbounds}
bind  $w.frame.dataleftaxesbounds <KP_Enter> {SelectAxesbounds}
bind  $w.frame.dataupaxesbounds   <Return> {SelectAxesbounds}
bind  $w.frame.dataupaxesbounds   <KP_Enter> {SelectAxesbounds}

frame $w.frame.marg4 -borderwidth 0
pack $w.frame.marg4  -in $w.frame -side top   -fill x

frame $w.frame.marg41 -borderwidth 0
pack $w.frame.marg41  -in $w.frame -side top   -fill x

label $w.frame.labeltop -text    "           Top:"
entry $w.frame.datamargt -relief sunken  -textvariable Tmargins
label $w.frame.labelwidthaxesbounds -text    "  Width: "
entry $w.frame.datawidthaxesbounds -relief sunken  -textvariable axes_boundsW

label $w.frame.labelbottom -text "      Bottom:"
entry $w.frame.datamargb -relief sunken  -textvariable Bmargins
label $w.frame.labelheightaxesbounds -text    " Height:"
entry $w.frame.dataheightaxesbounds -relief sunken  -textvariable axes_boundsH

pack $w.frame.labeltop     $w.frame.datamargt $w.frame.labelwidthaxesbounds  $w.frame.datawidthaxesbounds -in  $w.frame.marg4  -side left -fill x -pady 1.m -padx 1.m
pack $w.frame.labelbottom  $w.frame.datamargb $w.frame.labelheightaxesbounds $w.frame.dataheightaxesbounds -in  $w.frame.marg41 -side left -fill x -pady 1.m -padx 1.m
bind  $w.frame.datamargt <Return> {SelectMargins}
bind  $w.frame.datamargb <Return> {SelectMargins}
bind  $w.frame.datawidthaxesbounds   <Return> {SelectAxesbounds}
bind  $w.frame.dataheightaxesbounds  <Return> {SelectAxesbounds}
bind  $w.frame.datamargt <KP_Enter> {SelectMargins}
bind  $w.frame.datamargb <KP_Enter> {SelectMargins}
bind  $w.frame.datawidthaxesbounds   <KP_Enter> {SelectAxesbounds}
bind  $w.frame.dataheightaxesbounds  <KP_Enter> {SelectAxesbounds}


#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both  

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom

########### Viewpoint onglet ######################################
###################################################################
set w [Notebook:frame $uf.n Viewpoint]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#view
 frame $w.frame.view -borderwidth 0
 pack $w.frame.view  -in $w.frame -side top -fill x -pady 2m
 label $w.frame.viewlabel  -text "                 View:  "
 checkbutton $w.frame.viewb  -textvariable viewToggle  \
     -variable viewToggle  -onvalue "2d" -offvalue "3d" \
     -command "toggleView" 
 pack $w.frame.viewlabel -in $w.frame.view -side left
 pack $w.frame.viewb  -in $w.frame.view  -side left  -fill x -pady 2m -padx 2m

#rotation_angles
frame $w.frame.rotang -borderwidth 0
pack $w.frame.rotang  -in $w.frame -side top -fill x -pady 2m
label $w.frame.rotanglabel  -text "Rotation angles: "
entry $w.frame.rotalpha -relief sunken  -textvariable curalpharotation
entry $w.frame.rottheta -relief sunken  -textvariable curthetarotation
pack $w.frame.rotanglabel  -in $w.frame.rotang -side left  -pady 2m -padx 2m
pack $w.frame.rotalpha $w.frame.rottheta -in $w.frame.rotang  -in $w.frame.rotang -side left  -pady 2m -padx 2m
bind  $w.frame.rotalpha <Return> {setAlphaAngle} 
bind  $w.frame.rottheta <Return> {setThetaAngle} 
bind  $w.frame.rotalpha <KP_Enter> {setAlphaAngle} 
bind  $w.frame.rottheta <KP_Enter> {setThetaAngle} 
bind  $w.frame.rotalpha <FocusOut> {setAlphaAngle} 
bind  $w.frame.rottheta <FocusOut> {setThetaAngle} 





#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both  -pady 60m

#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"
pack $w.b -side bottom



pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes



# les proc associes:

proc SelectAxesbounds {} {
    global axes_boundsL axes_boundsU axes_boundsW axes_boundsH
    
    if { ($axes_boundsL == "") || ($axes_boundsU == "") ||($axes_boundsW == "") ||($axes_boundsH == "") } {
	tk_messageBox -icon error -type ok -title "Axes bounds selection failed" -message "You must fill in the 4 fields"
	return
    }
    
    ScilabEval "global ged_handle;ged_handle.axes_bounds=\[$axes_boundsL $axes_boundsU $axes_boundsW $axes_boundsH \]" "seq"
}

proc SelectMargins {} {
global Lmargins Rmargins Tmargins Bmargins
    
    if { ($Lmargins == "") || ($Rmargins == "") ||($Tmargins == "") ||($Bmargins == "") } {
	tk_messageBox -icon error -type ok -title "Margins selection failed" -message "You must fill in the 4 fields"
	return
    }
ScilabEval "global ged_handle;ged_handle.margins=\[$Lmargins $Rmargins $Tmargins $Bmargins \]"
}
proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
}

proc SelectClipBox {} {
    global Xclipbox Yclipbox Wclipbox Hclipbox curclipstate
    if { ($Xclipbox == "") || ($Yclipbox == "") ||($Wclipbox == "") ||($Hclipbox == "") } {
	tk_messageBox -icon error -type ok -title "Clip box selection failed" -message "You must fill in the 4 fields"
	return
    }
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



proc setAlphaAngle {} {
    global curalpharotation curthetarotation viewToggle

    if { ($curalpharotation == "") || ($curthetarotation == "") } {
	tk_messageBox -icon error -type ok -title "Rotation angles selection failed" -message "You must fill in the 2 fields"
	return
    }

    ScilabEval "global ged_handle;ged_handle.rotation_angles=\[$curalpharotation $curthetarotation\]"
   if { ( $curalpharotation == 0) && ( $curthetarotation == 270) } {
	set viewToggle "2d"
    } else {
	set viewToggle "3d"
    }
}

proc setThetaAngle {} {
    global curalpharotation curthetarotation viewToggle
    
    if { ($curalpharotation == "") || ($curthetarotation == "") } {
	tk_messageBox -icon error -type ok -title "Rotation angles selection failed" -message "You must fill in the 2 fields"
	return
    }
    
    ScilabEval "global ged_handle;ged_handle.rotation_angles=\[$curalpharotation $curthetarotation\]"
    if { ( $curalpharotation == 0) && ( $curthetarotation == 270) } {
	set viewToggle "2d"
    } else {
	set viewToggle "3d"
    }
}

proc toggleView {} {
global viewToggle curalpharotation curthetarotation old_curalpharotation old_curthetarotation
ScilabEval "global ged_handle;ged_handle.view='$viewToggle'"
    if { $viewToggle == "2d" } {
	set old_curalpharotation $curalpharotation
	set old_curthetarotation $curthetarotation
	set curalpharotation 0
	set curthetarotation 270
    } else {
	set curalpharotation $old_curalpharotation
	set curthetarotation $old_curthetarotation
    }
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

proc setXdb {} {
    global dbxmin dbxmax

    if { $dbxmin=="" | $dbxmax=="" } {
	return
    }

    if { $dbxmin > $dbxmax } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message " Xmin > Xmax !"
    } else {
	ScilabEval "setXdb($dbxmin, $dbxmax);"
    }
}

proc setYdb {} {
    global dbymin dbymax

    if { $dbymin=="" | $dbymax=="" } {
	return
    }

    if { $dbymin > $dbymax } {
	tk_messageBox -icon error -type ok -title "Incorrect input" -message " Ymin > Ymax !"
    } else {
	ScilabEval "setYdb($dbymin, $dbymax);"
    }
}


proc setZdb {} {
    global dbzmin dbzmax

    if { $dbzmin=="" | $dbzmax=="" } {
	return
    }

    if { $dbzmin > $dbzmax } {
	 tk_messageBox -icon error -type ok -title "Incorrect input" -message " Zmin > Zmax !"
    } else {
	ScilabEval "setZdb($dbzmin, $dbzmax);"
    }
}



proc setForeColor {w index} {  
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

proc setBackColor {w index} {  
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color

    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color

    }
}


proc setColor {w colorindex} {    
global red green blue color
ScilabEval "global ged_handle;ged_handle.foreground=$colorindex;"
}
proc setFontColor {w index} {    
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	
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
proc setFontSize {w fs} {
ScilabEval "global ged_handle;ged_handle.labels_font_size=$fs;"
}


proc toggleVisibilityX { frame } {
    global Xaxes_visibleToggle
    ScilabEval "global ged_handle;ged_handle.axes_visible(1)='$Xaxes_visibleToggle'"

    OnOffForeground $frame $Xaxes_visibleToggle
}

proc toggleVisibilityY { frame } {
    global Yaxes_visibleToggle
    ScilabEval "global ged_handle;ged_handle.axes_visible(2)='$Yaxes_visibleToggle'"
    
    OnOffForeground $frame $Yaxes_visibleToggle
}

proc toggleVisibilityZ { frame } {
    global Zaxes_visibleToggle
    ScilabEval "global ged_handle;ged_handle.axes_visible(3)='$Zaxes_visibleToggle'"
    
    OnOffForeground $frame $Zaxes_visibleToggle
}


proc toggleVisibilitylabx { frame } {
    global xlabel_visibility
    ScilabEval "global ged_handle;ged_handle.x_labels.visible='$xlabel_visibility'"
    
    OnOffForeground $frame $xlabel_visibility
}

proc toggleVisibilitylaby { frame } {
    global ylabel_visibility
    ScilabEval "global ged_handle;ged_handle.y_labels.visible='$ylabel_visibility'"
    
    OnOffForeground $frame $ylabel_visibility
}

proc toggleVisibilitylabz { frame } {
    global zlabel_visibility
    ScilabEval "global ged_handle;ged_handle.z_labels.visible='$zlabel_visibility'"
    
    OnOffForeground $frame $zlabel_visibility
}


proc toggleVisibilitytitle { frame } {
    global titlelabel_visibility
    ScilabEval "global ged_handle;ged_handle.title.visible='$titlelabel_visibility'"

    OnOffForeground $frame $titlelabel_visibility
}



proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}

proc toggleIsoview { frame } {
    global isoToggle
    ScilabEval "global ged_handle;ged_handle.isoview='$isoToggle'"

    OnOffForeground $frame $isoToggle
}

proc toggleCubview { frame } {
    global cubToggle
    ScilabEval "global ged_handle;ged_handle.cube_scaling='$cubToggle'"

    OnOffForeground $frame $cubToggle
}

proc toggleBox { frame } {
    global boxToggle
    ScilabEval "global ged_handle;ged_handle.box='$boxToggle'"

    OnOffForeground $frame $boxToggle
}
proc toggleX {} {
    global xToggle
    ScilabEval "LogtoggleX('$xToggle')"
}
proc toggleY {} {
global yToggle
ScilabEval "LogtoggleY('$yToggle')"
}
proc toggleZ {} {
global zToggle
ScilabEval "LogtoggleZ('$zToggle')"
}
proc toggleLimits { frame } {
    global limToggle
    ScilabEval "global ged_handle;ged_handle.tight_limits='$limToggle'"

    OnOffForeground $frame $limToggle
}


# X LABEL
proc setXGridColor {w index} {
    global RED BLUE GREEN msdos
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.grid(1)=$index;"
	#nothing to draw: grey color used
	if { $msdos == "F" } {
	    set color [format \#e6e7e6]
	} else {
	    set color [format \#d6d3ce]
	}
	$w config  -activebackground $color -troughcolor $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.grid(1)=$index;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; ged_handle.grid(1)=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
}


proc setXlabel {} {
global xlabel
ScilabEval "global ged_handle;ged_handle.x_label.text=ConvStrings('$xlabel')"
}

proc setXFontLabelSize {w siz} {
ScilabEval "global ged_handle;ged_handle.x_label.font_size=$siz"
}

proc setXFontLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}


# Y LABEL
proc setYGridColor {w index} {
    global RED BLUE GREEN msdos
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.grid(2)=$index;"
	#nothing to draw: grey color used
	if { $msdos == "F" } {
	    set color [format \#e6e7e6]
	} else {
	    set color [format \#d6d3ce]
	}
	$w config  -activebackground $color -troughcolor $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.grid(2)=$index;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; ged_handle.grid(2)=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}









proc setYlabel {} {
global ylabel
ScilabEval "global ged_handle;ged_handle.y_label.text=ConvStrings('$ylabel')"
}

proc setYFontLabelSize {w siz} {
ScilabEval "global ged_handle;ged_handle.y_label.font_size=$siz"
}

proc setYFontLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color

    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}


# Z LABEL
proc setZGridColor {w index} {
    global RED BLUE GREEN msdos
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.grid(3)=$index;"
	#nothing to draw: grey color used
	if { $msdos == "F" } {
	    set color [format \#e6e7e6]
	} else {
	    set color [format \#d6d3ce]
	}
	$w config  -activebackground $color -troughcolor $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.grid(3)=$index;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; ged_handle.grid(3)=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setZlabel {} {
global zlabel
ScilabEval "global ged_handle;ged_handle.z_label.text=ConvStrings('$zlabel')"
}

proc setZFontLabelSize {w siz} {
ScilabEval "global ged_handle;ged_handle.z_label.font_size=$siz"
}

proc setZFontLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 

	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
	
    }
}




# Title
proc setTitleLabel {} {
global tlabel
ScilabEval "global ged_handle;ged_handle.title.text=ConvStrings('$tlabel');"
}

proc setTitleFontLabelSize {w siz} {
ScilabEval "global ged_handle;ged_handle.title.font_size=$siz"
}

proc setTitleFontLabelColor {w  index} {
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]

	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}




proc setlabels {} {
global ylabel
global xlabel
ScilabEval "global ged_handle;ged_handle.x_label.text=ConvStrings('$xlabel');ged_handle.y_label.text=ConvStrings('$ylabel')"
}

proc SelectXpos {w args} {
global Xlabelpos
ScilabEval "global ged_handle;ged_handle.x_location='$Xlabelpos'"
}
proc SelectYpos {w args} {
global Ylabelpos
ScilabEval "global ged_handle;ged_handle.y_location='$Ylabelpos'"
}








proc SelectXFontStyle {w args} {
global Xlabelfontstyle
ScilabEval "setLabelsFontStyle('x','$Xlabelfontstyle')"
}

proc SelectYFontStyle {w args} {
global Ylabelfontstyle
ScilabEval "setLabelsFontStyle('y','$Ylabelfontstyle')"
}


proc SelectZFontStyle {w args} {
global Zlabelfontstyle
ScilabEval "setLabelsFontStyle('z','$Zlabelfontstyle')"
}

proc SelectTitleFontStyle {w args} {
global TITLEfontstyle
ScilabEval "setLabelsFontStyle('t','$TITLEfontstyle')"
}


proc SelectFontStyle {w args} {
global fontstyle
ScilabEval "setFontStyle('$fontstyle')"
}



proc toggleClear { frame } {
    global curautoclear
    ScilabEval "global ged_handle;ged_handle.auto_clear='$curautoclear'"
    
    OnOffForeground $frame $curautoclear
}


proc toggleScale { frame } {
    global curautoscale
    ScilabEval "global ged_handle;ged_handle.auto_scale='$curautoscale'"

     OnOffForeground $frame $curautoscale
}


# proc toggleFill {} {
# global curfillmode
# ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"
# }


proc PopUp { w numpage} {
    variable www www2
    global LEI_x LEI_y LEI_z
    global LOCATIONS_X LABELS_X nbticks_x
    global LOCATIONS_Y LABELS_Y nbticks_y
    global LOCATIONS_Z LABELS_Z nbticks_z
    global fen1 fen2 fen3
    global SubticksEntryX SubticksEntryY SubticksEntryZ
    global XautoticksToggle YautoticksToggle ZautoticksToggle
    global StepEntryX StepEntryY StepEntryZ
    global Xaxes_visibleToggle Yaxes_visibleToggle Zaxes_visibleToggle

    set frameaxes $w

    set www .ticks
    catch {destroy $www}

    toplevel $www
    wm withdraw  $www
    update idletasks
    wm transient $www $w
    wm deiconify $www
    
    wm title $www  "Edit Axes Ticks"
    wm geometry $www 405x690
#    wm geometry $www 530x750
    wm iconname $www "TE"
    grab set $www
#    bell -displayof $w
    
    set topf  [frame $www.topf]
    set titf1 [TitleFrame $topf.titf1 -text "Ticks Editor"]
    
    set parent  [$titf1 getframe]
    set pw1  [PanedWindow $parent.pw -side top]
    set pane3  $pw1  
    
    #sauv www
    set old_www $www


    # Make a frame scrollable

    set sw [ScrolledWindow $pane3.sw -relief sunken -borderwidth 2]
    # pack $sw -in .sw -fill both -expand true 
    set sf [ScrollableFrame $sw.f]
    $sw setwidget $sf
    set uf [$sf getframe]

    set w $uf
    set fra [frame $w.frame -borderwidth 0]
    pack $fra  -anchor w -fill both

    
    Notebook:create $uf.n -pages {"X Axis" "Y Axis" "Z Axis"} -pad 20   -height 560 -width 330
    pack $uf.n -in $uf -fill both -expand yes

    Notebook:raise.page $uf.n $numpage

    set w2 $www
    
    ########### X onglet ##############################################
    ###################################################################
    set fen1 [Notebook:frame $uf.n "X Axis"]

#meme choix que Mtlb
    set LEI_x $nbticks_x
    set numpage 0
    
    frame $fen1.frame -borderwidth 0
    pack $fen1.frame -anchor w -fill both


    #visibility of X axis
    frame $fen1.frame.vis -borderwidth 0
    pack $fen1.frame.vis  -in $fen1.frame -side top -fill x -pady 1.m
    label $fen1.frame.vislabel  -text "           Visibility:  "
    checkbutton $fen1.frame.visb  -text "on"\
        -variable Xaxes_visibleToggle  -onvalue "on" -offvalue "off" \
        -command "toggleVisibilityX $fen1.frame.visb"
    OnOffForeground $fen1.frame.visb $Xaxes_visibleToggle

    pack $fen1.frame.vislabel -in $fen1.frame.vis -side left
    pack $fen1.frame.visb  -in $fen1.frame.vis  -side left  -fill x

    frame $fen1.frame.xautoticks -borderwidth 0
    pack $fen1.frame.xautoticks  -in $fen1.frame -side top -fill x -pady 1.m
    label $fen1.frame.xautotickslabel  -text "        Auto ticks:  "
    checkbutton $fen1.frame.xautoticksb  -text "on"\
	-variable XautoticksToggle  -onvalue "on" -offvalue "off" \
	-command "toggleXautoticks $frameaxes $numpage $fen1.frame.xautoticksb"
    OnOffForeground $fen1.frame.xautoticksb $XautoticksToggle

    pack $fen1.frame.xautotickslabel -in $fen1.frame.xautoticks -side left
    pack $fen1.frame.xautoticksb  -in $fen1.frame.xautoticks  -side left  -fill x -pady 1.m -padx 1.m

    frame $fen1.frame.step -borderwidth 0
    pack $fen1.frame.step  -in $fen1.frame -side top -fill x -pady 1.m
    label $fen1.frame.steplabel  -text "           Step by: "
    entry $fen1.frame.stepe  -relief sunken  -justify right \
	-background white -textvariable StepEntryX -width 10
    bind  $fen1.frame.stepe <Return> "SetStep $frameaxes $numpage 1"
    bind  $fen1.frame.stepe <KP_Enter> "SetStep $frameaxes $numpage 1"
    pack $fen1.frame.steplabel $fen1.frame.stepe -in $fen1.frame.step -side left -pady 1.m -padx 1.m

    frame $fen1.frame.subticks -borderwidth 0
    pack $fen1.frame.subticks  -in $fen1.frame -side top -fill x -pady 1.m
    label $fen1.frame.subtickslabel  -text "         Sub ticks: "

#    puts "SubticksEntryX vaut: $SubticksEntryX"

    entry $fen1.frame.subtickse  -relief sunken  -justify right \
	-background white -textvariable SubticksEntryX -width 10
    bind  $fen1.frame.subtickse <Return> "SetSubticksX"
    bind  $fen1.frame.subtickse <KP_Enter> "SetSubticksX"
    pack $fen1.frame.subtickslabel $fen1.frame.subtickse -in $fen1.frame.subticks -side left -pady 1.m -padx 1.m

    frame $fen1.frame.fdata -borderwidth 0
    pack $fen1.frame.fdata  -in $fen1.frame -side top   -fill x
    
    scrollbar $fen1.frame.ysbar -orient vertical -command   {$fen1.frame.c yview}
    canvas $fen1.frame.c -width 8i -height 3i  -yscrollcommand {$fen1.frame.ysbar set}
    
    $fen1.frame.c create text 100 10 -anchor c -text "Locations"
    $fen1.frame.c create text 230 10 -anchor c -text "Labels"
      
    for {set i 1} {$i<=$nbticks_x} {incr i} {
	set bb [expr 10+(25*$i)]
	$fen1.frame.c create text 10 $bb -anchor c -text $i
	#Locations
	set aa [expr 100]
	entry  $fen1.frame.c.locationsdata$i  -relief sunken  -justify right -width 14\
	    -background white -textvariable LOCATIONS_X($i) 
	#	bind  $w.frame.c.locationsdata$i <Return> "setTicksLocations $w $i "
	bind  $fen1.frame.c.locationsdata$i <ButtonPress-1> "setLEI_x $i;"
	bind  $fen1.frame.c.locationsdata$i <Return> "TicksApplyX $fen1"
	bind  $fen1.frame.c.locationsdata$i <KP_Enter> "TicksApplyX $fen1"
	$fen1.frame.c create window $aa $bb -anchor c -window $fen1.frame.c.locationsdata$i
	
	
	#Labels
	set aa [expr 230]
	entry  $fen1.frame.c.labelsdata$i  -relief sunken   -justify left -width 14\
	    -background white -textvariable LABELS_X($i)
	#	bind  $fen1.frame.c.labelsdata$i <Return> "setTicksLabels $w $i "
	bind  $fen1.frame.c.labelsdata$i <ButtonPress-1> "setLEI_x $i;"
	bind  $fen1.frame.c.labelsdata$i <Return> "TicksApplyX $fen1"
	bind  $fen1.frame.c.labelsdata$i <KP_Enter> "TicksApplyX $fen1"
	$fen1.frame.c create window $aa $bb -anchor c -window $fen1.frame.c.labelsdata$i
    }

    $fen1.frame.c configure -scrollregion [$fen1.frame.c bbox all] -yscrollincrement 0.1i
    
    pack  $fen1.frame.ysbar -side right -fill y
    pack  $fen1.frame.c
    
    
    #sep bar
    frame $fen1.sep -height 2 -borderwidth 1 -relief sunken
    pack $fen1.sep -fill both -pady 5m  

    
     #exit button
    frame $fen1.buttons -borderwidth 0
    pack  $fen1.buttons -anchor w -fill both  -side bottom   -fill x
    
    button $fen1.buttons.apply -text Apply -command "TicksApplyX $fen1"
    button $fen1.buttons.b -text Quit -command "destroy $old_www"
    
    pack $fen1.buttons.apply  $fen1.buttons.b -in  $fen1.buttons \
	-side left   -fill x  -expand 1 -pady 2m

    frame $fen1.boutons -borderwidth 0
    pack $fen1.boutons -anchor w -fill both  -side bottom   -fill x
    
    #Insert/ Delete buttons
    button $fen1.boutons.buttoninsert -text Insert -command "TicksInsertX $fen1 $frameaxes "
    button $fen1.boutons.buttondelete -text Delete -command "TicksDeleteX $fen1 $frameaxes "
    pack $fen1.boutons.buttoninsert $fen1.boutons.buttondelete -in  $fen1.boutons \
	-side left   -fill x  -expand 1 -pady 2m
    
    

    ########### Y onglet ##############################################
    ###################################################################
    set fen2 [Notebook:frame $uf.n "Y Axis"]
    
#    puts "1. fen2 vaut: $fen2"
    #meme choix que Mtlb
    set LEI_y $nbticks_y
    set numpage 1


    frame $fen2.frame -borderwidth 0
    pack $fen2.frame -anchor w -fill both

    #visibility of Y axis
    frame $fen2.frame.vis -borderwidth 0
    pack $fen2.frame.vis  -in $fen2.frame -side top -fill x -pady 1.m
    label $fen2.frame.vislabel  -text "           Visibility:  "
    checkbutton $fen2.frame.visb  -text "on"\
        -variable Yaxes_visibleToggle  -onvalue "on" -offvalue "off" \
        -command "toggleVisibilityY $fen2.frame.visb" 
    OnOffForeground $fen2.frame.visb $Yaxes_visibleToggle

    pack $fen2.frame.vislabel -in $fen2.frame.vis -side left
    pack $fen2.frame.visb  -in $fen2.frame.vis  -side left  -fill x

    frame $fen2.frame.yautoticks -borderwidth 0
    pack $fen2.frame.yautoticks  -in $fen2.frame -side top -fill x -pady 1.m
    label $fen2.frame.yautotickslabel  -text "        Auto ticks:  "
    checkbutton $fen2.frame.yautoticksb  -text "on"\
	-variable YautoticksToggle  -onvalue "on" -offvalue "off" \
	-command "toggleYautoticks $frameaxes $numpage $fen2.frame.yautoticksb"
    OnOffForeground $fen2.frame.yautoticksb $YautoticksToggle

    pack $fen2.frame.yautotickslabel -in $fen2.frame.yautoticks -side left
    pack $fen2.frame.yautoticksb  -in $fen2.frame.yautoticks  -side left  -fill x -pady 1.m -padx 1.m

    frame $fen2.frame.step -borderwidth 0
    pack $fen2.frame.step  -in $fen2.frame -side top -fill x -pady 1.m
    label $fen2.frame.steplabel  -text "           Step by: "
    entry $fen2.frame.stepe  -relief sunken  -justify right \
	-background white -textvariable StepEntryY -width 10
    bind  $fen2.frame.stepe <Return> "SetStep $frameaxes $numpage 2"
    bind  $fen2.frame.stepe <KP_Enter> "SetStep $frameaxes $numpage 2"
    pack $fen2.frame.steplabel $fen2.frame.stepe -in $fen2.frame.step -side left -pady 1.m -padx 1.m

    frame $fen2.frame.subticks -borderwidth 0
    pack $fen2.frame.subticks  -in $fen2.frame -side top -fill x -pady 1.m
    label $fen2.frame.subtickslabel  -text "         Sub ticks: "

    entry $fen2.frame.subtickse  -relief sunken  -justify right \
	-background white -textvariable SubticksEntryY -width 10
    bind  $fen2.frame.subtickse <Return> "SetSubticksY"
    bind  $fen2.frame.subtickse <KP_Enter> "SetSubticksY"
    pack $fen2.frame.subtickslabel $fen2.frame.subtickse -in $fen2.frame.subticks -side left -pady 1.m -padx 1.m


    frame $fen2.frame.fdata -borderwidth 0
    pack $fen2.frame.fdata  -in $fen2.frame -side top   -fill x
  
    canvas $fen2.frame.c -width 8i -height 3i  -yscrollcommand {$fen2.frame.ysbar set}
    scrollbar $fen2.frame.ysbar -orient vertical -command   {$fen2.frame.c yview}
    
    $fen2.frame.c create text 100 10 -anchor c -text "Locations"
    $fen2.frame.c create text 230 10 -anchor c -text "Labels"
      
    for {set i 1} {$i<=$nbticks_y} {incr i} {
	set bb [expr 10+(25*$i)]
	$fen2.frame.c create text 10 $bb -anchor c -text $i
	#Locations
	set aa [expr 100]
	entry  $fen2.frame.c.locationsdata$i  -relief sunken  -justify right  -width 14\
	    -background white -textvariable LOCATIONS_Y($i)
	#	bind  $w.frame.c.locationsdata$i <Return> "setTicksLocations $w $i "
	bind  $fen2.frame.c.locationsdata$i <ButtonPress-1> "setLEI_y $i;"
	bind  $fen2.frame.c.locationsdata$i <Return> "TicksApplyY $fen2"
	bind  $fen2.frame.c.locationsdata$i <KP_Enter> "TicksApplyY $fen2"
	$fen2.frame.c create window $aa $bb -anchor c -window $fen2.frame.c.locationsdata$i
	
	
	#Labels
	set aa [expr 230]
	entry  $fen2.frame.c.labelsdata$i  -relief sunken   -justify left  -width 14\
	    -background white -textvariable LABELS_Y($i)
	#	bind  $fen2.frame.c.labelsdata$i <Return> "setTicksLabels $w $i "
	bind  $fen2.frame.c.labelsdata$i <ButtonPress-1> "setLEI_y $i;"
	bind  $fen2.frame.c.labelsdata$i <Return> "TicksApplyY $fen2"
	bind  $fen2.frame.c.labelsdata$i <KP_Enter> "TicksApplyY $fen2"
	$fen2.frame.c create window $aa $bb -anchor c -window $fen2.frame.c.labelsdata$i
    }

    $fen2.frame.c configure -scrollregion [$fen2.frame.c bbox all] -yscrollincrement 0.1i
    
    pack  $fen2.frame.ysbar -side right -fill y
    pack  $fen2.frame.c
    
    #sep bar
    frame $fen2.sep -height 2 -borderwidth 1 -relief sunken
    pack $fen2.sep -fill both -pady 5m  

    
     #exit button
    frame $fen2.buttons -borderwidth 0
    pack  $fen2.buttons -anchor w -fill both  -side bottom   -fill x
    
    button $fen2.buttons.apply -text Apply -command "TicksApplyY $fen2"
    button $fen2.buttons.b -text Quit -command "destroy $old_www"
    
    pack $fen2.buttons.apply  $fen2.buttons.b -in  $fen2.buttons \
	-side left   -fill x  -expand 1 -pady 2m
    
    frame $fen2.boutons -borderwidth 0
    pack $fen2.boutons -anchor w -fill both  -side bottom   -fill x
    
    #Insert/ Delete buttons
    button $fen2.boutons.buttoninsert -text Insert -command "TicksInsertY $fen2 $frameaxes "
    button $fen2.boutons.buttondelete -text Delete -command "TicksDeleteY $fen2 $frameaxes "
    pack $fen2.boutons.buttoninsert $fen2.boutons.buttondelete -in  $fen2.boutons \
	-side left   -fill x  -expand 1 -pady 2m
    
    


     ########### Z onglet ##############################################
     ###################################################################
     set fen3 [Notebook:frame $uf.n "Z Axis"]
  
    #meme choix que Mtlb
    set LEI_z $nbticks_z
    set numpage 2


    frame $fen3.frame -borderwidth 0
    pack $fen3.frame -anchor w -fill both

    #visibility of Z axis
    frame $fen3.frame.vis -borderwidth 0
    pack $fen3.frame.vis  -in $fen3.frame -side top -fill x -pady 1.m
    label $fen3.frame.vislabel  -text "           Visibility:  "
    checkbutton $fen3.frame.visb  -text "on"\
        -variable Zaxes_visibleToggle  -onvalue "on" -offvalue "off" \
        -command "toggleVisibilityZ $fen3.frame.visb"
    OnOffForeground $fen3.frame.visb $Zaxes_visibleToggle

    pack $fen3.frame.vislabel -in $fen3.frame.vis -side left
    pack $fen3.frame.visb  -in $fen3.frame.vis  -side left  -fill x

    frame $fen3.frame.zautoticks -borderwidth 0
    pack $fen3.frame.zautoticks  -in $fen3.frame -side top -fill x -pady 1.m
    label $fen3.frame.zautotickslabel  -text "        Auto ticks:  "
    checkbutton $fen3.frame.zautoticksb  -text "on"\
	-variable ZautoticksToggle  -onvalue "on" -offvalue "off" \
	-command "toggleZautoticks $frameaxes $numpage $fen3.frame.zautoticksb"
    OnOffForeground $fen3.frame.zautoticksb $ZautoticksToggle

    pack $fen3.frame.zautotickslabel -in $fen3.frame.zautoticks -side left
    pack $fen3.frame.zautoticksb  -in $fen3.frame.zautoticks  -side left  -fill x -pady 1.m -padx 1.m

    frame $fen3.frame.step -borderwidth 0
    pack $fen3.frame.step  -in $fen3.frame -side top -fill x -pady 1.m
    label $fen3.frame.steplabel  -text "           Step by: "
    entry $fen3.frame.stepe  -relief sunken  -justify right \
	-background white -textvariable StepEntryZ -width 10
    bind  $fen3.frame.stepe <Return> "SetStep $frameaxes $numpage 3"
    bind  $fen3.frame.stepe <KP_Enter> "SetStep $frameaxes $numpage 3"
    pack $fen3.frame.steplabel $fen3.frame.stepe -in $fen3.frame.step -side left -pady 1.m -padx 1.m

    frame $fen3.frame.subticks -borderwidth 0
    pack $fen3.frame.subticks  -in $fen3.frame -side top -fill x -pady 1.m
    label $fen3.frame.subtickslabel  -text "         Sub ticks: "

    entry $fen3.frame.subtickse  -relief sunken  -justify right \
	-background white -textvariable SubticksEntryZ -width 10
    bind  $fen3.frame.subtickse <Return> "SetSubticksZ"
    bind  $fen3.frame.subtickse <KP_Enter> "SetSubticksZ"
    pack $fen3.frame.subtickslabel $fen3.frame.subtickse -in $fen3.frame.subticks -side left -pady 1.m -padx 1.m


    frame $fen3.frame.fdata -borderwidth 0
    pack $fen3.frame.fdata  -in $fen3.frame -side top   -fill x
  
    canvas $fen3.frame.c -width 8i -height 3i  -yscrollcommand {$fen3.frame.ysbar set}
    scrollbar $fen3.frame.ysbar -orient vertical -command   {$fen3.frame.c yview}
    
    $fen3.frame.c create text 100 10 -anchor c -text "Locations"
    $fen3.frame.c create text 230 10 -anchor c -text "Labels"
      
    for {set i 1} {$i<=$nbticks_z} {incr i} {
	set bb [expr 10+(25*$i)]
	$fen3.frame.c create text 10 $bb -anchor c -text $i
	#Locations
	set aa [expr 100]
	entry  $fen3.frame.c.locationsdata$i  -relief sunken  -justify right  -width 14\
	    -background white -textvariable LOCATIONS_Z($i)
	#	bind  $w.frame.c.locationsdata$i <Return> "setTicksLocations $w $i "
	bind  $fen3.frame.c.locationsdata$i <ButtonPress-1> "setLEI_z $i;"
	bind  $fen3.frame.c.locationsdata$i <Return> "TicksApplyZ $fen3"
	bind  $fen3.frame.c.locationsdata$i <KP_Enter> "TicksApplyZ $fen3"
	$fen3.frame.c create window $aa $bb -anchor c -window $fen3.frame.c.locationsdata$i
	
	
	#Labels
	set aa [expr 230]
	entry  $fen3.frame.c.labelsdata$i  -relief sunken   -justify left  -width 14\
	    -background white -textvariable LABELS_Z($i)
	#	bind  $fen3.frame.c.labelsdata$i <Return> "setTicksLabels $w $i "
	bind  $fen3.frame.c.labelsdata$i <ButtonPress-1> "setLEI_z $i;"
	bind  $fen3.frame.c.labelsdata$i <Return> "TicksApplyZ $fen3"
	bind  $fen3.frame.c.labelsdata$i <KP_Enter> "TicksApplyZ $fen3"
	$fen3.frame.c create window $aa $bb -anchor c -window $fen3.frame.c.labelsdata$i
    }

    $fen3.frame.c configure -scrollregion [$fen3.frame.c bbox all] -yscrollincrement 0.1i
    
    pack  $fen3.frame.ysbar -side right -fill y
    pack  $fen3.frame.c
    
    
    #sep bar
    frame $fen3.sep -height 2 -borderwidth 1 -relief sunken
    pack $fen3.sep -fill both -pady 5m  

    
     #exit button
    frame $fen3.buttons -borderwidth 0
    pack  $fen3.buttons -anchor w -fill both  -side bottom   -fill x
    
    button $fen3.buttons.apply -text Apply -command "TicksApplyZ $fen3"
    button $fen3.buttons.b -text Quit -command "destroy $old_www"
    
    pack $fen3.buttons.apply  $fen3.buttons.b -in  $fen3.buttons \
	-side left   -fill x  -expand 1 -pady 2m
    
    frame $fen3.boutons -borderwidth 0
    pack $fen3.boutons -anchor w -fill both  -side bottom   -fill x
    
    #Insert/ Delete buttons
    button $fen3.boutons.buttoninsert -text Insert -command "TicksInsertZ $fen3 $frameaxes "
    button $fen3.boutons.buttondelete -text Delete -command "TicksDeleteZ $fen3 $frameaxes "
    pack $fen3.boutons.buttoninsert $fen3.boutons.buttondelete -in  $fen3.boutons \
	-side left   -fill x  -expand 1 -pady 2m
    


    pack $sw $pw1 -fill both -expand yes
    pack $titf1 -padx 4 -side left -fill both -expand yes
    pack $topf -fill both -pady 2 -expand yes




}


############## TICKS ##############

### X

proc TicksInsertX { ww w } {
    global tmploc tmplab
    global LOCATIONS_X LABELS_X
    global LEI_x
    global nbticks_x

    set i $LEI_x

    if {$i == -1} {
	tk_messageBox -icon error -type ok -title "Insertion failed" -message "Select a cell first to insert after"
	return
    }

    for {set j 1} {$j<=$nbticks_x} {incr j} {
	set tmploc($j) $LOCATIONS_X($j)
	set tmplab($j) $LABELS_X($j)

	if {$j == $i} {
	    set tmploc($j) $LOCATIONS_X($j)
	    set tmplab($j) $LABELS_X($j)
	    break
	}
    }
    set fin [expr $nbticks_x+1]
    set index [expr $i+1]
    for {set j $index} {$j<=$fin} {incr j} {
	set ind [expr $j-1]
	set tmploc($j) $LOCATIONS_X($ind)
	set tmplab($j) $LABELS_X($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_X($j) $tmploc($j)
	set LABELS_X($j) $tmplab($j)
    }

    set nbticks_x $fin

    set numpage 0
    PopUp $w $numpage

}





proc TicksDeleteX { ww w } {
    global tmploc tmplab
    global LOCATIONS_X LABELS_X
    global LEI_x
    global nbticks_x


    set i $LEI_x

    if {$nbticks_x <= 2} {
	tk_messageBox -icon error -type ok -title "Deletion forbidden" -message "You must have at least 2 ticks"
	return
    }

    for {set j 1} {$j<=$nbticks_x} {incr j} {
	set tmploc($j) $LOCATIONS_X($j)
	set tmplab($j) $LABELS_X($j)

	if {$j == $i} {
	    break
	}
    }
    set fin [expr $nbticks_x-1]
    set index [expr $i+1]
    for {set j $i} {$j<=$fin} {incr j} {
	set ind [expr $j+1]
	set tmploc($j) $LOCATIONS_X($ind)
	set tmplab($j) $LABELS_X($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_X($j) $tmploc($j)
	set LABELS_X($j) $tmplab($j)
    }

    set nbticks_x $fin

    set numpage 0
    PopUp $w $numpage
    
}

proc TicksApplyX { w } {
    global LOCATIONS_X LABELS_X
    global nbticks_x 
    global XautoticksToggle
    #First, we pass the TLIST in 2 arrays to scilab
    #LOCATIONS_X
    for {set i 1} {$i<=$nbticks_x} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab($LOCATIONS_X($index),$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab2($LOCATIONS_X($index),$index,ged_tmp_LOCATIONS)"  "seq"
	}
    }
     #LABELS_X
    for {set i 1} {$i<=$nbticks_x} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LABELS=GetTab('$LABELS_X($index)',$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LABELS=GetTab2('$LABELS_X($index)',$index,ged_tmp_LABELS)"  "seq"
	}
    }
    
    ScilabEval "setTicksTList('X',ged_tmp_LOCATIONS,ged_tmp_LABELS)" "seq"
    
    set XautoticksToggle "off"
    OnOffForeground $w.frame.xautoticksb $XautoticksToggle
}


### Y

proc TicksInsertY { ww w } {
    global tmploc tmplab
    global LOCATIONS_Y LABELS_Y
    global LEI_y
    global nbticks_y
 
    set i $LEI_y

    if {$i == -1} {
	tk_messageBox -icon error -type ok -title "Insertion failed" -message "Select a cell first to insert after"
	return
    }

    for {set j 1} {$j<=$nbticks_y} {incr j} {
	set tmploc($j) $LOCATIONS_Y($j)
	set tmplab($j) $LABELS_Y($j)

	if {$j == $i} {
	    set tmploc($j) $LOCATIONS_Y($j)
	    set tmplab($j) $LABELS_Y($j)
	    break
	}
    }
    set fin [expr $nbticks_y+1]
    set index [expr $i+1]
    for {set j $index} {$j<=$fin} {incr j} {
	set ind [expr $j-1]
	set tmploc($j) $LOCATIONS_Y($ind)
	set tmplab($j) $LABELS_Y($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Y($j) $tmploc($j)
	set LABELS_Y($j) $tmplab($j)
    }

    set nbticks_y $fin

    set numpage 1
    PopUp $w $numpage

}





proc TicksDeleteY { ww w } {
    global tmploc tmplab
    global LOCATIONS_Y LABELS_Y
    global LEI_y
    global nbticks_y


    set i $LEI_y

    if {$nbticks_y <= 2} {
	tk_messageBox -icon error -type ok -title "Deletion forbidden" -message "You must have at least 2 ticks"
	return
    }

    for {set j 1} {$j<=$nbticks_y} {incr j} {
	set tmploc($j) $LOCATIONS_Y($j)
	set tmplab($j) $LABELS_Y($j)

	if {$j == $i} {
	    break
	}
    }
    set fin [expr $nbticks_y-1]
    set index [expr $i+1]
    for {set j $i} {$j<=$fin} {incr j} {
	set ind [expr $j+1]
	set tmploc($j) $LOCATIONS_Y($ind)
	set tmplab($j) $LABELS_Y($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Y($j) $tmploc($j)
	set LABELS_Y($j) $tmplab($j)
    }

    set nbticks_y $fin

    set numpage 1
    PopUp $w $numpage
    
}

proc TicksApplyY { w } {
    global LOCATIONS_Y LABELS_Y
    global nbticks_y
    global YautoticksToggle

    #First, we pass the TLIST in 2 arrays to scilab
    #LOCATIONS_Y
    for {set i 1} {$i<=$nbticks_y} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab($LOCATIONS_Y($index),$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab2($LOCATIONS_Y($index),$index,ged_tmp_LOCATIONS)"  "seq"
	}
    }
     #LABELS_Y
    for {set i 1} {$i<=$nbticks_y} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LABELS=GetTab('$LABELS_Y($index)',$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LABELS=GetTab2('$LABELS_Y($index)',$index,ged_tmp_LABELS)"  "seq"
	}
    }
    
    ScilabEval "setTicksTList('Y',ged_tmp_LOCATIONS,ged_tmp_LABELS)" "seq"
    
    set YautoticksToggle "off"
    OnOffForeground $w.frame.yautoticksb $YautoticksToggle
}



### Z

proc TicksInsertZ { ww w } {
    global tmploc tmplab
    global LOCATIONS_Z LABELS_Z
    global LEI_z
    global nbticks_z

    set i $LEI_z

    if {$i == -1} {
	tk_messageBox -icon error -type ok -title "Insertion failed" -message "Select a cell first to insert after"
	return
    }

    for {set j 1} {$j<=$nbticks_z} {incr j} {
	set tmploc($j) $LOCATIONS_Z($j)
	set tmplab($j) $LABELS_Z($j)

	if {$j == $i} {
	    set tmploc($j) $LOCATIONS_Z($j)
	    set tmplab($j) $LABELS_Z($j)
	    break
	}
    }
    set fin [expr $nbticks_z+1]
    set index [expr $i+1]
    for {set j $index} {$j<=$fin} {incr j} {
	set ind [expr $j-1]
	set tmploc($j) $LOCATIONS_Z($ind)
	set tmplab($j) $LABELS_Z($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Z($j) $tmploc($j)
	set LABELS_Z($j) $tmplab($j)
    }

    set nbticks_z $fin

    set numpage 2
    PopUp $w $numpage

}





proc TicksDeleteZ { ww w } {
    global tmploc tmplab
    global LOCATIONS_Z LABELS_Z
    global LEI_z
    global nbticks_z


    set i $LEI_z

    if {$nbticks_z <= 2} {
	tk_messageBox -icon error -type ok -title "Deletion forbidden" -message "You must have at least 2 ticks"
	return
    }

    for {set j 1} {$j<=$nbticks_z} {incr j} {
	set tmploc($j) $LOCATIONS_Z($j)
	set tmplab($j) $LABELS_Z($j)

	if {$j == $i} {
	    break
	}
    }
    set fin [expr $nbticks_z-1]
    set index [expr $i+1]
    for {set j $i} {$j<=$fin} {incr j} {
	set ind [expr $j+1]
	set tmploc($j) $LOCATIONS_Z($ind)
	set tmplab($j) $LABELS_Z($ind)
    }

    for {set j 1} {$j<=$fin} {incr j} {
	set LOCATIONS_Z($j) $tmploc($j)
	set LABELS_Z($j) $tmplab($j)
    }

    set nbticks_z $fin

    set numpage 2
    PopUp $w $numpage
    
}

proc TicksApplyZ { w } {
    global LOCATIONS_Z LABELS_Z
    global nbticks_z
    global ZautoticksToggle

    #First, we pass the TLIST in 2 arrays to scilab
    #LOCATIONS_Z
    for {set i 1} {$i<=$nbticks_z} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab($LOCATIONS_Z($index),$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LOCATIONS=GetTab2($LOCATIONS_Z($index),$index,ged_tmp_LOCATIONS)"  "seq"
	}
    }
     #LABELS_Z
    for {set i 1} {$i<=$nbticks_z} {incr i} {
	set index $i

	if { $i == 1 } {
	    ScilabEval "ged_tmp_LABELS=GetTab('$LABELS_Z($index)',$index)"  "seq"
	} else {
	    ScilabEval "ged_tmp_LABELS=GetTab2('$LABELS_Z($index)',$index,ged_tmp_LABELS)"  "seq"
	}
    }
    
    ScilabEval "setTicksTList('Z',ged_tmp_LOCATIONS,ged_tmp_LABELS)" "seq"
    
    set ZautoticksToggle "off"
    OnOffForeground $w.frame.zautoticksb $ZautoticksToggle
}




proc setLEI_x { i } {
    global LEI_x

    set LEI_x $i
#    return $LEI
}

proc setLEI_y { i } {
    global LEI_y

    set LEI_y $i
#    return $LEI
}

proc setLEI_z { i } {
    global LEI_z

    set LEI_z $i
#    return $LEI
}

proc toggleXautoticks { win numpage frame } {
    global XautoticksToggle
    
    ScilabEval "global ged_handle;ged_handle.auto_ticks(1)='$XautoticksToggle'" "seq"
    ScilabEval "TCL_EvalStr(\"Reload_and_popup \"+\"$win $numpage\")" "seq"

    OnOffForeground $frame $XautoticksToggle
}

proc toggleYautoticks { win numpage frame } {
    global YautoticksToggle
    
    ScilabEval "global ged_handle;ged_handle.auto_ticks(2)='$YautoticksToggle'" "seq"
    ScilabEval "TCL_EvalStr(\"Reload_and_popup \"+\"$win $numpage\")" "seq"

    OnOffForeground $frame $YautoticksToggle
}

proc toggleZautoticks { win numpage frame } {
    global ZautoticksToggle
    
    ScilabEval "global ged_handle;ged_handle.auto_ticks(3)='$ZautoticksToggle'" "seq"
    ScilabEval "TCL_EvalStr(\"Reload_and_popup \"+\"$win $numpage\")" "seq"

    OnOffForeground $frame $ZautoticksToggle
}


proc SetStep { w numpage xyz} {
    # I rely on a.data_bounds data
    variable min max
    variable step
    global StepEntryX StepEntryY StepEntryZ
    global dbxmin dbxmax dbymin dbymax dbzmin dbzmax
    global LOCATIONS_X LABELS_X
    global LOCATIONS_Y LABELS_Y
    global LOCATIONS_Z LABELS_Z
    global nbticks_x nbticks_y nbticks_z
    
    if { $xyz == 1 } {
	set min $dbxmin
	set max $dbxmax
	set nbticks_x 0
	set step $StepEntryX
    } else {
	if { $xyz == 2 } {
	    set min $dbymin
	    set max $dbymax
	    set nbticks_y 0
	    set step $StepEntryY
	} else {
	    if { $xyz == 3 } {
		set min $dbzmin
		set max $dbzmax
		set nbticks_z 0
		set step $StepEntryZ
	    }
	}
    }
    
    set min [expr floor($min)]
    set max [expr ceil ($max)]
    
    set x 0
    set i 0
    
#     puts "tout debut"
#     puts "min = $min"
#     puts "max = $max"
    
    if {$step <= 0  | $step==""} {
	tk_messageBox -icon error -type ok -title "Wrong Step" -message "The step must be positive !"
	return
    }
    
    set x $min
    
    while {$x<$max} {

	set ii [expr $i+1]
	if { $xyz == 1 } {
	    set LOCATIONS_X($ii) $x
	    set LABELS_X($ii) $x
	    set nbticks_x [expr $nbticks_x +1]
	} else {
	    if { $xyz == 2 } {
		set LOCATIONS_Y($ii) $x
		set LABELS_Y($ii) $x
		set nbticks_y [expr $nbticks_y +1]
	    } else {
		if { $xyz == 3 } {
		    set LOCATIONS_Z($ii) $x
		    set LABELS_Z($ii) $x
		    set nbticks_z [expr $nbticks_z +1]
		}
	    }
	}
	incr i
	set x [expr $min + $i*$step]
    }

#     puts "AVANT avant one more to finish"
  
#     puts "i= $i"
    # one more to finish
#    incr i
    set ii [expr $i+1]
#     puts "avant one more to finish"
#     puts "i= $i"
#     puts "step = $step"
#     puts "min = $min"
    set x $max
    if { $xyz == 1 } {
	set LOCATIONS_X($ii) $x
	set LABELS_X($ii) $x
	set nbticks_x [expr $nbticks_x +1]
    } else {
	if { $xyz == 2 } {
	    set LOCATIONS_Y($ii) $x
	    set LABELS_Y($ii) $x
	    set nbticks_y [expr $nbticks_y +1]
	} else {
	    if { $xyz == 3 } {
		set LOCATIONS_Z($ii) $x
		set LABELS_Z($ii) $x
		set nbticks_z [expr $nbticks_z +1]
	    }
	}
    }
    
    PopUp $w $numpage
}



proc SetSubticksX { } {
    global SubticksEntryX

    if {$SubticksEntryX < 0 | $SubticksEntryX==""} {
	tk_messageBox -icon error -type ok -title "Incorrect sub ticks value" -message "Select a positive integer value"
	return
    }
    ScilabEval "Subtickstoggle($SubticksEntryX,1)" "seq"
}


proc SetSubticksY { } {
    global SubticksEntryY
 
    if {$SubticksEntryY < 0 | $SubticksEntryY==""} {
	tk_messageBox -icon error -type ok -title "Incorrect sub ticks value" -message "Select a positive integer value"
	return
    }
    ScilabEval "Subtickstoggle($SubticksEntryY,2)" "seq"
}

proc SetSubticksZ { } {
    global SubticksEntryZ
    
    if {$SubticksEntryZ < 0 | $SubticksEntryZ==""} {
	tk_messageBox -icon error -type ok -title "Incorrect sub ticks value" -message "Select a positive integer value"
	return
    }
    ScilabEval "Subtickstoggle($SubticksEntryZ,3)" "seq"
}

proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"
}


proc Reload_and_popup { win numpage } {
    global LOCATIONS_X LABELS_X
    global LOCATIONS_Y LABELS_Y
    global LOCATIONS_Z LABELS_Z
    global nbticks_x nbticks_y nbticks_z
    
    ScilabEval "global ged_handle; ReLoadTicks2TCL(ged_handle); " "seq"
    ScilabEval "TCL_EvalStr(\"PopUp \"+\"$win $numpage\")" "seq"

}


proc toggleReverselabx { frame } {
    global Xaxes_reverseToggle
    ScilabEval "global ged_handle;ged_handle.axes_reverse(1)='$Xaxes_reverseToggle'"

    OnOffForeground $frame $Xaxes_reverseToggle

}

proc toggleReverselaby { frame } {
    global Yaxes_reverseToggle
    ScilabEval "global ged_handle;ged_handle.axes_reverse(2)='$Yaxes_reverseToggle'"

    OnOffForeground $frame $Yaxes_reverseToggle
}

proc toggleReverselabz { frame } {
    global Zaxes_reverseToggle
    ScilabEval "global ged_handle;ged_handle.axes_reverse(3)='$Zaxes_reverseToggle'"

    OnOffForeground $frame $Zaxes_reverseToggle
}
