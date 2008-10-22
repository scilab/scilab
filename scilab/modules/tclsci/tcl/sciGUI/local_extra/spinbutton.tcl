#
# Spinbutton Widget
#              by Waster
#
# Mail :   waster@iname.com
# ICQ  :   27216346
#

set spinbutton(win) ""

#
# These four procedures are for
# disabling/enabling buttons
#
proc spinbutton:upenable {w} {
global spinbutton
  set spinbutton($w,updisabled) 0
  $w.f.up delete all
  $w.f.up create line 5 1 8 8 -fill #a0a0a0
  $w.f.up create line 5 1 9 8 -fill #000000
  $w.f.up create line 2 7 8 7 -fill #a0a0a0
  $w.f.up create line 2 8 5 2 -fill #e0e0e0
  $w.f.up create line 1 8 5 1 -fill #ffffff
  $w.f.up create line 2 8 9 8 -fill #000000
  bind $w.f.up <Button-1> "set spinbutton($w,ptime) 1;set spinbutton($w,press) 1; spinbutton:uppress $w"
  bind $w.f.up <ButtonRelease-1> "set spinbutton($w,press) 0; spinbutton:uprelease $w"
}

proc spinbutton:updisable {w} {
global spinbutton
  set spinbutton($w,updisabled) 1
  $w.f.up delete all
  $w.f.up create line 5 1 9 8 -fill #606060
  $w.f.up create line 1 8 5 1 -fill #e0e0e0
  $w.f.up create line 2 8 9 8 -fill #606060
  bind $w.f.up <Button-1> ""
  bind $w.f.up <ButtonRelease-1> ""
}

proc spinbutton:downenable {w} {
global spinbutton
  set spinbutton($w,downdisabled) 0
  $w.f.down delete all
  $w.f.down create line 5 8 8 1 -fill #000000
  $w.f.down create line 5 8 9 1 -fill #000000
  $w.f.down create line 2 2 8 2 -fill #c0c0c0
  $w.f.down create line 2 1 5 7 -fill #e0e0e0
  $w.f.down create line 1 1 5 8 -fill #c0c0c0
  $w.f.down create line 2 1 9 1 -fill #e0e0e0
  bind $w.f.down <Button-1> "set spinbutton($w,ptime) 1; set spinbutton($w,press) -1; spinbutton:downpress $w"
  bind $w.f.down <ButtonRelease-1> "set spinbutton($w,press) 0; spinbutton:downrelease $w"
}

proc spinbutton:downdisable {w} {
global spinbutton
  set spinbutton($w,downdisabled) 1
  $w.f.down delete all
  $w.f.down create line 5 8 9 1 -fill #606060
  $w.f.down create line 1 1 5 8 -fill #e0e0e0
  $w.f.down create line 2 1 9 1 -fill #606060
  bind $w.f.down <Button-1> ""
  bind $w.f.down <ButtonRelease-1> ""
}

#
# This procedure is called whenever
# the user presses enter in the entry
# or a .path set ?var? command is called
#
proc spinbutton:pressenter {w} {
global spinbutton
  set a [$w.ent get]
  if {$a==""} {$w.ent insert end $spinbutton($w,val); return}

  set b [string index $a 0]
  set a [string range $a 1 end]

  if {[string trim $b {0 1 2 3 4 5 6 7 8 9 - +}]!="" || [string trim $a {0 1 2 3 4 5 6 7 8 9}]!=""} {
    $w.ent delete 0 end; $w.ent insert end $spinbutton($w,val); return}

  set a [expr $b$a]    ;#We use this line to delete any existing +'s

  if {$spinbutton($w,max)!="" && $a>$spinbutton($w,max)} {set a $spinbutton($w,max)}
  if {$spinbutton($w,min)!="" && $a<$spinbutton($w,min)} {set a $spinbutton($w,min)}
  if {$a==$spinbutton($w,max)} {spinbutton:updisable $w}
  if {$a==$spinbutton($w,min)} {spinbutton:downdisable $w}
  if {$spinbutton($w,updisabled)==1 && $a<$spinbutton($w,max)} {spinbutton:upenable $w}
  if {$spinbutton($w,downdisabled)==1 && $a>$spinbutton($w,min)} {spinbutton:downenable $w}

  set spinbutton($w,val) $a
  $w.ent delete 0 end
  $w.ent insert end $a

  eval $spinbutton($w,exec)
}

#
# These four procedures are called whenever
# user presses one of the buttons
#
proc spinbutton:uppress {w} {
global spinbutton
  if {$spinbutton($w,press)!="1"} {bind $w.f.up <Button-1> "set spinbutton($w,ptime) 1;set spinbutton($w,press) 1; spinbutton:uppress $w";return}

  if {$spinbutton($w,ptime)<="1"} {
  $w.f.up delete all  
  $w.f.up create line 5 1 8 8 -fill #e0e0e0
  $w.f.up create line 5 1 9 8 -fill #ffffff
  $w.f.up create line 2 7 8 7 -fill #e0e0e0
  $w.f.up create line 2 8 5 2 -fill #a0a0a0
  $w.f.up create line 1 8 5 1 -fill #000000
  $w.f.up create line 2 8 9 8 -fill #ffffff
  }

  incr spinbutton($w,val)
  $w.ent delete 0 end
  $w.ent insert 0 $spinbutton($w,val)

  if {$spinbutton($w,downdisabled)==1 && $spinbutton($w,min)<$spinbutton($w,val)} {spinbutton:downenable $w}
  if {$spinbutton($w,max)==$spinbutton($w,val)} {spinbutton:updisable $w; eval $spinbutton($w,exec); return}

  if {$spinbutton($w,ptime)==1} {bind $w.f.up <Button-1> ""; after 500 "spinbutton:uppress $w"} else {after 10 "spinbutton:uppress $w"}
  incr spinbutton($w,ptime)
  eval $spinbutton($w,exec)
}

proc spinbutton:uprelease {w} {
global spinbutton
  set spinbutton($w,press) 0
  $w.f.up delete all
  $w.f.up create line 5 1 8 8 -fill #a0a0a0
  $w.f.up create line 5 1 9 8 -fill #000000
  $w.f.up create line 2 7 8 7 -fill #a0a0a0
  $w.f.up create line 2 8 5 2 -fill #e0e0e0
  $w.f.up create line 1 8 5 1 -fill #ffffff
  $w.f.up create line 2 8 9 8 -fill #000000
}

proc spinbutton:downpress {w} {
global spinbutton
  if {$spinbutton($w,press)!="-1"} {bind $w.f.down <Button-1> "set spinbutton($w,ptime) 1; set spinbutton($w,press) -1; spinbutton:downpress $w";return}
  if {$spinbutton($w,ptime)<="1"} {
  set wdown $w.f.down
  $w.f.down delete all
  $w.f.down create line 5 8 8 1 -fill #ffffff
  $w.f.down create line 5 8 9 1 -fill #ffffff
  $w.f.down create line 2 2 8 2 -fill #a0a0a0
  $w.f.down create line 2 1 5 7 -fill #000000
  $w.f.down create line 1 1 5 8 -fill #a0a0a0
  $w.f.down create line 2 1 9 1 -fill #000000
  }
  set spinbutton($w,val) [expr $spinbutton($w,val)-1]
  $w.ent delete 0 end
  $w.ent insert 0 $spinbutton($w,val)

  if {$spinbutton($w,updisabled)==1 && $spinbutton($w,max)>$spinbutton($w,val)} {spinbutton:upenable $w}
  if {$spinbutton($w,min)==$spinbutton($w,val)} {spinbutton:downdisable $w; eval $spinbutton($w,exec); return}

  if {$spinbutton($w,ptime)==1} {bind $w.f.down <Button-1> ""; after 500 "spinbutton:downpress $w";} else {after 10 "spinbutton:downpress $w"}
  incr spinbutton($w,ptime)
  eval $spinbutton($w,exec)
}

proc spinbutton:downrelease {w} {
global spinbutton
  set spinbutton($w,press) 0
  $w.f.down delete all
  $w.f.down create line 5 8 8 1 -fill #000000
  $w.f.down create line 5 8 9 1 -fill #000000
  $w.f.down create line 2 2 8 2 -fill #c0c0c0
  $w.f.down create line 2 1 5 7 -fill #e0e0e0
  $w.f.down create line 1 1 5 8 -fill #c0c0c0
  $w.f.down create line 2 1 9 1 -fill #e0e0e0
}

#
# This procedure creates a new spinbutton widget
#
proc spinbutton {w args} {
global spinbutton

  if {[lsearch $spinbutton(win) $w]!=-1} {error "$w already exists"; return}
  lappend spinbutton(win) $w
  set spinbutton($w,val) 0
  set spinbutton($w,press) 0  ;#"-1" if down, "1" if up, "0" if none
  set spinbutton($w,ptime) 1
  set spinbutton($w,updisabled) 0
  set spinbutton($w,downdisabled) 0
  set spinbutton($w,max) ""
  set spinbutton($w,min) ""
  set spinbutton($w,exec) ""

  frame $w
  entry $w.ent -width 3 -justify right
  $w.ent insert end 0
  frame $w.f
  canvas $w.f.up -width 9 -height 9 -borderwidth 0 -highlightthickness 0
  canvas $w.f.down -width 9 -height 9 -borderwidth 0 -highlightthickness 0

  pack $w.ent -side left -expand 1 -fill x
  pack $w.f -side left
  pack $w.f.up
  pack $w.f.down

  $w.f.up create line 5 1 8 8 -fill #a0a0a0
  $w.f.up create line 5 1 9 8 -fill #000000
  $w.f.up create line 2 7 8 7 -fill #a0a0a0
  $w.f.up create line 2 8 5 2 -fill #e0e0e0
  $w.f.up create line 1 8 5 1 -fill #ffffff
  $w.f.up create line 2 8 9 8 -fill #000000
  $w.f.down create line 5 8 8 1 -fill #000000
  $w.f.down create line 5 8 9 1 -fill #000000
  $w.f.down create line 2 2 8 2 -fill #c0c0c0
  $w.f.down create line 2 1 5 7 -fill #e0e0e0
  $w.f.down create line 1 1 5 8 -fill #c0c0c0
  $w.f.down create line 2 1 9 1 -fill #e0e0e0

  bind $w.ent <Return> "spinbutton:pressenter $w"
  bind $w.f.up <Button-1> "set spinbutton($w,ptime) 1;set spinbutton($w,press) 1;spinbutton:uppress $w"
  bind $w.f.up <ButtonRelease-1> "set spinbutton($w,press) 0; set spinbutton($w,ptime) 1; set spinbutton($w,press) 0; spinbutton:uprelease $w"
  bind $w.f.down <Button-1> "set spinbutton($w,ptime) 1;set spinbutton($w,press) -1;spinbutton:downpress $w"
  bind $w.f.down <ButtonRelease-1> "set spinbutton($w,press) 0; set spinbutton($w,ptime) 1; set spinbutton($w,press) 0; spinbutton:downrelease $w"

  # We shouldn't leave any data behind us...
  bind $w <Destroy> "spinbutton:destroy $w"

  spinbutton:createproc $w
  eval $w configure $args
}

proc spinbutton:destroy {w} {
global spinbutton
catch {rename spinbutton:$w ""}
  set index [lsearch $spinbutton(win) $w]
  set spinbutton(win) [lreplace $spinbutton(win) $index $index]
  unset spinbutton($w,val)
  unset spinbutton($w,press)
  unset spinbutton($w,ptime)
  unset spinbutton($w,updisabled)
  unset spinbutton($w,downdisabled)
  unset spinbutton($w,max)
  unset spinbutton($w,min)
  unset spinbutton($w,exec)
}



#
# This procedure makes a command named $window
# (.n configure...) for the spinbutton
#
proc spinbutton:createproc {w} {
global spinbutton

  rename $w spinbutton:$w   ;#We change the name of the window/frame (.n) to (spinbutton:.n)

  set start "\{"
  set end "\}"
  set prefix "set w $w\n"   ;# Here we set the variable w to $w... $w will be automatically replaced because of the " chars
  set main {                 #We start the procedure here...
            global spinbutton

            switch -- $command {
              get {return [$w.ent get]}
              set {$w.ent delete 0 end; $w.ent insert 0 $args; spinbutton:pressenter $w}
              configure {
                  foreach {tag value} $args {
                      switch -- $tag {
                          -limit {set min [lindex $value 0]
                                  set max [lindex $value 1]
                                  if {$max>=$min} {set spinbutton($w,max) $max
                                                   set spinbutton($w,min) $min
                                                   if {$spinbutton($w,val)<$min} {set spinbutton($w,val) $min; $w.ent delete 0 end; $w.ent insert end $min; spinbutton:downdisable $w}
                                                   if {$spinbutton($w,val)>$max} {set spinbutton($w,val) $max; $w.ent delete 0 end; $w.ent insert end $max; spinbutton:updisable $w}
                                                   }
                                  }
                          -relief {eval spinbutton:$w configure -relief $value}
                          -borderwidth {eval spinbutton:$w configure -borderwidth $value; $w.ent configure -borderwidth $value}
                          -selectborderwidth {eval spinbutton:$w configure -selectborderwidth $value}
                          -highlightthickness {eval spinbutton:$w configure -highlighthickness $value}
                          -cursor {eval spinbutton:$w configure -cursor $value}
                          -width {eval $w.ent configure -width $value}
                          -justify {eval $w.ent configure -justify $value}
                          -takefocus {eval $w.ent configure -takefocus $value}
                          -insertontime {eval $w.ent configure -insertontime $value}
                          -insertofftime {eval $w.ent configure -insertofftime $value}
                          -insertbackground {eval $w.ent configure -insertbackground $value}
                          -foreground {eval $w.ent configure -foreground $value}
                          -font {eval $w.ent configure -font $value}
                          -background {eval spinbutton:$w configure -background $value}
                          -entbackground {eval $w.ent configure -background $value}
                          -highlightbackground {eval $w.ent configure -highlightbackground $value}
                          -highlightcolor {eval $w.ent configure -highlightcolor $value}
                          -exportselection {eval $w.ent configure -exportselection $value}
                          -selectforeground {eval $w.ent configure -selectforeground $value}
                          -selectbackground {eval $w.ent configure -selectbackground $value}
                          -insertwidth {eval $w.ent configure -insertwidth $value}
                          -insertborderwidth {eval $w.ent configure -insertborderwidth $value}
                          -execute {set spinbutton($w,exec) $value}
                          -state {if {$value=="disabled"} {
                                             spinbutton:updisable $w
                                             spinbutton:downdisable $w
                                             $w.ent configure -state disabled}
                                  if {$value=="normal"} {
                                             $w.ent configure -state normal
                                             $w set [$w get]}}
                          }
                      }
                 }
            }
       }   ;#We finish the procedure

  eval proc $w {{command args}} $start$prefix$main$end

}

