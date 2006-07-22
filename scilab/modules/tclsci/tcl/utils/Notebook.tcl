# A Notebook widget for Tcl/Tk
#
# Copyright (C) 1996,1997,1998 D. Richard Hipp
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
#   drh@acm.org
#   http://www.hwaci.com/drh/

#
# jb Silvy. To know the current page
#

proc Notebook:getCurrentPage { w } {
  global Notebook
  return $Notebook($w,top)
}

#
# Create a new notebook widget
#

package provide Notebook

proc Notebook:create {w args} {
  global Notebook
  set Notebook($w,width) 400
  set Notebook($w,height) 300
  set Notebook($w,pages) {}
  set Notebook($w,top) 0
  set Notebook($w,pad) 5
  set Notebook($w,fg,on) black
  set Notebook($w,fg,off) grey50
  canvas $w -bd 0 -highlightthickness 0 -takefocus 0
  set Notebook($w,bg) [$w cget -bg]
  bind $w <1> "Notebook:click $w %x %y"
  bind $w <Configure> "Notebook:scheduleExpand $w"
  eval Notebook:config $w $args
}

#
# Change configuration options for the notebook widget
#
proc Notebook:config {w args} {
  global Notebook
  foreach {tag value} $args {
    switch -- $tag {
      -width {
        set Notebook($w,width) $value
      }
      -height {
        set Notebook($w,height) $value
      }
      -pages {
        set Notebook($w,pages) $value
      }
      -pad {
        set Notebook($w,pad) $value
      }
      -bg {
        set Notebook($w,bg) $value
      }
      -fg {
        set Notebook($w,fg,on) $value
      }
      -disabledforeground {
        set Notebook($w,fg,off) $value
      }
    }
  }

  #
  # After getting new configuration values, reconstruct the widget
  #
  $w delete all
  set Notebook($w,x1) $Notebook($w,pad)
  set Notebook($w,x2) [expr $Notebook($w,x1)+2]
  set Notebook($w,x3) [expr $Notebook($w,x2)+$Notebook($w,width)]
  set Notebook($w,x4) [expr $Notebook($w,x3)+2]
  set Notebook($w,y1) [expr $Notebook($w,pad)+2]
  set Notebook($w,y2) [expr $Notebook($w,y1)+2]
  set Notebook($w,y5) [expr $Notebook($w,y1)+30]
  set Notebook($w,y6) [expr $Notebook($w,y5)+2]
  set Notebook($w,y3) [expr $Notebook($w,y6)+$Notebook($w,height)]
  set Notebook($w,y4) [expr $Notebook($w,y3)+2]
  set x $Notebook($w,x1)
  set cnt 0
  set y7 [expr $Notebook($w,y1)+10]
  foreach p $Notebook($w,pages) {
    set Notebook($w,p$cnt,x5) $x
    set id [$w create text 0 0 -text $p -anchor nw -tags "p$cnt t$cnt" -font {Arial -15}  -fill $Notebook($w,fg,off)]
    set bbox [$w bbox $id]
    set width [lindex $bbox 2]
    $w move $id [expr $x+10] $y7
    $w create line \
       $x $Notebook($w,y5)\
       $x $Notebook($w,y2) \
       [expr $x+2] $Notebook($w,y1) \
       [expr $x+$width+16] $Notebook($w,y1) \
       -width 2 -fill white -tags p$cnt
    $w create line \
       [expr $x+$width+16] $Notebook($w,y1) \
       [expr $x+$width+18] $Notebook($w,y2) \
       [expr $x+$width+18] $Notebook($w,y5) \
       -width 2 -fill black -tags p$cnt
    set x [expr $x+$width+20]
    set Notebook($w,p$cnt,x6) [expr $x-2]
    if {![winfo exists $w.f$cnt]} {
      frame $w.f$cnt -bd 0
    }
    $w.f$cnt config -bg $Notebook($w,bg)
    place $w.f$cnt -x $Notebook($w,x2) -y $Notebook($w,y6) \
      -width $Notebook($w,width) -height $Notebook($w,height)
    incr cnt
  }
  $w create line \
     $Notebook($w,x1) [expr $Notebook($w,y5)-2] \
     $Notebook($w,x1) $Notebook($w,y3) \
     -width 2 -fill white
  $w create line \
     $Notebook($w,x1) $Notebook($w,y3) \
     $Notebook($w,x2) $Notebook($w,y4) \
     $Notebook($w,x3) $Notebook($w,y4) \
     $Notebook($w,x4) $Notebook($w,y3) \
     $Notebook($w,x4) $Notebook($w,y6) \
     $Notebook($w,x3) $Notebook($w,y5) \
     -width 2 -fill black
  $w config -width [expr $Notebook($w,x4)+$Notebook($w,pad)] \
            -height [expr $Notebook($w,y4)+$Notebook($w,pad)] \
            -bg $Notebook($w,bg)
  set top $Notebook($w,top)
  set Notebook($w,top) -1
  Notebook:raise.page $w $top
}

#
# This routine is called whenever the mouse-button is pressed over
# the notebook.  It determines if any page should be raised and raises
# that page.
#
proc Notebook:click {w x y} {
  global Notebook
  if {$y<$Notebook($w,y1) || $y>$Notebook($w,y6)} return
  set N [llength $Notebook($w,pages)]
  for {set i 0} {$i<$N} {incr i} {
    $w itemconfigure t$i -fill $Notebook($w,fg,off)
    if {$x>=$Notebook($w,p$i,x5) && $x<=$Notebook($w,p$i,x6)} {
      Notebook:raise.page $w $i
      #break
    }
  }
}

#
# For internal use only.  This procedure raised the n-th page of
# the notebook
#
proc Notebook:raise.page {w n} {
  global Notebook
  if {$n<0 || $n>=[llength $Notebook($w,pages)]} return
  set top $Notebook($w,top)
  if {$top>=0 && $top<[llength $Notebook($w,pages)]} {
    $w move p$top 0 2
    
  }
  $w move p$n 0 -2
  #$w itemconfigure t$n -font {Arial 12}
  $w itemconfigure t$n -fill $Notebook($w,fg,on)
  $w delete topline
  if {$n>0} {
    $w create line \
       $Notebook($w,x1) $Notebook($w,y6) \
       $Notebook($w,x2) $Notebook($w,y5) \
       $Notebook($w,p$n,x5) $Notebook($w,y5) \
       $Notebook($w,p$n,x5) [expr $Notebook($w,y5)-2] \
       -width 2 -fill white -tags topline
  }
  $w create line \
    $Notebook($w,p$n,x6) [expr $Notebook($w,y5)-2] \
    $Notebook($w,p$n,x6) $Notebook($w,y5) \
    -width 2 -fill white -tags topline
  $w create line \
    $Notebook($w,p$n,x6) $Notebook($w,y5) \
    $Notebook($w,x3) $Notebook($w,y5) \
    -width 2 -fill white -tags topline
  set Notebook($w,top) $n
  raise $w.f$n
}

#
# Change the page-specific configuration options for the notebook
#
proc Notebook:pageconfig {w name args} {
  global Notebook
  set i [lsearch $Notebook($w,pages) $name]
  if {$i<0} return
  foreach {tag value} $args {
    switch -- $tag {
      -state {
        if {"$value"=="disabled"} {
          $w itemconfig t$i -fg $Notebook($w,fg,off)
        } else {
          $w itemconfig t$i -fg $Notebook($w,fg,on)
        }
      }
      -onexit {
        set Notebook($w,p$i,onexit) $value
      }
    }
  }
}

#
# This procedure raises a notebook page given its name.  But first
# we check the "onexit" procedure for the current page (if any) and
# if it returns false, we don't allow the raise to proceed.
#
proc Notebook:raise {w name} {
  global Notebook
  set i [lsearch $Notebook($w,pages) $name]
  if {$i<0} return
  if {[info exists Notebook($w,p$i,onexit)]} {
    set onexit $Notebook($w,p$i,onexit)
    if {"$onexit"!="" && [eval uplevel #0 $onexit]!=0} {
      Notebook:raise.page $w $i
    }
  } else {
    Notebook:raise.page $w $i
  }
}

#
# Return the frame associated with a given page of the notebook.
#
proc Notebook:frame {w name} {
  global Notebook
  set i [lsearch $Notebook($w,pages) $name]
  if {$i>=0} {
    return $w.f$i
  } else {
    return {}
  }
}

#
# Try to resize the notebook to the next time we become idle.
#
proc Notebook:scheduleExpand w {
  global Notebook
  if {[info exists Notebook($w,expand)]} return
  set Notebook($w,expand) 1
  after idle "Notebook:expand $w"
}

#
# Resize the notebook to fit inside its containing widget.
#
proc Notebook:expand w {
  global Notebook
  set wi [expr [winfo width $w]-($Notebook($w,pad)*2+4)]
  set hi [expr [winfo height $w]-($Notebook($w,pad)*2+36)]
  Notebook:config $w -width $wi -height $hi
  catch {unset Notebook($w,expand)}
}

