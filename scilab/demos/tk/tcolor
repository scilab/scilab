
set w .foo
catch {destroy .foo}

toplevel .foo
wm title .foo "Color Editor"

# Global variables that control the program:
#
# colorSpace -			Color space currently being used for
#				editing.  Must be "rgb", "cmy", or "hsb".
# label1, label2, label3 -	Labels for the scales.
# red, green, blue -		Current color intensities in decimal
#				on a scale of 0-65535.
# color -			A string giving the current color value
#				in the proper form for x:
#				#RRRRGGGGBBBB
# updating -			Non-zero means that we're in the middle of
#				updating the scales to load a new color,so
#				information shouldn't be propagating back
#				from the scales to other elements of the
#				program:  this would make an infinite loop.
# command -			Holds the command that has been typed
#				into the "Command" entry.
# autoUpdate -			1 means execute the update command
#				automatically whenever the color changes.
# name -			Name for new color, typed into entry.


unset -nocomplain label1 label2 label3
unset -nocomplain red green blue
unset -nocomplain colorSpace color updating autoUpdate name

set colorSpace hsb
set red 65535
set green 0
set blue 0
set color #ffff00000000
set updating 0
set autoUpdate 1
set name ""
 
# Create the menu bar at the top of the window.

frame .foo.menu -relief raised -borderwidth 2
pack .foo.menu -side top -fill x
menubutton .foo.menu.file -text File -menu .foo.menu.file.m -underline 0
menu .foo.menu.file.m
.foo.menu.file.m add radio -label "RGB color space" -variable colorSpace \
	-value rgb -underline 0 -command {changeColorSpace rgb}
.foo.menu.file.m add radio -label "CMY color space" -variable colorSpace \
	-value cmy -underline 0 -command {changeColorSpace cmy}
.foo.menu.file.m add radio -label "HSB color space" -variable colorSpace \
	-value hsb -underline 0 -command {changeColorSpace hsb}
.foo.menu.file.m add separator
.foo.menu.file.m add radio -label "Automatic updates" -variable autoUpdate \
	-value 1 -underline 0
.foo.menu.file.m add radio -label "Manual updates" -variable autoUpdate \
	-value 0 -underline 0
.foo.menu.file.m add separator
.foo.menu.file.m add command -label "Exit program" -underline 0 \
	-command "destroy .foo"
pack .foo.menu.file -side left

# Create the command entry window at the bottom of the window, along
# with the update button.

frame .foo.bot -relief raised -borderwidth 2
pack .foo.bot -side bottom -fill x
label .foo.commandLabel -text "Command:"
entry .foo.command -relief sunken -borderwidth 2 -textvariable command \
	-font {Courier 12}
button .foo.update -text Update -command doUpdate
pack .foo.commandLabel -in .foo.bot -side left
pack .foo.update -in .foo.bot -side right -pady .1c -padx .25c
pack .foo.command -in .foo.bot -expand yes -fill x -ipadx 0.25c

# Create the listbox that holds all of the color names in rgb.txt,
# if an rgb.txt file can be found.

frame .foo.middle -relief raised -borderwidth 2
pack .foo.middle -side top -fill both


    #set i $sci/demos/tk/rgb.txt
    if [file readable $rgb] {
    set f [open $rgb]
    frame .foo.middle.left
    pack .foo.middle.left -side left -padx .25c -pady .25c
    listbox .foo.names -width 20 -height 12 -yscrollcommand ".foo.scroll set" \
	    -relief sunken -borderwidth 2 -exportselection false
    bind .foo.names <Double-1> {
	    tc_loadNamedColor [.foo.names get [.foo.names curselection]]
    }
    scrollbar .foo.scroll -orient vertical -command ".foo.names yview" \
	    -relief sunken -borderwidth 2
    pack .foo.names -in .foo.middle.left -side left
    pack .foo.scroll -in .foo.middle.left -side right -fill y
    while {[gets $f line] >= 0} {
	if {[llength $line] == 4} {
	    .foo.names insert end [lindex $line 3]
	}
    }
    close $f
    }
# Create the three scales for editing the color, and the entry for
# typing in a color value.

frame .foo.middle.middle
pack .foo.middle.middle -side left -expand yes -fill y
frame .foo.middle.middle.1
frame .foo.middle.middle.2
frame .foo.middle.middle.3
frame .foo.middle.middle.4
pack .foo.middle.middle.1 .foo.middle.middle.2 .foo.middle.middle.3 -side top -expand yes
pack .foo.middle.middle.4 -side top -expand yes -fill x
foreach i {1 2 3} {
    label .foo.label$i -textvariable label$i
    scale .foo.scale$i -from 0 -to 1000 -length 6c -orient horizontal \
	    -command tc_scaleChanged
    pack .foo.scale$i .foo.label$i -in .foo.middle.middle.$i -side top -anchor w
}
label .foo.nameLabel -text "Name:"
entry .foo.name -relief sunken -borderwidth 2 -textvariable name -width 10 \
	-font {Courier 12}
pack .foo.nameLabel -in .foo.middle.middle.4 -side left
pack .foo.name -in .foo.middle.middle.4 -side right -expand 1 -fill x
bind .foo.name <Return> {tc_loadNamedColor $name}

# Create the color display swatch on the right side of the window.

frame .foo.middle.right
pack .foo.middle.right -side left -pady .25c -padx .25c -anchor s
frame .foo.swatch -width 2c -height 5c -background $color
label .foo.value -textvariable color -width 13 -font {Courier 12}
pack .foo.swatch -in .foo.middle.right -side top -expand yes -fill both
pack .foo.value -in .foo.middle.right -side bottom -pady .25c

# The procedure below is invoked when one of the scales is adjusted.
# It propagates color information from the current scale readings
# to everywhere else that it is used.

proc tc_scaleChanged args {
    global red green blue colorSpace color updating autoUpdate
    if $updating {
	return
    }
    if {$colorSpace == "rgb"} {
	set red   [format %.0f [expr [.foo.scale1 get]*65.535]]
	set green [format %.0f [expr [.foo.scale2 get]*65.535]]
	set blue  [format %.0f [expr [.foo.scale3 get]*65.535]]
    } else {
	if {$colorSpace == "cmy"} {
	    set red   [format %.0f [expr {65535 - [.foo.scale1 get]*65.535}]]
	    set green [format %.0f [expr {65535 - [.foo.scale2 get]*65.535}]]
	    set blue  [format %.0f [expr {65535 - [.foo.scale3 get]*65.535}]]
	} else {
	    set list [hsbToRgb [expr {[.foo.scale1 get]/1000.0}] \
		    [expr {[.foo.scale2 get]/1000.0}] \
		    [expr {[.foo.scale3 get]/1000.0}]]
	    set red [lindex $list 0]
	    set green [lindex $list 1]
	    set blue [lindex $list 2]
	}
    }
    set color [format "#%04x%04x%04x" $red $green $blue]
    .foo.swatch config -bg $color
    if $autoUpdate doUpdate
    update idletasks
}

# The procedure below is invoked to update the scales from the
# current red, green, and blue intensities.  It's invoked after
# a change in the color space and after a named color value has
# been loaded.

proc tc_setScales {} {
    global red green blue colorSpace updating
    set updating 1
    if {$colorSpace == "rgb"} {
	.foo.scale1 set [format %.0f [expr $red/65.535]]
	.foo.scale2 set [format %.0f [expr $green/65.535]]
	.foo.scale3 set [format %.0f [expr $blue/65.535]]
    } else {
	if {$colorSpace == "cmy"} {
	    .foo.scale1 set [format %.0f [expr (65535-$red)/65.535]]
	    .foo.scale2 set [format %.0f [expr (65535-$green)/65.535]]
	    .foo.scale3 set [format %.0f [expr (65535-$blue)/65.535]]
	} else {
	    set list [rgbToHsv $red $green $blue]
	    .foo.scale1 set [format %.0f [expr {[lindex $list 0] * 1000.0}]]
	    .foo.scale2 set [format %.0f [expr {[lindex $list 1] * 1000.0}]]
	    .foo.scale3 set [format %.0f [expr {[lindex $list 2] * 1000.0}]]
	}
    }
    set updating 0
}

# The procedure below is invoked when a named color has been
# selected from the listbox or typed into the entry.  It loads
# the color into the editor.

proc tc_loadNamedColor name {
    global red green blue color autoUpdate

    if {[string index $name 0] != "#"} {
	set list [winfo rgb .foo.swatch $name]
	set red [lindex $list 0]
	set green [lindex $list 1]
	set blue [lindex $list 2]
    } else {
	case [string length $name] {
	    4 {set format "#%1x%1x%1x"; set shift 12}
	    7 {set format "#%2x%2x%2x"; set shift 8}
	    10 {set format "#%3x%3x%3x"; set shift 4}
	    13 {set format "#%4x%4x%4x"; set shift 0}
	    default {error "syntax error in color name \"$name\""}
	}
	if {[scan $name $format red green blue] != 3} {
	    error "syntax error in color name \"$name\""
	}
	set red [expr $red<<$shift]
	set green [expr $green<<$shift]
	set blue [expr $blue<<$shift]
    }
    tc_setScales
    set color [format "#%04x%04x%04x" $red $green $blue]
    .foo.swatch config -bg $color
    if $autoUpdate doUpdate
}

# The procedure below is invoked when a new color space is selected.
# It changes the labels on the scales and re-loads the scales with
# the appropriate values for the current color in the new color space

proc changeColorSpace space {
    global label1 label2 label3
    if {$space == "rgb"} {
	set label1 Red
	set label2 Green
	set label3 Blue
	tc_setScales
	return
    }
    if {$space == "cmy"} {
	set label1 Cyan
	set label2 Magenta
	set label3 Yellow
	tc_setScales
	return
    }
    if {$space == "hsb"} {
	set label1 Hue
	set label2 Saturation
	set label3 Brightness
	tc_setScales
	return
    }
}

# The procedure below converts an RGB value to HSB.  It takes red, green,
# and blue components (0-65535) as arguments, and returns a list containing
# HSB components (floating-point, 0-1) as result.  The code here is a copy
# of the code on page 615 of "Fundamentals of Interactive Computer Graphics"
# by Foley and Van Dam.

proc rgbToHsv {red green blue} {
    if {$red > $green} {
	set max $red.0
	set min $green.0
    } else {
	set max $green.0
	set min $red.0
    }
    if {$blue > $max} {
	set max $blue.0
    } else {
	if {$blue < $min} {
	    set min $blue.0
	}
    }
    set range [expr $max-$min]
    if {$max == 0} {
	set sat 0
    } else {
	set sat [expr {($max-$min)/$max}]
    }
    if {$sat == 0} {
	set hue 0
    } else {
	set rc [expr {($max - $red)/$range}]
	set gc [expr {($max - $green)/$range}]
	set bc [expr {($max - $blue)/$range}]
	if {$red == $max} {
	    set hue [expr {.166667*($bc - $gc)}]
	} else {
	    if {$green == $max} {
		set hue [expr {.166667*(2 + $rc - $bc)}]
	    } else {
		set hue [expr {.166667*(4 + $gc - $rc)}]
	    }
	}
	if {$hue < 0.0} {
	    set hue [expr $hue + 1.0]
	}
    }
    return [list $hue $sat [expr {$max/65535}]]
}

# The procedure below converts an HSB value to RGB.  It takes hue, saturation,
# and value components (floating-point, 0-1.0) as arguments, and returns a
# list containing RGB components (integers, 0-65535) as result.  The code
# here is a copy of the code on page 616 of "Fundamentals of Interactive
# Computer Graphics" by Foley and Van Dam.

proc hsbToRgb {hue sat value} {
    set v [format %.0f [expr 65535.0*$value]]
    if {$sat == 0} {
	return "$v $v $v"
    } else {
	set hue [expr $hue*6.0]
	if {$hue >= 6.0} {
	    set hue 0.0
	}
	scan $hue. %d i
	set f [expr $hue-$i]
	set p [format %.0f [expr {65535.0*$value*(1 - $sat)}]]
	set q [format %.0f [expr {65535.0*$value*(1 - ($sat*$f))}]]
	set t [format %.0f [expr {65535.0*$value*(1 - ($sat*(1 - $f)))}]]
	case $i \
	    0 {return "$v $t $p"} \
	    1 {return "$q $v $p"} \
	    2 {return "$p $v $t"} \
	    3 {return "$p $q $v"} \
	    4 {return "$t $p $v"} \
	    5 {return "$v $p $q"}
	error "i value $i is out of range"
    }
}

# The procedure below is invoked when the "Update" button is pressed,
# and whenever the color changes if update mode is enabled.  It
# propagates color information as determined by the command in the
# Command entry.

proc doUpdate {} {
    global color command  red green blue
    set newCmd $command
    regsub -all %% $command $color newCmd
    eval $newCmd

    ScilabEval "execstr('RGB=\[$red,$green,$blue\]/65535','errcatch');"
}

changeColorSpace hsb
