
#  set env(SCIPATH) "~/scilab/"

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

global  visToggle limToggle boxToggle isoToggle gridToggle 
global  xToggle yToggle red green blue color 
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

#axes_visible
global visToggle

global xGrif yGrid zGrid

global viewToggle isoToggle cubToggle
global dbxmin dbxmax dbymin dbymax dbzmin dbzmax

global msdos

global Xlabelfontstyle Ylabelfontstyle Zlabelfontstyle TITLEfontstyle fontstyle

# add for XF init only : to remove after...


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


set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Axes Editor"
wm iconname $ww "AE"
wm geometry $ww 650x700
#wm maxsize  $ww 450 560


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

label $w.frame.selgedobjectlabel  -height 0 -text "Edit properties for:    " -width 0 
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

Notebook:create .axes.n -pages {X Y Z Title Style Aspect Viewpoint} -pad 20 
pack .axes.n -fill both -expand 1


########### X onglet ##############################################
###################################################################
set w [Notebook:frame .axes.n X]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#x label
frame $w.frame.lbx -borderwidth 0
pack $w.frame.lbx  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.xlabel -text "     Label:  "
entry $w.frame.xlabel1 -relief sunken  -textvariable xlabel 
pack $w.frame.xlabel -in  $w.frame.lbx -side left
pack $w.frame.xlabel1  -in  $w.frame.lbx  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.xlabel1 <Return> {setXlabel} 

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

pack $w.frame.gridcolorlabel -in  $w.frame.gridcol -side left 
pack  $w.frame.gridcolor  -in  $w.frame.gridcol    -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $xGrid

#X Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.datalabel -text "Data bounds:"
entry $w.frame.databmin -relief sunken  -textvariable dbxmin 
entry $w.frame.databmax -relief sunken  -textvariable dbxmax
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab  -side left -expand 1 -fill x 
pack $w.frame.databmax -in  $w.frame.datab  -side left -expand 1 -fill x 
bind  $w.frame.databmin <Return> {setXdb} 
bind  $w.frame.databmax <Return> {setXdb} 

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.scalesw.label -height 0 -text "       Scale:  " -width 0 
radiobutton $w.frame.scalesw.radioLIN -text "Linear" -variable xToggle -value "n"    -command "toggleX" 
radiobutton $w.frame.scalesw.radioLOG -text "Logarithmic" -variable xToggle -value "l" 	  -command "toggleX" 	   

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left 

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom

########### Y onglet ##############################################
###################################################################
 set w [Notebook:frame .axes.n Y]


frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#y label
frame $w.frame.lby -borderwidth 0
pack $w.frame.lby  -in $w.frame -side top   -fill x  -pady 2m

label $w.frame.ylabel -text "     Label:  "
entry $w.frame.ylabel1 -relief sunken  -textvariable ylabel 
pack $w.frame.ylabel -in  $w.frame.lby -side left
pack $w.frame.ylabel1  -in  $w.frame.lby  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.ylabel1 <Return> {setYlabel} 

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
bind  $w.frame.databmax <Return> {setYdb} 

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.scalesw.label -height 0 -text "       Scale:  " -width 0 
radiobutton $w.frame.scalesw.radioLIN -text "Linear" -variable yToggle -value "n"    -command "toggleY" 
radiobutton $w.frame.scalesw.radioLOG -text "Logarithmic" -variable yToggle -value "l" 	  -command "toggleY" 	   

pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left 
pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left 

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 

########### Z onglet ##############################################
###################################################################
 set w [Notebook:frame .axes.n Z]


frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#z label
frame $w.frame.lbz -borderwidth 0
pack $w.frame.lbz  -in $w.frame -side top   -fill x -pady 2m

label $w.frame.zlabel -text "     Label:  "
entry $w.frame.zlabel1 -relief sunken  -textvariable zlabel 
pack $w.frame.zlabel -in  $w.frame.lbz -side left
pack $w.frame.zlabel1  -in  $w.frame.lbz  -expand 1 -fill x -pady 2m -padx 2m
bind  $w.frame.zlabel1 <Return> {setZlabel} 

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
pack  $w.frame.gridcolor -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
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
bind  $w.frame.databmax <Return> {setZdb} 

#Scale log or linear
#frame $w.frame.scalesw  -borderwidth 0
#pack $w.frame.scalesw  -in $w.frame -side top   -fill x
#
#label $w.frame.scalesw.label -height 0 -text "       Scale:  " -width 0 
#radiobutton $w.frame.scalesw.radioLIN -text "Linear" -variable zToggle -value "n"    -command "toggleZ" 
#radiobutton $w.frame.scalesw.radioLOG -text "Logarithmic" -variable zToggle -value "l" 	  -command "toggleZ" 	   
#
#pack $w.frame.scalesw.label  -in  $w.frame.scalesw -side left 
#pack $w.frame.scalesw.radioLIN -in  $w.frame.scalesw -side left 
#pack $w.frame.scalesw.radioLOG  -in  $w.frame.scalesw    -side left 

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 

########### Title onglet ##############################################
###################################################################
 set w [Notebook:frame .axes.n Title]


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


#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 


########### Style onglet ##########################################
###################################################################
set w [Notebook:frame .axes.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x -pady 2m
label $w.frame.vislabel  -text "   Visibility:  "
checkbutton $w.frame.visib  -textvariable curvis  \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis" 
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


#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom

########### Aspect onglet #########################################
###################################################################
set w [Notebook:frame .axes.n Aspect]

frame $w.frame -borderwidth 0
pack $w.frame  -anchor w -fill both


#visible
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.vislabel  -text "        Visible:  "
checkbutton $w.frame.visb  -textvariable visToggle  \
    -variable visToggle  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.vislabel -in $w.frame.vis -side left
pack $w.frame.visb  -in $w.frame.vis  -side left  -fill x -pady 1.m -padx 1.m

#auto clear
frame $w.frame.clear -borderwidth 0
pack $w.frame.clear  -in $w.frame  -side top  -fill x -pady 1.m
label $w.frame.clearlabel  -text "  Auto clear:  "
checkbutton $w.frame.clearib  -textvariable curautoclear \
    -variable curautoclear  -onvalue "on" -offvalue "off" \
    -command "toggleClear" 
pack $w.frame.clearlabel -in $w.frame.clear   -side left
pack $w.frame.clearib  -in $w.frame.clear    -side left -fill x -pady 1.m -padx 1.m

#auto scal
frame $w.frame.scal -borderwidth 0
pack $w.frame.scal  -in $w.frame  -side top -fill x -pady 1.m
label $w.frame.scallabel  -text "  Auto scale:  "
checkbutton $w.frame.scalib  -textvariable curautoscale \
    -variable curautoscale  -onvalue "on" -offvalue "off" \
    -command "toggleScale" 
pack $w.frame.scallabel -in $w.frame.scal  -side left
pack $w.frame.scalib  -in $w.frame.scal    -side left  -fill x -pady 1.m -padx 1.m

#box
frame $w.frame.bxd -borderwidth 0
pack $w.frame.bxd  -in $w.frame  -side top -fill x -pady 1.m
label $w.frame.boxlabel  -text "        Boxed:  "
checkbutton $w.frame.box  -textvariable boxToggle  \
    -variable boxToggle  -onvalue "on" -offvalue "off" \
    -command "toggleBox" 
pack $w.frame.boxlabel -in $w.frame.bxd  -side left
pack $w.frame.box  -in $w.frame.bxd    -side left -fill x -pady 1.m -padx 1.m

#isoview
frame $w.frame.iso -borderwidth 0
pack $w.frame.iso  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.isolabel  -text "      Isoview: "
checkbutton $w.frame.isob  -textvariable isoToggle  \
    -variable isoToggle  -onvalue "on" -offvalue "off" \
    -command "toggleIsoview" 
pack $w.frame.isolabel -in $w.frame.iso -side left
pack $w.frame.isob  -in $w.frame.iso  -side left -fill x -pady 1.m -padx 1.m

#Tight Limits
frame $w.frame.lim  -borderwidth 0
pack $w.frame.lim  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.limitlabel -text " Tight limits: "
checkbutton $w.frame.limit  -textvariable limToggle  \
    -variable limToggle  -onvalue "on" -offvalue "off" \
    -command "toggleLimits" 
pack $w.frame.limitlabel -in $w.frame.lim -side left
pack $w.frame.limit  -in $w.frame.lim  -side left  -fill x -pady 1.m -padx 1.m


#cubescaling
frame $w.frame.cub -borderwidth 0
pack $w.frame.cub  -in $w.frame -side top -fill x -pady 1.m
label $w.frame.cublabel  -text "Cube scaling:"
checkbutton $w.frame.cubb  -textvariable cubToggle  \
    -variable cubToggle  -onvalue "on" -offvalue "off" \
    -command "toggleCubview" 
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
bind  $w.frame.datay <Return> {SelectClipBox}

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
bind  $w.frame.datah <Return> {SelectClipBox}

#margins
frame $w.frame.marg1 -borderwidth 0
pack $w.frame.marg1  -in $w.frame -side top   -fill x
label $w.frame.labelmarg -text "  Margins:"
pack $w.frame.labelmarg -in  $w.frame.marg1 -side left

frame $w.frame.marg2 -borderwidth 0
pack $w.frame.marg2  -in $w.frame -side top   -fill x

frame $w.frame.marg21 -borderwidth 0
pack $w.frame.marg21  -in $w.frame -side top   -fill x

label $w.frame.labelleft -text  "          Left: "
entry $w.frame.datamargl -relief sunken  -textvariable Lmargins
label $w.frame.labelright -text "         Right:"
entry $w.frame.datamargr -relief sunken  -textvariable Rmargins

pack $w.frame.labelleft  $w.frame.datamargl  -in  $w.frame.marg2 -side left  -fill x -pady 1.m -padx 1.m
pack $w.frame.labelright $w.frame.datamargr  -in  $w.frame.marg21 -side left -fill x -pady 1.m -padx 1.m 
bind  $w.frame.datamargl <Return> {SelectMargins}
bind  $w.frame.datamargr <Return> {SelectMargins}

frame $w.frame.marg4 -borderwidth 0
pack $w.frame.marg4  -in $w.frame -side top   -fill x

frame $w.frame.marg41 -borderwidth 0
pack $w.frame.marg41  -in $w.frame -side top   -fill x

label $w.frame.labeltop -text    "          Top: "
entry $w.frame.datamargt -relief sunken  -textvariable Tmargins
label $w.frame.labelbottom -text "     Bottom :"
entry $w.frame.datamargb -relief sunken  -textvariable Bmargins

pack $w.frame.labeltop     $w.frame.datamargt -in  $w.frame.marg4  -side left -fill x -pady 1.m -padx 1.m
pack $w.frame.labelbottom  $w.frame.datamargb -in  $w.frame.marg41 -side left -fill x -pady 1.m -padx 1.m
bind  $w.frame.datamargt <Return> {SelectMargins}
bind  $w.frame.datamargb <Return> {SelectMargins}


#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom


########### Viewpoint onglet ######################################
###################################################################
set w [Notebook:frame .axes.n Viewpoint]

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





#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom



# les proc associes:
proc SelectMargins {} {
global Lmargins Rmargins Tmargins Bmargins
ScilabEval "global ged_handle;ged_handle.margins=\[$Lmargins $Rmargins $Tmargins $Bmargins \]"
}
proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
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



proc setAlphaAngle {} {
    global curalpharotation curthetarotation viewToggle
    ScilabEval "global ged_handle;ged_handle.rotation_angles=\[$curalpharotation $curthetarotation\]"
   if { ( $curalpharotation == 0) && ( $curthetarotation == 270) } {
	set viewToggle "2d"
    } else {
	set viewToggle "3d"
    }
}

proc setThetaAngle {} {
global curalpharotation curthetarotation viewToggle
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
ScilabEval "setXdb($dbxmin, $dbxmax);"
}

proc setYdb {} {
global dbymin dbymax
ScilabEval "setYdb($dbymin, $dbymax);"
}


proc setZdb {} {
global dbzmin dbzmax
ScilabEval "setZdb($dbzmin, $dbzmax);"
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


proc toggleVisibility {} {
global visToggle
ScilabEval "global ged_handle;ged_handle.axes_visible='$visToggle'"
}


proc toggleVis {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
}

proc toggleIsoview {} {
global isoToggle
ScilabEval "global ged_handle;ged_handle.isoview='$isoToggle'"
}

proc toggleCubview {} {
global cubToggle
ScilabEval "global ged_handle;ged_handle.cube_scaling='$cubToggle'"
}

proc toggleBox {} {
global boxToggle
ScilabEval "global ged_handle;ged_handle.box='$boxToggle'"
}
proc toggleX {} {
global xToggle
ScilabEval "LogtoggleX('$xToggle')"
}
proc toggleY {} {
global yToggle
ScilabEval "LogtoggleY('$yToggle')"
}
proc toggleLimits {} {
global limToggle
ScilabEval "global ged_handle;ged_handle.tight_limits='$limToggle'"
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
ScilabEval "global ged_handle;ged_handle.x_label.text='$xlabel'"
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
ScilabEval "global ged_handle;ged_handle.y_label.text='$ylabel'"
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
ScilabEval "global ged_handle;ged_handle.z_label.text='$zlabel'"
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
ScilabEval "global ged_handle;ged_handle.title.text='$tlabel'"
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
ScilabEval "global ged_handle;ged_handle.x_label.text='$xlabel';ged_handle.y_label.text='$ylabel'"
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



proc toggleClear {} {
global curautoclear
ScilabEval "global ged_handle;ged_handle.auto_clear='$curautoclear'"
}


proc toggleScale {} {
global curautoscale
ScilabEval "global ged_handle;ged_handle.auto_scale='$curautoscale'"
}


# proc toggleFill {} {
# global curfillmode
# ScilabEval "global ged_handle;ged_handle.fill_mode='$curfillmode'"
# }
