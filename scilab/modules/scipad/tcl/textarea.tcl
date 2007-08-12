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

proc TextStyles { t } {
    global colorpref
    foreach c1 $colorpref {global $c1}
    global actbptextFont
    global linenumbersmargins

    set REPLACEDTEXTCOLOR $FOUNDTEXTCOLOR
    set FAKESELCOLOR $SELCOLOR

    $t configure -fg $FGCOLOR -bg $BGCOLOR -insertbackground $CURCOLOR \
        -selectbackground $SELCOLOR 
    $t tag configure parenthesis -foreground $PARCOLOR
    $t tag configure bracket -foreground $BRAKCOLOR
    $t tag configure brace -foreground $BRACCOLOR
    $t tag configure punct -foreground $PUNCOLOR
    $t tag configure intfun -foreground $INTFCOLOR
    $t tag configure command -foreground $COMMCOLOR
    $t tag configure libfun -foreground $LFUNCOLOR
    $t tag configure userfun -foreground $USERFUNCOLOR
    $t tag configure scicos -foreground $SCICCOLOR
    $t tag configure predef -foreground $PDEFCOLOR
    $t tag configure operator -foreground $OPCOLOR
    $t tag configure textquoted -foreground $QTXTCOLOR
    $t tag configure rem2 -foreground $REMCOLOR
    $t tag configure xmltag -foreground $XMLCOLOR
    $t tag configure number -foreground $NUMCOLOR
    $t tag configure breakpoint -background $BREAKPOINTCOLOR
    $t tag configure activebreakpoint -background $BREAKPOINTCOLOR
    $t tag configure activebreakpoint -font $actbptextFont \
        -relief raised -borderwidth 2
    $t tag configure foundtext -background $FOUNDTEXTCOLOR
    $t tag configure replacedtext -background $REPLACEDTEXTCOLOR
    $t tag configure fakeselection -background $FAKESELCOLOR
    $t tag raise activebreakpoint breakpoint
    $t tag raise sel activebreakpoint

    if {$linenumbersmargins != "hide" && [isdisplayed $t]} {
        set tapwfr [getpaneframename $t]
        $tapwfr.margin configure -background $BGLNMARGCOLOR -foreground $FGLNMARGCOLOR
    }
}

proc setwingeom {wintoset} {
# set $wintoset toplevel position such that the window is centered both
# horizontally and vertically in the screen
# the window is also set to be of fixed size (i.e. non resizable)
# note: this proc must obviously be called after the content of $wintoset
# has been created
    global pad
    wm resizable $wintoset 0 0
    set myx [expr {([winfo screenwidth  $pad]/2) - \
                ([winfo reqwidth  $wintoset]/2)}]
    set myy [expr {([winfo screenheight $pad]/2) - \
                ([winfo reqheight $wintoset]/2)}]
    wm geometry $wintoset +$myx+$myy
}

proc highlighttextarea {textarea} {
# Set the visual hint such that $textarea can be recognized as being the
# active buffer
    global pad
    foreach pw [getlistofpw] {
        foreach pa [$pw panes] {
            $pa configure -background gray
        }
    }
    if {[isdisplayed $textarea]} {
        [getpaneframename $textarea] configure -background black
    } else {
        # should never happen because highlighttextarea is supposed
        # to be called only with a currently visible textarea argument
        # however, at least one case is still not solved that will trigger
        # the "invalid command name "none"" bug, but I couldn't reproduce
        # yet: it has to do with clicking Button-1 in Scipad while a dnd is
        # processed - Detailed error message is:
        #
        # invalid command name "none"
        # invalid command name "none"
        #     while executing
        # "[getpaneframename $textarea] configure -background black"
        #     (procedure "highlighttextarea" line 10)
        #     invoked from within
        # "highlighttextarea $textarea"
        #     (procedure "focustextarea" line 19)
        #     invoked from within
        # "focustextarea .scipad.new4 "
        #     invoked from within
        # "if {$dndinitiated == "true"} {  set dndinitiated "false" ;  } else {  if {[info exists listoffile(".scipad.new4",fullname)]} {  focustextarea .scipad....}}"
        #     (command bound to event)
        #
        tk_messageBox -message "Unexpected condition triggered in proc highlighttextarea. Clicking OK in this dialog will display the full error message. Please report to the Bugzilla and detail precisely what you were doing when this happened."
        # trigger the error
        [getpaneframename $textarea] configure -background black
    }
}

proc togglewordwrap {} {
    global wordWrap listoftextarea
    foreach ta $listoftextarea {
        $ta configure -wrap $wordWrap
    }
    if {$wordWrap != "none"} {        
        # remove x scrollbars
        foreach ta $listoftextarea {
            if {[isdisplayed $ta]} {
                set tapwfr [getpaneframename $ta]
                pack forget $tapwfr.xscroll
                destroy $tapwfr.xscroll
                pack forget $tapwfr.bottom
                destroy $tapwfr.bottom
            }
        }
    } else {
        # display x scrollbars
        foreach ta $listoftextarea {
            if {[isdisplayed $ta]} {
                set tapwfr [getpaneframename $ta]
                frame $tapwfr.bottom
                pack $tapwfr.bottom -side bottom -expand 0 -fill both
                scrollbar $tapwfr.xscroll -command "$ta xview" \
                    -takefocus 0 -orient horizontal
                pack $tapwfr.xscroll -in $tapwfr.bottom -side bottom \
                    -expand 1 -fill x
            }
        }
    }
}
