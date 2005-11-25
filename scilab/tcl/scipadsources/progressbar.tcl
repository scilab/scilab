# This file has been downloaded from Donal K. Fellows' Tcl archive at
#   http://www.man.ac.uk/~zzcgudf/tcl/mwidx.html#progress
#
# A few words about it's licence can be found at 
#   http://www.cs.man.ac.uk/~fellowsd/tcl/
#
# It has been edited to remove the demo code and tune a bit the widget

######################################################################

# This code is Copyright 1999 Donal K. Fellows

# You may use this code under license. The license is exactly the
# license used by the Tcl distribution, as described in the file
# "license.terms" distributed there. (If you need a specific version,
# the version distributed with Tcl 8.0.6 will do, but that's the same
# as the version distributed with Tcl 8.3.5 so you've got a wide range
# of versions to get the same license from.)

######################################################################

option add *Progress.undoneForeground black  widgetDefault
option add *Progress.undoneBackground white  widgetDefault
option add *Progress.doneForeground   white  widgetDefault
option add *Progress.doneBackground   green4 widgetDefault
option add *Progress.borderWidth      1      widgetDefault
option add *Progress.relief           sunken widgetDefault

proc Progress {w args} {
    uplevel 1 [list frame $w -class Progress] $args

    foreach {val} {
        undoneForeground doneForeground
        undoneBackground doneBackground
    } {
        set class [string toupper [string index $val 0]\
            ][string range $val 1 end]
        set $val [option get $w $val $class]
    }

    set varname progressPercent($w)

    frame $w.l -borderwidth 0 -background $undoneBackground
    label $w.l.l -textvariable $varname -borderwidth 0 \
        -foreground $undoneForeground -background $undoneBackground
    $w.l configure -height [expr {int([winfo reqheight $w.l.l]+2)}]
    frame $w.l.fill -background $doneBackground
    label $w.l.fill.l -textvariable $varname -borderwidth 0 \
        -foreground $doneForeground -background $doneBackground

    bind $w.l <Configure> [list ProgressConf $w "%w"]

    pack $w.l -fill both -expand 1
    place $w.l.l -relx 0.5 -rely 0.5 -anchor center
    place $w.l.fill -x 0 -y 0 -relheight 1 -relwidth 0
    place $w.l.fill.l -x 0 -rely 0.5 -anchor center

    SetProgress $w 0
    return $w
}

proc ProgressConf {w width} {
    place conf $w.l.fill.l -x [expr {int($width/2)}]
}

proc SetProgress {win value {range 100} {textlabel ""}} {
    global progressPercent
    set progress [expr {int(100*$value)/int($range)}]
    set relwidth [expr {double($value)/double($range)}]
    place conf $win.l.fill -relwidth $relwidth
    set progressPercent($win) "$textlabel - ${progress}%"
}
