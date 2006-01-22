#####################################################################
#
# There are two relevant arrays used for colorization and keyword
# matching: chset and words.
#
# words contains elements addressed as words(MODE.TAG.INITIAL). 
# MODE by now is always scilab,
# TAG is one of {comm intfun predef libfun scicos}
# INITIAL is a single character, a valid scilab name initial.
# Each element is a tcl list containing all the words of the given TAG
# beginning with INITIAL (not all possible INITIALs need to be present,
# if there are no keywords beginning with that letter).
# The script dynamickeywords.sce actually orders them alphabetically.
# This is not exploited by the colorize procs, nor for autocompletion.
# 
# chset contains elements addressed as chset(MODE.TAG). Each element there
# is a string of all the represented initials of the keywords in class
# MODE.TAG. Also this is presently alphabetically sorted, though the fact
# is not exploited.
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
    global words chset env
    set ownpath "$env(SCIPATH)/tcl/scipadsources"
# empty initialization of the keyword arrays, for the 
#  detached invocation of scipad (and for early-bird colorization requests)    
    set chset(scilab.command) {}
    set chset(scilab.intfun) {}
    set chset(scilab.predef) {}
    set chset(scilab.libfun) {}
    set chset(scilab.scicos) {}
# ask to scilab about keywords:
# Warning: "seq" option only is mandatory so that colorization
#          works with scipad somefile.sci
    ScilabEval_lt "exec $ownpath/dynamickeywords.sce;" "seq"
# old code to load a word file with additional keywords: maybe someday it will
# turn useful...
#     set type {}
#     set col {} 
#     set f [open $ownpath/words r]
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
    $w tag remove text $begin $ende
    $w tag remove rem2 $begin $ende
    $w tag remove xmltag $begin $ende
    $w tag remove textquoted $begin $ende  
    $w tag remove indentation $begin $ende
}

proc colorize {w cpos iend} {
# Colorize in textarea w from start position cpos to end position iend
# Warning: if cpos denotes a position located *after* iend, nothing is done
    global words chset listoffile
    set schema $listoffile("$w",language)
    regsub -all "scilab." [array names chset -glob scilab\.*] "" scitags
    $w mark set begin "$cpos"
    $w mark set ende "$iend"
    remalltags $w begin ende

    # TAGS:
    # order matters here - for instance textquoted has to be after operator, 
    # so operators are not colorized within strings

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

    #ES: why at all call ":" a "brace"? 
    # "{}" are anyway parsed for matching pairs (proc blinkbrace)
    $w mark set last begin
    while {[set ind [$w search -regexp {[\{\}:]} last ende]] != {}} {
        if {[$w compare $ind >= last]} {
            $w mark set last $ind+1c
            $w tag add brace $ind last
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

    # number can contain +-. so follows operator (?)
    set numregexp {((\.\d+)|(\m\d+(\.\d*)?))([deDE][+\-]?\d{1,3})?\M}
    $w mark set last begin
    while {[set ind [$w search -regexp -count mlen -- $numregexp last ende]] != {}} {
        if {[$w compare $ind >= last]} {
            $w mark set last "$ind + $mlen c"
            remalltags $w $ind last
            $w tag add number $ind last
        } else break
    }

    # Scilab keywords
    if {$schema=="scilab"} {
        set sciChset {[A-Za-z0-9_\%\#\!\$\?]}
        $w mark set last begin
        while {[set ind [$w search -regexp $sciChset \
                            last ende]] != {}} {
            if {[$w compare $ind >= last]} {
                set kword ""
                regexp $sciChset+ [$w get $ind "$ind lineend"] kword
                if {$kword != ""} {
                    set num [string length $kword]
                    $w mark set last $ind
                    $w mark set next "$ind + $num c"
                    set initial [string range $kword 0 0]
                    foreach itag $scitags {
                        if {[string first $initial $chset(scilab.$itag)]>=0} {
                           if {[lsearch -exact $words(scilab.$itag.$initial) \
                                    $kword] != -1} {
                                $w tag add $itag last next
                           }
                        }
                    }
                    $w mark set last next+1c
                } else {
                    $w mark set last last+1c
                }
            } else break
        }
    }

    # XML (#ES this is a problem as <> are also operators)
    if {$schema=="xml"} {
        $w mark set last begin
        while {[set ind [$w search -regexp "<" last ende]] != {}} {
            if {[$w compare $ind >= last]} {
                set res ""
                regexp ">" [$w get $ind end] res
                set num [string length $res]
                $w mark set last "$ind + $num c"
                $w mark set next {last+1c wordend}
                $w tag add xmltag last-1c next+1c
                $w mark set last next+1c
            } else break
        }
    }

    # Text quoted with single/double quotes as textquoted, and Scilab remarks
    colorizestringsandcomments_sd $w [$w index begin] [$w index ende]

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
            set simpstrRE {((?:.|\A(?:))["'][^"'\n]*["'])}
        } else {
            # double quotes only
            set quotespattern {"}
            set simpstrRE {((?:.|\A(?:))"[^"'\n]*")}
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

    $w mark set last $thebegin
    set resi {}
    set simpstr {}
    set contstr {}
    set outstrcomm {}

    while {[regexp -indices -- $textcommfullRE [$w get last $theend] \
            resi simpstr contstr outstrcomm]} {
        foreach {i j} $resi {}
        set ind [$w index "last + $i c"]
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
        set num [expr $j - $i + 1]

        set done false
        # try first case: string on a single line
        if {[lindex $simpstr 0] != -1} {
            foreach {i1 j1} $simpstr {}
            if {$i == $i1 && $j == $j1} {
                # we're really in the first case
                $w mark set last "$ind +$num c"
                if {![string match $quotespattern [$w get $ind]]} {
                    # in this case we did not match at the beginning of the
                    # string but one character before the opening quote
                    set ind "$ind + 1 c"
                } else {
                    # we matched at the opening quote - no adjustment to $ind
                }
                # textquoted deletes any other tag
                remalltags $w $ind last
                $w tag add textquoted $ind last
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
                $w mark set last "$ind +$num c"
                # the comment part must be separated from the string part, and
                # this has to be done for each continued line but the last one
                if {![string match $quotespattern [$w get $ind]]} {
                    # in this case we did not match at the beginning of the
                    # string but one character before the opening quote
                    set ind "$ind + 1 c"
                } else {
                    # we matched at the opening quote - no adjustment to $ind
                }
                set subtext [$w get $ind last]
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
                remalltags $w $colstart last
                $w tag add textquoted $colstart last
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
                $w mark set last "$ind +$num c"
                # rem2 deletes any other tag
                remalltags $w $ind last
                $w tag add rem2 $ind last
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
            # this is to avoid an endless loop
            $w mark set last "$ind + $num c"
        }

        set resi {}
        set simpstr {}
        set contstr {}
        set outstrcomm {}
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

    set progressbarId [scan $progressbar $pad.cp%d]

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
    set incre 5

    set curend "$thebegin + $incre l"
    set curend [getendofcolorization $w $curend]

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

        set curend "1.0"
        while {[$w compare $curend < end]} {
            colorize $w $curend \
                    [set curend [getendofcolorization $w "$curend + 10 l"]]
            scan [$w index $curend] "%d.%d" ycur xcur
            scan [$w index end]     "%d.%d" yend xend
            SetProgress $progressbar $ycur $yend $listoffile("$w",displayedname)
            update idletasks
        }

        colorize $w [getstartofcolorization $w "$curend - 10 l"] end

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

proc changelanguage {newlanguage} {
    global listoffile
    set textarea [gettextareacur]
    set oldlanguage $listoffile("$textarea",language)
    if {$oldlanguage != $newlanguage} {
        set listoffile("$textarea",language) $newlanguage
        showinfo [mc "Wait seconds while recolorizing file"]
        schememenus $textarea
        tagcontlines $textarea
        backgroundcolorize $textarea
        keyposn $textarea
    }
}

proc schememenus {textarea} {
    global pad listoffile
    global Shift_F11 Shift_F12
    global MenuEntryId
    set dm $pad.filemenu.debug
    if {$listoffile("$textarea",language) == "scilab"} {
        #enable "Load into scilab"
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Load into Scilab"]) -state normal
        # restore bindings
        bind $pad <Control-l> {execfile}
        bind $pad <F5> {filetosave %W; execfile}
        # enable all the debug entries
        # this is set selectively in function of the debugger state
        setdbmenuentriesstates_bp
        # enable "Show continued lines"
        $pad.filemenu.options entryconfigure $MenuEntryId($pad.filemenu.options.[mcra "Show c&ontinued lines"]) -state normal
    } else {
        #disable "Load into scilab"
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Load into Scilab"]) -state disabled
        #disable all the Debug entries
        for {set i 1} {$i<=[$dm index last]} {incr i} {
            if {[$dm type $i] == "command"} {
                $dm entryconfigure $i -state disabled
            }
        }
        #disable "create help skeleton"
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help s&keleton..."]) -state disabled
        # remove bindings
        bind $pad <Control-l> {}
        bind $pad <F5> {}
        # remove debugger bindings
        bind $pad <F9> {}
        bind $pad <Control-F9> {}
        bind $pad <F10> {}
        bind $pad <Control-F11> {}
        bind $pad <F11> {}
#       pbind $pad $Shift_F11 {}
        pbind $pad $Shift_F12 {}
        bind $pad <F12> {}
        bind $pad <Control-F12> {}
        # disable "Show continued lines"
        $pad.filemenu.options entryconfigure $MenuEntryId($pad.filemenu.options.[mcra "Show c&ontinued lines"]) -state disabled
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
# We rely on BGCOLOR and  FGCOLOR to be surely members of the color lists
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
# refresh all color settings for all the opened buffers (only one color was
#  changed)
       foreach i $listoftextarea {
           TextStyles $i; update
       }
       tagcontlinesinallbuffers
   }
}

proc refreshQuotedStrings {} {
    global listoftextarea
    showinfo [mc "Wait seconds while recolorizing file"]
    foreach w $listoftextarea {
        $w tag remove rem2 1.0 end
        $w tag remove textquoted 1.0 end
        colorizestringsandcomments_sd $w 1.0 end
    }
}

proc getstartofcolorization {w ind} {
# return start bound required for proper recolorization
# and take care of nearby continued lines
    if {[iscontinuedline $w "$ind - 1 l"]} {
        set uplimit [getstartofcontline $w "$ind - 1 l"]
    } else {
        set uplimit [$w index "$ind linestart"]
    }
    return $uplimit
}

proc getendofcolorization {w ind} {
# return end bound required for proper recolorization
# and take care of nearby continued lines
    if {[iscontinuedline $w "$ind + 1 l"]} {
        set dnlimit [getendofcontline $w "$ind + 1 l"]
    } else {
        set dnlimit [$w index "$ind + 1 l lineend"]
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
