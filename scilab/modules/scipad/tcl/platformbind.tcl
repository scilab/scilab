#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#
# We have found that some key sequences are given a variety of names
#  on different platforms. E.g. Shift-F12 can be called XF86_Switch_VT_12
#  on most unices, or even Shift-SunF37 on Suns. Therefore, when we need to
#  bind these sequences, we try and catch all the known possibilities,
#  using pbind instead of bind. The lists of possible names are defined in 
#  commonbindings.tcl.
proc pbind {w events action} {
    foreach e $events {
        catch {bind $w <$e> $action} 
    }
} 


##################################################
# procedures for supporting several switchable sets of bindings
##################################################
proc loadbindings {} {
    global binddir bindentry pad
    set BindFiles [lsort [globtails $binddir *.tcl]]
    foreach m $BindFiles {
        set bindentry [file rootname $m]
        # for some reason $bindentry might be empty under cygwin
        # this is probably due to the /cygdrive/ syntax for paths
        if {$bindentry != ""} {
            set bindset($bindentry) {}
            source [file join $binddir $m]
        }
    }
}

#each binding file contains sbind as a drop-in replacement of bind
proc sbind {scope event action} {
    global pad bindset
    global bindentry; # a little dirty to pass it as a global
    lappend bindset($bindentry) [list $scope $event $action]
}


proc rebind {} {
    global bindset bindstyle
# first null all bindings defined for all the other styles 
    set allstyles [array names bindset]
    foreach i $allstyles {
        if {$i != $bindstyle} {
            foreach b $bindset($i) {
                set scope [lindex $b 0]
                set event [lindex $b 1]
                bind $scope $event {}
            }
        }
    }

# then effect the current ones
    foreach b $bindset($bindstyle) {
                set scope [lindex $b 0]
                set event [lindex $b 1]
                set action [lindex $b 2]
                bind $scope $event $action
    }

#finally redraw the menues, so that the accelerators are updated
    createmenues
    setdbmenuentriesstates_bp
}

proc findbinding {command} {
    global bindset bindstyle
    set event ""
    foreach b $bindset($bindstyle) {
        if  {[lindex $b 2] == $command} {
            set event [lindex $b 1]
            break
        }
    }
    return $event
}

proc bindenable {tag command} {
    set b [findbinding $command]
    if {$b !=""} {
        bind $tag $b "$command"
    }
}

proc binddisable {tag command} {
    set b [findbinding $command]
    if {$b !=""} {
        bind $tag $b {}
    }
}

proc validpostfix {char status options} {
#      puts "$char $status"
    if {[regexp $char $options] & [expr {!($status & 252)}]} {
          showinfo " "
      } else {
          showinfo [mc "Invalid keyboard sequence!"]        
          bell; 
      }
}
