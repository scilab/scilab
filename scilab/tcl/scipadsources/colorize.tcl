#######################################
# added by Matthieu PHILIPPE 21/11/2001
#
proc load_words {} {
        global        words chset env

        set ownpath "$env(SCIPATH)/tcl"
        set type {}
        set col {}
        set f [open $ownpath/words r]
        while {[gets $f line] > 0} {
                if {[lindex $line 0] == {#MODE}} {
                        set type [lindex $line 1]
                } elseif {[lindex $line 0] == {#TAG}} {
                        set col [lindex $line 1]
                        set chset($type.$col) {}
                } else {
                        set ch [string range $line 0 0]
                        append chset($type.$col) $ch
                        set words($type.$col.$ch) $line
                }
        }
}

#ES 2/10/2003
proc remalltags {w begin ende} {
        $w tag remove parenthesis $begin $ende
        $w tag remove bracket $begin $ende
        $w tag remove brace $begin $ende
        $w tag remove punct $begin $ende
        $w tag remove operator $begin $ende
        $w tag remove number $begin $ende
        $w tag remove keywords $begin $ende
        $w tag remove text $begin $ende
        $w tag remove rem2 $begin $ende
        $w tag remove xmltag $begin $ende
        $w tag remove textquoted $begin $ende  
        $w tag remove indentation $begin $ende
            }


proc colorize {w cpos iend} {
# modified for schemas, ES 27/5/04
        global        words chset
        global listoffile 
        set num 0
        set textarea [gettextareacur]
        set schema $listoffile("$textarea",language)
        $w mark set begin "$cpos linestart"
        $w mark set ende "$iend+1l linestart"
        remalltags $w begin ende
# TAGS:
# order matters here - for instance textquoted has to be after operator, so 
# operators are not colorized within strings
#
# punctuation
        if {$schema=="scilab"} {
          $w mark set last begin
          while {[set ind [$w search -regexp {[;,]} last ende]] != {}} {
                if {[$w compare $ind >= last]} {
                        $w mark set last $ind+1c
                        $w tag add punct $ind last
                } else break
          }
        }
# parentheses
        $w mark set last begin
        while {[set ind [$w search -regexp {[()]} last ende]] != {}} {
                if {[$w compare $ind >= last]} {
                        $w mark set last $ind+1c
                        $w tag add parenthesis $ind last
                } else break
        }
# brackets
        $w mark set last begin
        while {[set ind [$w search -regexp {[\[\]]} last ende]] != {}} {
                if {[$w compare $ind >= last]} {
                        $w mark set last $ind+1c
                        $w tag add bracket $ind last
                } else break
        }
#ES: why at all call ":" a "brace? 
# "{}" are anyway parsed above for matching pairs
        $w mark set last begin
        while {[set ind [$w search -regexp {[\{\}:]} last ende]] != {}} {
                if {[$w compare $ind >= last]} {
                        $w mark set last $ind+1c
                        $w tag add brace $ind last
                } else break
        }
##ES: numbers (the regexp can be perfectioned -- it matches e.g. single e6)
# number can contain +-. so follows operator (?)
        $w mark set last begin
        while {[set ind [$w search -regexp {\m\d*\.?\d*[deDE]?\-?\d{1,3}\M} \
                             last ende]] != {}} {
                if {[$w compare $ind >= last]} {
                        $w mark set last $ind+1c
                        $w tag add number $ind last
                } else break
        }
# operators
        if {$schema=="scilab"} {
            $w mark set last begin
            while {[set ind [$w search -regexp {['\.+\-*\/\\\^=\~$|&<>]} \
                             last ende]] != {}} {
                if {[$w compare $ind >= last]} {
                        $w mark set last $ind+1c
                        $w tag add operator $ind last
                } else break
            }
        }
# Scilab keywords
       if {$schema=="scilab"} {
          set sciChset "(\[^A-Za-z0-9_\%\]|^)\[$chset(scilab.col1)\]"
           $w mark set last begin
           while {[set ind [$w search -count num -regexp $sciChset last ende]]\
                   != {}} {
             if {[$w compare $ind >= last]} {
                 set res ""
                 regexp $sciChset [$w get $ind "$ind wordend+1c"] res
                 if {$res != ""} {
                     set num [string length $res]
                     $w mark set last "$ind + $num c"
                     $w mark set next {last wordend}
                     set word [$w get last-1c next]
                     set initial [string range $word 0 0]
                     if {[string first $initial $sciChset]>=0} {
                          if {[lsearch -exact $words(scilab.col1.$initial) \
                                $word] != -1} {
                              $w tag add keywords last-1c next
                          }
                     }
                     $w mark set last next-1c
                 } else {
                     $w mark set last last+1c
                 }
             } else break
           }
        }
# XML (#ES this is a problem as <> are also operators)
        if {$schema=="xml"} {
            $w mark set last begin
            while {[set ind [$w search  -regexp "<" last ende]] != {}} {
              if {[$w compare $ind >= last]} {
                   set res ""
                regexp ">" [$w get $ind end] res
                set num [string length $res]
                $w mark set last "$ind + $num c"
                $w mark set next {last+1c wordend}
                $w tag add xmltag last-1c next+1c
#                $w tag add xmltag $ind last
                $w mark set last next+1c
              } else break
            }
        }
# Text
        $w mark set last begin
        while { [set ind [$w search -count num -regexp \
                          {"[^"]*("|$)} last ende]] != {}} {
              if {[$w compare $ind >= last]} {
                  set res ""
                  $w mark set endetext "$ind lineend"
                  regexp {"[^"]*"} [$w get last endetext] res
                  set num [string length $res]
                  if {$num <= 0} {
                      $w mark set last "$ind + 1c"
                  } else {
                      $w mark set last "$ind + $num c"
                 # textquoted deletes any other tag
                      remalltags $w $ind last
                      $w tag add textquoted $ind last
                  }          
              } else break
         }
# scilab remark
         if {$schema=="scilab"} {
              $w mark set last begin
              while {[set ind [$w search -exact {//} last ende]] != {}} {
                  if {[$w compare $ind >= last]} {
#ES
                      $w mark set last "$ind+1l linestart"
# tags as rem2 only if not already textquoted (does not tag as comment "..//..")
                      if {[lsearch [$w tag names $ind] "textquoted"] ==-1 } {
                 # rem2 deletes any other tag
                           remalltags $w $ind "$ind lineend" 
                           $w tag add rem2 $ind "$ind lineend" }
                  } else break
              }
          }
# anyway, commented textquoted are displayed as rem2
          $w tag raise rem2 textquoted
}


#ES 27/5/04
proc changelanguage {newlanguage} {
    global listoffile lang
    set textarea [gettextareacur]
    set oldlanguage $listoffile("$textarea",language)
    if {$oldlanguage != $newlanguage} {
       set listoffile("$textarea",language) $newlanguage
       if {$lang == "eng"} {
          showinfo "Wait seconds while recolorizing file"
       } else {
          showinfo "Patientez un instant le temps de la colorisation"
       }
       schememenus $textarea
       colorize $textarea 1.0 end
       keyposn $textarea
    }
}

#ES,FV 27/5/04 
proc schememenus {textarea} {
  global pad listoffile
 # set dm $pad.filemenu.debug
  if {$listoffile("$textarea",language) == "scilab"} {
#enable "Load into scilab"
        $pad.filemenu.exec entryconfigure 1 -state normal
#enable all the Debug entries
#        for {set i 1} {$i<=[$dm index last]} {incr i} {
#            if {[$dm type $i] == "command"} {
#               $dm entryconfigure $i -state normal
#            }
#        }
# restore their bindings
    bind $pad <Control-l> {execfile}
    bind $pad <F5> {filetosave %W; execfile}
	bind $pad <F9> {insertremove_bp}
	bind $pad <Control-F9> {removeall_bp}
	bind $pad <F10> {configurefoo_bp}
	bind $pad <Control-F11> {execfile_bp}
	bind $pad <F11> {resume_bp}
	bind $pad <Shift-F11> {insertremovedebug_bp}
	bind $pad <Shift-F12> {goonwo_bp}
	bind $pad <F12> {stepbystep_bp}
    bind $pad <Control-F12> {showwatch_bp}
  } else {
#disable "Load into scilab"
        $pad.filemenu.exec entryconfigure 1 -state disabled
#disable all the Debug entries
#        for {set i 1} {$i<=[$dm index last]} {incr i} {
#            if {[$dm type $i] == "command"} {
#                $dm entryconfigure $i -state disabled
#            }
#        }
# remove their bindings
    bind $pad <Control-l> {}
    bind $pad <F5> {}
	bind $pad <F9> {}
	bind $pad <Control-F9> {}
	bind $pad <F10> {}
	bind $pad <Control-F11> {}
	bind $pad <F11> {}
	bind $pad <Shift-F11> {}
	bind $pad <Shift-F12> {}
	bind $pad <F12> {}
    bind $pad <Control-F12> {}
  }
}
