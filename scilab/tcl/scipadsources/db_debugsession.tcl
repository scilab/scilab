proc checkscilabbusy {{mb "message"}} {
    global sciprompt
# <TODO> Remove the bypass of this proc
#return "OK"
    if [ expr [string compare $sciprompt -1] == 0 ] {
        if {$mb != "nomessage"} {
            set mes [mc "Scilab is working, wait for the prompt to issue a debugger command."]
            set tit [mc "Scilab working"]
            tk_messageBox -message $mes -title $tit -type ok -icon info
        }
        return "busy"
    } else {
        return "OK"
    }
}

proc runtocursor_bp {} {
    if {[checkscilabbusy] == "OK"} {
        set textarea [gettextareacur]
        set infun [whichfun [$textarea index insert] $textarea]
        if {$infun!=""} {
            set cursorfunname [lindex $infun 0]
            set cursorfunline [lindex $infun 1]
            set rfn $cursorfunname
            set rfl $cursorfunline
            insertremove_bp
            tonextbreakpoint_bp
            set comm1 "\[db_l,db_m\]=where();"
            set comm2 "if size(db_l,1)>=3 then"
            set comm3 "TCL_EvalStr(\"scipad eval {set checklist \[ list \"+string(db_l(3))+\" \"+string(db_m(3))+\" $rfl $rfn \] }\");"
            set comm4 "else"
            set comm5 "TCL_EvalStr(\"scipad eval {set checklist \[ list 0 \"\"\"\" $rfl $rfn \] }\");"
            set comm6 "end;"
            set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6]
            ScilabEval "$fullcomm" "seq"
            ScilabEval "TCL_EvalStr(\"scipad eval {set bptcheckresult \[makelinecheck_bp \$checklist\] }\");" "seq"
            ScilabEval "flush"
            while {$bptcheckresult == "WrongBpt" && [getdbstate] == "DebugInProgress"} {
                tonextbreakpoint_bp
                ScilabEval "$fullcomm" "seq"
                ScilabEval "TCL_EvalStr(\"scipad eval {set bptcheckresult \[makelinecheck_bp \$checklist\] }\");" "seq"
                ScilabEval "flush"
            }
            if {[getdbstate] != "DebugInProgress"} {
                tk_messageBox -message [mc "Cursor position is out of reach!"] -icon info
            }
            insertremove_bp
        } else {
            # <TODO> .sce case
            showinfo [mc "Cursor must be in a function"]
        }
    }
}

proc makelinecheck_bp {cl} {
    set fl  [lindex $cl 0]
    set fn  [lindex $cl 1]
    set rfl [lindex $cl 2]
    set rfn [lindex $cl 3]
    if {$fn==$rfn && $fl== $rfl} {
#tk_messageBox -message "RightBpt\n$fl    $fn\n$rfl    $rfn"
        return "RightBpt"
    } else {
#tk_messageBox -message "WrongBpt\n$fl    $fn\n$rfl    $rfn"
        return "WrongBpt"
    }
}

proc tonextbreakpoint_bp {} {
    if {[getdbstate] == "ReadyForDebug"} {
        execfile_bp
    } elseif {[getdbstate] == "DebugInProgress"} {
        resume_bp
    } else {
        tk_messageBox -message "Unexpected debug state in proc tonextbreakpoint_bp: please report"
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
            if {$setbpcomm != ""} {
                setdbstate "DebugInProgress"
                set commnvars [createsetinscishellcomm]
                set watchsetcomm [lindex $commnvars 0]
                if {$watchsetcomm != ""} {
                    ScilabEval "$watchsetcomm"  "seq"
                }
# <TODO> A ScilabEval "seq" never causes an error, this only happens with "sync"
#        It would be a good idea to arrange for an error to be reported even with a seq,
#        but this is outside of Scipad.
                if {[catch {ScilabEval "$setbpcomm; $funnameargs; $removecomm"  "seq"}]} {
                    scilaberror $funnameargs
                }
                updateactivebreakpoint
                getfromshell
                checkendofdebug_bp
            } else {
                if {[catch {ScilabEval "$funnameargs" "seq"}]} {
                    scilaberror $funnameargs
                }
            }
        } else {
            # <TODO> .sce case
##            execfile
        }
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
# be executed. This should no be such a tricky thing to do, drawing inspiration
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
                ScilabEval "$watchsetcomm" "seq"
                set returnwithvars [lindex $commnvars 1]
                ScilabEval "$returnwithvars" "seq"
            } else {
                ScilabEval "resume(0)" "seq"
            }
            updateactivebreakpoint
            getfromshell
            checkendofdebug_bp
        } else {
            # <TODO> .sce case
            # Sending \n is if mode(6) mode(0) is used. If pause, there is no
            # need to distinguish between .sci and .sce (resume is sent for both)
 #           ScilabEval " " "seq"
        }
    }
}

proc goonwo_bp {} {
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            removeallactive_bp
            removescilab_bp "with_output"
            ScilabEval "resume(0)" "seq"
            getfromshell
        }
        setdbstate "ReadyForDebug"
    }
}

proc break_bp {} {
    if {[checkscilabbusy "nomessage"] != "OK"} {
        ScilabEval "flush"
        ScilabEval "pause" "seq"
        updateactivebreakpoint 4
        getfromshell 4
# <TODO> Remove next line and allow to continue debug
# Problem: Scilab does not stop at breakpoints located after the break command point!
        setdbstate "NoDebug"
    } else {
        showinfo [mc "No effect - The debugged file is not stuck"]
    }
}

proc canceldebug_bp {} {
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            removeallactive_bp
            ScilabEval "abort" "seq"
            removescilab_bp "with_output"
            getfromshell
        }
        setdbstate "NoDebug"
    }
}

proc scilaberror {funnameargs} {
    global errnum errline errmsg errfunc
    ScilabEval "\[db_str,db_n,db_l,db_func\]=lasterror();\
                TCL_EvalStr(\"scipad eval { global errnum errline errmsg errfunc; \
                                           set errnum  \"+string(db_n)+\"; \
                                           set errline \"+string(db_l)+\"; \
                                           set errfunc \"\"\"+strsubst(db_func,\"\"\"\",\"\\\"\"\")+\"\"\"; \
                                           set errmsg  \"\"\"+db_str+\"\"\"}\")" \
               "sync" "seq"
    tk_messageBox -title [mc "Scilab execution error"] \
      -message [concat [mc "The shell reported an error while trying to execute "]\
      $funnameargs ": error " $errnum ", " $errmsg [mc " at line "]\
      $errline [mc " of "] $errfunc]
    showinfo [mc "Execution aborted!"]
    if {[getdbstate] == "DebugInProgress"} {
        canceldebug_bp
    }
    blinkline $errline $errfunc
}

proc blinkline {li ma {nb 3}} {
    for {set i 0} {$i < $nb} {incr i} {
        updateactivebreakpointtag $li $ma
        update idletasks
        after 500
        updateactivebreakpointtag 0 ""
        update idletasks
        after 500
    }
}
