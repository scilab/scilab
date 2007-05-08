#####################################################################
#
# There are two relevant arrays used for colorization and keyword
# matching: chset and words.
#
# words contains elements addressed as words(MODE.TAG.INITIAL). 
# MODE by now is always scilab,
# TAG is one of {comm intfun predef libfun scicos userfun}
# INITIAL is a single character, a valid scilab name initial.
# Each element is a tcl list containing all the words of the given TAG
# beginning with INITIAL (not all possible INITIALs need to be present,
# if there are no keywords beginning with that letter).
# The script dynamickeywords.sce actually orders them alphabetically.
# This is not exploited by the colorize procs, nor for autocompletion.
#
# Special case for the words array: TAG == userfun - this entry is not
# sorted, and does not only contain the functions names but for each
# function a list of the following form: {$fname $buf $precf}, where:
#    . $fname : function name
#    . $buf : name of the textarea containing that function
#    . precf : physical line number in $buf identifying the beginning
#              of $fname
# This is required for the goto function feature, in order not to
# confuse functions with the same name defined more than once (in a
# single buffer or among opened buffers)
# 
# chset contains elements addressed as chset(MODE.TAG). Each element there
# is a string of all the represented initials of the keywords in class
# MODE.TAG. Also this is presently alphabetically sorted, though the fact
# is not exploited.
# Special case: TAG == userfun - this entry is not sorted
#
# Thus a word encountered in the text is matched with the database of
# keywords in the following way:
#
# take the first character of the word
# if initial is in the relevant chset(MODE.TAG)
#   check for a match in {words($MODE.$TAG.$INITIAL)}
#
#####################################################################


proc load_words {} {
    global words chset intmacdir
# empty initialization of the keyword arrays, for the 
#  detached invocation of scipad (and for early-bird colorization requests)    
    set chset(scilab.command) {}
    set chset(scilab.intfun) {}
    set chset(scilab.predef) {}
    set chset(scilab.libfun) {}
    set chset(scilab.scicos) {}
    # ask to scilab about keywords:
    # Warning: "seq" option only is mandatory so that colorization
    #          works with scipad somefile.sci (i.e. the ScilabEval
    #          below must be queued before colorization starts in
    #          the file that will be loaded through the famous
    #          TCL_EvalStr("ScilabEval "TCL_EvalStr(""openfile ...
    #          (see scipad.sci)
    # Note: "sync" "seq" would have been the right thing to do, but for
    #       some unclear reason words and chset are unknown in that case
    #       when the exec returns (running in sync mode is the same as
    #       running in a function: local vars are unknown upon function
    #       return). What is however strange is that this applies to slave
    #       Tcl interpreters too... See also proc setScipadVersionString
    #       for a simpler example of this strange behavior
    ScilabEval_lt "exec \"$intmacdir/dynamickeywords.sce\";" "seq"

# old code to load a word file with additional keywords: maybe someday it will
# turn useful...
#     set type {}
#     set col {} 
#     set f [open $intmacdir/words r]
#     while {[gets $f line] > 0} { 
#         if {[lindex $line 0] == {#MODE}} {
#             set type [lindex $line 1]
#         } elseif {[lindex $line 0] == {#TAG}} {
#             set col [lindex $line 1]
#             set chset($type.$col) {}
#         } else {
#             set ch [string range $line 0 0]
#             append chset($type.$col) $ch
#             set words($type.$col.$ch) $line
#         }
#     }
#     close $f    

    set chset(scilab.userfun) {}

#presently empty lists for other schemes
#    set chset(none) {}
#    set chset(xml) {}
}

proc remalltags {w begin ende} {
    $w tag remove parenthesis $begin $ende
    $w tag remove bracket $begin $ende
    $w tag remove brace $begin $ende
    $w tag remove punct $begin $ende
    $w tag remove operator $begin $ende
    $w tag remove number $begin $ende
    $w tag remove intfun $begin $ende
    $w tag remove command $begin $ende
    $w tag remove predef $begin $ende
    $w tag remove libfun $begin $ende
    $w tag remove scicos $begin $ende
    $w tag remove rem2 $begin $ende
    $w tag remove xmltag $begin $ende
    $w tag remove textquoted $begin $ende  
    # tag userfun is deleted in proc docolorizeuserfun
}

proc colorizetag {w pat tagname str start} {
# ancillary for proc colorize, written for speed
    set allmatch [regexp -all -inline -indices -- $pat $str]
    set ind $start
    set previ 0
    foreach amatch $allmatch {
        foreach {i j} $amatch {}
        set star [$w index "$ind  + [expr {$i - $previ}] c"]
        set stop [$w index "$star + [expr {$j - $i + 1}] c"]
        $w tag add $tagname $star $stop
        set ind $star
        set previ $i
    }
}

proc colorize {w cpos iend} {
# Colorize in textarea $w from start position cpos to end position iend
# Warning: if cpos denotes a position located *after* iend, nothing is done
    global words chset listoffile
    global snRE floatingpointnumberREpat

    set schema $listoffile("$w",language)
    set colori $listoffile("$w",colorize)

    # get the list of all scilab mode tags and remove the userfun tag
    # since it it treated specially by proc docolorizeuserfun
    regsub -all "scilab." [array names chset -glob scilab\.*] "" scitags
    set userfunpos [lsearch $scitags "userfun"]
    set scitags [lreplace $scitags $userfunpos $userfunpos]

    set cpos [$w index $cpos]
    set iend [$w index $iend]

    remalltags $w $cpos $iend

    # if colorize is set to false, the only thing to do is to remove colorization tags
    if {!$colori} {return}

    set searchedstr [$w get $cpos $iend]

    # TAGS:
    # order matters here - for instance textquoted has to be after operator, 
    # so operators are not colorized within strings

    # punctuation
    if {$schema=="scilab"} {
        colorizetag $w {[;,]} punct $searchedstr $cpos
    }

    # parentheses
    colorizetag $w {[()]} parenthesis $searchedstr $cpos

    # brackets
    colorizetag $w {[\[\]]} bracket $searchedstr $cpos

    # braces
    #ES: why at all call ":" a "brace"? 
    colorizetag $w {[\{\}:]} brace $searchedstr $cpos

    # operators
    if {$schema=="scilab"} {
        colorizetag $w {['\.+\-*\/\\\^=\~$|&<>]} operator $searchedstr $cpos
    }

    # numbers can contain +-. therefore follows operator colorization
    colorizetag $w $floatingpointnumberREpat number $searchedstr $cpos

    # Scilab keywords - they are Scilab "names" (see help names)
    if {$schema=="scilab"} {
        set allmatch [regexp -all -inline -indices -- $snRE $searchedstr]
        set ind $cpos
        set previ 0
        foreach amatch $allmatch {
            foreach {i j} $amatch {}
            set star [$w index "$ind + [expr {$i - $previ}] c"]
            set stop [$w index "$star + [expr {$j - $i + 1}] c"]
            set kword [$w get $star $stop]
            set initial [string range $kword 0 0]
            foreach itag $scitags {
                if {[string first $initial $chset(scilab.$itag)]>=0} {
                    if {[lsearch -exact $words(scilab.$itag.$initial) \
                            $kword] != -1} {
                        $w tag add $itag $star $stop
                    }
                }
            }
            set ind $star
            set previ $i
        }
    }

    # XML (#ES this is a problem as <> are also operators)
    if {$schema=="xml"} {
        # the regexp pattern is a bit oversimplified
        # it does not check properly nested <> constructs, it just
        # looks for the first closing delimiter
        # therefore <hello<<world> is recognized as a single XML tag
        # do we really want it? - In general, nested <> are not
        # properly dealt with here (<TODO>)
        colorizetag $w {<[^>]*>} xmltag $searchedstr $cpos
    }

    # Text quoted with single/double quotes as textquoted, and Scilab remarks
    colorizestringsandcomments_sd $w $cpos $iend

}

proc colorizestringsandcomments_sd {w thebegin theend} {
# colorize strings and comments between indices $thebegin and $theend in
# textarea $w
# consider single quotes (') and double quotes ("), or double quotes only
# based on the Options menu setting
# if language scheme is anything but scilab, // comments are ignored and
# strings are colorized if they are on a single physical line

    global scilabSingleQuotedStrings
    global listoffile

    set schema $listoffile("$w",language)

    if {$schema == "scilab"} {
        # deal with comments and continued strings only in scilab
        # language scheme

        if {$scilabSingleQuotedStrings == "yes"} {
            # single quotes or double quotes
            set quotespattern {["']}

            # since in Scilab it is legal to declare a="string', no effort is
            # made here to detect unbalanced quotes by their type

            # regular expression matching a simple string on a (non-continued) line
            # actually the character just before the string is part of the match
            # this allows for removing incorrect colorization of multiple matrix
            # transpose on a single line
            set simpstrRE {((?:[^\w_#!?$\]\}\)"']|\A(?:))["'][^"'\.\n]*(?:\.?[^"'\.\n]*)+["'])}

            # regular expression matching a continued string possibly ending with a
            # comment, if this string was not already matched by $simpstrRE
            # actually the character just before the string is part of the match
            # this allows for removing incorrect colorization of multiple matrix
            # transpose on a single line
            set contstrRE {((?:[^\w_#!?$\]\}\)"']|\A(?:))["'](?:(?:[^"'\.\n]*(?:\.[^"'\.\n]+)*\.{2,} *)+(?://[^\n]*)?\n)+[^"'\n]*["'])}

        } else {
            # double quotes only
            set quotespattern {"}

            set simpstrRE {((?:[^\w_#!?$\]\}\)"]|\A(?:))"[^"\.\n]*(?:\.?[^"\.\n]*)+")}
            set contstrRE {((?:[^\w_#!?$\]\}\)"]|\A(?:))"(?:(?:[^"\.\n]*(?:\.[^"\.\n]+)*\.{2,} *)+(?://[^\n]*)?\n)+[^"\n]*")}
        }

        # regular expression matching a comment outside of a continued string
        # if this comment was not already matched by $simpstrRE or $contstrRE
        set outstrcommRE {(//[^\n]*)}

        # regular expression matching in two separate parts the continued string
        # part and the comment part of a continued line containing a comment
        set separationRE {([^\.\n]*(?:\.[^\.\n]+)*\.{2,} *)((//[^\n]*)?)}

        set textcommfullRE "$simpstrRE|$contstrRE|$outstrcommRE"

    } else {
        # ignore comments and continued strings if language scheme is
        # anything but scilab

        if {$scilabSingleQuotedStrings == "yes"} {
            # single quotes or double quotes
            set quotespattern {["']}
            set simpstrRE {(?:(?:.|\A(?:))["'][^"'\n]*["'])}
        } else {
            # double quotes only
            set quotespattern {"}
            set simpstrRE {(?:(?:.|\A(?:))"[^"'\n]*")}
        }

        set separationRE {}
        set textcommfullRE "$simpstrRE"
    }

    colorizestringsandcomments $w $thebegin $theend \
            $textcommfullRE $separationRE $quotespattern
}

proc colorizestringsandcomments {w thebegin theend textcommfullRE separationRE quotespattern} {
# colorize properly comments and text quoted with single or double quotes,
# depending on the regexps received, while taking care of continued lines
# possibly containing interlaced comments (this is legal in Scilab)

    global listoffile

    set resi {}
    set simpstr {}
    set contstr {}
    set outstrcomm {}

    set allmatch [regexp -all -inline -indices -- $textcommfullRE [$w get $thebegin $theend]]

    set star $thebegin
    set previ 0

    set schema $listoffile("$w",language)

    if {$schema == "scilab"} {
        foreach {resi simpstr contstr outstrcomm} $allmatch {
            foreach {i j} $resi {}
            set star [$w index "$star  + [expr {$i - $previ}] c"]
            set ind $star
            # $ind contains now the start index for a not yet colorized match
            # with either:
            #   1 - a simple string without continuation dots, e.g.:
            #         "there is no co.mm.ent//in this str.ing"
            #   2 - a string formed by continued lines, possibly with embedded
            #       comments, e.g.:
            #         "a continued ..  // this part is a comment
            #          line with 1 comment"
            #   3 - a regular comment outside of a string declaration, e.g.:
            #         // there is no string in this "tricky" comment
            # the matching order is mandatorily the order above as coded
            # in the regular expression - this order *must* be followed below
            # cases 1 and 2: the match starts at the character before the
            # opening quote, or at an empty string before the quote
            # case 3: the match starts at the first slash indicating a comment

            # length of the match
            set num [expr {$j - $i + 1}]

            set done false
            # try first case: string on a single line
            if {[lindex $simpstr 0] != -1} {
                foreach {i1 j1} $simpstr {}
                if {$i == $i1 && $j == $j1} {
                    # we're really in the first case
                    $w mark set stop "$ind +$num c"
                    if {![string match $quotespattern [$w get $ind]]} {
                        # in this case we did not match at the beginning of the
                        # string but one character before the opening quote
                        set ind "$ind + 1 c"
                    } else {
                        # we matched at the opening quote - no adjustment to $ind
                    }
                    # textquoted deletes any other tag
                    remalltags $w $ind stop
                    $w tag add textquoted $ind stop
                    set done true
                } else {
                    # some part located after $ind matches $simpstrRE
                    # ignore this for now and try cases 2 and 3 first
                }
            }

            # try second case: string on continued lines, possibly with comments
            if {!$done && [lindex $contstr 0] != -1} {
                foreach {i1 j1} $contstr {}
                if {$i == $i1 && $j == $j1} {
                    # we're really in the second case
                    $w mark set stop "$ind +$num c"
                    # the comment part must be separated from the string part, and
                    # this has to be done for each continued line but the last one
                    if {![string match $quotespattern [$w get $ind]]} {
                        # in this case we did not match at the beginning of the
                        # string but one character before the opening quote
                        set ind "$ind + 1 c"
                    } else {
                        # we matched at the opening quote - no adjustment to $ind
                    }
                    set subtext [$w get $ind stop]
                    set strpart {}
                    set commpart {}
                    set colstart [$w index $ind]
                    while {[regexp -indices -- $separationRE $subtext \
                            res1 strpart commpart]} {
                        # colorize string part of the line
                        set scolstart [$w index "$colstart + [lindex $strpart 0] c"]
                        set scolstop  [$w index "$colstart + [lindex $strpart 1] c + 1 c"]
                        remalltags $w $scolstart $scolstop
                        $w tag add textquoted $scolstart $scolstop
                        # colorize comment part of the line
                        if {$commpart != {}} {
                            set ccolstart [$w index "$colstart + [lindex $commpart 0] c"]
                            set ccolstop  [$w index "$colstart + [lindex $commpart 1] c + 1 c"]
                            remalltags $w $ccolstart $ccolstop
                            $w tag add rem2 $ccolstart $ccolstop
                        }
                        set colstart [$w index "$colstart + [lindex $res1 1] c + 1 c"]
                        set subtext [string replace $subtext 0 [lindex $res1 1]]
                        set strpart {}
                        set commpart {}
                    }
                    # treat the special case of the last line, which is not a
                    # continued line (doesn't match $separationRE)
                    remalltags $w $colstart stop
                    $w tag add textquoted $colstart stop
                    set done true
                } else {
                    # some part located after $ind matches $contstrRE
                    # ignore this for now and try case 3 first
                }
            }

            # try third case: regular comment outside of a string
            if {!$done && [lindex $outstrcomm 0] != -1} {
                foreach {i1 j1} $outstrcomm {}
                if {$i == $i1 && $j == $j1} {
                    # we're really in the third case
                    $w mark set stop "$ind +$num c"
                    # rem2 deletes any other tag
                    remalltags $w $ind stop
                    $w tag add rem2 $ind stop
                    set done true
                } else {
                    # some part located after $ind matches $outstrcommRE
                    # but this should have been dealt with in the previous cases...
                }
            }

            if {!$done} {
                # should never happen
                tk_messageBox -message "Colorization algorithm fooled!\n\
                    Position is $ind and regexp match length is $num.\n\
                    Text at this place is:\n[$w get $ind "$ind + $num c"]\n\
                    Please report to the Bugzilla (include your current file)." \
                    -icon warning -title "Colorization error"
                # this is to avoid to repeat the message
                break
            }

            set resi {}
            set simpstr {}
            set contstr {}
            set outstrcomm {}
            set previ $i
        }

    } else {
        # non scilab schema, i.e. xml or none
        foreach resi $allmatch {
            foreach {i j} $resi {}
            set star [$w index "$star  + [expr {$i - $previ}] c"]
            set ind $star
            # length of the match
            set num [expr {$j - $i + 1}]
            $w mark set stop "$ind +$num c"
            if {![string match $quotespattern [$w get $ind]]} {
                # in this case we did not match at the beginning of the
                # string but one character before the opening quote
                set ind "$ind + 1 c"
            } else {
                # we matched at the opening quote - no adjustment to $ind
            }
            # textquoted deletes any other tag
            remalltags $w $ind stop
            $w tag add textquoted $ind stop
            set previ $i
        }
    }
}

proc dobackgroundcolorize {w thebegin progressbar} {
# do colorization in background
# actually this uses a trick to keep Scipad responsive while colorization
# is in progress - useful for large files
# colorization is performed by small line increments that are colorized
# one after the other when Scipad is idle
# adjustment of increment ($incre) is important:
#   . if decreased, the complete colorization process will last longer
#     due to time overhead from this proc
#   . if increased, Scipad will be less responsive
# the progressbar is also updated accordingly

    global pad nbfilescurrentlycolorized listoffile

    # avoid Tcl error when Scipad is closed during colorization
    if {![info exist pad]} {
        return
    }

    scan $progressbar $pad.cp%d progressbarId

    # if the file has been closed during colorization, destroy the
    # progressbar and abort colorization
    if {![info exist listoffile("$w",fullname)]} {
        destroy $progressbar
        incr nbfilescurrentlycolorized -1
        return
    }

    # if the file has been asked for recolorization, abort it and reuse
    # the same progressbar to relaunch colorization
    if {$listoffile("$w",progressbar_id) == ""} {
        # relaunch colorization from the beginning, using the already
        # packed progressbar
        SetProgress $progressbar 0
        set listoffile("$w",progressbar_id) $progressbarId
        dobackgroundcolorize $w 1.0 $progressbar
        return
    }

    # colorization not aborted - normal process
    set incre 20

    set curend "$thebegin + $incre l"
    set curend [getendofcolorization $w $curend]

    # if the file just needs removal of the colorization tags, do it at once
    if {!$listoffile("$w",colorize)} {
        set curend [$w index end]
    }

    scan [$w index $curend] "%d.%d" ycur xcur
    scan [$w index end]     "%d.%d" yend xend
    SetProgress $progressbar $ycur $yend $listoffile("$w",displayedname)

    if {[$w compare $curend < end]} {
        colorize $w $thebegin [$w index $curend]
        set newbegin [$w index $curend]
        after idle [list after 1 dobackgroundcolorize $w $newbegin $progressbar]
    } else {
        # last colorization step
        colorize $w $thebegin end
        set listoffile("$w",progressbar_id) ""
        incr nbfilescurrentlycolorized -1
        destroy $progressbar
        # update status info - might be needed since logical line info makes
        # use of proc whichfun, which in turn uses colorization info
        keyposn $w
        backgroundcolorizeuserfun
    }
}

proc backgroundcolorize {w} {
# if allowed, launch background colorization of a buffer from its start,
# initialize the colorization progressbar,
# and keep track of the buffers currently being colorized

    global backgroundtasksallowed nbfilescurrentlycolorized
    global pad progressbarId listoffile

    if {!$backgroundtasksallowed} {
        # colorize actually in foreground

        set progressbar [Progress $pad.cp0]
        pack $progressbar -fill both -expand 0 -before $pad.pw0 -side bottom

        set thebegin "1.0"
        set curend $thebegin
        while {[$w compare $thebegin < end]} {
            set thebegin $curend
            # if the file just needs removal of the colorization tags, do it at once
            if {!$listoffile("$w",colorize)} {
                set curend [$w index end]
            }
            colorize $w $thebegin \
                    [set curend [getendofcolorization $w "$curend + 10 l"]]
            scan [$w index $thebegin] "%d.%d" ycur xcur
            scan [$w index end]       "%d.%d" yend xend
            SetProgress $progressbar $ycur $yend $listoffile("$w",displayedname)
            update idletasks
        }

        colorize $w [getstartofcolorization $w "$curend - 10 l"] end

        docolorizeuserfun

        destroy $progressbar

    } else {

        # check if the file is already being colorized (can be the case when
        # switching scheme for instance)
        # reset colorization of the file already being colorized - this
        # is achieved by emtying the progressbar id, which is detected
        # in the background colorization proc that is already running
        if {$listoffile("$w",progressbar_id) != ""} {
            set listoffile("$w",progressbar_id) ""
        } else {
            # start colorization of a file currently not being colorized
            incr progressbarId
            incr nbfilescurrentlycolorized
            set listoffile("$w",progressbar_id) $progressbarId
            set progressbar [Progress $pad.cp$progressbarId]
            pack $progressbar -fill both -expand 0 -before $pad.pw0 -side bottom
            dobackgroundcolorize $w 1.0 $progressbar
        }

    }
}

proc colorizationinprogress {} {
# return true if colorization of a buffer is in progress
# certain actions (those that make use of the colorization result such as the
# rem2 or textquoted tag) cannot be executed during colorization

    global nbfilescurrentlycolorized

    if {$nbfilescurrentlycolorized > 0} {
        set mes [mc "You can't do that while colorization is in progress.\
                 Please try again when finished."]
        set tit [mc "Command forbidden during colorization"]
        tk_messageBox -message $mes -icon warning -title $tit
        return true
    } else {
        return false
    }

}

proc iscurrentbufnotcolorized {} {
# return true if current buffer is not colorized
# certain debugger commands cannot be executed if colorization is disabled
    global ColorizeIt
    if {!$ColorizeIt} {
        set mes [mc "You must enable colorization to be able to launch the debugger!"]
        set tit [mc "The debugger needs colorization"]
        tk_messageBox -message $mes -icon error -title $tit
        return true
    } else {
        return false
    }
}

proc backgroundcolorizeuserfun {} {
# launch a docolorizeuserfun command in background for all the visible buffers
# docolorizeuserfun can be a bit long to execute for large buffers despite
# the efforts put in speed improvement
# if many docolorizeuserfun are waiting for execution, e.g. when the user types
# quicker than the position can be updated by Scipad, there can be many
# docolorizeuserfun commands pending -> first delete them since they are now
# pointless and launch only the latest command
# docolorizeuserfun is catched to deal more easily with buffers that were
# closed before the command could be processed
    global backgroundtasksallowed
    if {$backgroundtasksallowed} {
        after cancel [list after 1 "catch \"docolorizeuserfun\""]
        after idle   [list after 1 "catch \"docolorizeuserfun\""]
    } else {
        docolorizeuserfun
    }
}

proc docolorizeuserfun {} {
# detect and colorize user functions
# recreate the list of all user defined functions in all the textareas
# and tag (colorize) user function keywords in all the visible buffers
# note that neither chset nor words are sorted for userfun tag
    global listoffile listoftextarea
    global words chset
    global notsnccRE notsncclookaheadRE
    global maxcharinascilabname

    set mode scilab
    set tag userfun

    # note wrt to peers:
    #    - tags are shared among peers
    #    - $listoffile("$ta",colorize) is the same for all peers of $ta
    # there is therefore no need to perform the tasks in this proc for all
    # textareas, it is only required for any one of the peers (and of course
    # for textareas that have no peers)
    set nopeerslistoftextarea [filteroutpeers $listoftextarea]

    foreach ta $nopeerslistoftextarea {
        if {[isdisplayed $ta]} {
            $ta tag remove userfun 1.0 end
        }
    }

    # remove userfun entries from the words array and reset chset
    foreach initialtag [array names words $mode.$tag.*] {
        unset words($initialtag)
    }
    set chset($mode.$tag) ""

    # populate arrays chset and words
    # note: no check for duplicates is made, but this cannot happen since
    # the arrays have just been cleaned - apparent duplicates can however
    # appear if a function is defined more than once in a buffer or across
    # opened buffers (the entries in words will at least differ by the
    # line number)
    foreach ta $nopeerslistoftextarea {
        if {$listoffile("$ta",colorize)} {
            set funsinthatbuf [lindex [getallfunsintextarea $ta] 1]
            if {[lindex $funsinthatbuf 0] == "0NoFunInBuf"} {
                continue
            }
            foreach {fname fline precf} $funsinthatbuf {
                set ch [string range $fname 0 0]
                if {[string first $ch $chset($mode.$tag)] == -1} {
                    append chset($mode.$tag) $ch
                }
                lappend words($mode.$tag.$ch) [list $fname $ta $precf]
            }
        }
    }

    # colorize function names in the function definition line as well as
    # calls to these functions, if not in a comment nor in a string
    # to improve drastically performance, better use a regexp once with
    # many alternatives (foo1|foo2|foo3|...) rather than running a simpler
    # regexp (fooi) n times (n being the number of defined functions)
    # speed gain is approximately 20%
    # tagging is done only in visible buffers
    foreach ta $nopeerslistoftextarea {
        if {[isdisplayed $ta] && $listoffile("$ta",colorize)} {

            # don't colorize user function in buffers other than scilab
            if {$listoffile("$ta",language) != "scilab"} {
                continue
            }

            # the leading space is added so that the regexp can match at
            # the very beginning of the text
            # regexping for $notsnccRE|(?:) is not correct because it would
            # match function names inside longer Scilab names
            set fulltext " [$ta get 1.0 end]"

            # construct the composite regexp pattern
            set atleastone false
            set pat $notsnccRE
            append pat {(}
            foreach mode_tag_initial [array names words -glob $mode.$tag.*] {
                foreach elt $words($mode_tag_initial) {
                    set fname [lindex $elt 0]
                    # A question mark or dollar sign in the function name must
                    # be escaped otherwise the regexp compilation fails
                    # (Scilab function names can contain in particular ? or $)
                    set escfunname [string map {"\?" "\\\?"} $fname]
                    set escfunname [string map {"\$" "\\\$"} $escfunname]
                    append pat $escfunname {|}
                    set atleastone true
                }
            }
            if {$atleastone} {
                set pat [string range $pat 0 "end-1"]
            }
            append pat {)} $notsncclookaheadRE

            set allmatch [regexp -all -inline -indices -- $pat $fulltext]

            # parse regexp results and tag with userfun accordingly
            set ind "1.0"
            set previ 1 ;# and not 0 because of the added leading space in $fulltext
            foreach {fullmatch funnamematch} $allmatch {
                foreach {i j} $funnamematch {}
                set star [$ta index "$ind + [expr {$i - $previ}] c"]
                set malength [expr {$j - $i + 1}]
                set stop [$ta index "$star + $malength c"]
                if {[lsearch [$ta tag names $star] "rem2"] == -1} {
                    if {[lsearch [$ta tag names $star] "textquoted"] == -1} {
                        if {$malength > $maxcharinascilabname} {
                            # clip tagging length to $maxcharinascilabname characters,
                            # since this is the Scilab limitation - this is to remind
                            # the user of this limit
                            # Scipad is not limited in function names, but Scilab is
                            set stop [$ta index "$stop - [expr {$malength - $maxcharinascilabname}] c"]
                        }
                        $ta tag add "userfun" $star $stop
                    }
                }
                set ind $star
                set previ $i
            }

            # userfun is of higher priority than operator - must do this
            # because of the $ case, which is an operator, and can also be
            # in a function name
            $ta tag raise userfun operator
        }
    }

}

proc changelanguage {newlanguage} {
    global listoffile pad

    set textarea [gettextareacur]
    set oldlanguage $listoffile("$textarea",language)

    if {$oldlanguage != $newlanguage} {
        foreach ta [getfullpeerset $textarea] {
            set listoffile("$ta",language) $newlanguage
        }
        showinfo [mc "Wait seconds while recolorizing file"]
        schememenus $textarea
        tagcontlines $textarea
        backgroundcolorize $textarea

        # if this is a new file not opened from disk,
        # then the filename extension must be changed
        if {$listoffile("$textarea",new) == 1} {
            if {$newlanguage == "xml"} {
                set ext "xml"
            } else {
                # scilab or none
                set ext "sce"
            }

            # replace extension of file name by the new extension
            foreach ta [getfullpeerset $textarea] {
                set ilab [extractindexfromlabel $pad.filemenu.wind \
                          $listoffile("$ta",displayedname)]
                foreach {dispname peerid} [removepeerid $listoffile("$ta",displayedname)] {}
                set dispname [string replace $dispname "end-2" end $ext]
                set dispname [appendpeerid $dispname $peerid]
                set listoffile("$ta",displayedname) $dispname                    
                set listoffile("$ta",fullname) \
                        [string replace $listoffile("$ta",fullname) "end-2" end $ext]
                # update the windows menu label
                setwindowsmenuentrylabel $ilab $listoffile("$textarea",displayedname)
                RefreshWindowsMenuLabelsWrtPruning
            }
        }
    }
}

proc switchcolorizefile {} {
# switch colorization of the current buffer on or off
    global listoffile ColorizeIt
    global funnameargs funnames
    set w [gettextareacur]
    foreach ta [getfullpeerset $w] {
        set listoffile("$ta",colorize) $ColorizeIt
    }
    if {$ColorizeIt} {
        showinfo [mc "Wait seconds while recolorizing file"]
    }
    if {!$ColorizeIt} {
        # prevent the configure box to open from a buffer that can no more
        # contain functions since colorization is off
        set funnameargs ""
        set funnames ""
    }
    backgroundcolorize $w
}

proc schememenus {textarea} {
    global pad listoffile
    global Shift_F8 Shift_F11 Shift_F12
    global watch watchwinicons watchwinstepicons
    global MenuEntryId
    set dm $pad.filemenu.debug
    set dms $pad.filemenu.debug.step
    if {$listoffile("$textarea",language) == "scilab"} {
        #enable "Load into scilab"
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Load into Scilab"]) -state normal
        # restore bindings
        bindenable $pad execfile
        bind $pad <F5> {filetosave %W; execfile}
        # enable the debug entries and watch window icons
        # this is set selectively in function of the debugger state
        setdbmenuentriesstates_bp
        # enable "create help skeleton" - done in proc keyposn
        # enable "Show continued lines"
        $pad.filemenu.options.colorizeoptions entryconfigure $MenuEntryId($pad.filemenu.options.colorizeoptions.[mcra "Show c&ontinued lines"]) -state normal
    } else {
        #disable "Load into scilab"
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Load into Scilab"]) -state disabled
        # remove bindings
        binddisable $pad execfile
        bind $pad <F5> {}
        #disable all the Debug menues entries
        for {set i 1} {$i<=[$dm index last]} {incr i} {
            if {[$dm type $i] == "command" || [$dm type $i] == "cascade"} {
                $dm entryconfigure $i -state disabled
            }
        }
        for {set i 1} {$i<=[$dms index last]} {incr i} {
            if {[$dms type $i] == "command" || [$dms type $i] == "cascade"} {
                $dms entryconfigure $i -state disabled
            }
        }
        # remove debugger bindings
        bind all <F9> {}
        bind all <Control-F9> {}
        bind all <F10> {}
        bind all <Control-F11> {}
        bind all <F11> {}
        pbind all $Shift_F11 {}
        pbind all $Shift_F8 {}
        bind all <F8> {}
        bind all <Control-F8> {}
        pbind all $Shift_F12 {}
        bind all <F12> {}
        bind all <Control-F12> {}
        # disable watch window icons
        if {[info exists watchwinicons] && [info exists watchwinstepicons]} {
            if {[winfo exists $watch]} {
                set wi $watchwinicons
                set wis $watchwinstepicons
                [lindex $wi $MenuEntryId($dm.[mcra "&Configure execution..."])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Go to next b&reakpoint"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step &into"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step o&ver"])] configure -state disabled
                [lindex $wis $MenuEntryId($dms.[mcra "Step &out"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Run to re&turn point"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Run to c&ursor"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "G&o on ignoring any breakpoint"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "&Break"])] configure -state disabled
                [lindex $wi $MenuEntryId($dm.[mcra "Cance&l debug"])] configure -state disabled
            }
        }
        #disable "create help skeleton"
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help s&keleton..."]) -state disabled
        # disable "Show continued lines"
        $pad.filemenu.options.colorizeoptions entryconfigure $MenuEntryId($pad.filemenu.options.colorizeoptions.[mcra "Show c&ontinued lines"]) -state disabled
    }
    if {$listoffile("$textarea",language) == "xml"} {
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Compile as &help page"]) -state normal
    } else {
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Compile as &help page"]) -state disabled
    }
}

proc colormenuoption {c} {
# called when changing a color option from menu. Chooses the new color and
# refreshes whatever needed
    global bgcolors fgcolors listoftextarea pad
    foreach c1 "$bgcolors $fgcolors" {global $c1}
    set newcol [tk_chooseColor -initialcolor [set $c] -title [mc $c]]
    if {$newcol != ""} {
        set $c $newcol
        set i 0
        # refresh all the colors of the menu labels (one was changed)
        # We rely on BGCOLOR and  FGCOLOR to be surely members of the
        # color lists
        foreach c $bgcolors {
            incr i
            $pad.filemenu.options.colors entryconfigure $i \
               -background [set $c] -foreground $FGCOLOR  -activeforeground $FGCOLOR
        }
        foreach c $fgcolors {
            incr i
            $pad.filemenu.options.colors entryconfigure $i \
               -foreground [set $c]  -activeforeground [set $c]\
               -background $BGCOLOR
        }
        updateactiveforegroundcolormenu
        # refresh all color settings for all the opened buffers (only one
        # color was changed)
        # we need to do it also for peers, because TextStyles is not only tag
        # management, it is also configuration of the text widget itself
        # ($FGCOLOR $BGCOLOR $CURCOLOR $SELCOLOR)
        foreach i $listoftextarea {
            TextStyles $i
        }
        tagcontlinesinallbuffers
    }
}

proc refreshQuotedStrings {} {
    global listoftextarea
    showinfo [mc "Wait seconds while recolorizing file"]
    foreach w [filteroutpeers $listoftextarea] {
        $w tag remove rem2 1.0 end
        $w tag remove textquoted 1.0 end
        backgroundcolorize $w
    }
}

proc getstartofcolorization {w ind} {
# return start bound required for proper recolorization
# and take care of nearby continued lines
    global listoffile
    switch -- $listoffile("$w",language) {
        scilab {
            if {[iscontinuedline $w "$ind - 1 l"]} {
                set uplimit [getstartofcontline $w "$ind - 1 l"]
            } else {
                set uplimit [$w index "$ind linestart"]
            }
        }
        xml {
            set prevdelimiterpos [$w search -backwards -- {<} $ind 1.0]
            if {$prevdelimiterpos != ""} {
                set uplimit [$w index $prevdelimiterpos]
            } else {
                set uplimit [$w index "$ind linestart"]
            }
        }
        default {
            set uplimit [$w index "$ind linestart"]
        }
    }
    return $uplimit
}

proc getendofcolorization {w ind} {
# return end bound required for proper recolorization
# and take care of nearby continued lines
    global listoffile
    switch -- $listoffile("$w",language) {
        scilab {
            if {[iscontinuedline $w "$ind + 1 l"]} {
                set dnlimit [getendofcontline $w "$ind + 1 l"]
            } else {
                set dnlimit [$w index "$ind + 1 l lineend"]
            }
        }
        xml {
            set nextdelimiterpos [$w search -- {>} "$ind - 1 c" end]
            if {$nextdelimiterpos != ""} {
                set dnlimit [$w index "$nextdelimiterpos + 1 c"]
            } else {
                set dnlimit [$w index "$ind lineend"]
            }
        }
        default {
            set dnlimit [$w index "$ind + 1 l lineend"]
        }
    }
    return $dnlimit
}

proc updateactiveforegroundcolormenu {} {
    global pad
    for {set i 0} {$i<=[$pad.filemenu.options.colors index last]} {incr i} {
        if {[$pad.filemenu.options.colors type $i] != "separator" && [$pad.filemenu.options.colors type $i] != "tearoff"} {
                $pad.filemenu.options.colors entryconfigure $i -activebackground [shade \
                        [$pad.filemenu.options.colors entrycget $i -activeforeground] \
                        [$pad.filemenu.options.colors entrycget $i -background] 0.5]
        }
    }
}

# shade --
#
#   Returns a shade between two colors
#
# Arguments:
#   orig    start #rgb color
#   dest    #rgb color to shade towards
#   frac    fraction (0.0-1.0) to move $orig towards $dest
#
# This proc was copied from http://aspn.activestate.com/ASPN/Cookbook/Tcl/Recipe/133529
# Since errors were found e.g. for   shade black green2 0.8   when trying to use this color,
# format "\#%02x%02x%02x"  was changed into  format "\#%4.4x%4.4x%4.4x"
#
proc shade {orig dest frac} {
    global pad
    if {$frac >= 1.0} { return $dest } elseif {$frac <= 0.0} { return $orig }
    foreach {origR origG origB} [rgb2dec $orig] \
            {destR destG destB} [rgb2dec $dest] {
         set shade [format "\#%4.4x%4.4x%4.4x" \
            [expr {int($origR+double($destR-$origR)*$frac)}] \
            [expr {int($origG+double($destG-$origG)*$frac)}] \
            [expr {int($origB+double($destB-$origB)*$frac)}]]
    return $shade
    }
}

# rgb2dec --
#
#   Turns #rgb into 3 elem list of decimal vals.
#
# Arguments:
#   c    The #rgb hex of the color to translate, or a known color name
# Results:
#   List of three decimal numbers, corresponding to #RRRRGGGGBBBB color
#
# This proc was inspired by http://aspn.activestate.com/ASPN/Cookbook/Tcl/Recipe/133529
# and changed by Donald Arsenau based on a discussion on comp.lang.tcl. See:
# http://groups.google.fr/group/comp.lang.tcl/browse_thread/thread/83264d872c0f13cc/e65d91f5f4261239
#
proc rgb2dec cv {
    set c [string tolower $cv]
    if {[catch {winfo rgb . $c} rgb]} {
        error "bad color value \"$cv\""
    }
    return $rgb
}
