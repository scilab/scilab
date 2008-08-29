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

proc execfile {{buf "current"}} {
# return argument: 0=success, 1=scilab busy, 2=cancel overwrite, -1=fail
    global listoffile pad
    global tmpdir

    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }

    if {[$textarea index end-1c] == 1.0} {
        showinfo [mc "No point in loading an empty file!"]
        return 2
    }

    if {[isscilabbusy 1 $listoffile("$textarea",fullname)]} {return 1}

    set savedintempdir false

    if {[ismodified $textarea]} {
        # try to save the file in a temporary directory
        set nametosave [file join $tmpdir [file tail $listoffile("$textarea",fullname)]]
        if {[catch {writefileondisk $textarea $nametosave 0}] != 0} {
            set answer [tk_messageBox -title [mc "Silent file save failed"] \
                    -icon question -type yesnocancel \
                    -message [mc "File could not be saved in a temporary location.\nOverwrite original file?"] ]
            switch -- $answer {
                yes    { filetosave $textarea; set f $listoffile("$textarea",fullname); set doexec 1 }
                no     { set doexec 0 }
                cancel { set doexec 0; return 2 }
            }
        } else {
            set f $nametosave
            set doexec 1
            set savedintempdir true
        }
    } else {
        # file is not modified wrt to its version on disk - use this version on disk
        set f $listoffile("$textarea",fullname)
        set doexec 1
    }

    if $doexec {
        if {[catch {ScilabEval_lt "exec(\"$f\");" "sync" "seq"}]} {
            scilaberror $listoffile("$textarea",fullname)
            set outval -1
        } else {
            showinfo [mc "Exec done"]
            set outval 0
        }
    }

    if {$savedintempdir} {
        catch {file delete -- $f}
    }

    # this is in case a script modifies a file opened in Scipad
    checkifanythingchangedondisk $pad

    return $outval
}

proc execselection {} {
# run the Scipad selection in Scilab
# note: block selection is supported
    global tcl_platform pad

    # execselection cannot be executed since it needs the colorization results
    if {[colorizationinprogress]} {return}

    if {[isscilabbusy 2]} {return}

    set textareacur [gettextareacur]
    set selindices [gettaselind $textareacur any]
    if {$selindices != ""} {
        set f [gettatextstring $textareacur $selindices]
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
#        if { $i2>$i1 } {
#            set funselstart [lindex [whichfun $i1] 0]
#            set funselend [lindex [whichfun $i2] 0]
#        } else {
#            set funselstart [lindex [whichfun $i2] 0]
#            set funselend [lindex [whichfun $i1] 0]
#        }
#        tk_messageBox -message $funselstart"--"$funselend
#        if { $funselend !={} && $funselstart == {}} {
#            append comm ",endfunction"}
#            if { $funselend !={} && $funselstart != $funselend} {
#                tk_messageBox -message \
#                    "How do you pretend Scilab to evaluate the bottom of a function definition without its header?"
#                return
#        }
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
        # this is in case the evaluated script modifies a file opened in Scipad
        checkifanythingchangedondisk $pad
    }
}

proc importmatlab {} {
    global pad listoffile
    global tileprocalreadyrunning
    global bug2672_shows_up Tk85
    global preselectedfilterinimportmatlabbox

    if {$tileprocalreadyrunning} {return}

    if {[isscilabbusy 3]} {return}

    set matfiles [mc "Matlab files"]
    set allfiles [mc "All files"]
    set types [concat "{\"$matfiles\"" "{*.m}}" \
                      "{\"$allfiles\"" "{* *.*}}" ]
    set dtitle [mc "Matlab file to convert"]
    if {$Tk85} {
        # make use of TIP242 (-typevariable option)
        # note that $bug2672_shows_up is necessary false (see
        # definition of bug2672_shows_up)
            set sourcefile [tk_getOpenFile -filetypes $types -parent $pad -title "$dtitle" \
                                           -typevariable preselectedfilterinimportmatlabbox]
    } else {
        if {$bug2672_shows_up} {
            set sourcefile [tk_getOpenFile -filetypes $types -title "$dtitle"]
        } else {
            set sourcefile [tk_getOpenFile -filetypes $types -parent $pad -title "$dtitle"]
        }
    }
    if {$sourcefile !=""} {
        set sourcedir [file dirname $sourcefile]
        set destfile [file rootname $sourcefile].sci
        set convcomm "execstr(\"res=mfile2sci(\"\"$sourcefile\"\",\
                      \"\"$sourcedir\"\",%f,%f,1,%t)\",\"errcatch\",\"m\")"
        set impcomm \
            "if $convcomm==0 then \
               TCL_EvalStr(\"delinfo; openfile \"\"$destfile\"\"\",\"scipad\"); \
             else; \
               TCL_EvalStr(\"failmatlabimp\",\"scipad\");\
             end"
        showinfo [mc "Scilab is converting, please hold on..." ]
        ScilabEval_lt $impcomm "sync" "seq"
    }
}

proc failmatlabimp {} {
    global ScilabBugzillaURL
    tk_messageBox -title [mc "Matlab file import"]  \
      -message [concat [mc "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to"] \
                    $ScilabBugzillaURL] \
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
    if {[isscilabbusy 0]} {return}
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
# save the file and call xmlfiletohtml (in Scilab 4), or do nothing
# (in Scilab 5) because of bug 3015

    global listoffile
    global Scilab5

    if {[isscilabbusy 4]} {return}

    if {$Scilab5} {
        pleaseuseabetterscilabversion 3015
    
    } else {
        filetosavecur
        set filetocomp $listoffile("[gettextareacur]",fullname)
        set filename [file tail    $filetocomp]
        set filepath [file dirname $filetocomp]
        set cwd [pwd]
        cd $filepath
        ScilabEval_lt "xmlfiletohtml(\"$filename\")" sync
        cd $cwd
    }
}

proc ScilabEval_lt {comm {opt1 ""} {opt2 ""}} {
# ScilabEval with length test
# This is needed because ScilabEval cannot accept commands longer than bsiz
# (they are truncated by Scilab). Workaround: Long commands shall be saved
# into a file that is exec'ed by ScilabEval.
# This proc checks first the length of the command passed to ScilabEval.
# - If this length is smaller than bsiz-1, pass the command to ScilabEval for
# execution.
# - If this length is greater than bsiz-1 but smaller than lsiz-1, save the
# command in a file and do a ScilabEval exec("the_file"). If this fails
# (wrong permission rights...) then warn the user that something really weird
# might happen since there is no way to pass the command to Scilab.
# - If this length is greater than lsiz-1, warn the user that the command
# cannot be passed to Scilab

    # this global solves bugs 1848 and 1853 even if sciprompt is not used in proc ScilabEval_lt
    global sciprompt

    global tmpdir
    set bsiz_1  4095   ;# Must be consistent with bsiz defined in stack.h
    set lsiz_1 65535   ;# Must be consistent with lsiz defined in stack.h
    set commlength [string length $comm]
    if {$commlength <= $bsiz_1} {
        # No problem to process this
        ScilabEval $comm $opt1 $opt2
    } elseif {$commlength <= $lsiz_1} {
        # Command is too long for a direct ScilabEval but can be passed through an exec'ed file
        # Create a file in tmpdir, and save the command in it.
        # Large (>$splitsize) commands are splitted into smaller parts, and trailing dots
        # are added.
        # This part is catched to take into account possible access (permissions) errors
        if {[catch {
            set fname [file join $tmpdir "ScilabEval_command.sce"]
            set splitsize 4000 ;# arbitrary but works up to approx. 4095
            set nbparts [expr {[string length $comm] / $splitsize + 1}]
            set fid [open $fname w]
            set startpos 0
            for {set i 1} {$i < $nbparts} {incr i} {
                set stoppos  [expr {$i * $splitsize - 1}]
                # Warning: the string must not be split (.. added) just after a dot!
                # Here possible endless loop if $comm contains only dots, but why would this happen?
                while {[string index $comm $stoppos] == "."} {incr stoppos}
                puts $fid "[string range $comm $startpos $stoppos].."
                set startpos [incr stoppos]
            }
            puts $fid [string range $comm $stoppos end]
            close $fid
            ScilabEval "exec(\"$fname\");" $opt1 $opt2
        }] != 0} {
            tk_messageBox  -title [mc "ScilabEval command cannot be passed to Scilab!"] -icon warning -type ok \
                           -message [concat [mc impossibleScilabEval_message] "ScilabEval" $comm $opt1 $opt2]
        }
    } else {
        # Command is definitely too long to be passed to Scilab, even if exec'ed in a file
        # If the command was nevertheless sent, it would trigger error 108
        # Even tk_messageBox does not accept too large -message content
        set comm [concat "[string range $comm 0 4000]..." [mc "(end of command skipped)"] ]
        tk_messageBox  -title [mc "ScilabEval command cannot be passed to Scilab!"] -icon warning -type ok \
                       -message [concat [mc impossibleScilabEval_message2] "ScilabEval" $comm $opt1 $opt2]
    }
}

proc cleantmpScilabEvalfile {} {
# Try to remove the possibly existing files in tmpdir
# created by ScilabEval_lt
    global tmpdir
    catch {file delete [file join $tmpdir "ScilabEval_command.sce"]}
}

proc isscilabbusy {{messagenumber "nomessage"} args} {
# check if Scilab is busy or not
# return true if busy, and false if idle
# $messagenumber, if present, gives a message id to display in a message box
# additional arguments may be passed to customize the message
# if $messagenumber is not given, then no message will be displayed and the
# test on Scilab idleness is silent
    global sciprompt
    if {[string compare $sciprompt -1] == 0} {
        if {$messagenumber == "nomessage"} {
            return true
        }
        switch -exact -- $messagenumber {
            0 { set mes \
                [mc "Scilab is working, please wait for the prompt to execute this command!"]
              }
            1 { set mes [concat \
                [mc "Scilab is working, wait for the prompt to load file"] \
                 [lindex $args 0] ]
              }
            2 { set mes \
                [mc "Scilab is working, wait for the prompt to execute the selection."]
              }
            3 { set mes \
                [mc "Scilab is working, wait for the prompt to convert a Matlab file."]
              }
            4 { set mes \
                [mc "Scilab is working, wait for the prompt to compile the help file."]
              }
            5 { set mes \
                [mc "Scilab is working, wait for the prompt to issue a debugger command."]
              }
            default { set mes \
                "Unexpected message number in proc isscilabbusy - Please report."
            }
        }
        tk_messageBox -message $mes -title [mc "Scilab working"] -type ok -icon info
        return true
    } else {
        return false
    }
}

proc scilaberror {funnameargs} {
    global ScilabErrorMessageBox
    global errnum errline errmsg errfunc
    # Communication between Scipad and Scilab through
    # Tcl global interp is not supported by Scilab 5
    # See http://wiki.scilab.org/Tcl_Thread
    global Scilab5
    if {$ScilabErrorMessageBox} {
        if {$Scilab5} {
            # Let Scilab Display the message so that there is no more locking process
            # for communication between Tcl and Scilab.
            # Lasterror outputs are already known in scilab, let's use them.
            #
            # We need to escape ' in scilab strings using ''
            # Used after Scipad Localisation => string map {' ''}
            #
            ScilabEval_lt "\[db_str,db_n,db_l,db_func\]=lasterror();" "sync" "seq"
            set shellError [string map {' ''} [concat [mc "The shell reported an error while trying to execute "] $funnameargs [mc ": error "]]]
            set lineError [string map {' ''} [mc "at line "]]
            set funcError [string map {' ''} [mc " of "]]
            set winTitle [string map {' ''} [mc "Scilab execution error"]]
            ScilabEval_lt "messagebox(\[\"$shellError\"+string(db_n); \
                                        db_str; \
                                        \"$lineError\"+string(db_l)+\"$funcError\"+string(db_func)\], \"$winTitle\", \"error\")" \
                          "sync" "seq"
        } else {
            ScilabEval_lt "\[db_str,db_n,db_l,db_func\]=lasterror();" "sync" "seq"
            ScilabEval_lt  "TCL_SetVar(\"errnum\", string(db_n), \"scipad\");" "sync" "seq"
            ScilabEval_lt  "TCL_SetVar(\"errline\", string(db_l), \"scipad\");" "sync" "seq"
            ScilabEval_lt  "TCL_SetVar(\"errfunc\", strsubst(db_func,\"\"\"\",\"\\\"\"\"), \"scipad\")" "sync" "seq"
            ScilabEval_lt  "TCL_SetVar(\"errmsg\" , strsubst( \
                                             strsubst( \
                                             strsubst( \
                                             strsubst( \
                                             strsubst( \
                                                        strcat(stripblanks(db_str),ascii(13)) \
                                                              ,\"\"\"\",\"\\\"\"\") \
                                                              ,\"''\",\"\\''\") \
                                                              ,\"$\",\"\\$\") \
                                                              ,\"\[\",\"\\\[\") \
                                                              ,\"\]\",\"\\\]\") \
                                           , \"scipad\" )" "sync" "seq"
            tk_messageBox -title [mc "Scilab execution error"] \
                -message [append dummyvar [mc "The shell reported an error while trying to execute "]\
                              $funnameargs [mc ": error "] $errnum "\n" $errmsg "\n" [mc "at line "]\
                              $errline [mc " of "] $errfunc]
        }
    }
    showinfo [mc "Execution aborted!"]
    if {[getdbstate] != "NoDebug"} {
        canceldebug_bp
    }
    blinkline $errline $errfunc
}

proc blinkline {li ma {nb 3}} {
# Blink $nb times logical line $li in macro function named $ma
# The macro is supposed to be defined in one of the opened buffers (no
# opening of files occur here)
# Warning: This proc is also used from outside of Scipad by edit_error
    global SELCOLOR
    set funtogoto [funnametofunnametafunstart $ma]
    if {$funtogoto != ""} {
        dogotoline "logical" $li "function" $funtogoto
        set w [lindex $funtogoto 1]
        set i1 [$w index "insert linestart"]
        set i2 [$w index "insert lineend + 1c"]
        for {set i 0} {$i < $nb} {incr i} {
            $w tag add blinkedline $i1 $i2
            $w tag configure blinkedline -background $SELCOLOR
            update idletasks
            after 500
            $w tag remove blinkedline $i1 $i2
            update idletasks
            # do not wait when it's the last blink:
            # control is given back to the caller ASAP
            if {$i < $nb} {
                after 500
            }
        }
    } else {
        # function not found among opened buffers, do nothing
    }
}
