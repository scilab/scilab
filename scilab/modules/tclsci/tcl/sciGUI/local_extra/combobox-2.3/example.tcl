# A simple font display utility.
# 
# This program uses two variations of the combobox. One, for the font
# family, is non-editable (ie: the user can only pick from the list). 
# The other, for font size, is editable. The user can pick from the list 
# or enter their own size

# substitute your favorite method here...
source combobox.tcl
package require combobox 2.3
catch {namespace import combobox::*}

proc main {} {
    # this lets us be reentrant...
    destroy .fontpicker .msg

    # default values
    set ::size       12
    set ::family     [lindex [lsort [font families]] 0]
    set ::slant      roman
    set ::weight     normal
    set ::overstrike 0
    set ::underline  0

    wm title . "Combobox Example"

    # the main two areas: a frame to hold the font picker widgets
    # and a label to display a sample from the font
    set fp [frame .fontpicker]
    set msg [label .msg -borderwidth 2 -relief groove -width 30 -height 4]

    pack $fp -side top -fill x
    pack $msg -side top -fill both -expand y -pady 2

    $msg configure -text [join [list \
	    "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
	    "abcdefghijklmnopqrstuvwxyz" \
	    "0123456789~`!@#$%^&*()_-+=" \
	    "{}[]:;\"'<>,.?/"] "\n"]

    # this will set the font of the message according to our defaults
    changeFont $msg

    # font family...
    label $fp.famLabel -text "Font Family:"
    combobox $fp.famCombo \
	    -borderwidth 1 \
	    -textvariable family \
	    -editable false \
	    -highlightthickness 1 \
	    -command [list changeFont $msg]


    grid $fp.famLabel -row 0 -column 0 -sticky e
    grid $fp.famCombo -row 0 -column 1 -sticky ew
    grid columnconfigure $fp 1 -weight 1

    # we'll do these one at a time so we can find the widest one and
    # set the width of the combobox accordingly (hmmm... wonder if this
    # sort of thing should be done by the combobox itself...?)
    set widest 0
    foreach family [lsort [font families]] {
	if {[set length [string length $family]] > $widest} {
	    set widest $length
	}
	$fp.famCombo list insert end $family
    }
    $fp.famCombo configure -width $widest

    # the font size. We know we are puting a fairly small, finite
    # number of items in this combobox, so we'll set its maxheight
    # to zero so it will grow to fit the number of items
    label $fp.sizeLabel -text "Font Size:"
    combobox $fp.sizeCombo \
	    -borderwidth 1 \
	    -highlightthickness 1 \
	    -maxheight 0 \
	    -width 3 \
	    -textvariable size \
	    -editable true \
	    -command [list changeFont $msg]

    grid $fp.sizeLabel -row 0 -column 2 -sticky e -padx 2
    grid $fp.sizeCombo -row 0 -column 3 -sticky new 
    grid columnconfigure $fp 3 -weight 1
    eval $fp.sizeCombo list insert end [list 8 9 10 12 14 16 18 20 24 28 32 36]

    # a dummy frame to give a little spacing...
    frame $fp.dummy -width 5
    grid $fp.dummy -row 0 -column 4 

    # bold
    set bold "bold"
    checkbutton $fp.bold -variable weight -indicatoron false \
	    -onvalue bold -offvalue normal \
	    -text "B" -width 2 -height 1 \
	    -font {-weight bold -family Times -size 12} \
	    -highlightthickness 1 -padx 0 -pady 0 -borderwidth 1 \
	    -command [list changeFont $msg]
    grid $fp.bold -row 0 -column 5 -sticky nsew

    # underline
    checkbutton $fp.underline -variable underline -indicatoron false \
	    -onvalue 1 -offvalue 0 \
	    -text "U" -width 2 -height 1 \
	    -font {-underline 1 -family Times -size 12} \
	    -highlightthickness 1 -padx 0 -pady 0 -borderwidth 1 \
	    -command [list changeFont $msg]
    grid $fp.underline -row 0 -column 6 -sticky nsew

    # italic
    checkbutton $fp.italic -variable slant -indicatoron false \
	    -onvalue italic -offvalue roman \
	    -text "I" -width 2 -height 1 \
	    -font {-slant italic -family Times -size 12} \
	    -highlightthickness 1 -padx 0 -pady 0 -borderwidth 1 \
	    -command [list changeFont $msg]
    grid $fp.italic -row 0 -column 7 -sticky nsew

    # overstrike
    checkbutton $fp.overstrike -variable overstrike -indicatoron false \
	    -onvalue 1 -offvalue 0 \
	    -text "O" -width 2 -height 1 \
	    -font {-overstrike 1 -family Times -size 12} \
	    -highlightthickness 1 -padx 0 -pady 0 -borderwidth 1 \
	    -command [list changeFont $msg]
    grid $fp.overstrike -row 0 -column 8 -sticky nsew

    # this gives us relatively square buttons that fill the frame
    # in the Y direction
    bind $fp <Configure> {
	grid columnconfigure %W 5 -minsize %h
	grid columnconfigure %W 6 -minsize %h
	grid columnconfigure %W 7 -minsize %h
	grid columnconfigure %W 8 -minsize %h
    }
    # put focus on the first widget
    catch {focus $fp.famCombo}

    return ""
}

# this proc changes the font. It is called by various methods, so
# the only parameter we are guaranteed is the first one since
# we supply it ourselves...
proc ::changeFont {w args} {
    foreach foo [list family size weight underline slant overstrike] {
	if {[set ::$foo] == ""} {
	    return
	}
    }
    set ::fontspec [list \
	    -family     $::family \
	    -size       $::size \
	    -weight     $::weight \
	    -underline  $::underline \
	    -slant      $::slant \
	    -overstrike $::overstrike \
    ]
    $w configure -font $::fontspec
}

main
