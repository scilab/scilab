##
## Copyright 1996-8 Jeffrey Hobbs, jeff.hobbs@acm.org
##
## Initiated: 28 October 1996
##
package provide BalloonHelp 2.0

##------------------------------------------------------------------------
## PROCEDURE
##	balloonhelp
##
## DESCRIPTION
##	Implements a balloon help system
##
## ARGUMENTS
##	balloonhelp <option> ?arg?
##
## clear ?pattern?
##	Stops the specified widgets (defaults to all) from showing balloon
##	help.
##
## delay ?millisecs?
##	Query or set the delay.  The delay is in milliseconds and must
##	be at least 50.  Returns the delay.
##
## disable OR off
##	Disables all balloon help.
##
## enable OR on
##	Enables balloon help for defined widgets.
##
## <widget> ?-index index? ?message?
##	If -index is specified, then <widget> is assumed to be a menu
##	and the index represents what index into the menu (either the
##	numerical index or the label) to associate the balloon help
##	message with.  Balloon help does not appear for disabled menu items.
##	If message is {}, then the balloon help for that
##	widget is removed.  The widget must exist prior to calling
##	balloonhelp.  The current balloon help message for <widget> is
##	returned, if any.
##
## RETURNS: varies (see methods above)
##
## NAMESPACE & STATE
##	The global array BalloonHelp is used.  Procs begin with BalloonHelp.
## The overrideredirected toplevel is named $BalloonHelp(TOPLEVEL).
##
## EXAMPLE USAGE:
##	balloonhelp .button "A Button"
##	balloonhelp .menu -index "Load" "Loads a file"
##
##------------------------------------------------------------------------

namespace eval ::Widget::BalloonHelp {;

namespace export -clear balloonhelp
variable BalloonHelp

## The extra :hide call in <Enter> is necessary to catch moving to
## child widgets where the <Leave> event won't be generated
bind Balloons <Enter> [namespace code {
    #BalloonHelp:hide
    variable BalloonHelp
    set BalloonHelp(LAST) -1
    if {$BalloonHelp(enabled) && [info exists BalloonHelp(%W)]} {
	set BalloonHelp(AFTERID) [after $BalloonHelp(DELAY) \
		[namespace code [list show %W $BalloonHelp(%W)]]]
    }
}]

bind Menu <<MenuSelect>>	[namespace code { menuMotion %W }]
bind Balloons <Leave>		[namespace code hide]
bind Balloons <Any-KeyPress>	[namespace code hide]
bind Balloons <Any-Button>	[namespace code hide]

array set BalloonHelp {
    enabled	1
    DELAY	500
    AFTERID	{}
    LAST	-1
    TOPLEVEL	.__balloonhelp__
}

proc balloonhelp {w args} {
    variable BalloonHelp
    switch -- $w {
	clear	{
	    if {[llength $args]==0} { set args .* }
	    clear $args
	}
	delay	{
	    if {[llength $args]} {
		if {![regexp {^[0-9]+$} $args] || $args<50} {
		    return -code error "BalloonHelp delay must be an\
			    integer greater than 50 (delay is in millisecs)"
		}
		return [set BalloonHelp(DELAY) $args]
	    } else {
		return $BalloonHelp(DELAY)
	    }
	}
	off - disable	{
	    set BalloonHelp(enabled) 0
	    hide
	}
	on - enable	{
	    set BalloonHelp(enabled) 1
	}
	default {
	    set i $w
	    if {[llength $args]} {
		set i [uplevel 1 [namespace code "register [list $w] $args"]]
	    }
	    set b $BalloonHelp(TOPLEVEL)
	    if {![winfo exists $b]} {
		toplevel $b
		wm overrideredirect $b 1
		wm positionfrom $b program
		wm withdraw $b
		pack [label $b.l -highlightthickness 0 -relief raised -bd 1 \
			-background yellow]
	    }
	    if {[info exists BalloonHelp($i)]} { return $BalloonHelp($i) }
	}
    }
}

;proc register {w args} {
    variable BalloonHelp
    set key [lindex $args 0]
    while {[string match -* $key]} {
	switch -- $key {
	    -index	{
		if {[catch {$w entrycget 1 -label}]} {
		    return -code error "widget \"$w\" does not seem to be a\
			    menu, which is required for the -index switch"
		}
		set index [lindex $args 1]
		set args [lreplace $args 0 1]
	    }
	    default	{
		return -code error "unknown option \"$key\": should be -index"
	    }
	}
	set key [lindex $args 0]
    }
    if {[llength $args] != 1} {
	return -code error "wrong \# args: should be \"balloonhelp widget\
		?-index index? message\""
    }
    if {[string match {} $key]} {
	clear $w
    } else {
	if {![winfo exists $w]} {
	    return -code error "bad window path name \"$w\""
	}
	if {[info exists index]} {
	    set BalloonHelp($w,$index) $key
	    #bindtags $w [linsert [bindtags $w] end BalloonsMenu]
	    return $w,$index
	} else {
	    set BalloonHelp($w) $key
	    bindtags $w [linsert [bindtags $w] end Balloons]
	    return $w
	}
    }
}

;proc clear {{pattern .*}} {
    variable BalloonHelp
    foreach w [array names BalloonHelp $pattern] {
	unset BalloonHelp($w)
	if {[winfo exists $w]} {
	    set tags [bindtags $w]
	    if {[set i [lsearch $tags Balloons]] != -1} {
		bindtags $w [lreplace $tags $i $i]
	    }
	    ## We don't remove BalloonsMenu because there
	    ## might be other indices that use it
	}
    }
}

;proc show {w msg {i {}}} {
    ## Use string match to allow that the help will be shown when
    ## the pointer is in any child of the desired widget
    if {![winfo exists $w] || ![string match \
	    $w* [eval winfo containing [winfo pointerxy $w]]]} return

    variable BalloonHelp
    global tcl_platform
    set b $BalloonHelp(TOPLEVEL)
    $b.l configure -text $msg
    update idletasks
    if {[string compare {} $i]} {
	set y [expr [winfo rooty $w]+[$w yposition $i]+25]
	if {($y+[winfo reqheight $b])>[winfo screenheight $w]} {
	    set y [expr [winfo rooty $w]+[$w yposition $i]-\
		    [winfo reqheight $b]-5]
	}
    } else {
	set y [expr [winfo rooty $w]+[winfo height $w]+5]
	if {($y+[winfo reqheight $b])>[winfo screenheight $w]} {
	    set y [expr [winfo rooty $w]-[winfo reqheight $b]-5]
	}
    }
    set x [expr [winfo rootx $w]+([winfo width $w]-[winfo reqwidth $b])/2]
    if {$x<0} {
	set x 0
    } elseif {($x+[winfo reqwidth $b])>[winfo screenwidth $w]} {
	set x [expr [winfo screenwidth $w]-[winfo reqwidth $b]]
    }
    wm geometry $b +$x+$y
    if {[string match windows $tcl_platform(platform)]} {
	## Yes, this is only needed on Windows
	update idletasks
    }
    wm deiconify $b
    raise $b
}

;proc menuMotion {w} {
    variable BalloonHelp
    if {$BalloonHelp(enabled)} {
	set cur [$w index active]
	## The next two lines (all uses of LAST) are necessary until the
	## <<MenuSelect>> event is properly coded for Unix/(Windows)?
	if {$cur == $BalloonHelp(LAST)} return
	set BalloonHelp(LAST) $cur
	## a little inlining - this is :hide
	after cancel $BalloonHelp(AFTERID)
	catch {wm withdraw $BalloonHelp(TOPLEVEL)}
	if {[info exists BalloonHelp($w,$cur)] || \
		(![catch {$w entrycget $cur -label} cur] && \
		[info exists BalloonHelp($w,$cur)])} {
	    set BalloonHelp(AFTERID) [after $BalloonHelp(DELAY) \
		    [namespace code [list show $w $BalloonHelp($w,$cur) $cur]]]
	}
    }
}

;proc hide {args} {
    variable BalloonHelp
    after cancel $BalloonHelp(AFTERID)
    catch {wm withdraw $BalloonHelp(TOPLEVEL)}
}

}; # end namespace ::Widget::BalloonHelp

namespace eval :: {namespace import -force ::Widget::BalloonHelp::balloonhelp}
