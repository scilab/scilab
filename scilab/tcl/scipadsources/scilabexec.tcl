proc execfile {{buf "current"}} {
# return argument: 0=success, 1 scilab busy, 2 cancel, -1 fail
    global listoffile sciprompt
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    if {[$textarea index end-1c] == 1.0} {
        showinfo [mc "No point in loading an empty file!"]
        return 2
    }
    set doexec 1
    if {[ismodified $textarea]} {
        set answer [tk_messageBox -message [concat [mc "The contents of"] \
                  $listoffile("$textarea",fullname) \
                  [mc "may have changed, do you wish to save your changes?"] ] \
                -title [mc "Save Confirm?"] -type yesnocancel -icon question]
        case $answer {
            yes { filetosave $textarea; set doexec 1 }
            no { set doexec 0 }
            cancel { set doexec 0; return 2 }
        }
    }
    if $doexec {
        if [ expr [string compare $sciprompt -1] == 0 ] {
            tk_messageBox -message [concat \
                     [mc "Scilab is working, wait for the prompt to load file"] \
                      $listoffile("$textarea",fullname) ] \
                        -title [mc "Scilab working"] -type ok -icon info
            return 1
        } else {
            set f $listoffile("$textarea",fullname)
            if {[catch {ScilabEval_lt "exec(\"$f\");" "sync" "seq"}]} {
                scilaberror $listoffile("$textarea",fullname)
                return -1
            } else {
                showinfo [mc "Exec done"]
                return 0 
            }
        }
    }
}

proc execselection {} {
    global sciprompt textareacur tcl_platform

    # execselection cannot be executed since it needs the colorization results
    if {[colorizationinprogress]} {return}

    if [ expr [string compare $sciprompt -1] == 0 ] {
       tk_messageBox -message \
               [mc "Scilab is working, wait for the prompt to execute the selection."] \
            -title [mc "Scilab working"] -type ok -icon info
     } else {
        set seltexts [selection own]
        if {[string range $seltexts 0 [expr [string length $textareacur]-1]]\
                 == $textareacur} {
            if [catch {selection get -selection PRIMARY} sel] {
            } else {
                set f [selection get]
                #SciEval does not digest multilines, nor comments. The following hacks are 
                # not optimal - they can produce very long lines, and get confused about 
                # quoted strings containing //.
                #strip comments from // to \n (note - \n stays, as the interpreter allows 
                #    "...//bla\n rest" ) (NOTE: this way strings like "...//..." are truncated 
                #    -- FIXIT -- has to use tag textquoted information)
                regsub -all -line "//.*(\\n|\\Z)" $f "\n" f1
                unset f
                # remove trailing white space
                regsub -all -line "^\\s*" $f1 " " f2
                unset f1
                #join continued lines
                regsub -all -line "\\.{2,} *\\n" $f2 "" f3
                unset f2
                #join multilines with ";"
                regsub -all -line "\\n" $f3 ";" comm
                unset f3
                # last hack - add a final endfunction if there is an unterminated
                # function in the selection: TODO (try to involve proc whichfun)
                # Things are complicated because the selection may either include
                #  the originating "function" or not
                set i1 [$textareacur index sel.first]
                set i2 [$textareacur index sel.last]
#TODO ES 9/10/03
#                if { $i2>$i1 } {
#                    set funselstart [lindex [whichfun $i1] 0]
#                    set funselend [lindex [whichfun $i2] 0]
#                } else {
#                    set funselstart [lindex [whichfun $i2] 0]
#                    set funselend [lindex [whichfun $i1] 0]
#                }
#                tk_messageBox -message $funselstart"--"$funselend
#                if { $funselend !={} && $funselstart == {}} {
#                    append comm ",endfunction"}
#                    if { $funselend !={} && $funselstart != $funselend} {
#                        tk_messageBox -message \
#                            "How do you pretend Scilab to evaluate the bottom of a function definition without its header?"
#                        return
#                }

                # Besides, I'd like to see screen output too.
                regsub -all -line "\"" $comm "\"\"" dispcomm
                regsub -all -line "'" $dispcomm "''" dispcomm1
                unset dispcomm
                # The following test is to cope with string length limits in C language using %s
                # The hardwired limit in character length is 509-13 since (quote from the MSDN
                # Library - Oct 2001):
                # ANSI compatibility requires a compiler to accept up to 509 characters in a string
                # literal after concatenation. The maximum length of a string literal allowed in
                # Microsoft C is approximately 2,048 bytes.
                # (end of quote)
                # Because I don't know the limit for other compilers, I keep 509 as the maximum
                # above which the string is not displayed. Anyway, more than this is very hard
                # to read in the Scilab shell.
                if {[string length $dispcomm1] < 496} {
                    ScilabEval_lt "mprintf(\"%s\\n\",\"$dispcomm1\")"
                }
                ScilabEval_lt $comm
            }
        }
    }
}

proc importmatlab {} {
    global pad listoffile sciprompt
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    if [ expr [string compare $sciprompt -1] == 0 ] {
        tk_messageBox -message \
                [mc "Scilab is working, wait for the prompt to convert a Matlab file."] \
                -title [mc "Scilab working"] -type ok -icon info
     } else {
        set matfiles [mc "Matlab files"]
        set allfiles [mc "All files"]
        set types [concat "{\"$matfiles\"" "{*.m}}" \
                          "{\"$allfiles\"" "{* *.*}}" ]
        set dtitle [mc "Matlab file to convert"]
        set sourcefile [tk_getOpenFile -filetypes $types -parent $pad -title "$dtitle"]
        if {$sourcefile !=""} {
            set sourcedir [file dirname $sourcefile]
            set destfile [file rootname $sourcefile].sci 
            set convcomm "execstr(\"res=mfile2sci(\"\"$sourcefile\"\",\
                          \"\"$sourcedir\"\",%f,%f,1,%t)\",\"errcatch\",\"m\")"
            set impcomm \
                "if $convcomm==0 then \
                   TCL_EvalStr(\"scipad eval {delinfo; openfile \"\"$destfile\"\"} \"); \
                 else; \
                   TCL_EvalStr(\"scipad eval {failmatlabimp} \");\
                 end"
            showinfo [mc "Scilab is converting, please hold on..." ]
            ScilabEval_lt $impcomm "sync" "seq"
        }
    }
}

proc failmatlabimp {} {
    tk_messageBox -title [mc "Matlab file import"]  \
      -message [mc "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"] \
      -icon error
}

proc helpskeleton {} {
    global listoffile
    # first exec the file in scilab, so that the current function is
    #  really defined; then call help_skeleton, and pipe the
    # result to a new (unsaved) buffer.
    # NB: execing the file can have far-reaching consequences
    #  if the file does more than just defining functions. 
    # Responsibility left to the user.   
    global tileprocalreadyrunning
    if {$tileprocalreadyrunning} {return}
    set indexin [[gettextareacur] index insert]
    scan $indexin "%d.%d" ypos xpos
    set infun [whichfun $indexin]
    set funname [lindex $infun 0]
    if {[execfile]==0} {
        set pathprompt [mc "Please select destination path for the xml source of the help file" ]
        set dir [tk_chooseDirectory -title $pathprompt]
        if {$dir != ""} {
            set xmlfile [file join $dir $funname.xml]
            set warntitle [concat [mc "Older version of"] $xmlfile [mc "found!"] ]
            set warnquest [concat [mc "An old version of"] $xmlfile [mc "already exists: open the old file instead?"] ]
            set warnold [mc "Existing file" ]
            set warnnew [mc "New skeleton"]
            if [file exists $xmlfile] {
                set answer [tk_dialog .existxml $warntitle $warnquest \
                      questhead 0 $warnold $warnnew]
            } else {
                set answer 1
            }
            if $answer {
                  ScilabEval_lt "help_skeleton(\"$funname\",\"$dir\")" "sync"
            }
            openfile $xmlfile
        }
    }
}

proc xmlhelpfile {} {
    global listoffile sciprompt
    # save the file and call xmlfiletohtml. Catch and popup the error messages.
    filetosavecur
    if [ expr [string compare $sciprompt -1] == 0 ] {
        tk_messageBox -message \
              [mc "Scilab is working, wait for the prompt to compile the help file."] \
            -title [mc "Scilab working"] -type ok -icon info
    } else {
        set filetocomp $listoffile("[gettextareacur]",fullname)
        set filename [file tail    $filetocomp]
        set filepath [file dirname $filetocomp]
        set cwd [pwd]
        cd $filepath
        ScilabEval "xmlfiletohtml(\"$filename\")" sync
        cd $cwd
    }
}

proc ScilabEval_lt {comm {opt1 ""} {opt2 ""}} {
# ScilabEval with length test
# This is needed because ScilabEval cannot accept commands longer than bsiz (they
# are truncated by Scilab). Workaround: Long commands shall be saved into a file
# that is exec'ed by ScilabEval.
# This proc checks first the length of the command passed to ScilabEval.
# If this length is smaller than bsiz-1, pass the command to ScilabEval for execution.
# If this length is greater than bsiz-1, save the command in a file and do a
# ScilabEval exec("the_file"). If this fails (wrong permission rights...) then warn the
# user that something really weird might happen since there is no way to pass the
# command to Scilab.
    global tmpdir
    set bsiz_1 4095   ;# Must be consistent with bsiz defined in routines/stack.h
    if {[string length $comm] <= $bsiz_1} {
        # No problem to process this
        ScilabEval $comm $opt1 $opt2
    } else {
        # Command is too long
        # Create a file in tmpdir, and save the command in it.
        # Large (>$splitsize) commands are splitted into smaller parts, and trailing dots
        # are added.
        # This part is catched to take into account possible access (permissions) errors
        if {[catch {
            set fname [file join $tmpdir "ScilabEval_command.sce"]
            set splitsize 4000 ;# arbitrary but works up to approx. 4095
            set nbparts [expr [string length $comm] / $splitsize + 1]
            set fid [open $fname w]
            puts $fid "mode(-1);"
            set startpos 0
            for {set i 1} {$i < $nbparts} {incr i} {
                set stoppos  [expr $i * $splitsize - 1]
                # Warning: the string must not be split (.. added) just after a dot!
                # Here possible endless loop if $comm contains only dots, but why would this happen?
                while {[string index $comm $stoppos] == "."} {incr stoppos}
                puts $fid "[string range $comm $startpos $stoppos].."
                set startpos [incr stoppos]
            }
            puts $fid [string range $comm $stoppos end]
            close $fid
            ScilabEval "exec(\"$fname\")" $opt1 $opt2
        }] != 0} {
            tk_messageBox  -title [mc "ScilabEval command cannot be passed to Scilab!"] -icon warning -type ok \
                           -message [concat [mc impossibleScilabEval_message] "ScilabEval" $comm $opt1 $opt2]
        }

    }
}
