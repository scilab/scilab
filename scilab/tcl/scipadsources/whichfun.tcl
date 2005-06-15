proc whichfun {indexin {buf "current"}} {
    global listoffile
    # it is implicitely meant that indexin refers to a position in buf,
    # i.e. in textareacur if buf is not passed to whichfun

    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }

    # return {} if the language is anything but scilab
    if {$listoffile("$textarea",language) != "scilab"} {return {}}

    set indexin [$textarea index $indexin]
    scan $indexin "%d.%d" ypos xpos

    set lfunpos [list]
    set curpos 1.0
    set amatch "firstloop"
    while {$amatch != ""} {
        # search for the next "function" or "endfunction" which is not in a
        # comment nor in a string
        set amatch [$textarea search -exact -regexp "\\m(end)?function\\M" $curpos $indexin]
        if {$amatch!=""} {
            while {[lsearch [$textarea tag names $amatch] "textquoted"] !=-1 || \
                   [lsearch [$textarea tag names $amatch] "rem2"      ] !=-1} {
                set amatch [$textarea search -exact -regexp "\\m(end)?function\\M" "$amatch+8c" $indexin]
                if {$amatch==""} break
            }
        }
        if {$amatch!=""} {
            if {[$textarea get $amatch] == "e"} {
                # "endfunction" found
                if {![$textarea compare "$indexin-11c" < $amatch]} {
                    # the 'if' above is to include the "endfunction" word
                    # into the core of the function
                    set lfunpos [lreplace $lfunpos end end]
                }
            } else {
                # "function" found
                lappend lfunpos $amatch
            }
            set curpos "$amatch+1c"
        }
    }

    set insidefun 1
    if {$lfunpos == [list]} {
        set insidefun 0
    } else { 
        set precfun [lindex $lfunpos end]
        set i1 [$textarea index "$precfun+8c"]
        # look for the end of the function line definition, taking into account
        # continued lines and possible comments after the trailing dots
        # comments are removed from the function definition, and continued
        # lines are concatenated to form a single line
        set i2 [$textarea index "$precfun lineend"]
        set firstcommchar [lindex [$textarea tag nextrange "rem2" $i1 $i2] 0]
        if {$firstcommchar == ""} {
            set firstcommchar $i2
        }
        set funline [$textarea get $i1 $firstcommchar]
        set funline [string trim $funline]
        set lineend "[string index $funline end-1][string index $funline end]"
        while {$lineend == ".."} {
            # this is a continued line
            set funline [string trimright $funline "."]
            set i1 "$i2 + 1 line linestart"
            set i2 [$textarea index "$i2 + 1 line lineend"]
            set firstcommchar [lindex [$textarea tag nextrange "rem2" $i1 $i2] 0]
            if {$firstcommchar == ""} {
                set firstcommchar $i2
            }
            set contline [$textarea get "$i2 linestart" $firstcommchar]
            set contline [string trim $contline]
            set funline "$funline$contline"
            set lineend "[string index $funline end-1][string index $funline end]"
        }
        # find the function name, excluding too pathological cases
        set funname ""
#        set funpat  "\[\%\#\]*\\m\[\\w%\#\]*\\M\[%\#\]*"
        set funpat {[\%\#\!\$\?]*\m[\w\#\!\$\?]*\M[\#\!\$\?]*}
        if {[set i3 [string first "=" $funline]] !={}} {
            regexp -start [expr $i3+1] $funpat $funline funname  
        } else {
            regexp  $funpat $funline funname  
        }
        if {$funname==""} {set insidefun 0}
    }

    if {$insidefun == 0} {
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
                    [lsearch $dottedlineslist [$textarea index "$ind linestart"]] == -1 &&
                    [lsearch [$textarea tag names $ind] "textquoted"] ==-1 } {
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
#        tk_messageBox -message [concat \
#                               [mc "Being at line"] $ypos \
#                               [mc ", function"] $funname \
#                               [mc "begins at"] $precfun \
#                               [mc ", and there are"] $contlines \
#                               [mc "continued lines, i.e. we are at line"] $lineinfun \
#                               [mc "of"] $funname "\n$funline"]
        return [list $funname $lineinfun $funline $precfun $contlines] 
    }
}

proc showwhichfun {} {
    set textarea [gettextareacur]
    set infun [whichfun [$textarea index insert]]
    if {$infun !={} } {
        set funname [lindex $infun 0]
        set lineinfun [lindex $infun 1]
        tk_messageBox -message [concat [mc "Function"] $funname [mc "line"] $lineinfun] \
                      -title [mc "Which function?"]
    } else {
        tk_messageBox -message [mc "The cursor is not currently inside a function body."] \
                      -title [mc "Which function?"]
    }
}
