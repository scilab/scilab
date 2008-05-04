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
proc whichfun {indexin {buf "current"}} {
# This proc checks whether the text position $indexin provided in buffer $buf
# is in a function or not.
# If no  : return an empty list
# If yes : return a list with the following data:
#       $funname   : function name
#       $lineinfun : logical line number of $indexin in function $funname
#       $funline   : definition line of the function, e.g. [a,b]=foo(c,d)
#       $precfun   : index where $funname definition starts (position of the f
#                    of the word "function")
#       $contlines : number of continued lines between $precfun and $indexin
    global listoffile
    global funlineREpat1 funlineREpat2 scilabnameREpat

    # it is implicitely meant that indexin refers to a position in buf,
    # i.e. in textareacur if buf is not passed to whichfun
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }

    # return {} if the language is anything but scilab
    if {$listoffile("$textarea",language) != "scilab"} {return {}}

    # return {} if the colorization of buffer is off
    if {!$listoffile("$textarea",colorize)} {return {}}

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
        # (well, at least a priori - see below)
        set precfun [lindex $lfunpos end]
        # get the full function definition line, including possible
        # continued lines and comments
        set pat "$funlineREpat1$scilabnameREpat$funlineREpat2"
        if {![regexp -- $pat [$textarea get $precfun end] funline]} {
            # special case: the regexp didn't match, which means that the
            # function definition line contains only the word "function",
            # with no function name (unterminated edit for example)
            set insidefun 0
        } else {
            # remove the leading keyword function (8 characters)
            set funline [string range $funline 8 end]
            # remove continued lines tags and comments
            set funline [trimcontandcomments $funline]
            # find function name
            set funname [extractfunnamefromfunline $funline]
            # funname=="" might happen if regexp $pat does not match while function
            # detection above did find the word "function" - really? such a case
            # should have been trapped above by testing [info exists funline], no?
            # anyway, this doesn't harm, so:
            if {$funname==""} {set insidefun 0}
        }
    }

    if {$insidefun == 0} {
        return {}
    } else {
        # check how many continued (.. or unmatched bracket or brace in a single
        # physical line) lines between $indexin and $precfun, and derive the
        # current logical line within the function definition
        set contlines [countcontlines $textarea $precfun $indexin]
        scan $precfun "%d." beginfunline 
        set lineinfun [expr {$ypos - $beginfunline - $contlines + 1}]

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

proc getendfunctionpos {ta precfun {exittypes "only_endfunction"}} {
# look for endfunction corresponding to $precfun (beginning of function
# definition)
# if $exittypes == "only_endfunction":
#    only the position of the endfunction keyword is returned
#    the position returned is the index in $ta of the first n of the word
#    endfunction corresponding to the function definition starting at $precfun
#    if no endfunction corresponding to $precfun is found, then -1 is returned
# otherwise:
#    the proc returns a list of indices, each of them corresponding to an
#    exit point (a return point) of the function starting at $precfun
#    keywords considered as exit points are "return", "resume" and
#    "endfunction". The position returned is the position of the second
#    character of each of these keywords
#    if no endfunction corresponding to $precfun is found, then -1 is returned
#    even if there were return or resume before in the text
    if {$exittypes == "only_endfunction"} {
        set searchpat {\m(end)?function\M}
    } else {
        set searchpat {\m(((end)?function)|(return)|(resume))\M}
    }
    set lfunpos [list $precfun]
    set repos [list ]
    set amatch [$ta search -exact -regexp "\\mfunction\\M" $precfun end]
    set curpos [$ta index "$amatch + 1c"]
    set amatch "firstloop"
    while {[llength $lfunpos] != 0} {
        # search for the next "function" or "endfunction" which is not in a
        # comment nor in a string
        set amatch [$ta search -exact -regexp $searchpat $curpos end]
        if {$amatch != ""} {
            while {[lsearch [$ta tag names $amatch] "textquoted"] !=-1 || \
                   [lsearch [$ta tag names $amatch] "rem2"      ] !=-1} {
                set amatch [$ta search -exact -regexp $searchpat "$amatch+6c" end]
                if {$amatch==""} break
            }
        }
        if {$amatch != ""} {
            if {[$ta get $amatch] == "e"} {
                # "endfunction" found
                if {![$ta compare "end-11c" < $amatch]} {
                    # the 'if' above is to include the "endfunction" word
                    # into the core of the function
                    set lfunpos [lreplace $lfunpos end end]
                }
            } elseif {[$ta get $amatch] == "f"} {
                # "function" found
                lappend lfunpos $amatch
            } else {
                # "return" or "resume" found
                # be clever, and don't include what is in a nested function
                # of the function starting at $precfun (those positions are
                # not return points of function starting at $precfun)
                if {[llength $lfunpos] == 1} {
                    lappend repos [$ta index "$amatch + 1c"]
                }
            }
            set curpos [$ta index "$amatch + 1c"]
        } else {
            set curpos -1
            break
        }
    }
    if {$exittypes == "only_endfunction"} {
        return $curpos
    } else {
        if {$curpos != "-1"} {
            lappend repos $curpos
            return $repos
        } else {
            return $curpos
        }
    }
}

proc tagcontlinesinallbuffers {} {
    global listoftextarea
    foreach w [filteroutpeers $listoftextarea] {
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
        # position differences [expr {$j - $i}] instead
        # of course, bracing expr provides a good amount of performance too
        # For a 15000 lines test buffer performance gain factor is better
        # than 20:1, depending on the buffer content
        set mi [$w index "$ind + [expr {$i - $previ}] c"]
        set mj [$w index "$mi + [expr {$j - $i}] c"]
        $w tag add contline "$mi linestart" "$mj lineend + 1 c"
        set ind $mi
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
# items enclosed in balanced $opdel (open delimiter) and $cldel (close
# delimiter)
# Any level of nesting is achievable through $nestlevel (but not an
# arbitrary level), but performance has to be considered
# Note: the regexp returned uses only non reporting parenthesis, which is
# required to properly match continued lines (proc tagcontlines), but also
# to avoid a huge performance impact if sub-matches would be reported
# This proc allows to match for instance (brackets are the delimiters, $nestlevel is > 2):
# [ you [simply [ can't] match [arbitrarily nested] constructs [with regular expressions]]]

    set qtext {(?:(?:["'][^"']*["'])*)}
    set op "\\$opdel"
    set nodel "(?:\[^\\$opdel\\$cldel\"'\]*|$qtext)"
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
# contrary to proc getrealendofcontline, this proc does not always return
# the real end of the continued line, instead it returns "$ind lineend"
# if line at $ind is not tagged as a continued line 
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

proc getrealstartofcontline {w ind} {
# return start bound of a line
# and take care of nearby continued lines
# this proc is the same as getstartofcolorization
    return [getstartofcolorization $w $ind]
}

proc getrealendofcontline {w ind} {
# return end bound of a line
# and take care of nearby continued lines
# this proc is different from getendofcolorization and also from
# getendofcontline (see comments in the latter)
    return [getendofcolorization $w [$w index "$ind - 1 l"]]
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
# "textarea1 { $funname11 $funline11 $precfun11  $funname12 $funline12 $precfun12  ... }
#  textarea2 { $funname21 $funline21 $precfun21  ... }
#  ...
# "
# The order of the buffers in the output is the order in $listoftextarea,
# which is always the order of opening of the buffers (the order in the
# Windows menu is only a display order)
    global listoftextarea
    set hitslist ""
    foreach textarea [filteroutpeers $listoftextarea] {
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
#   $textarea { "0NoFunInBuf" 0 0 }
# Note that the leading zero in "0NoFunInBuf" is here so that the latter cannot
# be a valid function name in Scilab (they can't start with a number)
# If there is at least one function definition in $buf, then $result_of_whichfun
# is a flat list of proc whichfun results preceded by the textarea name:
#   $textarea { $funname1 $funline1 $precfun1  $funname2 $funline2 $precfun2  ... }
#       $funname   : function name
#       $funline   : definition line of the function, e.g. [a,b]=foo(c,d)
#       $precfun   : physical line number where $funname is defined in $buf
# Note further that the order of the functions returned is the order of their
# definition in the buffer, i.e. the order of the function definition lines
# when reading the buffer from its start to its end (nested functions are not
# special in any respect). This order is important because it is used in
# proc execfile_bp to eliminate nested functions from the list

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

    # return if colorization of the buffer is switched off
    if {!$listoffile("$textarea",colorize)} {
        return {$textarea { "0NoFunInBuf" 0 0 }}
    }

    set pat "$funlineREpat1$scilabnameREpat$funlineREpat2"

    set hitslist ""

    set allfun [regexp -all -inline -indices -- $pat [$textarea get 1.0 end]]

    set ind "1.0"
    set previ 0
    foreach {fullmatch funname} $allfun {
        foreach {i j} $fullmatch {}
        set star [$textarea index "$ind + [expr {$i - $previ}] c"]
        set stop [$textarea index "$star + [expr {$j - $i}] c"]
        if {[lsearch [$textarea tag names $star] "rem2"] == -1} {
            if {[lsearch [$textarea tag names $star] "textquoted"] == -1} {
                # skip the keyword "function" (8 characters)
                set funline [$textarea get "$star + 8 c" $stop]
                # remove continued lines tags and comments
                set funline [trimcontandcomments $funline]
                # get function name
                set funname [extractfunnamefromfunline $funline]
                # get physical line number of function definition
                set funstart [$textarea index "$star linestart"]
                # braces around $funline mandatory because $funline can
                # contain spaces
                set hitslist "$hitslist $funname {$funline} $funstart"
            }
        }
        set ind $star
        set previ $i
    }

    if {$hitslist == ""} {
        return [list $textarea [list "0NoFunInBuf" 0 0]]
    } else {
        return [list $textarea $hitslist]
    }
}

proc funnametofunnametafunstart {functionname} {
# given a function name, this proc looks in all the opened buffers and
# tries to find a function with this name
# if it succeeds, then the proc returns a list with one getallfunsintextarea
# result, more precisely, this is a list {$funcname $ta $funstartline}
# if it does not succeed, then the return value is ""
# <TODO> This search might fail if the same function name can be found in more
#        than one single buffer - in this case, the first match is returned
#        this proc should be improved to prompt the user whenever there is
#        more than one match
#        Hmm, maybe the ultimate fix would rather be to maintain a list of
#        functions defined in the buffers i.e. listoftextarea("$ta",definedfuns)
#        this should be dynamical - this would be good for performance since
#        getallfunsinalltextareas, which is usually the slowest proc, would
#        have to be called much less often
#        Handling this is needed because this proc is not only used in the
#        debugger but also elsewhere (proc blinkline, that is even called from
#        outside of Scipad by edit_error - asking the user where it should
#        blink is probably not the best thing to do!!)
#        In the debugger there can no longer be duplicate functions, this case
#        is trapped and the debug won't run
    set fundefs [getallfunsinalltextareas]
    set funstruct ""
    foreach {ta fundefsinta} $fundefs {
        foreach {funcname funcline funstartline} $fundefsinta {
            if {$funcname == $functionname} {
                set funstruct [list $funcname $ta $funstartline]
                break
            }
        }
    }
    return $funstruct
}

proc trimcontandcomments {str} {
# remove comments and continued lines from $str and return the resulting
# string
    global scommRE scontRE2
    regsub -all -- $scontRE2 $str  "" str2
    regsub -all -- $scommRE  $str2 "" str2
    set str2 [string trim $str2]
    return $str2
}

proc extractfunnamefromfunline {str} {
# find the function name in a function definition line
    global snRE
    set funname ""
    if {[set i [string first "=" $str]] != {}} {
        regexp -start [expr {$i + 1}] -- $snRE $str funname  
    } else {
        regexp -- $snRE $str funname  
    }
    return $funname
}

proc isnocodeline {w ind} {
# return true if the line containing index $ind in textarea $w
# is a no code line, i.e. if this line either:
#     - is empty
# or  - contains only blanks (spaces or tabs)
# or  - contains only a comment with possible leading blanks
# otherwise, return false
    global sblRE scommRE
    if {[regexp -- "^$sblRE$scommRE?\$" \
                   [$w get "$ind linestart" "$ind lineend"]]} {
        return true
    }
    return false
}

proc bufferhaslevelzerocode {w} {
# return true if textarea $w has at least one character of "level zero" code,
# i.e. at least one character outside of a function and that is part of an
# executable statement (i.e. an uncommented non blank character)
# this file is therefore either a pure .sce file or a mixed .sce/.sci

    set out false

    # these characters can be present between functions even if uncommented,
    # they don't denote main level code
    set nocodechars {" " "\t" "\n"}

    set funsinthatbuf [lindex [getallfunsintextarea $w] 1]

    if {[lindex $funsinthatbuf 0] == "0NoFunInBuf"} {
        set out true
    } else {

        # is there main level code before the first function definition,
        # or between function definitions?
        set ind "1.0"
        foreach {fname fline precf} $funsinthatbuf {
            # if this function is nested in a higher level function,
            # skip it since only the intervals between highest level
            # functions must be scanned
            if {[whichfun $precf $w] != {}} {
                # there is no $ind adjustment to do because if this function
                # is nested, it is inside a function starting before it,
                # which means that $ind has already been adjusted to the
                # correct endfunction keyword at the end of the previous
                # iteration
                continue
            }
            while {[$w compare $ind < $precf]} {
                set ch [$w get $ind]
                # $ch is a character from a code statement if it is a non
                # commented char different from space, newline, or tab
                if {[lsearch [$w tag names $ind] "rem2"] == -1} {
                    if {[lsearch $nocodechars $ch] == -1} {
                        set out true
                        break
                    }
                }
                set ind [$w index "$ind + 1 c"]
            }
            if {$out} {break}
            set endpos [getendfunctionpos $w $precf]
            if {$endpos != -1} {
                set ind [$w index "$endpos + 10 c"]
            } else {
                # function not terminated by an endfunction keyword
                # then we have reached the end of the buffer
                # and there is no need to break
                # there is neither no need to return a special error
                # code since proc checkforduplicateorunterminatedfuns
                # has been called long before and the code we're now
                # in won't be run when an unterminated function exists
                set ind "end"
            }
        }

        # is there main level code after the last function definition?
        while {[$w compare $ind < end]} {
            set ch [$w get $ind]
            # $ch is a character from a code statement if it is a non
            # commented char different from space, newline, or tab
            if {[lsearch [$w tag names $ind] "rem2"] == -1} {
                if {[lsearch $nocodechars $ch] == -1} {
                    set out true
                    break
                }
            }
            set ind [$w index "$ind + 1 c"]
        }
    }

    return $out
}

proc getlistofancillaries {ta fun tag {lifun -1}} {
# scan function $fun from textarea $ta for words tagged as $tag
# and return these words in a list
#   - duplicate names are removed from the list
#   - tagged text inside functions nested in $fun is ignored
#   - if the line argument $lifun is given, then this proc
#     only returns the ancillaries from this logical line in
#     function $fun
#   - if the line argument $libfun is not given, then all the
#     ancillaries of $fun are returned
    set listofancill [list ]
    set allfunshere [lindex [getallfunsintextarea $ta] 1]
    foreach {funname funline precfun} $allfunshere {
        if {$funname != $fun} {
            continue
        }
        # function of interest is located, create list of
        # all calls to ancillaries tagged as $tag
        set endpos [getendfunctionpos $ta $precfun]
        if {$endpos == -1} {
            # should never happen since handled much ealier by checkforduplicateorunterminatedfuns, but...
            tk_messageBox -message "Unexpected missing endfunction in proc getlistofancillaries: please report"
        }
        foreach {i j} [$ta tag ranges $tag] {
            if {[$ta compare $precfun <= $i]} {
                if {[$ta compare $j <= $endpos]} {
                    # check that the tagged text actually belongs
                    # to $fun and not to an ancillary of $fun
                    foreach {fn lif fl pf cl} [whichfun $i $ta] {}
                    if {$fn == $fun} {
                        # the tagged text is in the right function
                        # (not in a nested fun of the right one)
                        # now check that the line where the tagged
                        # text appears is the given line $lifun
                        # or skip this test if $libfun was not given
                        if {$lifun == -1 || $lifun == $lif} {
                            lappend listofancill [$ta get $i $j]
                        }
                    }
                }
            }
        }
        # remove duplicates
        for {set i 0} {$i < [llength $listofancill]} {incr i} {
            for {set j [expr {$i + 1}]} {$j < [llength $listofancill]} {incr j} {
                if {[lindex $listofancill $j] == [lindex $listofancill $i]} {
                    set listofancill [lreplace $listofancill $j $j]
                    incr j -1
                }
            }
        }
    }
    return $listofancill
}
