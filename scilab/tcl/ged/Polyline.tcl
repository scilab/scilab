set w .axes
catch {destroy $w}
toplevel $w
wm title $w "Polyline Editor"
wm iconname $w "PE"
global  visToggle red green blue color
set visToggle on

 
# add for XF init only : to remove after...
#set ncolors 32
#set curcolor 4

frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $w"
pack $w.buttons.dismiss  -side bottom -expand 1

frame $w.frame -borderwidth 10
pack $w.frame

#Visibility
label $w.frame.visiblelabel -height 0 -text Visibility -width 0 
checkbutton $w.frame.visible  -textvariable visToggle -indicatoron 1 \
    -variable visToggle  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel $w.frame.visible  -side top -anchor w

#Style
label $w.frame.stylelabel  -height 0 -text Style -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 interpolated staircase barplot arrowed filled
$w.frame.style activate 0

bind  $w.frame.style  <<ListboxSelect>> {
	    SelectStyle [ $w.frame.style get [ $w.frame.style curselection]]
    }
pack $w.frame.stylelabel $w.frame.style  -side top -anchor w

#Color scale
label $w.frame.colorlabel -height 0 -text Color -width 0 
#         -foreground $color
scale $w.frame.color -orient horizontal -length 284 -from 1 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 
pack $w.frame.colorlabel $w.frame.color  -side top -anchor w
$w.frame.color set $curcolor 

#Thickness scale
label $w.frame.scalelabel -height 0 -text Thickness -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel $w.frame.thickness  -side top -anchor w
$w.frame.thickness set $curthick



proc setColor {w colorindex} {    
global red green blue color
#     ScilabEval "tkgetrgb($colorindex)"
#     set color [format "#%04x%04x%04x" $red $green $blue]
#     ScilabEval "disp('OK')"

ScilabEval "global h;h.foreground=$colorindex;"
}

proc setThickness {w thick} {
ScilabEval "global h;h.thickness=$thick;"

#ScilabEval "disp($thick)"
}

proc toggleVisibility {} {
global visToggle
ScilabEval "global h;h.visible='$visToggle'"
#ScilabEval "disp('$visToggle')"
}

proc SelectStyle {sel} {
ScilabEval "setStyle('$sel')"
}
