set w .axes
catch {destroy $w}
toplevel $w
wm title $w "Rectangle Editor"
wm iconname $w "RE"
wm geometry $w 400x340
global  curvis red green blue color filToggle
#set visToggle on


frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $w"
pack $w.buttons.dismiss  -side bottom -expand 1

frame $w.frame -borderwidth 10
pack $w.frame  -anchor w -fill both



#Visibility
frame $w.frame.vis  -borderwidth 0
pack $w.frame.vis  -in $w.frame -side top -fill x

label $w.frame.visiblelabel -height 0 -text "Visibility:  " -width 0 
checkbutton $w.frame.visible  -textvariable curvis -indicatoron 1 \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel   -in $w.frame.vis  -side left
pack $w.frame.visible  -in $w.frame.vis  -side left -fill x


#Line Style
frame $w.frame.rectst  -borderwidth 0
pack $w.frame.rectst  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Style:       " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"
$w.frame.style activate 0

bind  $w.frame.style  <<ListboxSelect>> {
	    SelectStyle [ $w.frame.style get [ $w.frame.style curselection]]
    }
pack $w.frame.stylelabel -in $w.frame.rectst   -side left
pack $w.frame.style   -in $w.frame.rectst   -side left -fill x


#filled
frame $w.frame.fil  -borderwidth 0
pack $w.frame.fil  -in $w.frame  -side top  -fill x

label $w.frame.filledlabel -height 0 -text "Filled:      " -width 0 
checkbutton $w.frame.filled  -textvariable filToggle -indicatoron 1 \
    -variable filToggle  -onvalue "on" -offvalue "off" \
    -command "toggleFilled" 
pack $w.frame.filledlabel  -in $w.frame.fil  -side left
pack $w.frame.filled  -in $w.frame.fil   -side left -fill x


#Color scale
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Color:      " -width 0 
#         -foreground $color
scale $w.frame.color -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setColor $w.frame.color" -tickinterval 0 
frame $w.frame.sample -height 1.2c -width 1c

pack $w.frame.colorlabel -in $w.frame.clrf -side left
pack $w.frame.color  $w.frame.sample -in  $w.frame.clrf -side left -expand 1 -fill x -pady 2m -padx 2m
$w.frame.color set $curcolor


#Thickness scale
frame $w.frame.thk  -borderwidth 0
pack $w.frame.thk  -side top -fill x
label $w.frame.scalelabel -height 0 -text "Thickness:" -width 0 
scale $w.frame.thickness -orient horizontal -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk -expand yes -fill x
$w.frame.thickness set $curthick


#Toogles
frame $w.frame.tgl  -borderwidth 1 -relief sunken
pack $w.frame.tgl  -side bottom  -fill x


proc setColor {w index} {    
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	.axes.frame.sample config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.frame.sample config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.frame.sample config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.frame.sample config -background $color
	
    }
}

proc setThickness {w thick} {
ScilabEval "global ged_handle;ged_handle.thickness=$thick;"
}

proc toggleVisibility {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
}
proc toggleFilled {} {
global filToggle
ScilabEval "global ged_handle;ged_handle.fill_mode='$filToggle'"
}

proc SelectStyle {sel} {
ScilabEval "setLineStyle('$sel')"
}
