proc whichfun {indexin {buf "current"}} {
    global lang listoffile
#it is implicitely meant that indexin refers to a position in textareacur
# FV 13/05/04, added capability for looking in a buffer which is not the current one
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    # ES 29/5/04: return {} if the language is anything but scilab
    if {$listoffile("$textarea",language) != "scilab"} {return {}}
    scan $indexin "%d.%d" ypos xpos
# search for the previous "function" which is not in a comment nor
# in a string
    set precfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mfunction\\M" $indexin 0.0]
    if {$precfun!=""} {
# FV 13/05/04, changed ==1 to !=-1 (twice) to take breakpoint tag into account
        while {[lsearch [$textarea tag names $precfun] "textquoted"] !=-1 | \
	       [lsearch  [$textarea tag names $precfun] "rem2"] !=-1} {
          set precfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mfunction\\M" $precfun 0.0]
 	  if {$precfun==""} break
	}
    }
# search for the previous "endfunction" which is not in a comment nor
# in a string
    set precendfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mendfunction\\M" $indexin 0.0]
    if {$precendfun!=""} {
# FV 13/05/04, changed ==1 to !=-1 (twice) to take breakpoint tag into account
        while {[lsearch [$textarea tag names $precendfun] "textquoted"] !=-1 | \
	       [lsearch  [$textarea tag names $precendfun] "rem2"] !=-1} {
          set precendfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mendfunction\\M" $precendfun 0.0]
 	  if {$precendfun==""} break
	}
    }
    set insidefun 1
    if {$precfun == "" | $precendfun > $precfun} {
      set insidefun 0
    } else { 
# find the function name, excluding too pathological cases
      set i1 [$textarea index "$precfun+8c"]
      set i2 [$textarea index "$precfun lineend"]
      set funline [$textarea get $i1 $i2]
      set funname ""
      set funpat  "\[\%\#\]*\\m\[\\w%\#\]*\\M\[%\#\]*"
      if {[set i3 [string first "=" $funline]] !={}} {
	  regexp -start [expr $i3+1] $funpat $funline funname  
      } else {
	  regexp  $funpat $funline funname  
      }
      if {$funname==""} {set insidefun 0}
    }
    if {$insidefun == 0} {
#      tk_messageBox -message \
#	  "The cursor is not currently inside a function body"
	return {}
    } else {
# check how many continued (...) lines between $indexin and $precfun,
#  and derive the current line within the function definition
        set last $precfun
        set contlines 0
        set dottedlineslist {}
        while {[set ind [$textarea search -regexp "\\.{2,} *(//.*)?\\Z"\
			     $last $indexin]] != {}} {
            if {[$textarea compare $ind >= $last] } {
                set last "$ind+1l linestart"
                if { [lsearch [$textarea tag names $ind] "rem2"] ==-1 &&
                     [$textarea compare $last <= $indexin] } {
                    set contlines [expr $contlines+1]
                    set dottedlineslist [linsert $dottedlineslist end \
                                         [$textarea index "$ind linestart"]]
                }
            } else break
        }
        proc checkcontbraceorbracket {precfun indexin textarea openchar closechar dottedlineslist} {
            set last $precfun
            set contlines 0
            while {[set ind [$textarea search $openchar \
                             $last $indexin]] != {}} {
                if {[$textarea compare $ind >= $last] &&
                    [lsearch $dottedlineslist [$textarea index "$ind linestart"]] == -1 } {
                    set ind2 [$textarea search $closechar $ind end]
                    if {$ind2 != {}} {
                        if {[$textarea compare "$ind2 linestart" > "$ind linestart"]} {
                            incr contlines [expr int([$textarea index "$ind2 linestart"]\
                                                   - [$textarea index "$ind linestart"]) ]
                        }
                        if {[$textarea compare $ind2 > $indexin]} {
                            incr contlines [expr int([$textarea index "$indexin linestart"]\
                                                   - [$textarea index "$ind2 linestart"]) ]
                        }
                    }
                    set last "$ind+1l linestart"
                } else break
            }
            return $contlines
        }
        incr contlines [checkcontbraceorbracket $precfun $indexin $textarea "\{" "\}" $dottedlineslist]
        incr contlines [checkcontbraceorbracket $precfun $indexin $textarea "\[" "\]" $dottedlineslist]

        scan $precfun "%d." beginfunline 
	set lineinfun [expr $ypos-$beginfunline-$contlines+1]
# FV 13/05/04, message box commented
#      if {$lang == "eng"} {
#        tk_messageBox -message \
#	   "Being at line $ypos, function $funname begins at $precfun, and there are $contlines continued lines, i.e. we are at line $lineinfun of $funname"
#      } else {
#        tk_messageBox -message \
#	   "Etant à la ligne $ypos, la fonction $funname débute à $precfun, et il y a $contlines lignes multiples, i.e. nous sommes à la ligne $lineinfun de $funname"
#      }
        return [list $funname $lineinfun $funline $precfun] 
    }
}

proc showwhichfun {} {
    global lang
    set textarea [gettextareacur]
    set infun [whichfun [$textarea index insert]]
    if {$infun !={} } {
	set funname [lindex $infun 0]
	set lineinfun [lindex $infun 1]
      if {$lang == "eng"} {
        tk_messageBox -message "Function $funname line $lineinfun"
      } else {
        tk_messageBox -message "Fonction $funname ligne $lineinfun"
      }
    } else {
      if {$lang == "eng"} {
        tk_messageBox -message \
	    "The cursor is not currently inside a function body"
      } else {
        tk_messageBox -message \
	    "Le curseur ne se trouve pas à l'intérieur d'une fonction"
      }
    }
}
