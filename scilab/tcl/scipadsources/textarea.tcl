#create textarea
set taille [expr [font measure $textFont " "] *3]

# creates the default textarea 
##ES was here: added insertofftime 0 and exportselection
## Francois Vogel, 21/04/04: changed insertofftime to 500, and added insertontime 500
## FV 13/05/04: added test with tk_version on ES request
text $pad.textarea -relief sunken -bd 2 -xscrollcommand "$pad.xscroll set" \
	-yscrollcommand "$pad.yscroll set" -wrap $wordWrap -width 1 -height 1 \
        -fg $FGCOLOR -bg $BGCOLOR  -setgrid 1 -font $textFont -tabs $taille \
        -insertwidth 3 -insertborderwidth 2 \
        -insertbackground $CURCOLOR -selectbackground $SELCOLOR -exportselection 1
set textareacur $pad.textarea  
if {[expr $tk_version] >= 8.4} {
    $textareacur configure -insertofftime 500 -insertontime 500
} else {
    $textareacur configure -insertofftime 0
}
####
##ES: remember fontsize
setfontscipad $FontSize

scrollbar $pad.yscroll -command "$textareacur yview"
scrollbar $pad.xscroll -command "$textareacur xview" -orient horizontal
pack $textareacur  -in  $pad.bottomleftmenu -side left -expand 1 -fill both
pack $pad.yscroll -in $pad.bottomrightmenu -side right -expand 1 -fill both
pack $pad.xscroll -in $pad.bottombottommenu -expand 1 -fill x 
focus $textareacur

# added by matthieu PHILIPPE 21/11/2001
proc gettextareacur {} {
    global textareacur
    return $textareacur
}
proc settextareacur {val} {
    global textareacur
    set textareacur $val
}

# nondefOpts - return a list of options with values 
#              that are different than the default value.
#
# ARGUMENTS:
#	widget	the widget whose options are to be constructed
#
# RETURN VALUE:
#	a list with option/value pairs
#
# NOTES
#	The configure message to a widget returns either an option
#	description:
#		{argvName dbName dbClass defValue curValue}
#	or a synonym description:
#		{argvName dbName}
#
proc nondefOpts {widget} {
	set options {}
	foreach i [$widget configure] {
		if {[llength $i] != 2} {
			set optname [lindex $i 0]
			set defval [lindex $i 3]
			set curval [lindex $i 4]
			#if [string compare $defval $curval] {
				lappend options $optname $curval
			#}
		}
	}
	return $options
}

#
#
# dupWidget - make a copy of a widget.
#
# ARGUMENTS:
#	widget	the widget to be duplicated
#	name	the name for the new widget
#
# RETURN VALUE:
#	a new widget
#
proc dupWidgetOption {widget name} {
	return [eval  \
           "[string tolower [winfo class $widget]] $name [nondefOpts $widget]"]
}

#
#
#matthieu PHLIPPE 12/12/2001
proc dupWidgetText {widget name} {
	$name insert end [$widget get 1.0 end]
}
##

###ES: who uses this, besides TextStyles which sets it? 
# set the indent spacing (in cm) for lists
# TK uses a "weird" tabbing model that causes \t to insert a single
# space if the current line position is past the tab setting
proc scipadindent {textarea cm} {
    set tabs [expr $cm / 2.0]
    $textarea configure -tabs ${tabs}c
    set tab [expr 1 * $cm]
    $textarea tag configure indentation -lmargin1 ${tab}c -lmargin2 ${tab}c \
	    -tabs "[expr $tab + $tabs]c [expr $tab + 2*$tabs]c"
}

proc TextStyles { t } {
    global FGCOLOR PARCOLOR BRAKCOLOR BRACCOLOR PUNCOLOR KEYWCOLOR OPCOLOR
    global TXTCOLOR QTXTCOLOR REMCOLOR XMLCOLOR NUMCOLOR
    global BREAKPOINTCOLOR

    $t tag configure parenthesis -foreground $PARCOLOR
    $t tag configure bracket -foreground $BRAKCOLOR
    $t tag configure brace -foreground $BRACCOLOR
    $t tag configure punct -foreground $PUNCOLOR
    $t tag configure keywords -foreground $KEYWCOLOR
    $t tag configure operator -foreground $OPCOLOR
    $t tag configure text -foreground $FGCOLOR
    $t tag configure textquoted -foreground $QTXTCOLOR
    $t tag configure rem2 -foreground $REMCOLOR
    $t tag configure xmltag -foreground $XMLCOLOR
    $t tag configure number -foreground $NUMCOLOR
    scipadindent $t .8
# FV 13/05/04
    $t tag configure breakpoint -background $BREAKPOINTCOLOR
    $t tag configure activebreakpoint -underline true
}

TextStyles $textareacur

