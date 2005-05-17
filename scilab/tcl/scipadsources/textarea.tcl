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

proc dupWidgetText {widget name} {
    $name insert end [$widget get 1.0 end]
}

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
    global colorpref
    foreach c1 $colorpref {global $c1}
    global actbptextFont textFont

    set REPLACEDTEXTCOLOR $FOUNDTEXTCOLOR
    set FAKESELCOLOR $SELCOLOR

    $t configure -fg $FGCOLOR -bg $BGCOLOR -insertbackground $CURCOLOR \
        -selectbackground $SELCOLOR 
    $t tag configure parenthesis -foreground $PARCOLOR
    $t tag configure bracket -foreground $BRAKCOLOR
    $t tag configure brace -foreground $BRACCOLOR
    $t tag configure punct -foreground $PUNCOLOR
    $t tag configure keywords -foreground $KEYWCOLOR
    $t tag configure libfun -foreground $LFUNCOLOR
    $t tag configure predef -foreground $PDEFCOLOR
    $t tag configure operator -foreground $OPCOLOR
    $t tag configure text -foreground $FGCOLOR
    $t tag configure textquoted -foreground $QTXTCOLOR
    $t tag configure rem2 -foreground $REMCOLOR
    $t tag configure xmltag -foreground $XMLCOLOR
    $t tag configure number -foreground $NUMCOLOR
    scipadindent $t .8
    $t tag configure breakpoint -background $BREAKPOINTCOLOR
    $t tag configure activebreakpoint -font $actbptextFont \
        -relief raised -borderwidth 2
    $t tag configure foundtext -background $FOUNDTEXTCOLOR
    $t tag configure replacedtext -background $REPLACEDTEXTCOLOR
    $t tag configure fakeselection -background $FAKESELCOLOR
    $t tag raise sel activebreakpoint
    $t tag raise sel breakpoint
}

proc setfontscipad {FontSize} {
    global textFont menuFont pad
    global listoftextarea
    set textFont -Adobe-courier-medium-R-Normal-*-$FontSize-*
    set menuFont -adobe-helvetica-bold-r-normal--$FontSize-*
    set actbptextFont -Adobe-courier-bold-R-Normal-*-[expr $FontSize + 2]-*
    $pad.filemenu configure -font $menuFont
    $pad.filemenu.files configure -font $menuFont
    $pad.filemenu.edit configure -font $menuFont
    $pad.filemenu.search configure -font $menuFont
    $pad.filemenu.wind configure -font $menuFont
    $pad.filemenu.scheme configure -font $menuFont
    $pad.filemenu.options configure -font $menuFont
    $pad.filemenu.options.fontsize configure -font $menuFont
    $pad.filemenu.options.colors configure -font $menuFont
    $pad.filemenu.options.tabs configure -font $menuFont
    # warning! tk_optionMenu added ".menu" to the menu name given on creation!
    $pad.filemenu.options.tabs.indentspaces.menu configure -font $menuFont
    $pad.filemenu.options.filenames configure -font $menuFont
    $pad.filemenu.options.recent.menu configure -font $menuFont
    $pad.filemenu.exec configure -font $menuFont
    $pad.filemenu.debug configure -font $menuFont
    $pad.filemenu.debug.step configure -font $menuFont
    $pad.filemenu.help configure -font $menuFont
    foreach textarea $listoftextarea {
        $textarea configure -font $textFont
        $textarea tag configure activebreakpoint -font $actbptextFont
    }
    $pad.statusind configure -font $menuFont
    $pad.statusind2 configure -font $menuFont
    $pad.statusmes configure -font $menuFont
    showinfo [concat [mc "Font size"] $FontSize ]
}

proc exitapp { {quittype yesno} } {
# exit app
    global listoftextarea
    if {[getdbstate] == "DebugInProgress"} canceldebug_bp
    foreach textarea $listoftextarea {
        closecur $quittype
    }
}

proc setwingeom {wintoset} {
# proc to set child window position
    global pad
    wm resizable $wintoset 0 0
    set myx [expr (([winfo screenwidth $pad]/2) - \
                ([winfo reqwidth $wintoset]))]
    set myy [expr (([winfo screenheight $pad]/2) - \
                ([winfo reqheight $wintoset]/2))]
    wm geometry $wintoset +$myx+$myy
}
