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
    $t tag configure activebreakpoint -relief raised -borderwidth 1
}


#ES: instead of several setfontscipadN{}
proc setfontscipad {FontSize} {
    global textFont menuFont lang pad
    global listoftextarea
    set textFont -Adobe-courier-medium-R-Normal-*-$FontSize-*
    set menuFont -adobe-helvetica-bold-r-normal--$FontSize-*
    $pad.filemenu configure -font $menuFont
    $pad.filemenu.files configure -font $menuFont
    $pad.filemenu.edit configure -font $menuFont
    $pad.filemenu.search configure -font $menuFont
    $pad.filemenu.wind configure -font $menuFont
    $pad.filemenu.options configure -font $menuFont
    $pad.filemenu.exec configure -font $menuFont
    #$pad.filemenu.debug configure -font $menuFont
    $pad.filemenu.help configure -font $menuFont
    foreach textarea $listoftextarea {$textarea configure -font $textFont}
    $pad.statusind configure -font $menuFont
    $pad.statusind2 configure -font $menuFont
    $pad.statusmes configure -font $menuFont
    if {$lang=="eng"} {showinfo "Font size $FontSize"
    } else { showinfo "Police taille $FontSize"}   
}


# exit app
proc exitapp {} {
    global listoffile listoftextarea pad

    foreach textarea $listoftextarea {
	#inccount $textarea
	closefile $textarea
    }
#    unset pad  ## not needed if closefile unsets it
}


# proc to set child window position
proc setwingeom {wintoset} {
    global pad
    wm resizable $wintoset 0 0
    set myx [expr (([winfo screenwidth $pad]/2) - \
		       ([winfo reqwidth $wintoset]))]
    set myy [expr (([winfo screenheight $pad]/2) - \
		       ([winfo reqheight $wintoset]/2))]
    wm geometry $wintoset +$myx+$myy
}
