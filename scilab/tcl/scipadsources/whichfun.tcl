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

proc countcontlines {w {indstart "1.0"} {indstop "insert"}} {
# Count the continued lines in textarea $w
# Counting is done between the line containing $indstart down to $indstop
# <TODO> improve performance of this proc because it is used each time the
#        user hits a key or clicks in the textarea (called by keyposn)
#        usability problems start from, say, more than 5000 lines in the
#        buffer (.sce files) or in a function

    # regular expression matching a continued line identified as such because
    # it has trailing dots possibly followed by a comment
    set dotcontlineRE {(^([^/]/?)*\.{2,} *(//.*)?$)}

    # regular expression matching a continued line identified as such because
    # it has unbalanced brackets possibly followed by a comment
    set bracketscontlineRE {(^(([^/"']/?)*(["'][^"']*["'])*)*\[[^\]]*(((\[[^\[\]]*\])*[^\]]*)*\n)+(([^/]/?)*\.{2,} *(//.*)?\n)*)}

    # regular expression matching a continued line identified as such because
    # it has unbalanced braces possibly followed by a comment
    set bracescontlineRE   {(^(([^/"']/?)*(["'][^"']*["'])*)*\{[^\}]*(((\{[^\{\}]*\})*[^\}]*)*\n)+(([^/]/?)*\.{2,} *(//.*)?\n)*)}

    set contlineRE "$bracketscontlineRE|$bracescontlineRE|$dotcontlineRE"

    set contlines 0
    set indstop [$w index "$indstop linestart"]
    set ind [$w index "$indstart linestart"]

    while {[regexp -line -indices -- $contlineRE [$w get $ind $indstop] resi]} {
        foreach {i j} $resi {}
        set matchstart [$w index "$ind + $i c"]
        set matchlength [expr $j - $i + 1]
        set matchtext [$w get $matchstart "$matchstart + $matchlength c"]
        set nbcontlines [regexp -all -- {\n} $matchtext]
        if {$nbcontlines == 0} {
            # the line matches $dotcontlineRE, and with the -line option, this
            # regexp does not match the trailing \n
            set nbcontlines 1
        }
        incr contlines $nbcontlines
        set ind [$w index "$matchstart + $matchlength c + 1 l linestart"]
    }
    return $contlines
}

proc iscontinuedline {textarea ind} {
# return true if line containing index $ind is a continued line
# return false otherwise
    set i1 [$textarea index "$ind linestart"]
    set i2 [$textarea index "$ind linestart + 1l"]
    set nbcl [countcontlines $textarea $i1 $i2]
    if {$nbcl == 0} {
        return false
    } elseif {$nbcl == 1} {
        return true
    } else {
        # shouldn't happen
        return "error"
    }
}

proc getstartofcontline {textarea ind} {
# if line at $ind is a continued line
#    return index linestart of the continued line containing $ind
# otherwise
#    return "$ind linestart"
    set nclup 0
    while {[iscontinuedline $textarea "$ind - $nclup l"]} {
        incr nclup
        if {[$textarea compare "$ind - $nclup l linestart" == "1.0"]} {
            break
        }
    }
    return [$textarea index "$ind + 1 l - $nclup l linestart"]
}

proc getendofcontline {textarea ind} {
# if line at $ind is a continued line
#    return index lineend of the continued line containing $ind
# otherwise
#    return "$ind lineend"
    set ncldown 0
    while {[iscontinuedline $textarea "$ind + $ncldown l"]} {
        incr ncldown
        if {[$textarea compare "$ind + $ncldown l lineend" >= "end"]} {
            break
        }
    }
    return [$textarea index "$ind + $ncldown l lineend"]
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
# Get all the functions defined in the given textarea
# Return a list {$buf $result_of_whichfun}
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

    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }

    # return if the language is anything but scilab
    if {$listoffile("$textarea",language) != "scilab"} {
        return {$textarea { "0NoFunInBuf" 0 0 }}
    }

    set hitslist ""
    set nextfun [$textarea search -exact -forwards -regexp\
                 "\\mfunction\\M" 1.0 end ]
    while {$nextfun != ""} {
        while {[lsearch [$textarea tag names $nextfun] "textquoted"] != -1 || \
               [lsearch [$textarea tag names $nextfun] "rem2"] != -1 } {
            set nextfun [$textarea search -exact -forwards -regexp\
                         "\\mfunction\\M" "$nextfun +8c" end]
            if {$nextfun == ""} break
        }
        if {$nextfun != ""} {
            set infun [whichfun [$textarea index "$nextfun +1c"] $textarea]
        } else {
            set infun {}
        }
        if {$infun != {} } {
            set hitslist "$hitslist [lindex $infun 0] {[lindex $infun 2]} [lindex $infun 3]"
            set nextfun [$textarea search -exact -forwards -regexp\
                         "\\mfunction\\M" "$nextfun +8c" end]
        }
    }
    if {$hitslist == ""} {
        return [list $textarea [list "0NoFunInBuf" 0 0]]
    } else {
        return [list $textarea $hitslist]
    }
}
