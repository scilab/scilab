set w .poly
catch {destroy $w}
toplevel $w
wm title $w "Axes Editor"
wm iconname $w "AE"
global  visToggle limToggle boxToggle isoToggle gridToggle xtoggle ytoggle red green blue color xlabel ylabel zlabel tlabel


frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $w"
pack $w.buttons.dismiss  -side top -expand 1

frame $w.frame -borderwidth 1  -relief sunken
pack $w.frame

#Color scale
frame $w.frame.clr  -borderwidth 0
pack $w.frame.clr  -side bottom -fill x
label $w.frame.colorlabel -height 0 -text "Color      " -width 0 
#         -foreground $color

scale $w.frame.color -orient horizontal -length 284 -from 1 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 
pack $w.frame.colorlabel -in $w.frame.clr -side left
pack $w.frame.color  -in $w.frame.clr  -expand yes -fill x
$w.frame.color set $curcolor

#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side bottom -fill x

label $w.frame.scalelabel -height 0 -text Thickness -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 30 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack  $w.frame.thickness  -in $w.frame.thk -expand yes -fill x
$w.frame.thickness set $curthick

#Fonts

frame $w.frame.fonts  -borderwidth 1 -relief sunken
pack $w.frame.fonts  -side bottom  -expand yes -fill x -fill y

#FontColor scale
frame $w.frame.fontclr  -borderwidth 0
pack $w.frame.fontclr   -in $w.frame.fonts -side right
label $w.frame.fontcolorlabel -height 0 -text "Font Color" -width 0 
scale $w.frame.fontcolor -orient vertical -from 1 -to $ncolors \
	 -resolution 1.0 -command "setFontColor $w.frame.fontcolor" -tickinterval 0 
pack $w.frame.fontcolorlabel -in $w.frame.fontclr -side bottom
pack $w.frame.fontcolor  -in $w.frame.fontclr -expand yes -fill y
#$w.frame.fontcolor set $curfontcolor

#Fontsize scale
frame $w.frame.fontssz  -borderwidth 0
pack $w.frame.fontssz  -in $w.frame.fonts  -side right  

label $w.frame.fontsizelabel -height 0 -text "Font size" -width 0 
scale $w.frame.fontsize -orient vertical  -from 0 -to 6 \
	 -resolution 1.0 -command "setFontsize $w.frame.fontsize" -tickinterval 0
pack $w.frame.fontsizelabel  -in $w.frame.fontssz -side bottom 
pack $w.frame.fontsize -in $w.frame.fontssz  -expand yes  -fill y
$w.frame.fontsize set $curfontsize

#Fonts Style
frame $w.frame.fontsst  -borderwidth 0
pack $w.frame.fontsst  -in $w.frame.fonts -side right 

label $w.frame.stylelabel  -height 0 -text "Font Style" -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"
$w.frame.style activate 0
bind  $w.frame.style  <<ListboxSelect>> {
	    SelectFontStyle [ $w.frame.style get [ $w.frame.style curselection]]
    }
pack $w.frame.stylelabel -in $w.frame.fontsst   -side bottom 
pack $w.frame.style -in $w.frame.fontsst  -fill x

#Toggles
frame $w.frame.tgl  -borderwidth 1 -relief sunken
pack $w.frame.tgl  -side bottom  -fill x

#Visibility
frame $w.frame.vis  -borderwidth 0
pack $w.frame.vis  -in $w.frame.tgl -side left

label $w.frame.visiblelabel  -text Visibility 
checkbutton $w.frame.visible  -textvariable visToggle \
    -variable visToggle  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel  -in $w.frame.vis -side bottom 
pack $w.frame.visible  -in $w.frame.vis 

#Tight Limits
frame $w.frame.lim  -borderwidth 0
pack $w.frame.lim  -in $w.frame.tgl -side right 
label $w.frame.limitlabel -text "Thight limits"
checkbutton $w.frame.limit  -textvariable limToggle  \
    -variable limToggle  -onvalue "on" -offvalue "off" \
    -command "toggleLimits" 
pack $w.frame.limitlabel -in $w.frame.lim -side bottom 
pack $w.frame.limit  -in $w.frame.lim 

#box
frame $w.frame.bxd -borderwidth 0
pack $w.frame.bxd  -in $w.frame.tgl -side right  -expand yes 
label $w.frame.boxlabel  -text "Boxed"
checkbutton $w.frame.box  -textvariable boxToggle  \
    -variable boxToggle  -onvalue "on" -offvalue "off" \
    -command "toggleBox" 
pack $w.frame.boxlabel -in $w.frame.bxd -side bottom 
pack $w.frame.box  -in $w.frame.bxd  

#isoview
frame $w.frame.iso -borderwidth 0
pack $w.frame.iso  -in $w.frame.tgl -side right  -expand yes 
label $w.frame.isolabel  -text "Isoview"
checkbutton $w.frame.isob  -textvariable isoToggle  \
    -variable isoToggle  -onvalue "on" -offvalue "off" \
    -command "toggleIsoview" 
pack $w.frame.isolabel -in $w.frame.iso -side bottom 
pack $w.frame.isob  -in $w.frame.iso 

#grid
#frame $w.frame.grd -borderwidth 0
#pack $w.frame.grd  -in $w.frame.tgl -side right  -expand yes 
#label $w.frame.gridlabel -height 0 -text "grid" -width 0 
#checkbutton $w.frame.grid  -textvariable gridToggle -indicatoron 1 \
#    -variable gridToggle  -onvalue "on" -offvalue "off" \
#    -command "toggleGrid" 
#pack $w.frame.gridlabel -in $w.frame.grd -side bottom 
#pack $w.frame.grid  -in $w.frame.grd  -expand yes  -fill x

#Title and Axes labels
frame $w.frame.lb  -borderwidth 1 -relief sunken
pack $w.frame.lb  -side bottom  -fill x

#Axes labels
frame $w.frame.alb  -borderwidth 1 -relief sunken
pack $w.frame.alb  -in  $w.frame.lb -side bottom  -fill x

#x label
frame $w.frame.lbx -borderwidth 0
pack $w.frame.lbx  -in $w.frame.alb -side left   

label $w.frame.xlabel -text "x label"
entry $w.frame.xlabel1 -relief sunken  -textvariable xlabel 
pack $w.frame.xlabel -in  $w.frame.lbx -side left
pack $w.frame.xlabel1  -in  $w.frame.lbx   
bind  $w.frame.xlabel1 <Return> {setXlabel} 

#y label
frame $w.frame.lby  -borderwidth 0
pack $w.frame.lby  -in $w.frame.alb -side left  
label $w.frame.ylabel  -text "y label" 
entry $w.frame.ylabel1 -relief sunken  -textvariable ylabel

pack $w.frame.ylabel  -in  $w.frame.lby -side left 
pack $w.frame.ylabel1  -in  $w.frame.lby  
bind  $w.frame.ylabel1 <Return> {setYlabel}

#z label
frame $w.frame.lbz  -borderwidth 0
pack $w.frame.lbz  -in $w.frame.alb -side left  
label $w.frame.zlabel  -text "z label" 
entry $w.frame.zlabel1 -relief sunken  -textvariable zlabel

pack $w.frame.zlabel  -in  $w.frame.lbz -side left 
pack $w.frame.zlabel1  -in  $w.frame.lbz  
bind  $w.frame.zlabel1 <Return> {setZlabel}


#on peut remplacer les bind ci-dessus par un bouton:
#button  $w.frame.update -text Update -command setlabels
#pack $w.frame.update  -in $w.frame.lb -side right -expand yes  -fill x

#title
frame $w.frame.lbt  -borderwidth 0
pack $w.frame.lbt  -in $w.frame.lb -side bottom   -fill x
label $w.frame.tlabel -height 0 -text "title" -width 0 
entry $w.frame.tlabel1 -relief sunken  -textvariable tlabel

pack $w.frame.tlabel  -in  $w.frame.lbt -side left 
pack $w.frame.tlabel1  -in  $w.frame.lbt  -fill x
bind  $w.frame.tlabel1 <Return> {setTitle}



#Axes graduation modes
frame $w.frame.ll  -borderwidth 1 -relief sunken
pack $w.frame.ll  -side bottom  -fill x

#x lin/log
frame $w.frame.llx -borderwidth 0
pack $w.frame.llx  -in $w.frame.ll -side left 

label $w.frame.xllabel -height 0 -text "x Lin/log" -width 0 
checkbutton $w.frame.x  -textvariable xToggle -indicatoron 1 \
    -variable xToggle  -onvalue "n" -offvalue "l" \
    -command "toggleX" 
pack $w.frame.xllabel -in  $w.frame.llx -side left 
pack $w.frame.x  -in  $w.frame.llx 

#y lin/log
frame $w.frame.lly  -borderwidth 0
pack $w.frame.lly  -in $w.frame.ll -side left  -expand yes 
label $w.frame.yllabel -height 0 -text "y Lin/log" -width 0 
checkbutton $w.frame.y  -textvariable yToggle -indicatoron 1 \
    -variable yToggle  -onvalue "n" -offvalue "l" \
    -command "toggleY" 
pack $w.frame.yllabel  -in  $w.frame.lly -side left 
pack $w.frame.y  -in  $w.frame.lly 


#Axes Position
frame $w.frame.pos  -borderwidth 1  -relief sunken
pack $w.frame.pos  -side bottom  -fill x


#Xpos
frame $w.frame.px  -borderwidth 0
pack $w.frame.px  -in $w.frame.pos -side right  -expand yes  -fill x

label $w.frame.xposlabel  -height 0 -text "x axis location" -width 0 
listbox $w.frame.xpos  -height 0  -width 0
$w.frame.xpos insert 0 bottom middle top
$w.frame.xpos activate 0 
bind  $w.frame.xpos  <<ListboxSelect>> {
	    SelectXpos [ $w.frame.xpos get [ $w.frame.xpos curselection]]
    }
pack $w.frame.xposlabel -in  $w.frame.px -side bottom 
pack $w.frame.xpos  -in  $w.frame.px -expand yes  -fill x

#Ypos
frame $w.frame.py  -borderwidth 0
pack $w.frame.py  -in $w.frame.pos -side right  -expand yes  -fill x
label $w.frame.yposlabel  -height 0 -text "y axis location" -width 0 
listbox $w.frame.ypos  -height 0  -width 0
$w.frame.ypos insert 0 left middle right
$w.frame.ypos activate 0 
bind  $w.frame.ypos  <<ListboxSelect>> {
	    SelectYpos [ $w.frame.ypos get [ $w.frame.ypos curselection]]
    }
pack $w.frame.yposlabel -in  $w.frame.py -side bottom 
pack $w.frame.ypos   -in  $w.frame.py -expand yes  -fill x

proc setColor {w colorindex} {    
global red green blue color
ScilabEval "global h;h.foreground=$colorindex;"
}
proc setFontColor {w colorindex} {    
global red green blue color
ScilabEval "global h;h.font_color=$colorindex;"
}
proc setThickness {w thick} {
ScilabEval "global h;h.thickness=$thick;"
}
proc setFontsize {w fs} {
ScilabEval "global h;h.labels_font_size=$fs;"
}

proc toggleVisibility {} {
global visToggle
ScilabEval "global h;h.axes_visible='$visToggle'"
}
proc toggleIsoview {} {
global isoToggle
ScilabEval "global h;h.isoview='$isoToggle'"
}

proc toggleBox {} {
global boxToggle
ScilabEval "global h;h.box='$boxToggle'"
}
proc toggleX {} {
global xToggle
ScilabEval "global h;h.log_flags='$xToggle'+part(h.log_flags,2)"
}
proc toggleY {} {
global yToggle
ScilabEval "global h;h.log_flags=part(h.log_flags,1)+'$yToggle'"
}
proc toggleLimits {} {
global limToggle
ScilabEval "global h;h.tight_limits='$limToggle'"
}

proc setXlabel {} {
global xlabel
ScilabEval "disp('ici');global h;h.x_label.text='$xlabel'"
}
proc setYlabel {} {
global ylabel
ScilabEval "global h;h.y_label.text='$ylabel'"
}
proc setZlabel {} {
global zlabel
ScilabEval "global h;h.z_label.text='$zlabel'"
}
proc setTitle {} {
global tlabel
ScilabEval "global h;h.title.text='$tlabel'"
}

proc setlabels {} {
global ylabel
global xlabel
    ScilabEval "global h;h.x_label.text='$xlabel';h.y_label.text='$ylabel'"
}

proc SelectXpos {sel} {
ScilabEval "global h;h.x_location='$sel'"
}
proc SelectYpos {sel} {
ScilabEval "global h;h.y_location='$sel'"
}
proc SelectFontStyle {sel} {
ScilabEval "setFontStyle('$sel')"
}

