proc checkscilabbusy {{mb "message"}} {
    global sciprompt lang
    if [ expr [string compare $sciprompt -1] == 0 ] {
        if {$mb != "nomessage"} {
            if {$lang == "eng"} {
                set mes "Scilab is working, wait for the prompt to issue a\
                     debugger command."
                set tit "Scilab working"
            } else {
                set mes "Scilab est occupé, attendez le prompt pour\
                     effectuer des commandes de débug."
                set tit "Scilab occupé"
            }
            tk_messageBox -message $mes -title $tit -type ok -icon info
        }
        return "busy"
    } else {
        return "OK"
    }
}

proc execfile_bp {} {
    global funnameargs listoftextarea funsinbuffer waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        set removecomm [removescilab_bp "no_output"]
        set setbpcomm ""
        foreach textarea $listoftextarea {
            set tagranges [$textarea tag ranges breakpoint]
    #        set nlins -1
     #       set nlins -2
            foreach {tstart tstop} $tagranges {
                set infun [whichfun [$textarea index $tstart] $textarea]
                if {$infun !={} } {
                    set funname [lindex $infun 0]
                    set lineinfun [expr [lindex $infun 1] - 1]
                    set setbpcomm [concat $setbpcomm "setbpt(\"$funname\",$lineinfun);"]
               } else {
                    # <TODO> .sce case: I thought about:
                    # - inserting pause before each bp, or
                    # - inserting mode(6) plus mode(0) before each bp
                    # but none is satisfactory. Using mode() will fail in loops,
                    # and pause is very limited (no way to add a new bp during debug,
                    # or to remove all bp to finish execution ignoring them)
    #                incr nlins 1
     #               incr nlins 2
    #                $textarea insert "$tstart +$nlins l linestart" "pause\n"
     #               $textarea insert "$tstart +$nlins l linestart" "mode(6)\nmode(0)\n"
                }
            }
        }
        if {$funnameargs != ""} {
            execfile "current"
            set funname [string range $funnameargs 0 [expr [string first "(" $funnameargs] - 1]]
            foreach textarea $listoftextarea {
                if {[info exists funsinbuffer($textarea)]} {
                    if {[lsearch $funsinbuffer($textarea) $funname] != -1 && \
                         $textarea != [gettextareacur]} {
                        execfile $textarea
                    }
                }
            }
# Leading spaces here to avoid possible loss of first characters
# However this does not work for long lines created by execfile (i.e.
# lines that take a long time to display or execute in Scilab window)
            if {$setbpcomm != ""} {
                setdbstate "DebugInProgress"
                set commnvars [createsetinscishellcomm]
                set watchsetcomm [lindex $commnvars 0]
                if {$watchsetcomm != ""} {
                    ScilabEval "     $watchsetcomm"  "seq"
                }
                while {[checkscilabbusy "nomessage"] == "busy"} {}
                ScilabEval "     $setbpcomm; $funnameargs,$removecomm"  "seq"
                set filename [creategetfromshellcomm]
                if {$filename != "emptyfile"} {
                    ScilabEval "     exec(\"$filename\");"  "seq"
                }
                while {[checkscilabbusy "nomessage"] == "busy"} {}
                updateactivebreakpoint
            } else {
                ScilabEval "     $funnameargs"  "seq"
            }
        } else {
            # <TODO> .sce case
##            execfile
        }
#        showinfo " "
    }
}

proc stepbystep_bp {} {
# <TODO> step by step support
# I have no satisfactory solution for the time being.
# The heart of the matter with step by step execution is that
# once the execution is stopped there is no way of knowing what is the next
# line of code to execute. Of course, it is usually the next code line in the
# sci file, but this is not necessarily true in for, while, if, and case
# structures. I do not foresee any other remedy than a complete code analysis
# performed in tcl (!), but this is a huge task I'm not prepared to go into.
# Moreover, all this analysis is already (and surely better) done by the
# Scilab interpreter, therefore the best way would probably be to add a new
# Scilab function that would return the line number of the next instruction to
# be executed. This should no be such a tricky thing to do drawing inspiration
# e.g. from setbpt. 
    tk_messageBox -message "Sorry, step execution not yet implemented!"
}

proc resume_bp {} {
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            set commnvars [createsetinscishellcomm]
            set watchsetcomm [lindex $commnvars 0]
            if {$watchsetcomm != ""} {
                ScilabEval "     $watchsetcomm"  "seq"
                set returnwithvars [lindex $commnvars 1]
                while {[checkscilabbusy "nomessage"] == "busy"} {}
    # [..]=resume(..) was bugged (see bug 818)
    # The changes made by the user in the watch window were not reflected in the calling
    # workspace in Scilab. The correction is part of CVS from 18/06/04 or later, and
    # it will be included in Scilab 3.0
                ScilabEval "     $returnwithvars"  "seq"
            } else {
                while {[checkscilabbusy "nomessage"] == "busy"} {}
                ScilabEval "     resume(0)"
            }
            set filename [creategetfromshellcomm]
            if {$filename != "emptyfile"} {
                ScilabEval "     exec(\"$filename\");"  "seq"
            }
            while {[checkscilabbusy "nomessage"] == "busy"} {}
            updateactivebreakpoint
            while {[checkscilabbusy "nomessage"] == "busy"} {}
            checkendofdebug_bp
        } else {
            # <TODO> .sce case
            # Sending \n is if mode(6) mode(0) is used. If pause, there is no
            # need to distinguish between .sci and .sce (resume is sent for both)
 #           ScilabEval " "
        }
#        showinfo " "
    }
}

proc goonwo_bp {} {
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            [gettextareacur] tag remove activebreakpoint 1.0 end
            removescilab_bp "with_output"
            ScilabEval "     resume(0)" 
            while {[checkscilabbusy "nomessage"] == "busy"} {}
            set filename [creategetfromshellcomm]
            if {$filename != "emptyfile"} {
                ScilabEval "     exec(\"$filename\");"  "seq"
            }
        }
        setdbstate "ReadyForDebug"
#        showinfo " "
    }
}

proc dispcallstack_bp {} {
    if {[checkscilabbusy] == "OK"} {
        ScilabEval " whereami()"
    }
}

proc canceldebug_bp {} {
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            [gettextareacur] tag remove activebreakpoint 1.0 end
            removescilab_bp "with_output"
            ScilabEval "     abort"
            while {[checkscilabbusy "nomessage"] == "busy"} {}
            set filename [creategetfromshellcomm]
            if {$filename != "emptyfile"} {
                ScilabEval "     exec(\"$filename\");"  "seq"
            }
        }
        setdbstate "NoDebug"
#        showinfo " "
    }
}
