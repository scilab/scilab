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
