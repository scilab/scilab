proc tonextbreakpoint_bp {{checkbusyflag 1} {stepmode "nostep"}} {

    if {[getdbstate] == "ReadyForDebug"} {
        clearscilaberror
        execfile_bp $stepmode
    } elseif {[getdbstate] == "DebugInProgress"} {
        resume_bp $checkbusyflag $stepmode
    } else {
        tk_messageBox -message "Unexpected debug state in proc tonextbreakpoint_bp: please report"
    }
}

proc execfile_bp {{stepmode "nostep"}} {
    global funnameargs listoftextarea funsinbuffer waitmessage watchvars
    global setbptonreallybreakpointedlinescmd
    if {[isscilabbusy 5]} {return}
    showinfo $waitmessage

    # create the setbpt command
    set setbpcomm ""
    foreach textarea $listoftextarea {
        set tagranges [$textarea tag ranges breakpoint]
        foreach {tstart tstop} $tagranges {
            set infun [whichfun [$textarea index $tstart] $textarea]
            if {$infun !={} } {
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set setbpcomm [concat $setbpcomm "setbpt(\"$funname\",$lineinfun);"]
            } else {
                # <TODO> .sce case if some day the parser uses pseudocode noops
                # with the wrapper implementation, breakpoints are always
                # inside a function at the time of exec
            }
        }
    }

    # exec the required file(s)
    if {$funnameargs != ""} {
        execfile "current"
        # exec file containing the function to debug
        # <TODO> this fails if the same function name can be found in more
        #        than one single buffer
        # note : we can't exec *all* buffers because some might contain
        # non-Scilab scripts, which is not checked by execfile
        set funname [string range $funnameargs 0 [expr [string first "(" $funnameargs] - 1]]
        foreach textarea $listoftextarea {
            if {[info exists funsinbuffer($textarea)]} {
                if {[lsearch $funsinbuffer($textarea) $funname] != -1 && \
                     $textarea != [gettextareacur]} {
                    execfile $textarea
                }
            }
        }
        set setbptonreallybreakpointedlinescmd $setbpcomm
        setdbstate "DebugInProgress"
        set commnvars [createsetinscishellcomm $watchvars]
        set watchsetcomm [lindex $commnvars 0]
        if {$watchsetcomm != ""} {
            ScilabEval_lt "$watchsetcomm"  "seq"
        }
        ScilabEval_lt "$setbpcomm; $funnameargs;" "seq"
        updateactivebreakpoint
        getfromshell
        checkendofdebug_bp $stepmode
    } else {
        # <TODO> .sce case if some day the parser uses pseudocode noops
    }
}

# <TODO> step by step support
# I have no satisfactory solution for the time being.
# The heart of the matter with step by step execution is that
# once the execution is stopped there is no way of knowing what is the next
# line of code to execute. Of course, it is usually the next code line in the
# sci file, but this is not necessarily true in for, while, if, and case
# structures. I do not foresee any other remedy than a complete code analysis
# performed in Tcl (!), but this is a huge task I'm not prepared to go into.
# Moreover, all this analysis is already (and surely better) done by the
# Scilab interpreter, therefore the best way would probably be to add a new
# Scilab function that would return the line number of the next instruction to
# be executed. This should no be such a tricky thing to do, drawing inspiration
# e.g. from setbpt, where, whatln and so on. However, despite my repeated
# demands for information about the internals to the Scilab team, I never could
# obtain any documentation nor help on this topic. Hence an elegant solution
# for step execution is still to be achieved. Better than nothing, I
# implemented a brute force approach that basically sets a breakpoint
# everywhere. This works but is obviously sub-optimal. Moreover, this
# implementation does not allow to step into non opened files that the debugger
# could open, e.g. library files.

proc stepbystepinto_bp {{checkbusyflag 1}} {
# perform "step into" debug
    stepbystep_bp $checkbusyflag "into"
}

proc stepbystepover_bp {{checkbusyflag 1}} {
# perform "step over" debug
    stepbystep_bp $checkbusyflag "over"
}

proc stepbystepout_bp {{checkbusyflag 1}} {
# perform "step out" debug
    stepbystep_bp $checkbusyflag "out"
}

proc stepbystep_bp {checkbusyflag stepmode} {
# set a breakpoint in Scilab on really every line of every function
# of every opened buffer consistently with the current step mode,
# run execution, delete all those breakpoints and restore the
# breakpoints that were really set by the user
    global funnameargs

    if {[getdbstate] == "ReadyForDebug"} {
        # always a busy check - this code part cannot be entered
        # while skipping lines without executable statements
        # (which might occur during step by step)
        if {[isscilabbusy 5]} {return}

        clearscilaberror

#        showwrappercode

        if {$funnameargs != ""} {
            set funname [string range $funnameargs 0 [expr [string first "(" $funnameargs] - 1]]
            ScilabEval_lt "setbpt(\"$funname\",1);" "seq"
        } else {
            # <TODO> .sce case if some day the parser uses pseudocode noops
        }
        # here tricky (but correct) behaviour!! (see below for same comment)
        execfile_bp $stepmode
        if {$funnameargs != ""} {
            ScilabEval_lt "delbpt(\"$funname\",1);" "seq"
        } else {
            # <TODO> .sce case if some day the parser uses pseudocode noops
        }

        # hidewrappercode is performed in checkendofdebug_bp

    } elseif {[getdbstate] == "DebugInProgress"} {
        # no busy check to allow to skip lines without code (step by step)
        if {$checkbusyflag} {
            if {[isscilabbusy 5]} {return}
        }
        if {$funnameargs != ""} {

#            showwrappercode

            # because the user can open or close files during debug,
            # getlogicallinenumbersranges must be called at each step
            switch -- $stepmode {
                "into"  {set stepscope "allscilabbuffers"}
                "over"  {set stepscope "currentcontext"}
                "out"   {set stepscope "callingcontext"}
                default {set stepscope "allscilabbuffers" ;# should never happen}
            }
            set cmd [getlogicallinenumbersranges $stepscope]
            # check Scilab limits in terms of breakpoints
            if {$cmd == "-1"} {
                # abort step-by-step command - do nothing
            } elseif {$cmd == "0"} {
                # execute "Go to next breakpoint" instead
                tonextbreakpoint_bp
            } else {
                # no limit exceeded - go on one step
                regsub -all -- {\(} $cmd "setbpt(" cmdset
                regsub -all -- {\(} $cmd "delbpt(" cmddel
                ScilabEval_lt "$cmdset" "seq"
                # here tricky (but correct) behaviour!!
                # resume_bp calls checkendofdebug_bp that constructs a string
                # containing TCL_EvalStr("Scilab_Eval_lt ... seq"","scipad")
                # this string is itself evaluated by a ScilabEval_lt seq, so the order
                # in queue is first Tcl_EvalStr("Scilab_Eval_lt ... seq"","scipad")
                # queued by checkendofdebug_bp, and second $cmddel queued from here
                # Then Scilab executes the statements one by one: the first item
                # results in queueing at the end what is in the ScilabEval_lt (the ...
                # above) and that's all. Then $cmddel gets executed, and finally
                # the ... get executed
                # Order of execution is therefore $cmddel and, after it only, the
                # contents of proc checkendofdebug_bp
                resume_bp $checkbusyflag $stepmode
                ScilabEval_lt "$cmddel" "seq"
            }

            # hidewrappercode is performed in checkendofdebug_bp

        } else {
            # <TODO> .sce case if some day the parser uses pseudocode noops
        }

    } else {
        tk_messageBox -message "Unexpected debug state in proc stepbystep_bp: please report"
    }
}

proc getlogicallinenumbersranges {stepscope} {
# get all logical line numbers ranges of all functions from the given
# step by step scope $stepscope, which can be
#   - allscilabbuffers : functions from all scilab scheme buffers
#   - configuredfoo    : only the function that was selected in the
#                        configure box
#   - currentcontext   : functions listed in the call stack at the
#                        current stop point
#   - callingcontext   : functions listed in the call stack at the
#                        current stop point, but the first one
# return value is normally a single string:
#   ("$fun1",[1,max1]);("$fun2",[1,max2]);...;("$funN",[1,maxN]);
# this format is especially useful when this string is used to set or
# delete breakpoints in all the lines - just use a regsub to replace
# the opening parenthesis by setbpt( or delbpt(
# in case any Scilab limit is exceeded, return value is a string containing
# a return code:
#   "0"  : the calling procedure should apply "Go to next breakpoint" instead
#          of the intended step-by-step command
#   "-1" : the calling procedure should cancel the step-by-step command

    global ScilabCodeMaxBreakpointedMacros ScilabCodeMaxBreakpoints
    global debugassce

    set cmd ""
    set nbmacros 0 ; # used to test max number of breakpointed macros
    set nbbreakp 0 ; # used to test max number of breakpoints

    foreach {ta funsinthatta} [getallfunsinalltextareas] {
        if {[lindex $funsinthatta 0] == "0NoFunInBuf"} {
            continue
        }
        incr nbmacros
        foreach {funname funline precfun} $funsinthatta {

            if {![isinstepscope $funname $stepscope]} {
                continue
            }

            set curpos [getendfunctionpos $ta $precfun]

            # $curpos now contains the index in $ta of the first n of the word
            # endfunction corresponding to $funname
            set nbcontlines [countcontlines $ta $precfun $curpos]
            scan $precfun "%d." startoffun 
            scan $curpos  "%d." endoffun 
            set lastlogicalline [expr $endoffun - $startoffun - $nbcontlines +1]

            # if the debug occurs on a .sce file wrapped in a function, the
            # last four logical line numbers contain the code added to return
            # local variables to the calling level and should not be
            # breakpointed since they constitute hidden code
            if {$debugassce} {
                incr lastlogicalline -4
            }

            append cmd "(\"$funname\",\[1:" $lastlogicalline "\]);"

            incr nbbreakp $lastlogicalline
        }
    }

    # From help setbpt: The maximum number of functions with breakpoints
    #                   enabled must be less than 100 and the maximum number
    #                   of breakpoints is set to 1000
    # Check it and ask what to do if any limit is exceeded
    if {$nbmacros >= $ScilabCodeMaxBreakpointedMacros} {
        set mes [concat [mc "You have currently"] $nbmacros [mc "functions in your opened files."] \
                        [mc "Scilab supports a maximum of"] $ScilabCodeMaxBreakpointedMacros \
                        [mc "possible breakpointed functions (see help setbpt)."] \
                        [mc "Step-by-step hence cannot be performed."] \
                        [mc "This command will actually run to the next breakpoint."] ]
        set answer [tk_messageBox -message $mes -title [mc "Too many breakpointed functions"] \
                        -icon warning -type okcancel]
        switch -- $answer {
            ok     {set cmd "0"}
            cancel {set cmd "-1"}
        }
    }
    if {$nbbreakp >= $ScilabCodeMaxBreakpoints} {
        set mes [concat [mc "Executing this command would require to set"] $nbbreakp \
                        [mc "breakpoints in your opened files."] \
                        [mc "Scilab supports a maximum of"] $ScilabCodeMaxBreakpoints \
                        [mc "possible breakpoints in Scilab (see help setbpt)."] \
                        [mc "Step-by-step hence cannot be performed."] \
                        [mc "This command will actually run to the next breakpoint."] ]
        set answer [tk_messageBox -message $mes -title [mc "Too many breakpoints"] \
                        -icon warning -type okcancel]
        switch -- $answer {
            ok     {set cmd "0"}
            cancel {set cmd "-1"}
        }
    }

    return $cmd
}

proc isinstepscope {funname stepscope} {
# return true if function name $funname is in step scope $stepscope
# see proc getlogicallinenumbersranges for the available scopes
    global funnameargs
    global callstackfuns
    if {$stepscope == "allscilabbuffers"} {
        return true
    } elseif {$stepscope == "configuredfoo"} {
        set oppar [expr [string first "\(" $funnameargs] - 1]
        set configuredfunname [string range $funnameargs 0 $oppar]
        if {$funname == $configuredfunname} {
            return true
        } else {
            return false
        }
    } elseif {$stepscope == "currentcontext"} {
        # note: variable callstackfuns is set by Scilab script FormatWhereForDebugWatch
        if {[lsearch -exact $callstackfuns $funname] != -1} {
            return true
        } else {
            return false
        }
    } elseif {$stepscope == "callingcontext"} {
        # note: variable callstackfuns is set by Scilab script FormatWhereForDebugWatch
        if {[llength $callstackfuns] > 1} {
            set callcont [lreplace $callstackfuns 0 0]
            if {[lsearch -exact $callcont $funname] != -1} {
                return true
            } else {
                return false
            }
        } else {
            return false
        }
    } else {
        tk_messageBox -message "Unexpected step scope ($stepscope) in proc isinstepscope. Please report."
        return false
    }
}

proc runtocursor_bp {{checkbusyflag 1} {skipbptmode 0}} {
    global cursorfunname cursorfunline

    # no busy check to allow to skip stops at the wrong breakpoint
    if {$checkbusyflag} {
        if {[isscilabbusy 5]} {return}
    }

    set textarea [gettextareacur]

    # if the cursor is in the wrapper code (.sce files case), move it out
    if {[lsearch [$textarea tag names insert] "db_wrapper"] != -1} {
        set wrapstart [$textarea tag prevrange "db_wrapper" insert]
        set wrapstart [lindex $wrapstart 0]
        $textarea mark set insert "$wrapstart - 1 c"
    }

    set infun [whichfun [$textarea index insert] $textarea]
    if {$infun!=""} {
        if {!$skipbptmode} {
            set cursorfunname [lindex $infun 0]
            # substract 1 since we want to stop before this line and not after
            set cursorfunline [expr [lindex $infun 1] - 1]
            if {$cursorfunline == 0} {
                # cursor is on the function definition line
                set cursorfunline 1
            }
        }
        set setbpcomm "setbpt(\"$cursorfunname\",$cursorfunline);"
        ScilabEval_lt $setbpcomm "seq"
        tonextbreakpoint_bp $checkbusyflag "runtocur"
        set delbpcomm "delbpt(\"$cursorfunname\",$cursorfunline);"
        ScilabEval_lt $delbpcomm "seq"
    } else {
        showinfo [mc "Cursor must be in a function"]
    }
}

proc iscursorplace_bp {} {
# return true if the current stop occurs at line $cursorfunline of
# function $cursorfunname
# return false otherwise
    global cursorfunname cursorfunline
    global callstackfuns callstacklines
    if {[lindex $callstackfuns 0] == $cursorfunname} {
        if {[expr [lindex $callstacklines 0] -1] == $cursorfunline} {
            return true
        }
    }
    return false
}

proc resume_bp {{checkbusyflag 1} {stepmode "nostep"}} {
    global funnameargs waitmessage watchvars

    # no busy check to allow to skip lines without code (step by step)
    if {$checkbusyflag} {
        if {[isscilabbusy 5]} {return}
    }

    showinfo $waitmessage
    if {$funnameargs != ""} {
        set commnvars [createsetinscishellcomm $watchvars]
        set watchsetcomm [lindex $commnvars 0]
        if {$watchsetcomm != ""} {
            ScilabEval_lt "$watchsetcomm" "seq"
            set returnwithvars [lindex $commnvars 1]
            ScilabEval_lt "$returnwithvars" "seq"
        } else {
            ScilabEval_lt "resume(0)" "seq"
        }
        updateactivebreakpoint
        getfromshell
        checkendofdebug_bp $stepmode
    } else {
        # <TODO> .sce case if some day the parser uses pseudocode noops
    }
}

proc goonwo_bp {} {
    global funnameargs waitmessage

    if {[isscilabbusy 5]} {return}

    showinfo $waitmessage
    if {$funnameargs != ""} {
        removeallactive_bp
        removescilab_bp "with_output"
        ScilabEval_lt "resume(0)" "seq"
        getfromshell
    }
    setdbstate "ReadyForDebug"
}

proc break_bp {} {
    if {[isscilabbusy]} {

# Many solutions were explored, none is fully functional.
# Apparently the problem boils down to sending a sync command
# while in a seq execution (the one from the very beginning
# of the debug, which executes the function to debug)
# See also bug 1086 for that sort of issues

# 1. send a seq pause preceded by flush
# two flushes since the first one may just queue new commands
# (see proc checkendofdebug_bp)
#        ScilabEval_lt "flush"
#        ScilabEval_lt "flush"
#        ScilabEval_lt "pause" "seq"
#        updateactivebreakpoint 4
#        getfromshell 4

# 2. send a sync pause
# apparently same result with:
#       ScilabEval_lt "pause" "sync"
#       updateactivebreakpoint 3
#       getfromshell 3

# 3. launch a new core Fortran command that only says call sigbas(2)
#    i.e. set iflag to 1 (true), i.e. an interrupt has occurred
#    Note that this command already exists in C (void SignalCtrC)
#      ScilabEval "breaksgl" "sync" "seq"
#      updateactivebreakpoint 4
#      getfromshell 4

# 4. set breakpoints everywhere during run (details copied
#    from stepbystep_bp)
if {1} {
global funnameargs
global setbptonreallybreakpointedlinescmd
set checkbusyflag 0
        if {$funnameargs != ""} {
            # because the user can open or close files during debug,
            # getlogicallinenumbersranges must be called at each step
            set stepmode "into"
            set stepscope "allscilabbuffers"
            set cmd [getlogicallinenumbersranges $stepscope]
            # check Scilab limits in terms of breakpoints
            if {$cmd == "-1" || $cmd == "0"} {
                # impossible to set the required breakpoints
                tk_messageBox -message "Too many bpts or bpted funs!"
                return
            } else {
                # no limit exceeded - go on one 
                regsub -all -- {\(} $cmd "setbpt(" cmdset
                regsub -all -- {\(} $cmd "delbpt(" cmddel
                ScilabEval_lt "$cmdset" "sync" ;# "seq"
                updateactivebreakpoint 4
                ScilabEval_lt "$cmddel" "seq"
                getfromshell 4
            }
        } else {
            # <TODO> .sce case if some day the parser uses pseudocode noops
    #        resume_bp
        }
}

# <TODO> Remove next line and allow to continue debug
# Problem: Scilab does not stop at breakpoints located after the break command point!
#        setdbstate "NoDebug"
    } else {
        showinfo [mc "No effect - The debugged file is not stuck"]
    }
}

proc canceldebug_bp {} {
    global funnameargs waitmessage

    if {[isscilabbusy 5]} {return}

    if {[getdbstate] == "DebugInProgress"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            removeallactive_bp
            ScilabEval_lt "abort" "seq"
            removescilab_bp "with_output"
            getfromshell
            cleantmpScilabEvalfile
        }
    } else {
        # [getdbstate] is "ReadyForDebug" - nothing to do
    }

    scedebugcleanup_bp 

    # dbstate must be set explicitely to NoDebug here, because
    # scedebugcleanup_bp does nothing for .sci files
    setdbstate "NoDebug"

}
