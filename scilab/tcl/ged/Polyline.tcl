
set sourcedir [file join "$env(SCIPATH)" "tcl" "utils"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]

global curvis curthick RED GREEN BLUE
global polyVAL nbcol nbrow


# set ncolors 32
# set nbcol 2
# set nbrow 6
# set curcolor 1
# set curthick 2

# set polyVAL(1,1) 1
# set polyVAL(1,2) 2
# set polyVAL(1,3) 3
# set polyVAL(1,4) 4
# set polyVAL(1,5) 5
# set polyVAL(1,6) 6


# set polyVAL(2,1) 1.3
# set polyVAL(2,2) 2.6
# set polyVAL(2,3) 3.5
# set polyVAL(2,4) 4.5
# set polyVAL(2,5) 5.8
# set polyVAL(2,6) 6.8



set ww .axes
catch {destroy $ww}
toplevel $ww
wm title $ww "Polyline Editor"
wm iconname $ww "PE"
wm geometry $ww 650x700



Notebook:create .axes.n -pages {Style Data} -pad 20 
pack .axes.n -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame .axes.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#Visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x

label $w.frame.visiblelabel -height 0 -text "Visibility:   " -width 0 
checkbutton $w.frame.visible  -textvariable curvis -indicatoron 1 \
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVisibility" 
pack $w.frame.visiblelabel  -in $w.frame.vis  -side left
pack $w.frame.visible  -in $w.frame.vis  -side left -fill x


#Style
frame $w.frame.curvst  -borderwidth 0
pack $w.frame.curvst  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Line style:    " -width 0 
listbox $w.frame.style  -height 0  -width 0
$w.frame.style insert 0 interpolated staircase barplot arrowed filled
$w.frame.style activate 0

bind  $w.frame.style  <<ListboxSelect>> {
	    SelectStyle [ selection get]
    }
pack $w.frame.stylelabel -in $w.frame.curvst   -side left
pack $w.frame.style   -in $w.frame.curvst   -fill x

#Color scale
frame $w.frame.clrf  -borderwidth 0
pack $w.frame.clrf  -in $w.frame -side top  -fill x

label $w.frame.colorlabel -height 0 -text "Color:       " -width 0 
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

label $w.frame.scalelabel -height 0 -text "Thickness: " -width 0 
scale $w.frame.thickness -orient horizontal -length 284 -from 1 -to 20 \
	 -resolution 1.0 -command "setThickness $w.frame.thickness" -tickinterval 0
pack $w.frame.scalelabel -in $w.frame.thk -side left 
pack $w.frame.thickness  -in $w.frame.thk -expand yes -fill x
$w.frame.thickness set $curthick


#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 

########### Data onglet ###########################################
###################################################################
set w [Notebook:frame .axes.n Data]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

frame $w.frame.fdata -borderwidth 0
pack $w.frame.fdata  -in $w.frame -side top   -fill x

scrollbar $w.frame.ysbar -orient vertical -command   {$w.frame.c yview}
canvas $w.frame.c -width 8i -height 6i  -yscrollcommand {$w.frame.ysbar set}

$w.frame.c create text 160 10 -anchor c -text "X"
$w.frame.c create text 310 10 -anchor c -text "Y"

if { $nbcol == 3 } {
    $w.frame.c create text 460 10 -anchor c -text "Z"
}

for {set i 1} {$i<=$nbrow} {incr i} {
  
    for {set j 1} {$j<=$nbcol} {incr j} {
	entry  $w.frame.c.data$i$j  -relief sunken  -textvariable polyVAL($i,$j)
	bind  $w.frame.c.data$i$j <Return> "setData $i $j"
	set aa [expr 10+($j*150)]
	set bb [expr 10+(25*$i)]
	$w.frame.c create window $aa $bb -anchor c -window $w.frame.c.data$i$j
    }
}

$w.frame.c configure -scrollregion [$w.frame.c bbox all] -yscrollincrement 0.1i

pack  $w.frame.ysbar -side right -fill y
pack  $w.frame.c


#exit button
frame $w.buttons
button $w.b -text Quit -command "destroy $ww"
pack $w.b -side bottom 


#les proc asssocies
proc setData { i j } {
global polyVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i,$j)=$polyVAL($i,$j);\",\'errcatch\',\'n\');"
}

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
	.axes.n.f0.frame.sample config -background $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	.axes.n.f0.frame.sample config -background $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	.axes.n.f0.frame.sample config -background $color
    } else { 
	ScilabEval "global ged_handle; ged_handle.foreground=$index;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x $REDCOL $GRECOL $BLUCOL]
	
	.axes.n.f0.frame.sample config -background $color
	
    }
}



proc setThickness {w thick} {
ScilabEval "global ged_handle;ged_handle.thickness=$thick;"

#ScilabEval "disp($thick)"
}

proc toggleVisibility {} {
global curvis
ScilabEval "global ged_handle;ged_handle.visible='$curvis'"
#ScilabEval "disp('$visToggle')"
}

proc SelectStyle {sel} {
ScilabEval "setStyle('$sel')"
}
