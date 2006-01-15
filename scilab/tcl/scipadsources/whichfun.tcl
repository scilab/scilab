proc whichfun {indexin {buf "current"}} {
# This proc checks whether the text position $indexin provided in buffer $buf
# is in a function or not.
# If no  : return an empty list
# If yes : return a list with the following data:
#       $funname   : function name
#       $lineinfun : logical line number of $indexin in function $funname
#       $funline   : definition line of the function, e.g. [a,b]=foo(c,d)
#       $precfun   : physical line number where $funname is defined
#       $contlines : number of continued lines between $precfun and $indexin
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
        # endfunction was found above, therefore we're not in a function
        set insidefun 0
    } else {
        # function was found above, therefore we are in a function
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
        # check how many continued (.. or unmatched bracket or brace in a single
        # physical line) lines between $indexin and $precfun, and derive the
        # current logical line within the function definition
        set contlines [countcontlines $textarea $precfun $indexin]
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

proc tagcontlinesinallbuffers {} {
    global listoftextarea
    foreach w $listoftextarea {
        tagcontlines $w
    }
}

proc tagcontlines {w} {
# detect and tag continued lines in the entire textarea $w
# the whole line must be tagged, from the first character until and including
# the \n at the end so that proc getstartofcontline and getendofcontline work
# as expected
# this proc also takes care of the visual appearance of continued lines

    global showContinuedLines listoffile
    global bgcolors
    foreach c1 $bgcolors {global $c1}
    global dotcontlineRE bracketscontlineRE bracescontlineRE

    # don't tag anything if the language is not scilab
    if {$listoffile("$w",language) != "scilab"} {
        $w tag remove contline 1.0 end
        return
    }

    set contlineRE "$bracketscontlineRE|$bracescontlineRE|$dotcontlineRE"

    $w tag remove contline 1.0 end
    set ind "1.0"
    set previ 0
    set cont [regexp -all -inline -line -indices -- $contlineRE [$w get "1.0" end]]
    foreach fullmatch $cont {
        foreach {i j} $fullmatch {}
        # what really cuts down performance is to do [$w index "1.0 + $i c"]
        # many times with $i being large (>100000, which is fairly common
        # for say 15000 lines buffers), thus avoid to do it at all and use
        # position differences [expr $j - $i] instead
        # For a 15000 lines test buffer performance gain factor is around 20:1
        # depending on the buffer content
        set mi [$w index "$ind + [expr $i - $previ] c"]
        set mj [$w index "$mi + [expr $j - $i] c"]
        $w tag add contline "$mi linestart" "$mj lineend + 1 c"
        set ind [$w index "$ind + [expr $i - $previ] c"]
        set previ $i
    }
    if {$showContinuedLines} {
        $w tag configure contline -background $CONTLINECOLOR
    } else {
        $w tag configure contline -background {}
    }
    $w tag lower contline
}

proc createnestregexp {nestlevel opdel cldel} {
# Create a regular expression able to match $nestlevel levels of nested
# items enclosed in balanced $opdel (open delimiter) and $cldel close
# delimiter
# Any level of nesting is achievable through $nestlevel (but not an
# arbitrary level), but performance has to be considered
# Note: the regexp returned uses only non reporting parenthesis, which is
# required to properly match continued lines (proc tagcontlines), but also
# to avoid a huge performance impact if sub-matches would be reported
# This proc allows to match for instance (brackets are the delimiters, $nestlevel is > 2):
# [ you [simply [ can't] match [arbitrarily nested] constructs [with regular expressions]]]

    set op "\\$opdel"
    set nodel "\[^\\$opdel\\$cldel\]*"
    set cl "\\$cldel"

    set RE {}
    append RE $op $nodel

    for {set i 2} {$i <= $nestlevel} {incr i} {
        append RE {(?:} {(?:} $op $nodel
    }

    for {set i 2} {$i <= $nestlevel} {incr i} {
        append RE $cl {)*} $nodel {)+}
    }

    append RE $cl

    return $RE
}

proc countcontlines {w {indstart "1.0"} {indstop "insert"}} {
# Count the continued lines in textarea $w
# Counting is done between the line containing $indstart (included) down to
# $indstop (included)
# continued lines are supposed to have been tagged previously
    set taggedcl [$w tag ranges contline]
    set contlines 0
    set indstart [$w index "$indstart linestart"]
    set indstop  [$w index "$indstop  lineend  "]
    foreach {i1 i2} $taggedcl {
        if {[$w compare $i1 > $indstop]} {
            break
        }
        if {[$w compare $i1 >= $indstart]} {
            # include the last line (untagged) of the continued line
            set i2 [$w index "$i2 lineend"]
            if {[$w compare $i2 > $indstop]} {
                set i2 $indstop
            }
            incr contlines [regexp -all -- {\n} [$w get $i1 $i2]]
        }
    }
    return $contlines
}

proc iscontinuedline {textarea ind} {
# return true if line containing index $ind is a continued line
# return false otherwise
    if {[lsearch [$textarea tag names $ind] contline] != -1} {
        return true
    } else {
        return false
    }
}

proc getstartofcontline {textarea ind} {
# if line at $ind is a continued line
#    return index linestart of the continued line containing $ind
# otherwise
#    return "$ind linestart"
    if {[lsearch [$textarea tag names $ind] contline] != -1} {
        # + 1 c below to deal with the case where $ind is at the beginning
        # of a line ($textarea tag prevrange contline $ind would return the
        # previous range, not the one starting at $ind) - since the trailing
        # \n is also tagged as contline, it works for any $ind position in
        # the line
        return [lindex [$textarea tag prevrange contline "$ind + 1 c"] 0]
    } else {
        return [$textarea index "$ind linestart"]
    }
}

proc getendofcontline {textarea ind} {
# if line at $ind is a continued line
#    return index lineend of the continued line containing $ind
#    the real end of the continued line is returned, i.e. it includes
#    the last line of the continued line (this physical line is not
#    tagged as a continued line)
# otherwise
#    return "$ind lineend"
    if {[lsearch [$textarea tag names $ind] contline] != -1} {
        # + 1 c below to deal with the case where $ind is at the beginning
        # of a line ($textarea tag prevrange contline $ind would return the
        # previous range, not the one starting at $ind) - since the trailing
        # \n is also tagged as contline, it works for any $ind position in
        # the line
        return [$textarea index \
                "[lindex [$textarea tag prevrange contline "$ind + 1 c"] 1] \
                 lineend"]
    } else {
        return [$textarea index "$ind lineend"]
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

proc getallfunsinalltextareas {} {
# Get all the functions defined in all the opened textareas
# (those that have scilab as scheme)
# Result is a string with getallfunsintextarea results:
# "{textarea1 { $funname11 $funline11 $precfun11  $funname12 $funline12 $precfun12  ... }}
#  {textarea2 { $funname21 $funline21 $precfun21  ... }}
# "
    global listoftextarea
    set hitslist ""
    foreach textarea $listoftextarea {
        set hitslistinbuf [getallfunsintextarea $textarea]
        set hitslist "$hitslist $hitslistinbuf"
    }
    return $hitslist
}

proc getallfunsintextarea {{buf "current"}} {
# Get all the functions defined in the given textarea (only if scheme is scilab)
# Return a list {$buf $result_of_whichfun}
# Continued lines and comments are trimmed so that the function definition line
# returned constitutes a single line
# If there is no function in $buf, then $result_of_whichfun is the following list:
#   { "0NoFunInBuf" 0 0 }
# Note that the leading zero in "0NoFunInBuf" is here so that the latter cannot
# be a valid function name in Scilab (they can't start with a number)
# If there is at least one function definition in $buf, then $result_of_whichfun
# is a list of proc whichfun results:
#   { $funname1 $funline1 $precfun1  $funname2 $funline2 $precfun2  ... }
#       $funname   : function name
#       $funline   : definition line of the function, e.g. [a,b]=foo(c,d)
#       $precfun   : physical line number where $funname is defined in $buf

    global listoffile
    global funlineREpat1 funlineREpat2 scilabnameREpat

    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }

    # return if the language is anything but scilab
    if {$listoffile("$textarea",language) != "scilab"} {
        return {$textarea { "0NoFunInBuf" 0 0 }}
    }

    set pat "$funlineREpat1$scilabnameREpat$funlineREpat2"

    set hitslist ""

    set allfun [regexp -all -inline -indices -- $pat [$textarea get "1.0" end]]

    foreach {fullmatch funname} $allfun {
        foreach {i j} $fullmatch {}
        set star [$textarea index "1.0 + $i c"]
        if {[lsearch [$textarea tag names $star] "textquoted"] == -1 && \
            [lsearch [$textarea tag names $star] "rem2"] == -1 } {
            # whichfun trims continued lines and comments
            set infun [whichfun [$textarea index "$star +1c"] $textarea]
            if {$infun != {} } {
                set hitslist "$hitslist [lindex $infun 0] {[lindex $infun 2]} [lindex $infun 3]"
            }
        }
    }

    if {$hitslist == ""} {
        return [list $textarea [list "0NoFunInBuf" 0 0]]
    } else {
        return [list $textarea $hitslist]
    }
}
