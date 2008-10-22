# A Resizable Notebook widget for Tcl/Tk
# $Revision: 1.1 $
#
# Copyright (C) 1999 Daniel Roche
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
# 
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA  02111-1307, USA.
#
# Author contact information:
#   daniel.roche@bigfoot.com
#   http://www.multimania.com/droche/
#
# Thanks to Earl Roosa <earl.roosa@db.com> for the addtab patch
#
#########################################################
# 
# Rnotebook:create new_widget_name [option value ...]
#  where options are :
#   [-borderwidth i]      set the borderwidth of the tabs and the frame
#   [-height i]           set the height of the tabs in pixel
#   [-tabs tablist]       set the tabs according to each element of the list
#   [-nbtab i]            set the tabs number, by default the tabs are named 
#                         according to their position number
#   [-padx i]             set the space between each tabs in pixel
#   [-background col]     set the background color for the tabs and the frame
#   [-foreground col]     set the background color for the tabs
#
# Rnotebook:config widget_name [option value ...]
#  where options are :
#   [-borderwidth i]      set the borderwidth of the tabs and the frame
#   [-height i]           set the height of the tabs in pixel
#   [-padx i]             set the space between each tabs in pixel
#   [-background col]     set the background color for the tabs and the frame
#   [-foreground col]     set the background color for the tabs
#   [-tabs tablist]       set the tabs according to each element of the list
#   [-nbtab i]            set the tabs number, by default the tabs are named 
#                         according to their position number
#
# Rnotebook:raise widget_name page_number
#  raise the specified page number of the specified notebook
#
# Rnotebook:frame widget_name page_number
#  return the frame widget associated with the specified page number
#  it is a real tcl 'frame' widget and thus can be configured as any other frame.
#
# Rnotebook:button widget_name page_number
#  return the button widget associated with the specified page number
#  it is a real tcl 'button' widget and thus can be configured as any other button.
#
#########################################################


#
# Create a new notebook widget
#
proc Rnotebook:create {w args} {
    global Rnotebook tcl_platform
    # create/init base widget
    frame $w -borderwidth 0 -relief flat
    # init default data structure
    set Rnotebook($w,nbtab) 1
    set Rnotebook($w,tabs) {1}
    set Rnotebook($w,current) 0
    set Rnotebook($w,theight) 30
    set Rnotebook($w,padx) 0
    set Rnotebook($w,bwidth) 2
    set Rnotebook($w,background) [$w cget -background]
    set Rnotebook($w,foreground) black
    # parse arguments
    foreach {tag value} $args {
	switch -- $tag {
	    -borderwidth {
		set Rnotebook($w,bwidth) $value
	    }
	    -height {
		set Rnotebook($w,theight) $value
	    }
	    -tabs {
		set Rnotebook($w,tabs) $value
		set Rnotebook($w,nbtab) [llength $value]
	    }
	    -nbtab {
		set Rnotebook($w,nbtab) $value
	    }
	    -padx {
		set Rnotebook($w,padx) $value
	    }
	    -background {
		set Rnotebook($w,background) $value
	    }
	    -foreground {
		set Rnotebook($w,foreground) $value
	    }
	}
    }
    # build associated widgets
    frame $w.tabs -borderwidth 0 -relief flat
	    #-background $Rnotebook($w,background)
    frame $w.body -borderwidth $Rnotebook($w,bwidth) -relief raised \
	    -background $Rnotebook($w,background)
    frame $w.mask -borderwidth 0 -relief flat -background $Rnotebook($w,background)
    frame $w.mskl -borderwidth 0 -relief flat -background $Rnotebook($w,background)
    if [string compare $tcl_platform(platform) windows] {
	# Unix look :-)
	frame $w.mskl.ml -borderwidth $Rnotebook($w,bwidth) -relief sunken
	place $w.mskl.ml -x -$Rnotebook($w,bwidth) -y -$Rnotebook($w,bwidth) \
		-width [expr 2 * $Rnotebook($w,bwidth)] \
		-height [expr 3 * $Rnotebook($w,bwidth)]
    } else {
	# losedoze look :-(
	frame $w.mskl.ml -borderwidth $Rnotebook($w,bwidth) -relief raised
	place $w.mskl.ml -x 0 -y -$Rnotebook($w,bwidth) \
		-width [expr 2 * $Rnotebook($w,bwidth)] \
		-height [expr 4 * $Rnotebook($w,bwidth)]
    }
    frame $w.mskr -borderwidth 0 -relief flat -background $Rnotebook($w,background)
    if [string compare $tcl_platform(platform) windows] {
	# Unix look :-)
	frame $w.mskr.mr -borderwidth $Rnotebook($w,bwidth) -relief sunken
	place $w.mskr.mr -x 0 -y -$Rnotebook($w,bwidth) \
		-width [expr 2 * $Rnotebook($w,bwidth)] \
		-height [expr 3 * $Rnotebook($w,bwidth)]
    } else {
	# losedoze look :-(
	frame $w.mskr.mr -borderwidth $Rnotebook($w,bwidth) -relief raised
	place $w.mskr.mr -x -$Rnotebook($w,bwidth) -y -$Rnotebook($w,bwidth) \
		-width [expr 2 * $Rnotebook($w,bwidth)] \
		-height [expr 4 * $Rnotebook($w,bwidth)]
    }
    place $w.tabs -x 0 -y 0 -relwidth 1.0 -height $Rnotebook($w,theight)
    place $w.body -x 0 -y $Rnotebook($w,theight) -relwidth 1.0 \
	    -relheight 1.0 -height -$Rnotebook($w,theight)
    for {set ind 1} {$ind <= $Rnotebook($w,nbtab)} {incr ind} {
        Rnotebook:addtab $w $ind
    }
    # show page 1
    eval Rnotebook:raise $w 1
}

proc Rnotebook:addtab {w ind} {
    global Rnotebook
    set i2 [expr $ind - 1]
    set txt [lindex $Rnotebook($w,tabs) $i2]
    if {! [string length $txt] } {
        set txt $ind
    }
    button $w.tabs.b$ind -text $txt -borderwidth $Rnotebook($w,bwidth) \
	 -background $Rnotebook($w,background) \
	 -foreground $Rnotebook($w,foreground) \
	 -command "Rnotebook:raise $w $ind"
    pack $w.tabs.b$ind -side left -pady 0 -padx $Rnotebook($w,padx) -fill y
    bind $w.tabs.b$ind <Configure> "Rnotebook:raise $w current"
    frame $w.body.f$ind -borderwidth 0 -background $Rnotebook($w,background)

}

#
# Change configuration options for the notebook widget
#
proc Rnotebook:config {w args} {
    global Rnotebook tcl_platform
    set nbt $Rnotebook($w,nbtab)
    foreach {tag value} $args {
	switch -- $tag {
	    -borderwidth {
		set Rnotebook($w,bwidth) $value
		$w.body configure -borderwidth $Rnotebook($w,bwidth)
		$w.mskl.ml configure -borderwidth $Rnotebook($w,bwidth)
		$w.mskr.mr configure -borderwidth $Rnotebook($w,bwidth)
		set lst [winfo children $w.tabs]
		foreach wid $lst {
		    $wid configure -borderwidth $Rnotebook($w,bwidth)
		}
		if [string compare $tcl_platform(platform) windows] {
		    # Unix look :-)
		    place $w.mskl.ml -x -$Rnotebook($w,bwidth) \
			    -y -$Rnotebook($w,bwidth) \
			    -width [expr 2 * $Rnotebook($w,bwidth)] \
			    -height [expr 3 * $Rnotebook($w,bwidth)]
		} else {
		    # losedoze look :-(
		    place $w.mskl.ml -x 0 -y -$Rnotebook($w,bwidth) \
			    -width [expr 2 * $Rnotebook($w,bwidth)] \
			    -height [expr 4 * $Rnotebook($w,bwidth)]
		}
		if [string compare $tcl_platform(platform) windows] {
		    # Unix look :-)
		    place $w.mskr.mr -x 0 -y -$Rnotebook($w,bwidth) \
			    -width [expr 2 * $Rnotebook($w,bwidth)] \
			    -height [expr 3 * $Rnotebook($w,bwidth)]
		} else {
		    # losedoze look :-(
		    place $w.mskr.mr -x -$Rnotebook($w,bwidth) -y \
			    -$Rnotebook($w,bwidth) \
			    -width [expr 2 * $Rnotebook($w,bwidth)] \
			    -height [expr 4 * $Rnotebook($w,bwidth)]
		}
	    }
	    -background {
		set Rnotebook($w,background) $value
		$w.body configure -background $Rnotebook($w,background)
		$w.mskl.ml configure -background $Rnotebook($w,background)
		$w.mskr.mr configure -background $Rnotebook($w,background)
		set lst [winfo children $w.tabs]
		foreach wid $lst {
		    $wid configure -background $Rnotebook($w,background)
		}
		$w.mask configure -background $Rnotebook($w,background)
	    }
	    -foreground {
		set Rnotebook($w,foreground) $value
		set lst [winfo children $w.tabs]
		foreach wid $lst {
		    $wid configure -foreground $Rnotebook($w,foreground)
		}
	    }
	    -height {
		set Rnotebook($w,theight) $value
		place $w.tabs -x 0 -y 0 -relwidth 1.0 -height $Rnotebook($w,theight)
		place $w.body -x 0 -y $Rnotebook($w,theight) -relwidth 1.0 \
			-relheight 1.0 -height -$Rnotebook($w,theight)
	    }
	    -padx {
		set Rnotebook($w,padx) $value
		set lst [winfo children $w.tabs]
		foreach wid $lst {
		    pack $wid -padx $Rnotebook($w,padx)
		}
	    }
	    -tabs {
		set Rnotebook($w,tabs) $value
		set Rnotebook($w,nbtab) [llength $value]
	    }
	    -nbtab {
		set Rnotebook($w,nbtab) $value
	    }
	}
    }
    if {$nbt < $Rnotebook($w,nbtab)} {
        for {set ind [expr $nbt+1]} {$ind <= $Rnotebook($w,nbtab)} {incr ind} {
            Rnotebook:addtab $w $ind
        }
    }
}

#
# This procedure raises a notebook page given its number.
# first page is number 1
#
proc Rnotebook:raise {w num} {
    global Rnotebook
    if { ![string compare $num "current"] } {
	set num $Rnotebook($w,current)
    }
    if { $num == 0 } {
	set num 1
    }
    if { $num != $Rnotebook($w,current) } {
	pack forget $w.body.f$Rnotebook($w,current)
	pack $w.body.f$num -fill both -expand 1
    }
    set Rnotebook($w,current) $num
    set bw $Rnotebook($w,bwidth)

    set x0 [expr [winfo x $w.tabs.b$num] + [winfo x $w.tabs] + $bw ]
    set y0 [expr [winfo y $w.tabs.b$num] + [winfo y $w.tabs] + \
	    [winfo height $w.tabs.b$num] - $bw]
    set w0 [expr [winfo width $w.tabs.b$num] - ($bw * 2)] 
    set h0 [expr $bw * 2]
    place $w.mask -x $x0 -y $y0 -width $w0 -height $h0

    set x1 [expr $x0 - $bw]
    set y1 $y0
    set w1 $bw
    set h1 $h0
    place $w.mskl -x $x1 -y $y1 -width $w1 -height $h1
    
    set x2 [expr $x0 + $w0]
    set y2 $y0
    set w2 $bw
    set h2 $h0
    place $w.mskr -x $x2 -y $y2 -width $w2 -height $h2
}

#
# Return the frame associated with a given page number
#
proc Rnotebook:frame {w num} {
  global Rnotebook
  set i "$w.body.f$num"
  if [ winfo exists $i] {
    return $i
  } else {
    return {}
  }
}

#
# Return the button associated with a given page number
#
proc Rnotebook:button {w num} {
  global Rnotebook
  set i "$w.tabs.b$num"
  if [ winfo exists $i] {
    return $i
  } else {
    return {}
  }
}


