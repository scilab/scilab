
set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]

global  visToggle limToggle boxToggle isoToggle gridToggle 
global  xToggle yToggle red green blue color 
global  xlabel ylabel zlabel tlabel
global  curvis cubToggle viewToggle

global xlabel_foreground ylabel_foreground zlabel_foreground titlelabel_foreground
global xlabel_fontsize ylabel_fontsize zlabel_fontsize  titlelabel_fontsize

global ncolors fcolor curthick curvis curfontsize curfontcolor

#axes_visible
global visToggle

global xGrif yGrid zGrid

global viewToggle isoToggle cubToggle
global dbxmin dbxmax dbymin dbymax dbzmin dbzmax

global msdos

# add for XF init only : to remove after...


#    set ncolors 32
#    set curcolor 4
#    set curfontsize 1
#    set curfontcolor 7
#    set curthick 1

#    set xToggle "n"
#    set yToggle "n"

#    set xlabel_foreground 1
#    set ylabel_foreground 1
#    set zlabel_foreground 1
#    set titlelabel_foreground 1

#    set xlabel_fontsize 1
#    set ylabel_fontsize 1
#    set zlabel_fontsize 1
#    set titlelabel_fontsize 1


#    set xGrid -1
#    set yGrid -1
#    set zGrid -1

#    set boxToggle "on"

#    set fcolor -1
#    set bcolor -2

#    set viewToggle "2D"
#    set isoToggle "off"
#    set limToggle "off"
#    set cubToggle "off"
#    set visToggle "on"
#    set curvis "on"


set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Axes Editor"
wm iconname $ww "AE"
wm geometry $ww 450x580
#wm maxsize  $ww 450 560

Notebook:create .axes.n -pages {X Y Z Title Style Axes_Aspect} -pad 20 
pack .axes.n -fill both -expand 1


########### X onglet ##############################################
###################################################################
set w [Notebook:frame .axes.n X]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#x label
frame $w.frame.lbx -borderwidth 0
pack $w.frame.lbx  -in $w.frame -side top   -fill x

label $w.frame.xlabel -text "     Label: "
entry $w.frame.xlabel1 -relief sunken  -textvariable xlabel 
pack $w.frame.xlabel -in  $w.frame.lbx -side left
pack $w.frame.xlabel1  -in  $w.frame.lbx -fill x
bind  $w.frame.xlabel1 <Return> {setXlabel} 

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setXFontLabelColor $w.frame.fontcolor" -tickinterval 0 
frame $w.frame.samplelabel -height 1.2c -width 1c

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor  $w.frame.samplelabel -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $xlabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x

label $w.frame.fontsizlabel -height 0 -text "      Size: " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setXFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz  -fill x
$w.frame.fontsize set $xlabel_fontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x

label $w.frame.stylelabel  -height 0 -text "Font Style: " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"
$w.frame.style activate 0
bind  $w.frame.style  <<ListboxSelect>> {
	    SelectXFontStyle [ selection get ]
    }
pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -fill x


#Xpos
frame $w.frame.px  -borderwidth 0
pack $w.frame.px  -in $w.frame -side top   -fill x

label $w.frame.xposlabel  -height 0 -text "  Location: " -width 0 
listbox $w.frame.xpos  -height 0  -width 0
$w.frame.xpos insert 0 top middle bottom
$w.frame.xpos activate 0 
bind  $w.frame.xpos  <<ListboxSelect>> {
	    SelectXpos [ selection get ]
    }
pack $w.frame.xposlabel -in  $w.frame.px -side left
pack $w.frame.xpos  -in  $w.frame.px -fill x

#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x

label $w.frame.gridcolorlabel -height 0 -text "Grid Color: " -width 0 
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setXGridColor $w.frame.gridcolor" -tickinterval 0 
frame $w.frame.sample -height 1.2c -width 1c

pack $w.frame.gridcolorlabel -in  $w.frame.gridcol -side left 
pack  $w.frame.gridcolor  $w.frame.sample  -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $xGrid

#X Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x

label $w.frame.datalabel -text "Data bounds:"
entry $w.frame.databmin -relief sunken  -textvariable dbxmin 
entry $w.frame.databmax -relief sunken  -textvariable dbxmax
pack $w.frame.datalabel -in  $w.frame.datab -side left
pack $w.frame.databmin -in  $w.frame.datab   -side left -expand 1 -fill x 
pack $w.frame.databmax -in  $w.frame.datab   -side left -expand 1 -fill x 
bind  $w.frame.databmin <Return> {setXdb} 
bind  $w.frame.databmax <Return> {setXdb} 

#Scale log or linear
frame $w.frame.scalesw  -borderwidth 0
pack $w.frame.scalesw  -in $w.frame -side top   -fill x

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
pack $w.frame.lby  -in $w.frame -side top   -fill x

label $w.frame.ylabel -text "     Label: "
entry $w.frame.ylabel1 -relief sunken  -textvariable ylabel 
pack $w.frame.ylabel -in  $w.frame.lby -side left
pack $w.frame.ylabel1  -in  $w.frame.lby -fill x
bind  $w.frame.ylabel1 <Return> {setYlabel} 

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setYFontLabelColor $w.frame.fontcolor" -tickinterval 0 
frame $w.frame.samplelabel -height 1.2c -width 1c

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor  $w.frame.samplelabel -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $ylabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x

label $w.frame.fontsizlabel -height 0 -text "      Size: " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setYFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz  -fill x
$w.frame.fontsize set $ylabel_fontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x

label $w.frame.stylelabel  -height 0 -text "Font Style: " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"
$w.frame.style activate 0
bind  $w.frame.style  <<ListboxSelect>> {
	    SelectYFontStyle [ selection get ]
    }
pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -fill x

#Ypos
frame $w.frame.py  -borderwidth 0
pack $w.frame.py  -in $w.frame -side top   -fill x

label $w.frame.yposlabel  -height 0 -text "  Location: " -width 0 
listbox $w.frame.ypos  -height 0  -width 0
$w.frame.ypos insert 0 left middle right
$w.frame.ypos activate 0 
bind  $w.frame.ypos  <<ListboxSelect>> {
	    SelectYpos [ selection get ]
    }
pack $w.frame.yposlabel -in  $w.frame.py -side left
pack $w.frame.ypos  -in  $w.frame.py -fill x

#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x

label $w.frame.gridcolorlabel -height 0 -text "Grid Color: " -width 0 
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setYGridColor $w.frame.gridcolor" -tickinterval 0 
frame $w.frame.sample -height 1.2c -width 1c

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack $w.frame.gridcolor  $w.frame.sample  -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $yGrid

#Y Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x

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
pack $w.frame.scalesw  -in $w.frame -side top   -fill x

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
pack $w.frame.lbz  -in $w.frame -side top   -fill x

label $w.frame.zlabel -text "     Label: "
entry $w.frame.zlabel1 -relief sunken  -textvariable zlabel 
pack $w.frame.zlabel -in  $w.frame.lbz -side left
pack $w.frame.zlabel1  -in  $w.frame.lbz -fill x
bind  $w.frame.zlabel1 <Return> {setZlabel} 

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setZFontLabelColor $w.frame.fontcolor" -tickinterval 0 
frame $w.frame.samplelabel -height 1.2c -width 1c

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor  $w.frame.samplelabel -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $zlabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x

label $w.frame.fontsizlabel -height 0 -text "      Size: " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setZFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz  -fill x
$w.frame.fontsize set $zlabel_fontsize


#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x

label $w.frame.stylelabel  -height 0 -text "Font Style: " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"
$w.frame.style activate 0
bind  $w.frame.style  <<ListboxSelect>> {
       SelectZFontStyle [ selection get ]
    }
pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -fill x

#Zpos
#frame $w.frame.pz  -borderwidth 0
#pack $w.frame.pz  -in $w.frame -side top   -fill x
#
#label $w.frame.zposlabel  -height 0 -text "  Location: " -width 0 
#listbox $w.frame.zpos  -height 0  -width 0
#$w.frame.zpos insert 0 top middle bottom
#$w.frame.zpos activate 0 
#bind  $w.frame.zpos  <<ListboxSelect>> {
#	    SelectZpos [ selection get ]
#    }
#pack $w.frame.zposlabel -in  $w.frame.pz -side left
#pack $w.frame.zpos  -in  $w.frame.pz -fill x

#Grid
frame $w.frame.gridcol  -borderwidth 0
pack $w.frame.gridcol  -in $w.frame -side top   -fill x

label $w.frame.gridcolorlabel -height 0 -text "Grid Color: " -width 0 
scale $w.frame.gridcolor -orient horizontal -from -1 -to $ncolors \
	 -resolution 1.0 -command "setZGridColor $w.frame.gridcolor" -tickinterval 0 
frame $w.frame.sample -height 1.2c -width 1c

pack $w.frame.gridcolorlabel  -in  $w.frame.gridcol -side left 
pack  $w.frame.gridcolor  $w.frame.sample  -in  $w.frame.gridcol   -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.gridcolor set $zGrid

#Z Data bounds
frame $w.frame.datab  -borderwidth 0
pack $w.frame.datab  -in $w.frame -side top   -fill x

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
pack $w.frame.lbtitle  -in $w.frame -side top   -fill x

label $w.frame.titlelabel -text "     Label: "
entry $w.frame.titlelabel1 -relief sunken  -textvariable tlabel 
pack $w.frame.titlelabel -in  $w.frame.lbtitle -side left
pack $w.frame.titlelabel1  -in  $w.frame.lbtitle -fill x
bind  $w.frame.titlelabel1 <Return> {setTitleLabel} 

#Font color
frame $w.frame.fontcol  -borderwidth 0
pack $w.frame.fontcol  -in $w.frame -side top   -fill x

label $w.frame.fontcolorlabel -height 0 -text "     Color: " -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setTitleFontLabelColor $w.frame.fontcolor" -tickinterval 0 
frame $w.frame.samplelabel -height 1.2c -width 1c

pack $w.frame.fontcolorlabel  -in  $w.frame.fontcol -side left 
pack $w.frame.fontcolor  $w.frame.samplelabel -in  $w.frame.fontcol -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $titlelabel_foreground


#Font size
frame $w.frame.fontsiz  -borderwidth 0
pack $w.frame.fontsiz  -in $w.frame -side top   -fill x

label $w.frame.fontsizlabel -height 0 -text "      Size: " -width 0 
scale $w.frame.fontsize -orient horizontal -from 0 -to 6 \
	 -resolution 1.0 -command "setTitleFontLabelSize $w.frame.fontsize" -tickinterval 0 

pack $w.frame.fontsizlabel  -in  $w.frame.fontsiz -side left 
pack $w.frame.fontsize  -in  $w.frame.fontsiz  -fill x
$w.frame.fontsize set $titlelabel_fontsize

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame -side top -fill x

label $w.frame.stylelabel  -height 0 -text "Font Style: " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"
$w.frame.style activate 0
bind  $w.frame.style  <<ListboxSelect>> {
    SelectTitleFontStyle [ selection get ]
    }
pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst  -fill x


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
pack $w.frame.vis  -in $w.frame  -side top -fill x
label $w.frame.vislabel  -text "Visibility:  "
checkbutton $w.frame.visib  -textvariable curvis  \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis" 
pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Font Style: " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"
$w.frame.style activate 0
bind  $w.frame.style  <<ListboxSelect>> {
    SelectFontStyle [ selection get ]
    }
pack $w.frame.stylelabel -in $w.frame.fontsst   -side left
pack $w.frame.style -in $w.frame.fontsst   -fill x


#FontColor scale
frame $w.frame.fontclr  -borderwidth 0
pack $w.frame.fontclr   -in $w.frame -side top  -fill x
label $w.frame.fontcolorlabel -height 0 -text "Font Color" -width 0 
scale $w.frame.fontcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0 
frame $w.frame.samplefont -height 1.2c -width 1c

pack $w.frame.fontcolorlabel -in $w.frame.fontclr -side left
pack $w.frame.fontcolor  $w.frame.samplefont -in  $w.frame.fontclr -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fontcolor set $curfontcolor

#Fontsize scale
frame $w.frame.fontssz  -borderwidth 0
pack $w.frame.fontssz  -in $w.frame    -side top -fill x

label $w.frame.fontsizelabel -height 0 -text "Font size" -width 0 
scale $w.frame.fontsize -orient horizontal  -from 0 -to 6 \
	 -resolution 1.0 -command "setFontSize $w.frame.fontsize" -tickinterval 0
pack $w.frame.fontsizelabel  -in $w.frame.fontssz -side left
pack $w.frame.fontsize -in $w.frame.fontssz  -fill x
$w.frame.fontsize set $curfontsize


#Color scale foregound
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x
label $w.frame.fcolorlabel -height 0 -text "Foreground\n   Color: " -width 0 

scale $w.frame.fcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setForeColor $w.frame.fcolor" -tickinterval 0 
frame $w.frame.samplefore -height 1.2c -width 1c

pack $w.frame.fcolorlabel -in $w.frame.clrf -side left
pack $w.frame.fcolor  $w.frame.samplefore -in  $w.frame.clrf -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.fcolor set $fcolor


#Color scale background
frame $w.frame.clrb  -borderwidth 0
pack $w.frame.clrb  -in $w.frame -side top  -fill x
label $w.frame.bcolorlabel -height 0 -text "Background\n   Color: " -width 0 

scale $w.frame.bcolor -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setBackColor $w.frame.bcolor" -tickinterval 0 
frame $w.frame.sampleback -height 1.2c -width 1c

pack $w.frame.bcolorlabel -in $w.frame.clrb -side left
pack $w.frame.bcolor  $w.frame.sampleback -in  $w.frame.clrb -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.bcolor set $bcolor


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x

label $w.frame.scalelabel -height 0 -text Thickness -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 30 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack  $w.frame.thickness  -in $w.frame.thk -expand yes -fill x
$w.frame.thickness set $curthick

#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom

########### Axes_Aspect onglet ####################################
###################################################################
set w [Notebook:frame .axes.n Axes_Aspect]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both


#visible
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame -side top -fill x -pady 2m
label $w.frame.vislabel  -text "   Visible:        "
checkbutton $w.frame.visb  -textvariable visToggle  \
    -variable visToggle  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.vislabel -in $w.frame.vis -side left
pack $w.frame.visb  -in $w.frame.vis  -side left -fill x

#box
frame $w.frame.bxd -borderwidth 0
pack $w.frame.bxd  -in $w.frame  -side top -fill x
label $w.frame.boxlabel  -text "   Boxed:        "
checkbutton $w.frame.box  -textvariable boxToggle  \
    -variable boxToggle  -onvalue "on" -offvalue "off" \
    -command "toggleBox" 
pack $w.frame.boxlabel -in $w.frame.bxd  -side left
pack $w.frame.box  -in $w.frame.bxd    -side left -fill x

#view
frame $w.frame.view -borderwidth 0
pack $w.frame.view  -in $w.frame -side top -fill x -pady 2m
label $w.frame.viewlabel  -text "   View:          "
checkbutton $w.frame.viewb  -textvariable viewToggle  \
    -variable viewToggle  -onvalue "2d" -offvalue "3d" \
    -command "toggleView" 
pack $w.frame.viewlabel -in $w.frame.view -side left
pack $w.frame.viewb  -in $w.frame.view  -side left -fill x

#isoview
frame $w.frame.iso -borderwidth 0
pack $w.frame.iso  -in $w.frame -side top -fill x -pady 2m
label $w.frame.isolabel  -text "   Isoview:     "
checkbutton $w.frame.isob  -textvariable isoToggle  \
    -variable isoToggle  -onvalue "on" -offvalue "off" \
    -command "toggleIsoview" 
pack $w.frame.isolabel -in $w.frame.iso -side left
pack $w.frame.isob  -in $w.frame.iso  -side left -fill x

#Tight Limits
frame $w.frame.lim  -borderwidth 0
pack $w.frame.lim  -in $w.frame -side top -fill x -pady 2m
label $w.frame.limitlabel -text "   Tight limits: "
checkbutton $w.frame.limit  -textvariable limToggle  \
    -variable limToggle  -onvalue "on" -offvalue "off" \
    -command "toggleLimits" 
pack $w.frame.limitlabel -in $w.frame.lim -side left
pack $w.frame.limit  -in $w.frame.lim  -side left -fill x


#cubescaling
frame $w.frame.cub -borderwidth 0
pack $w.frame.cub  -in $w.frame -side top -fill x -pady 2m
label $w.frame.cublabel  -text "   Cubescaling:"
checkbutton $w.frame.cubb  -textvariable cubToggle  \
    -variable cubToggle  -onvalue "on" -offvalue "off" \
    -command "toggleCubview" 
pack $w.frame.cublabel -in $w.frame.cub -side left
pack $w.frame.cubb  -in $w.frame.cub  -side left -fill x



#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom




# les proc associes:
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
	.axes.n.f4.frame.samplefore config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f4.frame.samplefore config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f4.frame.samplefore config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f4.frame.samplefore config -background $color
	
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
	.axes.n.f4.frame.sampleback config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f4.frame.sampleback config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f4.frame.sampleback config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.background=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f4.frame.sampleback config -background $color
	
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
	.axes.n.f4.frame.samplefont config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f4.frame.samplefont config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f4.frame.samplefont config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.font_color=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f4.frame.samplefont config -background $color
	
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

proc toggleView {} {
global viewToggle
ScilabEval "global ged_handle;ged_handle.view='$viewToggle'"
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
	.axes.n.f0.frame.sample config -background $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.grid(1)=$index;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f0.frame.sample config -background $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; ged_handle.grid(1)=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f0.frame.sample config -background $color
	
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
	.axes.n.f0.frame.samplelabel config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f0.frame.samplelabel config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f0.frame.samplelabel config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.x_label.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f0.frame.samplelabel config -background $color
	
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
	.axes.n.f1.frame.sample config -background $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.grid(2)=$index;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f1.frame.sample config -background $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; ged_handle.grid(2)=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f1.frame.sample config -background $color
	
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
	.axes.n.f1.frame.samplelabel config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f1.frame.samplelabel config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f1.frame.samplelabel config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.y_label.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f1.frame.samplelabel config -background $color
	
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
	.axes.n.f2.frame.sample config -background $color
    } 
    
    if { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.grid(3)=$index;"
	#like $index==1: display black color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f2.frame.sample config -background $color
    }
    
    if { ( $index != -1) && ( $index != 0) } {
	ScilabEval "global ged_handle; ged_handle.grid(3)=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f2.frame.sample config -background $color
	
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
	.axes.n.f2.frame.samplelabel config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f2.frame.samplelabel config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f2.frame.samplelabel config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.z_label.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f2.frame.samplelabel config -background $color
	
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
	.axes.n.f3.frame.samplelabel config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f3.frame.samplelabel config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f3.frame.samplelabel config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.title.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f3.frame.samplelabel config -background $color
	
    }
}




proc setlabels {} {
global ylabel
global xlabel
ScilabEval "global ged_handle;ged_handle.x_label.text='$xlabel';ged_handle.y_label.text='$ylabel'"
}

proc SelectXpos {sel} {
ScilabEval "global ged_handle;ged_handle.x_location='$sel'"
}
proc SelectYpos {sel} {
ScilabEval "global ged_handle;ged_handle.y_location='$sel'"
}


proc SelectXFontStyle {sel} {
ScilabEval "setLabelsFontStyle('x','$sel')"
}


proc SelectYFontStyle {sel} {
ScilabEval "setLabelsFontStyle('y','$sel')"
}


proc SelectZFontStyle {sel} {
ScilabEval "setLabelsFontStyle('z','$sel')"
}

proc SelectTitleFontStyle {sel} {
ScilabEval "setLabelsFontStyle('t','$sel')"
}


proc SelectFontStyle {sel} {
ScilabEval "setFontStyle('$sel')"
}

