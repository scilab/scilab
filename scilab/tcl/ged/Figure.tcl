set w .figure
catch {destroy $w}
toplevel $w
wm title $w "Figure Editor"
wm iconname $w "FE"
wm geometry $w 400x300

frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m
button $w.buttons.dismiss -text Quit -command "destroy $w"
pack $w.buttons.dismiss  -side bottom -expand 1

frame $w.frame -borderwidth 10
pack $w.frame -anchor w -fill both

