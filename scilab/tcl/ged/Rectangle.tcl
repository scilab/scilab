set w .axes
catch {destroy $w}
toplevel $w
wm title $w "Rectangle Editor"
wm iconname $w "RE"
global  visToggle red green blue color filToggle
set visToggle on


frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $w"
pack $w.buttons.dismiss  -side bottom -expand 1

frame $w.frame -borderwidth 10
pack $w.frame

#Color scale
label $w.frame.colorlabel -height 0 -text Color -width 0 
#         -foreground $color
scale $w.frame.color -orient horizontal -length 284 -from 1 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 
pack $w.frame.colorlabel $w.frame.color  -side bottom -anchor w
$w.frame.color set $curcolor 

#Thickness scale
label $w.frame.scalelabel -height 0 -text Thickness -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel $w.frame.thickness  -side bottom -anchor w
$w.frame.thickness set $curthick

#Toogles
frame $w.frame.tgl  -borderwidth 1 -relief sunken
pack $w.frame.tgl  -side bottom  -fill x


#Visibility
frame $w.frame.vis  -borderwidth 0
pack $w.frame.vis  -in $w.frame.tgl -side left
label $w.frame.visiblelabel -height 0 -text Visibility -width 0 
checkbutton $w.frame.visible  -textvariable visToggle -indicatoron 1 \
    -variable visToggle  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel  -in $w.frame.vis -side bottom 
pack $w.frame.visible  -in $w.frame.vis 

#filled
frame $w.frame.fil  -borderwidth 0
pack $w.frame.fil  -in $w.frame.tgl -side left

label $w.frame.filledlabel -height 0 -text "Filled" -width 0 
checkbutton $w.frame.filled  -textvariable filToggle -indicatoron 1 \
    -variable filToggle  -onvalue "on" -offvalue "off" \
    -command "toggleFilled" 
pack $w.frame.filledlabel  -in $w.frame.fil -side bottom 
pack $w.frame.filled  -in $w.frame.fil

#Line Style
label $w.frame.stylelabel  -height 0 -text Style -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"
$w.frame.style activate 0

bind  $w.frame.style  <<ListboxSelect>> {
	    SelectStyle [ $w.frame.style get [ $w.frame.style curselection]]
    }
pack $w.frame.stylelabel $w.frame.style  -side bottom -anchor w


proc setColor {w colorindex} {    
global red green blue color
ScilabEval "global h;h.foreground=$colorindex;"
}

proc setThickness {w thick} {
ScilabEval "global h;h.thickness=$thick;"
}

proc toggleVisibility {} {
global visToggle
ScilabEval "global h;h.visible='$visToggle'"
}
proc toggleFilled {} {
global filToggle
ScilabEval "global h;h.fill_mode='$filToggle'"
}

proc SelectStyle {sel} {
ScilabEval "setLineStyle('$sel')"
}
