proc load_words {} {
    global words chset env

    set ownpath "$env(SCIPATH)/tcl/scipadsources"
    #load the keywords which cannot be yet derived from scilab, from 
    #  the "word" file (control keywords, internal functions, scicos palettes)
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
    close $f
    # ask to scilab about keywords: the scripts sets at run time
    #  chset(scilab.predef.$) and chset(scilab.libfun.$)
    set chset(scilab.predef) {}
    set chset(scilab.libfun) {}
    ScilabEval "exec $ownpath/dynamickeywords.sce;" "seq"
}

proc remalltags {w begin ende} {
    $w tag remove parenthesis $begin $ende
    $w tag remove bracket $begin $ende
    $w tag remove brace $begin $ende
    $w tag remove punct $begin $ende
    $w tag remove operator $begin $ende
    $w tag remove number $begin $ende
    $w tag remove keywords $begin $ende
    $w tag remove predef $begin $ende
    $w tag remove libfun $begin $ende
    $w tag remove text $begin $ende
    $w tag remove rem2 $begin $ende
    $w tag remove xmltag $begin $ende
    $w tag remove textquoted $begin $ende  
    $w tag remove indentation $begin $ende
}

proc colorize {w cpos iend} {
    global words chset listoffile scilabSingleQuotedStrings
    set num 0
    set textarea [gettextareacur]
    set schema $listoffile("$textarea",language)
    $w mark set begin "$cpos linestart"
    $w mark set ende "$iend+1l linestart"
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

    #ES: why at all call ":" a "brace? 
    # "{}" are anyway parsed above for matching pairs
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
    # (the regexp has to be perfectioned -- it matches e.g. single e6)
    set numregexp {\m\d*\.?\d*([deDE][+\-]?\d{1,3})?\M}
    #	set numregexp {\m(\d+|\.\d+|\d+\.\d*)([deDE][+\-]?\d{1,3})?\M}
    $w mark set last begin
    while {[set ind [$w search -regexp $numregexp last ende]] != {}} {
        if {[$w compare $ind >= last]} {
            $w mark set last $ind+1c
            remalltags $w $ind last
            $w tag add number $ind last
        } else break
    }

    # Scilab keywords
    if {$schema=="scilab"} {
        set sciChset {[A-Za-z0-9_\%\#\!\$\?]}
        $w mark set last begin
        set displ 0
        while {[set ind [$w search -count displ -regexp $sciChset \
                            last ende]] != {}} {
            if {[$w compare $ind >= last]} {
                set kword ""
                regexp $sciChset+ [$w get $ind "$ind lineend"] kword
                if {$kword != ""} {
                    set num [string length $kword]
                    $w mark set last $ind
                    $w mark set next "$ind + $num c"
                    set initial [string range $kword 0 0]
                    if {[string first $initial $chset(scilab.col1)]>=0} {
                        if {[lsearch -exact $words(scilab.col1.$initial) \
                                    $kword] != -1} {
                            $w tag add keywords last next
                        }
                    }
                    if {[string first $initial $chset(scilab.predef)]>=0} {
                        if {[lsearch -exact $words(scilab.predef.$initial) \
                                    $kword] != -1} {
                            $w tag add predef last next
                        }
                    }
                    if {[string first $initial $chset(scilab.libfun)]>=0} {
                        if {[lsearch -exact $words(scilab.libfun.$initial) \
                                    $kword] != -1} {
                            $w tag add libfun last next
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
        while {[set ind [$w search  -regexp "<" last ende]] != {}} {
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

    # tag 'sometext' with single quotes as textquoted
     #let it be optional -- but be aware that if the option is changed, 
     # it will affect only tagging of the subsequently added text.
     # if the option is to be changed within scilab, the change
     # has to trigger a recolorization of all buffers 
    if {$scilabSingleQuotedStrings == "yes"} {
        $w mark set last begin
        while { [set ind [$w search -count num -regexp \
                              {'[^']*('|$)} last ende]] != {}} {
            if {[$w compare $ind >= last]} {
                set res ""
                $w mark set endetext "$ind lineend"
                regexp {'[^']*'} [$w get last endetext] res
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
    }

    # scilab remark
    if {$schema=="scilab"} {
        $w mark set last begin
        while {[set ind [$w search -exact {//} last ende]] != {}} {
            if {[$w compare $ind >= last]} {
                $w mark set last "$ind+1l linestart"
                # tags as rem2 only if not already textquoted
                # (does not tag as comment "..//..")
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


proc changelanguage {newlanguage} {
    global listoffile
    set textarea [gettextareacur]
    set oldlanguage $listoffile("$textarea",language)
    if {$oldlanguage != $newlanguage} {
        set listoffile("$textarea",language) $newlanguage
        showinfo [mc "Wait seconds while recolorizing file"]
        schememenus $textarea
        colorize $textarea 1.0 end
        keyposn $textarea
    }
}

proc schememenus {textarea} {
    global pad listoffile
    set dm $pad.filemenu.debug
    if {$listoffile("$textarea",language) == "scilab"} {
        #enable "Load into scilab"
        $pad.filemenu.exec entryconfigure 1 -state normal
        # restore bindings
        bind $pad <Control-l> {execfile}
        bind $pad <F5> {filetosave %W; execfile}
        # enable all the debug entries
        # this is set selectively in function of the debugger state
        setdbmenuentriesstates_bp
    } else {
        #disable "Load into scilab"
        $pad.filemenu.exec entryconfigure 1 -state disabled
        #disable all the Debug entries
        for {set i 1} {$i<=[$dm index last]} {incr i} {
            if {[$dm type $i] == "command"} {
                $dm entryconfigure $i -state disabled
            }
        }
        #disable "create help skeleton"
        $pad.filemenu.files entryconfigure 8 -state disabled
        # remove bindings
        bind $pad <Control-l> {}
        bind $pad <F5> {}
        # remove debugger bindings
        bind $pad <F9> {}
        bind $pad <Control-F9> {}
        bind $pad <F10> {}
        bind $pad <Control-F11> {}
        bind $pad <F11> {}
#       bind $pad <Shift-F11> {}
        bind $pad <Shift-F12> {}
        bind $pad <F12> {}
        bind $pad <Control-F12> {}
    }
    if {$listoffile("$textarea",language) == "xml"} {
        $pad.filemenu.files entryconfigure 9 -state normal
    } else {
        $pad.filemenu.files entryconfigure 9 -state disabled
    }
}

proc colormenuoption {c} {
# called when changing a color option from menu. Chooses the new color and
# refreshes whatever needed
   global colorpref listoftextarea pad
   foreach c1 $colorpref {global $c1}
   set newcol [tk_chooseColor -initialcolor [set $c] -title [mc $c]]
   if {$newcol != ""} {
       set $c $newcol
       set i 0
# refresh all the colors of the menu labels (one was changed)
       foreach c $colorpref {
           incr i
           $pad.filemenu.options.colors entryconfigure $i -foreground [set $c] 
       }
# refresh all color settings for all the opened buffers (only one color was
#  changed)
       foreach i $listoftextarea {
           TextStyles $i; update
           }
   }
}


proc refreshQuotedStrings {} {
    global listoftextarea scilabSingleQuotedStrings
    showinfo [mc "Wait seconds while recolorizing file"]
    foreach w $listoftextarea {
        $w mark set begin 1.0
        $w mark set ende end
        $w mark set last begin
        if {$scilabSingleQuotedStrings=="yes"} {
#this is taken directly from proc colorize
          while { [set ind [$w search -count num -regexp \
                              {'[^']*('|$)} last ende]] != {}} {
            if {[$w compare $ind >= last]} {
                set res ""
                $w mark set endetext "$ind lineend"
                regexp {'[^']*'} [$w get last endetext] res
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
        } else {
            while {[set ind [$w tag nextrange textquoted last end]] !={} } {
              set i1 [lindex $ind 0]
              set i2 [lindex $ind 1]
              if {[$w get $i1] == "\'" } {
                   colorize $w $i1 $i2
                  }
              $w mark set last $i2+1c
            }
        }
    }
}
