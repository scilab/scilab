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
                getfromshell
                updateactivebreakpoint
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
    # [..]=resume(..) was bugged (see bug 818)
    # The changes made by the user in the watch window were not reflected in the calling
    # workspace in Scilab. The correction is part of CVS from 18/06/04 or later, and
    # it has been included in Scilab 3.0
                ScilabEval "$returnwithvars" "seq"
            } else {
                ScilabEval "resume(0)" "seq"
            }
            getfromshell
            updateactivebreakpoint
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
            [gettextareacur] tag remove activebreakpoint 1.0 end
            removescilab_bp "with_output"
            ScilabEval "resume(0)" "seq"
            getfromshell
        }
        setdbstate "ReadyForDebug"
    }
}

proc canceldebug_bp {} {
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            [gettextareacur] tag remove activebreakpoint 1.0 end
            ScilabEval "abort" "seq"
            removescilab_bp "with_output"
# Next line should not be required but it is since the workspace is erased under some
# circumstances like after the ScilabEval "abort" above. See bug #633.
# <TODO> Once this bug is solved, getdebuggersciancillaries_bp on next line can be removed.
            getdebuggersciancillaries_bp
            getfromshell
        }
        setdbstate "NoDebug"
    }
}

proc scilaberror {funnameargs} {
    global errnum errline errmsg errfunc lang
    ScilabEval "\[db_str,db_n,db_l,db_func\]=lasterror();\
                TK_EvalStr(\"scipad eval { global errnum errline errmsg errfunc; \
                                           set errnum  \"+string(db_n)+\"; \
                                           set errline \"+string(db_l)+\"; \
                                           set errfunc \"\"\"+db_func+\"\"\"; \
                                           set errmsg \"\"\"+db_str+\"\"\"}\")" \
               "sync" "seq"
    if {$lang == "eng"} {
        tk_messageBox -title "Scilab execution error" \
          -message [concat "The shell reported an error while trying to execute "\
          $funnameargs ": error " $errnum ", " $errmsg " at line "\
          $errline " of function " $errfunc]
        showinfo "Execution aborted!"
    } else {
        tk_messageBox -title "Erreur Scilab durant l'exécution" \
          -message [concat "Le shell a rencontré une erreur en tentant d'exécuter "\
          $funnameargs ": erreur " $errnum ", " $errmsg " ligne "\
          $errline " de la fonction " $errfunc]
        showinfo "Exécution arrêtée!"
    }
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
